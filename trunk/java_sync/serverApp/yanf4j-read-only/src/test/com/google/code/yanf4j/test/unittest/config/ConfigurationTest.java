package com.google.code.yanf4j.test.unittest.config;

import com.google.code.yanf4j.config.Configuration;

import junit.framework.TestCase;

public class ConfigurationTest extends TestCase {
	Configuration configuration;

	@Override
	protected void setUp() throws Exception {
		configuration = new Configuration();
	}

	public void testConfig() {
		assertTrue(configuration.isHandleReadWriteConcurrently());
		assertTrue(configuration.isReuseAddress());
		assertTrue(configuration.isTcpNoDelay());
		assertEquals(16 * 1024, configuration.getTcpRecvBufferSize());
		assertEquals(32 * 1024, configuration.getTcpSessionReadBufferSize());
		assertEquals(0,configuration.getSoTimeout());
	}

	public void testNotExist() {
		try {
			Configuration config = new Configuration("notexists.properties");
			assertTrue(config.isReuseAddress());
			assertFalse(config.isTcpNoDelay());
			assertEquals(16 * 1024, config.getTcpRecvBufferSize());
			assertEquals(32 * 1024, config.getTcpSessionReadBufferSize());
			assertEquals(0,configuration.getSoTimeout());
		} catch (Exception e) {
			assertEquals("Could not find resource notexists.properties", e
					.getMessage());
		}
	}

	public void testNull() {
		Configuration config = new Configuration(null);
		assertTrue(config.isReuseAddress());
		assertFalse(config.isTcpNoDelay());
		assertEquals(16 * 1024, config.getTcpRecvBufferSize());
		assertEquals(32 * 1024, config.getTcpSessionReadBufferSize());
		assertEquals(0,configuration.getSoTimeout());
	}

}
