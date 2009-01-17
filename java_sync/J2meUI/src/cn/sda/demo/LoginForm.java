package cn.sda.demo;

import cn.sda.event.MainMenuOnButtonEvent;
import cn.sda.ui.SDABaseControl;
import cn.sda.ui.SDABevel;
import cn.sda.ui.SDAButton;
import cn.sda.ui.SDAConsts;
import cn.sda.ui.SDAEdit;
import cn.sda.ui.SDAForm;
import cn.sda.ui.SDAInputPanel;
import cn.sda.ui.SDALabel;
import cn.sda.ui.SDAMainMenu;
import cn.sda.ui.SDAPopMenu;

/**
 *
 * @author Administrator
 */
public class LoginForm extends SDAForm {

    private SDABevel panel=new SDABevel();
    private SDALabel userNameLabel = new SDALabel();
    private SDALabel userPassLabel = new SDALabel();
    private SDAEdit userNameEdit = new SDAEdit();
    private SDAEdit userPassEdit = new SDAEdit();
    private SDAMainMenu mainMenu=new SDAMainMenu();
    private SDAButton OkBtn=new SDAButton();
    private SDAButton ExitBtn=new SDAButton();

    public LoginForm(String caption) {
        super(caption);
        panel.setDock(SDAConsts.dsFill);
        panel.setBackColor(SDAConsts.clWhite);
        panel.setCtl3d(false);
        panel.setShape(SDABevel.bsSpacer);
        AddControl(panel);
        
        userNameLabel.setAutoSize(true);
        userPassLabel.setAutoSize(true);
        userNameLabel.setBackColor(SDAConsts.clWhite);
        userPassLabel.setBackColor(SDAConsts.clWhite);
        userNameEdit.setWidth(140);
        userPassEdit.setWidth(140);
        userPassEdit.setPasswordChar('*');
        userNameLabel.setText("用户名称");
        userPassLabel.setText("用户密码");        
        userNameEdit.setImeType(SDAInputPanel.imLowerCase);
        userPassEdit.setImeType(SDAInputPanel.imDigit);
        //主菜单
        this.mainMenu.setBackImage(((MIDlet1)Application).image3);
        this.mainMenu.setLeftMenuName("输入法");
        this.mainMenu.setRightMenuName("登录");
        this.mainMenu.setLeftMemu(this.mainMenu.getInputMemu());
        this.mainMenu.setOnMainMenuButtonEvent(new MainMenuOnButtonEvent() {

            public void Event(SDAPopMenu memu, String MenuButtonCaption) {
                if(MenuButtonCaption.equals("登录")){
                    if(userNameEdit.getText().length()==0||userPassEdit.getText().length()==0){
                        Application.MessageBox("警告", "帐号或者密码不能够为空！", SDAConsts.MB_OK);
                    }else{
                        Close();
                    }
                }
            }
        });
        this.setMainMemu(this.mainMenu);
        
        OkBtn.setText("确定");
        ExitBtn.setText("关闭");
        //加入
        panel.AddControl(userNameLabel);
        panel.AddControl(userNameEdit);
        panel.AddControl(userPassEdit);
        panel.AddControl(userPassLabel);
        panel.AddControl(OkBtn);
        panel.AddControl(ExitBtn);
        //布局
        panel.setAlignment(userNameLabel, userNameEdit, SDAConsts.amhSpaceEqually, 2);
        panel.setAlignment(userNameLabel, userNameEdit, SDAConsts.amvTop, 0);
        panel.setAlignment(userNameLabel, userPassLabel, SDAConsts.amvSpaceEqually, 6);
        panel.setAlignment(userNameLabel, userPassLabel, SDAConsts.amhleft, 0);
        panel.setAlignment(userPassLabel, userPassEdit, SDAConsts.amhSpaceEqually, 2);
        panel.setAlignment(userPassLabel, userPassEdit, SDAConsts.amvTop, 0);
        panel.setAlignment(userPassEdit, new SDABaseControl[]{OkBtn,ExitBtn}, SDAConsts.amvSpaceEqually, 6);
        panel.setAlignment(userPassEdit, OkBtn, SDAConsts.amhleft, 0);
        panel.setAlignment(OkBtn, ExitBtn, SDAConsts.amhSpaceEqually, 6);
        panel.setAlignment(OkBtn, ExitBtn, SDAConsts.amvTop, 0);
        panel.setAlignment(panel, new SDABaseControl[]{userNameEdit,userNameLabel,userPassEdit,userPassLabel,OkBtn,ExitBtn}, 
                SDAConsts.amhInCenter, 0);
        panel.setAlignment(panel, new SDABaseControl[]{userNameEdit,userNameLabel,userPassEdit,userPassLabel,OkBtn,ExitBtn}, 
                SDAConsts.amvInCenter, 0);
    }
}
