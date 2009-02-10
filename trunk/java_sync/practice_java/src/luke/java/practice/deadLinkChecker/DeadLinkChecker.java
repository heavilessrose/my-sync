package luke.java.practice.deadLinkChecker;

public class DeadLinkChecker {

	private String page;

	public DeadLinkChecker(String page) {
		this.page = page;
	}

	public static void main(String[] args) {
		DeadLinkChecker checker = new DeadLinkChecker(args[0]);
	}
}
