import java.util.TimerTask;

class SimpleTimerTask extends TimerTask {

	public SimpleTimerTask(mainCanvas maincanvas, short word0) {
		canvas = maincanvas;
		timerType = word0;
	}

	public void run() {
		canvas.SimpleTimerEndEvent(timerType);
	}

	private mainCanvas canvas;
	private short timerType;
}
