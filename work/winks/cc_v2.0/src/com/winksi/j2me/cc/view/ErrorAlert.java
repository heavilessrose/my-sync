package com.winksi.j2me.cc.view;

import com.winksi.j2me.cc.common.CPProperty;
import com.winksi.j2me.cc.control.GUIController;
import javax.microedition.lcdui.Alert;
import javax.microedition.lcdui.Command;
import javax.microedition.lcdui.CommandListener;
import javax.microedition.lcdui.Displayable;

/**
 *
 * @author Administrator
 */
public class ErrorAlert extends Alert implements CommandListener{
    private GUIController controller;
    private Command ok;
    public ErrorAlert(GUIController controller){
        super("");
        this.controller=controller;
        ok=new Command(CPProperty.COMMAND_OK[this.controller.getLanguage()],Command.OK,1);
        addCommand(ok);
        setCommandListener(this);
    }

    public void commandAction(Command arg0, Displayable arg1) {
        if(arg0==ok){
            if (this.getTimeout() == Alert.FOREVER) {
                controller.handleEvent(GUIController.EventID.EVENT_EXIT, null);
            }else{
                controller.handleEvent(GUIController.EventID.EVENT_CURRENT, null);
            }
        }
    }

}
