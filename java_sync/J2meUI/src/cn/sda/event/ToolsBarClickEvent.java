package cn.sda.event;

import cn.sda.ui.ToolButtonUI;

/**
 * 
 * @author Administrator
 */
abstract public class ToolsBarClickEvent {
	abstract public void Event(ToolButtonUI button, int buttonIndex);
}
