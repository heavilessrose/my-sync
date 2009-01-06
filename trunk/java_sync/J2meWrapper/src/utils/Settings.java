package utils;

public class Settings {

	private static int canvasWidth = 0;
	private static int canvasHeight = 0;

	private static String imagePath = "/res/images/";

	public static boolean hasVibrate = false;

	/**
	 * **********************************************
	 * Gets the base image path
	 *
	 * @return the base image path
	 *         ***********************************************
	 */
	public static void setCanvas(int width, int height) {
		canvasWidth = width;
		canvasHeight = height;

		// luke remove
		//        GraphicsManager.getInstance().setupCanvas();
	}

	/**
	 * *******************************************
	 * Get the height of the canvas
	 *
	 * @returns the height of the entire canvas
	 * ********************************************
	 */
	public static int getCanvasHeight() {
		return canvasHeight;
	}

	/**
	 * *******************************************
	 * Get the width of the canvas
	 *
	 * @returns the width of the entire canvas
	 * ********************************************
	 */
	public static int getCanvasWidth() {
		return canvasWidth;
	}

	/**
	 * **********************************************
	 * Gets the base image path
	 *
	 * @return the base image path
	 *         ***********************************************
	 */
	public static void setImagePath(String path) {
		imagePath = path;
	}

	/**
	 * **********************************************
	 * Gets the base image path
	 *
	 * @return the base image path
	 *         ***********************************************
	 */
	public static String getImagePath() {
		return imagePath;
	}

	/**
	 * **********************************************
	 * Get the current language settting
	 *
	 * @return ***********************************************
	 */
	public static String getLanguage() {
		return "en_US";
	}
}
