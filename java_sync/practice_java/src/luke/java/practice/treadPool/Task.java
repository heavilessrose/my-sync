package luke.java.practice.treadPool;

public abstract class Task {
	public enum State {
		/* �½� */NEW, /* ִ���� */RUNNING, /* ����� */FINISHED
	}

	// ����״̬
	private State state = State.NEW;

	public void setState(State state) {
		this.state = state;
	}

	public State getState() {
		return state;
	}

	/** ���������Ҫ��ɵľ��幤�� */
	public abstract void deal();
}
