package com.google.code.yanf4j.test.example.chat;

import java.io.IOException;

import com.google.code.yanf4j.config.Configuration;
import com.google.code.yanf4j.nio.TCPController;
import com.google.code.yanf4j.nio.impl.SessionFlowController;
import com.google.code.yanf4j.nio.impl.StringCodecFactory;

public class ChatServer {
	public static void main(String[] args) throws IOException {
		SessionFlowController controller = new TCPController(new Configuration());
		controller
				.open(8080, new ChatServerHandler(), new StringCodecFactory());
	}
}
