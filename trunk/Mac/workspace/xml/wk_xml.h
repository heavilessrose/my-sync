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
*Id: wk_xml.h,v 1.00 
**************************************************************************    
* File Description
* ----------------
* Color call XML Parser internal define. 
**************************************************************************/

#ifndef _WK_XML_H_
#define _WK_XML_H_

#if 0//ndef NULL
#define NULL ((void*)0) 
int Winks_assert( int a );
void* Winks_salAlloc( int size );
int Winks_salFree( void* p );
int Winks_mem_cpy( void* pd, void* ps, int len );
int Winks_mem_set( void* pd, int data, int len );

#endif

//句柄类型码
#define WINKS_WX_GENERATE    0    //句柄用来生成XML文档    0
#define WINKS_WX_PARSE        1    //句柄用来解析XML文档    1

//参数操作码
#define WINKS_WX_PFILE        0    //传入参数为文件名    0    
#define WINKS_WX_PBUFF        1    //传入参数为数据块    1    

//返回错误码
#define WINKS_WX_ESUCCESS    0    //函数正确返回    0
#define WINKS_WX_EERROR        -1    //函数未知错误返回    -1
#define WINKS_WX_EXMLINVALID    -2    //XML文档无效    -2
#define WINKS_WX_ESTRINVALID    -3    //XML数据结构无效    -3
#define WINKS_WX_ENOTFINISH        -4    //XML文档尚未结束    -4
#define WINKS_WX_EFINISH        -5    //XML文档已经结束    -5
#define WINKS_WX_ERESOURCE        -6    //资源不足    -6
#define WINKS_WX_EPARA            -7    //Parameter invalid
#define WINKS_WX_EBUFFSHORT        -8    //内存长度过短

//#define WINKS_WX_EXMLMEMLOW        -20    //内部使用

//编码格式
#define WINKS_WX_CUTF8        1    //UTF-8编码格式    1
#define WINKS_WX_CUNKNOW    0    //未知编码格式    -1

typedef struct tag_Winks_XmlGen_s
{
    char* pgbuff;
    int pglen;
    int pgoff;
    int generr;
}Winks_XmlGen_s;

//本结构用来保存XML元素属性内容。按照本设计本结构暂不支持，仅作为扩展使用。
//本结构公开给上层使用。
typedef struct tag_Winks_XmlAttribute_s
{
    struct tag_Winks_XmlAttribute_s* pnext;    //链表指针
    char* pname;                            //名字内容
    char* pvalue;                            //数值内容
}Winks_XmlAttribute_s;

//本结构用来保存XML元素内容。为了便于创建和检索，整个XML文档中的元素结构采用二叉树方式保存。
//本结构公开给上层使用。
typedef struct tag_Winks_XmlElement_s
{
    struct tag_Winks_XmlElement_s* pchild;        //元素子节点
    struct tag_Winks_XmlElement_s* pbrother;    //元素兄弟节点
    struct tag_Winks_XmlElement_s* pparent;        //元素父节点
    char* ptag;                                    //元素名称
    int taglen;
    Winks_XmlAttribute_s* pattribute;            //元素属性
    char* pcontent;                                //元素内容
    int conlen;
}Winks_XmlElement_s;

//本结构保存XML文档数据结构。
//本结构公开给上层使用。
typedef struct tag_Winks_Xml_s
{
    unsigned short mainver;                //主版本号
    unsigned short slavever;            //副版本号
    int encoding;                        //编码格式
    int ifsd;                            //SDDecl
    Winks_XmlElement_s* pelement;        //根元素
}Winks_Xml_s;

/*
    本结构保存XML句柄内存结构。
    整个内存结构使用固定长度内存链表实现，该结构有两个应用场景。
1.    XML解析时解析器在该链表中自己组织数据，使得应用程序可以通过公开的结构读取到数据。
2.    XML生成时，应用程序使用该结构将XML文档传入。
    本结构公开给上层使用。
*/
typedef struct tag_Winks_XmlMem_s
{
    struct tag_Winks_XmlMem_s* pnext;    //链表指针
    int len;                            //data len
    int offset;                            //data offset
    char* pdata;                        //数据指针
}Winks_XmlMem_s;

#define WINKS_WX_DFTAGLEN        20
#define WINKS_WX_DFCONLEN        32
#define WINKS_WX_DFNEWLEN        1024
typedef struct tag_Winks_XmlEleInt_s
{
    struct tag_Winks_XmlEleInt_s* pchild;        //元素子节点
    struct tag_Winks_XmlEleInt_s* pbrother;    //元素兄弟节点
    struct tag_Winks_XmlEleInt_s* pparent;        //元素父节点
    char* ptag;                                    //元素名称
    int taglen;
    Winks_XmlAttribute_s* pattribute;            //元素属性
    char* pcontent;                                //元素内容
    int conlen;
    int tagbflen;
    int conbflen;
    int checklen;
//    char tagbuf[WINKS_WX_DFTAGLEN];
//    char conbuf[WINKS_WX_DFCONLEN];
}Winks_XmlEleInt_s;

/*
    本结构保存XML文档数据结构内部结构。与公开的数据结构相比，本结构多了一个指向保留数据的内存指针，这样就可以让XML模块释放申请的内存。
    本结构不公开给上层使用。
*/
typedef struct tag_Winks_XmlInt_s
{
    unsigned short mainver;
    unsigned short slavever;
    int encoding;
    int ifsd;        //SDDecl
    Winks_XmlEleInt_s* pelement;
    Winks_XmlEleInt_s* pparele;        //解析当前元素位置
    Winks_XmlMem_s* pmem;
    int charstate;
    int docstate;
    int ifcommand;
    int parerr;
}Winks_XmlInt_s;


#endif