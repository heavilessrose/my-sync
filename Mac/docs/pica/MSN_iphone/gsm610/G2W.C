// it converts from gsm 6.10 format to wav49 format

#include "Converter.h"

#include <stdio.h>

static unsigned int  	LARc[8], Nc[4], Mc[4], bc[4], xmaxc[4], xmc[13*4];

typedef unsigned char		gsm_byte;
typedef unsigned char		wav_byte;


void readGSM_33(gsm_byte *c){

	/* GSM_MAGIC  = (*c >> 4) & 0xF; */

//	if (((*c >> 4) & 0x0F) != GSM_MAGIC) return -1;

	LARc[0]  = (*c++ & 0xF) << 2;		/* 1 */
	LARc[0] |= (*c >> 6) & 0x3;
	LARc[1]  = *c++ & 0x3F;
	LARc[2]  = (*c >> 3) & 0x1F;
	LARc[3]  = (*c++ & 0x7) << 2;
	LARc[3] |= (*c >> 6) & 0x3;
	LARc[4]  = (*c >> 2) & 0xF;
	LARc[5]  = (*c++ & 0x3) << 2;
	LARc[5] |= (*c >> 6) & 0x3;
	LARc[6]  = (*c >> 3) & 0x7;
	LARc[7]  = *c++ & 0x7;
	Nc[0]  = (*c >> 1) & 0x7F;
	bc[0]  = (*c++ & 0x1) << 1;
	bc[0] |= (*c >> 7) & 0x1;
	Mc[0]  = (*c >> 5) & 0x3;
	xmaxc[0]  = (*c++ & 0x1F) << 1;
	xmaxc[0] |= (*c >> 7) & 0x1;
	xmc[0]  = (*c >> 4) & 0x7;
	xmc[1]  = (*c >> 1) & 0x7;
	xmc[2]  = (*c++ & 0x1) << 2;
	xmc[2] |= (*c >> 6) & 0x3;
	xmc[3]  = (*c >> 3) & 0x7;
	xmc[4]  = *c++ & 0x7;
	xmc[5]  = (*c >> 5) & 0x7;
	xmc[6]  = (*c >> 2) & 0x7;
	xmc[7]  = (*c++ & 0x3) << 1;		/* 10 */
	xmc[7] |= (*c >> 7) & 0x1;
	xmc[8]  = (*c >> 4) & 0x7;
	xmc[9]  = (*c >> 1) & 0x7;
	xmc[10]  = (*c++ & 0x1) << 2;
	xmc[10] |= (*c >> 6) & 0x3;
	xmc[11]  = (*c >> 3) & 0x7;
	xmc[12]  = *c++ & 0x7;
	Nc[1]  = (*c >> 1) & 0x7F;
	bc[1]  = (*c++ & 0x1) << 1;
	bc[1] |= (*c >> 7) & 0x1;
	Mc[1]  = (*c >> 5) & 0x3;
	xmaxc[1]  = (*c++ & 0x1F) << 1;
	xmaxc[1] |= (*c >> 7) & 0x1;
	xmc[13]  = (*c >> 4) & 0x7;
	xmc[14]  = (*c >> 1) & 0x7;
	xmc[15]  = (*c++ & 0x1) << 2;
	xmc[15] |= (*c >> 6) & 0x3;
	xmc[16]  = (*c >> 3) & 0x7;
	xmc[17]  = *c++ & 0x7;
	xmc[18]  = (*c >> 5) & 0x7;
	xmc[19]  = (*c >> 2) & 0x7;
	xmc[20]  = (*c++ & 0x3) << 1;
	xmc[20] |= (*c >> 7) & 0x1;
	xmc[21]  = (*c >> 4) & 0x7;
	xmc[22]  = (*c >> 1) & 0x7;
	xmc[23]  = (*c++ & 0x1) << 2;
	xmc[23] |= (*c >> 6) & 0x3;
	xmc[24]  = (*c >> 3) & 0x7;
	xmc[25]  = *c++ & 0x7;
	Nc[2]  = (*c >> 1) & 0x7F;
	bc[2]  = (*c++ & 0x1) << 1;		/* 20 */
	bc[2] |= (*c >> 7) & 0x1;
	Mc[2]  = (*c >> 5) & 0x3;
	xmaxc[2]  = (*c++ & 0x1F) << 1;
	xmaxc[2] |= (*c >> 7) & 0x1;
	xmc[26]  = (*c >> 4) & 0x7;
	xmc[27]  = (*c >> 1) & 0x7;
	xmc[28]  = (*c++ & 0x1) << 2;
	xmc[28] |= (*c >> 6) & 0x3;
	xmc[29]  = (*c >> 3) & 0x7;
	xmc[30]  = *c++ & 0x7;
	xmc[31]  = (*c >> 5) & 0x7;
	xmc[32]  = (*c >> 2) & 0x7;
	xmc[33]  = (*c++ & 0x3) << 1;
	xmc[33] |= (*c >> 7) & 0x1;
	xmc[34]  = (*c >> 4) & 0x7;
	xmc[35]  = (*c >> 1) & 0x7;
	xmc[36]  = (*c++ & 0x1) << 2;
	xmc[36] |= (*c >> 6) & 0x3;
	xmc[37]  = (*c >> 3) & 0x7;
	xmc[38]  = *c++ & 0x7;
	Nc[3]  = (*c >> 1) & 0x7F;
	bc[3]  = (*c++ & 0x1) << 1;
	bc[3] |= (*c >> 7) & 0x1;
	Mc[3]  = (*c >> 5) & 0x3;
	xmaxc[3]  = (*c++ & 0x1F) << 1;
	xmaxc[3] |= (*c >> 7) & 0x1;
	xmc[39]  = (*c >> 4) & 0x7;
	xmc[40]  = (*c >> 1) & 0x7;
	xmc[41]  = (*c++ & 0x1) << 2;
	xmc[41] |= (*c >> 6) & 0x3;
	xmc[42]  = (*c >> 3) & 0x7;
	xmc[43]  = *c++ & 0x7;			/* 30  */
	xmc[44]  = (*c >> 5) & 0x7;
	xmc[45]  = (*c >> 2) & 0x7;
	xmc[46]  = (*c++ & 0x3) << 1;
	xmc[46] |= (*c >> 7) & 0x1;
	xmc[47]  = (*c >> 4) & 0x7;
	xmc[48]  = (*c >> 1) & 0x7;
	xmc[49]  = (*c++ & 0x1) << 2;
	xmc[49] |= (*c >> 6) & 0x3;
	xmc[50]  = (*c >> 3) & 0x7;
	xmc[51]  = *c & 0x7;			/* 33 */

}


void conv66(gsm_byte * d, wav_byte * c){

	gsm_byte frame_chain;
	unsigned int sr;

	readGSM_33(d);// puts all the parameters into LARc etc.



			sr = 0;
			sr = (sr >> 6) | (LARc[0] << 10);
			sr = (sr >> 6) | (LARc[1] << 10);
			*c++ = sr >> 4;
			sr = (sr >> 5) | (LARc[2] << 11);
			*c++ = sr >> 7;
			sr = (sr >> 5) | (LARc[3] << 11);
			sr = (sr >> 4) | (LARc[4] << 12);
			*c++ = sr >> 6;
			sr = (sr >> 4) | (LARc[5] << 12);
			sr = (sr >> 3) | (LARc[6] << 13);
			*c++ = sr >> 7;
			sr = (sr >> 3) | (LARc[7] << 13);
			sr = (sr >> 7) | (Nc[0] << 9);
			*c++ = sr >> 5;
			sr = (sr >> 2) | (bc[0] << 14);
			sr = (sr >> 2) | (Mc[0] << 14);
			sr = (sr >> 6) | (xmaxc[0] << 10);
			*c++ = sr >> 3;
			sr = (sr >> 3 )|( xmc[0] << 13);
			*c++ = sr >> 8;
			sr = (sr >> 3 )|( xmc[1] << 13);
			sr = (sr >> 3 )|( xmc[2] << 13);
			sr = (sr >> 3 )|( xmc[3] << 13);
			*c++ = sr >> 7;
			sr = (sr >> 3 )|( xmc[4] << 13);
			sr = (sr >> 3 )|( xmc[5] << 13);
			sr = (sr >> 3 )|( xmc[6] << 13);
			*c++ = sr >> 6;
			sr = (sr >> 3 )|( xmc[7] << 13);
			sr = (sr >> 3 )|( xmc[8] << 13);
			*c++ = sr >> 8;
			sr = (sr >> 3 )|( xmc[9] << 13);
			sr = (sr >> 3 )|( xmc[10] << 13);
			sr = (sr >> 3 )|( xmc[11] << 13);
			*c++ = sr >> 7;
			sr = (sr >> 3 )|( xmc[12] << 13);
			sr = (sr >> 7 )|( Nc[1] << 9);
			*c++ = sr >> 5;
			sr = (sr >> 2 )|( bc[1] << 14);
			sr = (sr >> 2 )|( Mc[1] << 14);
			sr = (sr >> 6 )|( xmaxc[1] << 10);
			*c++ = sr >> 3;
			sr = (sr >> 3 )|( xmc[13] << 13);
			*c++ = sr >> 8;
			sr = (sr >> 3 )|( xmc[14] << 13);
			sr = (sr >> 3 )|( xmc[15] << 13);
			sr = (sr >> 3 )|( xmc[16] << 13);
			*c++ = sr >> 7;
			sr = (sr >> 3 )|( xmc[17] << 13);
			sr = (sr >> 3 )|( xmc[18] << 13);
			sr = (sr >> 3 )|( xmc[19] << 13);
			*c++ = sr >> 6;
			sr = (sr >> 3 )|( xmc[20] << 13);
			sr = (sr >> 3 )|( xmc[21] << 13);
			*c++ = sr >> 8;
			sr = (sr >> 3 )|( xmc[22] << 13);
			sr = (sr >> 3 )|( xmc[23] << 13);
			sr = (sr >> 3 )|( xmc[24] << 13);
			*c++ = sr >> 7;
			sr = (sr >> 3 )|( xmc[25] << 13);
			sr = (sr >> 7 )|( Nc[2] << 9);
			*c++ = sr >> 5;
			sr = (sr >> 2 )|( bc[2] << 14);
			sr = (sr >> 2 )|( Mc[2] << 14);
			sr = (sr >> 6 )|( xmaxc[2] << 10);
			*c++ = sr >> 3;
			sr = (sr >> 3 )|( xmc[26] << 13);
			*c++ = sr >> 8;
			sr = (sr >> 3 )|( xmc[27] << 13);
			sr = (sr >> 3 )|( xmc[28] << 13);
			sr = (sr >> 3 )|( xmc[29] << 13);
			*c++ = sr >> 7;
			sr = (sr >> 3 )|( xmc[30] << 13);
			sr = (sr >> 3 )|( xmc[31] << 13);
			sr = (sr >> 3 )|( xmc[32] << 13);
			*c++ = sr >> 6;
			sr = (sr >> 3 )|( xmc[33] << 13);
			sr = (sr >> 3 )|( xmc[34] << 13);
			*c++ = sr >> 8;
			sr = (sr >> 3 )|( xmc[35] << 13);
			sr = (sr >> 3 )|( xmc[36] << 13);
			sr = (sr >> 3 )|( xmc[37] << 13);
			*c++ = sr >> 7;
			sr = (sr >> 3 )|( xmc[38] << 13);
			sr = (sr >> 7 )|( Nc[3] << 9);
			*c++ = sr >> 5;
			sr = (sr >> 2 )|( bc[3] << 14);
			sr = (sr >> 2 )|( Mc[3] << 14);
			sr = (sr >> 6 )|( xmaxc[3] << 10);
			*c++ = sr >> 3;
			sr = (sr >> 3 )|( xmc[39] << 13);
			*c++ = sr >> 8;
			sr = (sr >> 3 )|( xmc[40] << 13);
			sr = (sr >> 3 )|( xmc[41] << 13);
			sr = (sr >> 3 )|( xmc[42] << 13);
			*c++ = sr >> 7;
			sr = (sr >> 3 )|( xmc[43] << 13);
			sr = (sr >> 3 )|( xmc[44] << 13);
			sr = (sr >> 3 )|( xmc[45] << 13);
			*c++ = sr >> 6;
			sr = (sr >> 3 )|( xmc[46] << 13);
			sr = (sr >> 3 )|( xmc[47] << 13);
			*c++ = sr >> 8;
			sr = (sr >> 3 )|( xmc[48] << 13);
			sr = (sr >> 3 )|( xmc[49] << 13);
			sr = (sr >> 3 )|( xmc[50] << 13);
			*c++ = sr >> 7;
			sr = (sr >> 3 )|( xmc[51] << 13);
			sr = sr >> 4;
			*c = sr >> 8;
//			s->frame_chain = *c;
			frame_chain = *c;

		readGSM_33(d+33);// puts all the parameters into LARc etc.


			sr = 0;
//			sr = (sr >> 4 )|( s->frame_chain << 12);
			sr = (sr >> 4 )|( frame_chain << 12);

			sr = (sr >> 6 )|( LARc[0] << 10);
			*c++ = sr >> 6;
			sr = (sr >> 6 )|( LARc[1] << 10);
			*c++ = sr >> 8;
			sr = (sr >> 5 )|( LARc[2] << 11);
			sr = (sr >> 5 )|( LARc[3] << 11);
			*c++ = sr >> 6;
			sr = (sr >> 4 )|( LARc[4] << 12);
			sr = (sr >> 4 )|( LARc[5] << 12);
			*c++ = sr >> 6;
			sr = (sr >> 3 )|( LARc[6] << 13);
			sr = (sr >> 3 )|( LARc[7] << 13);
			*c++ = sr >> 8;
			sr = (sr >> 7 )|( Nc[0] << 9);
			sr = (sr >> 2 )|( bc[0] << 14);
			*c++ = sr >> 7;
			sr = (sr >> 2 )|( Mc[0] << 14);
			sr = (sr >> 6 )|( xmaxc[0] << 10);
			*c++ = sr >> 7;
			sr = (sr >> 3 )|( xmc[0] << 13);
			sr = (sr >> 3 )|( xmc[1] << 13);
			sr = (sr >> 3 )|( xmc[2] << 13);
			*c++ = sr >> 6;
			sr = (sr >> 3 )|( xmc[3] << 13);
			sr = (sr >> 3 )|( xmc[4] << 13);
			*c++ = sr >> 8;
			sr = (sr >> 3 )|( xmc[5] << 13);
			sr = (sr >> 3 )|( xmc[6] << 13);
			sr = (sr >> 3 )|( xmc[7] << 13);
			*c++ = sr >> 7;
			sr = (sr >> 3 )|( xmc[8] << 13);
			sr = (sr >> 3 )|( xmc[9] << 13);
			sr = (sr >> 3 )|( xmc[10] << 13);
			*c++ = sr >> 6;
			sr = (sr >> 3 )|( xmc[11] << 13);
			sr = (sr >> 3 )|( xmc[12] << 13);
			*c++ = sr >> 8;
			sr = (sr >> 7 )|( Nc[1] << 9);
			sr = (sr >> 2 )|( bc[1] << 14);
			*c++ = sr >> 7;
			sr = (sr >> 2 )|( Mc[1] << 14);
			sr = (sr >> 6 )|( xmaxc[1] << 10);
			*c++ = sr >> 7;
			sr = (sr >> 3 )|( xmc[13] << 13);
			sr = (sr >> 3 )|( xmc[14] << 13);
			sr = (sr >> 3 )|( xmc[15] << 13);
			*c++ = sr >> 6;
			sr = (sr >> 3 )|( xmc[16] << 13);
			sr = (sr >> 3 )|( xmc[17] << 13);
			*c++ = sr >> 8;
			sr = (sr >> 3 )|( xmc[18] << 13);
			sr = (sr >> 3 )|( xmc[19] << 13);
			sr = (sr >> 3 )|( xmc[20] << 13);
			*c++ = sr >> 7;
			sr = (sr >> 3 )|( xmc[21] << 13);
			sr = (sr >> 3 )|( xmc[22] << 13);
			sr = (sr >> 3 )|( xmc[23] << 13);
			*c++ = sr >> 6;
			sr = (sr >> 3 )|( xmc[24] << 13);
			sr = (sr >> 3 )|( xmc[25] << 13);
			*c++ = sr >> 8;
			sr = (sr >> 7 )|( Nc[2] << 9);
			sr = (sr >> 2 )|( bc[2] << 14);
			*c++ = sr >> 7;
			sr = (sr >> 2 )|( Mc[2] << 14);
			sr = (sr >> 6 )|( xmaxc[2] << 10);
			*c++ = sr >> 7;
			sr = (sr >> 3 )|( xmc[26] << 13);
			sr = (sr >> 3 )|( xmc[27] << 13);
			sr = (sr >> 3 )|( xmc[28] << 13);
			*c++ = sr >> 6;
			sr = (sr >> 3 )|( xmc[29] << 13);
			sr = (sr >> 3 )|( xmc[30] << 13);
			*c++ = sr >> 8;
			sr = (sr >> 3 )|( xmc[31] << 13);
			sr = (sr >> 3 )|( xmc[32] << 13);
			sr = (sr >> 3 )|( xmc[33] << 13);
			*c++ = sr >> 7;
			sr = (sr >> 3 )|( xmc[34] << 13);
			sr = (sr >> 3 )|( xmc[35] << 13);
			sr = (sr >> 3 )|( xmc[36] << 13);
			*c++ = sr >> 6;
			sr = (sr >> 3 )|( xmc[37] << 13);
			sr = (sr >> 3 )|( xmc[38] << 13);
			*c++ = sr >> 8;
			sr = (sr >> 7 )|( Nc[3] << 9);
			sr = (sr >> 2 )|( bc[3] << 14);
			*c++ = sr >> 7;
			sr = (sr >> 2 )|( Mc[3] << 14);
			sr = (sr >> 6 )|( xmaxc[3] << 10);
			*c++ = sr >> 7;
			sr = (sr >> 3 )|( xmc[39] << 13);
			sr = (sr >> 3 )|( xmc[40] << 13);
			sr = (sr >> 3 )|( xmc[41] << 13);
			*c++ = sr >> 6;
			sr = (sr >> 3 )|( xmc[42] << 13);
			sr = (sr >> 3 )|( xmc[43] << 13);
			*c++ = sr >> 8;
			sr = (sr >> 3 )|( xmc[44] << 13);
			sr = (sr >> 3 )|( xmc[45] << 13);
			sr = (sr >> 3 )|( xmc[46] << 13);
			*c++ = sr >> 7;
			sr = (sr >> 3 )|( xmc[47] << 13);
			sr = (sr >> 3 )|( xmc[48] << 13);
			sr = (sr >> 3 )|( xmc[49] << 13);
			*c++ = sr >> 6;
			sr = (sr >> 3 )|( xmc[50] << 13);
			sr = (sr >> 3 )|( xmc[51] << 13);
			*c++ = sr >> 8;

}



