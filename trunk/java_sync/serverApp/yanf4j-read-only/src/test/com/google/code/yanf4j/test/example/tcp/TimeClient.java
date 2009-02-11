package com.google.code.yanf4j.test.example.tcp;

import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.net.Socket;

public class TimeClient {
	public static void main(String[] args) throws Exception {
		for (int i = 0; i < 10; i++) {
			Socket socket = new Socket("localhost", 8090);
			BufferedReader reader = new BufferedReader(new InputStreamReader(
					socket.getInputStream()));
			System.out.println(reader.readLine());
			Thread.sleep(1000);
			socket.close();

		}
	}
}
