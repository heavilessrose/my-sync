package com.srijeeb.jme;
import javax.microedition.lcdui.*;
import javax.microedition.media.*;

public class PropertyForm extends Form implements CommandListener {

	private final static Command CMD_EXIT = new Command("Exit", Command.EXIT, 1);
    private final static Command CMD_OK = new Command("Ok", Command.SCREEN, 1);

	private MobileVideoApp parentMidlet = null;

	protected PropertyForm(String in, MobileVideoApp parentMidlet_) {
		super(in);
		this.parentMidlet = parentMidlet_;
		initComponents();
	}

	public void initComponents() {
		append(JMEUtility.getImage("/images/banner.png"));
		append(getOptions());
		append(JMEUtility.getImage("/images/separator.png"));
		append(getServerAddressField());
		append(JMEUtility.getImage("/images/separator.png"));
		append(JMEUtility.getImage("/images/separator.png"));
		append(getStringItem("version", "microedition.media.version"));
		append(JMEUtility.getImage("/images/separator.png"));
		append(getStringItem("Audio Capture", "supports.audio.capture"));
		append(JMEUtility.getImage("/images/separator.png"));
		append(getStringItem("Video Capture", "supports.video.capture"));
		append(JMEUtility.getImage("/images/separator.png"));
		append(getStringItem("Recording", "supports.recording"));
		append(JMEUtility.getImage("/images/separator.png"));
		append(getStringItem("Audio Enc", "audio.encodings"));
		append(JMEUtility.getImage("/images/separator.png"));
		append(getStringItem("Video Enc", "video.encodings"));
		append(JMEUtility.getImage("/images/separator.png"));
		append(getStringItem("Video Snp Enc", "video.snapshot.encodings"));
		append(JMEUtility.getImage("/images/separator.png"));
		append(getStringItem("Stream Cont", "streamable.contents"));
		append(JMEUtility.getImage("/images/separator.png"));
		append(getStringItemFromLocalDevice("BT Recieve MTU max", "bluetooth.l2cap.receiveMTU.max"));
		append(JMEUtility.getImage("/images/separator.png"));
		append(getStringItemFromLocalDevice("BT max connected device", "bluetooth.connected.devices.max"));

		append(JMEUtility.getImage("/images/separator.png"));
		append(JMEUtility.getImage("/images/separator.png"));
		append(getSupportedProtocols());
		append(JMEUtility.getImage("/images/separator.png"));
		append(getSupportedContentTypeForHttp());

		addCommand(CMD_OK);
		addCommand(CMD_EXIT);
		setCommandListener(this);
	}

	private StringItem getStringItem(String name, String propertyName) {
		String value = System.getProperty(propertyName);
		return new StringItem("[" + name + "]", value);
	}

	private StringItem getStringItemFromLocalDevice(String name, String propertyName) {
		String value = javax.bluetooth.LocalDevice.getProperty(propertyName);
		return new StringItem("[" + name + "]", value);
	}
	private StringItem getSupportedProtocols() {
		return new StringItem("[Protocols]", concatArray(Manager.getSupportedProtocols(null)) );
	}

	private StringItem getSupportedContentTypeForHttp() {
		return new StringItem("[Content http]", concatArray(Manager.getSupportedContentTypes("http")) );
	}


	Image[] imageArray = {JMEUtility.getImage("/images/work-offline.png")};
	private ChoiceGroup options = null;
	private ChoiceGroup getOptions() {
		if ( options == null )
			options = new ChoiceGroup("", Choice.MULTIPLE ,
									  new String[]{"Work Offline"}, imageArray);
		return options;
	}
	TextField serverAddress = null;
	private TextField getServerAddressField() {
		if ( serverAddress == null )
			serverAddress = new TextField("[Server Address] ", "127.0.0.1:9080", 25, TextField.ANY);
		return serverAddress;
	}
	public void commandAction(Command c, Displayable d) {
        if (c == CMD_EXIT) {
			parentMidlet.destroyApp(true);
			parentMidlet.notifyDestroyed();
		}
        else if (c == CMD_OK) {

			if ( !options.isSelected(0) && serverAddress.getString().trim().length() <= 0 ) {
				JMEUtility.showErrorAlert("ERROR", "Server Address is mandatory if Work Offline not Selected.", 3000, this, parentMidlet);
				return;
			}
			HttpCommandExecuter.setServiceURL("http://" + serverAddress.getString().trim() + "/MMAWeb/MMAServlet");

			parentMidlet.setOffline(options.isSelected(0));
			parentMidlet.setServerAddress(serverAddress.getString().trim());
			OptionForm optionForm = new OptionForm("",parentMidlet,this);
			parentMidlet.getDisplay().setCurrent(optionForm);
		}
	}

	public String concatArray(String[] list) {
		String ret = "";
		if ( list != null && list.length > 0 ) {
			for ( int i=0; i < list.length; i++ ) {
				ret += list[i];
				if ( i < (list.length-1)) {
					ret += "|";
				}
			}
		}
		return ret;
	}

}