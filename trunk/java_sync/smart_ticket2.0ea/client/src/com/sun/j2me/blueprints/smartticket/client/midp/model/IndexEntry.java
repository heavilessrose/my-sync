package com.sun.j2me.blueprints.smartticket.client.midp.model;

import java.io.*;

/** 封装一条RMS记录 */
public class IndexEntry {

	public static final int TYPE_ANY = -1;
	public static final int TYPE_THEATER = 0;
	public static final int TYPE_THEATER_SCHEDULE = 1;
	public static final int TYPE_MOVIE = 2;
	public static final int TYPE_MOVIE_RATING = 3;
	public static final int TYPE_ACCOUNT_INFO = 4;
	public static final int TYPE_PREFERENCES = 5;
	public static final int TYPE_SYNC_ANCHOR = 6;
	public static final int TYPE_RESOURCE_BUNDLE = 7;
	public static final int MODE_ANY = -1;
	public static final int MODE_PERSISTING = 0;
	public static final int MODE_CACHING = 1;
	private int type;
	private String key;
	private long expirationDate;
	private boolean marked;
	private int recordId;
	private int mode = MODE_CACHING;

	public IndexEntry(int recordId, int type, String key) {
		this(recordId, type, key, MODE_CACHING);

		return;
	}

	public IndexEntry(int recordId, int type, String key, int mode) {
		this.recordId = recordId;
		this.type = type;
		this.key = key;
		this.mode = mode;

		return;
	}

	private IndexEntry() {
	}

	public int getType() {
		return type;
	}

	public String getKey() {
		return key;
	}

	public long getExpirationDate() {
		return expirationDate;
	}

	public void setExpirationDate(long expirationDate) {
		this.expirationDate = expirationDate;
	}

	public boolean isMarked() {
		return marked;
	}

	public void setMarked(boolean marked) {
		this.marked = marked;
	}

	public int getRecordId() {
		return recordId;
	}

	public int getMode() {
		return mode;
	}

	/** 
	 * 线性化
	 * 
	 * 返回要写入的字节数组
	 * @return
	 * @throws IOException
	 */
	public byte[] serialize() throws IOException {
		ByteArrayOutputStream byteStream = new ByteArrayOutputStream();
		DataOutputStream dataStream = new DataOutputStream(byteStream);

		dataStream.writeInt(type);
		dataStream.writeUTF(key);
		dataStream.writeInt(mode);
		dataStream.writeLong(expirationDate);
		dataStream.writeBoolean(marked);
		dataStream.writeInt(recordId);

		return byteStream.toByteArray();
	}

	/** 
	 * 反线性化
	 * 
	 * 返回一条记录对应的IndexEntry
	 * @param data
	 * @return
	 * @throws IOException
	 */
	public static IndexEntry deserialize(byte[] data) throws IOException {
		ByteArrayInputStream byteStream = new ByteArrayInputStream(data);
		DataInputStream dataStream = new DataInputStream(byteStream);
		IndexEntry indexEntry = new IndexEntry();

		indexEntry.type = dataStream.readInt();
		indexEntry.key = dataStream.readUTF();
		indexEntry.mode = dataStream.readInt();
		indexEntry.expirationDate = dataStream.readLong();
		indexEntry.marked = dataStream.readBoolean();
		indexEntry.recordId = dataStream.readInt();

		return indexEntry;
	}

	// Optimization to avoid deserializing and instantiating an IndexEntry when matching RMS records
	public static boolean matches(byte[] data, String key, int type, int mode) throws IOException {
		ByteArrayInputStream byteStream = new ByteArrayInputStream(data);
		DataInputStream dataStream = new DataInputStream(byteStream);

		return (type == dataStream.readInt() || type == TYPE_ANY)
				&& (dataStream.readUTF().equals(key) || key == null)
				&& (mode == dataStream.readInt() || mode == MODE_ANY);
	}

}
