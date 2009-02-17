package luke.java.practice.treadPool;

public abstract class Task {
	public enum State {
		/* 新建 */NEW, /* 执行中 */RUNNING, /* 已完成 */FINISHED
	}

	// 任务状态
	private State state = State.NEW;

	public void setState(State state) {
		this.state = state;
	}

	public State getState() {
		return state;
	}

	/** 这个任务需要完成的具体工作 */
	public abstract void deal();
}
