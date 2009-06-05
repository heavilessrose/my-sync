#ifndef _WK_XMLGB_H_
#define _WK_XMLGB_H_

//返回错误码
#define WINKS_WX_ESUCCESS	0	//函数正确返回	0
#define WINKS_WX_EERROR		-1	//函数未知错误返回	-1
#define WINKS_WX_EXMLINVALID	-2	//XML文档无效	-2
#define WINKS_WX_ESTRINVALID	-3	//XML数据结构无效	-3
#define WINKS_WX_ENOTFINISH		-4	//XML文档尚未结束	-4
#define WINKS_WX_EFINISH		-5	//XML文档已经结束	-5
#define WINKS_WX_ERESOURCE		-6	//资源不足	-6
#define WINKS_WX_EPARA			-7	//Parameter invalid
#define WINKS_WX_EBUFFSHORT		-8	//内存长度过短

//编码格式
#define WINKS_WX_CUTF8		1	//UTF-8编码格式	1
#define WINKS_WX_CUNKNOW	0	//未知编码格式	-1

//本结构用来保存XML元素属性内容。按照本设计本结构暂不支持，仅作为扩展使用。
//本结构公开给上层使用。
typedef struct tag_Winks_XmlAttribute_s
{
	struct tag_Winks_XmlAttribute_s* pnext;	//链表指针
	char* pname;							//名字内容
	char* pvalue;							//数值内容
}Winks_XmlAttribute_s;

//本结构用来保存XML元素内容。为了便于创建和检索，整个XML文档中的元素结构采用二叉树方式保存。
//本结构公开给上层使用。
typedef struct tag_Winks_XmlElement_s
{
	struct tag_Winks_XmlElement_s* pchild;		//元素子节点
	struct tag_Winks_XmlElement_s* pbrother;	//元素兄弟节点
	struct tag_Winks_XmlElement_s* pparent;		//元素父节点
	char* ptag;									//元素名称
	int taglen;
	Winks_XmlAttribute_s* pattribute;			//元素属性
	char* pcontent;								//元素内容
	int conlen;
}Winks_XmlElement_s;

//本结构保存XML文档数据结构。
//本结构公开给上层使用。
typedef struct tag_Winks_Xml_s
{
	unsigned short mainver;				//主版本号
	unsigned short slavever;			//副版本号
	int encoding;						//编码格式
	int ifsd;							//SDDecl
	Winks_XmlElement_s* pelement;		//根元素
}Winks_Xml_s;

/*
	本结构保存XML句柄内存结构。
	整个内存结构使用固定长度内存链表实现，该结构有两个应用场景。
1.	XML解析时解析器在该链表中自己组织数据，使得应用程序可以通过公开的结构读取到数据。
2.	XML生成时，应用程序使用该结构将XML文档传入。
	本结构公开给上层使用。
*/
typedef struct tag_Winks_XmlMem_s
{
	struct tag_Winks_XmlMem_s* pnext;	//链表指针
	int len;							//data len
	int offset;							//data offset
	char* pdata;						//数据指针
}Winks_XmlMem_s;

int Winks_XML_Generate( Winks_Xml_s* pXml, char* pSource, int* pSlen );
int Winks_XML_Parse( Winks_XmlMem_s* pData, Winks_Xml_s** ppXml );
int Winks_XML_Free(Winks_Xml_s* pXml );

#endif