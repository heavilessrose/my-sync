package org.kalmeo.kuix.core;

import javax.microedition.midlet.MIDlet;

import org.kalmeo.kuix.widget.Desktop;

/**
 * 在Kuix初始化期间执行定制.
 * This interface permit to implements custom value during Kuix engine init
 * process.<br>
 * The default implementation of this interface is {@link KuixMIDlet}. But you
 * can init Kuix without the {@link KuixMIDlet} class by implementing you own
 * {@link KuixInitializer} and add this couple of lines to you standard Java ME
 * application :
 * 
 * <pre>
 * KuixInitializer initializer = new MyKuixInitializer();
 * KuixCanvas canvas = new KuixCanvas(initializer, true);
 * KuixConverter converter = new KuixConverter();
 * Kuix.initialize(null, canvas, converter);
 * </pre>
 * 
 * After to be able to switch screen to the KuixCanvas by :
 * 
 * <pre>
 * Display.getDisplay(&lt;myMIDlet&gt;).setCurrent(Kuix.getCanvas());
 * </pre>
 * 
 * @author bbeaulant
 */
public interface KuixInitializer {

	/**
	 * Returns the application MIDlet instance.
	 * 
	 * @return the {@link MIDlet} instance.
	 */
	public MIDlet getMIDlet();

	/**
	 * Returns the initalization background color.
	 * 
	 * @return a color int representation.
	 */
	public int getInitializationBackgroundColor();

	/**
	 * Returns the initalization message color.
	 * 
	 * @return ta color int representation.
	 */
	public int getInitializationMessageColor();

	/**
	 * Returns the initalization message (e.g. "Loading") or <code>null</code>
	 * if no message.
	 * 
	 * @return a string
	 */
	public String getInitializationMessage();

	/**
	 * Returns the initalization message (e.g. "/img/loading.png") or
	 * <code>null</code> if no image.
	 * 
	 * @return a string
	 */
	public String getInitializationImageFile();

	/**
	 * Implement in this method all your style initializations (e.g.
	 * <code>Kuix.loadCss("myStyle.css");</code>).
	 */
	public void initDesktopStyles();

	/**
	 * Init the Desktop's content. This method is call during the initialization
	 * process, then it is preferable to load the first screen there.
	 * 
	 * @param desktop
	 */
	public void initDesktopContent(Desktop desktop);

	/**
	 * Invoked to destroy the MIDlet implementation.
	 */
	public void destroyImpl();

	/**
	 * This method is invoked when a Kuix internal debug infos key event
	 * occured.<br>
	 * You can override this method to implement your own debug infos process.
	 */
	public void processDebugInfosKeyEvent();

}
