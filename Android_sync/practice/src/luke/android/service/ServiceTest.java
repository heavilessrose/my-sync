package luke.android.service;

import android.app.Service;
import android.content.Intent;
import android.os.IBinder;

public class ServiceTest extends Service {

	@Override
	public IBinder onBind(Intent intent) {
		return null;
	}

}
