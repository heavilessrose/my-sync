package com.pica.msn;

public class UiEventListener {

	public static final int UI_EVENT_BASE =  0x100;
	
	public static final int UI_EVENT_SET_STATUS =  UI_EVENT_BASE + 1;
	public static final int UI_EVENT_SET_VCARD  =  UI_EVENT_BASE + 2;
	public static final int UI_EVENT_DELETE_ROSTER  =  UI_EVENT_BASE + 3;
	public static final int UI_EVENT_BLOCK  =  UI_EVENT_BASE + 4;
	public static final int UI_EVENT_UNBLOCK  =  UI_EVENT_BASE + 5;
	public static final int UI_EVENT_ADD_ROSTER  =  UI_EVENT_BASE + 6;
	public static final int UI_EVENT_SEND_ROSTER  =  UI_EVENT_BASE + 7;	
	public static final int UI_EVENT_EMOTION  =  UI_EVENT_BASE + 8;
	public static final int UI_EVENT_SHORTMESSAGE  =  UI_EVENT_BASE + 9;
	public static final int UI_EVENT_SET_SYSTEM  =  UI_EVENT_BASE + 10;
	public static final int UI_EVENT_DETAIL_INFO  =  UI_EVENT_BASE + 11;
	public static final int UI_EVENT_USER_LIST  =  UI_EVENT_BASE + 12;
	/**���������Ϊ��*/
	public static final int UI_EVENT_PASSWORD_NULL_ERROR  =  UI_EVENT_BASE + 13;
	public static final int UI_EVENT_SAVE_PASSWROD  =  UI_EVENT_BASE + 14;
	public static final int UI_EVENT_AUTO_LOGIN  =  UI_EVENT_BASE + 15;
	/**��˽����*/
	public static final int UI_EVENT_FIRST_TIME  =  UI_EVENT_BASE + 16;
	public static final int UI_EVENT_ERROR_USERNAME  =  UI_EVENT_BASE + 17;
	public static final int UI_EVENT_ERROR_PASSWORD  =  UI_EVENT_BASE + 18;
	public static final int UI_EVENT_ADD_CONFRIM  =  UI_EVENT_BASE + 19;
	public static final int UI_EVENT_ERROR_MESSAGE  =  UI_EVENT_BASE + 20;
	public static final int UI_EVENT_ABOUT  =  UI_EVENT_BASE + 21;
	public static final int UI_EVENT_HELP  =  UI_EVENT_BASE + 22;
	public static final int UI_EVENT_UPDATE  =  UI_EVENT_BASE + 23;
	/**ע��*/
	public static final int UI_EVENT_LOGOUT  =  UI_EVENT_BASE + 24;
	public static final int UI_EVENT_DELETE_ONE_SESSION=UI_EVENT_BASE+25;
	/**�˳�*/
	public static final int UI_EVENT_EXIT  =  UI_EVENT_BASE + 26;
	/**����*/
	public static final int UI_EVENT_INVITE = UI_EVENT_BASE + 27;
	/**�鿴������*/
	public static final int UI_EVENT_CHECK_INVITE = UI_EVENT_BASE + 28;
	/**������ʻ�Ϊ��*/

	public static final int UI_EVENT_USERNAME_NULL_ERROR  =  UI_EVENT_BASE + 29;
	

	public static final int UI_EVENT_SESSION_HELP= UI_EVENT_BASE + 30;	
	/**���������¼*/
	public static final int UI_EVENT_SAVE_HISTORY= UI_EVENT_BASE + 31;	
	/**�����¼*/
	public static final int UI_EVENT_CHECK_HISTORY= UI_EVENT_BASE + 32;
	/**��������*/
	public static final int UI_EVENT_FLOW_SETTING=UI_EVENT_BASE+33;
	/**������������ ���� ����*/
	public static final int UI_EVENT_OUTER_VOICECLIP=UI_EVENT_BASE+34;
	/**����ͷ�� �ϴ� */
	public static final int UI_EVENT_OUTER_VCARDHEAD=UI_EVENT_BASE+35;
	/**Zwp �������*/
	public static final int UI_EVENT_ZWAP_LOAD=UI_EVENT_BASE+36;
	/**����֪ͨ*/
	public static final int UI_EVENT_LOGIN_LOSE=UI_EVENT_BASE+37;
	/**�Ƿ�ɾ��ѡ�е������¼*/
	public static final int UI_EVENT_DELETE_HISTORY = UI_EVENT_BASE+38;
	/**��¼��ʱ���������������6λ*/
	public static final int UI_EVENT_LOGIN_PASSWORD_ERROR = UI_EVENT_BASE+39;
	/**��¼��ʱ�����������׺�����ʼ���ַ����ѡ���׺*/
	public static final int UI_EVENT_LOGIN_EMAIL_ERROR = UI_EVENT_BASE+40;
	/**�˺��������ط���½����ʾ*/
	public static final int UI_EVENT_LOGOUT_AVALIABLE = UI_EVENT_BASE+41;
	
	
	/** �����ע���û���Ӧ����3λ���� */
	public static final int UI_EVENT_REGISTER_USERNAME_ERROR = UI_EVENT_BASE + 42;
	/** �����ע������Ӧ����6λ���� */
	public static final int UI_EVENT_REGISTER_PASSWORD_ERROR = UI_EVENT_BASE + 43;
	/** �����ע���û�����������ĸ��ͷ */
	public static final int UI_EVENT_REGISTER_USERNAME_HEAD_ERROR = UI_EVENT_BASE + 44;
	/** ע��ʧ�ܣ����Ժ�����*/
	public static final int EVENT_REGISTER_FAILURE = UI_EVENT_BASE + 45;
	/** ע��ɹ� */
	public static final int EVENT_REGISTER_SUCCESS = UI_EVENT_BASE + 46;
	/** ע��ʧ�� */
	public static final int EVENT_REGISTER_ERROR = UI_EVENT_BASE + 47;
}
