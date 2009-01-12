package net.sf.mvc.mobile;

import javax.microedition.lcdui.Command;
import javax.microedition.lcdui.Displayable;
import javax.microedition.lcdui.Form;
import javax.microedition.lcdui.Gauge;

import net.sf.mvc.mobile.command.ActionCommand;

public class DefaultProgressView extends Form implements ProgressListner {
	private Gauge gauge;
	private String labelPrefix = "${loading}: ";

	public DefaultProgressView() {
		super("${please.wait}");
		if("MIDP-1.0".equals(System.getProperty("microedition.profiles")))
			gauge = new Gauge(labelPrefix, false, 100, 50);
		else 
			gauge = new Gauge(labelPrefix, false, -1 /*Gauge.INDEFINITE*/,
					0 /*Gauge.CONTINUOUS_RUNNING*/);
		append(gauge);
		addCommand(new ActionCommand("${abort}",Command.BACK, 1){
			public Navigation execute(Displayable d) throws Exception {
				throw new InterruptedException();
			}			
		});
	}

	public Displayable getDisplay() {
		return this;
	}

	public void setMaxValue(int i) {
		gauge.setMaxValue(i == 0 ? 1 : i);
	}

	public void setValue(int i) {
//		int percent = (i+1) * 100 / gauge.getMaxValue();
//		gauge.setLabel(labelPrefix + percent + "%");
		gauge.setValue(i);
	}

	public Gauge getGauge() {
		return gauge;
	}

	public String getLabelPrefix() {
		return labelPrefix;
	}

	public void setLabelPrefix(String labelPrefix) {
		this.labelPrefix = labelPrefix;
	}
}
