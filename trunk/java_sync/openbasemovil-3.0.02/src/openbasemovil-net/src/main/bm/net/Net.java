package bm.net;

import bm.net.bind.Binding;
import bm.net.rpc.RemoteCall;
import bm.net.rpc.RemoteCallException;

/*
 * File Information
 *
 * Created on       : 27-nov-2006 8:07:53
 * Created by       : narciso
 * Last modified by : $Author:narciso $
 * Last modified on : $Date:2007-02-03 19:40:06 +0100 (sáb, 03 feb 2007) $
 * Revision         : $Revision:3 $
 */

/**
 * Net package constants and common package configuration.<br/>
 * Net class stores user authentication information, so authenticated calls can
 * pick it directly from here instead of requiring more parameters.<br/>
 * You must configure this settings in order for them to perform the
 * authentication.
 * 
 * @author <a href="mailto:narciso@elondra.org">Narciso Cerezo</a>
 * @version $Revision:3 $
 */
public class Net {
	/**
	 * Error code: Remote call unrecoverable error, probably bad method call.
	 */
	public static final int ERR_RC_UNRECOVERABLE_ERROR = 5001;
	/**
	 * Error code: Remote call not configured, you missed to set the default
	 * remote call url.
	 */
	public static final int ERR_RC_NOT_CONFIGURED = 5002;
	/**
	 * Error code: Remote call invokation, the server threw an error during
	 * method invokation.
	 */
	public static final int ERR_RC_INVOKATION = 5003;

	/**
	 * Error code: Could not open http(s) connection.
	 */
	public static final int ERR_HTTP_OPEN = 5050;
	/**
	 * Error code: Could not open the output stream to send data.
	 */
	public static final int ERR_HTTP_BEGIN_POST = 5051;
	/**
	 * Error code: Server returned an error response code (500).
	 */
	public static final int ERR_HTTP_SERVER = 5052;
	/**
	 * Error code: Server returned an unhandled response code (not 200).
	 */
	public static final int ERR_HTTP_UNHANDLED = 5053;
	/**
	 * Error code: An IO error ocurred while reading or sending data.
	 */
	public static final int ERR_HTTP_IO = 5054;
	/**
	 * Error code: An IO error ocurred while getting url.
	 */
	public static final int ERR_HTTP_GET = 5055;

	/**
	 * Error code: error sending data chunk.
	 */
	public static final int ERR_TRANSPORT_SEND_CHUNK = 5100;
	/**
	 * Error code: the server returned an abort response.
	 */
	public static final int ERR_TRANSPORT_SEND_ABORTED = 5101;
	/**
	 * Error code: The upload ticket has been invalidated by the server.
	 */
	public static final int ERR_TRANS_INVALID_TICKET = 5102;
	/**
	 * Error code: could not write the header.
	 */
	public static final int ERR_TRANS_HEADER_SERIALIZE = 5103;
	/**
	 * Error code: could not read the header.
	 */
	public static final int ERR_TRANS_HEADER_DESERIALIZE = 5104;

	/**
	 * Error code: The requested download ticket is invalid.
	 */
	public static final int ERR_PKTIS_INVALID_TICKET = 5150;
	/**
	 * Error code: The server has encountered an error while preparing the
	 * requested download.
	 */
	public static final int ERR_PKTIS_PREPARING_DOWNLOAD = 5151;
	/**
	 * Error code: The maximum wait time was reached without the download being
	 * ready.
	 */
	public static final int ERR_PKTIS_TIMEOUT = 5152;

	public static final int RC_SUCCESS = 0;
	public static final int RC_INVALID_OBJECT = 1;
	public static final int RC_INVALID_METHOD = 2;
	public static final int RC_NOT_ALLOWED = 3;
	public static final int RC_INVALID_ARGUMENTS = 4;
	public static final int RC_INVOKATION_EXCEPTION = 5;
	public static final int RC_INVALID_CRC = 6;

	public static final int MAX_RETRIES = 5;
	public static final int WAIT_TIME = 5 * 1000;

	public static final int PKT_SMALL = 5120; // 5KB
	public static final int PKT_MEDIUM = 10240; // 10KB
	public static final int PKT_NORMAL = 25600; // 25KB
	public static final int PKT_BIG = 51200; // 50KB
	public static final int PKT_MIN_FREE_MEMORY = PKT_BIG * 3; // Minimum memory
																// to set
																// PKT_BIG

	public static final int PKT_RECEIVED_OK = 0;
	public static final int PKT_SEND_AGAIN = 1;
	public static final int PKT_ABORT = 2;

	// Wait times: maximum wait time: 15 minutes
	public static final long PKTIS_MAX_WAIT = 15 * 60 * 1000;
	// Wait steps: 5s, 10s, 20, and 30s
	public static final long PKTIS_WAIT_TIME[] = new long[] { 5000, 10000,
			20000, 30000 };

	public static final int PKTIS_DOWNLOAD_NOT_READY = -1;
	public static final int PKTIS_INVALID_TICKET = -2;
	public static final int PKTIS_ERROR_PREPARING_DOWNLOAD = -3;

	public static final int NET_STATUS_OK = 0;
	public static final int NET_STATUS_NO_SERVER = 1;
	public static final int NET_STATUS_NO_NETWORK = 2;

	/**
	 * This encryption level means that no encryption is performed, this is the
	 * default level.
	 */
	public static final int NET_ENCRYPTION_LEVEL_NONE = 0;
	/**
	 * Encrypt only sensitive data, such as login and password information.
	 */
	public static final int NET_ENCRYPTION_LEVEL_SENSITIVE = 1;
	/**
	 * Encrypt everything using a custom encryption algorithm. This causes some
	 * overhead.
	 */
	public static final int NET_ENCRYPTION_LEVEL_MEDIUM = 2;
	/**
	 * Encrypt everything using a secure HTTP connection, aka HTTPS.
	 */
	public static final int NET_ENCRYPTION_LEVEL_HIGH = 3;

	private static String deviceGuid;
	private static Long deviceId;
	private static int encryptionLevel = NET_ENCRYPTION_LEVEL_NONE;

	/**
	 * Get the configured device globally unique identifier (GUID).
	 * 
	 * @return device guid, null if not configured
	 */
	public static String getDeviceGuid() {
		return deviceGuid;
	}

	/**
	 * Set the device globally unique identifier (GUID).
	 * 
	 * @param deviceGuid
	 *            device guid
	 */
	public static synchronized void setDeviceGuid(final String deviceGuid) {
		Net.deviceGuid = deviceGuid;
	}

	/**
	 * Get the configured device id, as seen by the server.
	 * 
	 * @return device id
	 */
	public static Long getDeviceId() {
		return deviceId;
	}

	/**
	 * Set the device id.
	 * 
	 * @param deviceId
	 *            device id
	 */
	public static synchronized void setDeviceId(final Long deviceId) {
		Net.deviceId = deviceId;
	}

	/**
	 * Set the encryption level used for network transmissions.<br/>
	 * The default is to use no encryption at all. That's the fastest setting
	 * and is usually enough as all basemovil data is normaly compressed and
	 * packaged in such a way that will take some time to figure out what is
	 * being transferred.<br/>
	 * Nevertheless, for some small pieces of data compression might not be
	 * used, for example for login in. If you set encryption level to SENSITIVE,
	 * this kind of information will be encrypted using a custom algorithm.
	 * Currently Blowfish is the algorithm used, it's a strong encryption
	 * algorithm with a low foot print in memory and a good speed.<br/>
	 * The best solution would be to turn securityLevel to HIGH, but that's not
	 * nowadays a good one. HIGH level uses HTTPS instead of HTTP, and many
	 * phones out there do not support HTTPS or do not have the required root
	 * certificates, creating a usability nightmare.<br/>
	 * If you're very concerned about security and are willing to loose some
	 * speed for it, then you can set it to MEDIUM. This level uses the same
	 * encryption algorithm as SENSITIVE but for everything, not just sensitive
	 * information.
	 * 
	 * @return encryption level
	 */
	public static int getEncryptionLevel() {
		return encryptionLevel;
	}

	/**
	 * Set the encryption level.
	 * 
	 * @param encryptionLevel
	 *            encryption level
	 */
	public static void setEncryptionLevel(final int encryptionLevel) {
		Net.encryptionLevel = encryptionLevel;
	}

	/**
	 * Check network connectivity.<br/>
	 * This method first makes a ping to the bmServer ping service, if that's ok
	 * it returns NET_STATUS_OK.<br/>
	 * If that fails, it tries to get a well known page from the Internet, to
	 * check if the problem is on the network or on the bmServer. If this goes
	 * ok, it returns NET_STATUS_NO_SERVER, otherwise NET_STATUS_NOT_NETWORK.
	 * 
	 * @return status code
	 */
	public static int checkNetwork() {
		try {
			// 1. Call basemovil server ping service
			final RemoteCall rc = new RemoteCall("LoginHandler", "ping", 0);
			rc.invoke();
			return NET_STATUS_OK;
		} catch (RemoteCallException e) {
			return NET_STATUS_NO_SERVER;
		} catch (NetException e) {
			final Binding binding = Binding.create();
			try {
				binding.get("http://www.yahoo.com");
				return NET_STATUS_NO_SERVER;
			} catch (NetException e1) {
				return NET_STATUS_NO_NETWORK;
			} finally {
				binding.close();
			}
		}
	}
}
