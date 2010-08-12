#ifndef _XMLGB_H_
#define _XMLGB_H_

//���ش�����
#define WX_ESUCCESS		0     //������ȷ����    0
#define WX_EERROR			-1    //����δ֪���󷵻�    -1
#define WX_EXMLINVALID	-2    //XML�ĵ���Ч    -2
#define WX_ESTRINVALID    -3    //XML���ݽṹ��Ч    -3
#define WX_ENOTFINISH		-4    //XML�ĵ���δ����    -4
#define WX_EFINISH        -5    //XML�ĵ��Ѿ�����    -5
#define WX_ERESOURCE		-6    //��Դ����    -6
#define WX_EPARA			-7    //Parameter invalid
#define WX_EBUFFSHORT		-8    //�ڴ泤�ȹ���

//�����ʽ
#define WX_CUTF8		1    //UTF-8�����ʽ    1
#define WX_CUNKNOW	0    //δ֪�����ʽ    -1

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

int XML_Generate( Xml_s* pXml, char* pSource, int* pSlen );
int XML_Parse( XmlMem_s* pData, Xml_s** ppXml );
int XML_Free(Xml_s* pXml );

#endif