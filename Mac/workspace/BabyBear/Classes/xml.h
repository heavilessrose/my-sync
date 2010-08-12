#ifndef _XML_H_
#define _XML_H_

#if 0//ndef NULL
#define NULL ((void*)0) 
int assert( int a );
void* salAlloc( int size );
int salFree( void* p );
int mem_cpy( void* pd, void* ps, int len );
int mem_set( void* pd, int data, int len );

#endif

//句柄类型码
#define WX_GENERATE    0    //句柄用来生成XML文档    0
#define WX_PARSE        1    //句柄用来解析XML文档    1

//参数操作码
#define WX_PFILE        0    //传入参数为文件名    0    
#define WX_PBUFF        1    //传入参数为数据块    1    

//返回错误码
#define WX_ESUCCESS    0    //函数正确返回    0
#define WX_EERROR        -1    //函数未知错误返回    -1
#define WX_EXMLINVALID    -2    //XML文档无效    -2
#define WX_ESTRINVALID    -3    //XML数据结构无效    -3
#define WX_ENOTFINISH        -4    //XML文档尚未结束    -4
#define WX_EFINISH        -5    //XML文档已经结束    -5
#define WX_ERESOURCE        -6    //资源不足    -6
#define WX_EPARA            -7    //Parameter invalid
#define WX_EBUFFSHORT        -8    //内存长度过短

//#define WX_EXMLMEMLOW        -20    //内部使用

//编码格式
#define WX_CUTF8        1    //UTF-8编码格式    1
#define WX_CUNKNOW    0    //未知编码格式    -1

typedef struct tag_XmlGen_s
{
    char* pgbuff;
    int pglen;
    int pgoff;
    int generr;
}XmlGen_s;

//本结构用来保存XML元素属性内容。按照本设计本结构暂不支持，仅作为扩展使用。
//本结构公开给上层使用。
typedef struct tag_XmlAttribute_s
{
    struct tag_XmlAttribute_s* pnext;    //链表指针
    char* pname;                            //名字内容
    char* pvalue;                            //数值内容
}XmlAttribute_s;

//本结构用来保存XML元素内容。为了便于创建和检索，整个XML文档中的元素结构采用二叉树方式保存。
//本结构公开给上层使用。
typedef struct tag_XmlElement_s
{
    struct tag_XmlElement_s* pchild;        //元素子节点
    struct tag_XmlElement_s* pbrother;    //元素兄弟节点
    struct tag_XmlElement_s* pparent;        //元素父节点
    char* ptag;                                    //元素名称
    int taglen;
    XmlAttribute_s* pattribute;            //元素属性
    char* pcontent;                                //元素内容
    int conlen;
}XmlElement_s;

//本结构保存XML文档数据结构。
//本结构公开给上层使用。
typedef struct tag_Xml_s
{
    unsigned short mainver;                //主版本号
    unsigned short slavever;            //副版本号
    int encoding;                        //编码格式
    int ifsd;                            //SDDecl
    XmlElement_s* pelement;        //根元素
}Xml_s;

/*
    本结构保存XML句柄内存结构。
    整个内存结构使用固定长度内存链表实现，该结构有两个应用场景。
1.    XML解析时解析器在该链表中自己组织数据，使得应用程序可以通过公开的结构读取到数据。
2.    XML生成时，应用程序使用该结构将XML文档传入。
    本结构公开给上层使用。
*/
typedef struct tag_XmlMem_s
{
    struct tag_XmlMem_s* pnext;    //链表指针
    int len;                            //data len
    int offset;                            //data offset
    char* pdata;                        //数据指针
}XmlMem_s;

#define WX_DFTAGLEN        20
#define WX_DFCONLEN        32
#define WX_DFNEWLEN        1024
typedef struct tag_XmlEleInt_s
{
    struct tag_XmlEleInt_s* pchild;        //元素子节点
    struct tag_XmlEleInt_s* pbrother;    //元素兄弟节点
    struct tag_XmlEleInt_s* pparent;        //元素父节点
    char* ptag;                                    //元素名称
    int taglen;
    XmlAttribute_s* pattribute;            //元素属性
    char* pcontent;                                //元素内容
    int conlen;
    int tagbflen;
    int conbflen;
    int checklen;
//    char tagbuf[WX_DFTAGLEN];
//    char conbuf[WX_DFCONLEN];
}XmlEleInt_s;

/*
    本结构保存XML文档数据结构内部结构。与公开的数据结构相比，本结构多了一个指向保留数据的内存指针，这样就可以让XML模块释放申请的内存。
    本结构不公开给上层使用。
*/
typedef struct tag_XmlInt_s
{
    unsigned short mainver;
    unsigned short slavever;
    int encoding;
    int ifsd;        //SDDecl
    XmlEleInt_s* pelement;
    XmlEleInt_s* pparele;        //解析当前元素位置
    XmlMem_s* pmem;
    int charstate;
    int docstate;
    int ifcommand;
    int parerr;
}XmlInt_s;


#endif