package bm.core.event;

/*
 * File Information
 *
 * Created by       : narciso
 * Last modified by : $Author:narciso $
 * Last modified on : $Date:2007-02-03 19:34:57 +0100 (sáb, 03 feb 2007) $
 * Revision         : $Revision:4 $
 * 
 * modified luke 09.2.17
 */

/**
 * A EventListener listens to ProgressEvents.
 * 
 * an interface that must be implemented in order to be able to listen to
 * events.
 * 
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision:4 $
 */
public interface EventListener {
	/**
	 * Handle event.
	 * 
	 * @param event
	 *            event to handle
	 */
	void handleEvent(Event event);
}
