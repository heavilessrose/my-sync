import javax.microedition.lcdui.Canvas;
import javax.microedition.lcdui.Display;
import javax.microedition.midlet.MIDlet;
import javax.microedition.midlet.MIDletStateChangeException;

public class COD extends MIDlet {

	public COD() {
		display = Display.getDisplay(this);
	}

	public void startApp() throws MIDletStateChangeException {
		if (canvas == null)
			canvas = new mainCanvas(this);
		canvas.repaint();
		display.setCurrent(canvas);
	}

	public void pauseApp() {
		canvas.pause();
	}

	public void midletExit() {
		destroyApp(true);
		notifyDestroyed();
	}

	public void destroyApp(boolean flag) {
	}

	mainCanvas canvas;
	Display display;
}
