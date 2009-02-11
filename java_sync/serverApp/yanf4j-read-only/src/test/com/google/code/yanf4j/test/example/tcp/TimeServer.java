package com.google.code.yanf4j.test.example.tcp;

import java.io.IOException;
import com.google.code.yanf4j.nio.Controller;
import com.google.code.yanf4j.nio.TCPController;
import com.google.code.yanf4j.nio.impl.StringCodecFactory;
import java.net.InetSocketAddress;


public class TimeServer {
	public static void main(String[] args) {
		Controller controller = new TCPController();
		try {
			controller.open(new InetSocketAddress(8090), new TimeHandler(),
					new StringCodecFactory());
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
}
