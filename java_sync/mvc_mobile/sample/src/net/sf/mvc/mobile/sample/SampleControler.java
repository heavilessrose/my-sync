package net.sf.mvc.mobile.sample;

import javax.microedition.midlet.MIDlet;

import net.sf.mvc.mobile.Controler;
import net.sf.mvc.mobile.LazyInit;
import net.sf.mvc.mobile.Page;
import net.sf.mvc.mobile.sample.model.EnterNameModel;
import net.sf.mvc.mobile.sample.model.SayHelloModel;
import net.sf.mvc.mobile.sample.view.EnterNameView;
import net.sf.mvc.mobile.sample.view.SayHelloView;

public class SampleControler extends Controler {

	public SampleControler(MIDlet midlet) {
		super(midlet);
	}

	protected void createNavigation() {
		/* Pages can be indexed by any object, not only string.
		 * The value of the pages map can by either Page or LazyInit
		 * Using LazyInit allows for creation of forms and their logic (model) only when the page is accessed
		 */
		pages.put("enter-name", new Page(new EnterNameModel(SampleControler.this), new EnterNameView()));
		pages.put("say-hello", new LazyInit(){
			public Page create() {
				return new Page(new SayHelloModel(SampleControler.this), new SayHelloView());
			}			
		});
	}

	public void start() {
		try {
			show("enter-name", null);
		} catch (Exception e) {
			show(e);
		}		
	}

}
