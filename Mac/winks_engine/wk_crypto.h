
#ifdef __cplusplus
extern "C" { 
#endif

/* base64编码函数
* mode: 0 - 标准base64，1 - base64url
*/
extern int Winks_Base64Encode(unsigned int mode, const unsigned char *src, unsigned int src_len, unsigned char *dest, unsigned int dest_len);

/* base64解码函数
* mode: 0 - 标准base64，1 - base64url
*/
extern int Winks_Base64Decode(unsigned int mode, const unsigned char *src, unsigned char *dest, unsigned int dest_len);

/* AES加密函数 - 终端只需完成加密算法，解密在服务器端完成。
 * function: Winks_Encrypt

 * parameters:
 *   const unsigned char *src: 明文
 *   unsigned int src_len: 明文长度
 *   unsigned char *dest: 输出buf
 *   unsigned int dest_len: 输出buf 长度

 * return:
 *   成功返回: 0
 *   失败返回: -1
 */
extern int Winks_Encrypt(const unsigned char *src, unsigned int src_len, unsigned char *dest, unsigned int dest_len);

/* AES 加密并对加密后的数据做base64url 编码 */
extern int Winks_EncryptAndBase64(const unsigned char *src, unsigned int src_len, unsigned char *dest, unsigned int dest_len);

#ifdef __cplusplus
}
#endif

