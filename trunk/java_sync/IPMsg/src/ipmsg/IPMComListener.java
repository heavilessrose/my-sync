/*
 *	IP Messenger Event Commnucation Listener Interface
 *		1997/10/16 (C) Copyright T.Kazawa (Digitune)
 */

package ipmsg;

import java.util.EventListener;

public interface IPMComListener extends EventListener {

	public abstract void receive(IPMComEvent ev);
}
