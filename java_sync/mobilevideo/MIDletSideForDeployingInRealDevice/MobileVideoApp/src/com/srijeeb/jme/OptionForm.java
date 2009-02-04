package com.srijeeb.jme;

import javax.microedition.lcdui.*;
import javax.microedition.midlet.MIDlet;
import java.io.*;

public class OptionForm extends Form implements CommandListener {

	private final static Command CMD_EXIT = new Command("Exit", Command.EXIT, 1);
	private final static Command CMD_OK = new Command("Ok", Command.SCREEN, 1);
	private final static Command CMD_BACK = new Command("Back", Command.SCREEN, 1);

	private MobileVideoApp parentMidlet = null;
	private Form parentForm = null;

	protected OptionForm(String in, MobileVideoApp parentMidlet_, Form parentForm_) {
		super(in);
		this.parentMidlet = parentMidlet_;
		this.parentForm = parentForm_;
		initComponents();
	}

	public void initComponents() {

		append(JMEUtility.getImage("/images/banner.png"));
		append(getOptions());
		addCommand(CMD_EXIT);
		addCommand(CMD_OK);
		addCommand(CMD_BACK);
		setCommandListener(this);
	}

	Image[] imageArray = {JMEUtility.getImage("/images/Icon.png"),
							JMEUtility.getImage("/images/Icon.png"),
							JMEUtility.getImage("/images/Icon.png")};

	private ChoiceGroup options = null;
	private ChoiceGroup getOptions() {
		if ( options == null )
			options = new ChoiceGroup("", Choice.EXCLUSIVE,
									  new String[]{"Open Svr Video", "Open Video by Id", "Record Video"}, imageArray);
		return options;
	}

	public void commandAction(Command c, Displayable d) {
        if (c == CMD_EXIT) {
			parentMidlet.destroyApp(true);
			parentMidlet.notifyDestroyed();
		}
		else if (c == CMD_OK) {
			//System.out.println("c == CMD_OK");
			if ( options.isSelected(0)) {

				VideoFromDirectServerForm aVideoFromDirectServerForm =
					new VideoFromDirectServerForm("",parentMidlet,this);
				parentMidlet.getDisplay().setCurrent(aVideoFromDirectServerForm);
			}
			else if ( options.isSelected(1)) {
				StoredVideoViewForm aStoredVideoViewForm =
					new StoredVideoViewForm("",parentMidlet,this);
				parentMidlet.getDisplay().setCurrent(aStoredVideoViewForm);

			}
			else if ( options.isSelected(2)) {
				VideoRecordingForm aVideoRecordingForm =
					new VideoRecordingForm("",parentMidlet,this);
				parentMidlet.getDisplay().setCurrent(aVideoRecordingForm);
			}

		}
		else if (c == CMD_BACK){
			parentMidlet.getDisplay().setCurrent(parentForm);
		}

	}
}