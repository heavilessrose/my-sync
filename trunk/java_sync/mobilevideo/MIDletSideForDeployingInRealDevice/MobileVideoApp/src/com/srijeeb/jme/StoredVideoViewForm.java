package com.srijeeb.jme;

import javax.microedition.lcdui.*;
import javax.microedition.media.*;
import javax.microedition.media.control.*;
import javax.microedition.midlet.MIDlet;
import java.io.*;

import com.srijeeb.dto.*;

public class StoredVideoViewForm extends Form implements CommandListener {

	private final static Command CMD_EXIT = new Command("Exit", Command.EXIT, 1);
	private final static Command CMD_OK = new Command("Ok", Command.SCREEN, 1);
	private final static Command CMD_BACK = new Command("Back", Command.SCREEN, 1);

	private MobileVideoApp parentMidlet = null;
	private Form parentForm = null;


	private Player player;
	private VideoControl videoControl;

	protected StoredVideoViewForm(String in, MobileVideoApp parentMidlet_, Form parentForm_) {
		super(in);
		this.parentMidlet = parentMidlet_;
		this.parentForm = parentForm_;
		initComponents();
	}

	public void initComponents() {

		append(JMEUtility.getImage("/images/banner.png"));
		append(getVideoId());
		addCommand(CMD_EXIT);
		addCommand(CMD_OK);
		addCommand(CMD_BACK);
		setCommandListener(this);
	}

	TextField videoId = null;
	private TextField getVideoId() {
		if ( videoId == null )
			videoId = new TextField("[Video Id] ", "", 25, TextField.ANY);
		return videoId;
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




	private void openStoredVideoFromServer() {

		try {

			DTOMediaRetrieveIn aDTOMediaRetrieveIn = new DTOMediaRetrieveIn();

			aDTOMediaRetrieveIn.mediaId = videoId.getString();

			DTOMediaRetrieveOut aDTOMediaRetrieveOut =
				HttpCommandExecuter.getInstance().executeHttp(aDTOMediaRetrieveIn);

			if ( aDTOMediaRetrieveOut != null ) {
				if ( aDTOMediaRetrieveOut.successFlag != null &&
					aDTOMediaRetrieveOut.successFlag.equals("S") &&
					aDTOMediaRetrieveOut.outByteArray != null &&
					aDTOMediaRetrieveOut.outByteArray.length > 0 ) {

					releaseResources();

					InputStream is = new
					    ByteArrayInputStream(aDTOMediaRetrieveOut.outByteArray);

					System.out.println("aDTOMediaRetrieveOut.contentType=" + aDTOMediaRetrieveOut.contentType);

					player = Manager.createPlayer(is, aDTOMediaRetrieveOut.contentType);

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
				}
				else {
					JMEUtility.showErrorAlert("ERROR", "Image fetch Unsuccessful", 4000, this, parentMidlet);
				}
			}
			else {
				JMEUtility.showErrorAlert("ERROR", "Null Return from Server", 4000, this, parentMidlet);
			}





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
			openStoredVideoFromServer();
		}
	}
}