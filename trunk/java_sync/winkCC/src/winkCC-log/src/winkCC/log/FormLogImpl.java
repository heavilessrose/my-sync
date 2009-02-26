package winkCC.log;

public class FormLogImpl implements ILog, ILogConfigurator {
	private String shortName;

	private void doLog(final String level, final String message,
			final Throwable e) {

	}

	public void debug(String message) {
		doLog(DEBUG, message, null);
	}

	public void debug(String message, Throwable e) {
		doLog(DEBUG, message, e);
	}

	public void debug(Throwable e) {
		doLog(DEBUG, null, e);
	}

	public void error(String message) {
		doLog(ERROR, message, null);
	}

	public void error(String message, Throwable e) {
		doLog(ERROR, message, e);
	}

	public void error(Throwable e) {
		doLog(ERROR, null, e);
	}

	public void fatal(String message) {
		doLog(FATAL, message, null);
	}

	public void fatal(String message, Throwable e) {
		doLog(FATAL, message, e);
	}

	public void fatal(Throwable e) {
		doLog(FATAL, null, e);
	}

	public void info(String message) {
		doLog(INFO, message, null);
	}

	public void warn(String message) {
		doLog(WARN, message, null);
	}

	public void warn(String message, Throwable e) {
		doLog(WARN, message, e);
	}

	public void warn(Throwable e) {
		doLog(WARN, null, e);
	}

	public void setName(String name) {
		this.shortName = name;
	}

	public void clear() {
		// TODO Auto-generated method stub
		
	}

	public String getContent() {
		// TODO Auto-generated method stub
		return null;
	}

}
