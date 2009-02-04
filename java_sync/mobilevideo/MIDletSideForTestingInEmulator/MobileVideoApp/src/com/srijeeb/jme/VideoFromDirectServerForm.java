package com.srijeeb.jme;

import javax.microedition.lcdui.*;
import javax.microedition.media.*;
import javax.microedition.media.control.*;
import javax.microedition.midlet.MIDlet;
import java.io.*;

public class VideoFromDirectServerForm extends Form implements CommandListener {

	private final static Command CMD_EXIT = new Command("Exit", Command.EXIT, 1);
	private final static Command CMD_OK = new Command("Ok", Command.SCREEN, 1);
	private final static Command CMD_BACK = new Command("Back", Command.SCREEN, 1);

	private MobileVideoApp parentMidlet = null;
	private Form parentForm = null;


	private Player player;
	private VideoControl videoControl;

	protected VideoFromDirectServerForm(String in, MobileVideoApp parentMidlet_, Form parentForm_) {
		super(in);
		this.parentMidlet = parentMidlet_;
		this.parentForm = parentForm_;
		initComponents();
	}

	public void initComponents() {

		append(JMEUtility.getImage("/images/banner.png"));
		append(getVideoName());
		addCommand(CMD_EXIT);
		addCommand(CMD_OK);
		addCommand(CMD_BACK);
		setCommandListener(this);
	}

	TextField videoName = null;
	private TextField getVideoName() {
		if ( videoName == null )
			videoName = new TextField("[Video Name] ", "", 25, TextField.ANY);
		return videoName;
	}

	public void commandAction(Command c, Displayable d) {
        if (c == CMD_EXIT) {
			releaseResources();
			parentMidlet.destroyApp(true);
			parentMidlet.notifyDestroyed();
		}
        if (c == CMD_BACK) {
			releaseResources();
			parentMidlet.getDisplay().setCurrent(parentForm);
		}
		else if (c == CMD_OK) {
			(new VideoOpenThread()).start();
		}

	}




	private void openVideoFromServer() {

		try {

			releaseResources();

			String fileURL = "http://" + parentMidlet.getServerAddress() + "/MMAWeb/MMAFiles/" + videoName.getString().trim();
			player = Manager.createPlayer(fileURL);

			player.realize();

			videoControl = (VideoControl)player.getControl("VideoControl");
			if ( videoControl == null ) {
				JMEUtility.showErrorAlert("ERROR", "Video not Supported!!!", 4000, this, parentMidlet);
				return;
			}

			VideoCanvas aVideoCanvas = new VideoCanvas();
			aVideoCanvas.initControls(videoControl, player);
			aVideoCanvas.addCommand(CMD_BACK);
			aVideoCanvas.addCommand(CMD_EXIT);
			aVideoCanvas.setCommandListener(this);
			parentMidlet.getDisplay().setCurrent(aVideoCanvas);

			player.start();



		}catch(Exception e) {
			e.printStackTrace();
			JMEUtility.showErrorAlert("ERROR", e.getMessage(), 4000, this, parentMidlet);
		}

	}

	private void releaseResources() {
		if ( player != null ) {
			try {
				player.stop();
				player.close();
			}catch(Exception e){}
		}
	}

	class VideoOpenThread extends Thread {

		public VideoOpenThread(){
		}

		public void run() {
			openVideoFromServer();
		}
	}
}