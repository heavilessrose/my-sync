#ifndef _WK_SCREEN_H_
#define _WK_SCREEN_H_

#define WINKS_UI_SCREEN_ID_BASE     0
//#define MAX_SCREEN_ID_NUMBER        12

enum screen_id
{
    WINKS_UI_UNACTIVATED_ID = WINKS_UI_SCREEN_ID_BASE + 1,
    WINKS_UI_MAINMENU_ID,
    WINKS_UI_ABOUT_ID,
    WINKS_UI_HELP_ID,
    WINKS_UI_DIY_ID,
    WINKS_UI_NORMALSETTING_ID,
    WINKS_UI_SHOWSETTING_ID,
    WINKS_UI_ADVANCESETTING_ID,
    WINKS_UI_UPDATESETTING_ID,
    WINKS_UI_ADDFRIENDS_ID,
    WINKS_UI_FRIENDSPREVIEW_ID,
    WINKS_UI_APN_ID,
	WINKS_UI_MYLIBRARY_ID,
	WINKS_UI_WINKSSTORE_ID,
	WINKS_UI_SETWINKS_ID,
	WINKS_UI_SHARE_ID,
	WINKS_UI_LOADING_ID,
    //WINKS_UI_CCSHOW_ID,
    MAX_SCREEN_ID_NUMBER
};

// UnActivated Screen
int Winks_UI_Enter_UnActivated(int Opcode);
int Winks_UI_Exit_UnActivated(Winks_Widget_ScnLayer_s** ppScnLayer, int Option, unsigned long wParam, unsigned long lParam);

// MainMenu Screen
int Winks_UI_Enter_MainMenu(int Opcode);
int Winks_UI_Exit_MainMenu(Winks_Widget_ScnLayer_s** ppScnLayer, int Option, unsigned long wParam, unsigned long lParam);

// About Screen
int Winks_UI_Enter_About(int Opcode);
int Winks_UI_Exit_About(Winks_Widget_ScnLayer_s** ppScnLayer, int Option, unsigned long wParam, unsigned long lParam);

// Help Screen
int Winks_UI_Enter_Help(int Opcode);
int Winks_UI_Exit_Help(Winks_Widget_ScnLayer_s** ppScnLayer, int Option, unsigned long wParam, unsigned long lParam);

// Diy Screen
int Winks_UI_Enter_Diy(int Opcode);
int Winks_UI_Exit_Diy(Winks_Widget_ScnLayer_s** ppScnLayer, int Option, unsigned long wParam, unsigned long lParam);

// NormalSetting Screen
int Winks_UI_Enter_NormalSetting(int Opcode);
int Winks_UI_Exit_NormalSetting(Winks_Widget_ScnLayer_s** ppScnLayer, int Option, unsigned long wParam, unsigned long lParam);

// ShowSetting Screen
int Winks_UI_Enter_ShowSetting(int Opcode);
int Winks_UI_Exit_ShowSetting(Winks_Widget_ScnLayer_s** ppScnLayer, int Option, unsigned long wParam, unsigned long lParam);

// AdvanceSetting Screen
int Winks_UI_Enter_AdvanceSetting(int Opcode);
int Winks_UI_Exit_AdvanceSetting(Winks_Widget_ScnLayer_s** ppScnLayer, int Option, unsigned long wParam, unsigned long lParam);

// UpdateSetting Screen
int Winks_UI_Enter_UpdateSetting(int Opcode);
int Winks_UI_Exit_UpdateSetting(Winks_Widget_ScnLayer_s** ppScnLayer, int Option, unsigned long wParam, unsigned long lParam);

// AddFriends Screen
int Winks_UI_Enter_AddFriends(int Opcode);
int Winks_UI_Exit_AddFriends(Winks_Widget_ScnLayer_s** ppScnLayer, int Option, unsigned long wParam, unsigned long lParam);

// FriendsPreview Screen
int Winks_UI_Enter_FriendsPreview(int Opcode);
int Winks_UI_Exit_FriendsPreview(Winks_Widget_ScnLayer_s** ppScnLayer, int Option, unsigned long wParam, unsigned long lParam);

// Apn Screen
int Winks_UI_Enter_Apn(int Opcode);
int Winks_UI_Exit_Apn(Winks_Widget_ScnLayer_s** ppScnLayer, int Option, unsigned long wParam, unsigned long lParam);

// MyLibrary Screen
int Winks_UI_Enter_MyLibrary(int Opcode);
int Winks_UI_Exit_MyLibrary(Winks_Widget_ScnLayer_s** ppScnLayer, int Option, unsigned long wParam, unsigned long lParam);

// WinksStore Screen
int Winks_UI_Enter_WinksStore(int Opcode);
int Winks_UI_Exit_WinksStore(Winks_Widget_ScnLayer_s** ppScnLayer, int Option, unsigned long wParam, unsigned long lParam);

// SetWinks Screen
int Winks_UI_Enter_SetWinks(int Opcode);
int Winks_UI_Exit_SetWinks(Winks_Widget_ScnLayer_s** ppScnLayer, int Option, unsigned long wParam, unsigned long lParam);

// Share Screen
int Winks_UI_Enter_Share(int Opcode);
int Winks_UI_Exit_Share(Winks_Widget_ScnLayer_s** ppScnLayer, int Option, unsigned long wParam, unsigned long lParam);

// Loading Screen
int Winks_UI_Enter_Loading(int Opcode);
int Winks_UI_Exit_Loading(Winks_Widget_ScnLayer_s** ppScnLayer, int Option, unsigned long wParam, unsigned long lParam);


#endif // #ifndef _WK_SCREEN_H_