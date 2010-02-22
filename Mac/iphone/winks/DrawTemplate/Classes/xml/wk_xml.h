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


//���������
#define WINKS_WX_GENERATE    0    //�����������XML�ĵ�    0
#define WINKS_WX_PARSE        1    //�����������XML�ĵ�    1

//����������
#define WINKS_WX_PFILE        0    //�������Ϊ�ļ���    0    
#define WINKS_WX_PBUFF        1    //�������Ϊ���ݿ�    1    

//���ش�����
#define WINKS_WX_ESUCCESS    0    //������ȷ����    0
#define WINKS_WX_EERROR        -1    //����δ֪���󷵻�    -1
#define WINKS_WX_EXMLINVALID    -2    //XML�ĵ���Ч    -2
#define WINKS_WX_ESTRINVALID    -3    //XML���ݽṹ��Ч    -3
#define WINKS_WX_ENOTFINISH        -4    //XML�ĵ���δ����    -4
#define WINKS_WX_EFINISH        -5    //XML�ĵ��Ѿ�����    -5
#define WINKS_WX_ERESOURCE        -6    //��Դ����    -6
#define WINKS_WX_EPARA            -7    //Parameter invalid
#define WINKS_WX_EBUFFSHORT        -8    //�ڴ泤�ȹ���

//#define WINKS_WX_EXMLMEMLOW        -20    //�ڲ�ʹ��

//�����ʽ
#define WINKS_WX_CUTF8        1    //UTF-8�����ʽ    1
#define WINKS_WX_CUNKNOW    0    //δ֪�����ʽ    -1

typedef struct tag_Winks_XmlGen_s
{
    char* pgbuff;
    int pglen;
    int pgoff;
    int generr;
}Winks_XmlGen_s;

//���ṹ��������XMLԪ���������ݡ����ձ���Ʊ��ṹ�ݲ�֧�֣�����Ϊ��չʹ�á�
//���ṹ�������ϲ�ʹ�á�
typedef struct tag_Winks_XmlAttribute_s
{
    struct tag_Winks_XmlAttribute_s* pnext;    //����ָ��
    char* pname;                            //��������
    char* pvalue;                            //��ֵ����
}Winks_XmlAttribute_s;

//���ṹ��������XMLԪ�����ݡ�Ϊ�˱��ڴ����ͼ���������XML�ĵ��е�Ԫ�ؽṹ���ö�������ʽ���档
//���ṹ�������ϲ�ʹ�á�
typedef struct tag_Winks_XmlElement_s
{
    struct tag_Winks_XmlElement_s* pchild;        //Ԫ���ӽڵ�
    struct tag_Winks_XmlElement_s* pbrother;    //Ԫ���ֵܽڵ�
    struct tag_Winks_XmlElement_s* pparent;        //Ԫ�ظ��ڵ�
    char* ptag;                                    //Ԫ������
    int taglen;
    Winks_XmlAttribute_s* pattribute;            //Ԫ������
    char* pcontent;                                //Ԫ������
    int conlen;
}Winks_XmlElement_s;

//���ṹ����XML�ĵ����ݽṹ��
//���ṹ�������ϲ�ʹ�á�
typedef struct tag_Winks_Xml_s
{
    unsigned short mainver;                //���汾��
    unsigned short slavever;            //���汾��
    int encoding;                        //�����ʽ
    int ifsd;                            //SDDecl
    Winks_XmlElement_s* pelement;        //��Ԫ��
}Winks_Xml_s;

/*
    ���ṹ����XML����ڴ�ṹ��
    �����ڴ�ṹʹ�ù̶������ڴ�����ʵ�֣��ýṹ������Ӧ�ó�����
1.    XML����ʱ�������ڸ��������Լ���֯���ݣ�ʹ��Ӧ�ó������ͨ�������Ľṹ��ȡ�����ݡ�
2.    XML����ʱ��Ӧ�ó���ʹ�øýṹ��XML�ĵ����롣
    ���ṹ�������ϲ�ʹ�á�
*/
typedef struct tag_Winks_XmlMem_s
{
    struct tag_Winks_XmlMem_s* pnext;    //����ָ��
    int len;                            //data len
    int offset;                            //data offset
    char* pdata;                        //����ָ��
}Winks_XmlMem_s;

int Winks_XML_Generate( Winks_Xml_s* pXml, char* pSource, int* pSlen );
int Winks_XML_Parse( Winks_XmlMem_s* pData, Winks_Xml_s** ppXml );
int Winks_XML_Free(Winks_Xml_s* pXml );

#define WINKS_WX_DFTAGLEN        20
#define WINKS_WX_DFCONLEN        32
#define WINKS_WX_DFNEWLEN        1024
typedef struct tag_Winks_XmlEleInt_s
{
    struct tag_Winks_XmlEleInt_s* pchild;        //Ԫ���ӽڵ�
    struct tag_Winks_XmlEleInt_s* pbrother;    //Ԫ���ֵܽڵ�
    struct tag_Winks_XmlEleInt_s* pparent;        //Ԫ�ظ��ڵ�
    char* ptag;                                    //Ԫ������
    int taglen;
    Winks_XmlAttribute_s* pattribute;            //Ԫ������
    char* pcontent;                                //Ԫ������
    int conlen;
    int tagbflen;
    int conbflen;
    int checklen;
//    char tagbuf[WINKS_WX_DFTAGLEN];
//    char conbuf[WINKS_WX_DFCONLEN];
}Winks_XmlEleInt_s;

/*
    ���ṹ����XML�ĵ����ݽṹ�ڲ��ṹ���빫�������ݽṹ��ȣ����ṹ����һ��ָ�������ݵ��ڴ�ָ�룬�����Ϳ�����XMLģ���ͷ�������ڴ档
    ���ṹ���������ϲ�ʹ�á�
*/
typedef struct tag_Winks_XmlInt_s
{
    unsigned short mainver;
    unsigned short slavever;
    int encoding;
    int ifsd;        //SDDecl
    Winks_XmlEleInt_s* pelement;
    Winks_XmlEleInt_s* pparele;        //������ǰԪ��λ��
    Winks_XmlMem_s* pmem;
    int charstate;
    int docstate;
    int ifcommand;
    int parerr;
}Winks_XmlInt_s;


#endif