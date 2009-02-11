package com.google.code.yanf4j.test.unittest.utils;

import java.nio.ByteBuffer;

import com.google.code.yanf4j.config.Configuration;
import com.google.code.yanf4j.util.ByteBufferUtils;

import junit.framework.TestCase;

public class ByteBufferUtilsTest extends TestCase {

	public void testIncreaseBlankBufferCapatity() {
		ByteBuffer buffer = ByteBuffer.allocate(1024);
		buffer = ByteBufferUtils.increaseBufferCapatity(buffer);

		assertEquals(1024 + Configuration.DEFAULT_INCREASE_BUFF_SIZE, buffer
				.capacity());
		buffer = ByteBufferUtils.increaseBufferCapatity(buffer);
		assertEquals(1024 + 2 * Configuration.DEFAULT_INCREASE_BUFF_SIZE,
				buffer.capacity());

	}

	public void testIncreaseNotBlankBufferCapatity() {
		ByteBuffer buffer = ByteBuffer.allocate(1024);
		buffer.putInt(100);
		buffer = ByteBufferUtils.increaseBufferCapatity(buffer);
		assertEquals(1024 + Configuration.DEFAULT_INCREASE_BUFF_SIZE, buffer
				.capacity());
		assertEquals(4, buffer.position());
		assertEquals(1024 + Configuration.DEFAULT_INCREASE_BUFF_SIZE - 4,
				buffer.remaining());
		buffer.putLong(100l);
		assertEquals(12, buffer.position());
		buffer = ByteBufferUtils.increaseBufferCapatity(buffer);
		assertEquals(12, buffer.position());
		assertEquals(1024 + 2 * Configuration.DEFAULT_INCREASE_BUFF_SIZE - 4
				- 8, buffer.remaining());

	}

	public void testIncreaseNullBufferCapacity() {
		try {
			assertNull(ByteBufferUtils.increaseBufferCapatity(null));
		} catch (IllegalArgumentException e) {
			assertEquals("buffer is null", e.getMessage());
		}
	}

	public void testFlip() {
		ByteBuffer[] buffers = new ByteBuffer[2];
		ByteBufferUtils.flip(buffers);
		buffers[0] = ByteBuffer.allocate(4).putInt(4);
		buffers[1] = ByteBuffer.allocate(10).put("hello".getBytes());

		assertEquals(4, buffers[0].position());
		assertEquals(5, buffers[1].position());
		assertEquals(4, buffers[0].limit());
		assertEquals(10, buffers[1].limit());
		ByteBufferUtils.flip(buffers);
		assertEquals(0, buffers[0].position());
		assertEquals(0, buffers[1].position());
		assertEquals(4, buffers[0].limit());
		assertEquals(5, buffers[1].limit());

		ByteBufferUtils.flip(null);
	}

	public void testClear() {
		ByteBuffer[] buffers = new ByteBuffer[2];
		ByteBufferUtils.clear(buffers);
		buffers[0] = ByteBuffer.allocate(4).putInt(4);
		buffers[1] = ByteBuffer.allocate(10).put("hello".getBytes());

		assertEquals(4, buffers[0].position());
		assertEquals(5, buffers[1].position());
		assertEquals(4, buffers[0].limit());
		assertEquals(10, buffers[1].limit());
		assertEquals(0, buffers[0].remaining());
		assertEquals(5, buffers[1].remaining());
		ByteBufferUtils.clear(buffers);
		assertEquals(0, buffers[0].position());
		assertEquals(0, buffers[1].position());
		assertEquals(4, buffers[0].limit());
		assertEquals(10, buffers[1].limit());
		assertEquals(4, buffers[0].remaining());
		assertEquals(10, buffers[1].remaining());
		ByteBufferUtils.clear(null);
	}

	public void testHasRemaining() {
		ByteBuffer[] buffers = new ByteBuffer[2];
		// 数组元素为null
		assertFalse(ByteBufferUtils.hasRemaining(buffers));
		buffers[0] = ByteBuffer.allocate(4).putInt(4);
		buffers[1] = ByteBuffer.allocate(10).put("hello".getBytes());
		assertTrue(ByteBufferUtils.hasRemaining(buffers));

		buffers[1].put("yanfj".getBytes());
		assertFalse(ByteBufferUtils.hasRemaining(buffers));

		ByteBufferUtils.clear(buffers);
		assertTrue(ByteBufferUtils.hasRemaining(buffers));
		// 测试3个
		ByteBuffer[] moreBuffers = new ByteBuffer[3];
		moreBuffers[0] = ByteBuffer.allocate(4).putInt(4);
		moreBuffers[1] = ByteBuffer.allocate(10).put("hello".getBytes());
		moreBuffers[2] = ByteBuffer.allocate(12).putLong(9999);
		assertTrue(ByteBufferUtils.hasRemaining(moreBuffers));
		moreBuffers[2].putInt(4);
		assertTrue(ByteBufferUtils.hasRemaining(moreBuffers));
		moreBuffers[1].put("yanfj".getBytes());
		assertFalse(ByteBufferUtils.hasRemaining(moreBuffers));

		// 测试null
		assertFalse(ByteBufferUtils.hasRemaining(null));
	}
}
