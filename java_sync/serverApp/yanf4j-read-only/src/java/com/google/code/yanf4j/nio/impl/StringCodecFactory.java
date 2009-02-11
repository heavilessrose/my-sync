package com.google.code.yanf4j.nio.impl;

/**
 *Copyright [2008] [dennis zhuang]
 *Licensed under the Apache License, Version 2.0 (the "License");
 *you may not use this file except in compliance with the License. 
 *You may obtain a copy of the License at 
 *             http://www.apache.org/licenses/LICENSE-2.0 
 *Unless required by applicable law or agreed to in writing, 
 *software distributed under the License is distributed on an "AS IS" BASIS, 
 *WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, 
 *either express or implied. See the License for the specific language governing permissions and limitations under the License
 */
import java.nio.ByteBuffer;

import com.google.code.yanf4j.nio.CodecFactory;
import com.google.code.yanf4j.util.ByteBufferUtils;

public class StringCodecFactory implements CodecFactory<String> {
	private static final ByteBuffer SPLIT = ByteBuffer.wrap("\r\n".getBytes());

	static class StringDecoder implements Decoder<String> {
		public String decode(ByteBuffer buffer) {
			String result = null;
			int index = ByteBufferUtils.indexOf(buffer, SPLIT);
			if (index >= 0) {
				int limit = buffer.limit();
				buffer.limit(index);
				byte[] bytes = new byte[buffer.remaining()];
				buffer.get(bytes);
				result = new String(bytes);
				buffer.limit(limit);
				buffer.position(index + SPLIT.remaining());

			}
			return result;
		}
	}

	private static Decoder<String> decoder = new StringDecoder();

	public Decoder<String> getDecoder() {
		return decoder;

	}

	static class StringEncoder implements Encoder<String> {
		public ByteBuffer[] encode(String message) {
			ByteBuffer buff = ByteBuffer.allocate(((String) message).length())
					.put(((String) message).getBytes());
			buff.flip();
			return new ByteBuffer[] { buff, SPLIT.slice() };
		}

	}

	private static Encoder<String> encoder = new StringEncoder();

	public Encoder<String> getEncoder() {
		return encoder;
	}

}
