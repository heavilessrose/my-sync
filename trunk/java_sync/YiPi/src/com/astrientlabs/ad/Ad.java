/*
 * Copyright (C) 2006 Astrient Labs, LLC Licensed under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except in compliance 
 * with the License. You may obtain a copy of the License at 
 * 
 *    http://www.apache.org/licenses/LICENSE-2.0 
 * 
 * Unless required by applicable law or agreed to in writing, software 
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT 
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the 
 * License for the specific language governing permissions and limitations under
 * the License.
 * 
 * Astrient Labs, LLC 
 * www.astrientlabs.com 
 * rashid@astrientlabs.com
 * Rashid Mayes 2006
 */
package com.astrientlabs.ad;

import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;

import javax.microedition.lcdui.Image;

/** 广告管理 */
public class Ad {
	private String name;
	private String infoURL;
	private long displayTime;
	private byte[] data;
	private Image image;

	public long getDisplayTime() {
		return displayTime;
	}

	public void setDisplayTime(long displayTime) {
		this.displayTime = displayTime;
	}

	public Image getImage() {
		if (image == null) {
			if (data != null) {
				image = Image.createImage(data, 0, data.length);
				data = null;
			}
		}

		return image;
	}

	public void setImage(Image image) {
		this.image = image;
	}

	public String getInfoURL() {
		return infoURL;
	}

	public void setInfoURL(String infoURL) {
		this.infoURL = infoURL;
	}

	public String getName() {
		return name;
	}

	public void setName(String name) {
		this.name = name;
	}

	public byte[] toPersistentFormat() throws IOException {
		ByteArrayOutputStream baos = new ByteArrayOutputStream();
		DataOutputStream dos = new DataOutputStream(baos);

		dos.writeUTF(name);
		dos.writeUTF(infoURL);
		dos.writeLong(displayTime);
		dos.write(data);
		dos.flush();

		return baos.toByteArray();
	}

	public void fromPersistentFormat(byte[] bytes) throws IOException {
		ByteArrayInputStream bais = new ByteArrayInputStream(bytes);
		DataInputStream dis = new DataInputStream(bais);

		name = dis.readUTF();
		infoURL = dis.readUTF();
		displayTime = dis.readLong();

		ByteArrayOutputStream temp = new ByteArrayOutputStream();
		byte[] ba = new byte[1 * 1024];

		int read;
		while ((read = dis.read(ba)) != -1) {
			temp.write(ba, 0, read);
		}

		data = temp.toByteArray();
	}
}
