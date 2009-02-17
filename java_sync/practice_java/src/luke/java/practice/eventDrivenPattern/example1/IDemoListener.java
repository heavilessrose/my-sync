package luke.java.practice.eventDrivenPattern.example1;

import java.util.EventListener;

/**
 * DemoEvent�¼���ļ������ӿ� - �������������ʵ�ֵķ���
 * <p>
 * ������Ŀ���Ƕ��¼�������Ӧ����ˣ��������ӿڷ����У�Ӧ�ö���һ�������������¼�������ν���¼���������
 * �¼����������¼�������Ϊ��ڲ������������¼����д���Ĵ���.
 * </p>
 * 
 * @author WangYinghua
 * 
 */
public interface IDemoListener extends EventListener {
	public boolean handleEvent(DemoEvent event);
}
