package com.srijeeb.jme;

import javax.microedition.lcdui.*;
import javax.microedition.media.*;
import javax.microedition.media.control.*;
import javax.microedition.midlet.MIDlet;
import java.io.*;
import com.srijeeb.dto.*;

public class VideoRecordingForm extends Form
	implements CommandListener, PlayerListener {

	private final static Command CMD_EXIT = new Command("Exit", Command.EXIT, 1);
	private final static Command CMD_BACK = new Command("Back", Command.SCREEN, 1);
	private final static Command CMD_RECORD = new Command("Record", Command.SCREEN, 1);
	private final static Command CMD_STOP_RECORD = new Command("Stop Record", Command.SCREEN, 1);
	private final static Command CMD_SHOW_RECORDED = new Command("Show Recorded", Command.SCREEN, 1);
	private final static Command CMD_MUTE = new Command("Mute", Command.SCREEN, 1);
	private final static Command CMD_UNMUTE = new Command("UnMute", Command.SCREEN, 1);
	private final static Command CMD_HIGHEST_VOLUME = new Command("Highest Vol", Command.SCREEN, 1);
	private final static Command CMD_MEDIUM_VOLUME = new Command("Medium Vol", Command.SCREEN, 1);
	private final static Command CMD_UPLOAD_TO_SERVER = new Command("Upload to Server", Command.SCREEN, 1);

	private Player player;

	private VideoControl videoControl;
	private RecordControl recordControl;
	private VolumeControl volumeControl;

	private VideoCanvas aVideoCanvas;

	String contentType = null;
	ByteArrayOutputStream output = null;

	private short currentState = -1;
	private static final short CUSTOM_SHOW_RECORDED = 1;
	private static final short CUSTOM_OK_TO_RECORD = 2;

	VideoRecordingThread videoRecordThread = null;

	private MobileVideoApp parentMidlet = null;
	private VideoRecordingForm self = null;
	private Form parentForm = null;

	protected VideoRecordingForm(String in, MobileVideoApp parentMidlet_, Form parentForm_) {
		super(in);
		this.parentMidlet = parentMidlet_;
		this.parentForm = parentForm_;
		self = this;
		initComponents();
	}

	public void initComponents() {

		addCommand(CMD_EXIT);
		addCommand(CMD_BACK);
		setCommandListener(this);

		(new CameraThread()).start();
	}


	public void commandAction(Command c, Displayable d) {
        if (c == CMD_EXIT) {
			parentMidlet.destroyApp(true);
			parentMidlet.notifyDestroyed();
		}
        else if (c == CMD_BACK) {
			releaseResources();
			parentMidlet.getDisplay().setCurrent(parentForm);
		}
		else if (c == CMD_RECORD) {
			videoRecordThread = new VideoRecordingThread();
			videoRecordThread.start();
		}
		else if (c == CMD_STOP_RECORD) {
			videoRecordThread.stopRecord();
			aVideoCanvas.addCommand(CMD_UPLOAD_TO_SERVER);
		}
		else if (c == CMD_SHOW_RECORDED) {
			currentState = CUSTOM_SHOW_RECORDED;
			aVideoCanvas.addCommand(CMD_MUTE);
			aVideoCanvas.addCommand(CMD_HIGHEST_VOLUME);
			aVideoCanvas.addCommand(CMD_MEDIUM_VOLUME);
			aVideoCanvas.removeCommand(CMD_SHOW_RECORDED);
			( new VideoViewerThread()).start();
		}
		else if (c == CMD_MUTE) {
			if ( volumeControl != null ) {
				if ( !volumeControl.isMuted() ) {
					volumeControl.setMute(true);
					aVideoCanvas.removeCommand(CMD_MUTE);
					aVideoCanvas.addCommand(CMD_UNMUTE);
				}
			}
			else {
				JMEUtility.showErrorAlert("ERROR", "NOT ABLE TO GET VOLUME CONTROL", 4000, this, parentMidlet);
			}
		}
		else if (c == CMD_UNMUTE) {
			if ( volumeControl != null ) {
				if ( volumeControl.isMuted() ) {
					volumeControl.setMute(false);
					aVideoCanvas.removeCommand(CMD_UNMUTE);
					aVideoCanvas.addCommand(CMD_MUTE);
				}
			}
			else {
				JMEUtility.showErrorAlert("ERROR", "NOT ABLE TO GET VOLUME CONTROL", 4000, this, parentMidlet);
			}
		}
		else if (c == CMD_HIGHEST_VOLUME) {
			if ( volumeControl != null ) {
				volumeControl.setLevel(100);
			}
			else {
				JMEUtility.showErrorAlert("ERROR", "NOT ABLE TO GET VOLUME CONTROL", 4000, this, parentMidlet);
			}
		}
		else if (c == CMD_MEDIUM_VOLUME) {
			if ( volumeControl != null ) {
				volumeControl.setLevel(40);
			}
			else {
				JMEUtility.showErrorAlert("ERROR", "NOT ABLE TO GET VOLUME CONTROL", 4000, this, parentMidlet);
			}
		}
		else if (c == CMD_UPLOAD_TO_SERVER) {
			( new UploadToServerThread()).start();
		}

	}

	public void playerUpdate(Player plyr, String evt, Object evtData) {

		if ( evt == PlayerListener.STARTED && currentState != CUSTOM_SHOW_RECORDED ) {
			aVideoCanvas.addCommand(CMD_RECORD);
			aVideoCanvas.removeCommand(CMD_MUTE);
			aVideoCanvas.removeCommand(CMD_UNMUTE);
			aVideoCanvas.removeCommand(CMD_HIGHEST_VOLUME);
			aVideoCanvas.removeCommand(CMD_MEDIUM_VOLUME);
		}
		else if ( evt == PlayerListener.RECORD_STARTED ) {
			aVideoCanvas.removeCommand(CMD_RECORD);
			aVideoCanvas.addCommand(CMD_STOP_RECORD);
		}
		else if ( evt == PlayerListener.RECORD_STOPPED ) {
			aVideoCanvas.removeCommand(CMD_STOP_RECORD);
			aVideoCanvas.addCommand(CMD_SHOW_RECORDED);
		}
		else if ( evt == PlayerListener.END_OF_MEDIA && currentState == CUSTOM_SHOW_RECORDED) {
			currentState = CUSTOM_OK_TO_RECORD;
			aVideoCanvas.addCommand(CMD_RECORD);
			aVideoCanvas.removeCommand(CMD_STOP_RECORD);
			aVideoCanvas.removeCommand(CMD_SHOW_RECORDED);

			(new CameraThread()).start();

		}
		else if ( evt == PlayerListener.RECORD_ERROR ) {
			JMEUtility.showErrorAlert("ERROR", (String)evtData, 4000, this, parentMidlet);
		}
		else if ( evt == PlayerListener.ERROR ) {
			JMEUtility.showErrorAlert("ERROR", (String)evtData, 4000, this, parentMidlet);
		}
	}

	private void showCamera() {

		try {

			releaseResources();
			player = Manager.createPlayer("capture://video");
			player.addPlayerListener(this);
			player.realize();

			videoControl = (VideoControl)player.getControl("VideoControl");
			aVideoCanvas = new VideoCanvas();
			aVideoCanvas.initControls(videoControl, player);

			aVideoCanvas.addCommand(CMD_BACK);
			aVideoCanvas.addCommand(CMD_EXIT);
			aVideoCanvas.setCommandListener(this);
			parentMidlet.getDisplay().setCurrent(aVideoCanvas);

			player.start();
			contentType = player.getContentType();


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

	class CameraThread extends Thread {

		public CameraThread() {
		}

		public void run() {
			showCamera();
		}
	}

	class VideoRecordingThread extends Thread {
		RecordControl rc;

		public VideoRecordingThread() {
		}

		public void run() {
			recordVideo();
			//recordDummy();
		}

		public void recordVideo(){
			try {

				rc = (RecordControl)player.getControl("RecordControl");
				if ( rc == null ) {
					System.out.println("NULL RECORD CONTROL");
					JMEUtility.showErrorAlert("ERROR", "Recording of Video not supported", 4000, self, parentMidlet);
					return;
				}
				output = new ByteArrayOutputStream();
				rc.setRecordStream(output);
				rc.startRecord();

			}
			catch (Exception e) {
				JMEUtility.showErrorAlert("ERROR", e.getMessage(), 4000, self, parentMidlet);

			}
		}
		public void stopRecord(){
			try{
				if ( rc != null ) {
					rc.stopRecord();
					rc.commit();
				}

				//aVideoCanvas.removeCommand(CMD_STOP_RECORD);
				//aVideoCanvas.addCommand(CMD_SHOW_RECORDED);
			}catch (Exception e){
				JMEUtility.showErrorAlert("ERROR", e.getMessage(), 4000, self, parentMidlet);
			}
		}

		public void recordDummy() {
			try {

				InputStream is = getClass().getResourceAsStream("/video/dummyrecord.mpg");
				output = new ByteArrayOutputStream();
				int len = is.available();
				byte[] bytes = new byte[len];
				is.read(bytes);
				output.write(bytes, 0, bytes.length);

				aVideoCanvas.removeCommand(CMD_RECORD);
				aVideoCanvas.addCommand(CMD_STOP_RECORD);

				contentType = "video/mpeg";


			}catch(Exception e){
				JMEUtility.showErrorAlert("ERROR", e.getMessage(), 4000, self, parentMidlet);
			}
		}
	}

	class VideoViewerThread extends Thread {

		public VideoViewerThread() {
		}
		public void run() {
			viewVideo();
		}

		public void viewVideo() {
			try {
				releaseResources();

				ByteArrayInputStream bis = new ByteArrayInputStream(output.toByteArray());

				System.out.println("contentType=" + contentType);


				player = Manager.createPlayer(bis, contentType);
				player.realize();
				videoControl = (VideoControl)player.getControl("VideoControl");
				volumeControl = (VolumeControl)player.getControl("VolumeControl");
				volumeControl.setLevel(100);

				if ( aVideoCanvas != null ) {
					aVideoCanvas.initControls(videoControl, player);
					parentMidlet.getDisplay().setCurrent(aVideoCanvas);
					player.start();
				}
			}
			catch(Exception e){
				JMEUtility.showErrorAlert("ERROR", e.getMessage(), 4000, self, parentMidlet);
			}
		}

	}

	class UploadToServerThread extends Thread {

		public UploadToServerThread() {

		}
		public void run() {
			uploadVideo();
		}

		public void uploadVideo() {

			try {
				if ( output != null ) {
					byte[] bytes = output.toByteArray();
					DTOMediaStoreIn aDTOMediaStoreIn = new DTOMediaStoreIn();
					aDTOMediaStoreIn.inpByteArray = bytes;
					aDTOMediaStoreIn.extension = JMEUtility.findFileExtension(contentType);

					DTOMediaStoreOut aDTOMediaStoreOut =
						HttpCommandExecuter.getInstance().executeHttp(aDTOMediaStoreIn);

					if ( aDTOMediaStoreOut != null ) {
						if ( aDTOMediaStoreOut.successFlag != null &&
							aDTOMediaStoreOut.successFlag.equals("S") ) {
							JMEUtility.showInfoAlert("Information", "Movie uploaded in Server successfully", 4000, self, parentMidlet);
						}
						else {
							JMEUtility.showErrorAlert("ERROR", "Image upload Unsuccessful", 4000, self, parentMidlet);
						}
					}
					else {
						JMEUtility.showErrorAlert("ERROR", "Null Return from Server", 4000, self, parentMidlet);
					}

				}
				else {
					JMEUtility.showErrorAlert("ERROR", "Recorded Video not Found for upload", 4000, self, parentMidlet);
				}
			}catch(Exception e) {
				e.printStackTrace();
				JMEUtility.showErrorAlert("ERROR", e.getMessage(), 4000, self, parentMidlet);
			}

		}

	}

}