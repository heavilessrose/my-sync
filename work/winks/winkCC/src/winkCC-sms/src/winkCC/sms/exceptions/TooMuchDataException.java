/**
 * A binary payload is encoded using 8-bit data, allowing 140 bytes per segment ¡ª 133 if a port number is used.
 * Sending a larger message may result in an IOException. 
 */
package winkCC.sms.exceptions;

import java.io.IOException;

/**
 * @author WangYinghua
 * 
 */
public class TooMuchDataException extends IOException {
	public TooMuchDataException() {

	}

	public TooMuchDataException(String msg) {
		super(msg);
	}
}
