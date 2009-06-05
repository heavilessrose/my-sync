#ifndef _WK_STRING_H_
#define _WK_STRING_H_

enum string_id
{
    WINKS_UI_NULL_ID = 0,
    // @@@主界面：
    WINKS_STRID_MAIN_TOURONLINE,                               // 在线体验
    WINKS_STRID_MAIN_ACTIVATE,                                 // 激活
    WINKS_STRID_MAIN_DIY,                                      // DIY
    WINKS_STRID_MAIN_MYLIBRARY,                                // 我的彩像库
    WINKS_STRID_MAIN_WINKSSTORE,                               // 彩像商城
    WINKS_STRID_MAIN_SETWINKS,                                 // 彩像定制
    WINKS_STRID_MAIN_SETTINGS,                                 // 设置
    WINKS_STRID_MAIN_SHARE,                                    // 推荐给好友

    // @@@设置界面：
    WINKS_STRID_SETTING_USEINROAMING,                          // 漫游时使用彩像
    WINKS_STRID_SETTING_NOTUSEINROAMING,                       // 漫游时不使用彩像
    WINKS_STRID_SETTING_SHOW,                                  // 展示设置
    WINKS_STRID_SETTING_ADVANCE,                               // 高级设置
    WINKS_STRID_SETTING_RESTORE,                               // 恢复默认设置
    WINKS_STRID_SETTING_BEFORECALL,                            // 接通前
    WINKS_STRID_SETTING_DURINGCALL,                            // 通话中
    WINKS_STRID_SETTING_AFTERCALL,                             // 挂断后
    WINKS_STRID_SETTING_UPDATE,                                // 更新设置
    WINKS_STRID_SETTING_UPDATEAFTERCALL,                       // 挂断后更新
    WINKS_STRID_SETTING_NOTUPDATEAFTERCALL,                    // 挂断后不更新
    WINKS_STRID_SETTING_FRIENDS,                               // 亲密好友
    WINKS_STRID_SETTING_UPDATEIMM,                             // 立即更新
    WINKS_STRID_SETTING_UPLOADCONTACTS,                        // 上传电话本
    WINKS_STRID_SETTING_SELAPN,                                // 选择接入点
    WINKS_STRID_SETTING_AUTOSTARTUP,                           // 开机自启动（已启用）
    WINKS_STRID_SETTING_NOTAUTOSTARTUP,                        // 开机自启动（已关闭）
    WINKS_STRID_SETTING_STOPSERVICE,                           // 停止服务
    WINKS_STRID_SETTING_STARTSERVICE,                          // 开启服务
    WINKS_STRID_SETTING_APN,                                   // 接入点
    WINKS_STRID_SETTING_PROXY,                                 // 代理服务器地址
    WINKS_STRID_SETTING_PROXYPORT,                             // 代理服务器端口
    WINKS_STRID_SETTING_LESSTRAFFIC,                           // 最省资费模式
    WINKS_STRID_SETTING_DATAFIRST,                             // 数据最新模式
    WINKS_STRID_SETTING_USERDEFINED,                           // 自定义
    WINKS_STRID_SETTING_DAILY,                                 // 每天：
    WINKS_STRID_SETTING_TIMES,                                 // 次

    // @@@关于界面：
    WINKS_STRID_ABOUT_CONTENT,                                 // 软件名称：彩像\n软件版本：V1.0\n开发人员：无线唯科开发组\n用户支持：086-010-58711685\nE-MAIL：service@winksi.com

	// DIY界面
	WINKS_STRID_ABOUT_WINKSNAME,							   // 彩像名称

    // @@@菜单：
    WINKS_STRID_MENU_OPTION,                                   // 选项
    WINKS_STRID_MENU_EXIT,                                     // 退出
    WINKS_STRID_MENU_OPEN,                                     // 打开
    WINKS_STRID_MENU_CHECKFORUPDATES,                          // 软件升级
    WINKS_STRID_MENU_HELP,                                     // 帮助
    WINKS_STRID_MENU_ABOUT,                                    // 关于
    WINKS_STRID_MENU_SELECT,                                   // 选择
    WINKS_STRID_MENU_CANCEL,                                   // 取消
    WINKS_STRID_MENU_OK,                                       // 确定
    WINKS_STRID_MENU_YES,                                      // 是
    WINKS_STRID_MENU_NO,                                       // 否
    WINKS_STRID_MENU_BACK,                                     // 返回
    WINKS_STRID_MENU_UPLOAD,                                   // 上传
    WINKS_STRID_MENU_ENABLE,                                   // 开启
    WINKS_STRID_MENU_DISABLE,                                  // 关闭
    WINKS_STRID_MENU_CONTACTS,                                 // 地址本
    WINKS_STRID_MENU_ADD,                                      // 添加
    WINKS_STRID_MENU_VIEWWINKS,                                // 查看彩像
    WINKS_STRID_MENU_DELETE,                                   // 删除
    WINKS_STRID_MENU_MARKUNMARK,                               // 标记/取消标记
    WINKS_STRID_MENU_MARK,                                     // 标记
    WINKS_STRID_MENU_MARKALL,                                  // 标记全部
    WINKS_STRID_MENU_UNMARK,                                   // 取消标记
    WINKS_STRID_MENU_UNMARKALL,                                // 全部取消标记
    WINKS_STRID_MENU_BUY,                                      // 购买
	WINKS_STRID_MENU_SELPIC,								   // 选择图片

    // @@@弹出对话框:
    WINKS_STRID_INFOBOX_CONNECTINGSERVER,                      // 正在连接服务器...
    WINKS_STRID_INFOBOX_ACTIVATING,                            // 正在激活彩像软件...
    WINKS_STRID_INFOBOX_CONFIRMATION_UPLCONTACTQUS,            // 为了方便您使用彩像，可以将电话本上传。电话本数据会受到严格保护。您是否要上传电话本？
    WINKS_STRID_INFOBOX_UPLOADINGCONTACTS,                     // 正在上传电话本...
    WINKS_STRID_INFOBOX_FINISHING,                             // 已完成：%d%%
    WINKS_STRID_INFOBOX_CONFIRMATION_SMS,                      // 激活过程中需要发送短信，可能会产生相关费用。您是否要继续激活？
    WINKS_STRID_INFOBOX_ACTIVATED,                             // 彩像软件已经激活。您也可以登录http://winksi.com来使用彩像服务。
    WINKS_STRID_INFOBOX_CONFIRMATION_UPDATESOFTNOW,            // 有新版本的彩像软件，是否马上升级？
    WINKS_STRID_INFOBOX_DOWNLOADINGSOFT,                       // 正在下载最新版本的软件...
    WINKS_STRID_INFOBOX_CURRENTVERSION,                        // 当前软件已是最新版本。
    WINKS_STRID_INFOBOX_CONFIRMATION_CONNECTAGAIN,             // 无法连接服务器。是否尝试再次连接？
    WINKS_STRID_INFOBOX_CHECKAPNSETTING,                       // 多次尝试均无法连接服务器。请检查网络设置。
    WINKS_STRID_INFOBOX_UPLOADINGIMG,                          // 正在上传图像...
    WINKS_STRID_INFOBOX_CONFIRMATION_SETDIYWINKS,              // 图像已经上传到服务器，正在审核。现在是否要将该图像定制给好友？
    WINKS_STRID_INFOBOX_CONFIRMATION_RESTORE,                  // 您的确要恢复默认设置吗?
    WINKS_STRID_INFOBOX_CONFIRMATION_STARTROAM,                // 在漫游时使用彩像业务可能导致较高的流量费用。您的确要开启此功能吗？
    WINKS_STRID_INFOBOX_CONFIRMATION_DELETEFRIENDS,            // 您的确要删除所选的好友吗？
    WINKS_STRID_INFOBOX_UPDATINGWINKS,                         // 正在更新好友的彩像...
    WINKS_STRID_INFOBOX_WINKSUPDATED,                          // 好友彩像已经更新。
    WINKS_STRID_INFOBOX_CONTACTSUPLOADED,                      // 电话本已经上传。
    WINKS_STRID_INFOBOX_CONFIRMATION_STOPSERVICE               // 停止服务后您将无法使用彩像业务。您的确要停止彩像服务吗？

};

#endif // #ifndef _WK_STRING_H_