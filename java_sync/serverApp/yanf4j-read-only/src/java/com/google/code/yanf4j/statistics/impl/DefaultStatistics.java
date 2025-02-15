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
package com.google.code.yanf4j.statistics.impl;

import com.google.code.yanf4j.statistics.Statistics;

/**
 * 类说明:默认统计类，不做任何统计
 * 
 * @author dennis zhuang
 */
public class DefaultStatistics implements Statistics {
	public void start() {

	}

	public long getStartedTime() {
		return 0;
	}

	public void reset() {

	}

	public void restart() {

	}

	public double getProcessedMessageAverageTime() {
		return 0;
	}

	public long getProcessedMessageCount() {
		return 0;
	}

	public void statisticsProcess(long n) {

	}

	public void stop() {

	}

	public long getRecvMessageCount() {

		return 0;
	}

	public long getRecvMessageTotalSize() {

		return 0;
	}

	public long getRecvMessageAverageSize() {

		return 0;
	}

	public double getRecvMessageCountPerSecond() {

		return 0;
	}

	public long getWriteMessageCount() {

		return 0;
	}

	public long getWriteMessageTotalSize() {

		return 0;
	}

	public long getWriteMessageAverageSize() {

		return 0;
	}

	public void statisticsRead(long n) {

	}

	public void statisticsWrite(long n) {

	}

	public double getWriteMessageCountPerSecond() {

		return 0;
	}

	public double getAcceptCountPerSecond() {
		return 0;
	}

	public void statisticsAccept() {

	}

}
