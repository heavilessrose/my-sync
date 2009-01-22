package luke.android.app.todoList;
import java.util.LinkedList;
import java.util.List;

import android.content.Context;
import android.util.AttributeSet;
import android.util.Log;
import android.view.MotionEvent;
import android.view.View;
import android.view.ViewConfiguration;
import android.widget.Adapter;
import android.widget.FrameLayout;
import android.widget.ListView;

public class CrossView extends FrameLayout {

	public CrossView(Context context) {
		super(context);
		this.setClickable(true);
	}
	
	public CrossView(Context context, AttributeSet attrs) {
		super(context, attrs);
		this.setClickable(true);
	}
	
	public CrossView(Context context, AttributeSet attrs, int defStyle) {
		super(context, attrs, defStyle);
		this.setClickable(true);
	}
	
	public MotionEvent downStart = null;
	
	public boolean onInterceptTouchEvent(MotionEvent event) {
		
		switch(event.getAction()) {
		case MotionEvent.ACTION_DOWN:
			// keep track of the starting down-event
			downStart = MotionEvent.obtain(event);
			break;
		case MotionEvent.ACTION_MOVE:
			// if moved horizontally more than slop*2, capture the event for ourselves
			float deltaX = event.getX() - downStart.getX();
			if(Math.abs(deltaX) > ViewConfiguration.getTouchSlop() * 2)
				return true;
			
			break;
			
		}
		
		// otherwise let the event slip through to children
		return false;
	
	}
	
	public boolean onTouchEvent(MotionEvent event) {
		
		// check if we crossed an item
		float targetWidth = this.getWidth() / 4;
		float deltaX = event.getX() - downStart.getX(),
			deltaY = event.getY() - downStart.getY();
		
		boolean movedAcross = (Math.abs(deltaX) > targetWidth);
		boolean steadyHand = (Math.abs(deltaX / deltaY) > 2);
		
		if(movedAcross && steadyHand) {
			boolean crossed = (deltaX > 0);
			
			// figure out which child view we crossed
			ListView list = (ListView)this.findViewById(android.R.id.list);
			int position = list.pointToPosition((int)downStart.getX(), (int)downStart.getY());
			
			// pass crossed event to any listeners
			for(OnCrossListener listener : listeners) {
				listener.onCross(position, crossed);
			}
			
			// and return true to consume this event
			return true;
	
		}
		
		return false;
	}

	private List<OnCrossListener> listeners = new LinkedList<OnCrossListener>();
	
	public void addOnCrossListener(OnCrossListener listener) {
		listeners.add(listener);
	}
	

}
