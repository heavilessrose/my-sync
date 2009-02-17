package luke.java.practice.eventDrivenPattern.example1;

/**
 * DemoEvent事件的监听器 - 提供监听器接口中定义的所有方法的实现代码. 复杂的监听器可以实现多种监听器接口，监听多种类型的事件。
 * 
 * @author WangYinghua
 * 
 */
public class MyListener implements IDemoListener {

	/** 事件处理方法 - 耗时或者导致程序阻塞的代码不应包含在事件处理方法中 */
	public boolean handleEvent(DemoEvent event) {
		Object eventSource = event.getSource();
		System.out.println(eventSource + "触发了DemoEvent事件,");
		System.out.println(this + "作出了响应");
		return false;
	}

}
