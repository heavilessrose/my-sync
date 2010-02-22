/**************************************************************************
Copyright (C) Beijing Mobile Winks Co., Ltd
All rights reserved.
 
This source code is part of winks software and is licensed by WINKS SOFTWARE LICENSE AGREEMENT.
 
The above copyright notice and this permission notice shall be included in all copies or substantial 
portions of the Software.
 
THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, 
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
IN NO EVENT SHALL ANY PERSON WHO HAS CONTRIBUTED TO OR IS THE OWNER OF ANY PART OF THIS SOFTWARE BE LIABLE 
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES 
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; 
OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, 
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED 
OF THE POSSIBILITY OF SUCH DAMAGE.
**************************************************************************
*Id: wk_ospubd.h,v 1.00 08/12/2009
**************************************************************************    
* File Description
* ----------------
* Configuration about project, just for dynamic load block. 
**************************************************************************/

#ifndef _WK_OSPUBD_H_
#define _WK_OSPUBD_H_

#ifdef __cplusplus
extern "C" {
#endif

/*************************************************
            系统接口
**************************************************/

#ifdef WINKS_SYMBIAN_V_9_0
#define WINKS_DB_FILE           "c:\\data\\others\\winks\\debugf.txt"
#else
#define WINKS_DB_FILE           WINKS_APP_PATH"debugf.txt"
#endif


/************************************************
                  MMU FUNCTION
************************************************/
#ifdef WINKS_DEBUG
    void* Winks_xsalAlloc(unsigned long size, char* file, unsigned long line);
    int Winks_xsalFree( void* point, char* file, unsigned long line);
    void Winks_xsalPrintMemInfo(void);
//    int WK_IsMemCleanup(char* file);
    #define Winks_salAlloc(x)      Winks_xsalAlloc( x, __FILE__, __LINE__ )
    #define Winks_salFree(x)       Winks_xsalFree( x, __FILE__, __LINE__ )
#else
    void* Winks_Alloc(unsigned int size);
    void Winks_Free(void *ptr);
    #define Winks_salAlloc         Winks_Alloc
    #define Winks_salFree          Winks_Free
#endif


/************************************************
                  ASSERT FUNCTION
************************************************/
#ifdef WINKS_CHECK_ASSERTIONS
    void Winks_assertfail(int assertFailType, const char *cond, const char *file, int line);
#endif


/************************************************
                  PRINT FUNCTION
************************************************/
    #define WINKS_PRINT_BUFF_SIZE 1024
    #define WINKS_PRINT_LEVEL_INFO 150
    #define WINKS_PRINT_LEVEL_WARN 200 
    #define WINKS_PRINT_LEVEL_ERR 250 
    #define WINKS_PRINT_LEVEL_NONE 256 
    #define WINKS_PRINT_LEVEL_INVALID (WINKS_PRINT_LEVEL_NONE+1)
#ifdef WINKS_SHOW_LOG
    void HEX_DataOut(const unsigned char *fragment, unsigned short length, const unsigned char *name);
    #define Winks_printHex HEX_DataOut
    void Winks_printfErr( char* format, ... );
    void Winks_printfWarn( char* format, ... );
    void Winks_printfInfo( char* format, ... );
    void Winks_printHexInfo(const unsigned char *fragment, unsigned short length, const unsigned char *name);
#else
    #define Winks_printHex(buff,len,head)

    #if defined(WINKS_SYMBIAN_V_9_0)
        static inline void Winks_printfErr( char* format, ... ){};
        static inline void Winks_printfWarn( char* format, ... ){};
        static inline void Winks_printfInfo( char* format, ... ){};
    #elif ((defined(WINKS_ARENA_V_1_2)) || (defined (WINKS_MTK) && defined(WINKS_SIMULATOR)) || (defined (WINKS_BREW) && defined(WINKS_SIMULATOR)))
        void Winks_printfErr( char* format, ... );
        void Winks_printfWarn( char* format, ... );
        void Winks_printfInfo( char* format, ... );
        #define Winks_printfErr(a)
        #define Winks_printfWarn(a)
        #define Winks_printfInfo(a)
    #else 
        #define Winks_printfErr(...)
        #define Winks_printfWarn(...)
        #define Winks_printfInfo(...)
    #endif
#endif


/************************************************
                  PRINT FUNCTION
************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
int Winks_strnicmp(const char *s1, const char * s2, unsigned int n);
int Winks_stricmp(const char* s1, const char* s2 );
int Winks_toupper(int c);
int Winks_atoi(const char *nptr);
long Winks_atol(const char *nptr);
#define Winks_mem_set memset
#define Winks_mem_cpy memcpy
#define strnicmp Winks_strnicmp
#define stricmp Winks_stricmp
#define atoi Winks_atoi
#define atol Winks_atol
#define toupper Winks_toupper
#ifdef __cplusplus
}
#endif
#endif/*_WK_OSPUBD_H_*/
