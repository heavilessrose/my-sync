#ifndef _WK_STRING_H_
#define _WK_STRING_H_

enum string_id
{
    WINKS_UI_NULL_ID = 0,
    // @@@�����棺
    WINKS_STRID_MAIN_TOURONLINE,                               // ��������
    WINKS_STRID_MAIN_ACTIVATE,                                 // ����
    WINKS_STRID_MAIN_DIY,                                      // DIY
    WINKS_STRID_MAIN_MYLIBRARY,                                // �ҵĲ����
    WINKS_STRID_MAIN_WINKSSTORE,                               // �����̳�
    WINKS_STRID_MAIN_SETWINKS,                                 // ������
    WINKS_STRID_MAIN_SETTINGS,                                 // ����
    WINKS_STRID_MAIN_SHARE,                                    // �Ƽ�������

    // @@@���ý��棺
    WINKS_STRID_SETTING_USEINROAMING,                          // ����ʱʹ�ò���
    WINKS_STRID_SETTING_NOTUSEINROAMING,                       // ����ʱ��ʹ�ò���
    WINKS_STRID_SETTING_SHOW,                                  // չʾ����
    WINKS_STRID_SETTING_ADVANCE,                               // �߼�����
    WINKS_STRID_SETTING_RESTORE,                               // �ָ�Ĭ������
    WINKS_STRID_SETTING_BEFORECALL,                            // ��ͨǰ
    WINKS_STRID_SETTING_DURINGCALL,                            // ͨ����
    WINKS_STRID_SETTING_AFTERCALL,                             // �ҶϺ�
    WINKS_STRID_SETTING_UPDATE,                                // ��������
    WINKS_STRID_SETTING_UPDATEAFTERCALL,                       // �ҶϺ����
    WINKS_STRID_SETTING_NOTUPDATEAFTERCALL,                    // �ҶϺ󲻸���
    WINKS_STRID_SETTING_FRIENDS,                               // ���ܺ���
    WINKS_STRID_SETTING_UPDATEIMM,                             // ��������
    WINKS_STRID_SETTING_UPLOADCONTACTS,                        // �ϴ��绰��
    WINKS_STRID_SETTING_SELAPN,                                // ѡ������
    WINKS_STRID_SETTING_AUTOSTARTUP,                           // �����������������ã�
    WINKS_STRID_SETTING_NOTAUTOSTARTUP,                        // �������������ѹرգ�
    WINKS_STRID_SETTING_STOPSERVICE,                           // ֹͣ����
    WINKS_STRID_SETTING_STARTSERVICE,                          // ��������
    WINKS_STRID_SETTING_APN,                                   // �����
    WINKS_STRID_SETTING_PROXY,                                 // �����������ַ
    WINKS_STRID_SETTING_PROXYPORT,                             // ����������˿�
    WINKS_STRID_SETTING_LESSTRAFFIC,                           // ��ʡ�ʷ�ģʽ
    WINKS_STRID_SETTING_DATAFIRST,                             // ��������ģʽ
    WINKS_STRID_SETTING_USERDEFINED,                           // �Զ���
    WINKS_STRID_SETTING_DAILY,                                 // ÿ�죺
    WINKS_STRID_SETTING_TIMES,                                 // ��

    // @@@���ڽ��棺
    WINKS_STRID_ABOUT_CONTENT,                                 // ������ƣ�����\n����汾��V1.0\n������Ա������Ψ�ƿ�����\n�û�֧�֣�086-010-58711685\nE-MAIL��service@winksi.com

	// DIY����
	WINKS_STRID_ABOUT_WINKSNAME,							   // ��������

    // @@@�˵���
    WINKS_STRID_MENU_OPTION,                                   // ѡ��
    WINKS_STRID_MENU_EXIT,                                     // �˳�
    WINKS_STRID_MENU_OPEN,                                     // ��
    WINKS_STRID_MENU_CHECKFORUPDATES,                          // �������
    WINKS_STRID_MENU_HELP,                                     // ����
    WINKS_STRID_MENU_ABOUT,                                    // ����
    WINKS_STRID_MENU_SELECT,                                   // ѡ��
    WINKS_STRID_MENU_CANCEL,                                   // ȡ��
    WINKS_STRID_MENU_OK,                                       // ȷ��
    WINKS_STRID_MENU_YES,                                      // ��
    WINKS_STRID_MENU_NO,                                       // ��
    WINKS_STRID_MENU_BACK,                                     // ����
    WINKS_STRID_MENU_UPLOAD,                                   // �ϴ�
    WINKS_STRID_MENU_ENABLE,                                   // ����
    WINKS_STRID_MENU_DISABLE,                                  // �ر�
    WINKS_STRID_MENU_CONTACTS,                                 // ��ַ��
    WINKS_STRID_MENU_ADD,                                      // ���
    WINKS_STRID_MENU_VIEWWINKS,                                // �鿴����
    WINKS_STRID_MENU_DELETE,                                   // ɾ��
    WINKS_STRID_MENU_MARKUNMARK,                               // ���/ȡ�����
    WINKS_STRID_MENU_MARK,                                     // ���
    WINKS_STRID_MENU_MARKALL,                                  // ���ȫ��
    WINKS_STRID_MENU_UNMARK,                                   // ȡ�����
    WINKS_STRID_MENU_UNMARKALL,                                // ȫ��ȡ�����
    WINKS_STRID_MENU_BUY,                                      // ����
	WINKS_STRID_MENU_SELPIC,								   // ѡ��ͼƬ

    // @@@�����Ի���:
    WINKS_STRID_INFOBOX_CONNECTINGSERVER,                      // �������ӷ�����...
    WINKS_STRID_INFOBOX_ACTIVATING,                            // ���ڼ���������...
    WINKS_STRID_INFOBOX_CONFIRMATION_UPLCONTACTQUS,            // Ϊ�˷�����ʹ�ò��񣬿��Խ��绰���ϴ����绰�����ݻ��ܵ��ϸ񱣻������Ƿ�Ҫ�ϴ��绰����
    WINKS_STRID_INFOBOX_UPLOADINGCONTACTS,                     // �����ϴ��绰��...
    WINKS_STRID_INFOBOX_FINISHING,                             // ����ɣ�%d%%
    WINKS_STRID_INFOBOX_CONFIRMATION_SMS,                      // �����������Ҫ���Ͷ��ţ����ܻ������ط��á����Ƿ�Ҫ�������
    WINKS_STRID_INFOBOX_ACTIVATED,                             // ��������Ѿ������Ҳ���Ե�¼http://winksi.com��ʹ�ò������
    WINKS_STRID_INFOBOX_CONFIRMATION_UPDATESOFTNOW,            // ���°汾�Ĳ���������Ƿ�����������
    WINKS_STRID_INFOBOX_DOWNLOADINGSOFT,                       // �����������°汾�����...
    WINKS_STRID_INFOBOX_CURRENTVERSION,                        // ��ǰ����������°汾��
    WINKS_STRID_INFOBOX_CONFIRMATION_CONNECTAGAIN,             // �޷����ӷ��������Ƿ����ٴ����ӣ�
    WINKS_STRID_INFOBOX_CHECKAPNSETTING,                       // ��γ��Ծ��޷����ӷ������������������á�
    WINKS_STRID_INFOBOX_UPLOADINGIMG,                          // �����ϴ�ͼ��...
    WINKS_STRID_INFOBOX_CONFIRMATION_SETDIYWINKS,              // ͼ���Ѿ��ϴ�����������������ˡ������Ƿ�Ҫ����ͼ���Ƹ����ѣ�
    WINKS_STRID_INFOBOX_CONFIRMATION_RESTORE,                  // ����ȷҪ�ָ�Ĭ��������?
    WINKS_STRID_INFOBOX_CONFIRMATION_STARTROAM,                // ������ʱʹ�ò���ҵ����ܵ��½ϸߵ��������á�����ȷҪ�����˹�����
    WINKS_STRID_INFOBOX_CONFIRMATION_DELETEFRIENDS,            // ����ȷҪɾ����ѡ�ĺ�����
    WINKS_STRID_INFOBOX_UPDATINGWINKS,                         // ���ڸ��º��ѵĲ���...
    WINKS_STRID_INFOBOX_WINKSUPDATED,                          // ���Ѳ����Ѿ����¡�
    WINKS_STRID_INFOBOX_CONTACTSUPLOADED,                      // �绰���Ѿ��ϴ���
    WINKS_STRID_INFOBOX_CONFIRMATION_STOPSERVICE               // ֹͣ����������޷�ʹ�ò���ҵ������ȷҪֹͣ���������

};

#endif // #ifndef _WK_STRING_H_