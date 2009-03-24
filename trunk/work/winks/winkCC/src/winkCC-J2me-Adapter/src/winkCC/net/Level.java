package winkCC.net;

/**
 * Define the logging levels we need. Adapted from the java.util.logging.Level class. 
 * See http://developers.sun.com/techtopics/mobility/midp/ttips/logging/
 * @author WangYinghua
 *
 */
public class Level {

	public static final Level FINE = new Level(1, "FINE");
	public static final Level INFO = new Level(2, "INFO");
	public static final Level WARNING = new Level(3, "WARNING");
	public static final Level SEVERE = new Level(4, "SEVERE");
	public static final Level OFF = new Level(100, "OFF");

	private Level(int level, String name) {
		_level = level;
		_name = "[" + name + "]";
	}

	public int intValue() {
		return _level;
	}

	public String toString() {
		return _name;
	}

	private int _level;
	private String _name;
}
