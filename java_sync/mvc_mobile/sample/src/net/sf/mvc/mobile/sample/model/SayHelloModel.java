package net.sf.mvc.mobile.sample.model;

import javax.microedition.lcdui.Command;
import javax.microedition.lcdui.Displayable;

import net.sf.mvc.mobile.Model;
import net.sf.mvc.mobile.Navigation;
import net.sf.mvc.mobile.command.ActionCommand;
import net.sf.mvc.mobile.command.NavigationCommand;
import net.sf.mvc.mobile.sample.SampleControler;
import net.sf.mvc.mobile.sample.view.SayHelloView;

public class SayHelloModel implements Model {

	private SayHelloView view;
	private String name;
	private SampleControler controler;

	public SayHelloModel(SampleControler controler) {
		this.controler = controler;
	}

	public void load(Object parameter) throws Exception {
		if(parameter == null) {
			controler.progressListner.setMaxValue(100);
			name = "";
			for(int i=0;i<100;++i) {
				if(i % 10 == 0)
					name += (char)('a'+i%60);
				Thread.sleep(50);
				controler.progressListner.setValue(i);
			}
		}else{
			name = (String) parameter;
		}
		view.setText(name);
	}	

	public void setView(Displayable v) {
		this.view = (SayHelloView) v;
		
		view.addCommand(new ActionCommand("Edit", Command.ITEM, 1) {
			public Navigation execute(Displayable d) throws Exception {
				System.out.println(".execute(), name="+name);
				return new Navigation("enter-name", "Edit your name", name);
			}			
		});
		view.addCommand(new NavigationCommand(
				new Navigation("enter-name", "Enter your new name", ""), "New name", Command.ITEM, 1));
	}
}