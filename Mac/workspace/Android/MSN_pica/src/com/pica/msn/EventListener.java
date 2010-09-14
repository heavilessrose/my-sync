package com.pica.msn;
public interface EventListener
{
	/**
	 * ע��ʧ��<br>
	 * �������ͣ�String ������ʾ�ı� ������Ϊ�գ�<br>
	 */
	public static final byte EVENT_ERGISTER_FAILURE = 0;
	
	/**
	 * ע��ɹ� ����ע��ɹ������Ϣҳ��<br>
	 * �������ͣ�String ��ʾ�ı�<br>
	 */
	public static final byte EVENT_REGISTER_SUCCESS=1;
	
	/**
	 * ��ʾ�ı������������ж�,���Ժ����ԡ��ص���¼ҳ��
	 * 
	 */
	public static final byte EVENT_NETWORK_FAILURE=2;
	
	/**
	 * ��ʾdispatch���ص�Notice<br>
	 * ���е�notice����String��ʽ ������noticeVector
	 */
	public static final byte EVENT_DISPATCH_NOTICE=3;
	
	/**
	 * ��ʾdispatch���ص�url ��ʾ����<br>
	 * ������jabber.updateUrl��jabber.updateVersion
	 */
	public static final byte EVENT_DISPATCH_URL=4;
	
	
	
	/**
	 * ��¼��� ��ʾ��ϵ��ҳ��
	 */
	public static final byte EVENT_LOGIN_SUCCESS=5;
	
	
	
	/**
	 * <br>
	 * �¼����壺��ȡZipwapҳ��ɹ�<br>
	 * �������ͣ�Object[]{DataInputStream,Integer}<br>
	 * �������壺ҳ������,����<br>
	 */
	public static final byte EVENT_ZIPWAP_OK = 6;

	/**
	 * <br>
	 * �¼����壺��ȡZipwapҳ��ʧ��<br>
	 * �������ͣ�Object<br>
	 * �������壺��(null)<br>
	 */
	public static final byte EVENT_ZIPWAP_ERROR = 7;
	
	/**
	 * ���͵�����ͳ�ư�<br>
	 * �������ͣ�String advist<br>
	 * */
	public static final byte EVENT_SEND_ADVIST=8;
	
	
	/**
	 * ע���ɹ� �˻ص���¼ҳ��<br>
	 * �����ͬ����ϵ�˲��� �������¼
	 * �������ͨע������ ����ʾע���ɹ�
	 * �������ͣ�NULL<br>
	 * */
	public static final byte EVENT_RECEIVE_LOGOUT=9;
	
	/**
	 * �˳��ɹ� �˳�����<br>
	 * �������ͣ�NULL<br>
	 * */
	public static final byte EVENT_RECEIVE_EXIT=10;
	
	/**
	 * ��¼��(Ҳ�����ǵ�½�ɹ���)�յ������ ������Ӧ�Ĵ�����ʾ�����˻ص���¼ҳ�棨�����Զ���¼��<br>
	 * �������ͣ�{String errorMsg,String errorCode }������ʾ�ʹ������<br>
	 * */
	public static final byte EVENT_RECEIVE_LOGIN_ERROR=11;
	
	/**
	 * �յ������ ��ʾ����<br>
	 * �������ͣ�String errorMsg������ʾ<br>
	 * */
	public static final byte EVENT_RECEIVE_OTHER_ERROR=12;
	
	
	/**
	 * ˢ����ϵ��ҳ��
	 */
	public static final byte EVENT_UPDATE_CONTACT_UI=13;
	
	/**
	 * �յ�presence type=unavailable�� ���û�����������˳���ע���������ʾ�ܳ�ʱ��û���� ΢����Ϊ���� <br>
	 * �������ͣ�null<br>
	 * ����������¼���� �����˳��������е�activity<br>
	 * */
	public static final byte EVENT_RECEIVE_MSN_LOGOUT=14;
	
	/**
	 * �յ�presence type=error <error>�� from=selectChatroomJid ���������汨��<br>
	 * �������ͣ�String errorMsg������ʾ<br>
	 * */
	public static final byte EVENT_RECEIVE_CHATROOM_ERROR=15;
	
	/**
	 * �յ�presence ���˵�����״̬�����ı�� �˳��ȴ�ҳ��<br>
	 * �������ͣ�null<br>
	 * */
	public static final byte EVENT_CHANGE_MY_STATE=16;
	
	/**
	 * ����һ�����ڽ��� �ҽ���ɹ��������� �˳�����ȴ�ҳ�� ���ֻỰҳ��
	 */
	public static final byte EVENT_ENTER_ROOM_OK=17;
	
	
	/**
	 * ���� �ж��Ƿ���msn.cn���û�
	 */
	public static final byte EVENT_SET_NEWMSNCN=18;
	
	/**
	 * �յ������������֤����������ʾ��������δ���ǵ�ǰ�ǵȴ�ҳ�棩<br>
	 * ���е���֤���� ��װ����app.verifyFriendVector��������
	 */
	public static final byte EVENT_RECEIVE_VERIFY_FRIEND=19;
	
	/**
	 * �����������֤����ȫ����ʾ�� Ҫ�˳���֤�������
	 */
	public static final byte EVENT_OVER_VERIFY_FRIEND=20;
	
	/**
	 * ���к����������֤û�д����� Ҫ��ʾ����
	 */
	public static final byte EVENT_MORE_VERIFY_FRIEND=21;
	
	
	/**
	 * �յ����������Ұ� �ж�ҳ����� ����ͻ����յ��µ��������б�󣬸����б���ʾ�����
	��ǰ����ͼΪ��WAP��ͼ��������ʾ��Ϣ���ȵ㻰���б��Ѹ��£��Ƿ����ڲ鿴������
	����û������ȷ���������л���ͼ����ϵ����ͼ��ͬʱչ�����ȵ㻰�⡱�顣			
	 */
	public static final byte EVENT_UPDATE_JOIN_CLUSTER=22;
	
	/**
	 * ���߳������� ����ҳ��
	 */
	public static final byte EVENT_KICKOFF_CHATROOM = 23;
	
	/**
	 * �������Ҽ�������� ����ҳ��
	 */
	public static final byte EVENT_BLACKLIST_CHATROOM = 24;
	

	/**
	 * ��¼��� ������û��������������ݿ�
	 */
	public static final byte EVENT_SAVE_LOGIN_USER=25;
	
	/**
	 * dispatch�������¼ ������ʾ ֱ���˵���¼ҳ�漴��
	 */
	public static final byte EVENT_DISPATCH_REFUSE_LOGIN=26;
	
	/**
	 * dispatchǿ������ <br>  
	 * ��Ϊ�Ѿ����ù������ ���������˵���¼ҳ�漴��
	 */
	public static final byte EVENT_DISPATCH_MUST_UPGRADE=27;
	
	/**
	 * <br>
	 * �¼����壺�յ��Լ��ĸ������� <br>
	 * �˳��ȴ�ҳ�� 
	 * �������ͣ�Object<br>
	 * �������壺��(null)<br>
	 */
	public static final byte EVENT_MY_VCARD_RECEIVED = 28;
	
	
	/**�Ự�����б�� ����-����-���� ˢ�²˵�*/
	public static final byte EVENT_UPDATE_SESSION_FLAG=29;
	
	/**
	 * <br>
	 * �¼����壺�յ�����Ϣ<br>
	 * �������ͣ�Object<br>
	 * �������壺��(null)<br>
	 */
	public static final byte EVENT_RECEIVED_MESSAGE  = 30;
	
	/**
	 * <br>
	 * �¼����壺������Ϣ<br>
	 * �������ͣ�Message<br>
	 */
	public static final byte EVENT_SEND_MESSAGE = 31;
	
	/**
	 * �򿪶Ի����� 
	 *
	 */
	public static final byte EVENT_OPEN_CHAT_WINDOW = 32;
	

	
	//#ifdef support_head_image
	/**
	 * �յ���ϵ�˵�ͷ�� ��Ҫˢ��<br>
	 * �������� Contact ����ϵ��
	 */
	public static final byte EVENT_RECEIVE_CONTACT_PORTRAIT=33;
	//#endif
	
	/***
	 * �����ϵ�˳ɹ� <br>
	 * �������� null<br>
	 * ���¸���ϵ�˵����ݿ�<br>
	 * �����һ������ϵ�� ����Imid �ҵ�İ���˻Ự������Ƿ��и���ϵ�� ����������ϵ�˸�������ϵ��
	 */
	public static final byte EVENT_ADD_CONTACT_OK=34;
	
	
	/***
	 * ɾ����ϵ�˳ɹ� �˳��ȴ�ҳ�� ��ʾɾ���ɹ�<br>
	 * �������� null
	 */
	public static final byte EVENT_DELETE_CONTACT_OK=35;
	
	/***
	 * ɾ������ֹ��ϵ�˳ɹ� �˳��ȴ�ҳ�� ��ʾɾ������ֹ�ɹ�<br>
	 * �������� null
	 */
	public static final byte EVENT_DELETE_BLOCK_CONTACT_OK=36;
	
	/***
	 * ��ֹ��ϵ�˳ɹ� �˳��ȴ�ҳ�� ��ʾ��ֹ�ɹ�<br>
	 * �������� String nickname ����ϵ�˵�nickname
	 */
	public static final byte EVENT_BLOCK_CONTACT_OK=37;
	
	/***
	 * �����ֹ��ϵ�˳ɹ� �˳��ȴ�ҳ�� ��ʾ�����ֹ�ɹ�<br>
	 * �������� String nickname ����ϵ�˵�nickname
	 */
	public static final byte EVENT_UNBLOCK_CONTACT_OK=38;
	
	/**
	 * �����������
	 */
	public static final byte EVENT_FLASH_COMPLETE = 39;
	
	/**
	 * �û��Լ��뿪�����ҳɹ� �����ȴ�ҳ�� ҳ��Ҫת����ϵ��ҳ��
	 */
	public static final byte EVENT_CHATROOM_LOGOUT_OK = 40;
	
	/**
	 * ������ϵ���뿪������ �����ǰҳ���������Һ����б�ҳ�� ��Ҫˢ��
	 */
	public static final byte EVENT_OTHER_FREIEND_LOGOUT_CHATROOM = 41;

	/**
	 * �û��Լ��˳������ҳɹ� �����ȴ�ҳ�� ҳ��Ҫת����ϵ��ҳ��
	 */
	public static final byte EVENT_CHATROOM_SIGNOFF_OK=42;

	/**
	 * <br>
	 * �¼����壺�յ������ҵ�����Ϣ<br>
	 * �������ͣ�Object<br>
	 * �������壺��(null)<br>
	 */
	public static final byte EVENT_CHATROOM_MESSAGE_RECEIVED = 43;
	
	/**
	 * �յ���ҳҳ�Ź�� ����Ļ�ϻ��Ƴ���<br>
	 * �������ͣ�Item �����<br>
	 */
	public static final byte EVENT_RECEIVE_MAIN_FOOTER_AD_ITEM=44;
	
	/**
	 * �յ���ϵ��ҳ�Ź�� ����Ļ�ϻ��Ƴ���<br>
	 * �������ͣ�Item �����<br>
	 */
	public static final byte EVENT_RECEIVE_ROSTER_FOOTER_AD_ITEM=45;
	
	/**
	 * �յ��Ự�б�ҳ�Ź�� ����Ļ�ϻ��Ƴ���<br>
	 * �������ͣ�Item �����<br>
	 */
	public static final byte EVENT_RECEIVE_SESSION_FOOTER_AD_ITEM=46;
	
	
	/**
	 * ������ϵ�˽��������� �����ǰҳ���������Һ����б�ҳ�� ��Ҫˢ��
	 */
	public static final byte EVENT_OTHER_FREIEND_ENTER_CHATROOM = 47;
	
	/**
	 * ��¼��ʱ ��ʾ�û���¼��ʱ3���Ӻ� �˻ص���¼ҳ��
	 * */
	public static final byte EVENT_WAITING_LOGIN_TIMEOUT=48;
	
	/***
	 * �����ȴ����� ��ʱ ���û���ʾ3��� �˳����ϼ�ҳ��
	 */
	public static final byte EVENT_WAITING_OTHER_TIMEOUT=49;

	
	//#ifdef support_save_rms
	/**
	 * �յ�addition==0  ͬ������ ��Ҫ��ʾ�û�Ȼ��ע���˳�
	 * */
	public static final byte EVENT_RECEIVE_SYNCHRONIZE_ERROR=50;
	//#endif
	
	/**�����ж� Ȼ��ˢ�»Ự����ҳ��Ĳ˵��Ͷ���<br>
	 * @param obj contact ��ǰ�ı����ϵ�˶���
	 * */
	public static final byte EVENT_UPDATE_CHAT_UI=51;
	
	/***
	 * ��ʾ �ڴ治����
	 */
	public static final byte EVENT_OUT_OF_MEMORY=52;
	
	/**��������Ϣ ������Ϣ����*/
	public static final byte EVENT_SOUND_NEW_MESSENGE=53;
	
	/**��¼�ɹ� ��ʼ���������� 60�뷢��һ���ո�*/
	public static final byte EVENT_SEND_HAERT=54;
	
	/**dispatch���ӳɹ� ��¼�ȴ�ҳ���ɲ��ҷ�����->���ӷ�����*/
	public static final byte EVENT_RECEIVE_DISPATCH_OK=55;
	
	/**��¼Pica�ɹ� ��ʼ��¼msn ��¼�ȴ�ҳ�������ӷ�����->��ȡ�������ϵ�*/
	public static final byte EVENT_RECEIVE_LOGIN_PICA_OK=56;
	
	/**��������ϵ�� ����ϵ����������*/
	public static final byte EVENT_SOUND_NEW_ONLINE=57;
	
	/**����������Ϣ ������Ϣ���� ��������Ч��*/
	public static final byte EVENT_SOUND_NEW_NUDGE=58;
	
	/**������һ���Ự ��ӵ��б���ȥ*/
	public static final byte EVENT_ADD_NEW_SESSION=59;
	
	/**ˢ����������ػ�*/
	public static final byte EVENT_UPDATE_INPRESEA_UI=60;
	
	/**�رյ�ǰ��activity*/
	public static final byte EVENT_FINISH_ACTIVITY=61;
	
	/**�رյ�ǰ��activity ����login activity*/
	public static final byte EVENT_CLOSE_TO_LOGIN=62;
	
	/**��ʾ��Ϣ Object<br>
	 * �������ͣ�String text ��ʾ�ı�<br>
	 * */
	public static final byte EVENT_SHOW_INFO=63;
	
//	/**��ʾ��Ϣ Object[]<br>
//	 * �������ͣ�String title �����ı�<br>
//	 * �������ͣ�String text ��ʾ�ı�<br>
//	 * �������ͣ�String leftCommand ������ı� <br>
//	 * �������ͣ�String rightCommand ������ı� <br>
//	 * */
//	public static final byte EVNET_SHOW_DIALOG=64;
	
	
	/**
	 * ��¼���յ������ ������Ӧ�Ĵ�����ʾ�����˻ص���¼ҳ�棨�����Զ���¼��<br>
	 * �������ͣ�{String errorMsg,String errorCode }������ʾ�ʹ������<br>
	 * */
	public static final byte EVENT_RECEIVE_LOGIN_ERROR_LOGINMONITOR=65;
	
	/**
	 *�յ������ ������Ӧ�Ĵ�����ʾ�����˻ص���¼ҳ�棨�����Զ���¼��<br>
	 * �������ͣ�{String errorMsg,String errorCode }������ʾ�ʹ������<br>
	 * */
	public static final byte EVENT_RECEIVE_LOGIN_ERROR_MAIN=66;
	
	/***
	 * ��loginmonitor �����ı���ʱ��
	 * ��������:String text
	 */
	public static final byte EVENT_LOGIN_MONITOR_SETTEXT=67;
	
	/**�رն���ҳ��*/
	public static final byte EVENT_STOP_MONITOR=68;
	/**ˢ��main activity*/
	public static final int EVENT_ROSTER  =  69;
	public static final int EVENT_SESSION  =  70;	
	
	/**
	 * �յ������ļ������� ����ѡ��ҳ��
	 */
	public static final byte EVENT_RECEIVE_FILE_INVITE=71;
	
	
	/**
	 * <br>
	 * �¼����壺����һ����Ϣ�����״̬<br>
	 * �������ͣ�Object<br>
	 * �������壺��(null)<br>
	 */
	public static final byte EVENT_UPDATE_MESSAGE  = 72;
	
	/**
	 * <br>
	 * �¼����壺����һ����Ϣ�Ĵ���ָʾ��<br>
	 * �������ͣ�Object<br>
	 * �������壺��(null)<br>
	 */
	public static final byte EVENT_UPDATE_GAUGE_MESSAGE  = 73;
	
	public static final int EVENT_RECORD_COMPLETE  =  74;
	
	/**ˢ��msnС�˶���*/
	public static final byte EVENT_UPDATE_MONITOR_UI=75;


	/**����ͼƬ�ļ���Ϊͷ��*/
	public static final byte EVENT_SEND_VCARDHEAD_I=76;
	
	/**������Ƭ��Ϊͷ��*/
	public static final byte EVENT_SEND_VCARDHEAD_P=77;	

	/**���´����ϴ�ͷ��Ĺ�������ֵ*/
	public static final byte EVENT_UPDATE_OUTER_HEAD_GAUGE=78;
	/**���´����ϴ�ͷ��Ĺ�������״̬ ������ȥ�������� ���� ȡ�� ���쳣֮���*/
	public static final byte EVENT_UPDATE_OUTER_HEAD_STATUS=79;
	
	/**�Ự�������б�� ˢ�¶���*/
	public static final byte EVENT_UPDATE_SESSION_TOP_NAME=80;
	
	/**��ĳһ����ϵ�˽������ݿ���� ����/����*/
	//public static final byte EVENT_ADD_ONE_CONTACT_TO_DATABASE=81;
	
	/**��ĳһ����ϵ�˽������ݿ���� ɾ��*/
	//public static final byte EVENT_DELETE_ONE_CONTACT_TO_DATABASE=82;

	/**ÿ5�����ж�һ���Ƿ���Ҫ������ϵ���б�*/
	//public static final byte EVENT_SAVE_CONTACT_LIST=81;
	
	/**������顢��ϵ�����ݵ��ļ���*/
	public static final byte EVENT_SAVE_CONTACT_DATA=84;
	
	/**�յ���ϵ��״̬�ı��*/
	public static final byte EVENT_RECEIVE_CONTACT_CHANGE_STATE=85;	
	
	/**�յ���ϵ�˰�*/
	public static final byte EVENT_RECEIVE_ROSTER=86;
	
	/**�յ������*/
	public static final byte EVENT_RECEIVE_GROUP=87;
	
	/**�յ����������*/
	public static final byte EVENT_RECEIVE_GROUP_COMPLETE=88;
	
	/**����ڴ��������ϵ�˷������ϵ�˵��¼�*/
	public static final byte EVENT_RECEIVE_ADDITION_EQUALS_ZERO=89;
	
	/**������ʾ������ϵ�˵��¼�*/
	public static final byte EVENT_ROSTER_GROUP_BY_NORMAL=90;
	/**ˢ��main activity*/
	public static final byte EVENT_ZWP  =  91;
	/**Zwp ���ؽ���*/
	public static final byte EVENT_ZWP_LOAD  =  92;
	/**Zwp ȡ�����ؽ���*/
	public static final byte EVENT_ZWP_CANCEL  =  93;
	/**���� ���ҳ��*/
	public static final byte EVENT_AD_UPDATA  =  94;
	
	public static final byte OPEN_CHAT_FROM_SEARCH_ROSTER = 95;

	//////////////////���Ӱ�״̬������
	/**��״̬����*/
	public static final byte EVENT_ROSTER_GROUP_BY_STATE = 96;
	/**�����Ƿ���ʾ������ϵ��*/
	public static final byte EVENT_SET_IS_SHOW_ONLINE = 97;
	
	/**MyItem ��������logo��������*/
	public static final byte EVENT_MY_ITEM_LOGO = 98;
	
	/**MyItem ��������info��������*/
	public static final byte EVENT_MY_ITEM_INFO = 99;
	
	/**����ȡ��ͷ��*/
	public static final byte EVENT_UPDATE_PORTRAIT_HASH = 100;
	
	/**�����������¼�*/
	public static final byte EVENT_MAIN_AD_CLICK = 101;
	/**
	 * �¼�����ʱ���ص��ķ���<br>
	 * @param eventType �¼�����
	 * @param obj �¼���ز������������ͺ��¼��������
	 */
	public void eventAction(byte eventType,Object obj);
	
	
}