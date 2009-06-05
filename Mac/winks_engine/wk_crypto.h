
#ifdef __cplusplus
extern "C" { 
#endif

/* base64���뺯��
* mode: 0 - ��׼base64��1 - base64url
*/
extern int Winks_Base64Encode(unsigned int mode, const unsigned char *src, unsigned int src_len, unsigned char *dest, unsigned int dest_len);

/* base64���뺯��
* mode: 0 - ��׼base64��1 - base64url
*/
extern int Winks_Base64Decode(unsigned int mode, const unsigned char *src, unsigned char *dest, unsigned int dest_len);

/* AES���ܺ��� - �ն�ֻ����ɼ����㷨�������ڷ���������ɡ�
 * function: Winks_Encrypt

 * parameters:
 *   const unsigned char *src: ����
 *   unsigned int src_len: ���ĳ���
 *   unsigned char *dest: ���buf
 *   unsigned int dest_len: ���buf ����

 * return:
 *   �ɹ�����: 0
 *   ʧ�ܷ���: -1
 */
extern int Winks_Encrypt(const unsigned char *src, unsigned int src_len, unsigned char *dest, unsigned int dest_len);

/* AES ���ܲ��Լ��ܺ��������base64url ���� */
extern int Winks_EncryptAndBase64(const unsigned char *src, unsigned int src_len, unsigned char *dest, unsigned int dest_len);

#ifdef __cplusplus
}
#endif

