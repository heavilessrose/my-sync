package net.sf.mvc.mobile;

import java.util.Hashtable;
import java.util.NoSuchElementException;
import java.util.Vector;

import javax.microedition.lcdui.Alert;
import javax.microedition.lcdui.AlertType;
import javax.microedition.lcdui.Command;
import javax.microedition.lcdui.CommandListener;
import javax.microedition.lcdui.Display;
import javax.microedition.lcdui.Displayable;
import javax.microedition.midlet.MIDlet;

import net.sf.mvc.mobile.command.ActionCommand;

public abstract class Controler implements CommandListener {
	protected final MIDlet midlet;
	protected Hashtable pages;
	public ProgressListner progressListner;
	protected Vector skipOnBack = new Vector();
	protected Displayable previousView = null;

	public Controler(MIDlet midlet) {
		this.midlet = midlet;
		pages = new Hashtable();
		progressListner = new DefaultProgressView();
		createNavigation();
		skipOnBack.addElement("Alert");
		skipOnBack.addElement("DefaultProgressView");
	}

	public MIDlet getMidlet() {
		return midlet;
	}

	public abstract void start();

	protected abstract void createNavigation();

	public void commandAction(final Command c, final Displayable d) {
		Runnable execute = new Runnable() {
			public void run() {
				Displayable progressView = progressListner.getDisplay();
				try {
					//TODO: start after .5 sec
					setView(progressView);
					ActionCommand command = (ActionCommand) c;
					Navigation navigation = command.execute(d);
					if (navigation != null)
						show(navigation.getName(), navigation.getParameter());
					else
						back();
				} catch (InterruptedException ex) {
					if(getView() == progressView)
						back();	
				} catch (Exception ex) {
					show(ex);
				} 
			}			
		};
		
		new Thread(execute).start();
	}

	protected void back() {
		if(previousView == null) 
			quit();
		else
			setView(previousView);
	}

	protected void quit() {
		Alert alert = new Alert("${quit}?","${quit.or.restart}?",null, AlertType.CONFIRMATION);
		alert.addCommand(new ActionCommand("${quit}", Command.BACK, 1){
			public Navigation execute(Displayable d) throws Exception {
				midlet.notifyDestroyed();
				return null;
			}						
		});
		alert.addCommand(new ActionCommand("${restart}", Command.OK, 2){
			public Navigation execute(Displayable d) throws Exception {
				start();
				return null;
			}			
		});
		setView(alert);
	}

	public void show(Exception ex) {
		ex.printStackTrace();
		Alert alert = new Alert("${error}");
		alert.setString(ex+(ex.getMessage() == null ? "" : ":"+ex.getMessage()));
		alert.setType(AlertType.ERROR);
		alert.setTimeout(Alert.FOREVER);
		alert.addCommand(new ActionCommand("${OK}", Command.OK, 1){
			public Navigation execute(Displayable d) throws Exception {
				back();
				return null;
			}			
		});
		alert.addCommand(new ActionCommand("${quit}", Command.BACK, 1){
			public Navigation execute(Displayable d) throws Exception {
				midlet.notifyDestroyed();
				return null;
			}						
		});
		setView(alert);
	}

	protected void show(Object name, final Object parameter) throws Exception {
		Object entry = pages.get(name);
		if (entry == null)
			throw new NoSuchElementException("${page.not.found}:" + name);
		final Page page = getPage(entry);
		final Model model = page.getModel();
		model.load(parameter);
		setView(page.getView());
	}

	private Page getPage(Object entry) {
		Page page;

		if (entry instanceof Page) {
			page = (Page) entry;
		} else if (entry instanceof LazyInit) {
			LazyInit lazy = (LazyInit) entry;
			page = lazy.get();
		} else
			throw new IllegalArgumentException("${invalid.page.type}:" + entry.getClass());
		return page;
	}

	protected void setView(Displayable display) {
		final Displayable view = getView();
		if(view != null && !skipOnBack.contains(view.getClass().getName())) 
			previousView = view;
		display.setCommandListener(this);
		Display.getDisplay(midlet).setCurrent(display);
	}
	
	protected Displayable getView() {
		try {
			return Display.getDisplay(midlet).getCurrent();
		} catch (Exception ex) {
			return null;
		}
	}
}
