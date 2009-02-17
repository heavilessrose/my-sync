package luke.java.practice.eventDrivenPattern.example1;

import java.util.EventObject;

/**
 * 事件类
 * <p>
 * 所有的事件类必须继承Java事件基类,
 * EventObject定义了事件类的共有特征，其中，最重要的属性是Object类型的source，这个属性指明了事件的来源，即事件源。
 * </p>
 * 
 * @author WangYinghua
 * 
 */
public class DemoEvent extends EventObject {

	/**
	 * 唯一构造方法, 即所有事件必须在实例化时就指定事件源
	 * 
	 * @param source
	 *            事件源
	 */
	public DemoEvent(Object source) {
		super(source);
	}

}
