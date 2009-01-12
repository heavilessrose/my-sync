package events.eventDriveFramework;

import java.util.ArrayList;
import java.util.List;

/** �¼������Ŀ��ƿ�� */
public class Controller {
	private List<Event> eventList = new ArrayList<Event>();

	public void addEvent(Event ev) {
		eventList.add(ev);
	}

	public void run(){
		while(eventList.size() > 0){
			for(Event e:new ArrayList<Event>(eventList)){
				if(e.ready()){
					System.out.println(e);
					e.action();
					eventList.remove(e);
				}
			}
		}
	}
}
