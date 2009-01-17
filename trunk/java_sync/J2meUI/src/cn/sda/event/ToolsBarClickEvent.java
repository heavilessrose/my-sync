package cn.sda.event;

import cn.sda.ui.SDAToolButton;

/**
 * 
 * @author Administrator
 */
abstract public class ToolsBarClickEvent {
	abstract public void Event(SDAToolButton button, int buttonIndex);
}
