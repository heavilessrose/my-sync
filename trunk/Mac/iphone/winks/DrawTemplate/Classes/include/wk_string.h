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
    // @@@主界面：
    WINKS_STRID_MAIN_TOURONLINE,                               // 在线体验
    WINKS_STRID_MAIN_ACTIVATE,                                 // 激活
//    WINKS_STRID_MAIN_BTNTOURONLINE,                            // 按钮上使用的文字“                在线体验”
//    WINKS_STRID_MAIN_BTNBACTIVATE,                             // 按钮上使用的文字“                 激  活”
//    WINKS_STRID_MAIN_BTNTOURONLINE320240,                      // 按钮上使用的文字“      在线体验”
//    WINKS_STRID_MAIN_BTNBACTIVATE320240,                       // 按钮上使用的文字“       激  活”
    WINKS_STRID_MAIN_DIY,                                      // DIY
    WINKS_STRID_MAIN_MYLIBRARY,                                // 我的彩像库
    WINKS_STRID_MAIN_WINKSSTORE,                               // 彩像商城
    WINKS_STRID_MAIN_SETWINKS,                                 // 彩像定制
    WINKS_STRID_MAIN_SETTINGS,                                 // 设置
    WINKS_STRID_MAIN_SHARE,                                    // 推荐给好友
	WINKS_STRID_MAIN_MYCOLLECT,								   // 我的收藏

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
    WINKS_STRID_SETTING_SOFTOPENON,                            // 开机自启动（已启用）
    WINKS_STRID_SETTING_SOFTOPENOFF,                           // 开机自启动（已关闭）
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
    WINKS_STRID_ABOUT_CONTENTHEAD,                               // 彩像 \n 版本：
    WINKS_STRID_ABOUT_CONTENT,                                 //  \n 无线唯科（中国）有限公司 版权所有 \n 客服热线： \n 网址：http://www.winksi.com \n 

    // DIY界面
    WINKS_STRID_DIY_WINKSNAME,                                   // 彩像名称
    WINKS_STRID_DIY_INFOMSG_INPUTNAME,                           // 请输入彩像名称

    // 添加好友界面
    WINKS_STRID_ADDFRIENDS_INPUTNUM,                           // 请输入电话号码

    // 邀请好友界面
    WINKS_STRID_SHARE_SMSCONTENT,                               // 给你推荐一个好玩的软件，通话中可以看到对方设置的动画……

    // 禁用界面
    WINKS_STRID_DISABLE_USERDISABLED,                           // 用户被禁用
    WINKS_STRID_ERROR_MSG,                                       // 错误
    
    //流量统计界面
    WINKS_STRID_TODAY_TRAFFIC,                                 // 本日流量：
    WINKS_STRID_THIS_MONTH_TRAFFIC,                            // \n本月流量：
    WINKS_STRID_LAST_MONTH_TRAFFIC,                            // \n上月流量：
    WINKS_STRID_TOTAL_TRAFFIC,                                 // \n累积流量：
    WINKS_STRID_TRAFFIC_ZERO_TIME,                             // \n清零时间：
    WINKS_STRID_TRAFFIC_ONLYFORREFER,                          // \n\n\n以上数据仅供参考

    // @@@菜单：
    WINKS_STRID_MENU_EDIT,                                       // 编辑
    WINKS_STRID_MENU_SEND,                                       // 发送
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
    WINKS_STRID_MENU_SELPIC,                                   // 选择图片
    WINKS_STRID_MENU_EDITNAME,                                   // 编辑名称
	WINKS_STRID_MENU_REFRESH,									// 刷新
	WINKS_STRID_MENU_TRAFFICSTAT,                               // 流量统计
	WINKS_STRID_MENU_CLEANALL,                                  // 清零

    // @@@弹出对话框:
	WINKS_STRID_INFOBOX_CHECKWINKS_HINT,						// 是否立即查看彩像？
	WINKS_STRID_INFOBOX_WAITING,								// 请等候...
    WINKS_STRID_INFOBOX_ACTIVATING_NOHINT,                       // 正在激活中......
    WINKS_STRID_INFOBOX_CHECKUPDATE,                           // 正在检查最新版本的软件...
    WINKS_STRID_INFOBOX_OPENWINKS,                               // 您的彩像软件已关闭，是否现在开启软件？
    WINKS_STRID_INFOBOX_APPERROR,                               // 应用程序错误，请尽快与客服联系。
    WINKS_STRID_INFOBOX_NOSIM,                                   // 未插入SIM卡，不能使用彩像业务。
    WINKS_STRID_INFOBOX_LOADING,                               // 装载中...
    WINKS_STRID_INFOBOX_TRYAGAIN,                               //  \n 是否重试？
    WINKS_STRID_INFOBOX_MYLIBRARY,                               // 请按确定键，打开浏览器，进入我的彩像。
    WINKS_STRID_INFOBOX_SETWINKS,                               // 请按确定键，打开浏览器，设置彩像展示给谁。
    WINKS_STRID_INFOBOX_WINKSSTORE,                               // 请按确定键，打开浏览器，查看更多彩像。
    WINKS_STRID_INFOBOX_DIYSELPIC,                               // 您可以选择喜欢的图片做为彩像。
    WINKS_STRID_INFOBOX_SHARENUMFULL,                           // 每次最多推荐5个好友。
    WINKS_STRID_INFOBOX_USERDISABLED,                           // 您已被禁用，请尽快与客服联系。
    WINKS_STRID_INFOBOX_CONFIRMATION_ACTIVE,                   // 您还没有激活软件，软件功能不能使用。激活过程需要耗费极少的GPRS流量以及发送一条免费短信（相关费用由运营商收取，与唯科无关），是否 现在激活软件？
    WINKS_STRID_INFOBOX_SHARING,                               // 正在推荐给好友...
    WINKS_STRID_INFOBOX_SHARESUCCESS,                           // 已经成功推荐给好友。
    WINKS_STRID_INFOBOX_SHAREFAILED,                           // 推荐失败，请重新尝试。
    WINKS_STRID_INFOBOX_NUMBERADDED,                           // 该号码已经添加。
    WINKS_STRID_INFOBOX_NUMBERFULL,                               // 最多只能添加10个好友。
    WINKS_STRID_INFOBOX_PICSIZEOVERFLOW,                       // 您选择的图片过大，请重新选择。
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
	WINKS_STRID_INFOBOX_CHECKAPNSETTING_BREW,				   // 由于网络信号不好或服务器繁忙，连接失败；请检查网络设置。
    WINKS_STRID_INFOBOX_UPLOADINGIMG,                          // 正在上传图像...
    WINKS_STRID_INFOBOX_CONFIRMATION_SETDIYWINKS,              // 图像已经上传到服务器，正在审核。现在是否要将该图像定制给好友？
    WINKS_STRID_INFOBOX_CONFIRMATION_RESTORE,                  // 您的确要恢复默认设置吗?
    WINKS_STRID_INFOBOX_CONFIRMATION_STARTROAM,                // 在漫游时使用彩像业务可能导致较高的流量费用。您的确要开启此功能吗？
    WINKS_STRID_INFOBOX_CONFIRMATION_DELETEFRIENDS,            // 您的确要删除所选的好友吗？
    WINKS_STRID_INFOBOX_UPDATINGWINKS,                         // 正在更新好友的彩像...
    WINKS_STRID_INFOBOX_WINKSUPDATED,                          // 好友彩像已经更新。
    WINKS_STRID_INFOBOX_CONTACTSUPLOADED,                      // 电话本已经上传。
    WINKS_STRID_INFOBOX_CONFIRMATION_STOPSERVICE,              // 停止服务后您将无法使用彩像业务。您的确要停止彩像服务吗？
    WINKS_STRID_INFOBOX_SEND_SHARING,                          // 邀请已发出
    WINKS_STRID_INFOBOX_CONFIRMATION_CLEAN,                    // 确定将数据清空？
};

#endif // #ifndef _WK_STRING_H_
