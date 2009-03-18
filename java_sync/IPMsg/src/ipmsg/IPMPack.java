/*
 *	IP Messenger Packet Class
 *		1997/10/14 (C) Copyright T.Kazawa (Digitune)
 */

package ipmsg;

import java.io.UnsupportedEncodingException;
import java.util.StringTokenizer;
import java.util.NoSuchElementException;

import JP.digitune.util.ByteBuffer;
import JP.digitune.util.StringReplacer;

public class IPMPack {
	private byte[] pack;
	private long version = 0, no = 0, command = 0;
	private String user = null, host = null, extra = null, group = null;

	public IPMPack(long argver, long argno, String arguser
		, String arghost, long argcommand, String argextra, String arggroup) {
		version = argver;
		no = argno;
		user = arguser;
		host = arghost;
		command = argcommand;
		extra = argextra;
		group = arggroup;
	}

	private void packed() {
		StringBuffer strbuf = new StringBuffer();
		strbuf.append(Long.toString(version)).append(":");
		strbuf.append(Long.toString(no)).append(":");
		strbuf.append(user).append(":");
		strbuf.append(host).append(":");
		strbuf.append(Long.toString(command)).append(":");
		strbuf.append(extra);
		String tmpstr = new String(strbuf);
		String ls = System.getProperty("line.separator", "\n");
		String cr = "\n";
		tmpstr = StringReplacer.replaceString(tmpstr, ls, cr);
		ByteBuffer bb = new ByteBuffer();
		try {
			bb.append(tmpstr.getBytes("SJIS"));
		} catch (UnsupportedEncodingException ex) {
			ex.printStackTrace();
			return;
		}
		byte[] nullbyte = { 0 };
		bb.append(nullbyte);
		if (group != null && !group.equals("")) {
			try {
				bb.append(group.getBytes("SJIS"));
				bb.append(nullbyte);
			} catch (UnsupportedEncodingException ex) {}
		}
		pack = bb.getBytes();
	}

	public IPMPack(byte[] buf) {
		pack = buf;
		unpacked();
	}

	private void unpacked() {
		byte[] buf = pack;
		int j = buf.length - 1;
		if (buf[j] == 0) {
			while (buf[j] == 0)
				j--;
			byte[] tmpbuf = new byte[j + 1];
			System.arraycopy(buf, 0, tmpbuf, 0, tmpbuf.length);
			buf = tmpbuf;
		}
		int i = 0;
		for (i = 0; i < buf.length; i++)
			if (buf[i] == 0)
				break;
		if (i < buf.length) {
			j = buf.length - 1;
			byte[] tmpbuf = new byte[j - i];
			System.arraycopy(buf, i + 1, tmpbuf, 0, j - i);
			try {
				group = new String(tmpbuf, "SJIS");
			} catch (UnsupportedEncodingException ex) {
				ex.printStackTrace();
			}
			tmpbuf = new byte[i];
			System.arraycopy(buf, 0, tmpbuf, 0, i);
			buf = tmpbuf;
		}
		String tmpstr;
		try {
			tmpstr = new String(buf, 0, buf.length, "SJIS");
		} catch (UnsupportedEncodingException ex) {
			ex.printStackTrace();
			return;
		}
		String ls = System.getProperty("line.separator", "\n");
		String cr = "\n";
		tmpstr = StringReplacer.replaceString(tmpstr, ls, cr).trim();
		StringTokenizer tokenizer = new StringTokenizer(tmpstr, ":", false);
		try {
			version = Long.parseLong(tokenizer.nextToken());
			no = Long.parseLong(tokenizer.nextToken());
			user = tokenizer.nextToken();
			host = tokenizer.nextToken();
			command = Long.parseLong(tokenizer.nextToken());
		} catch (NoSuchElementException ex) {
			ex.printStackTrace();
			return;
		} catch (NumberFormatException ex) {
			ex.printStackTrace();
			return;
		}
		if (tokenizer.hasMoreTokens())
			extra = tokenizer.nextToken();
		while (tokenizer.hasMoreTokens())
			extra = extra + ':' + tokenizer.nextToken();
	}

	public boolean compare(IPMPack argpack) {
		if (user.equals(argpack.getUser())
			&& host.equals(argpack.getHost())
			&& no == argpack.getNo()
			&& command == argpack.getCommand())
			return true;
		return false;
	}

	public String getKey() {
		return user+":"+host+":"+no+":"+command;
	}

	public byte[] getBytes() {
		packed();
		return pack;
	}

	public void setBytes(byte[] argpack) {
		pack = argpack;
		unpacked();
	}

	public void setVersion(long argver) {
		version = argver;
	}

	public long getVersion() {
		return version;
	}

	public long getNo() {
		return no;
	}

	public void setNo(long argno) {
		no = argno;
	}

	public String getUser() {
		return user;
	}

	public void setUser(String arguser) {
		user = arguser;
	}

	public String getHost() {
		return host;
	}

	public void setHost(String arghost) {
		host = arghost;
	}

	public void setCommand(long argcommand) {
		command = argcommand;
	}

	public long getCommand() {
		return command;
	}

	public String getExtra() {
		return extra;
	}

	public String getGroup() {
		return group;
	}

	public void setGroup(String arggroup) {
		group = arggroup;
	}

	public void setExtra(String argextra) {
		extra = argextra;
	}
}
