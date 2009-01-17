package cn.sda.ui;

import java.util.*;

import javax.microedition.lcdui.*;
import javax.microedition.midlet.*;

import cn.sda.event.*;

/**
 *
 * @author Administrator
 */
abstract public class SDAApplication extends MIDlet {

    private static SDAApplication instance;
    protected final SDAApplication Application = this;
    private AppCanvas appCanvas = null;
    private Graphics graphics = null;
    private Display disp = Display.getDisplay(this);
    private SDAForm foucsedForm = null;
    private SDAForm mainForm = null;
    public SDAInputPanel inputPanel = new SDAInputPanel();
    private Vector dispList = new Vector();
    private SDAMessageBox msgBox;
    private TextEditor txtEditor = new TextEditor("编辑", "", 19, TextField.ANY);
    public static final int SKIN_STANDARD = 0;
    public static final int SKIN_POPULAR = 1;
    private SDASkins Skins = null;
    private int skin = SKIN_STANDARD;
    private boolean doubleBuffered = true;
    private boolean ctrl3d = false;
    private Timer runOneceTimer = new Timer();
    public SDAApplication() {
        instance = this;
        this.appCanvas = new AppCanvas();
        this.appCanvas.setFullScreenMode(true);
        this.disp.setCurrent(appCanvas);
    }

    public final void setSkin(int skin) {
        this.skin = skin;
        if (skin == SKIN_POPULAR) {
            Skins = null;
            Skins = SDASkins.CreateSkins();
        }
    }

    public final void setDoubleBuffered(boolean doubleBuffered) {
        this.doubleBuffered = doubleBuffered;
    }

    public void setCtrl3d(boolean ctrl3d) {
        this.ctrl3d = ctrl3d;
    }

    public void setMainForm(SDAForm mainForm) {
        this.mainForm = mainForm;
    }

    public final boolean isDoubleBuffered() {
        return this.doubleBuffered;
    }

    public final boolean hasPointer() {
        return this.appCanvas.hasPointerEvents();
    }

    protected final Graphics GetGraphics() {
        return this.graphics;
    }

    protected final Canvas GetCanvas() {
        return this.appCanvas;
    }

    protected SDAForm getFoucsedForm() {
        return foucsedForm;
    }
    public int getSkin() {
        return skin;
    }

    public boolean isCtrl3d() {
        return ctrl3d;
    }

    public static SDAApplication GetInstance() {
        return instance;
    }

    public void MessageBox(String Caption, String Text, int flags) {
        MessageBox(Caption, Text, flags, null);
    }

    public void MessageBox(String Caption, String Text, int flags,
                           MsgBoxCloseEvent closeEvent) {
        msgBox.closeEvents.addElement(closeEvent);
        msgBox.caption = Caption;
        msgBox.txtLabel.setText(Text);
        msgBox.button1.setVisible(false);
        msgBox.button2.setVisible(false);
        int btnFlags = flags % 10;
        switch (btnFlags) {
        case SDAConsts.MB_OK: {
            msgBox.button1.setVisible(true);
            msgBox.button1.setText("确定");
            break;
        }
        case SDAConsts.MB_OKCANCEL: {
            msgBox.button1.setVisible(true);
            msgBox.button2.setVisible(true);
            msgBox.button1.setText("确定");
            msgBox.button2.setText("取消");
            break;
        }
        case SDAConsts.MB_YES: {
            msgBox.button1.setVisible(true);
            msgBox.button1.setText("是");
            break;
        }
        case SDAConsts.MB_YESNO: {
            msgBox.button1.setVisible(true);
            msgBox.button2.setVisible(true);
            msgBox.button1.setText("是");
            msgBox.button2.setText("否");
            break;
        }
        default: {
            msgBox.button1.setVisible(true);
            msgBox.button1.setText("确定");
            break;
        }
        }
        //System.out.println("MessageBoxEnd");
        if (msgBox.button2.visible) {
            msgBox.button2.setFoucsed();
        } else if (msgBox.button1.visible) {
            msgBox.button1.setFoucsed();
        }
        msgBox.InternalShow();
    }

    protected final void ShowForm(SDAForm form) {
        int index = dispList.indexOf(form);
        //System.out.println("ShowForm index=" + index);
        if (index > -1) {
            dispList.removeElementAt(index);
        }
        dispList.addElement(form);
        foucsedForm = form;

        form.canvas = this.appCanvas;
        form.graphics = this.graphics;
        disp.setCurrent(appCanvas);
        form.Repaint();
    }

    protected final void CloseForm(SDAForm form) {
        int index = dispList.indexOf(form);
        if (index > -1) {
            dispList.removeElementAt(index);
        }
        if (!dispList.isEmpty()) {
            Object current = dispList.lastElement();
            if (current instanceof SDAForm) {
                ((SDAForm) current).Show();
            } else {
                disp.setCurrent((Displayable) current);
            }
        }else
        {
            if(mainForm!=null&&!mainForm.equals(form))
            {
                mainForm.Show();
            }
            else
            {
                Application.Exit();
            }
        }
    }

    public final void ShowDisplayable(Displayable displayable) {
        //System.out.println("ShowDisplayable");
        int index = dispList.indexOf(displayable);
        if (index > -1) {
            dispList.removeElementAt(index);
        }
        dispList.addElement(displayable);
        disp.setCurrent(displayable);
    }

    public final void CloseDisplayable(Displayable displayable) {
        //System.out.println("CloseDisplayable");
        int index = dispList.indexOf(displayable);
        if (index > -1) {
            //System.out.println("Remove displayable index=" + index);
            dispList.removeElementAt(index);
        }
        if (!dispList.isEmpty()) {
            Object current = dispList.lastElement();
            if (current instanceof SDAForm) {
                ((SDAForm) current).Show();
            } else {
                disp.setCurrent((Displayable) current);
            }
        }
    }

    protected void ShowEditor(SDABaseEdit edit) {
        txtEditor.doEdit(edit);
    }

    public final void startApp() {
        msgBox = new SDAMessageBox();
        runOneceTimer.schedule(new TimerTask() {
            public void run() {
                while (graphics == null) {
                    appCanvas.repaint();
                }
                if (foucsedForm != null) {
                    //System.out.println("startApp");
                    foucsedForm.Show();
                }
                runOneceTimer.cancel();
            }
        }, 0, 10);
    }

    public final void pauseApp() {
    }

    public final void destroyApp(boolean unconditional) {
    }

    public final void Exit() {
        this.quitApp();
    }

    public final void quitApp() {
        this.foucsedForm = null;
        this.appCanvas = null;
        this.dispList.removeAllElements();
        instance.destroyApp(true);
        instance.notifyDestroyed();
        instance = null;
    }

    class AppCanvas extends Canvas {

        public void paint(Graphics g) {
            //System.out.println("paint");
            if (graphics == null) {
                graphics = g;
                return;
            }
            if (!instance.isDoubleBuffered() && foucsedForm != null) {
                foucsedForm.graphics = g;
                foucsedForm.paint();
            }
        }

        protected void showNotify() {
            //System.out.println("showNotify");
            if (foucsedForm != null) {
                foucsedForm.paint();
            }
        }

        protected void pointerPressed(int x,
                                      int y) {
            if (foucsedForm != null) {
                foucsedForm.pointerPressed(x, y);
            }
        }

        protected void pointerReleased(int x,
                                       int y) {
            if (foucsedForm != null) {
                foucsedForm.pointerReleased(x, y);
            }
        }

        protected void pointerDragged(int x, int y) {
            if (foucsedForm != null) {
                foucsedForm.pointerDragged(x, y);
            }
        }

        protected void keyPressed(int keyCode) {
            //String keyName = getKeyName(keyCode).toUpperCase();
            if (foucsedForm != null) {
                foucsedForm.keyPressed(keyCode);
            }
        }

        protected void keyReleased(int keyCode) {
            if (foucsedForm != null) {
                foucsedForm.keyReleased(keyCode);
            }
        }
    }


    private class TextEditor extends TextBox implements CommandListener {

        private Command ExitBtn;
        private SDABaseEdit edit;

        public TextEditor(String arg0, String arg1, int arg2, int arg3) {
            super(arg0, arg1, arg2, arg3);
            ExitBtn = new Command("返回", Command.EXIT, 1);
            addCommand(ExitBtn);
            setCommandListener(this);
        }

        public void commandAction(Command c, Displayable s) {
            if (c.equals(ExitBtn)) {
                if (!edit.isReadOnly()) {
                    edit.setText(getString());
                }
                Application.CloseDisplayable(this);
                edit.form.paint();
            }
        }

        private void doEdit(SDABaseEdit edit) {
            this.edit = edit;
            this.setMaxSize(edit.getMaxLength());
            this.setString(edit.getText());
            Application.ShowDisplayable(this);
        }
    }


    class SDAMessageBox extends SDAForm {

        private int moduleResult = 1;
        private Vector closeEvents = new Vector();
        private SDALabel txtLabel = new SDALabel();
        private SDAButton button1 = new SDAButton(), button2 = new SDAButton();

        public SDAMessageBox() {
            int thisWidth = this.canvas.getWidth() / 5 * 4;
            int thisHeight = 90;
            this.setWidth(thisWidth);
            this.setHeight(thisHeight);
            this.setLeft((this.canvas.getWidth() - thisWidth) / 2);
            this.setTop((this.canvas.getHeight() - thisHeight) / 2);
            //this.setChildsLayout();
            System.out.println(this.getHeight());
            txtLabel.setAutoSize(true);
            AddControl(txtLabel);
            AddControl(button1);
            AddControl(button2);
            this.setAlignment(this, new SDABaseControl[] {txtLabel},
                              SDAConsts.amhCenter, 0);
            this.setAlignment(this, new SDABaseControl[] {txtLabel},
                              SDAConsts.amParentTopSpace, 10);
            this.setAlignment(button1, new SDABaseControl[] {button2},
                              SDAConsts.amvTop, 0);
            this.setAlignment(button1, new SDABaseControl[] {button2},
                              SDAConsts.amhSpaceEqually, 15);
            this.setAlignment(this, new SDABaseControl[] {button1, button2},
                              SDAConsts.amhInCenter, 0);

            this.setAlignment(this, new SDABaseControl[] {button1, button2},
                              SDAConsts.amParentBottomSpace, 10);

            button1.setOnClick(new NotifyEvent() {

                public void Event(SDABaseControl ctrl) {

                    moduleResult = 1;
                    if (!closeEvents.isEmpty()) {
                        Object eventObj = closeEvents.lastElement();
                        if (eventObj != null) {
                            MsgBoxCloseEvent event = (MsgBoxCloseEvent)
                                    eventObj;
                            event.Event(moduleResult);
                        }
                    }
                    msgBox.Close();
                }
            });
            button2.setOnClick(new NotifyEvent() {

                public void Event(SDABaseControl ctrl) {

                    moduleResult = 2;
                    if (!closeEvents.isEmpty()) {
                        Object eventObj = closeEvents.lastElement();
                        if (eventObj != null) {
                            MsgBoxCloseEvent event = (MsgBoxCloseEvent)
                                    eventObj;
                            event.Event(moduleResult);
                        }
                    }
                    msgBox.Close();
                }
            });
        }
    }

    public SDASkins getSkins() {
        return Skins;
    }    
}
