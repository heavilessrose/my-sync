package winkCC.net.http;

import javax.microedition.lcdui.Command;
import javax.microedition.lcdui.CommandListener;
import javax.microedition.lcdui.Displayable;
import javax.microedition.lcdui.Form;
import javax.microedition.lcdui.Gauge;

public class HttpWaitUI extends Form implements CommandListener, IHttpListener {
	private Gauge gauge;
	private Command cancel;
	private Displayable displayable;

	public HttpWaitUI() {
		super("Connecting");
		this.gauge = new Gauge("Progress", false, 100, 0);
		this.cancel = new Command("Cancel", Command.CANCEL, 0);
		append(gauge);
		addCommand(cancel);
		setCommandListener(this);
	}

	public void commandAction(Command c, Displayable d) {
		if (c == cancel) {
		}
	}

	public void dispose() {

	}

	public void onCancel() {
		HttpTest.goback();
	}

	public void onError() {

	}

	public void onFinish(byte[] data, int size) {

	}

	public void onProgress(int progress) {

	}
}