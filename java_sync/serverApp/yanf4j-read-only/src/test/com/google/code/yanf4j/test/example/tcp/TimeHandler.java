package com.google.code.yanf4j.test.example.tcp;

import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.Date;

import com.google.code.yanf4j.nio.Session;
import com.google.code.yanf4j.nio.impl.HandlerAdapter;

public class TimeHandler extends HandlerAdapter<String> {

	@Override
	public void onSessionStarted(Session session) {
		Date date = new Date();
		DateFormat dateFormat = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
		try {
			session.send(dateFormat.format(date));
			session.flush(); // 刷新缓冲队列
			session.close(); // 关闭连接
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
}
