package bm.core.log;

public class ConsoleLogImpl implements Log, LogConfigurator {
	private String name;

	public void clear() {

	}

	public void debug(String message) {
		System.out.println("[" + name + " Debug:]\n" + message);
	}

	public void debug(String message, Throwable e) {
		System.out.println("[" + name + " Debug:]\n" + message);
		e.printStackTrace();
	}

	public void debug(Throwable e) {
		e.printStackTrace();
	}

	public void error(String message) {
		System.err.println("[" + name + " Error:]\n" + message);
	}

	public void error(String message, Throwable e) {
		System.err.println("[" + name + " Error:]\n" + message);
		e.printStackTrace();
	}

	public void error(Throwable e) {
		e.printStackTrace();
	}

	public void fatal(String message) {
		System.err.println("[" + name + " Fatal:]\n" + message);
	}

	public void fatal(String message, Throwable e) {
		System.err.println("[" + name + " Fatal:]\n" + message);
		e.printStackTrace();
	}

	public void fatal(Throwable e) {
		e.printStackTrace();
	}

	public String getContent() {
		return null;
	}

	public void info(String message) {
		System.out.println("[" + name + " Info:]\n" + message);
	}

	public void warn(String message) {
		System.out.println("[" + name + " Warn:]\n" + message);
	}

	public void warn(String message, Throwable e) {
		System.out.println("[" + name + " Warn:]\n" + message);
		e.printStackTrace();
	}

	public void warn(Throwable e) {
		e.printStackTrace();
	}

	public void setName(String name) {
		this.name = name;
	}

}
