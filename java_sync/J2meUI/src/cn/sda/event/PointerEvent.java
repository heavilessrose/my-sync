package cn.sda.event;

import cn.sda.ui.BaseControl;

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
abstract public class PointerEvent {
    abstract public void Event(BaseControl ctrl,int x,int y);
}
