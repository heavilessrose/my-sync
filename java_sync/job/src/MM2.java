public class MM2 extends MM {
	public MM2() {
		super();
	}

	protected void ff() {
		// super.ff();
		System.out.println("MM2: ff()");
	}

	public void ff(int i) {
		// super.ff();
		// super.ff(3);
		System.out.println("MM2: " + i);
	}

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		new MM2();
	}

}
