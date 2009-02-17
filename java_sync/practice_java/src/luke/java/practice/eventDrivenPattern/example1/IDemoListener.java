package luke.java.practice.eventDrivenPattern.example1;

import java.util.EventListener;

/**
 * DemoEvent事件类的监听器接口 - 定义监听器必须实现的方法
 * <p>
 * 监听器目的是对事件作出响应，因此，监听器接口方法中，应该定义一个方法来处理事件，即所谓的事件处理方法。
 * 事件处理方法以事件对象作为入口参数，包含对事件进行处理的代码.
 * </p>
 * 
 * @author WangYinghua
 * 
 */
public interface IDemoListener extends EventListener {
	public boolean handleEvent(DemoEvent event);
}
