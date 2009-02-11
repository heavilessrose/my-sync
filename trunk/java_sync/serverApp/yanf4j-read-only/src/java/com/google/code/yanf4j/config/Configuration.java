package com.google.code.yanf4j.config;

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
import java.io.IOException;
import java.util.Properties;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.google.code.yanf4j.util.PropertyUtils;
import com.google.code.yanf4j.util.ResourcesUtils;

/**
 * 配置参数类，默认从classpath下的yanf4j.properties读取参数
 * 
 * @author dennis
 * 
 */
public class Configuration {
	private static final Log log = LogFactory.getLog(Configuration.class);

	public Configuration(String fileName) {
		if (fileName != null) {
			Properties props = null;
			try {
				props = ResourcesUtils.getResourceAsProperties(fileName);
			} catch (IOException e) {
				log.error("The resource " + fileName
						+ " is not exist,use default configuration", e);
			}
			if (props != null) {
				tcpSessionReadBufferSize = PropertyUtils.getPropertyAsInteger(
						props, "yanf4j.socket.read.buffer.size");
				statisticsServer = PropertyUtils.getPropertyAsBoolean(props,
						"yanf4j.statistics");
				readThreadCount = PropertyUtils.getPropertyAsInteger(props,
						"yanf4j.socket.readthread.count");
				reuseAddress = PropertyUtils.getPropertyAsBoolean(props,
						"yanf4j.socket.reuseaddr");
				tcpRecvBufferSize = PropertyUtils.getPropertyAsInteger(props,
						"yanf4j.socket.recv.buffer.size");
				tcpNoDelay = PropertyUtils.getPropertyAsBoolean(props,
						"yanf4j.socket.nodelay");
				timeout = PropertyUtils.getPropertyAsInteger(props,
						"yanf4j.socket.sotimeout");
				CHECK_SESSION_TIMEOUT_INTERVAL = PropertyUtils
						.getPropertyAsLong(props,
								"yanf4j.socket.checktimeout.interval");
				CHECK_SESSION_IDLE_INTERVAL = PropertyUtils
						.getPropertyAsInteger(props,
								"yanf4j.socket.checksessionidle.interval");
			}
		}

	}

	public Configuration() {
		this("yanf4j.properties");
	}

	private int tcpSessionReadBufferSize = 32 * 1024;

	private int timeout = 0;

	private int port = 0;

	private boolean statisticsServer = false;

	private boolean handleReadWriteConcurrently = true;

	private int readThreadCount = 1;

	public static final int DEFAULT_INCREASE_BUFF_SIZE = 32 * 1024;

	public static long CHECK_SESSION_TIMEOUT_INTERVAL = 2000;

	private boolean reuseAddress = false;

	private int tcpRecvBufferSize = 16 * 1024;

	private boolean tcpNoDelay = false; // true为禁止Nagle算法
	
	// 超过这个时间没有任何读取或者写操作，即调用handler的onIdle()方法
	public static int CHECK_SESSION_IDLE_INTERVAL = 2000;

	public int getTcpSessionReadBufferSize() {
		return tcpSessionReadBufferSize;
	}

	public boolean isHandleReadWriteConcurrently() {
		return handleReadWriteConcurrently;
	}

	public int getSoTimeout() {
		return timeout;
	}

	// 统计刷新间隔
	protected int statisticsInterval = 5 * 60;

	public int getStatisticsInterval() {
		return statisticsInterval;
	}

	public void setStatisticsInterval(int statisticsInterval) {
		this.statisticsInterval = statisticsInterval;
	}

	public void setTSoimeout(int timeout) {
		this.timeout = timeout;
	}

	public void setHandleReadWriteConcurrently(
			boolean handleReadWriteConcurrently) {
		this.handleReadWriteConcurrently = handleReadWriteConcurrently;
	}

	public void setTcpSessionReadBufferSize(int tcpHandlerReadBufferSize) {
		this.tcpSessionReadBufferSize = tcpHandlerReadBufferSize;
	}

	public int getPort() {
		return port;
	}

	public void setPort(int tcpPort) {
		this.port = tcpPort;
	}

	public boolean isStatisticsServer() {
		return statisticsServer;
	}

	public void setStatisticsServer(boolean statisticsServer) {
		this.statisticsServer = statisticsServer;
	}

	public int getReadThreadCount() {
		return readThreadCount;
	}

	public void setReadThreadCount(int tcpReadThreadCount) {
		this.readThreadCount = tcpReadThreadCount;
	}

	public boolean isReuseAddress() {
		return reuseAddress;
	}

	public void setReuseAddress(boolean tcpReuseAddress) {
		this.reuseAddress = tcpReuseAddress;
	}

	public int getTcpRecvBufferSize() {
		return tcpRecvBufferSize;
	}

	public void setTcpRecvBufferSize(int tcpRecvBufferSize) {
		this.tcpRecvBufferSize = tcpRecvBufferSize;
	}

	public boolean isTcpNoDelay() {
		return tcpNoDelay;
	}

	public void setTcpNoDelay(boolean tcp_no_delay) {
		tcpNoDelay = tcp_no_delay;
	}

}
