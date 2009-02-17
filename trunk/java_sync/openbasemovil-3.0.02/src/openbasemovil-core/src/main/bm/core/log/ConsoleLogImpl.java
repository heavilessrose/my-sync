package bm.core.log;

public class ConsoleLogImpl implements Log, LogConfigurator {
	private String name;

	public void clear() {

	}

	public void debug(String message) {
		System.out.println("[" + name + " Debug:] - " + message);
	}

	public void debug(String message, Throwable e) {
		System.out.println("[" + name + " Debug:] - " + message);
		e.printStackTrace();
	}

	public void debug(Throwable e) {
		e.printStackTrace();
	}

	public void error(String message) {
		System.err.println("[" + name + " Error:] - " + message);
	}

	public void error(String message, Throwable e) {
		System.err.println("[" + name + " Error:] - " + message);
		e.printStackTrace();
	}

	public void error(Throwable e) {
		e.printStackTrace();
	}

	public void fatal(String message) {
		System.err.println("[" + name + " Fatal:] - " + message);
	}

	public void fatal(String message, Throwable e) {
		System.err.println("[" + name + " Fatal:] - " + message);
		e.printStackTrace();
	}

	public void fatal(Throwable e) {
		e.printStackTrace();
	}

	public String getContent() {
		return null;
	}

	public void info(String message) {
		System.out.println("[" + name + " Info:] - " + message);
	}

	public void warn(String message) {
		System.out.println("[" + name + " Warn:] - " + message);
	}

	public void warn(String message, Throwable e) {
		System.out.println("[" + name + " Warn:] - " + message);
		e.printStackTrace();
	}

	public void warn(Throwable e) {
		e.printStackTrace();
	}

	public void setName(String name) {
		this.name = name;
	}

}
