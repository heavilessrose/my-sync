/*  
*   Byte-oriented AES-256 implementation.
*   All lookup tables replaced with 'on the fly' calculations. 
*
*   Copyright (c) 2007 Ilya O. Levin, http://www.literatecode.com
*
*   Permission to use, copy, modify, and distribute this software for any
*   purpose with or without fee is hereby granted, provided that the above
*   copyright notice and this permission notice appear in all copies.
*
*   THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
*   WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
*   MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
*   ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
*   WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
*   ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
*   OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/

//#include <stddef.h>
//#include <string.h>
//#include <assert.h>

#include "wk_public.h"
#include "wk_crypto.h"

#ifndef uint8_t
#define uint8_t  unsigned char
#endif

#define FE(x)   (((x)<<1) ^ ((((x)>>7) & 1) * 0x1b))
#define FD(x)  (((x) >> 1) ^ (((x) & 1) ? 0x8d : 0))

typedef struct {
	uint8_t key[32]; 
	uint8_t enckey[32]; 
	uint8_t deckey[32];
} aes256_context; 

static void aes256_init(aes256_context *, const uint8_t * /* key */);
static void aes256_done(aes256_context *);
static void aes256_encrypt_ecb(aes256_context *, uint8_t * /* plaintext */);
static void aes256_decrypt_ecb(aes256_context *, uint8_t * /* cipertext */);

static void AES_encrypt(const uint8_t * in, uint8_t * out, const aes256_context * ctx);
static void AES_decrypt(const uint8_t * in, uint8_t * out, const aes256_context * ctx);

static void AES_cbc_encrypt(const uint8_t *in, uint8_t *out, size_t length, const uint8_t *key, uint8_t *ivec);
static void AES_cbc_decrypt(const uint8_t *in, uint8_t *out, size_t length, const uint8_t *key, uint8_t *ivec);

typedef void (*block128_f)(const unsigned char in[16],
			unsigned char out[16],
			const void *ctx);

static void cbc128_encrypt(const unsigned char *in, unsigned char *out,
			size_t len, const void *ctx,
			unsigned char ivec[16], block128_f block);
static void cbc128_decrypt(const unsigned char *in, unsigned char *out,
			size_t len, const void *ctx,
			unsigned char ivec[16], block128_f block);

/* -------------------------------------------------------------------------- */
static uint8_t gf_alog(uint8_t x) // calculate anti-logarithm gen 3
{
    uint8_t atb = 1, z;

    while (x--) {z = atb; atb <<= 1; if (z & 0x80) atb^= 0x1b; atb ^= z;}

    return atb;
} /* gf_alog */

/* -------------------------------------------------------------------------- */
static uint8_t gf_log(uint8_t x) // calculate logarithm gen 3
{
    uint8_t atb = 1, i = 0, z;

    do {
        if (atb == x) break;
        z = atb; atb <<= 1; if (z & 0x80) atb^= 0x1b; atb ^= z;
    } while (++i > 0);

    return i;
} /* gf_log */


/* -------------------------------------------------------------------------- */
static uint8_t gf_mulinv(uint8_t x) // calculate multiplicative inverse
{
    return (x) ? gf_alog(255 - gf_log(x)) : 0;
} /* gf_mulinv */

/* -------------------------------------------------------------------------- */
static uint8_t rj_sbox(uint8_t x)
{
    uint8_t y, sb;

    sb = y = gf_mulinv(x);
    y = (y<<1)|(y>>7); sb ^= y;  y = (y<<1)|(y>>7); sb ^= y; 
    y = (y<<1)|(y>>7); sb ^= y;  y = (y<<1)|(y>>7); sb ^= y;

    return (sb ^ 0x63);
} /* rj_sbox */

/* -------------------------------------------------------------------------- */
static uint8_t rj_xtime(uint8_t x) 
{
    return (x & 0x80) ? ((x << 1) ^ 0x1b) : (x << 1);
} /* rj_xtime */


/* -------------------------------------------------------------------------- */
static void aes_subBytes(uint8_t *buf)
{
    register uint8_t i = 16;
    while (i--) buf[i] = rj_sbox(buf[i]);
} /* aes_subBytes */

/* -------------------------------------------------------------------------- */
static void aes_subBytes_inv(uint8_t *buf)
{
    register uint8_t i = 16, j;
    while (i--) 
    {
        j = 0; do { if (rj_sbox(j) == buf[i]) {buf[i] = j; break;} } while (++j);
    }
} /* aes_subBytes_inv */

/* -------------------------------------------------------------------------- */
static void aes_addRoundKey(uint8_t *buf, uint8_t *key)
{
    register uint8_t i = 16;
    while (i--) buf[i] ^= key[i];
} /* aes_addRoundKey */

/* -------------------------------------------------------------------------- */
static void aes_addRoundKey_cpy(uint8_t *buf, const uint8_t *key, uint8_t *cpk)
{
    register uint8_t i = 16;
    while (i--)  buf[i] ^= (cpk[i] = key[i]), cpk[16+i] = key[16 + i];
} /* aes_addRoundKey_cpy */


/* -------------------------------------------------------------------------- */
static void aes_shiftRows(uint8_t *buf)
{
    register uint8_t i, j; /* to make it potentially parallelable :) */

    i = buf[1]; buf[1] = buf[5]; buf[5] = buf[9]; buf[9] = buf[13]; buf[13] = i;
    i = buf[10]; buf[10] = buf[2]; buf[2] = i;
    j = buf[3]; buf[3] = buf[15]; buf[15] = buf[11]; buf[11] = buf[7]; buf[7] = j;
    j = buf[14]; buf[14] = buf[6]; buf[6]  = j;

} /* aes_shiftRows */

/* -------------------------------------------------------------------------- */
static void aes_shiftRows_inv(uint8_t *buf)
{
    register uint8_t i, j; /* same as above :) */

    i = buf[1]; buf[1] = buf[13]; buf[13] = buf[9]; buf[9] = buf[5]; buf[5] = i;
    i = buf[2]; buf[2] = buf[10]; buf[10] = i;
    j = buf[3]; buf[3] = buf[7]; buf[7] = buf[11]; buf[11] = buf[15]; buf[15] = j;
    j = buf[6]; buf[6] = buf[14]; buf[14] = j;

} /* aes_shiftRows_inv */

/* -------------------------------------------------------------------------- */
static void aes_mixColumns(uint8_t *buf)
{
    register uint8_t i, a, b, c, d, e;

    for (i = 0; i < 16; i += 4)
    {
        a = buf[i]; b = buf[i + 1]; c = buf[i + 2]; d = buf[i + 3];
        e = a ^ b ^ c ^ d;
        buf[i] ^= e ^ rj_xtime(a^b);   buf[i+1] ^= e ^ rj_xtime(b^c);
        buf[i+2] ^= e ^ rj_xtime(c^d); buf[i+3] ^= e ^ rj_xtime(d^a);
    }
} /* aes_mixColumns */

/* -------------------------------------------------------------------------- */
static void aes_mixColumns_inv(uint8_t *buf)
{
    register uint8_t i, a, b, c, d, e, x, y, z;

    for (i = 0; i < 16; i += 4)
    {
        a = buf[i]; b = buf[i + 1]; c = buf[i + 2]; d = buf[i + 3];
        e = a ^ b ^ c ^ d;
        z = rj_xtime(e);
        x = e ^ rj_xtime(rj_xtime(z^a^c));  y = e ^ rj_xtime(rj_xtime(z^b^d));
        buf[i] ^= x ^ rj_xtime(a^b);   buf[i+1] ^= y ^ rj_xtime(b^c);
        buf[i+2] ^= x ^ rj_xtime(c^d); buf[i+3] ^= y ^ rj_xtime(d^a);
    }
} /* aes_mixColumns_inv */

/* -------------------------------------------------------------------------- */
static void aes_expandEncKey(uint8_t *k, uint8_t *rc) 
{
    register uint8_t i;

    k[0] ^= rj_sbox(k[29]) ^ (*rc);
    k[1] ^= rj_sbox(k[30]);
    k[2] ^= rj_sbox(k[31]);
    k[3] ^= rj_sbox(k[28]);
    *rc = FE( *rc);

    for(i = 4; i < 16; i += 4)  k[i] ^= k[i-4],   k[i+1] ^= k[i-3],
                                k[i+2] ^= k[i-2], k[i+3] ^= k[i-1];
    k[16] ^= rj_sbox(k[12]);
    k[17] ^= rj_sbox(k[13]);
    k[18] ^= rj_sbox(k[14]);
    k[19] ^= rj_sbox(k[15]);

    for(i = 20; i < 32; i += 4) k[i] ^= k[i-4],   k[i+1] ^= k[i-3],
                                k[i+2] ^= k[i-2], k[i+3] ^= k[i-1];

} /* aes_expandEncKey */

/* -------------------------------------------------------------------------- */
static void aes_expandDecKey(uint8_t *k, uint8_t *rc) 
{
    uint8_t i;

    for(i = 28; i > 16; i -= 4) k[i+0] ^= k[i-4], k[i+1] ^= k[i-3], 
                                k[i+2] ^= k[i-2], k[i+3] ^= k[i-1];

    k[16] ^= rj_sbox(k[12]);
    k[17] ^= rj_sbox(k[13]);
    k[18] ^= rj_sbox(k[14]);
    k[19] ^= rj_sbox(k[15]);

    for(i = 12; i > 0; i -= 4)  k[i+0] ^= k[i-4], k[i+1] ^= k[i-3],
                                k[i+2] ^= k[i-2], k[i+3] ^= k[i-1];

    *rc = FD(*rc);
    k[0] ^= rj_sbox(k[29]) ^ (*rc);
    k[1] ^= rj_sbox(k[30]);
    k[2] ^= rj_sbox(k[31]);
    k[3] ^= rj_sbox(k[28]);
} /* aes_expandDecKey */


/* -------------------------------------------------------------------------- */
static void aes256_init(aes256_context *ctx, const uint8_t *k)
{
    uint8_t rcon = 1;
    register uint8_t i;
    for (i = 0; i < sizeof(ctx->key); i++) ctx->enckey[i] = ctx->deckey[i] = k[i];
    for (i = 8;--i;) aes_expandEncKey(ctx->deckey, &rcon);
} /* aes256_init */

/* -------------------------------------------------------------------------- */
static void aes256_done(aes256_context *ctx)
{
    register uint8_t i;
    for (i = 0; i < sizeof(ctx->key); i++) 
        ctx->key[i] = ctx->enckey[i] = ctx->deckey[i] = 0;
} /* aes256_done */

/* -------------------------------------------------------------------------- */
static void aes256_encrypt_ecb(aes256_context *ctx, uint8_t *buf)
{
    uint8_t i, rcon;

    aes_addRoundKey_cpy(buf, ctx->enckey, ctx->key);
    for(i = 1, rcon = 1; i < 14; ++i)
    {
        aes_subBytes(buf);
        aes_shiftRows(buf);
        aes_mixColumns(buf);
        if( i & 1 ) aes_addRoundKey( buf, &ctx->key[16]);
        else aes_expandEncKey(ctx->key, &rcon), aes_addRoundKey(buf, ctx->key);
    }
    aes_subBytes(buf);
    aes_shiftRows(buf);
    aes_expandEncKey(ctx->key, &rcon); 
    aes_addRoundKey(buf, ctx->key);
} /* aes256_encrypt */

/* -------------------------------------------------------------------------- */
static void aes256_decrypt_ecb(aes256_context *ctx, uint8_t *buf)
{
    uint8_t i, rcon;

    aes_addRoundKey_cpy(buf, ctx->deckey, ctx->key);
    aes_shiftRows_inv(buf);
    aes_subBytes_inv(buf);

    for (i = 14, rcon = 0x80; --i;)
    {
        if( ( i & 1 ) )           
        {
            aes_expandDecKey(ctx->key, &rcon);
            aes_addRoundKey(buf, &ctx->key[16]);
        }
        else aes_addRoundKey(buf, ctx->key);
        aes_mixColumns_inv(buf);
        aes_shiftRows_inv(buf);
        aes_subBytes_inv(buf);
    }
    aes_addRoundKey( buf, ctx->key); 
}  /* aes256_decrypt */

/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
static void cbc128_encrypt(const unsigned char *in, unsigned char *out,
			size_t len, const void *key,
			unsigned char ivec[16], block128_f block)
{
	size_t n;
	const unsigned char *iv = ivec;

	//assert(in && out && key && ivec);
	Winks_assert(in && out && key && ivec);

	while (len) {
		for(n=0; n<16 && n<len; ++n)
			out[n] = in[n] ^ iv[n];
		for(; n<16; ++n)
			out[n] = iv[n];
		(*block)(out, out, key);
		iv = out;
		if (len<=16) break;
		len -= 16;
		in  += 16;
		out += 16;
	}
	memcpy(ivec,iv,16);
}

static void cbc128_decrypt(const unsigned char *in, unsigned char *out,
			size_t len, const void *key,
			unsigned char ivec[16], block128_f block)
{
	size_t n;
	union { size_t align; unsigned char c[16]; } tmp;

	//assert(in && out && key && ivec);
	Winks_assert(in && out && key && ivec);

	while (len) {
		unsigned char c;
		(*block)(in, tmp.c, key);
		for(n=0; n<16 && n<len; ++n) {
			c = in[n];
			out[n] = tmp.c[n] ^ ivec[n];
			ivec[n] = c;
		}
		if (len<=16) {
			for (; n<16; ++n)
				ivec[n] = in[n];
			break;
		}
		len -= 16;
		in  += 16;
		out += 16;
	}
}

/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
static void AES_encrypt(const uint8_t * in, uint8_t * out, const aes256_context * ctx)
{
	if (in != out)
		memcpy(out, in, 16);
	aes256_encrypt_ecb((aes256_context *)ctx, out);
}

static void AES_decrypt(const uint8_t * in, uint8_t * out, const aes256_context * ctx)
{
	if (in != out)
		memcpy(out, in, 16);
	aes256_decrypt_ecb((aes256_context *)ctx, out);
}

static void AES_cbc_encrypt(const uint8_t *in, uint8_t *out, size_t len, const uint8_t *key, uint8_t *ivec)
{
	aes256_context	ctx;
	aes256_init(&ctx, key);

	cbc128_encrypt(in,out,len,&ctx,ivec,(block128_f)AES_encrypt);

	aes256_done(&ctx);
}

static void AES_cbc_decrypt(const uint8_t *in, uint8_t *out, size_t len, const uint8_t *key, uint8_t *ivec)
{
	aes256_context	ctx;
	aes256_init(&ctx, key);

	cbc128_decrypt(in,out,len,&ctx,ivec,(block128_f)AES_decrypt);

	aes256_done(&ctx);
}

/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */

/* base64编码函数
* mode: 0 - 标准base64，1 - base64url
*/
extern int Winks_Base64Encode(unsigned int mode, const unsigned char *src, unsigned int src_len, unsigned char *dest, unsigned int dest_len)
{
	/* 标准base64 */
	const unsigned char base64std_encode[] =
		"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	
	/* base64url */
	const unsigned char base64url_encode[] =
		"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_";

	unsigned int i;
	unsigned char *p = NULL;
	unsigned char *base64tab = NULL;

	Winks_assert(mode <= 1);
	Winks_assert(src && src_len && dest && dest_len);
	Winks_assert( (src_len /3 + 1) * 4 <  dest_len);

	switch (mode)
	{
	case 0:
		base64tab = (unsigned char *)base64std_encode;
		break;
	case 1:
		base64tab = (unsigned char *)base64url_encode;
		break;
	default:
		Winks_fail("");
		return -1;
	}

	i = 0;
	p = dest;

	if (src_len >= 3)
	{
		for (i = 0; i < src_len - 2; i += 3) {
			*p++ = base64tab[(src[i] >> 2) & 0x3F];
			*p++ = base64tab[((src[i] & 0x3) << 4) | ((int) (src[i + 1] & 0xF0) >> 4)];
			*p++ = base64tab[((src[i + 1] & 0xF) << 2) | ((int) (src[i + 2] & 0xC0) >> 6)];
			*p++ = base64tab[src[i + 2] & 0x3F];
		}
	}

	if (i < src_len) {
		*p++ = base64tab[(src[i] >> 2) & 0x3F];

		if (i == (src_len - 1)) {
			*p++ = base64tab[((src[i] & 0x3) << 4)];
			if (mode == 0) *p++ = '=';
		}
		else {
			*p++ = base64tab[((src[i] & 0x3) << 4) | ((int) (src[i + 1] & 0xF0) >> 4)];
			*p++ = base64tab[((src[i + 1] & 0xF) << 2)];
		}

		if (mode == 0) *p++ = '=';
	}

	*p++ = '\0';

	return p - dest;
}

/* base64解码函数
* mode: 0 - 标准base64，1 - base64url
*/
extern int Winks_Base64Decode(unsigned int mode, const unsigned char *src, unsigned char *dest, unsigned int dest_len)
{
	const unsigned char base64std_decode[128] =
	{
		64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
		64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
		64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 62, 64, 64, 64, 63,
		52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 64, 64, 64, 64, 64, 64,
		64,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14,
		15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 64, 64, 64, 64, 64,
		64, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
		41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 64, 64, 64, 64, 64
	};
	
	const unsigned char base64url_decode[128] =
	{
		64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
		64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
		64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 62, 64, 64,
		52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 64, 64, 64, 64, 64, 64,
		64,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14,
		15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 64, 64, 64, 64, 63,
		64, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
		41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 64, 64, 64, 64, 64
	};

	int nbytes;
	unsigned char *bufin;
	unsigned char *bufout;
	unsigned char *base64tab = NULL;

	switch (mode)
	{
	case 0:
		base64tab = (unsigned char *)base64std_decode;
		break;
	case 1:
		base64tab = (unsigned char *)base64url_decode;
		break;
	default:
		Winks_fail("");
		return -1;
	}

	bufin = (unsigned char *)src;
	while (base64tab[*(bufin++)] < 64) ;
	nbytes = (bufin - (unsigned char *)src) - 1;
	Winks_assert(nbytes % 4 != 1);

	bufin = (unsigned char *)src;
	bufout = (unsigned char *)dest;

	while (nbytes > 4) {
		*(bufout++) = (unsigned char) (base64tab[*bufin] << 2 | base64tab[bufin[1]] >> 4);
		*(bufout++) = (unsigned char) (base64tab[bufin[1]] << 4 | base64tab[bufin[2]] >> 2);
		*(bufout++) = (unsigned char) (base64tab[bufin[2]] << 6 | base64tab[bufin[3]]);
		bufin += 4;
		nbytes -= 4;
	}

	/* Note: (nbytes == 1) would be an error, so just ingore that case */
	if (nbytes > 1) {
		*(bufout++) = (unsigned char) (base64tab[*bufin] << 2 | base64tab[bufin[1]] >> 4);
	}
	if (nbytes > 2) {
		*(bufout++) = (unsigned char) (base64tab[bufin[1]] << 4 | base64tab[bufin[2]] >> 2);
	}
	if (nbytes > 3) {
		*(bufout++) = (unsigned char) (base64tab[bufin[2]] << 6 | base64tab[bufin[3]]);
	}

	return bufout - dest;
}

/* AES加密函数 - 终端只需完成加密算法，解密在服务器端完成。
NOTE: 目前只支持N * 16 字节大小的数据。

 * function: Winks_Encrypt

 * parameters:
 *   const unsigned char *src: 明文
 *   unsigned int src_len: 明文长度
 *   unsigned char *dest: 密文
 *   unsigned int dest_len: 输出buf 长度

 * return:
 *   成功返回: 0
 *   失败返回: -1
 */
extern int Winks_Encrypt(const unsigned char *src, unsigned int src_len, unsigned char *dest, unsigned int dest_len)
{
	unsigned char key[32] = {
		0x65, 0x1A, 0x40, 0xAD, 0xB3, 0xB4, 0x91, 0x88, 0xFB, 0xED, 0xEA, 0xD8, 0xC7, 0x9C, 0x14, 0x33,
		0xCE, 0xEF, 0xFD, 0x54, 0xD0, 0x09, 0x2A, 0x86, 0x75, 0x4A, 0xC0, 0x5D, 0x2A, 0xC9, 0xD5, 0x22
	};
	unsigned char ivec[16];
	unsigned char *buf = NULL;

	Winks_assert(src && dest);
	Winks_assert(src_len && dest_len && src_len <= dest_len);
	Winks_assert(src_len % 16 == 0);

	Winks_printf("Winks_Encrypt token:\n");
	Winks_printHex(src, (unsigned short)src_len, (unsigned char *)"");

	memset(ivec, 0, 16);
	AES_cbc_encrypt(src, dest, src_len, key, ivec);

	Winks_printf("Winks_Encrypt encrypted:\n");
	Winks_printHex(dest, (unsigned short)src_len, (unsigned char *)"");

#ifdef WINKS_DEBUG
	buf = Winks_salAlloc(src_len);
	Winks_assert(buf);

	memset(ivec, 0, 16);
	AES_cbc_decrypt(dest, buf, src_len, key, ivec);

	Winks_printf("Winks_Encrypt decrypted:\n");
	Winks_printHex(buf, (unsigned short)src_len, (unsigned char *)"");

	Winks_assert(memcmp(src, buf, src_len) == 0);

	Winks_salFree(buf);
	buf = NULL;
#endif /*WINKS_DEBUG*/

	return 0;
}

/*
* 加密并对加密后的数据做base64url 编码 
* NOTE: 目前只支持N * 16 字节大小的数据。
*/
extern int Winks_EncryptAndBase64(const unsigned char *src, unsigned int src_len, unsigned char *dest, unsigned int dest_len)
{
	unsigned char *buf = NULL;

	Winks_assert(src && dest);
	Winks_assert(src_len && dest_len && (src_len /3 + 1) * 4 <  dest_len);
	Winks_assert(src_len % 16 == 0);

	buf = Winks_salAlloc(src_len);
	Winks_assert(buf);

	Winks_Encrypt(src, src_len, buf, src_len);

	Winks_Base64Encode(1, buf, src_len, dest, dest_len);

	Winks_salFree(buf);
	buf = NULL;

	return 0;
}

#if 0
static const unsigned char s_send_data[] = {
	 0x50 ,0x4f ,0x53 ,0x54 ,0x20 ,0x2f ,0x20 ,0x48 ,0x54 ,0x54 ,0x50 ,0x2f ,0x31 ,0x2e ,0x31 ,0x0d
	,0x0a ,0x41 ,0x63 ,0x63 ,0x65 ,0x70 ,0x74 ,0x3a ,0x20 ,0x69 ,0x6d ,0x61 ,0x67 ,0x65 ,0x2f ,0x67
	,0x69 ,0x66 ,0x2c ,0x20 ,0x69 ,0x6d ,0x61 ,0x67 ,0x65 ,0x2f ,0x78 ,0x2d ,0x78 ,0x62 ,0x69 ,0x74
	,0x6d ,0x61 ,0x70 ,0x2c ,0x20 ,0x69 ,0x6d ,0x61 ,0x67 ,0x65 ,0x2f ,0x6a ,0x70 ,0x65 ,0x67 ,0x2c
	,0x20 ,0x69 ,0x6d ,0x61 ,0x67 ,0x65 ,0x2f ,0x70 ,0x6a ,0x70 ,0x65 ,0x67 ,0x2c ,0x20 ,0x61 ,0x70
	,0x70 ,0x6c ,0x69 ,0x63 ,0x61 ,0x74 ,0x69 ,0x6f ,0x6e ,0x2f ,0x78 ,0x2d ,0x73 ,0x68 ,0x6f ,0x63
	,0x6b ,0x77 ,0x61 ,0x76 ,0x65 ,0x2d ,0x66 ,0x6c ,0x61 ,0x73 ,0x68 ,0x2c ,0x20 ,0x2a ,0x2f ,0x2a
	,0x0d ,0x0a ,0x48 ,0x6f ,0x73 ,0x74 ,0x3a ,0x20 ,0x69 ,0x70 ,0x2e ,0x69 ,0x7a ,0x68 ,0x61 ,0x79
	,0x61 ,0x6e ,0x2e ,0x63 ,0x6f ,0x6d ,0x0d ,0x0a ,0x43 ,0x6f ,0x6e ,0x74 ,0x65 ,0x6e ,0x74 ,0x2d
	,0x4c ,0x65 ,0x6e ,0x67 ,0x74 ,0x68 ,0x3a ,0x20 ,0x31 ,0x34 ,0x36 ,0x0d ,0x0a ,0x43 ,0x6f ,0x6e
	,0x74 ,0x65 ,0x6e ,0x74 ,0x2d ,0x54 ,0x79 ,0x70 ,0x65 ,0x3a ,0x20 ,0x74 ,0x65 ,0x78 ,0x74 ,0x2f
	,0x2a ,0x0d ,0x0a ,0x55 ,0x73 ,0x65 ,0x72 ,0x2d ,0x41 ,0x67 ,0x65 ,0x6e ,0x74 ,0x3a ,0x20 ,0x55
	,0x41 ,0x2d ,0x77 ,0x69 ,0x6e ,0x6b ,0x73 ,0x32 ,0x2e ,0x30 ,0x0d ,0x0a ,0x0d ,0x0a ,0x00 ,0x00
	,0x00 ,0x92 ,0x00 ,0x01 ,0x00 ,0x00 ,0x00 ,0x01 ,0x01 ,0x00 ,0x02 ,0x00 ,0x00 ,0x00 ,0x01 ,0x01
	,0x00 ,0x03 ,0x00 ,0x00 ,0x00 ,0x02 ,0x7f ,0xed ,0x00 ,0x04 ,0x00 ,0x00 ,0x00 ,0x02 ,0x00 ,0x02
	,0x00 ,0x08 ,0x00 ,0x00 ,0x00 ,0x0b ,0x31 ,0x35 ,0x30 ,0x31 ,0x30 ,0x35 ,0x36 ,0x34 ,0x30 ,0x35
	,0x31 ,0x00 ,0x20 ,0x00 ,0x00 ,0x00 ,0x4f ,0x32 ,0x30 ,0x30 ,0x39 ,0x2d ,0x30 ,0x34 ,0x2d ,0x30
	,0x39 ,0x20 ,0x31 ,0x35 ,0x3a ,0x34 ,0x36 ,0x3a ,0x30 ,0x36 ,0x7c ,0x32 ,0x30 ,0x30 ,0x39 ,0x2d
	,0x30 ,0x34 ,0x2d ,0x30 ,0x39 ,0x20 ,0x31 ,0x35 ,0x3a ,0x34 ,0x36 ,0x3a ,0x30 ,0x36 ,0x7c ,0x32
	,0x30 ,0x30 ,0x39 ,0x2d ,0x30 ,0x34 ,0x2d ,0x30 ,0x39 ,0x20 ,0x31 ,0x35 ,0x3a ,0x34 ,0x36 ,0x3a
	,0x30 ,0x36 ,0x7c ,0x32 ,0x30 ,0x30 ,0x39 ,0x2d ,0x30 ,0x34 ,0x2d ,0x30 ,0x39 ,0x20 ,0x31 ,0x35
	,0x3a ,0x34 ,0x36 ,0x3a ,0x30 ,0x36 ,0x00 ,0x2b ,0x00 ,0x00 ,0x00 ,0x04 ,0x00 ,0x00 ,0x40 ,0x00
};

void DigitStr2PackedBcd(const char* /*in*/ src, size_t src_len, unsigned char* /*out*/ bcd)
{
	size_t i;
	int odds = src_len & 1;
	size_t bcd_len = src_len / 2;

	if (odds == 0)
		bcd[0] = (src[0] - 0x30);
	for (i = 1 - odds; i < bcd_len; i++)
		bcd[i] = ((src[i * 2 - 1 + odds] - 0x30) << 4) | (src[i * 2 + odds] - 0x30);
	bcd[bcd_len] = ((src[src_len - 1] - 0x30) << 4) | 0x0c;
}

int Winks_Base64Demo(void)
{
	/* 标准base64 */
	const unsigned char base64std_encode[] =
		"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";
	
	/* base64url */
	const unsigned char base64url_encode[] =
		"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_=";

	unsigned char temp[120];
	unsigned char test_buf1[120];
	unsigned char test_buf2[120];
	unsigned int test_len;
	unsigned int i;

	for (i = 1; i <= 64; i++)
	{
		/* 1 */
		strncpy(temp, base64std_encode, i);
		temp[i] = 0;

		Winks_Base64Encode(0, temp, i, test_buf1, 120);
		Winks_printf("base64std: %s\n", test_buf1);
		test_len = Winks_Base64Decode(0, (const unsigned char *)test_buf1, test_buf2, 120);
		Winks_printHex(test_buf2, (unsigned short)test_len, "test_buf");
		Winks_assert(memcmp(test_buf2, temp, i) == 0);

		/* 2 */
		strncpy(temp, base64url_encode, i);
		temp[i] = 0;
		
		Winks_Base64Encode(0, temp, i, test_buf1, 120);
		Winks_printf("base64std: %s\n", test_buf1);
		test_len = Winks_Base64Decode(0, (const unsigned char *)test_buf1, test_buf2, 120);
		Winks_printHex(test_buf2, (unsigned short)test_len, "test_buf");
		Winks_assert(memcmp(test_buf2, temp, i) == 0);
	}

	return 0;
}

int Winks_CryptoDemo1(void)
{
	aes256_context ctx;

	/* 密钥32 字节(256位) */
	//unsigned char key[32] = {
	//	0x00  ,0x01  ,0x02  ,0x03  ,0x04  ,0x05  ,0x06  ,0x07  ,0x08  ,0x09  ,0x0a  ,0x0b  ,0x0c  ,0x0d  ,0x0e  ,0x0f,
	//	0x00  ,0x01  ,0x02  ,0x03  ,0x04  ,0x05  ,0x06  ,0x07  ,0x08  ,0x09  ,0x0a  ,0x0b  ,0x0c  ,0x0d  ,0x0e  ,0x0f,
	//};
	unsigned char key[32] = {
		0x65, 0x1A, 0x40, 0xAD, 0xB3, 0xB4, 0x91, 0x88, 0xFB, 0xED, 0xEA, 0xD8, 0xC7, 0x9C, 0x14, 0x33,
		0xCE, 0xEF, 0xFD, 0x54, 0xD0, 0x09, 0x2A, 0x86, 0x75, 0x4A, 0xC0, 0x5D, 0x2A, 0xC9, 0xD5, 0x22
	};
	unsigned char text[16]= {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff};
	unsigned char out_text[16];

	unsigned char ivec[16];
	unsigned char text1[32]= {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xaa, 0xbb, 0xcc , 0xdd , 0xee, 0xff
						, 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xaa, 0xbb, 0xcc , 0xdd , 0xee, 0xff};
	unsigned char buf1[32];
	unsigned char buf2[32];

	unsigned char base64buf1[60];
	unsigned char base64buf2[60];
	unsigned char bcd[8];
	unsigned char original_token[32];

	int current_time;
	int current_time_be;
	int random_value;
	int i, j;

	memcpy(out_text, text, 16);

	/* 测试AES256_ecb 加密16字节(128位) 数据 */
	aes256_init(&ctx, key);
	aes256_encrypt_ecb(&ctx, out_text);
	aes256_done(&ctx);

	/* 测试AES256_ecb 解密16字节(128位) 数据 */
	aes256_init(&ctx, key);
	aes256_decrypt_ecb(&ctx, out_text);
	aes256_done(&ctx);

	Winks_printf("key:\n");
	Winks_printHex(key, 32, "");

	Winks_printf("text:\n");
	Winks_printHex(text1, 32, "");

	/* 测试AES256_cbc 加密32字节(256位) 数据 */
	memset(ivec, 0, 16);
	AES_cbc_encrypt(text1, buf1, 32, key, ivec);

	Winks_printf("encrypted:\n");
	Winks_printHex(buf1, 32, "");

	/* 测试AES256_cbc 解密32字节(256位) 数据 */
	memset(ivec, 0, 16);
	AES_cbc_decrypt(buf1, buf2, 32, key, ivec);

	Winks_printf("decrypted:\n");
	Winks_printHex(buf2, 32, "");

	//unsigned long long imsiLong = strtoull("460001234567890", NULL, 10);
	DigitStr2PackedBcd("460001234567890", 15, bcd);
	Winks_printf("packed bcd imsi:\n");
	Winks_printHex(bcd, 8, "");

	for (i = 0; i < 10; i++)
	{
		current_time = time(NULL);
		current_time_be = current_time;
		//current_time_be = htonl(current_time);

		srand(current_time);

		// token: current time
		memset(original_token, 0, 32);
		memcpy(&original_token[4], &current_time_be, 4);

		// token: random number
		for (j = 0; j < 4; j++)
		{
			random_value = rand();
			memcpy(&original_token[8+4*i], &random_value, 4);
		}

		// token: IMSI
		memcpy(&original_token[24], bcd, 8);

		//Winks_printf("key:\n");
		//Winks_printHex(key, 32, "");

		Winks_printf("original_token:\n");
		Winks_printHex(original_token, 32, "");
		
		memset(ivec, 0, 16);
		AES_cbc_encrypt(original_token, buf1, 32, key, ivec);

		Winks_printf("encrypted:\n");
		Winks_printHex(buf1, 32, "");

	#if 1 // 加入base64编解码
		Winks_Base64Encode(1, buf1, 32, base64buf1, 60);
		Winks_printf("base 64 encoded after encrypt:\n");
		Winks_printf("%s\n", base64buf1);

		Winks_Base64Decode(1, base64buf1, base64buf2, 60);
		Winks_printf("base 64 decoded before decrypt:\n");
		Winks_printHex(base64buf2, 32, "");

		Winks_assert(memcmp(buf1, base64buf2, 32) == 0);
	#endif

		memset(ivec, 0, 16);
		AES_cbc_decrypt(buf1, buf2, 32, key, ivec);

		Winks_printf("decrypted:\n");
		Winks_printHex(buf2, 32, "");

		Winks_assert(memcmp(original_token, buf2, 32) == 0);
	}

	return 0;
}

int Winks_CryptoDemo2(void)
{
	unsigned int i, size;
	unsigned char *buf = NULL;

	size = sizeof(s_send_data);
	buf = Winks_salAlloc(size * 3 / 2);
	Winks_assert(buf);

	for (i = 16; i < size; i+=16)
	{
		Winks_printf("Winks_CryptoDemo2 : %d\n", i);
		Winks_Encrypt(s_send_data, i, buf, size);
		Winks_EncryptAndBase64(s_send_data, i, buf, size);
	}

	Winks_salFree(buf);
	buf = NULL;

	return 0;
}
#endif

