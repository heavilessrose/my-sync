#ifndef _WK_IMAGE_H_
#define _WK_IMAGE_H_

enum image_id
{
    WINKS_IMGID_NULL_ID = 0,
    // 未激活页面
    WINKS_IMGID_UNACTIVATED_BTN,                                    // 未激活页面按钮背景
    WINKS_IMGID_UNACTIVATED_BTNHL,                                  // 未激活页面按钮高亮
    WINKS_IMGID_UNACTIVATED_PICFRM,                                 // 未激活页面展示图片边框
    // 主页面
    WINKS_IMGID_MAINMENU_ICON1,                                     // 主界面图标1
    WINKS_IMGID_MAINMENU_ICON2,                                     // 主界面图标2
    WINKS_IMGID_MAINMENU_ICON3,                                     // 主界面图标3
    WINKS_IMGID_MAINMENU_ICON4,                                     // 主界面图标4
    WINKS_IMGID_MAINMENU_ICON5,                                     // 主界面图标5
    WINKS_IMGID_MAINMENU_ICON6,                                     // 主界面图标6
    WINKS_IMGID_MAINMENU_HLBKG,                                     // 主界面高亮图标背景
    // 普通设置页面
    WINKS_IMGID_NORMALSETTING_ITEMBKG,                              // 普通设置列表项背景
    WINKS_IMGID_NORMALSETTING_ITEMHL,                               // 普通设置列表项高亮背景
    WINKS_IMGID_NORMALSETTING_ICON1,                                // 普通设置图标1
    WINKS_IMGID_NORMALSETTING_ICON2,                                // 普通设置图标2
    WINKS_IMGID_NORMALSETTING_ICON3,                                // 普通设置图标3
    WINKS_IMGID_NORMALSETTING_ICON4,                                // 普通设置图标4
    // 高级设置页面
    WINKS_IMGID_ADVANCESETTING_GPITEMBKG,                           // 高级设置组列表项背景
	WINKS_IMGID_ADVANCESETTING_GPITEMBKG2,							// 高级设置组列表项背景2
	WINKS_IMGID_ADVANCESETTING_GPITEMBKG3,							// 高级设置组列表项背景3
//  WINKS_IMGID_ADVANCESETTING_GPITEMHL,                            // 高级设置组列表项高亮背景
    WINKS_IMGID_ADVANCESETTING_ITEMBKG,                             // 高级设置列表项背景
    WINKS_IMGID_ADVANCESETTING_ITEMHL,                              // 高级设置列表项高亮背景
    // 更新设置页面
    WINKS_IMGID_UPDATESETTING_ITEMHL,                               // 更新设置列表项高亮背景
    WINKS_IMGID_UPDATESETTING_SELBLOCK,                             // 更新设置滑块
    WINKS_IMGID_UPDATESETTING_GLIDEPOLE,                            // 更新设置滑竿
    WINKS_IMGID_UPDATESETTING_GLIDEPOLEHL,                          // 更新设置高亮滑竿
    // 亲密好友页面
	WINKS_IMGID_FRIENDSETTING_ITEM,									// 亲密好友列表项背景
    WINKS_IMGID_FRIENDSETTING_ITEMHL,                               // 亲密好友列表项高亮背景
    
    // 通用图片
    WINKS_IMGID_PREVIEWWINKS_PICFRM,                                // 彩像预览边框
    // 弹出对话框
    WINKS_IMGID_POPUPDLG_BKG,                                       // 弹出对话框背景
    WINKS_IMGID_POPUPDLG_ICONSUCCESS,                               // 弹出对话框成功图标
    WINKS_IMGID_POPUPDLG_ICONERROR,                                 // 弹出对话框错误图标
    WINKS_IMGID_POPUPDLG_ICONWAITING1,                              // 弹出对话框等待图标1
    WINKS_IMGID_POPUPDLG_ICONWAITING2,                              // 弹出对话框等待图标2
    WINKS_IMGID_POPUPDLG_ICONWAITING3,                              // 弹出对话框等待图标3
    WINKS_IMGID_POPUPDLG_ICONWAITING4,                              // 弹出对话框等待图标4
    WINKS_IMGID_POPUPDLG_ICONWAITING5,                              // 弹出对话框等待图标5
    WINKS_IMGID_POPUPDLG_ICONWAITING6,                              // 弹出对话框等待图标6
    WINKS_IMGID_POPUPDLG_ICONWAITING7,                              // 弹出对话框等待图标7
    WINKS_IMGID_POPUPDLG_ICONWAITING8,                              // 弹出对话框等待图标8
    WINKS_IMGID_POPUPDLG_ICONWAITING9,                              // 弹出对话框等待图标9
    WINKS_IMGID_POPUPDLG_ICONWAITING10,                             // 弹出对话框等待图标10
    WINKS_IMGID_POPUPDLG_ICONWAITING11,                             // 弹出对话框等待图标11
    WINKS_IMGID_POPUPDLG_ICONWAITING12,                             // 弹出对话框等待图标12
    // Tap控件
    WINKS_IMGID_TAP_CURTAPBKG,                                      // Tap控件当前选中的标签背景
    WINKS_IMGID_TAP_CURTAPHL,                                       // Tap控件当前选中的标签高亮背景
    WINKS_IMGID_TAP_ICON1,                                          // Tap控件图标1
    WINKS_IMGID_TAP_ICON2,                                          // Tap控件图标2
    WINKS_IMGID_TAP_ICON3,                                          // Tap控件图标3
    WINKS_IMGID_TAP_ICON4,                                          // Tap控件图标4
    WINKS_IMGID_TAP_ICON5,                                          // Tap控件图标5
    WINKS_IMGID_TAP_ICON6,                                          // Tap控件图标6
    // 普通Title控件
    WINKS_IMGID_NLTITLE_BLACK_PIC,                                  // 黑-普通标题栏（全 236*31）
    WINKS_IMGID_NLTITLE_BLACK_PICL1,                                // 黑-普通标题栏（左1 68*31）
    WINKS_IMGID_NLTITLE_BLACK_PICL2,                                // 黑-普通标题栏（左2 91*31）
    WINKS_IMGID_NLTITLE_BLACK_PICL3,                                // 黑-普通标题栏（左3 118*31）
    WINKS_IMGID_NLTITLE_BLACK_PICR1,                                // 黑-普通标题栏（右1 168*31）
    WINKS_IMGID_NLTITLE_BLACK_PICR2,                                // 黑-普通标题栏（右2 145*31）
    WINKS_IMGID_NLTITLE_BLACK_PICR3,                                // 黑-普通标题栏（右3 118*31）
    WINKS_IMGID_NLTITLE_BLUE_PIC,                                   // 蓝-普通标题栏（全 236*31） 
    WINKS_IMGID_NLTITLE_BLUE_PICL1,                                 // 蓝-普通标题栏（左1 68*31） 
    WINKS_IMGID_NLTITLE_BLUE_PICL2,                                 // 蓝-普通标题栏（左2 91*31） 
    WINKS_IMGID_NLTITLE_BLUE_PICL3,                                 // 蓝-普通标题栏（左3 118*31）
    WINKS_IMGID_NLTITLE_BLUE_PICR1,                                 // 蓝-普通标题栏（右1 168*31）
    WINKS_IMGID_NLTITLE_BLUE_PICR2,                                 // 蓝-普通标题栏（右2 145*31）
    WINKS_IMGID_NLTITLE_BLUE_PICR3,                                 // 蓝-普通标题栏（右3 118*31）
    WINKS_IMGID_NLTITLE_HL_PIC,                                     // 高亮-普通标题栏（全 236*31） 
    WINKS_IMGID_NLTITLE_HL_PICL1,                                   // 高亮-普通标题栏（左1 68*31） 
    WINKS_IMGID_NLTITLE_HL_PICL2,                                   // 高亮-普通标题栏（左2 91*31） 
    WINKS_IMGID_NLTITLE_HL_PICL3,                                   // 高亮-普通标题栏（左3 118*31）
    WINKS_IMGID_NLTITLE_HL_PICR1,                                   // 高亮-普通标题栏（右1 168*31）
    WINKS_IMGID_NLTITLE_HL_PICR2,                                   // 高亮-普通标题栏（右2 145*31）
    WINKS_IMGID_NLTITLE_HL_PICR3,                                   // 高亮-普通标题栏（右3 118*31）

	WINKS_IMGID_NLTITLE_SHADOW,										// 普通标题栏下阴影（236*5）

	// 页面底边圆角遮罩
	WINKS_IMGID_BOTTOM_PIC,											// 页面底边圆角遮罩（240*10）
    
//  WINKS_IMGID_NLTITLE_BLACK_LEFT,                                 // 普通Title控件黑色背景左
//  WINKS_IMGID_NLTITLE_BLACK_MID,                                  // 普通Title控件黑色背景中
//  WINKS_IMGID_NLTITLE_BLACK_RIGHT,                                // 普通Title控件黑色背景右
//  WINKS_IMGID_NLTITLE_BLUE_LEFT,                                  // 普通Title控件蓝色背景左
//  WINKS_IMGID_NLTITLE_BLUE_MID,                                   // 普通Title控件蓝色背景中
//  WINKS_IMGID_NLTITLE_BLUE_RIGHT,                                 // 普通Title控件蓝色背景右
//  WINKS_IMGID_NLTITLE_HL_LEFT,                                    // 普通Title控件高亮背景左
//  WINKS_IMGID_NLTITLE_HL_MID,                                     // 普通Title控件高亮背景中
//  WINKS_IMGID_NLTITLE_HL_RIGHT,                                   // 普通Title控件高亮背景右
//  WINKS_IMGID_NLTITLE_BOTTOM,                                     // 普通Title控件底边框
    // 分割线
    WINKS_IMGID_SEPARATOR_H,                                        // 分割线 - 横向
    WINKS_IMGID_SEPARATOR_V,                                        // 分割线 - 纵向
    // 滚动条
//  WINKS_IMGID_SCROLLBAR_BLOCKTOP,                                 // 滚动条滑块上
//  WINKS_IMGID_SCROLLBAR_BLOCKMID,                                 // 滚动条滑块中
//  WINKS_IMGID_SCROLLBAR_BLOCKBOTTOM,                              // 滚动条滑块下
    // 箭头
    WINKS_IMGID_ARROW_LEFT,                                         // 向左箭头
    WINKS_IMGID_ARROW_RIGHT,                                        // 向右箭头
    WINKS_IMGID_ARROW_LEFTHL,                                       // 高亮向左箭头
    WINKS_IMGID_ARROW_RIGHTHL,                                      // 高亮向右箭头
    WINKS_IMGID_ARROW_DOWN,                                         // 向下箭头
    WINKS_IMGID_ARROW_DOWNHL,                                       // 高亮向下箭头
    WINKS_IMGID_ARROW_DOUBLERIGHT,                                  // 向右双箭头
    WINKS_IMGID_ARROW_DOUBLERIGHTHL,                                // 高亮向右双箭头
    // 文本框
    WINKS_IMGID_EDIT_PIC1,                                          // 文本输入框1 36*17
    WINKS_IMGID_EDIT_PIC2,                                          // 文本输入框2 91*17
    WINKS_IMGID_EDIT_PIC3,                                          // 文本输入框3 142*17
    WINKS_IMGID_EDIT_PIC4,                                          // 文本输入框4 153*17
//  WINKS_IMGID_EDIT_LEFT,                                          // 文本输入框左
//  WINKS_IMGID_EDIT_MID,                                           // 文本输入框中
//  WINKS_IMGID_EDIT_RIGHT,                                         // 文本输入框右
    // 弹出菜单
    WINKS_IMGID_POPUPMENU_ITEM,                                     // 弹出菜单项背景
    WINKS_IMGID_POPUPMENU_ITEMHL,                                   // 弹出菜单项高亮背景
    // 图标
    WINKS_IMGID_ICON_PHONEBOOKWHITE,                                // 白色电话本图标
    WINKS_IMGID_ICON_PHONEBOOKBLACK,                                // 黑色电话本图标
    WINKS_IMGID_ICON_RIGHTMARK,                                     // 对勾图标
    WINKS_IMGID_ICON_LOCKGRAY,                                      // 灰色锁图标
    WINKS_IMGID_ICON_LOCKGREEN,                                     // 绿色锁图标
    
    WINKS_IMGID_MAXNUM
};

#endif // #ifndef _WK_IMAGE_H_