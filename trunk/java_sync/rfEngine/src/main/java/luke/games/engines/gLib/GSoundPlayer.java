package luke.games.engines.gLib;

import javax.microedition.media.*;
import javax.microedition.media.control.*;
import java.io.*;

public class GSoundPlayer implements PlayerListener {
	// final static public short A0 =220;
	// final static public short B0b =233;
	// final static public short B0 =247;
	// final static public short C0 =262;
	// final static public short D0b =277;
	// final static public short D0 =294;
	// final static public short E0b =311;
	// final static public short E0 =330;
	// final static public short F0 =349;
	// final static public short G0b =370;
	// final static public short G0 =392;
	// final static public short A1b =416;
	// final static public short A1 =440;
	// final static public short B1b =466;
	// final static public short B1 =494;
	// final static public short C1 =523;
	// final static public short D1b =554;
	// final static public short D1 =587;
	// final static public short E1b =622;
	// final static public short E1 =659;
	// final static public short F1 =698;
	// final static public short G1b =740;
	// final static public short G1 =784;
	// final static public short A2b =831;
	// final static public short A2 =880;
	// final static public short B2b =932;
	// final static public short B2 =988;
	// final static public short C2 =1047;
	// final static public short D2b =1109;
	// final static public short D2 =1175;
	// final static public short E2b =1245;
	// final static public short E2 =1319;
	// final static public short F2 =1397;
	// final static public short G2b =1480;
	// final static public short G2 =1568;
	// final static public short A3b =1661;
	// final static public short A3 =1760;
	// final static public short B3b =1865;
	// final static public short B3 =1976;
	// final static public short C3 =2093;
	// final static public short D3b =2217;
	// final static public short D3 =2349;
	// final static public short E3b =2489;
	// final static public short E3 =2637;
	// final static public short F3 =2794;
	// final static public short G3b =2960;
	// final static public short G3 =3136;
	// final static public short A4b =3322;
	// final static public short A4 =3520;
	// final static public short B4b =3729;
	// final static public short B4 =3951;
	// final static public short C4 =4186;
	// final static public short D4b =4434;
	// final static public short D4 =4698;
	// final static public short E4b =4978;
	// final static public short E4 =5274;
	// final static public short F4 =5588;
	// final static public short G4b =5920;
	// final static public short G4 =6272;
	// final static public short A5b =6644;
	// final static public short A5 =7040;
	// final static public short B5b =7458;
	// final static public short B5 =7902;
	// final static public short C5 =8372;
	// final static public short D5b =8870;
	// final static public short D5 =9396;
	// final static public short E5b =9956;
	// final static public short E5 =10548;
	// final static public short F5 =11176;
	// final static public short G5b =11840;
	// final static public short G5 =12544;
	// final static public short A6b =13288;
	/** Note data C4 */
	final static public byte C4 = ToneControl.C4; // C4
	final static public byte D4 = (byte) (C4 + 2); // D4
	final static public byte E4 = (byte) (C4 + 4); // E4
	final static public byte F4 = (byte) (C4 + 5); // F4
	final static public byte G4 = (byte) (C4 + 7); // G4
	final static public byte A4 = (byte) (C4 + 9); // A4
	final static public byte B4 = (byte) (C4 + 11);// B4
	final static public byte C4_H = (byte) (C4 + 1); // C#4
	final static public byte D4_H = (byte) (C4 + 3); // D#4
	final static public byte F4_H = (byte) (C4 + 6); // F#4
	final static public byte G4_H = (byte) (C4 + 8); // G#4
	final static public byte A4_H = (byte) (C4 + 10);// A#4
	/** 一个8度的7个白键 */
	final static public byte[] OCTACHORD = { C4, D4, E4, F4, G4, A4, B4 };
	/** 一个8度的所有音符 */
	final static public byte[] OCTACHORD_FULL = { C4, C4_H, D4, D4_H, E4, F4,
			F4_H, G4, G4_H, A4, A4_H, B4 };

	// /**
	// * Sample Music
	// */
	// // "Mary Had A Little Lamb" has "ABAC" structure.
	// // Use block to repeat "A" section.
	// final static public byte tempo = 30; // set tempo to 120 bpm
	// final static public byte d = 8; // eighth-note
	// final static public byte rest = ToneControl.SILENCE; // rest
	// final static public byte[] SampleMusic = {
	// ToneControl.VERSION, 1, // version 1
	// ToneControl.TEMPO, tempo, // set tempo
	// ToneControl.BLOCK_START, 0, // start define "A" section
	// E4,d, D4,d, C4,d, E4,d, // content of "A" section
	// E4,d, E4,d, E4,d, rest,d,
	// ToneControl.BLOCK_END, 0, // end define "A" section
	// ToneControl.PLAY_BLOCK, 0, // play "A" section
	// D4,d, D4,d, D4,d, rest,d, // play "B" section
	// E4,d, G4,d, G4,d, rest,d,
	// ToneControl.PLAY_BLOCK, 0, // repeat "A" section
	// D4,d, D4,d, E4,d, D4,d, C4,d // play "C" section
	// };

	// --------------------------------------------------------------------------------------------
	private Player player;
	private VolumeControl vc;
	private ToneControl tc;
	private int Volume;

	/**
	 * 默认空构造类
	 */
	public GSoundPlayer() {
	}

	/**
	 * 从文件构造播放器
	 * 
	 * @param filename
	 *            Resource file
	 * @param type
	 *            Wave audio files: audio/x-wav AU audio files: audio/basic MP3
	 *            audio files: audio/mpeg MIDI files: audio/midi Tone sequences:
	 *            audio/x-tone-seq
	 * @param loop
	 *            Loop Count
	 */
	public GSoundPlayer(String filename, String type, int loop) {
		try {
			InputStream is = getClass().getResourceAsStream(filename);
			if (is == null) {
				GUtil.print("Error Load Sound File '" + filename + "' -_-!");
			} else {
				player = Manager.createPlayer(is, type);
				player.addPlayerListener(this);
				player.stop();
				player.realize();
				player.prefetch();
				player.setLoopCount(loop);
				vc = (VolumeControl) player.getControl("VolumeControl");
			}
		} catch (IOException e) {
			e.printStackTrace();
		} catch (MediaException e) {
			e.printStackTrace();
		}
	}

	/**
	 * 从TONE序列构造播放器
	 * 
	 * @param tone
	 *            tone = { ToneControl.VERSION, 1, // version 1
	 *            ToneControl.TEMPO, 30, // set tempo C4,8, //data start
	 *            {level,dot} C5,8 ... };
	 * @param loop
	 *            Loop Count
	 */
	public GSoundPlayer(byte[] tone, int loop) {
		try {
			player = Manager.createPlayer(Manager.TONE_DEVICE_LOCATOR);
			player.addPlayerListener(this);
			player.stop();
			player.realize();
			// player.prefetch();
			player.setLoopCount(loop);
			vc = (VolumeControl) player.getControl("VolumeControl");
			tc = (ToneControl) player.getControl("ToneControl");
			tc.setSequence(tone);
		} catch (IOException e) {
			e.printStackTrace();
		} catch (MediaException e) {
			e.printStackTrace();
		}
	}

	// ------------------------------------------------------------------------------------------------------------------------------
	/**
	 * 直接播放一段TONE声音
	 * 
	 * @param note
	 *            音高
	 * @param duration
	 *            音长
	 * @param volume
	 *            音量
	 */
	public void playTone(byte note, int duration, int volume) {
		try {
			Manager.playTone(note, duration, volume);
		} catch (MediaException e) {
			e.printStackTrace();
		}
	}

	/**
	 * 从头播放乐曲
	 * 
	 */
	final public void play() {
		try {
			if (player != null && player.getState() != Player.CLOSED
					&& tc != null && player.getState() == Player.REALIZED) {
				player.setMediaTime(-1);
				player.start();
			} else if (player != null && player.getState() == Player.PREFETCHED) {
				player.setMediaTime(-1);
				player.start();
			}
		} catch (MediaException e) {
			e.printStackTrace();
		}
	}

	/**
	 * 暂停播放乐曲
	 * 
	 */
	final public void pause() {
		try {
			if (player != null && player.getState() == Player.STARTED) {
				player.stop();
			}
		} catch (MediaException e) {

			e.printStackTrace();
		}
	}

	/**
	 * 恢复播放乐曲
	 * 
	 */
	final public void resume() {
		try {
			if (player != null && player.getState() != Player.CLOSED
					&& player.getState() != Player.STARTED) {
				player.start();
			}
		} catch (MediaException e) {

			e.printStackTrace();
		}
	}

	/**
	 * 释放资源<br>
	 * 使用完播放器后必须得做的事情,否则会内存溢出
	 * 
	 */
	final public void destroy() {
		try {
			if (player != null) {
				player.close();
			}
			if (tc != null)
				tc = null;
			if (vc != null)
				vc = null;
			System.gc();
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	/**
	 * 设置音量
	 * 
	 * @param level
	 *            音量0~100
	 */
	final public void setVolume(int level) {
		Volume = level;
		if (vc != null) {
			vc.setLevel(Volume);
		}
	}

	/**
	 * 得到音量
	 * 
	 * @return 音量
	 */
	final public int getVolume() {
		return (Volume);
	}

	/**
	 * 检查是否正在播放
	 * 
	 * @return 是否正在播放
	 */
	final public boolean isPlaying() {
		return (player != null && player.getState() == Player.STARTED);
	}

	/**
	 * 更新播放事件
	 */
	final public void playerUpdate(Player player, String event, Object eventData) {
		if (event == VOLUME_CHANGED) {
			GUtil.print("Volume Changed : " + vc.getLevel());
		}
	}
}