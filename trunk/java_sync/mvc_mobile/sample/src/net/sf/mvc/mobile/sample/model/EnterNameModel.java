package net.sf.mvc.mobile.sample.model;

import javax.microedition.lcdui.Command;
import javax.microedition.lcdui.Displayable;

import net.sf.mvc.mobile.Model;
import net.sf.mvc.mobile.Navigation;
import net.sf.mvc.mobile.command.ActionCommand;
import net.sf.mvc.mobile.command.NavigationCommand;
import net.sf.mvc.mobile.sample.SampleControler;
import net.sf.mvc.mobile.sample.view.EnterNameView;

public class EnterNameModel implements Model {

	private EnterNameView view;
	private SampleControler controler;

	public EnterNameModel(SampleControler controler) {
		this.controler = controler;
	}

	// setView is called before load
	public void setView(Displayable v) {
		this.view = (EnterNameView) v;
		
		 
		view.addCommand(new ActionCommand("Say hello",Command.SCREEN, 1){
			public Navigation execute(Displayable d) throws Exception {
				return new Navigation("say-hello",view.getName());
			}			
		});
		/* Please note that the command above can't be writter as
		 * 		view.addCommand(new NavigationCommand(
		 * 			new Navigation("say-hello",	view.getName()), "Say hello", Command.SCREEN, 1));
		 * because view.getName() has to be evaluated each time the command is executed
		 */
		
		view.addCommand(new ActionCommand("Quit",Command.SCREEN, 9){
			public Navigation execute(Displayable d) throws Exception {
				controler.getMidlet().notifyDestroyed();
				return null;
			}			
		});
		view.addCommand(new NavigationCommand(new Navigation("say-hello", null), 
				"Slow load", Command.SCREEN, 2));
		view.addCommand(new ActionCommand("Slow action", Command.SCREEN, 3){
			public Navigation execute(Displayable d) throws Exception {
				String name = view.getName();
				controler.progressListner.setMaxValue(name.length());
				for(int i=0;i<name.length();++i) {
					//slow operation:
					Thread.sleep(100);
					controler.progressListner.setValue(i);
				}
				return new Navigation("say-hello", name);
			}			
		});
	}

	public void load(Object parameter) throws Exception {
		if(parameter != null) {
			/* If multiple parameters are passed to a Page, then
			 * you must cast the supplied parameter object to Object[]
			 */
			Object[] params = (Object[]) parameter;
			String label = (String) params[0];
			String name = (String) params[1];
			view.setLabel(label);
			view.setName(name);
		} else {
			/* This is the first time we are showing EnterNameView
			 * @see SampleControler.start() - null parameter is passed.
			 * So we'll use default texts (defined in the view.
			 */
		}
	}
}
