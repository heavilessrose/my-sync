package luke.android.service.mplayer;

import luke.android.R;
import android.app.Service;
import android.content.Intent;
import android.media.MediaPlayer;
import android.os.IBinder;

public class Music extends Service {

	private MediaPlayer player;

	@Override
	public IBinder onBind(Intent arg0) {
		// TODO Auto-generated method stub
		return null;
	}

	public void onStart(Intent intent, int startId) {
		super.onStart(intent, startId);
		player = MediaPlayer.create(this, R.raw.gongfu);
		player.start();
	}

	public void onDestroy() {
		super.onDestroy();
		player.stop();
	}
}