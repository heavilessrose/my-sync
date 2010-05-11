/*
 * Copyright 1992 by Jutta Degener and Carsten Bormann, Technische
 * Universitaet Berlin.  See the accompanying file "COPYRIGHT" for
 * details.  THERE IS ABSOLUTELY NO WARRANTY FOR THIS SOFTWARE.
 */

/*$Header: /home/cvsroot/project/msnv5/client/iphone/MSN/gsm610/GSM.H,v 1.1 2008/12/15 06:19:24 zenghg Exp $*/

#ifndef	GSM_H
#define	GSM_H

#ifdef __cplusplus
#	define	NeedFunctionPrototypes	1
#endif

#if __STDC__
#	define	NeedFunctionPrototypes	1
#endif

#ifdef _NO_PROTO
#	undef	NeedFunctionPrototypes
#endif

#undef	GSM_P	/* gnu stdio.h actually defines this... 	*/

#if NeedFunctionPrototypes
#	define	GSM_P( protos )	protos
#else
#	define  GSM_P( protos )	( /* protos */ )
#endif

#ifdef NeedFunctionPrototypes
#   include	<stdio.h>		/* for FILE * 	*/
#endif

/*
 *	Interface
 */

typedef struct gsm_state * 	gsm;
typedef short		   	gsm_signal;		/* signed 16 bit */
typedef unsigned char		gsm_byte;
typedef gsm_byte 		gsm_frame[33];		/* 33 * 8 bits	 */

#define	GSM_MAGIC	0xD			  	/* 13 kbit/s RPE-LTP */

#define	GSM_PATCHLEVEL	4
#define	GSM_MINOR	0
#define	GSM_MAJOR	1

#define	GSM_OPT_VERBOSE	1
#define	GSM_OPT_FAST	2

#ifdef __cplusplus
  extern "C" gsm  gsm_create 	GSM_P((void));
  extern "C" void gsm_destroy GSM_P((gsm));

#else
  extern gsm  gsm_create 	GSM_P((void));
  extern void gsm_destroy GSM_P((gsm));
#endif

extern int  gsm_print   GSM_P((FILE *, gsm, gsm_byte  *));
extern int  gsm_option  GSM_P((gsm, int, int *));
#ifdef __cplusplus
  extern "C" void gsm_encode  GSM_P((gsm, gsm_signal *, gsm_byte  *));
  extern "C" int  gsm_decode  GSM_P((gsm, gsm_byte   *, gsm_signal *));
#else
  extern void gsm_encode  GSM_P((gsm, gsm_signal *, gsm_byte  *));
  extern int  gsm_decode  GSM_P((gsm, gsm_byte   *, gsm_signal *));
#endif

extern int  gsm_explode GSM_P((gsm, gsm_byte   *, gsm_signal *));
extern void gsm_implode GSM_P((gsm, gsm_signal *, gsm_byte   *));

#undef	GSM_P

#endif	/* GSM_H */
