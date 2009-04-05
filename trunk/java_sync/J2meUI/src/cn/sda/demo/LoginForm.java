package cn.sda.demo;

import cn.sda.event.MainMenuOnButtonEvent;
import cn.sda.ui.BaseControl;
import cn.sda.ui.Bevel;
import cn.sda.ui.ButtonUI;
import cn.sda.ui.ConstsUI;
import cn.sda.ui.EditUI;
import cn.sda.ui.FormUI;
import cn.sda.ui.InputPanel;
import cn.sda.ui.LabelUI;
import cn.sda.ui.MainMenuUI;
import cn.sda.ui.PopMenuUI;

/**
 *
 * @author Administrator
 */
public class LoginForm extends FormUI {

    private Bevel panel=new Bevel();
    private LabelUI userNameLabel = new LabelUI();
    private LabelUI userPassLabel = new LabelUI();
    private EditUI userNameEdit = new EditUI();
    private EditUI userPassEdit = new EditUI();
    private MainMenuUI mainMenu=new MainMenuUI();
    private ButtonUI OkBtn=new ButtonUI();
    private ButtonUI ExitBtn=new ButtonUI();

    public LoginForm(String caption) {
        super(caption);
        panel.setDock(ConstsUI.dsFill);
        panel.setBackColor(ConstsUI.clWhite);
        panel.setCtl3d(false);
        panel.setShape(Bevel.bsSpacer);
        AddControl(panel);
        
        userNameLabel.setAutoSize(true);
        userPassLabel.setAutoSize(true);
        userNameLabel.setBackColor(ConstsUI.clWhite);
        userPassLabel.setBackColor(ConstsUI.clWhite);
        userNameEdit.setWidth(140);
        userPassEdit.setWidth(140);
        userPassEdit.setPasswordChar('*');
        userNameLabel.setText("用户名称");
        userPassLabel.setText("用户密码");        
        userNameEdit.setImeType(InputPanel.imLowerCase);
        userPassEdit.setImeType(InputPanel.imDigit);
        //主菜单
        this.mainMenu.setBackImage(((MIDlet1)Application).image3);
        this.mainMenu.setLeftMenuName("输入法");
        this.mainMenu.setRightMenuName("登录");
        this.mainMenu.setLeftMemu(this.mainMenu.getInputMemu());
        this.mainMenu.setOnMainMenuButtonEvent(new MainMenuOnButtonEvent() {

            public void Event(PopMenuUI memu, String MenuButtonCaption) {
                if(MenuButtonCaption.equals("登录")){
                    if(userNameEdit.getText().length()==0||userPassEdit.getText().length()==0){
                        Application.MessageBox("警告", "帐号或者密码不能够为空！", ConstsUI.MB_OK);
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
        panel.setAlignment(userNameLabel, userNameEdit, ConstsUI.amhSpaceEqually, 2);
        panel.setAlignment(userNameLabel, userNameEdit, ConstsUI.amvTop, 0);
        panel.setAlignment(userNameLabel, userPassLabel, ConstsUI.amvSpaceEqually, 6);
        panel.setAlignment(userNameLabel, userPassLabel, ConstsUI.amhleft, 0);
        panel.setAlignment(userPassLabel, userPassEdit, ConstsUI.amhSpaceEqually, 2);
        panel.setAlignment(userPassLabel, userPassEdit, ConstsUI.amvTop, 0);
        panel.setAlignment(userPassEdit, new BaseControl[]{OkBtn,ExitBtn}, ConstsUI.amvSpaceEqually, 6);
        panel.setAlignment(userPassEdit, OkBtn, ConstsUI.amhleft, 0);
        panel.setAlignment(OkBtn, ExitBtn, ConstsUI.amhSpaceEqually, 6);
        panel.setAlignment(OkBtn, ExitBtn, ConstsUI.amvTop, 0);
        panel.setAlignment(panel, new BaseControl[]{userNameEdit,userNameLabel,userPassEdit,userPassLabel,OkBtn,ExitBtn}, 
                ConstsUI.amhInCenter, 0);
        panel.setAlignment(panel, new BaseControl[]{userNameEdit,userNameLabel,userPassEdit,userPassLabel,OkBtn,ExitBtn}, 
                ConstsUI.amvInCenter, 0);
    }
}
