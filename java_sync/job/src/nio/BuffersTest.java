package nio;

import java.nio.ByteBuffer;
import java.nio.CharBuffer;
import java.nio.charset.CharacterCodingException;
import java.nio.charset.Charset;
import java.nio.charset.CharsetDecoder;
import java.nio.charset.CharsetEncoder;

public class BuffersTest {

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		// ²úÉúÖ»¶Á»º³åÇø
		// CharBuffer chars = CharBuffer.wrap("char buffer:");
		CharBuffer chars = CharBuffer.allocate(32);
		chars.put("char buffer:");
		chars.mark();
		chars.put("add");
		for (int i = 0; chars.length() > 0; i++) {
			System.out.println(chars.get());
		}

		// chars.rewind();
		// chars.flip();
		chars.reset();
		for (int i = 0; chars.length() > 0; i++) {
			System.out.print(chars.get());
		}

		// ===============================×Ö·û±àÂë½âÂë
		Charset charset = Charset.forName("GBK");
		CharsetEncoder encoder = charset.newEncoder();
		CharsetDecoder decoder = charset.newDecoder();
		try {
			ByteBuffer bytes = encoder
					.encode(CharBuffer.wrap("string encoded±àÂëµÄ×Ö·û´®"));
			CharBuffer charBuffer = decoder.decode(bytes);
			System.out.println(charBuffer.toString());
		} catch (CharacterCodingException e) {
			e.printStackTrace();
		}
	}

}
