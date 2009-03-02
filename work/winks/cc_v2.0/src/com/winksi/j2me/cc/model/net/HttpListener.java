package com.winksi.j2me.cc.model.net;

public interface HttpListener {

	//void onSetSize(int size);

	void onFinish(byte[] result);

	void onProgress(int percent);

	void onError(int code);

	void setGprs(boolean b);
}
