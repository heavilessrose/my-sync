package winkCC.core.event;

import winkCC.core.CoreConstants;

/**
 * 继承Event的进度事件.</br> 提交ProgressEvents来提示进度
 * 
 * @author WangYinghua
 * @version 0.1
 */
public class ProgressEvent extends Event {
	private String title;
	private Integer currentValue;
	private Integer maxValue;
	// 事件源
	private Object source;
	// 阶段状态
	private String phase;
	private String message;
	private boolean animate;
	// 可中断标记
	private Boolean cancellable;

	public ProgressEvent() {
		type = PROGRESS;
	}

	public ProgressEvent(final Object source) {
		this();
		this.source = source;
	}

	public ProgressEvent(final Object source, final Integer maxValue,
			final Integer currentValue) {
		this();
		this.currentValue = currentValue;
		this.maxValue = maxValue;
		this.source = source;
	}

	public ProgressEvent(final int currentValue, final Integer maxValue,
			final Object source, final String phase) {
		this(source, maxValue, currentValue);
		this.phase = phase;
	}

	public ProgressEvent(final Object source, final Integer maxValue,
			final int currentValue) {
		this(source, maxValue, new Integer(currentValue));
	}

	public ProgressEvent(final Integer currentValue, final Integer maxValue,
			final Object source, final String phase) {
		this(source, maxValue, currentValue);
		this.phase = phase;
	}

	public String getTitle() {
		return title;
	}

	public synchronized void setTitle(final String title) {
		this.title = title;
	}

	public int getCurrentValue() {
		return currentValue != null ? currentValue.intValue() : 0;
	}

	public synchronized void setCurrentValue(final int currentValue) {
		this.currentValue = new Integer(currentValue);
	}

	public Integer getValue() {
		return currentValue;
	}

	public synchronized void setValue(final Integer value) {
		this.currentValue = value;
	}

	public Object getSource() {
		return source;
	}

	public synchronized void setSource(final Object source) {
		this.source = source;
	}

	public Integer getMaxValue() {
		return maxValue;
	}

	public synchronized void setMaxValue(final Integer maxValue) {
		this.maxValue = maxValue;
	}

	public String getPhase() {
		return phase;
	}

	public synchronized void setPhase(final String phase) {
		this.phase = phase;
	}

	public String getMessage() {
		return message;
	}

	public synchronized void setMessage(final String message) {
		this.message = message;
	}

	public boolean isAnimate() {
		return animate;
	}

	public synchronized void setAnimate(final boolean animate) {
		this.animate = animate;
	}

	public Boolean isCancellable() {
		return cancellable;
	}

	public void setCancellable(final boolean cancellable) {
		this.cancellable = (cancellable ? CoreConstants.TRUE
				: CoreConstants.FALSE);
	}

	public synchronized void increment() {
		if (currentValue != null) {
			currentValue = new Integer(currentValue.intValue() + 1);
		} else {
			currentValue = new Integer(1);
		}
	}
}
