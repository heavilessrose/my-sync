/*
 * IP Messenger Main Class
 *		1997/10/3 (C)Copyright T.Kazawa
 */

package ipmsg;

import JP.digitune.util.ResourceProperties;
import JP.digitune.util.StringReplacer;

import java.awt.*;
//import jp.kyasu.awt.*;
import java.awt.event.*;
import java.io.*;
import java.net.*;
import java.util.*;
import java.text.*;

import java.math.*;
import java.security.*;
import java.security.interfaces.*;
import java.security.spec.*;
import javax.crypto.*;
import javax.crypto.spec.*;

public class IPMsg implements IPMComListener {
	/*========== Constant Value ==========*/
	public static final long IPMSG_COMMASK		= 0x000000ffL;
	public static final long IPMSG_OPTMASK		= 0xffffff00L;
	
	public static final long IPMSG_NOOPERATION	= 0x00000000L;
	
	public static final long IPMSG_BR_ENTRY		= 0x00000001L;
	public static final long IPMSG_BR_EXIT		= 0x00000002L;
	public static final long IPMSG_ANSENTRY		= 0x00000003L;
	public static final long IPMSG_BR_ABSENCE	= 0x00000004L;
	
	public static final long IPMSG_BR_ISGETLIST = 0x00000018L;
	public static final long IPMSG_OKGETLIST	= 0x00000015L;
	public static final long IPMSG_GETLIST		= 0x00000016L;
	public static final long IPMSG_ANSLIST		= 0x00000017L;
	
	public static final long IPMSG_SENDMSG		= 0x00000020L;
	public static final long IPMSG_RECVMSG		= 0x00000021L;
	
	public static final long IPMSG_READMSG		= 0x00000030L;
	public static final long IPMSG_DELMSG		= 0x00000031L;
	
	public static final long IPMSG_GETINFO		= 0x00000040L;
	public static final long IPMSG_SENDINFO		= 0x00000041L;
	
	public static final long IPMSG_GETPUBKEY	= 0x00000072L;
	public static final long IPMSG_ANSPUBKEY	= 0x00000073L;
	
	// other opt
	public static final long IPMSG_ABSENCEOPT		= 0x00000100L;
	public static final long IPMSG_SERVEROPT		= 0x00000200L;
	public static final long IPMSG_DIALUPOPT		= 0x00010000L;
	public static final long IPMSG_ENCRYPTOPT		= 0x00400000L;
	public static final long IPMSG_ENCRYPTOPTOLD	= 0x00800000L;
	
	// send opt
	public static final long IPMSG_SENDCHECKOPT = 0x00000100L;
	public static final long IPMSG_SECRETOPT	= 0x00000200L;
	public static final long IPMSG_BROADCASTOPT = 0x00000400L;
	public static final long IPMSG_MULTICASTOPT = 0x00000800L;
	public static final long IPMSG_NOPOPUPOPT	= 0x00001000L;
	public static final long IPMSG_AUTORETOPT	= 0x00002000L;
	public static final long IPMSG_RETRYOPT		= 0x00004000L;
	public static final long IPMSG_PASSWORDOPT	= 0x00008000L;
	public static final long IPMSG_NOLOGOPT		= 0x00020000L;
	public static final long IPMSG_NEWMUTIOPT	= 0x00040000L;

	// encrypt opt
	public static final long IPMSG_RSA_512		= 0x00000001L;
	public static final long IPMSG_RSA_1024		= 0x00000002L;
	public static final long IPMSG_RSA_2048		= 0x00000004L;
	public static final long IPMSG_RC2_40		= 0x00001000L;
	public static final long IPMSG_RC2_128		= 0x00004000L;
	public static final long IPMSG_RC2_256		= 0x00008000L;
	public static final long IPMSG_BLOWFISH_128 = 0x00020000L;
	public static final long IPMSG_BLOWFISH_256 = 0x00040000L;
	
	public static final int	 MAXBUF				= 8192;
	/*========== end ==========*/
	
	private static final String P_FILE = "runtime.properties";
	private ResourceProperties pref = new ResourceProperties("ipmsg.resources");	
	private boolean state = false;
	private InetAddress localaddr;
	private long serial = 0;
	private String user, host;
	private Hashtable userlist = new Hashtable();
	private Hashtable dialupmember = new Hashtable();
	private Hashtable ipmlistener = new Hashtable();
	private Hashtable recentsend = new Hashtable();
	private int port;
	private int[] ports;
	private DatagramSocket dsock;
	private IPMProxy proxy;
	private int receivecount = 0;

	private class CryptoInfo {
		private long cap = 0L;
		private PublicKey publickey = null;
		public CryptoInfo(long cap, PublicKey publickey) {
			this.cap = cap;
			this.publickey = publickey;
		}
		public long getCap() {
			return cap;
		}
		public PublicKey getPublicKey() {
			return publickey;
		}
	}
	private boolean hasJCE = true;
	private Hashtable publickeys = new Hashtable();
	private PublicKey publickey = null;
	private PrivateKey privatekey = null;
	private long getCryptoCaps() {
		return IPMSG_RSA_512 | IPMSG_RSA_1024
			| IPMSG_RC2_40 | IPMSG_RC2_128
			| IPMSG_BLOWFISH_128;
	}
	
	String makeKey(IPMComEvent ipme) {
		IPMPack tmppack = ipme.getPack();
		String key = ipme.getIPMAddress() + ":"
			+ tmppack.getUser() + ":" + tmppack.getHost();
		return key;
	}
	
	public void receive(IPMComEvent ipme) {
		if (!state)
			return;
		IPMPack tmppack = ipme.getPack();
		long opt = tmppack.getCommand() & IPMSG_OPTMASK;
		
		switch ((int) (tmppack.getCommand() & IPMSG_COMMASK)) {
		case (int) IPMSG_BR_ENTRY:
			String nickname = pref.getProperty("nickName");
			IPMSend.send(dsock, makePack(IPMSG_ANSENTRY | getEntryOpt()
				, nickname, true), ipme.getIPMAddress());
		case (int) IPMSG_ANSENTRY:
			if (publickey != null)
				IPMSend.send(dsock, makePack(IPMSG_GETPUBKEY
					, Long.toString(getCryptoCaps(), 16).toUpperCase(), false)
					, ipme.getIPMAddress());
		case (int) IPMSG_BR_ABSENCE:
			userlist.put(makeKey(ipme), ipme);
			if ((opt & IPMSG_DIALUPOPT) != 0
				&& dialupmember.get(ipme.getIPMAddress().toString()) == null)
				dialupmember.put(ipme.getIPMAddress().toString()
					, ipme.getIPMAddress());
			IPMEvent ie = new IPMEvent(this, IPMEvent.UPDATELIST_EVENT
				, new Date(System.currentTimeMillis()), ipme);
			invokeListener(ie);
			break;
		case (int) IPMSG_BR_EXIT:
			userlist.remove(makeKey(ipme));
			dialupmember.remove(ipme.getIPMAddress().toString());
			publickeys.remove(ipme.getIPMAddress().toString());
			ie = new IPMEvent(this, IPMEvent.UPDATELIST_EVENT
				, new Date(System.currentTimeMillis()), ipme);
			invokeListener(ie);
			break;
		case (int) IPMSG_SENDMSG:
			if ((opt & IPMSG_SENDCHECKOPT) != 0)
				IPMSend.send(dsock
					, makePack(IPMSG_RECVMSG | IPMSG_AUTORETOPT
					, new Long(tmppack.getNo()).toString(), false)
					, ipme.getIPMAddress());
			if (new Boolean(pref.getProperty("absenceState")).booleanValue()
				&& (opt & IPMSG_AUTORETOPT) == 0)
				try {
					String tmpmsg = pref.getProperty("absenceMsg");
					if (!tmpmsg.equals(""))
						IPMSend.send(dsock
							, makePack(IPMSG_SENDMSG | IPMSG_AUTORETOPT
							, tmpmsg, false), ipme.getIPMAddress());
				} catch (MissingResourceException ex) {}
			if (!userlist.containsKey(makeKey(ipme))) {
				nickname = pref.getProperty("nickName");
				IPMSend.send(dsock
					, makePack(IPMSG_BR_ENTRY | IPMSG_AUTORETOPT
					| getEntryOpt(), nickname, true)
					, ipme.getIPMAddress());
			}
			if ((opt & IPMSG_ENCRYPTOPT) != 0)
				tmppack.setExtra(decryptMessage(tmppack.getExtra()));
			ie = new IPMEvent(this, IPMEvent.RECEIVEMSG_EVENT
				, new Date(System.currentTimeMillis()), ipme);
			invokeListener(ie);
			break;
		case (int) IPMSG_RECVMSG:
			try {
				Long tmpLong = new Long(tmppack.getExtra());
				NormalSend ns = (NormalSend) recentsend.get(tmpLong);
				if (ns != null) {
					ns.receiveReply();
					recentsend.remove(tmpLong);
				}
			} catch (NumberFormatException ex) {}
			break;
		case (int) IPMSG_READMSG:
			ie = new IPMEvent(this, IPMEvent.READMSG_EVENT
				, new Date(System.currentTimeMillis()), ipme);
			invokeListener(ie);
			break;
		case (int) IPMSG_DELMSG:
			ie = new IPMEvent(this, IPMEvent.DELETEMSG_EVENT
				, new Date(System.currentTimeMillis()), ipme);
			invokeListener(ie);
			break;
		case (int) IPMSG_GETPUBKEY:
			IPMSend.send(dsock, makePack(IPMSG_ANSPUBKEY
				, answerPublicKey(ipme.getPack()), false)
				, ipme.getIPMAddress());
			break;
		case (int) IPMSG_ANSPUBKEY:
			receivePublicKey(ipme);
			break;
		case (int) IPMSG_GETINFO:
			IPMSend.send(dsock, makePack(IPMSG_SENDINFO
				, pref.getProperty("version") + " ("
				+ System.getProperty("java.vendor") + " ver."
				+ System.getProperty("java.version") + "/"
				+ System.getProperty("os.name") + " "
				+ System.getProperty("os.version") + ")", false)
				, ipme.getIPMAddress());
			break;
		}
	}
	
	String[] cutCString(String cstr) {
		StringTokenizer tokenizer = new StringTokenizer(cstr, ",");
		String[] tmpstrs = new String[tokenizer.countTokens()];
		for (int i = 0; i < tmpstrs.length; i++)
			tmpstrs[i] = tokenizer.nextToken().trim();
		return tmpstrs;
	}
	
	IPMPack makePack(long com, String extra, boolean groupflag) {
		if (groupflag)
			return new IPMPack(1, getSerial(), user, host, com, extra
				, pref.getProperty("groupName"));
		else
			return new IPMPack(1, getSerial(), user, host, com, extra, null);
	}
	
	synchronized long getSerial() {
		return (System.currentTimeMillis() >> 10) + serial++;
	}
	
	IPMAddress[] getBroadcastAddr() {
		IPMAddress[] tmpaddr;
		int dialup = dialupmember.size();
		try {
			String tmpstr = pref.getProperty("broadcastAddr");
			String[] strbroadcasts = cutCString("255.255.255.255," + tmpstr);
			tmpaddr = new IPMAddress[strbroadcasts.length * ports.length
				+ dialup];
			for (int i = 0; i < strbroadcasts.length; i++)
				for (int j = 0; j < ports.length; j++)
					try {
						tmpaddr[i * ports.length + j]
							= new IPMAddress(ports[j]
								, InetAddress.getByName(strbroadcasts[i]));
					} catch (UnknownHostException ex) {
						tmpaddr[i * ports.length + j] = null;
					}
		} catch (MissingResourceException ex) {
			tmpaddr = new IPMAddress[ports.length + dialup];
			for (int i = 0; i < ports.length; i++)
				try {
					tmpaddr[i] = new IPMAddress(ports[i]
						, InetAddress.getByName("255.255.255.255"));
				} catch (UnknownHostException exx) {
					tmpaddr[i] = null;
				}
		}
		Enumeration enum = dialupmember.elements();
		for (int i = 0; i < dialup; i++)
			tmpaddr[i + tmpaddr.length - dialup]
				= (IPMAddress) enum.nextElement();
		return tmpaddr;
	}
	
	synchronized void invokeListener(IPMEvent tmpevent) {
		for (Enumeration enum = ipmlistener.elements()
			; enum.hasMoreElements(); ) {
			IPMListener listener = (IPMListener) enum.nextElement();
			listener.eventOccured(tmpevent);
		}
	}

	private static String bytesToString(byte[] b) {
		StringBuffer strbuf = new StringBuffer();
		for (int i = 0; i < b.length; i++) {
			int tmpb = (b[i] < 0) ? ((int) b[i]) + 0x100 : b[i];
			strbuf.append(Integer.toString((int)(tmpb / 16),16).toUpperCase());
			strbuf.append(Integer.toString((int)(tmpb % 16),16).toUpperCase());
		}
		return new String(strbuf);
	}

	byte[] stringToBytes(String src) {
		byte[] buf = new byte[src.length() / 2];
		for (int i = 0; i < src.length(); i += 2) {
			int b = Integer.parseInt(src.substring(i, ((i + 2) > src.length())
				? src.length() : i + 2), 16);
			b = (b > 127) ? b - 0x100 : b;
			buf[i / 2] = (byte) b;
		}
		return buf;
	}

	byte[] reverseBytes(byte[] src) {
		for (int i = 0; i < src.length / 2; i++) {
			byte b = src[i];
			src[i] = src[src.length - i - 1];
			src[src.length - i - 1] = b;
		}
		return src;
	}

	String encryptMessage(CryptoInfo ci, String src) {
		if (ci == null || ci.getPublicKey() == null)
			throw new IllegalStateException("publickey unknown.");
		try {
			long flag = 0L;
			int key_length = ci.getPublicKey().getEncoded().length;
			if (key_length > 200)
				flag |= IPMSG_RSA_2048;
			else if (key_length > 100)
				flag |= IPMSG_RSA_1024;
			else
				flag |= IPMSG_RSA_512;
			StringBuffer strbuf = new StringBuffer();
			SecretKey secretkey = null;
			String cipher_name = null;
			if ((ci.getCap() & IPMSG_BLOWFISH_128) != 0) {
				flag |= IPMSG_BLOWFISH_128;
				KeyGenerator kg = KeyGenerator.getInstance("Blowfish");
				kg.init(128);
				secretkey = kg.generateKey();
				cipher_name = pref.getProperty("Cipher2");
			} else if ((ci.getCap() & IPMSG_RC2_128) != 0) {
				flag |= IPMSG_RC2_128;
				KeyGenerator kg = KeyGenerator.getInstance("RC2");
				kg.init(128);
				secretkey = kg.generateKey();
				cipher_name = pref.getProperty("Cipher3");
			} else if ((ci.getCap() & IPMSG_RC2_40) != 0) {
				flag |= IPMSG_RC2_40;
				KeyGenerator kg = KeyGenerator.getInstance("RC2");
				kg.init(40);
				secretkey = kg.generateKey();
				cipher_name = pref.getProperty("Cipher3");
			} else {
				throw new IllegalStateException("no cap!");
			}
			strbuf.append(Long.toString(flag, 16).toUpperCase());
			strbuf.append(":");
			Cipher c = Cipher.getInstance(pref.getProperty("Cipher1"));
			c.init(Cipher.ENCRYPT_MODE, ci.getPublicKey());
			byte[] keydata = c.doFinal(secretkey.getEncoded());
			strbuf.append(bytesToString(keydata));
			strbuf.append(":");
			c = Cipher.getInstance(cipher_name);
			IvParameterSpec iv = new IvParameterSpec(new byte[8]);
			c.init(Cipher.ENCRYPT_MODE, secretkey, iv);
			byte[] msgdata = c.doFinal(src.getBytes("SJIS"));
			strbuf.append(bytesToString(msgdata));
			return new String(strbuf);
		} catch (Exception ex) {
			ex.printStackTrace();
		}
		return src;
	}

	String decryptMessage(String src) {
		if (privatekey == null)
			return src;
		try {
			StringTokenizer token = new StringTokenizer(src, ":");
			long cap = Long.parseLong(token.nextToken().toLowerCase(), 16);
			String cipher_name = pref.getProperty("Cipher2");
			if ((cap & (IPMSG_BLOWFISH_128 | IPMSG_BLOWFISH_256)) == 0) {
				cipher_name = pref.getProperty("Cipher3");
			}
			byte[] keydata = stringToBytes(token.nextToken());
			byte[] msgdata = stringToBytes(token.nextToken());
			Cipher c = Cipher.getInstance(pref.getProperty("Cipher1"));
			c.init(Cipher.DECRYPT_MODE, privatekey);
			byte[] skey = c.doFinal(keydata);
			c = Cipher.getInstance(cipher_name);
			IvParameterSpec iv = new IvParameterSpec(new byte[8]);
			SecretKeySpec sks1 = new SecretKeySpec(skey
				, pref.getProperty("Cipher2").substring(0
				, pref.getProperty("Cipher2").indexOf("/")));
			c.init(Cipher.DECRYPT_MODE, sks1, iv);
			byte[] msg = c.doFinal(msgdata);
			return new String(msg, "SJIS");
		} catch (Exception ex) {
			ex.printStackTrace();
			return "decrypt error.";
		}
	}

	String answerPublicKey(IPMPack pack) {
		if (publickey == null)
			return "0";
		StringBuffer result = new StringBuffer();
		try {
			result.append(Long.toString(getCryptoCaps(), 16).toUpperCase());
			result.append(":");
			result.append(
				makeRSAPublicKeyString((RSAPublicKey) publickey));
			return new String(result);
		} catch (Exception ex) {
			ex.printStackTrace();
			return "0";
		}
	}

	void receivePublicKey(IPMComEvent ipme) {
		if (publickey == null)
			return;
		try {
			StringTokenizer token
				= new StringTokenizer(ipme.getPack().getExtra(), ":");
			long cap = Long.parseLong(token.nextToken(), 16);
			KeyFactory kf = KeyFactory.getInstance("RSA");
			RSAPublicKeySpec keyspec = makePublicKeySpec(token.nextToken());
			PublicKey pubkey = kf.generatePublic(keyspec);
			CryptoInfo ci = new CryptoInfo(cap, pubkey);
			publickeys.put(ipme.getIPMAddress().toString(), ci);
		} catch (Exception ex) {
			ex.printStackTrace();
		}
	}
	
	RSAPublicKeySpec makePublicKeySpec(String src) throws Exception {
		StringTokenizer token = new StringTokenizer(src, "-");
		BigInteger exponent = new BigInteger(token.nextToken(), 16);
		BigInteger modulus = new BigInteger(token.nextToken(), 16);
		return new RSAPublicKeySpec(modulus, exponent);
	}

	RSAPrivateKeySpec makePrivateKeySpec(String src) throws Exception {
		StringTokenizer token = new StringTokenizer(src, "-");
		BigInteger exponent = new BigInteger(token.nextToken(), 16);
		BigInteger modulus = new BigInteger(token.nextToken(), 16);
		return new RSAPrivateKeySpec(modulus, exponent);
	}

	String makeRSAPublicKeyString(RSAPublicKey key) throws Exception {
		return key.getPublicExponent().toString(16).toUpperCase() + "-"
			+ key.getModulus().toString(16).toUpperCase();
	}

	String makeRSAPrivateKeyString(RSAPrivateKey key) throws Exception {
		return key.getPrivateExponent().toString(16).toUpperCase() + "-"
			+ key.getModulus().toString(16).toUpperCase();
	}

	long setupEncryption() {
		if (publickey != null && privatekey != null)
			return IPMSG_ENCRYPTOPT;
		else if (!hasJCE)
			return 0L;
		try {
			Class c = Class.forName(pref.getProperty("jceProvider"));
			Security.addProvider((Provider) c.newInstance());
		} catch (Exception ex) {
			System.err.println("Can't instanciate JCE Provider.->" + ex);
			hasJCE = false;
			return 0L;
		}
		try {
			try {
				KeyFactory kf = KeyFactory.getInstance("RSA");
				RSAPublicKeySpec pubspec
					= makePublicKeySpec(pref.getProperty("publicKey"));
				publickey = kf.generatePublic(pubspec);
				RSAPrivateKeySpec privspec
					= makePrivateKeySpec(pref.getProperty("privateKey"));
				privatekey = kf.generatePrivate(privspec);
			} catch (Exception e) {
				KeyPairGenerator kpg = KeyPairGenerator.getInstance("RSA");
				kpg.initialize(1024);
				KeyPair kp = kpg.generateKeyPair();
				publickey = kp.getPublic();
				privatekey = kp.getPrivate();
				pref.setProperty("publicKey", makeRSAPublicKeyString(
					(RSAPublicKey) publickey));
				pref.setProperty("privateKey", makeRSAPrivateKeyString(
					(RSAPrivateKey) privatekey));
			}
		} catch (Exception ex) {
			System.err.println("Can't create key pair.->" + ex);
			return 0L;
		}
		return IPMSG_ENCRYPTOPT;
	}

	long getEntryOpt() {
		long absence = (new Boolean(pref.getProperty("absenceState"))
			.booleanValue()) ? IPMSG_ABSENCEOPT : 0;
		long dialup = (new Boolean(pref.getProperty("dialupState"))
			.booleanValue()) ? IPMSG_DIALUPOPT: 0;
		return absence | dialup | setupEncryption();
	}
	
	void makeBroadcastRecvSockets() throws SocketException {
		try {
			String tmpstr = pref.getProperty("ports");
			String[] strports = cutCString(tmpstr);
			ports = new int[strports.length];
			for (int i = 0; i < ports.length; i++)
				ports[i] = Integer.parseInt(strports[i]);
		} catch (MissingResourceException ex) {
			ports = new int[1];
			ports[0] = 2425;
		}
		DatagramSocket[] tmpds = new DatagramSocket[ports.length];
		for (int i = 0; i < ports.length; i++)
			tmpds[i] = new DatagramSocket(ports[i]);
		InetAddress proxyaddr = null;
		try {
			String tmpstr = pref.getProperty("proxy");
			proxyaddr = InetAddress.getByName(tmpstr);
		} catch (Exception ex) {
			proxyaddr = null;
		}
		proxy = new IPMProxy(this, proxyaddr, new Boolean(
			pref.getProperty("proxyBroadcastAll")).booleanValue());
		for (int i = 0; i < ports.length; i++) {
			proxy.addBroadcastPort(ports[i]);
			IPMRecv tmprecv = new IPMRecv(tmpds[i]);
			tmprecv.addIPMComListener(this);
			tmprecv.addIPMComListener(proxy);
			tmprecv.start();
		}
	}
	
	class Child extends Thread {
		private DataInputStream din;
		
		public Child() {
			this.start();
		}
		
		public void run() {
			Socket sock;
			while (true) {
				try {
					sock = new Socket(localaddr, IPMProxy.PROXY_PORT);
					din = new DataInputStream(sock.getInputStream());
				} catch (IOException ex) {
					return;
				}
				byte[] buf = new byte[8192];
				IPMByteBuffer ipmbb = new IPMByteBuffer();
				outer:
				while (true) {
					while (!ipmbb.eop()) {
						int count = 0;
						try {
							count = din.read(buf);
						} catch (IOException ex) {
							break outer;
						}
						if (count == -1)
							break outer;
						ipmbb.append(buf, 0, count);
					}
					buf = ipmbb.getBytes();
					IPMProxyEvent ipmpe = new IPMProxyEvent(this, buf);
					try {
						if (!ipmpe.getToIPMAddress().getInetAddress()
							.equals(InetAddress.getByName("255.255.255.255")))
							continue;
					} catch (UnknownHostException ex) {
						continue;
					}
					IPMComEvent ipmce = new IPMComEvent(this
						, ipmpe.getToIPMAddress().getPort(), ipmpe.getPack()
						, ipmpe.getFromIPMAddress());
					receive(ipmce);
				}
				try {
					makeBroadcastRecvSockets();
					try {
						din.close();
						sock.close();
					} catch (IOException ex) {}
					return;
				} catch (SocketException ex) {
					ex.printStackTrace();
				}
			}
		}
	}
	
	public IPMsg() {
		try {
			FileInputStream filein = new FileInputStream(P_FILE);
			pref.load(filein);
			filein.close();
		} catch (FileNotFoundException ex) {
		} catch (IOException ex) {}
		user = System.getProperty("user.name", "No Name");
		try {
			host = InetAddress.getLocalHost().getHostName();
		} catch (UnknownHostException ex) {
			host = "Unknown Hostname";
		}
		try {
			try {
				localaddr
					= InetAddress.getByName(pref.getProperty("localAddress"));
			} catch (MissingResourceException ex) {
				localaddr = InetAddress.getLocalHost();
			}
		} catch (UnknownHostException ex) {
			ex.printStackTrace();
		}
	}
	
	public InetAddress getLocalAddress() {
		return localaddr;
	}
	
	public void entry() {
		DatagramSocket tmpds;
		Random rand = new Random(System.currentTimeMillis());
		while (true) {
			/* 49154 - 65534 */
			port = (int) (rand.nextLong() % 8192 + 57342);
			try {
				tmpds = new DatagramSocket(port);
			} catch (SocketException ex) {
				continue;
			}
			break;
		}
		dsock = tmpds;
		IPMRecv tmprecv = new IPMRecv(tmpds);
		tmprecv.addIPMComListener(this);
		tmprecv.start();
		try {
			makeBroadcastRecvSockets();
		} catch (SocketException ex) {
			ex.printStackTrace();
			new Child();
		}
		state = true;
		String nickname = pref.getProperty("nickName");
		new BroadcastSend(dsock, makePack(IPMSG_BR_ENTRY | getEntryOpt()
			, nickname, true), getBroadcastAddr());
	}
	
	public synchronized void savePref() {
		try {
			FileOutputStream fileout = new FileOutputStream(P_FILE);
			pref.save(fileout, "IP Messenger Runtime Resource");
		} catch (IOException ex) {}
	}
	
	public synchronized void exit() {
		state = false;
		BroadcastSend bs = new BroadcastSend(dsock, makePack(IPMSG_BR_EXIT
			, "", false), getBroadcastAddr());
		try {
			bs.join();
		} catch (InterruptedException ex) {
			ex.printStackTrace();
		}
		savePref();
	}
	
	public synchronized void addIPMListener(IPMListener listener) {
		ipmlistener.put(listener, listener);
	}
	
	public synchronized void removeIPMListener(IPMListener listener) {
		ipmlistener.remove(listener);
	}
	
	public synchronized void setPref(String key, String value) {
		pref.put(key, value);
	}
	
	public synchronized String getPref(String key) {
		return pref.getProperty(key);
	}
	
	public Hashtable getUserlist() {
		return userlist;
	}

	public synchronized void incReceiveCount() {
		receivecount++;
	}
	public synchronized void decReceiveCount() {
		receivecount--;
	}
	public boolean lessThanReceiveMax() {
		int recvmax = 500;
		try {
			recvmax = Integer.parseInt(pref.getProperty("receiveMax", "500"));
		} catch (NumberFormatException ex) {}
		return (recvmax > receivecount);
	}

	public void sendMsg(IPMAddress[] addrs, String msg, long flag) {
		if (addrs == null) {
			IPMPack ipmp = makePack(IPMSG_SENDMSG | IPMSG_BROADCASTOPT | flag
				, msg, false);
			BroadcastSend bs
				= new BroadcastSend(dsock, ipmp, getBroadcastAddr());
		} else if (addrs.length == 1) {
			if (publickeys.containsKey(addrs[0].toString())) {
				try {
					msg = encryptMessage(
						(CryptoInfo) publickeys.get(addrs[0].toString()), msg);
					flag |= IPMSG_ENCRYPTOPT;
				} catch (Exception ex) {}
			}
			IPMPack ipmp = makePack(IPMSG_SENDMSG | IPMSG_SENDCHECKOPT
				| flag, msg, false);
			NormalSend ns = new NormalSend(dsock, ipmp, addrs[0]);
			ns.addIPMComListener(new IPMComListener() {
				public void receive(IPMComEvent ipmce) {
					recentsend.remove(new Long(ipmce.getPack().getNo()));
					IPMEvent ie = new IPMEvent(this, IPMEvent.CANTSENDMSG_EVENT
						, new Date(System.currentTimeMillis()), ipmce);
					invokeListener(ie);
				}
			});
			recentsend.put(new Long(ipmp.getNo()), ns);
			ns.start();
		} else {
			for (int i = 0; i < addrs.length; i++) {
				long tmpflag = flag;
				String tmpmsg = msg;
				if (publickeys.containsKey(addrs[i].toString())) {
					try {
						tmpmsg = encryptMessage(
							(CryptoInfo) publickeys.get(addrs[i].toString())
							, msg);
						tmpflag |= IPMSG_ENCRYPTOPT;
					} catch (Exception ex) {}
				}
				IPMPack ipmp = makePack(IPMSG_SENDMSG | IPMSG_MULTICASTOPT
					| tmpflag, tmpmsg, false);
				IPMSend.send(dsock, ipmp, addrs[i]);
			}
		}
	}
	
	public void sendReadMsg(IPMEvent ipme) {
		IPMPack ipmp = makePack(IPMSG_READMSG | IPMSG_AUTORETOPT
			, new Long(ipme.getPack().getNo()).toString(), false);
		IPMSend.send(dsock, ipmp, ipme.getIPMAddress());
	}
	
	public void sendDeleteMsg(IPMEvent ipme) {
		IPMPack ipmp = makePack(IPMSG_DELMSG | IPMSG_AUTORETOPT
			, new Long(ipme.getPack().getNo()).toString(), false);
		IPMSend.send(dsock, ipmp, ipme.getIPMAddress());
	}
	
	public void getInfo() {
		BroadcastSend bs = new BroadcastSend(dsock, makePack(IPMSG_GETINFO
			, "", false), getBroadcastAddr());
	}
	
	public synchronized void refreshList() {
		IPMAddress[] tmpaddr = getBroadcastAddr();
		userlist = new Hashtable();
		dialupmember = new Hashtable();
		String nickname = pref.getProperty("nickName");
		new BroadcastSend(dsock, makePack(IPMSG_BR_ENTRY | getEntryOpt()
			, nickname, true), tmpaddr);
	}
	
	public void absenceStateChanged() {
		String nickname = pref.getProperty("nickName");
		new BroadcastSend(dsock, makePack(IPMSG_BR_ABSENCE | getEntryOpt()
			, nickname, true), getBroadcastAddr());
	}
	
	public String makeDateStr(Date now) {
		SimpleDateFormat dateformatter
			= new SimpleDateFormat("EEE MMM dd HH:mm:ss yyyy", Locale.US);
		dateformatter.setTimeZone(
			DateFormat.getDateInstance().getTimeZone());
		return "at " + dateformatter.format(now);
	}
	
	public String makeListStr(IPMPack aPack) {
		String tmpuser = null;
		if (aPack.getExtra() == null)
			tmpuser = aPack.getUser();
		else
			tmpuser = aPack.getExtra();
		StringBuffer strbuf = new StringBuffer();
		if ((aPack.getCommand() & IPMsg.IPMSG_ABSENCEOPT) != 0)
			tmpuser = tmpuser + "*";
		strbuf.append(tmpuser + " (");
		if (aPack.getGroup() != null)
			strbuf.append(aPack.getGroup() + "/");
		strbuf.append(aPack.getHost() + ")");
		return new String(strbuf);
	}

	public String[] makeListStrs(IPMPack aPack) {
		String tmpuser = "", tmpgroup = "", tmphost = "";
		if (aPack.getExtra() == null)
			tmpuser = aPack.getUser();
		else
			tmpuser = aPack.getExtra();
		if ((aPack.getCommand() & IPMsg.IPMSG_ABSENCEOPT) != 0)
			tmpuser = tmpuser + "*";
		if (aPack.getGroup() != null)
			tmpgroup = aPack.getGroup();
		tmphost = aPack.getHost();
		String[] result = { tmpuser, tmpgroup, tmphost };
		return result;
	}

	public String makeListStr(String auser, String agroup, String ahost) {
		StringBuffer strbuf = new StringBuffer();
		strbuf.append(auser);
		strbuf.append(" (");
		if (agroup != null && !agroup.equals("")) {
			strbuf.append(agroup);
			strbuf.append("/");
		}
		strbuf.append(ahost);
		strbuf.append(")");
		return new String(strbuf);
	}
	
	public synchronized void writeLog(String str1, String str2, String body) {
		String cr = System.getProperty("line.separator", "\n");
		try {
			String logname = pref.getProperty("logFilename");
			FileWriter fw = new FileWriter(logname, true);
			String tmpstr = "=====================================" + cr;
			fw.write(tmpstr, 0, tmpstr.length());
			tmpstr = " " + str1 + cr;
			fw.write(tmpstr, 0, tmpstr.length());
			tmpstr = "	" + str2 + cr;
			fw.write(tmpstr, 0, tmpstr.length());
			tmpstr = "-------------------------------------" + cr;
			fw.write(tmpstr, 0, tmpstr.length());
			tmpstr = StringReplacer.replaceString(body, "\n", cr) + cr + cr;
			fw.write(tmpstr, 0, tmpstr.length());
			fw.close();
		} catch (MissingResourceException ex) {
		} catch (IOException ex) {}
	}
}
