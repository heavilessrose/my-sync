/**************************************************************************
Copyright (C) Beijing Mobile Winks Co., Ltd
All rights reserved.

This source code is part of winks software and is licensed by WINKS SOFTWARE LICENSE AGREEMENT.

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, 
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO 
EVENT SHALL ANY PERSON WHO HAS CONTRIBUTED TO OR IS THE OWNER OF ANY PART OF THIS SOFTWARE BE LIABLE 
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR 
TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF 
ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**************************************************************************
*Id: wk_string.h,v 1.00 
**************************************************************************    
* File Description
* ----------------
* 
**************************************************************************/

#ifndef _WK_STRING_H_
#define _WK_STRING_H_

enum string_id
{
    WINKS_UI_NULL_ID = 0,
    // @@@�����棺
    WINKS_STRID_MAIN_TOURONLINE,                               // ��������
    WINKS_STRID_MAIN_ACTIVATE,                                 // ����
//    WINKS_STRID_MAIN_BTNTOURONLINE,                            // ��ť��ʹ�õ����֡�                �������顱
//    WINKS_STRID_MAIN_BTNBACTIVATE,                             // ��ť��ʹ�õ����֡�                 ��  �
//    WINKS_STRID_MAIN_BTNTOURONLINE320240,                      // ��ť��ʹ�õ����֡�      �������顱
//    WINKS_STRID_MAIN_BTNBACTIVATE320240,                       // ��ť��ʹ�õ����֡�       ��  �
    WINKS_STRID_MAIN_DIY,                                      // DIY
    WINKS_STRID_MAIN_MYLIBRARY,                                // �ҵĲ����
    WINKS_STRID_MAIN_WINKSSTORE,                               // �����̳�
    WINKS_STRID_MAIN_SETWINKS,                                 // ������
    WINKS_STRID_MAIN_SETTINGS,                                 // ����
    WINKS_STRID_MAIN_SHARE,                                    // �Ƽ�������
	WINKS_STRID_MAIN_MYCOLLECT,								   // �ҵ��ղ�

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
    WINKS_STRID_SETTING_SOFTOPENON,                            // �����������������ã�
    WINKS_STRID_SETTING_SOFTOPENOFF,                           // �������������ѹرգ�
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
    WINKS_STRID_ABOUT_CONTENTHEAD,                               // ���� \n �汾��
    WINKS_STRID_ABOUT_CONTENT,                                 //  \n ����Ψ�ƣ��й������޹�˾ ��Ȩ���� \n �ͷ����ߣ� \n ��ַ��http://www.winksi.com \n 

    // DIY����
    WINKS_STRID_DIY_WINKSNAME,                                   // ��������
    WINKS_STRID_DIY_INFOMSG_INPUTNAME,                           // �������������

    // ��Ӻ��ѽ���
    WINKS_STRID_ADDFRIENDS_INPUTNUM,                           // ������绰����

    // ������ѽ���
    WINKS_STRID_SHARE_SMSCONTENT,                               // �����Ƽ�һ������������ͨ���п��Կ����Է����õĶ�������

    // ���ý���
    WINKS_STRID_DISABLE_USERDISABLED,                           // �û�������
    WINKS_STRID_ERROR_MSG,                                       // ����
    
    //����ͳ�ƽ���
    WINKS_STRID_TODAY_TRAFFIC,                                 // ����������
    WINKS_STRID_THIS_MONTH_TRAFFIC,                            // \n����������
    WINKS_STRID_LAST_MONTH_TRAFFIC,                            // \n����������
    WINKS_STRID_TOTAL_TRAFFIC,                                 // \n�ۻ�������
    WINKS_STRID_TRAFFIC_ZERO_TIME,                             // \n����ʱ�䣺
    WINKS_STRID_TRAFFIC_ONLYFORREFER,                          // \n\n\n�������ݽ����ο�

    // @@@�˵���
    WINKS_STRID_MENU_EDIT,                                       // �༭
    WINKS_STRID_MENU_SEND,                                       // ����
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
    WINKS_STRID_MENU_SELPIC,                                   // ѡ��ͼƬ
    WINKS_STRID_MENU_EDITNAME,                                   // �༭����
	WINKS_STRID_MENU_REFRESH,									// ˢ��
	WINKS_STRID_MENU_TRAFFICSTAT,                               // ����ͳ��
	WINKS_STRID_MENU_CLEANALL,                                  // ����

    // @@@�����Ի���:
	WINKS_STRID_INFOBOX_CHECKWINKS_HINT,						// �Ƿ������鿴����
	WINKS_STRID_INFOBOX_WAITING,								// ��Ⱥ�...
    WINKS_STRID_INFOBOX_ACTIVATING_NOHINT,                       // ���ڼ�����......
    WINKS_STRID_INFOBOX_CHECKUPDATE,                           // ���ڼ�����°汾�����...
    WINKS_STRID_INFOBOX_OPENWINKS,                               // ���Ĳ�������ѹرգ��Ƿ����ڿ��������
    WINKS_STRID_INFOBOX_APPERROR,                               // Ӧ�ó�������뾡����ͷ���ϵ��
    WINKS_STRID_INFOBOX_NOSIM,                                   // δ����SIM��������ʹ�ò���ҵ��
    WINKS_STRID_INFOBOX_LOADING,                               // װ����...
    WINKS_STRID_INFOBOX_TRYAGAIN,                               //  \n �Ƿ����ԣ�
    WINKS_STRID_INFOBOX_MYLIBRARY,                               // �밴ȷ��������������������ҵĲ���
    WINKS_STRID_INFOBOX_SETWINKS,                               // �밴ȷ������������������ò���չʾ��˭��
    WINKS_STRID_INFOBOX_WINKSSTORE,                               // �밴ȷ����������������鿴�������
    WINKS_STRID_INFOBOX_DIYSELPIC,                               // ������ѡ��ϲ����ͼƬ��Ϊ����
    WINKS_STRID_INFOBOX_SHARENUMFULL,                           // ÿ������Ƽ�5�����ѡ�
    WINKS_STRID_INFOBOX_USERDISABLED,                           // ���ѱ����ã��뾡����ͷ���ϵ��
    WINKS_STRID_INFOBOX_CONFIRMATION_ACTIVE,                   // ����û�м��������������ܲ���ʹ�á����������Ҫ�ķѼ��ٵ�GPRS�����Լ�����һ����Ѷ��ţ���ط�������Ӫ����ȡ����Ψ���޹أ����Ƿ� ���ڼ��������
    WINKS_STRID_INFOBOX_SHARING,                               // �����Ƽ�������...
    WINKS_STRID_INFOBOX_SHARESUCCESS,                           // �Ѿ��ɹ��Ƽ������ѡ�
    WINKS_STRID_INFOBOX_SHAREFAILED,                           // �Ƽ�ʧ�ܣ������³��ԡ�
    WINKS_STRID_INFOBOX_NUMBERADDED,                           // �ú����Ѿ���ӡ�
    WINKS_STRID_INFOBOX_NUMBERFULL,                               // ���ֻ�����10�����ѡ�
    WINKS_STRID_INFOBOX_PICSIZEOVERFLOW,                       // ��ѡ���ͼƬ����������ѡ��
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
	WINKS_STRID_INFOBOX_CHECKAPNSETTING_BREW,				   // ���������źŲ��û��������æ������ʧ�ܣ������������á�
    WINKS_STRID_INFOBOX_UPLOADINGIMG,                          // �����ϴ�ͼ��...
    WINKS_STRID_INFOBOX_CONFIRMATION_SETDIYWINKS,              // ͼ���Ѿ��ϴ�����������������ˡ������Ƿ�Ҫ����ͼ���Ƹ����ѣ�
    WINKS_STRID_INFOBOX_CONFIRMATION_RESTORE,                  // ����ȷҪ�ָ�Ĭ��������?
    WINKS_STRID_INFOBOX_CONFIRMATION_STARTROAM,                // ������ʱʹ�ò���ҵ����ܵ��½ϸߵ��������á�����ȷҪ�����˹�����
    WINKS_STRID_INFOBOX_CONFIRMATION_DELETEFRIENDS,            // ����ȷҪɾ����ѡ�ĺ�����
    WINKS_STRID_INFOBOX_UPDATINGWINKS,                         // ���ڸ��º��ѵĲ���...
    WINKS_STRID_INFOBOX_WINKSUPDATED,                          // ���Ѳ����Ѿ����¡�
    WINKS_STRID_INFOBOX_CONTACTSUPLOADED,                      // �绰���Ѿ��ϴ���
    WINKS_STRID_INFOBOX_CONFIRMATION_STOPSERVICE,              // ֹͣ����������޷�ʹ�ò���ҵ������ȷҪֹͣ���������
    WINKS_STRID_INFOBOX_SEND_SHARING,                          // �����ѷ���
    WINKS_STRID_INFOBOX_CONFIRMATION_CLEAN,                    // ȷ����������գ�
};

#endif // #ifndef _WK_STRING_H_
