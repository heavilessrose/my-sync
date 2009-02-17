package luke.java.practice.eventDrivenPattern.example1;

/**
 * DemoEvent�¼��ļ����� - �ṩ�������ӿ��ж�������з�����ʵ�ִ���. ���ӵļ���������ʵ�ֶ��ּ������ӿڣ������������͵��¼���
 * 
 * @author WangYinghua
 * 
 */
public class MyListener implements IDemoListener {

	/** �¼������� - ��ʱ���ߵ��³��������Ĵ��벻Ӧ�������¼��������� */
	public boolean handleEvent(DemoEvent event) {
		Object eventSource = event.getSource();
		System.out.println(eventSource + "������DemoEvent�¼�,");
		System.out.println(this + "��������Ӧ");
		return false;
	}

}
