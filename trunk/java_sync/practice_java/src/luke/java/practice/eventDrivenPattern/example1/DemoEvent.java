package luke.java.practice.eventDrivenPattern.example1;

import java.util.EventObject;

/**
 * �¼���
 * <p>
 * ���е��¼������̳�Java�¼�����,
 * EventObject�������¼���Ĺ������������У�����Ҫ��������Object���͵�source���������ָ�����¼�����Դ�����¼�Դ��
 * </p>
 * 
 * @author WangYinghua
 * 
 */
public class DemoEvent extends EventObject {

	/**
	 * Ψһ���췽��, �������¼�������ʵ����ʱ��ָ���¼�Դ
	 * 
	 * @param source
	 *            �¼�Դ
	 */
	public DemoEvent(Object source) {
		super(source);
	}

}
