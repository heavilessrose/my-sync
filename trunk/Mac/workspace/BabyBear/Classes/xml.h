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

//���������
#define WX_GENERATE    0    //�����������XML�ĵ�    0
#define WX_PARSE        1    //�����������XML�ĵ�    1

//����������
#define WX_PFILE        0    //�������Ϊ�ļ���    0    
#define WX_PBUFF        1    //�������Ϊ���ݿ�    1    

//���ش�����
#define WX_ESUCCESS    0    //������ȷ����    0
#define WX_EERROR        -1    //����δ֪���󷵻�    -1
#define WX_EXMLINVALID    -2    //XML�ĵ���Ч    -2
#define WX_ESTRINVALID    -3    //XML���ݽṹ��Ч    -3
#define WX_ENOTFINISH        -4    //XML�ĵ���δ����    -4
#define WX_EFINISH        -5    //XML�ĵ��Ѿ�����    -5
#define WX_ERESOURCE        -6    //��Դ����    -6
#define WX_EPARA            -7    //Parameter invalid
#define WX_EBUFFSHORT        -8    //�ڴ泤�ȹ���

//#define WX_EXMLMEMLOW        -20    //�ڲ�ʹ��

//�����ʽ
#define WX_CUTF8        1    //UTF-8�����ʽ    1
#define WX_CUNKNOW    0    //δ֪�����ʽ    -1

typedef struct tag_XmlGen_s
{
    char* pgbuff;
    int pglen;
    int pgoff;
    int generr;
}XmlGen_s;

//���ṹ��������XMLԪ���������ݡ����ձ���Ʊ��ṹ�ݲ�֧�֣�����Ϊ��չʹ�á�
//���ṹ�������ϲ�ʹ�á�
typedef struct tag_XmlAttribute_s
{
    struct tag_XmlAttribute_s* pnext;    //����ָ��
    char* pname;                            //��������
    char* pvalue;                            //��ֵ����
}XmlAttribute_s;

//���ṹ��������XMLԪ�����ݡ�Ϊ�˱��ڴ����ͼ���������XML�ĵ��е�Ԫ�ؽṹ���ö�������ʽ���档
//���ṹ�������ϲ�ʹ�á�
typedef struct tag_XmlElement_s
{
    struct tag_XmlElement_s* pchild;        //Ԫ���ӽڵ�
    struct tag_XmlElement_s* pbrother;    //Ԫ���ֵܽڵ�
    struct tag_XmlElement_s* pparent;        //Ԫ�ظ��ڵ�
    char* ptag;                                    //Ԫ������
    int taglen;
    XmlAttribute_s* pattribute;            //Ԫ������
    char* pcontent;                                //Ԫ������
    int conlen;
}XmlElement_s;

//���ṹ����XML�ĵ����ݽṹ��
//���ṹ�������ϲ�ʹ�á�
typedef struct tag_Xml_s
{
    unsigned short mainver;                //���汾��
    unsigned short slavever;            //���汾��
    int encoding;                        //�����ʽ
    int ifsd;                            //SDDecl
    XmlElement_s* pelement;        //��Ԫ��
}Xml_s;

/*
    ���ṹ����XML����ڴ�ṹ��
    �����ڴ�ṹʹ�ù̶������ڴ�����ʵ�֣��ýṹ������Ӧ�ó�����
1.    XML����ʱ�������ڸ��������Լ���֯���ݣ�ʹ��Ӧ�ó������ͨ�������Ľṹ��ȡ�����ݡ�
2.    XML����ʱ��Ӧ�ó���ʹ�øýṹ��XML�ĵ����롣
    ���ṹ�������ϲ�ʹ�á�
*/
typedef struct tag_XmlMem_s
{
    struct tag_XmlMem_s* pnext;    //����ָ��
    int len;                            //data len
    int offset;                            //data offset
    char* pdata;                        //����ָ��
}XmlMem_s;

#define WX_DFTAGLEN        20
#define WX_DFCONLEN        32
#define WX_DFNEWLEN        1024
typedef struct tag_XmlEleInt_s
{
    struct tag_XmlEleInt_s* pchild;        //Ԫ���ӽڵ�
    struct tag_XmlEleInt_s* pbrother;    //Ԫ���ֵܽڵ�
    struct tag_XmlEleInt_s* pparent;        //Ԫ�ظ��ڵ�
    char* ptag;                                    //Ԫ������
    int taglen;
    XmlAttribute_s* pattribute;            //Ԫ������
    char* pcontent;                                //Ԫ������
    int conlen;
    int tagbflen;
    int conbflen;
    int checklen;
//    char tagbuf[WX_DFTAGLEN];
//    char conbuf[WX_DFCONLEN];
}XmlEleInt_s;

/*
    ���ṹ����XML�ĵ����ݽṹ�ڲ��ṹ���빫�������ݽṹ��ȣ����ṹ����һ��ָ�������ݵ��ڴ�ָ�룬�����Ϳ�����XMLģ���ͷ�������ڴ档
    ���ṹ���������ϲ�ʹ�á�
*/
typedef struct tag_XmlInt_s
{
    unsigned short mainver;
    unsigned short slavever;
    int encoding;
    int ifsd;        //SDDecl
    XmlEleInt_s* pelement;
    XmlEleInt_s* pparele;        //������ǰԪ��λ��
    XmlMem_s* pmem;
    int charstate;
    int docstate;
    int ifcommand;
    int parerr;
}XmlInt_s;


#endif