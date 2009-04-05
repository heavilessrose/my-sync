package cn.sda.event;

import cn.sda.ui.MenuItemUI;

/**
 * <p>Title: </p>
 *
 * <p>Description: </p>
 *
 * <p>Copyright: Copyright (c) 2008</p>
 *
 * <p>Company: </p>
 *
 * @author not attributable
 * @version 1.0
 */
abstract public class PopMenuOnClickEvent {
    abstract public void Event(MenuItemUI item,String itemName,String itemCaption);
}
