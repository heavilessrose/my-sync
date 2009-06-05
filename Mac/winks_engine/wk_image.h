#ifndef _WK_IMAGE_H_
#define _WK_IMAGE_H_

enum image_id
{
    WINKS_IMGID_NULL_ID = 0,
    // δ����ҳ��
    WINKS_IMGID_UNACTIVATED_BTN,                                    // δ����ҳ�水ť����
    WINKS_IMGID_UNACTIVATED_BTNHL,                                  // δ����ҳ�水ť����
    WINKS_IMGID_UNACTIVATED_PICFRM,                                 // δ����ҳ��չʾͼƬ�߿�
    // ��ҳ��
    WINKS_IMGID_MAINMENU_ICON1,                                     // ������ͼ��1
    WINKS_IMGID_MAINMENU_ICON2,                                     // ������ͼ��2
    WINKS_IMGID_MAINMENU_ICON3,                                     // ������ͼ��3
    WINKS_IMGID_MAINMENU_ICON4,                                     // ������ͼ��4
    WINKS_IMGID_MAINMENU_ICON5,                                     // ������ͼ��5
    WINKS_IMGID_MAINMENU_ICON6,                                     // ������ͼ��6
    WINKS_IMGID_MAINMENU_HLBKG,                                     // ���������ͼ�걳��
    // ��ͨ����ҳ��
    WINKS_IMGID_NORMALSETTING_ITEMBKG,                              // ��ͨ�����б����
    WINKS_IMGID_NORMALSETTING_ITEMHL,                               // ��ͨ�����б����������
    WINKS_IMGID_NORMALSETTING_ICON1,                                // ��ͨ����ͼ��1
    WINKS_IMGID_NORMALSETTING_ICON2,                                // ��ͨ����ͼ��2
    WINKS_IMGID_NORMALSETTING_ICON3,                                // ��ͨ����ͼ��3
    WINKS_IMGID_NORMALSETTING_ICON4,                                // ��ͨ����ͼ��4
    // �߼�����ҳ��
    WINKS_IMGID_ADVANCESETTING_GPITEMBKG,                           // �߼��������б����
	WINKS_IMGID_ADVANCESETTING_GPITEMBKG2,							// �߼��������б����2
	WINKS_IMGID_ADVANCESETTING_GPITEMBKG3,							// �߼��������б����3
//  WINKS_IMGID_ADVANCESETTING_GPITEMHL,                            // �߼��������б����������
    WINKS_IMGID_ADVANCESETTING_ITEMBKG,                             // �߼������б����
    WINKS_IMGID_ADVANCESETTING_ITEMHL,                              // �߼������б����������
    // ��������ҳ��
    WINKS_IMGID_UPDATESETTING_ITEMHL,                               // ���������б����������
    WINKS_IMGID_UPDATESETTING_SELBLOCK,                             // �������û���
    WINKS_IMGID_UPDATESETTING_GLIDEPOLE,                            // �������û���
    WINKS_IMGID_UPDATESETTING_GLIDEPOLEHL,                          // �������ø�������
    // ���ܺ���ҳ��
	WINKS_IMGID_FRIENDSETTING_ITEM,									// ���ܺ����б����
    WINKS_IMGID_FRIENDSETTING_ITEMHL,                               // ���ܺ����б����������
    
    // ͨ��ͼƬ
    WINKS_IMGID_PREVIEWWINKS_PICFRM,                                // ����Ԥ���߿�
    // �����Ի���
    WINKS_IMGID_POPUPDLG_BKG,                                       // �����Ի��򱳾�
    WINKS_IMGID_POPUPDLG_ICONSUCCESS,                               // �����Ի���ɹ�ͼ��
    WINKS_IMGID_POPUPDLG_ICONERROR,                                 // �����Ի������ͼ��
    WINKS_IMGID_POPUPDLG_ICONWAITING1,                              // �����Ի���ȴ�ͼ��1
    WINKS_IMGID_POPUPDLG_ICONWAITING2,                              // �����Ի���ȴ�ͼ��2
    WINKS_IMGID_POPUPDLG_ICONWAITING3,                              // �����Ի���ȴ�ͼ��3
    WINKS_IMGID_POPUPDLG_ICONWAITING4,                              // �����Ի���ȴ�ͼ��4
    WINKS_IMGID_POPUPDLG_ICONWAITING5,                              // �����Ի���ȴ�ͼ��5
    WINKS_IMGID_POPUPDLG_ICONWAITING6,                              // �����Ի���ȴ�ͼ��6
    WINKS_IMGID_POPUPDLG_ICONWAITING7,                              // �����Ի���ȴ�ͼ��7
    WINKS_IMGID_POPUPDLG_ICONWAITING8,                              // �����Ի���ȴ�ͼ��8
    WINKS_IMGID_POPUPDLG_ICONWAITING9,                              // �����Ի���ȴ�ͼ��9
    WINKS_IMGID_POPUPDLG_ICONWAITING10,                             // �����Ի���ȴ�ͼ��10
    WINKS_IMGID_POPUPDLG_ICONWAITING11,                             // �����Ի���ȴ�ͼ��11
    WINKS_IMGID_POPUPDLG_ICONWAITING12,                             // �����Ի���ȴ�ͼ��12
    // Tap�ؼ�
    WINKS_IMGID_TAP_CURTAPBKG,                                      // Tap�ؼ���ǰѡ�еı�ǩ����
    WINKS_IMGID_TAP_CURTAPHL,                                       // Tap�ؼ���ǰѡ�еı�ǩ��������
    WINKS_IMGID_TAP_ICON1,                                          // Tap�ؼ�ͼ��1
    WINKS_IMGID_TAP_ICON2,                                          // Tap�ؼ�ͼ��2
    WINKS_IMGID_TAP_ICON3,                                          // Tap�ؼ�ͼ��3
    WINKS_IMGID_TAP_ICON4,                                          // Tap�ؼ�ͼ��4
    WINKS_IMGID_TAP_ICON5,                                          // Tap�ؼ�ͼ��5
    WINKS_IMGID_TAP_ICON6,                                          // Tap�ؼ�ͼ��6
    // ��ͨTitle�ؼ�
    WINKS_IMGID_NLTITLE_BLACK_PIC,                                  // ��-��ͨ��������ȫ 236*31��
    WINKS_IMGID_NLTITLE_BLACK_PICL1,                                // ��-��ͨ����������1 68*31��
    WINKS_IMGID_NLTITLE_BLACK_PICL2,                                // ��-��ͨ����������2 91*31��
    WINKS_IMGID_NLTITLE_BLACK_PICL3,                                // ��-��ͨ����������3 118*31��
    WINKS_IMGID_NLTITLE_BLACK_PICR1,                                // ��-��ͨ����������1 168*31��
    WINKS_IMGID_NLTITLE_BLACK_PICR2,                                // ��-��ͨ����������2 145*31��
    WINKS_IMGID_NLTITLE_BLACK_PICR3,                                // ��-��ͨ����������3 118*31��
    WINKS_IMGID_NLTITLE_BLUE_PIC,                                   // ��-��ͨ��������ȫ 236*31�� 
    WINKS_IMGID_NLTITLE_BLUE_PICL1,                                 // ��-��ͨ����������1 68*31�� 
    WINKS_IMGID_NLTITLE_BLUE_PICL2,                                 // ��-��ͨ����������2 91*31�� 
    WINKS_IMGID_NLTITLE_BLUE_PICL3,                                 // ��-��ͨ����������3 118*31��
    WINKS_IMGID_NLTITLE_BLUE_PICR1,                                 // ��-��ͨ����������1 168*31��
    WINKS_IMGID_NLTITLE_BLUE_PICR2,                                 // ��-��ͨ����������2 145*31��
    WINKS_IMGID_NLTITLE_BLUE_PICR3,                                 // ��-��ͨ����������3 118*31��
    WINKS_IMGID_NLTITLE_HL_PIC,                                     // ����-��ͨ��������ȫ 236*31�� 
    WINKS_IMGID_NLTITLE_HL_PICL1,                                   // ����-��ͨ����������1 68*31�� 
    WINKS_IMGID_NLTITLE_HL_PICL2,                                   // ����-��ͨ����������2 91*31�� 
    WINKS_IMGID_NLTITLE_HL_PICL3,                                   // ����-��ͨ����������3 118*31��
    WINKS_IMGID_NLTITLE_HL_PICR1,                                   // ����-��ͨ����������1 168*31��
    WINKS_IMGID_NLTITLE_HL_PICR2,                                   // ����-��ͨ����������2 145*31��
    WINKS_IMGID_NLTITLE_HL_PICR3,                                   // ����-��ͨ����������3 118*31��

	WINKS_IMGID_NLTITLE_SHADOW,										// ��ͨ����������Ӱ��236*5��

	// ҳ��ױ�Բ������
	WINKS_IMGID_BOTTOM_PIC,											// ҳ��ױ�Բ�����֣�240*10��
    
//  WINKS_IMGID_NLTITLE_BLACK_LEFT,                                 // ��ͨTitle�ؼ���ɫ������
//  WINKS_IMGID_NLTITLE_BLACK_MID,                                  // ��ͨTitle�ؼ���ɫ������
//  WINKS_IMGID_NLTITLE_BLACK_RIGHT,                                // ��ͨTitle�ؼ���ɫ������
//  WINKS_IMGID_NLTITLE_BLUE_LEFT,                                  // ��ͨTitle�ؼ���ɫ������
//  WINKS_IMGID_NLTITLE_BLUE_MID,                                   // ��ͨTitle�ؼ���ɫ������
//  WINKS_IMGID_NLTITLE_BLUE_RIGHT,                                 // ��ͨTitle�ؼ���ɫ������
//  WINKS_IMGID_NLTITLE_HL_LEFT,                                    // ��ͨTitle�ؼ�����������
//  WINKS_IMGID_NLTITLE_HL_MID,                                     // ��ͨTitle�ؼ�����������
//  WINKS_IMGID_NLTITLE_HL_RIGHT,                                   // ��ͨTitle�ؼ�����������
//  WINKS_IMGID_NLTITLE_BOTTOM,                                     // ��ͨTitle�ؼ��ױ߿�
    // �ָ���
    WINKS_IMGID_SEPARATOR_H,                                        // �ָ��� - ����
    WINKS_IMGID_SEPARATOR_V,                                        // �ָ��� - ����
    // ������
//  WINKS_IMGID_SCROLLBAR_BLOCKTOP,                                 // ������������
//  WINKS_IMGID_SCROLLBAR_BLOCKMID,                                 // ������������
//  WINKS_IMGID_SCROLLBAR_BLOCKBOTTOM,                              // ������������
    // ��ͷ
    WINKS_IMGID_ARROW_LEFT,                                         // �����ͷ
    WINKS_IMGID_ARROW_RIGHT,                                        // ���Ҽ�ͷ
    WINKS_IMGID_ARROW_LEFTHL,                                       // ���������ͷ
    WINKS_IMGID_ARROW_RIGHTHL,                                      // �������Ҽ�ͷ
    WINKS_IMGID_ARROW_DOWN,                                         // ���¼�ͷ
    WINKS_IMGID_ARROW_DOWNHL,                                       // �������¼�ͷ
    WINKS_IMGID_ARROW_DOUBLERIGHT,                                  // ����˫��ͷ
    WINKS_IMGID_ARROW_DOUBLERIGHTHL,                                // ��������˫��ͷ
    // �ı���
    WINKS_IMGID_EDIT_PIC1,                                          // �ı������1 36*17
    WINKS_IMGID_EDIT_PIC2,                                          // �ı������2 91*17
    WINKS_IMGID_EDIT_PIC3,                                          // �ı������3 142*17
    WINKS_IMGID_EDIT_PIC4,                                          // �ı������4 153*17
//  WINKS_IMGID_EDIT_LEFT,                                          // �ı��������
//  WINKS_IMGID_EDIT_MID,                                           // �ı��������
//  WINKS_IMGID_EDIT_RIGHT,                                         // �ı��������
    // �����˵�
    WINKS_IMGID_POPUPMENU_ITEM,                                     // �����˵����
    WINKS_IMGID_POPUPMENU_ITEMHL,                                   // �����˵����������
    // ͼ��
    WINKS_IMGID_ICON_PHONEBOOKWHITE,                                // ��ɫ�绰��ͼ��
    WINKS_IMGID_ICON_PHONEBOOKBLACK,                                // ��ɫ�绰��ͼ��
    WINKS_IMGID_ICON_RIGHTMARK,                                     // �Թ�ͼ��
    WINKS_IMGID_ICON_LOCKGRAY,                                      // ��ɫ��ͼ��
    WINKS_IMGID_ICON_LOCKGREEN,                                     // ��ɫ��ͼ��
    
    WINKS_IMGID_MAXNUM
};

#endif // #ifndef _WK_IMAGE_H_