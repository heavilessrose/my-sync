package com.pica.msn;

import java.util.Vector;

import android.graphics.Bitmap;

import com.pica.ui.tools.RosterItem;

public class Item
{

	//public RosterItem ri;
	
	//public byte TreeList_TYPE; 
	//**************
	//һ���б���Ԫ�ص����� 
	//************
	
	/**�Ự��sid(���������һỰ�Ǹ������ҵ�jid) <br>
	 * ��֤������imid <br>
	 * �����Һ�����jid*/
	public String TOPITEM_JID;//jid  �Ự��sid(���������һỰ�Ǹ������ҵ�jid) ��֤������imid �����Һ�����jid
	/**��ʶһ���б�Ԫ�����ͣ��Ự,�����Һ���,���Һ��ѷ���*/
	//public byte TOPITEM_TYPE;//��ʶһ���б�Ԫ�����ͣ��Ự,�����Һ���,���Һ��ѷ���
	
	//**************
	//�����б���Ԫ�ص����� 
	//************
	/**��ʶ�����б�Ԫ�����ͣ�������,��ϵ��,Ƶ��*/
	//public byte SUBITEM_TYPE;//��ʶ�����б�Ԫ�����ͣ�������,��ϵ��,Ƶ��
	
	/**��ϵ��|Ƶ��|������ ��jid */
	//public String SUBITEM_JID;//jid ��ϵ�� Ƶ�� ������ ��������
	
//	/**��ϵ�˵�����״̬ �б������������ݵ�Ȩ�� Ƶ���������Ҷ�Ĭ��Ϊ�ѻ�*/
//	public byte SUBITEM_STATE=MSNApplication.STATE_OFFLINE; //����״̬ �б������������ݵ�Ȩ�� Ƶ���������Ҳ���ֵ ����Ĭ�϶���Ϊ0
//
//
//	
//	//*****************************
//	//����
//    //*****************************
//	/**��ϵ�˷������ID*/
//    public String GROUP_id;//  
//    /**��gb��*/
//    public String GROUP_gbcode;// 
//    /**����*/
//    public String GROUP_name;//  
//    /**������ �û��Զ������ �����ҷ��� Ƶ������ ���*/
//    public byte GROUP_flag;//  
//    /**���չ��״̬*/
//    public boolean  GROUP_isexpand;
//    /**���ڳ�Ա������*/
//    public short GROUP_itemnum;// 
//    /**��������״̬������*/
//    public String GROUP_maxweight_itemnum;
//    
//	//*****************************
//	//��ϵ��
//    //*****************************
//    /**��ϵ�˵�����*/
//    public String CONTACT_imid;//����
//    /**��ϵ�˵��ǳ�*/
//    public String CONTACT_nickname;// �ǳ�
//    /**��ϵ�˵ı�����ʾ����ǰ4���ַ���gb2312��*/
//    public String CONTACT_gbcode;//   ������ʾ����ǰ4���ַ���gb2312��
//    /**��ϵ�˵ķ���id*/
//    public String CONTACT_groupId;//����id
//    /**��ϵ�˵���ϵ�˱�־λ��������İ���ˡ���������*/
//    public byte CONTACT_contactFlag;// ��ϵ�˱�־λ��������İ���ˡ���������
//    /**��ϵ�˵��������*/
//    public String CONTACT_impresa;// �������
//    /**��ϵ�˵ĵ绰����*/
//    public String CONTACT_phone;//�绰���� ����ǿ� �����ӡ�����绰��
//    /**��ϵ�˵ĵ�ǰ״̬������˵�� ��busy|online��*/
//    public String CONTACT_statusShow;// ��ǰ״̬������˵�� busy online�� ���ñ��浽rms	
//    /**��ϵ�˵ĸ��˿ռ���� 0=û�и��� 1=�и���*/
//    public String CONTACT_gleam;//0 û�и��� 1 �и���
//    /**��ϵ�˵Ŀռ��ַ*/
//    public String CONTACT_url;// �ռ��ַ
//    
//    //public String CONTACT_mobile;//   0���ƶ��û�|1�ƶ��û�
//    
//    //#ifdef support_head_image
//    public String CONTACT_portraithash;// ��ϵ�˵�ͷ���ʶ�����ݸ�ֵ�ͻ��˿����ж��û���ͷ���Ƿ����仯
//	public byte[] CONTACT_imageData;// ͷ������
//	public boolean CONTACT_hasRMSImageData=false;//RMS�����Ƿ񱣴���ͷ������
//    /**��ǰ�ѷ��͹���ȡͷ���*/
//	public boolean CONTACT_hasSendGetImage=false;
//	//#endif
	
	//*****************************
	// ��Ϣ���ݽṹ
    //******************************
	/**��Ϣ����*/
	public byte MESSAGE_type;//  ��Ϣ����
	/**��Ϣ�����ĻỰID*/
	public String MESSAGE_sid;//  ��Ϣ�����ĻỰID
	/**��Ϣ ��Ϣ�����˵�jid(���Ƿ�������Ϣ��Ϊ������jid),İ������Ϣû�д��� */
	public String MESSAGE_jid;//  İ����û�д��� ��Ϣ�����˵�jid(���Ƿ�������Ϣ��Ϊ������jid)
	/**��Ϣ İ������Ϣ��İ����imid ��¼�˺�*/
	public String MESSAGE_imid;// İ������Ϣ��İ����imid
	/**��Ϣ ��Ϣ�������ǳ�(���Ƿ�������Ϣ��Ϊ�������ǳ�)*/
	public String MESSAGE_nickname;//  ��Ϣ�������ǳ�(���Ƿ�������Ϣ��Ϊ�������ǳ�)
	/**��Ϣ����*/
	public String MESSAGE_body;//   ��Ϣ����
	/**��Ϣ״̬ �Ƿ��Ƿ�����Ϣ*/
	public boolean MESSAGE_IS_SEND;//  
	/**��Ϣ��������*/
	public String MESSAGE_mime;//  ��������


	/**��Ϣʱ���*/
	public String MESSAGE_stamp;//  ʱ���
	/**��Ϣ ����Ϣ true=�� false=����*/
	public boolean MESSAGE_nudge=false;//true����
	/**��Ϣʧ�ܵ�ԭ��*/
	public String MESSAGE_reason;//����ʧ�ܵ�ԭ��
	/**��Ϣ status=��fail�� */
	public String MESSAGE_status;//status=��fail�� 
	/**��Ϣ Ⱥ��Ϣ group����chat ���Ļ� msn����û��*/
	public String MESSAGE_CLUSTER_TYPE;//group����chat Ⱥ��Ϣ ���Ļ� msn����û��
	/**��Ϣ�����ļ���*/
	public String MESSAGE_FILE_VOICE_NAME;
	/**��Ϣ������С*/
	public String MESSAGE_FILE_SIZE;
	/**��Ϣ������ ������������ʶ������ļ� */
	public String MESSAGE_FILE_transferID;
	/**��Ϣ������ ftid=��some-id��*/
	public String MESSAGE_FILE_ftID;
	/**��Ϣ������ ����base64��ʽ*/
	public byte[] MESSAGE_FILE_DATA;
	
	/**�ش�����*/
	public String MESSAGE_FILE_VOICE_TRANSFER_COUNT;
	
	/**���ڷ���ʱ�����*/
	public String MESSAGE_FILE_SEQID;
	
	public String MESSAGE_FILE_START;
	
	public String MESSAGE_FILE_END;
	
	/**��Ϣ������״̬ 
	 * <br>���ͣ��ȴ����͵ȴ������� ���Է��ܾ�<br>�����ڷ���<br>�������<br>������ȡ��(�����򱻶�)<br>�������쳣<br>
	 * <br>���գ��ȴ�����<br>�ȴ����Լ��ܾ�<br>���ڽ���<br>�������<br>������ȡ��<br>�������쳣*/
	public byte MESSAGE_FILE_STATUS_TYPE;//
	
	/**��Ϣ�������ص�ַ*/
	public String MESSAGE_FILE_URL;//  �ļ����ص�ַ
	
	/**��Ϣ������״̬ <br>
	 * ¼����
	 * <br>¼����� ���ڷ������
	 * <br>���ⷢ�����
	 * <br>���յ�һ���������� ����<br>������� ����
	 * <br>¼����� �������ڷ���<br>���� ���ڽ���<br>���� ����ȡ��<br>���� ����ȡ����
	 * */
	public byte MESSAGE_VOICE_STATUS_TYPE;//
	
	/**���������� ���� base64��ʽ*/
	public byte[] MESSAGE_VOICE_DATA;
	
	/**����������mime */
	public String MESSAGE_VOICE_MIME;
	
	/**���������Ĵ�С */
	public String MESSAGE_VOICE_SIZE;
	
	/**�����������ص�ַ*/
	public String MESSAGE_VOICE_URL;//  
	
	/**�Ѵ���İٷֱ� 0-100*/
	public String MESSAGE_GAUGE_INDEX;
	
	/**���������ڱ��ر�����ļ�·��*/
	public String MESSAGE_VOICE_PATH;
	
	/**�͸��ļ�/�������� ��صĴ��⴫�����*/
	public FileOuterTransfer MESSAGE_FILEOUTTRANSFER;
	
	//*****************************
	// �Ự���ݽṹ
    //******************************
	 /**�Ự����  SESSION_TYPE_CHAT  SESSION_TYPE_GROUP_CHAT  SESSION_TYPE_CLUSTER_CHAT*/
	 public byte SESSION_sessionType;// �Ự����  SESSION_TYPE_CHAT  SESSION_TYPE_GROUP_CHAT  SESSION_TYPE_CLUSTER_CHAT
	 /**�Ự��ʼʱ��*/
	 public String SESSION_time;//  �Ự��ʼʱ��
	 /**�Ự��Ա���� װ����ϵ��*/
	 public Vector SESSION_chaters;// �Ự��Ա
	 /**�Ự δ������Ϣ����*/
	 public String SESSION_numOfNewMsg;// δ������Ϣ����
	 /**�Ự װ����Ϣ����*/
	 public Vector SESSION_contents;// �Ự��Ϣ����
	 /**�Ự �����һỰ ���ε��û��嵥*/
	 public Vector SESSION_BLOCK_LIST;//�����һỰ ���ε��û��嵥
	 
	 //#ifdef support_save_rms
	 /**��ʷ�Ự ������û�����Ự��*/
	 public String SESSION_Record_saveName;//��ʷ�Ự ������û�����Ự��
	 /**��ʷ�Ự �Ự��Ϣ����*/
	 public Vector SESSION_Record_contents;//�Ự��Ϣ����
	 /**��ʷ�Ự �Ự��Ϣ������*/
	 public String SESSION_Record_contents_count;//�Ự��Ϣ������
	 //#endif
	 
	//*****************************
	// ��֤����ĺ������ݽṹ
	//******************************
	 /**��֤����ĺ��� �Ƿ�����չ��Ϣ ֻ��������Ϊ@msn.cn�û�ʱ��Я����չ��Ϣ*/
	 public boolean VERIFY_isHasProfile;//�Ƿ�����չ��Ϣ  ֻ��������Ϊmsn.cn�û�ʱ��Я����չ��Ϣ
	 /**��֤����ĺ��� imid ��¼�˺�*/
	 public String VERIFY_imid;
	 /**��֤����ĺ��� �ǳ�*/
	 public String VERIFY_nickname;//�ǳ�
	 /**��֤����ĺ��� ��ʵ����*/
	 public String VERIFY_realname;//��ʵ����
	 /**��֤����ĺ��� �Ա�*/
	 public String VERIFY_sex;//�Ա�
	 /**��֤����ĺ��� ����*/
	 public String VERIFY_age;//����
	 /**��֤����ĺ��� ʡ��*/
	 public String VERIFY_province;//ʡ��
	 /**��֤����ĺ��� ��������*/
	 public String VERIFY_desc;//��������

	//*****************************
	// ������ROOM���ݽṹ
	//******************************
	// public String CHATROOM_name;//����������
	// public byte CHATROOM_chatroomFlag;//��־ NORMAL_FLAG=0x00,//δ�����������״̬  ENTER_FLAG=0x01;//�ѽ����������״̬
	// public String CHATROOM_type;//type=��fixed�̶�|custom���ơ� zwp��ֻ����wap�Ĳ�����������Ƶ����
	 
	//*****************************
	// Ƶ�����ݽṹ String jid, String name, String groupname, String[][] options, boolean iswap, boolean auth
	//******************************
	// public String CHANNEL_groupname;
	// public String[][] CHANNEL_options;
	// public boolean CHANNEL_iswap;
	// public boolean CHANNEL_auth;
	// public String CHANNEL_nickname;
	 
	//*****************************
	// �����Һ������ݽṹ
	//****************************** 
	// public String CHATROOM_FRIEND_nickname;//�ǳ�
	// public String CHATROOM_FRIEND_imid;
	// public boolean CHATROOM_FRIEND_Block=false;//���θ������Һ���
	//*****************************

	 
	//*****************************
	// ���˵�¼����
	//****************************** 
	 //public String MYLOGIN_PICAALLID;//pica���� ����9035@pica
	 //public byte MYLOGIN_selectEmailDomain;//ѡ����ʺ�
	 //public boolean MYLOGIN_isUsingCmccProxy;//�Ƿ�ʹ�ô��������
	 //public boolean MYLOGIN_isSavePassword;//�Ƿ񱣴�����
	 //public boolean MYLOGIN_isSoundContactOnline;//�Ƿ񲥷���ϵ����������
	 //public boolean MYLOGIN_isSoundMessageReceive;//�Ƿ񲥷�������Ϣ����
	 //public boolean MYLOGIN_isAutoLogin;//�Ƿ��Զ���¼

	 
	 //public String MYLOGIN_username;//�༭��������û���
	 //public String MYLOGIN_allUsername;//��������ȫ���û���
	 //public String MYLOGIN_password;//����
	 
	
	 
	 ///////////// android�ݲ�����
	 
	 //#ifdef support_head_image
	 //public boolean MYLOGIN_isReceiveContactHeadImage;//�Ƿ����ͷ��
	 //#endif
	 
	 //#ifdef support_exchange_contact_group
	// public boolean MYLOGIN_isOnlyShowOnlineContact=true;//�Ƿ������ʾ������ϵ��
	 //#endif

	 //public String MYLOGIN_chat_header_AD_ID;//���id
	 //public String MYLOGIN_login_prog_AD_ID;//���id
	 //public String MYLOGIN_roster_footer_AD_ID;//���id
	 //public String MYLOGIN_main_footer_AD_ID;//���id
	 //public String MYLOGIN_session_footer_AD_ID;//���id
	 
	 
	 //*********************
	 //ҳ�Ź�� ����ʱ��ʹ��
	 //***********************
	public byte AD_type;//��ϵ��ҳ�š���ҳ��ҳ�š��Ự�б�ҳ�� 
	public byte AD_flag; //AD_IVR_FLAG = 0x00;//IVR��� AD_WAP_FLAG = 0x01;//wap���  AD_ZWP_FLAG = 0x02;//��wap���  AD_SMS_FLAG = 0x03;//���Ź��
	public String AD_id;
	public String AD_target;
	public String AD_param0;
	public String AD_src;
	public String AD_text;
	public String AD_dbid;
	public String AD_sale;
	public Bitmap AD_image;
	

}
