package cn.sda.event;

import cn.sda.ui.SDADataSet.Row;

/**
 * 
 * @author Administrator
 */
abstract public class DataScrollChangeEvent {
	abstract public void Event(Row row);
}
