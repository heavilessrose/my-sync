package utils;

import java.util.Random;

public final class RandomUtil {

	//	private static RandomUtil singleton = null;
	private static Random r;

	static {
		r = new Random();
	}

	/**
	 * 
	 * @return [0, Integer.MAX_VALUE]
	 */
	public static int getInt() {
		return Math.abs(r.nextInt());
	}

	/**
	 * 
	 * @param max
	 * @return [0, max]
	 */
	public static int getInt(int max) {
		return getInt(0, (max + 1));
	}

	/**
	 * 
	 * @param min
	 * @param max
	 * @return [min, max-1]
	 */
	public static int getInt(int min, int max) {
		if (min < 0)
			max += Math.abs(min);

		return ((Math.abs(r.nextInt()) % max) + min);
	}

	public static void setSeed(long l) {
		r.setSeed(l);
	}
}
