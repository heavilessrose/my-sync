package cn.sda.demo;

import javax.microedition.lcdui.*;
import cn.sda.ui.*;


/**
 * @author not attributable
 * @version 1.0
 */
public class MIDlet1 extends MainApp {
    public MainForm mainform;
    public LoginForm loginForm;
    //图片
    ImageListUI imageList=new ImageListUI();
    public Image image1=null;
    public Image image2=null;
    public Image image3=null;
    public MIDlet1() {
        //装载资源
        imageList.addImage("/cn/sda/demo/suo.png");
        imageList.addImage("/cn/sda/demo/pai.png");
        imageList.addImage("/cn/sda/demo/toolbar.png");
        image1 = imageList.getImage(0);
        image2 = imageList.getImage(1);
        image3 = imageList.getImage(2);
        mainform=new MainForm();
        loginForm=new LoginForm("登录");
        mainform.setShowCaption(true);
        mainform.setCaption("J2ME-DEMO设计");
        Application.setMainForm(mainform);
        loginForm.Show();
    }
}

