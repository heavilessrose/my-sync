package com.pica.msn;

public class Contact {

	/**��ϵ��|Ƶ��|������ ��jid */
	public String SUBITEM_JID;//jid ��ϵ�� Ƶ�� ������ ��������
	
	/**��ϵ�˵�����״̬ �б������������ݵ�Ȩ�� Ƶ���������Ҷ�Ĭ��Ϊ�ѻ�*/
	public byte SUBITEM_STATE=MSNApplication.STATE_OFFLINE; //����״̬ �б������������ݵ�Ȩ�� Ƶ���������Ҳ���ֵ ����Ĭ�϶���Ϊ0
	
	//*****************************
	//��ϵ��
    //*****************************
    /**��ϵ�˵�����*/
    public String CONTACT_imid;//����
    /**��ϵ�˵��ǳ�*/
    public String CONTACT_nickname;// �ǳ�
    /**��ϵ�˵ı�����ʾ����ǰ4���ַ���gb2312��*/
    public String CONTACT_gbcode;//   ������ʾ����ǰ4���ַ���gb2312��
    /**��ϵ�˵ķ���id*/
    public String CONTACT_groupId;//����id
    /**��ϵ�˵���ϵ�˱�־λ��������İ���ˡ���������*/
    public byte CONTACT_contactFlag;// ��ϵ�˱�־λ��������İ���ˡ���������
    /**��ϵ�˵��������*/
    public String CONTACT_impresa;// �������
    /**��ϵ�˵ĵ绰����*/
    public String CONTACT_phone;//�绰���� ����ǿ� �����ӡ�����绰��
    /**��ϵ�˵ĵ�ǰ״̬������˵�� ��busy|online��*/
    public String CONTACT_statusShow;// ��ǰ״̬������˵�� busy online�� ���ñ��浽rms	
    /**��ϵ�˵ĸ��˿ռ���� 0=û�и��� 1=�и���*/
    public String CONTACT_gleam;//0 û�и��� 1 �и���
    /**��ϵ�˵Ŀռ��ַ*/
    public String CONTACT_url;// �ռ��ַ
    
    //public String CONTACT_mobile;//   0���ƶ��û�|1�ƶ��û�
    
    //#ifdef support_head_image
    public String CONTACT_portraithash;// ��ϵ�˵�ͷ���ʶ�����ݸ�ֵ�ͻ��˿����ж��û���ͷ���Ƿ����仯
	public byte[] CONTACT_imageData;// ͷ������
	public boolean CONTACT_hasRMSImageData=false;//RMS�����Ƿ񱣴���ͷ������
    /**��ǰ�ѷ��͹���ȡͷ���*/
	public boolean CONTACT_hasSendGetImage=false;
	//#endif
	
	/**��������ͷ�� �����ж��Ƿ����Լ��ĸ���ͷ�� ��������Լ���imid ����Ϊ���Լ��ĸ���ͷ��*/
	public String CONTACT_OUTER_HEAD_ONESELF_IMID;
	/**���� ͷ�� ״̬*/
	public byte CONTACT_OUTER_HEAD_STATUS;
	
	
	/**�ش�����*/
	public String CONTACT_FILE_VOICE_TRANSFER_COUNT;
	
	/**�Ѵ���İٷֱ� 0-100*/
	public String CONTACT_GAUGE_INDEX;
	
	/**��Ϣ�������ص�ַ*/
	public String CONTACT_FILE_URL;//  �ļ����ص�ַ
	
	/**��Ϣ������ ����base64��ʽ*/
	public byte[] CONTACT_FILE_DATA;
	
	/**��Ϣ�����ļ���*/
	public String CONTACT_FILE_VOICE_NAME;
	
	/**�͸��ļ�/�������� ��صĴ��⴫�����*/
	public FileOuterTransfer CONTACT_FILEOUTTRANSFER;
	
	/**��Ϣ������״̬ 
	 * <br>���ͣ��ȴ����͵ȴ������� ���Է��ܾ�<br>�����ڷ���<br>�������<br>������ȡ��(�����򱻶�)<br>�������쳣<br>
	 * <br>���գ��ȴ�����<br>�ȴ����Լ��ܾ�<br>���ڽ���<br>�������<br>������ȡ��<br>�������쳣*/
	public byte CONTACT_FILE_STATUS_TYPE;//
}
