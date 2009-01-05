import com.nokia.mid.sound.Sound;
import com.nokia.mid.ui.FullCanvas;
import java.io.*;
import java.util.*;
import javax.microedition.lcdui.*;
import javax.microedition.midlet.MIDlet;
import javax.microedition.rms.RecordStore;
import javax.microedition.rms.RecordStoreNotFoundException;

class mainCanvas extends FullCanvas {

	public mainCanvas(COD cod1) {
		projectileArrayNext = new byte[20];
		projectileX = new int[20];
		projectileY = new int[20];
		projectileDirection = new byte[20];
		projectileTime = new byte[20];
		projectileType = new byte[20];
		playerArrayNext = new byte[45];
		playerX = new int[45];
		playerY = new int[45];
		playerDirection = new byte[45];
		playerMoving = new boolean[45];
		playerHealth = new byte[45];
		playerNextShot = new int[45];
		playerAimX = new int[2];
		playerAimY = new int[2];
		playerType = new byte[3];
		playerSpecial = new int[3];
		playerProtected = new int[3];
		playerInBuilding = new boolean[3];
		playerShowStatus = new int[3];
		specialArrayNext = new byte[40];
		specialArrayPrevious = new byte[40];
		specialX = new int[40];
		specialY = new int[40];
		specialType = new byte[40];
		specialDamage = new byte[40];
		squarePlayer = new byte[30];
		squareRow = new byte[30];
		squareCol = new byte[30];
		allowKeyPresses = true;
		random = new Random();
		missionTime = new int[9];
		MapData = new byte[128][11];
		cod = cod1;
		font = Font.getFont(0, 0, 8);
		fontHeight = (short) font.getHeight();
		largeFont = Font.getFont(64, 1, 0);
		largeFontHeight = (short) largeFont.getHeight();
		width = getWidth();
		height = getHeight();
		halfWidth = width >> 1;
		halfHeight = height >> 1;
		missionNumberCurrent = 0;
		missionNumberMax = 0;
		soundOn = true;
		for (int i = 0; i <= 8; i++)
			missionTime[i] = 0;

		try {
			if (RecordStore.listRecordStores() == null)
				writeRecords();
			else
				readRecords();
		} catch (Exception exception) {
			System.out.println("Record Store not found");
		}
		try {
			unitImage = Image.createImage("/player.png");
			symbolsImage = Image.createImage("/symbols.png");
			specialImage = Image.createImage("/activision.png");
			titleImage = Image.createImage("/title.png");
			arrowsImage = Image.createImage("/arrows.png");
			statusBar = Image.createImage(66, 14);
		} catch (Exception exception1) {
			System.out.println("Images not found");
		}
		int j = (height >> 4) + 1;
		if ((height & 0xf) > 0)
			j++;
		screenImageHeight = j << 4;
		screenImage = Image.createImage(176, screenImageHeight);
		screenImageGraphics = screenImage.getGraphics();
		introSound = new Sound(new byte[] { 2, 74, 58, -128, 64, 6, 81, -41,
				-56, -91, 7, 32, 106, 7, 18, 106, 7, 32, -110, 41, 65, -56, 26,
				-127, -58, 26, 97, 4, 36, -118, 80, 114, 6, -96, 113, 38, -96,
				114, 9, 34, -108, 28, -127, -88, 28, 97, -90, 16, 97, -56, 26,
				-126, 72, -91, 7, 32, 106, 9, 38, -110, 48, 65, -56, 36, -86,
				13, 52, -60, 13, 48, 0 }, 1);
		fireSound = new Sound(new byte[] { 2, 74, 58, -128, 64, 0, -79, -41,
				-53, 80, -83, 10, 80, 0 }, 1);
		pickupSound = new Sound(new byte[] { 2, 74, 58, -128, 64, 0, -111, -41,
				-56, -80, -84, 64, 0 }, 1);
		secondarySound = new Sound(new byte[] { 2, 74, 58, -128, 64, 0, -111,
				-41, -48, 89, -96, 0 }, 1);
		explosionSound = new Sound(new byte[] { 2, 74, 58, -128, 64, 1, 113,
				-41, -56, -63, 4, 104, 74, -124, 104, 74, -124, 104, -110, 48,
				0 }, 1);
		unitSound = new Sound(new byte[] { 2, 74, 58, -128, 64, 0, -47, -41,
				-44, 70, 4, 40, 70, 0, 0 }, 1);
		winSound = new Sound(
				new byte[] { 2, 74, 58, -128, 64, 1, -111, -41, -48, 97, -122,
						32, 98, 7, 32, 98, 7, 32, -110, 112, 48, -48, 0 }, 1);
		loseSound = new Sound(new byte[] { 2, 74, 58, -128, 64, 1, 81, -41,
				-51, 32, -45, 10, -96, -85, 8, -96, -117, 16, 105, 0, 0 }, 1);
		System.gc();
		PlaySound(introSound);
		screenType = 0;
		timer = new Timer();
		frameCount = 0;
		StartSimpleTimer((short) 1, 2500);
	}

	private void GetSpecialImage(String s) {
		specialImage = null;
		System.gc();
		try {
			specialImage = Image.createImage("/" + s + ".png");
		} catch (Exception exception) {
			System.out.println(s + ".png not found");
		}
	}

	private void GetGameImages(boolean flag) {
		titleImage = null;
		arrowsImage = null;
		System.gc();
		try {
			if (flag) {
				titleImage = Image.createImage("/axis.png");
				arrowsImage = Image.createImage("/bang.png");
			} else {
				titleImage = Image.createImage("/title.png");
				arrowsImage = Image.createImage("/arrows.png");
			}
		} catch (Exception exception) {
			System.out.println("GetGameImages failed");
		}
	}

	private void ReadMap() {
		projectileArrayUsedStart = -1;
		projectileArrayUnusedStart = 0;
		for (int i = 18; i >= 0; i--)
			projectileArrayNext[i] = (byte) (i + 1);

		projectileArrayNext[19] = -1;
		screenImageLeft = 0;
		for (int j = 0; j < 3; j++) {
			if (playerType[j] == 4 || playerType[j] == 3)
				playerSpecial[j] = 0;
			else
				playerSpecial[j] = INITIAL_AMMO[WEAPON_TYPE[playerType[j]]];
			playerHealth[j] = INITIAL_HEALTH[playerType[j]];
			playerNextShot[j] = 0;
			playerDirection[j] = 1;
			playerMoving[j] = false;
			playerProtected[j] = 0;
			playerInBuilding[j] = false;
			playerShowStatus[j] = 60;
		}

		Object obj = null;
		Object obj1 = null;
		Vector vector = new Vector();
		Vector vector1 = new Vector();
		Vector vector2 = new Vector();
		Vector vector3 = new Vector();
		Vector vector4 = new Vector();
		Vector vector5 = new Vector();
		try {
			InputStream inputstream = getClass().getResourceAsStream(
					"m" + missionNumberCurrent);
			DataInputStream datainputstream = new DataInputStream(inputstream);
			int k = datainputstream.read();
			int j1;
			int i2;
			do {
				j1 = k >>> 4 & 0xf;
				i2 = k & 0xf;
				if (j1 > 1)
					break;
				int k2 = datainputstream.read();
				vector1.addElement(new Integer(i2));
				vector2.addElement(new Integer(k2));
				k = datainputstream.read();
			} while (true);
			byte byte0 = 3;
			for (int k3 = vector1.size() - 1; k3 >= 0; k3--) {
				playerX[byte0] = ((Integer) vector1.elementAt(k3)).intValue() << 4;
				playerY[byte0] = ((Integer) vector2.elementAt(k3)).intValue() << 4;
				playerNextShot[byte0] = 0;
				int j3;
				if (missionNumberCurrent == 9)
					j3 = 2;
				else
					j3 = missionNumberCurrent / 3;
				if (j3 == 0)
					playerHealth[byte0] = 0;
				else
					playerHealth[byte0] = (byte) (Math.abs(random.nextInt()) % (j3 + 1));
				playerDirection[byte0] = 7;
				byte0++;
			}

			playerArrayNext[byte0 - 1] = -1;
			for (int l3 = byte0 - 2; l3 >= 0; l3--)
				playerArrayNext[l3] = (byte) (l3 + 1);

			if (byte0 > 3)
				opponentArrayStart = 3;
			else
				opponentArrayStart = -1;
			vector1 = new Vector();
			vector2 = new Vector();
			while (k != 240) {
				int l2 = datainputstream.read();
				if (j1 == 7) {
					screenImageLeft = ((i2 << 4) + 8) - width / 2;
					if (screenImageLeft < 0)
						screenImageLeft = 0;
					else if (screenImageLeft + width > 176)
						screenImageLeft = 176 - width;
					playerArrayStart = currentPlayer = 0;
					takeCover = false;
					playerX[0] = i2 << 4;
					playerY[0] = l2 << 4;
					playerX[1] = i2 - 1 << 4;
					playerY[1] = l2 + 1 << 4;
					playerX[2] = i2 + 1 << 4;
					playerY[2] = playerY[1];
				} else {
					vector.addElement(new Integer(j1));
					vector1.addElement(new Integer(i2));
					vector2.addElement(new Integer(l2));
				}
				k = datainputstream.read();
				j1 = k >>> 4 & 0xf;
				i2 = k & 0xf;
			}
			k = datainputstream.read();
			do {
				int k1 = k >>> 2 & 0x3f;
				i2 = k & 3;
				if (k1 == 63)
					break;
				vector3.addElement(new Integer(k1 - 1));
				switch (i2) {
				case 0: // '\0'
					vector4.addElement(new Integer(4));
					break;

				case 1: // '\001'
					vector4.addElement(new Integer(0));
					break;

				case 2: // '\002'
					vector4.addElement(new Integer(2));
					break;

				case 3: // '\003'
					vector4.addElement(new Integer(3));
					break;
				}
				vector5.addElement(new Integer(-1));
				k = datainputstream.read();
			} while (true);
			int i4 = -1;
			int j4 = 0;
			boolean flag = false;
			int k4 = -1;
			i2 = 0;
			do {
				if (i4 < 0) {
					j4 = 0;
					for (int l4 = 2; l4 >= 0; l4--) {
						int l = datainputstream.read();
						j4 += l << l4 * 8;
					}

					i4 = 3;
				}
				if (k4 == -1) {
					k4 = j4 >>> i4 * 6 & 0x3f;
					if (k4 == 63)
						break;
				} else {
					int i1 = j4 >>> i4 * 6 & 0x3f;
					if (flag) {
						if (i1 == 0) {
							MapData[i2 / 11][i2 % 11] = (byte) (k4 - 1);
							i2++;
							break;
						}
						for (int i5 = i1; i5 > 0; i5--) {
							MapData[i2 / 11][i2 % 11] = (byte) (k4 - 1);
							i2++;
						}

						flag = false;
						k4 = -1;
					} else if (i1 == 63) {
						flag = true;
					} else {
						MapData[i2 / 11][i2 % 11] = (byte) (k4 - 1);
						i2++;
						k4 = i1;
					}
				}
				i4--;
			} while (true);
			datainputstream.close();
			inputstream.close();
		} catch (Exception exception) {
			System.out.println("Exception: " + exception.toString());
		}
		byte byte1 = 0;
		byte byte2 = (byte) vector3.size();
		specialArrayExplodableStart = -1;
		byte abyte0[] = new byte[16];
		byte abyte1[] = new byte[16];
		for (int j5 = vector.size() - 1; j5 >= 0; j5--) {
			int l1 = ((Integer) vector.elementAt(j5)).intValue();
			if (specialArrayExplodableStart < 0 && l1 <= 6)
				specialArrayExplodableStart = byte1;
			byte byte3;
			byte byte4;
			switch (l1) {
			case 2: // '\002'
				byte3 = 3;
				byte4 = 1;
				break;

			case 3: // '\003'
				byte3 = 0;
				byte4 = 0;
				break;

			case 4: // '\004'
				byte3 = 1;
				byte4 = 0;
				break;

			case 6: // '\006'
				byte3 = 2;
				byte4 = 0;
				break;

			case 8: // '\b'
				byte3 = 7;
				byte4 = 4;
				break;

			case 9: // '\t'
				byte3 = 5;
				byte4 = 4;
				break;

			case 10: // '\n'
				byte3 = 4;
				byte4 = 4;
				break;

			case 11: // '\013'
				byte3 = 6;
				byte4 = 4;
				break;

			case 12: // '\f'
				byte3 = 8;
				byte4 = 5;
				break;

			case 5: // '\005'
			case 7: // '\007'
			default:
				byte3 = -1;
				byte4 = -1;
				break;
			}
			if (byte3 >= 0) {
				int j2 = ((Integer) vector1.elementAt(j5)).intValue();
				int i3 = ((Integer) vector2.elementAt(j5)).intValue();
				byte byte5 = 0;
				for (int k5 = (SPECIAL_GFX_W[byte3] >> 4) - 1; k5 >= 0; k5--) {
					for (int i6 = (SPECIAL_GFX_H[byte3] >> 4) - 1; i6 >= 0; i6--) {
						byte byte6;
						for (byte6 = 0; byte6 < byte5; byte6++) {
							if (abyte0[byte6] != MapData[i3 + i6][j2 + k5])
								continue;
							MapData[i3 + i6][j2 + k5] = abyte1[byte6];
							break;
						}

						if (byte6 == byte5) {
							abyte0[byte5] = MapData[i3 + i6][j2 + k5];
							abyte1[byte5] = byte2;
							byte5++;
							vector3.addElement(vector3.elementAt(MapData[i3
									+ i6][j2 + k5]));
							vector4.addElement(new Integer(byte4));
							if (byte4 == 5)
								vector5.addElement(new Integer(-1));
							else
								vector5.addElement(new Integer(byte1));
							MapData[i3 + i6][j2 + k5] = byte2;
							byte2++;
						}
					}

				}

				if (byte3 != 8) {
					specialX[byte1] = j2 << 4;
					specialY[byte1] = i3 << 4;
					specialType[byte1] = byte3;
					specialDamage[byte1] = 2;
					byte1++;
				}
			}
		}

		TileTextureData = new byte[byte2];
		TileBlockData = new byte[byte2];
		TileSpecialData = new byte[byte2];
		for (int l5 = 0; l5 < byte2; l5++) {
			TileTextureData[l5] = (byte) ((Integer) vector3.elementAt(l5))
					.intValue();
			TileBlockData[l5] = (byte) ((Integer) vector4.elementAt(l5))
					.intValue();
			TileSpecialData[l5] = (byte) ((Integer) vector5.elementAt(l5))
					.intValue();
		}

		abyte0 = new byte[28];
		byte byte7 = 0;
		for (int j6 = 0; j6 < 28; j6++)
			abyte0[j6] = 0;

		for (int k6 = 0; k6 < byte2; k6++)
			if (abyte0[TileTextureData[k6]] == 0) {
				abyte0[TileTextureData[k6]] = byte7;
				byte7++;
			}

		vector = null;
		vector1 = null;
		vector2 = null;
		vector3 = null;
		vector4 = null;
		vector5 = null;
		tilesImage = null;
		System.gc();
		GetSpecialImage("ground");
		tilesImage = Image.createImage(byte7 << 4, 16);
		Graphics g = tilesImage.getGraphics();
		for (int l6 = 0; l6 < 28; l6++)
			abyte0[l6] = 0;

		byte7 = 0;
		for (int i7 = 0; i7 < byte2; i7++)
			if (abyte0[TileTextureData[i7]] == 0) {
				g.setClip(byte7 << 4, 0, 16, 16);
				g.drawImage(specialImage, byte7 - TileTextureData[i7] % 7 << 4,
						-(TileTextureData[i7] / 7) << 4, 20);
				abyte0[TileTextureData[i7]] = byte7;
				TileTextureData[i7] = byte7;
				byte7++;
			} else {
				TileTextureData[i7] = abyte0[TileTextureData[i7]];
			}

		specialImage = null;
		System.gc();
		if (byte1 > 0) {
			specialArrayStart = 0;
			specialArrayNext[byte1 - 1] = -1;
			specialArrayPrevious[0] = -1;
			if (byte1 > 1) {
				for (int j7 = byte1 - 2; j7 >= 0; j7--) {
					specialArrayNext[j7] = (byte) (j7 + 1);
					specialArrayPrevious[j7 + 1] = (byte) j7;
				}

			}
		} else {
			specialArrayStart = -1;
		}
		playerAimX[0] = playerAimX[1] = playerAimY[0] = playerAimY[1] = -1;
		int k7 = Math.min(playerY[currentPlayer] + 16 + 22, 2048);
		screenImageTop = k7 - height;
		k7 = (k7 + 15 >> 4) << 4;
		screenSplit = k7 - screenImageHeight;
		screenImageDiscontinuity = screenImageHeight;
		int l7 = k7 >> 4;
		int i8 = screenImageHeight >> 4;
		for (int j8 = i8 - 1; j8 >= 0; j8--) {
			l7--;
			for (int k8 = 10; k8 >= 0; k8--) {
				screenImageGraphics.setClip(k8 << 4, j8 << 4, 16, 16);
				byte byte8 = TileTextureData[MapData[l7][k8]];
				screenImageGraphics.drawImage(tilesImage, k8 - byte8 << 4,
						j8 << 4, 20);
			}

		}

		GetSpecialImage("special");
		GetGameImages(true);
	}

	public final void pause() {
		cod.notifyPaused();
	}

	private final void readRecords() {
		try {
			RecordStore recordstore = RecordStore.openRecordStore("cod", false);
			byte abyte0[] = recordstore.getRecord(1);
			recordstore.closeRecordStore();
			missionNumberMax = abyte0[0];
			soundOn = abyte0[1] > 0;
			for (int i = 0; i <= missionNumberMax; i++) {
				missionTime[i] = 0;
				for (int j = 0; j < 4; j++)
					missionTime[i] = (missionTime[i] << 8)
							+ (abyte0[2 + 4 * i + j] & 0xff);

			}

		} catch (RecordStoreNotFoundException recordstorenotfoundexception) {
			writeRecords();
		} catch (Exception exception) {
		}
	}

	private final void writeRecords() {
		try {
			byte abyte0[] = new byte[4 * missionNumberMax + 6];
			abyte0[0] = missionNumberMax;
			abyte0[1] = (byte) (soundOn ? 1 : 0);
			for (int i = 0; i <= missionNumberMax; i++) {
				for (int j = 0; j < 4; j++)
					abyte0[2 + 4 * i + j] = (byte) (missionTime[i] >>> (3 - j) * 8 & 0xff);

			}

			RecordStore recordstore = RecordStore.openRecordStore("cod", true);
			if (recordstore.getNumRecords() == 0)
				recordstore.addRecord(abyte0, 0, abyte0.length);
			else
				recordstore.setRecord(1, abyte0, 0, abyte0.length);
			recordstore.closeRecordStore();
		} catch (Exception exception) {
		}
	}

	private void PlaySound(Sound sound) {
		if (soundOn)
			sound.play(1);
	}

	private void StopSound(Sound sound) {
		if (soundOn)
			sound.stop();
	}

	public final void exitGame() {
		cod.midletExit();
	}

	public final void paint(Graphics g) {
		if (screenType == 2) {
			g.drawImage(screenImage, -screenImageLeft, screenSplit
					- screenImageTop, 20);
			g.drawImage(screenImage, -screenImageLeft, screenSplit
					- screenImageTop - screenImageHeight, 20);
			for (byte byte0 = specialArrayStart; byte0 >= 0; byte0 = specialArrayNext[byte0]) {
				byte byte7 = specialType[byte0];
				if (byte7 < 0) {
					byte7 = (byte) (-1 - byte7);
					int i2 = (specialY[byte0] - screenImageTop)
							+ CRATER_DRAW_Y[byte7];
					if (i2 < height && i2 + CRATER_GFX_H[byte7] >= 0) {
						g.setClip(specialX[byte0] - screenImageLeft, i2,
								CRATER_GFX_W[byte7], CRATER_GFX_H[byte7]);
						g.drawImage(arrowsImage, specialX[byte0]
								- screenImageLeft - CRATER_GFX_X[byte7], i2
								- CRATER_GFX_Y[byte7], 20);
					}
				} else if (byte7 != 3) {
					g.setClip(specialX[byte0] - screenImageLeft,
							specialY[byte0] - screenImageTop,
							SPECIAL_GFX_W[byte7], SPECIAL_GFX_H[byte7]);
					if (byte7 == 7)
						g.drawImage(specialImage,
								specialX[byte0] - screenImageLeft
										- ((frameCount & 1) << 4) - 0,
								specialY[byte0] - screenImageTop - 32, 20);
					else
						g.drawImage(specialImage, specialX[byte0]
								- screenImageLeft - SPECIAL_GFX_X[byte7],
								specialY[byte0] - screenImageTop
										- SPECIAL_GFX_Y[byte7], 20);
				}
			}

			for (byte byte1 = projectileArrayUsedStart; byte1 >= 0; byte1 = projectileArrayNext[byte1])
				switch (projectileType[byte1]) {
				case 99: // 'c'
					int i = projectileX[byte1] - screenImageLeft;
					int j2 = projectileY[byte1] - screenImageTop;
					byte byte9 = projectileDirection[byte1];
					g.setClip(i - bulletClipDeltaX[byte9], j2
							- bulletClipDeltaY[byte9], bulletClipWidth[byte9],
							bulletClipHeight[byte9]);
					g.drawImage(specialImage, i - bulletImageDeltaX[byte9], j2
							- bulletImageDeltaY[byte9], 20);
					RedrawSceneryAbove(g, projectileX[byte1]
							- bulletClipDeltaX[byte9], projectileY[byte1]
							- bulletClipDeltaY[byte9], bulletClipWidth[byte9],
							bulletClipHeight[byte9], (byte) -1);
					break;

				case 2: // '\002'
					int j = projectileX[byte1] - screenImageLeft - 8;
					int k2 = projectileY[byte1] - screenImageTop - 8;
					g.setClip(j, k2, 16, 16);
					g.drawImage(specialImage, j - ((frameCount & 1) << 4) - 48,
							k2 - 32, 20);
					break;
				}

			squareMax = 0;
			byte byte2 = playerArrayStart;
			Image image = unitImage;
			if (byte2 == opponentArrayStart)
				image = titleImage;
			while (byte2 >= 0) {
				int l2 = playerY[byte2];
				if (l2 <= screenImageTop + height && l2 + 16 >= screenImageTop) {
					int k = playerX[byte2];
					g.setClip(k - screenImageLeft, l2 - screenImageTop, 16, 16);
					byte byte10 = playerDirection[byte2];
					if (byte10 > 4)
						byte10--;
					if (playerHealth[byte2] < 0) {
						byte10 = (byte) (-playerHealth[byte2]);
						if ((byte10 & 1) == 1) {
							byte10 = (byte) (byte10 >> 1 & 7);
							if (byte10 > 3)
								byte10++;
							g.drawImage(image, k - screenImageLeft - 16, l2
									- screenImageTop - (byte10 << 4), 20);
						}
					} else {
						g.drawImage(image, k
								- screenImageLeft
								- (playerMoving[byte2] ? (frameCount >> 1) << 4
										: 16), l2 - screenImageTop
								- (byte10 << 4), 20);
					}
					RedrawSceneryAbove(g, k, l2, 15, 15, byte2);
					if (TileBlockData[MapData[l2 + 8 >> 4][k + 8 >> 4]] == 3) {
						g.setClip(k - screenImageLeft,
								(l2 - screenImageTop) + 10, 16, 6);
						g.drawImage(screenImage, -screenImageLeft, screenSplit
								- screenImageTop, 20);
						g.drawImage(screenImage, -screenImageLeft, screenSplit
								- screenImageTop - screenImageHeight, 20);
					}
				}
				byte2 = playerArrayNext[byte2];
				if (byte2 == opponentArrayStart)
					image = titleImage;
			}
			for (byte byte11 = 0; byte11 < squareMax; byte11++) {
				g.setClip((squareCol[byte11] << 4) - screenImageLeft,
						(squareRow[byte11] << 4) - screenImageTop, 16, 16);
				byte byte3 = squarePlayer[byte11];
				if (byte3 < 3)
					g.setColor(65280);
				else
					g.setColor(0xff0000);
				g.fillRect((playerX[byte3] - screenImageLeft) + 5,
						(playerY[byte3] - screenImageTop) + 5, 6, 6);
			}

			for (byte byte4 = specialArrayExplodableStart; byte4 >= 0; byte4 = specialArrayNext[byte4]) {
				byte byte8 = specialType[byte4];
				if (byte8 == 3) {
					g.setClip(specialX[byte4] - screenImageLeft,
							specialY[byte4] - screenImageTop,
							SPECIAL_GFX_W[byte8], SPECIAL_GFX_H[byte8]);
					g.drawImage(specialImage, specialX[byte4] - screenImageLeft
							- SPECIAL_GFX_X[byte8], specialY[byte4]
							- screenImageTop - SPECIAL_GFX_Y[byte8], 20);
				}
			}

			for (byte byte5 = projectileArrayUsedStart; byte5 >= 0; byte5 = projectileArrayNext[byte5])
				switch (projectileType[byte5]) {
				case 0: // '\0'
					int l = projectileX[byte5] - screenImageLeft - 3;
					int i3 = projectileY[byte5] - screenImageTop - 5;
					g.setClip(l, i3, 7, 10);
					g.drawImage(specialImage, l - 1 - 64, i3, 20);
					break;

				case 1: // '\001'
					int i1 = projectileX[byte5] - screenImageLeft - 2;
					int j3 = projectileY[byte5] - screenImageTop - 2;
					g.setClip(i1, j3, 5, 5);
					g.drawImage(specialImage, i1 - 74, j3 - 15, 20);
					break;

				case 10: // '\n'
					int j1 = projectileX[byte5] - screenImageLeft - 16;
					int k3 = projectileY[byte5] - screenImageTop - 16;
					g.setClip(j1, k3, 32, 32);
					g.drawImage(arrowsImage, j1 - (projectileTime[byte5] << 5),
							k3 - 32, 20);
					break;
				}

			for (byte byte6 = playerArrayStart; byte6 != opponentArrayStart; byte6 = playerArrayNext[byte6])
				if (playerShowStatus[byte6] > clockTicks
						&& playerHealth[byte6] >= 0) {
					int k1 = playerX[byte6] - screenImageLeft;
					int l3 = playerY[byte6] - screenImageTop;
					g.setClip(k1 - 3, l3 + 16, 21, 7);
					g.setColor(0);
					g.fillRect(k1 - 3, l3 + 16, 21, 7);
					g.setColor(0xff0000);
					g.fillRect(k1 + 5, l3 + 18, playerHealth[byte6] * 4, 3);
					g.setClip(k1 - 2, l3 + 17, 5, 5);
					g.drawImage(symbolsImage, k1 - 2 - playerType[byte6] * 5,
							l3 + 17, 20);
					g.setClip(k1 + 4, l3 + 17, 13, 5);
					if (byte6 == currentPlayer)
						g.drawImage(symbolsImage, k1 - 35, l3 + 17, 20);
					else
						g.drawImage(symbolsImage, k1 - 22, l3 + 17, 20);
				}

			g.setClip(0, 0, width, height);
			g.drawImage(statusBar, 1, height - 14 - 1, 20);
			if (playerProtected[currentPlayer] > clockTicks) {
				g.setClip(67, height - 14 - 1, 12, 14);
				g.drawImage(symbolsImage, -15, height - 17, 20);
				g.setClip(0, 0, width, height);
			}
			if (showOrders > clockTicks) {
				g.setColor(0xffffff);
				g.setFont(font);
				if (takeCover)
					g.drawString("Orders: Take Cover", width - 2, height
							- fontHeight, 24);
				else
					g.drawString("Orders: Follow Me", width - 2, height
							- fontHeight, 24);
			}
		} else {
			g.setColor(0);
			g.fillRect(0, 0, width, height);
			g.setColor(0xffffff);
			g.setFont(largeFont);
			if (screenType >= 100) {
				g.drawString(Strings.helpTitles[screenType - 100], halfWidth,
						4, 17);
				ShowButtons(g, null, "BACK");
				g.setFont(font);
				PrintTextStrings(g);
			} else {
				switch (screenType) {
				case 2: // '\002'
				default:
					break;

				case 0: // '\0'
					if (frameCount == 2)
						g.drawImage(titleImage, halfWidth, halfHeight, 3);
					else
						g.drawImage(specialImage, halfWidth, halfHeight, 3);
					break;

				case 1: // '\001'
					g.drawImage(titleImage, halfWidth, 4, 17);
					PrintMenuStrings(g);
					break;

				case 3: // '\003'
					g.drawString("Help", halfWidth, 4, 17);
					ShowButtons(g, "SELECT", "BACK");
					PrintMenuStrings(g);
					break;

				case 4: // '\004'
				case 5: // '\005'
				case 14: // '\016'
					g.drawString("Options", halfWidth, 4, 17);
					ShowButtons(g, "SELECT", "BACK");
					PrintMenuStrings(g);
					break;

				case 15: // '\017'
					g.drawString("Reset Game", halfWidth, 4, 17);
					g.drawString("Are you sure?", halfWidth, halfHeight, 17);
					ShowButtons(g, "CONFIRM", "BACK");
					break;

				case 6: // '\006'
					g.drawImage(titleImage, halfWidth, 4, 17);
					ShowButtons(g, "SELECT", "BACK");
					PrintMenuStrings(g);
					break;

				case 7: // '\007'
				case 8: // '\b'
					g.drawString(Strings.missionTitle[missionNumberCurrent],
							halfWidth, 4, 17);
					ShowButtons(g, "CONTINUE", screenType != 7 ? "MENU"
							: "BACK");
					g.setFont(font);
					PrintTextStrings(g);
					break;

				case 11: // '\013'
					g.drawString(Strings.missionTitle[missionNumberCurrent],
							halfWidth, 4, 17);
					if (missionNumberCurrent < 8)
						ShowButtons(g, "CONTINUE", "MENU");
					else
						ShowButtons(g, null, "MENU");
					g.setFont(font);
					PrintTextStrings(g);
					break;

				case 12: // '\f'
				case 13: // '\r'
					ShowButtons(g, "RESTART", "MENU");
					g.drawString("All units killed", halfWidth, halfHeight, 17);
					break;

				case 9: // '\t'
					int l1 = halfWidth - 22;
					int i4 = 23;
					for (int j4 = 0; j4 < 3; j4++) {
						g.setClip(l1, i4, 12, 12);
						if (playerType[j4] >= 0)
							g.drawImage(symbolsImage, l1 - 12 * playerType[j4],
									i4 - 5, 20);
						else
							g.drawImage(symbolsImage, l1 - 60, i4 - 5, 20);
						l1 += 16;
					}

					l1 = 10;
					i4 += 16;
					g.setClip(l1, i4, 32, 48);
					g.drawImage(specialImage, l1 - 31 * frameCount, i4, 20);
					g.setClip(l1 + 4, i4 + 56, 5, 9);
					g.drawImage(arrowsImage, l1 + 4, i4 + 52, 20);
					g.setClip(l1 + 22, i4 + 56, 5, 9);
					g.drawImage(arrowsImage, l1 + 18, i4 + 52, 20);
					g.setClip(0, 0, width, height);
					g.drawString("Select Squad", halfWidth, 4, 17);
					if (playerType[0] == frameCount
							|| playerType[1] == frameCount
							|| playerType[2] == frameCount)
						g.drawString("DROP", 4, height - 4 - largeFontHeight,
								20);
					else if (playerType[0] < 0 || playerType[1] < 0
							|| playerType[2] < 0)
						g.drawString("SELECT", 4, height - 4 - largeFontHeight,
								20);
					if (playerType[0] >= 0 && playerType[1] >= 0
							&& playerType[2] >= 0)
						g.drawString("CONTINUE", width - 4, height - 4
								- largeFontHeight, 24);
					g.setFont(font);
					l1 = halfWidth + 16;
					i4 = halfHeight - 20;
					g.drawString(Strings.playerName[frameCount], l1, i4, 17);
					g.drawString("Armor: ", l1, i4 + fontHeight + 4, 17);
					g
							.drawString(
									Strings.armourLevel[INITIAL_HEALTH[frameCount] - 1],
									l1, i4 + 2 * fontHeight + 4, 17);
					g.drawString("Special: ", l1, i4 + 3 * fontHeight + 4, 17);
					g.drawString(Strings.playerSpecialName[frameCount], l1, i4
							+ 4 * fontHeight + 4, 17);
					break;

				case 10: // '\n'
					g.drawString("Select Squad", halfWidth, 4, 17);
					ShowButtons(g, "CONFIRM", "CHANGE");
					for (int k4 = 0; k4 < 3; k4++) {
						g.setClip((halfWidth - 50) + 34 * k4, halfHeight - 24,
								32, 48);
						g.drawImage(specialImage, ((halfWidth - 50) + 34 * k4)
								- 31 * playerType[k4], halfHeight - 24, 20);
					}

					break;

				case 16: // '\020'
					g.drawImage(titleImage, halfWidth, 4, 17);
					ShowButtons(g, "CONTINUE", null);
					g.setFont(font);
					PrintTextStrings(g);
					break;
				}
			}
		}
	}

	private void PrintMenuStrings(Graphics g) {
		for (int i = 0; i < menuLines; i++)
			g.drawString(menuStringArray[(menuStart + i) - 1], halfWidth,
					menuGraphicHeight + 1 + menuLineHeight * i, 17);

		int j = menuGraphicHeight + menuLineHeight
				* (menuSelection - menuStart);
		g.setClip(4, j, 16, 16);
		g.drawImage(unitImage, 4 - (frameCount << 4), j - 64, 20);
		g.setClip(width - 4 - 16, j, 16, 16);
		g.drawImage(unitImage, width - 4 - 16 - (frameCount << 4), j - 48, 20);
		if (menuStart > 1 || menuStart + menuLines <= menuLength) {
			g.setClip(halfWidth - 3, height - 4 - 9, 7, 4);
			g.drawImage(arrowsImage, halfWidth - 4, height - 4
					- (menuStart != 1 ? 13 : 9), 20);
			g.setClip(halfWidth - 3, height - 4 - 4, 7, 4);
			g.drawImage(arrowsImage, halfWidth - 4, height - 4
					- (menuStart + menuLines <= menuLength ? 13 : 17), 20);
		}
	}

	private void PrintTextStrings(Graphics g) {
		for (int i = 0; i < menuLines; i++)
			g.drawString(menuStringArray[(menuStart + i) - 1], 4,
					menuGraphicHeight + 1 + menuLineHeight * i, 20);

		if (menuStart > 1 || menuStart + menuLines <= menuLength) {
			int j = menuLines * menuLineHeight;
			g.drawRect(width - 4 - 7, menuGraphicHeight + 5, 6, j - 11);
			if (menuLines >= menuLength)
				g.fillRect(width - 4 - 5, menuGraphicHeight + 7, 3, j - 14);
			else
				g.fillRect(width - 4 - 5, menuGraphicHeight
						+ ((menuStart - 1) * (j - 14)) / menuLength + 7, 3,
						(menuLines * (j - 14)) / menuLength);
			g.setClip(width - 4 - 7, menuGraphicHeight, 7, 4);
			g.drawImage(arrowsImage, width - 4 - 8,
					menuStart != 1 ? menuGraphicHeight - 4
							: ((int) (menuGraphicHeight)), 20);
			g.setClip(width - 4 - 7, (menuGraphicHeight + j) - 4, 7, 4);
			g.drawImage(arrowsImage, width - 4 - 8, (menuGraphicHeight + j)
					- (menuStart + menuLines <= menuLength ? 13 : 17), 20);
		}
	}

	private void ShowButtons(Graphics g, String s, String s1) {
		if (s != null)
			g.drawString(s, 4, height - 4 - largeFontHeight, 20);
		if (s1 != null)
			g.drawString(s1, width - 4, height - 4 - largeFontHeight, 24);
	}

	private void CheckTextUpAndDown(int i) {
		if (i == 1 && menuStart > 1)
			menuStart--;
		else if (i == 6 && menuStart + menuLines <= menuLength)
			menuStart++;
	}

	private void CheckMenuUpAndDown(int i) {
		if (i == 1 && menuSelection > 1) {
			menuSelection--;
			if (menuSelection < menuStart)
				menuStart--;
			repaint();
		} else if (i == 6 && menuSelection < menuLength) {
			menuSelection++;
			if (menuSelection >= menuStart + menuLines)
				menuStart++;
			repaint();
		}
	}

	private void UpdateStatusBar() {
		Graphics g = statusBar.getGraphics();
		g.setColor(0);
		g.fillRect(0, 0, 66, 14);
		g.setClip(1, 1, 12, 12);
		g.drawImage(symbolsImage, 1 - 12 * playerType[currentPlayer], -4, 20);
		g.setClip(0, 0, 66, 14);
		for (int i = 0; i < 3; i++) {
			g.setColor(0xffffff);
			g.drawRect(14 + i * 9, 1, 9, 11);
			if (playerHealth[currentPlayer] > i) {
				g.setColor(0xff0000);
				g.fillRect(15 + i * 9, 2, 8, 10);
			}
		}

		g.setColor(0xffffff);
		g.fillRect(43, 1, 22, 12);
		if (playerType[currentPlayer] != 4 && playerType[currentPlayer] != 3) {
			g.setClip(43, 2, 10, 10);
			g.drawImage(specialImage, -21,
					2 - 10 * WEAPON_TYPE[playerType[currentPlayer]], 20);
			g.setClip(0, 0, 66, 14);
			g.setColor(0);
			g.setFont(font);
			g.drawString("" + playerSpecial[currentPlayer], 64, 2, 24);
		}
	}

	private void SetInvisibility(byte byte0, boolean flag) {
		if (flag) {
			if (byte0 == currentPlayer) {
				Graphics g = statusBar.getGraphics();
				g.setClip(43, 2, 10, 10);
				g.drawImage(symbolsImage, -29, -4, 20);
			}
			playerSpecial[byte0] = clockTicks + 180;
		} else {
			if (byte0 == currentPlayer) {
				Graphics g1 = statusBar.getGraphics();
				g1.setClip(43, 2, 10, 10);
				g1.setColor(0xffffff);
				g1.fillRect(43, 2, 10, 10);
			}
			playerSpecial[byte0] = 0;
		}
	}

	private void RedrawSceneryAbove(Graphics g, int i, int j, int k, int l,
			byte byte0) {
		for (int i1 = j >> 4; i1 <= j + l >> 4; i1++)
			if (i1 >= 0 && i1 < 128) {
				for (int j1 = i >> 4; j1 <= i + k >> 4; j1++)
					if (j1 >= 0 && j1 < 11
							&& TileBlockData[MapData[i1][j1]] == 2) {
						g.setClip((j1 << 4) - screenImageLeft, (i1 << 4)
								- screenImageTop, 16, 16);
						g.drawImage(tilesImage, (j1
								- TileTextureData[MapData[i1][j1]] << 4)
								- screenImageLeft, (i1 << 4) - screenImageTop,
								20);
						if (byte0 >= 0 && playerHealth[byte0] >= 0
								&& squareMax < 30) {
							squarePlayer[squareMax] = byte0;
							squareRow[squareMax] = (byte) i1;
							squareCol[squareMax] = (byte) j1;
							squareMax++;
						}
					}

			}

	}

	protected void keyPressed(int i) {
		if (screenType == 2) {
			nextKeyCode = i;
		} else {
			int j = getGameAction(i);
			if (screenType >= 100) {
				if (i == -7) {
					short word0 = (short) (screenType - 99);
					SetMenuScreenType((short) 3);
					StartSimpleTimer((short) 2, 100);
					SetMenuSelection(word0);
				} else {
					CheckTextUpAndDown(j);
				}
			} else {
				switch (screenType) {
				case 2: // '\002'
				case 13: // '\r'
				case 14: // '\016'
				default:
					break;

				case 0: // '\0'
					simpletimertask.cancel();
					GoToMainMenu(true);
					break;

				case 1: // '\001'
					if (i == -6 || j == 8)
						switch (menuSelection) {
						default:
							break;

						case 1: // '\001'
							if (missionNumberMax > 0) {
								SetMenuScreenType((short) 6);
							} else {
								simpletimertask.cancel();
								SetMenuScreenType((short) 7);
							}
							break;

						case 2: // '\002'
							SetMenuScreenType((short) 4);
							break;

						case 3: // '\003'
							SetMenuScreenType((short) 3);
							break;

						case 4: // '\004'
							simpletimertask.cancel();
							SetMenuScreenType((short) 16);
							break;
						}
					else
						CheckMenuUpAndDown(j);
					break;

				case 3: // '\003'
					if (i == -6 || j == 8) {
						simpletimertask.cancel();
						SetMenuScreenType((short) (99 + menuSelection));
						break;
					}
					if (i == -7) {
						GoToMainMenu(false);
						SetMenuSelection((short) 3);
						break;
					}
					if (i == 49) {
						if (clockTicks == 0)
							clockTicks++;
						break;
					}
					if (i == 51) {
						if (clockTicks == 2)
							clockTicks++;
						else
							clockTicks = 0;
						break;
					}
					if (i == 57) {
						if (clockTicks == 1) {
							clockTicks++;
							break;
						}
						if (clockTicks == 3)
							missionNumberMax = 8;
						else
							clockTicks = 0;
					} else {
						CheckMenuUpAndDown(j);
					}
					break;

				case 4: // '\004'
					if (i == -6 || j == 8) {
						switch (menuSelection) {
						case 1: // '\001'
							soundOn = !soundOn;
							menuStringArray[0] = soundOn ? "Sound: ON"
									: "Sound: OFF";
							writeRecords();
							break;

						case 2: // '\002'
							simpletimertask.cancel();
							screenType = 15;
							break;
						}
						break;
					}
					if (i == -7) {
						GoToMainMenu(false);
						SetMenuSelection((short) 2);
					} else {
						CheckMenuUpAndDown(j);
					}
					break;

				case 15: // '\017'
					if (i == -6 || j == 8) {
						missionNumberMax = 0;
						missionNumberCurrent = 0;
						for (int k = 0; k <= 8; k++)
							missionTime[k] = 0;

						writeRecords();
						GoToMainMenu(true);
						break;
					}
					if (i == -7) {
						StartSimpleTimer((short) 2, 100);
						screenType = 4;
					}
					break;

				case 5: // '\005'
					if (i == -6 || j == 8) {
						switch (menuSelection) {
						case 1: // '\001'
							soundOn = !soundOn;
							menuStringArray[0] = soundOn ? "Sound: ON"
									: "Sound: OFF";
							writeRecords();
							break;

						case 2: // '\002'
							simpletimertask.cancel();
							tilesImage = null;
							specialImage = null;
							titleImage = null;
							arrowsImage = null;
							System.gc();
							screenType = 14;
							StartSimpleTimer((short) 4, 400);
							break;

						case 3: // '\003'
							GetGameImages(false);
							GoToMainMenu(false);
							break;
						}
						break;
					}
					if (i == -7) {
						simpletimertask.cancel();
						frameCount = 0;
						StartSimpleTimer((short) 0, 50);
						screenType = 2;
					} else {
						CheckMenuUpAndDown(j);
					}
					break;

				case 6: // '\006'
					if (i == -6 || j == 8) {
						simpletimertask.cancel();
						missionNumberCurrent = (byte) ((missionNumberMax - menuSelection) + 1);
						SetMenuScreenType((short) 7);
						break;
					}
					if (i == -7)
						GoToMainMenu(false);
					else
						CheckMenuUpAndDown(j);
					break;

				case 7: // '\007'
				case 8: // '\b'
					if (i == -6 || j == 8) {
						GetSpecialImage("select");
						playerType[0] = playerType[1] = playerType[2] = -1;
						frameCount = 0;
						screenType = 9;
						break;
					}
					if (i == -7) {
						if (screenType == 8 || missionNumberMax == 0) {
							GoToMainMenu(true);
						} else {
							StartSimpleTimer((short) 2, 100);
							SetMenuScreenType((short) 6);
						}
					} else {
						CheckTextUpAndDown(j);
					}
					break;

				case 9: // '\t'
					if (i == -6 || j == 8) {
						int l = -1;
						int i1 = -1;
						for (int j1 = 0; j1 < 3; j1++) {
							if (playerType[j1] < 0 && l < 0)
								l = j1;
							if (playerType[j1] != frameCount)
								continue;
							i1 = j1;
							break;
						}

						if (i1 >= 0) {
							playerType[i1] = -1;
							for (; i1 < 2; i1++) {
								playerType[i1] = playerType[i1 + 1];
								playerType[i1 + 1] = -1;
							}

							break;
						}
						if (l >= 0) {
							playerType[l] = frameCount;
							frameCount = (byte) ((frameCount + 1) % 5);
						}
						break;
					}
					if (i == -7 && playerType[0] >= 0 && playerType[1] >= 0
							&& playerType[2] >= 0) {
						screenType = 10;
						break;
					}
					if (j == 2 || j == 1) {
						frameCount--;
						if (frameCount < 0)
							frameCount = 4;
						break;
					}
					if (j == 5 || j == 6)
						frameCount = (byte) ((frameCount + 1) % 5);
					break;

				case 10: // '\n'
					if (i == -6 || j == 8) {
						menuStringArray = null;
						specialImage = null;
						titleImage = null;
						arrowsImage = null;
						System.gc();
						ReadMap();
						System.gc();
						frameCount = 0;
						showOrders = 0;
						UpdateStatusBar();
						clockTicks = 0;
						endLevel = false;
						screenType = 2;
						startTime = System.currentTimeMillis();
						StartSimpleTimer((short) 0, 50);
						break;
					}
					if (i == -7) {
						frameCount = 0;
						screenType = 9;
					}
					break;

				case 11: // '\013'
					if ((i == -6 || j == 8) && missionNumberCurrent < 8) {
						missionNumberCurrent++;
						GetGameImages(false);
						SetMenuScreenType((short) 8);
						break;
					}
					if (i == -7) {
						GetGameImages(false);
						GoToMainMenu(true);
					} else {
						CheckTextUpAndDown(j);
					}
					break;

				case 12: // '\f'
					if (i == -6 || j == 8) {
						tilesImage = null;
						specialImage = null;
						titleImage = null;
						arrowsImage = null;
						System.gc();
						screenType = 13;
						StartSimpleTimer((short) 4, 400);
						break;
					}
					if (i == -7) {
						GetGameImages(false);
						GoToMainMenu(true);
					}
					break;

				case 16: // '\020'
					if (i == -6 || j == 8)
						cod.midletExit();
					else
						CheckTextUpAndDown(j);
					break;
				}
			}
			repaint();
		}
	}

	private void GoToMainMenu(boolean flag) {
		SetMenuScreenType((short) 1);
		frameCount = 0;
		if (flag)
			StartSimpleTimer((short) 2, 100);
	}

	private void SetMenuSelection(short word0) {
		menuStart = 1;
		menuSelection = word0;
		if (menuSelection >= menuStart + menuLines)
			menuStart = (short) ((menuSelection - menuLines) + 1);
	}

	protected void keyReleased(int i) {
		if (nextKeyCode == i)
			nextKeyCode = 0;
	}

	private void StartSimpleTimer(short word0, int i) {
		simpletimertask = new SimpleTimerTask(this, word0);
		timer.schedule(simpletimertask, i);
	}

	public void SimpleTimerEndEvent(short word0) {
		if (word0 == 0) {
			clockTicks++;
			if (frameCount == 5)
				frameCount = 0;
			else
				frameCount++;
			currentKeyCode = nextKeyCode;
			byte byte0 = -1;
			for (byte byte1 = playerArrayStart; byte1 >= 0; byte1 = playerArrayNext[byte1])
				if (playerHealth[byte1] < 0) {
					playerHealth[byte1]--;
					if (playerHealth[byte1] + 30 < 0) {
						if (byte0 >= 0) {
							playerArrayNext[byte0] = playerArrayNext[byte1];
						} else {
							playerArrayStart = playerArrayNext[byte1];
							if (playerArrayStart == opponentArrayStart) {
								PlaySound(loseSound);
								screenType = 12;
								allowKeyPresses = false;
								StartSimpleTimer((short) 3, 750);
								repaint();
								return;
							}
						}
						if (byte1 == opponentArrayStart)
							opponentArrayStart = playerArrayNext[byte1];
					} else {
						byte0 = byte1;
					}
				} else {
					byte0 = byte1;
				}

			for (byte byte2 = playerArrayStart; byte2 != opponentArrayStart; byte2 = playerArrayNext[byte2])
				if (playerType[byte2] == 3
						&& clockTicks == playerSpecial[byte2])
					SetInvisibility(byte2, false);

			byte byte7 = -1;
			byte byte9;
			for (byte byte8 = projectileArrayUsedStart; byte8 >= 0; byte8 = byte9) {
				boolean flag = false;
				byte byte10 = projectileDirection[byte8];
				projectileTime[byte8]++;
				byte9 = projectileArrayNext[byte8];
				switch (projectileType[byte8]) {
				case 99: // 'c'
					if (projectileTime[byte8] > 10) {
						flag = true;
					} else {
						projectileX[byte8] += bulletDeltaX[byte10];
						projectileY[byte8] += bulletDeltaY[byte10];
						int i = projectileX[byte8] + bulletEndDeltaX[byte10];
						int j2 = projectileY[byte8] + bulletEndDeltaY[byte10];
						if (i < 0 || i >= 176 || j2 < 0 || j2 >= 2048
								|| TileBlockData[MapData[j2 >> 4][i >> 4]] <= 1)
							flag = true;
						else
							flag = CheckPlayerDamage(i, i - 16, j2, j2 - 16,
									(byte) 1);
					}
					break;

				case 1: // '\001'
					if (projectileTime[byte8] > 2)
						flag = true;
					break;

				case 0: // '\0'
					if (projectileTime[byte8] > 8) {
						DoExplosion(byte8, 1);
					} else {
						projectileX[byte8] += bulletDeltaX[byte10];
						projectileY[byte8] += bulletDeltaY[byte10];
						int j = projectileX[byte8];
						int k2 = projectileY[byte8];
						if (j < -3 || j > 179 || k2 < -4 || k2 > 2053)
							flag = true;
					}
					break;

				case 2: // '\002'
					if (projectileTime[byte8] > 38)
						DoExplosion(byte8, 2);
					break;

				case 10: // '\n'
					if (projectileTime[byte8] > 4)
						flag = true;
					break;
				}
				if (flag) {
					if (byte7 >= 0)
						projectileArrayNext[byte7] = byte9;
					else
						projectileArrayUsedStart = byte9;
					projectileArrayNext[byte8] = projectileArrayUnusedStart;
					projectileArrayUnusedStart = byte8;
				} else {
					byte7 = byte8;
				}
			}

			playerMoving[currentPlayer] = false;
			if (currentKeyCode != 0) {
				int k6 = getGameAction(currentKeyCode);
				if (k6 == 8 || currentKeyCode == 53) {
					PlaySound(fireSound);
					StartProjectile(currentPlayer, (byte) 99);
					if (playerType[currentPlayer] == 3
							&& clockTicks < playerSpecial[currentPlayer])
						SetInvisibility(currentPlayer, false);
					nextKeyCode = 0;
				} else if (currentKeyCode == 42) {
					byte byte17 = playerType[currentPlayer];
					if (byte17 == 3
							&& clockTicks < playerSpecial[currentPlayer]) {
						PlaySound(secondarySound);
						int k = playerX[currentPlayer];
						int l2 = playerY[currentPlayer];
						for (byte byte3 = opponentArrayStart; byte3 >= 0; byte3 = playerArrayNext[byte3])
							if (playerHealth[byte3] >= 0
									&& playerX[byte3] <= k + 16
									&& playerX[byte3] >= k - 16
									&& playerY[byte3] <= l2 + 16
									&& playerY[byte3] >= l2 - 16)
								playerHealth[byte3] = -1;

						nextKeyCode = 0;
					} else if (clockTicks > playerNextShot[currentPlayer]
							&& playerSpecial[currentPlayer] > 0) {
						PlaySound(secondarySound);
						playerSpecial[currentPlayer]--;
						UpdateStatusBar();
						switch (byte17) {
						case 0: // '\0'
							StartProjectile(currentPlayer, (byte) 0);
							playerNextShot[currentPlayer] = clockTicks + 10;
							break;

						case 1: // '\001'
							StartProjectile(currentPlayer, (byte) 1);
							playerNextShot[currentPlayer] = clockTicks + 10;
							break;

						case 2: // '\002'
							StartProjectile(currentPlayer, (byte) 2);
							playerNextShot[currentPlayer] = clockTicks + 10;
							break;
						}
					}
				} else {
					if (currentKeyCode == -7) {
						SetMenuScreenType((short) 5);
						frameCount = 0;
						StartSimpleTimer((short) 2, 100);
						repaint();
						return;
					}
					if (currentKeyCode == 35) {
						currentPlayer = NextUnit(currentPlayer);
						PlaySound(unitSound);
						playerDirection[currentPlayer] = 1;
						nextKeyCode = 0;
						if (playerType[currentPlayer] == 4) {
							takeCover = true;
							showOrders = clockTicks + 50;
						}
						if (takeCover)
							SetHidingAims(currentPlayer);
						UpdateStatusBar();
						for (byte byte4 = playerArrayStart; byte4 != opponentArrayStart; byte4 = playerArrayNext[byte4])
							playerShowStatus[byte4] = clockTicks + 60;

					} else if (currentKeyCode == 48) {
						takeCover = !takeCover;
						if (takeCover)
							SetHidingAims(currentPlayer);
						else
							playerAimX[0] = playerAimX[1] = playerAimY[0] = playerAimY[1] = -1;
						showOrders = clockTicks + 50;
						nextKeyCode = 0;
					} else {
						byte byte11 = 4;
						switch (k6) {
						case 1: // '\001'
							byte11 = 1;
							break;

						case 2: // '\002'
							byte11 = 3;
							break;

						case 5: // '\005'
							byte11 = 5;
							break;

						case 6: // '\006'
							byte11 = 7;
							break;

						case 3: // '\003'
						case 4: // '\004'
						default:
							if (currentKeyCode >= 49 && currentKeyCode <= 57)
								byte11 = (byte) (currentKeyCode - 49);
							break;
						}
						if (byte11 != 4) {
							playerDirection[currentPlayer] = byte11;
							byte byte15 = ((byte) (TileBlockData[MapData[playerY[currentPlayer] + 8 >> 4][playerX[currentPlayer] + 8 >> 4]] != 3 ? 2
									: 1));
							if (byte11 < 3) {
								int k4 = MovePlayerUp(currentPlayer, byte15);
								if (k4 > 0)
									playerY[currentPlayer] -= k4;
								else
									byte11 += 3;
							} else if (byte11 > 5) {
								int l4 = MovePlayerDown(currentPlayer, byte15);
								if (l4 > 0)
									playerY[currentPlayer] += l4;
								else
									byte11 -= 3;
							}
							if (byte11 % 3 == 0) {
								int i5 = MovePlayerLeft(currentPlayer, byte15);
								if (i5 > 0)
									playerX[currentPlayer] -= i5;
								else
									byte11++;
							} else if (byte11 % 3 == 2) {
								int j5 = MovePlayerRight(currentPlayer, byte15);
								if (j5 > 0)
									playerX[currentPlayer] += j5;
								else
									byte11--;
							}
						}
						if (byte11 != 4)
							playerMoving[currentPlayer] = true;
						if (endLevel) {
							if (missionNumberCurrent == missionNumberMax
									&& missionNumberMax < 8) {
								missionNumberMax++;
								writeRecords();
							}
							PlaySound(winSound);
							SetMenuScreenType((short) 11);
							allowKeyPresses = false;
							StartSimpleTimer((short) 3, 750);
							repaint();
							return;
						}
					}
				}
			}
			if (playerDirection[currentPlayer] < 3) {
				int i3 = (screenImageTop + height) - playerY[currentPlayer]
						- 16 - 22;
				if (i3 > 0)
					MoveScreenUp(i3);
				else if (i3 < 0)
					MoveScreenDown(-i3);
			} else if (playerDirection[currentPlayer] > 5) {
				int j3 = playerY[currentPlayer] - screenImageTop - 22;
				if (j3 > 0)
					MoveScreenDown(j3);
				else if (j3 < 0)
					MoveScreenUp(-j3);
			}
			if (playerDirection[currentPlayer] % 3 == 0) {
				int l = (screenImageLeft + width) - playerX[currentPlayer] - 16
						- 22;
				if (l > 0)
					MoveScreenLeft(l);
				else if (l < 0)
					MoveScreenRight(-l);
			} else if (playerDirection[currentPlayer] % 3 == 2) {
				int i1 = playerX[currentPlayer] - screenImageLeft - 22;
				if (i1 > 0)
					MoveScreenRight(i1);
				else if (i1 < 0)
					MoveScreenLeft(-i1);
			}
			byte byte5 = NextUnit(currentPlayer);
			boolean flag1 = false;
			boolean flag2 = false;
			byte byte23 = 0;
			boolean flag7 = false;
			while (byte5 != currentPlayer) {
				if (playerHealth[byte5] >= 0) {
					byte byte21 = playerDirection[byte5];
					boolean flag6 = true;
					if (playerY[byte5] >= screenImageTop
							&& playerY[byte5] <= (screenImageTop + height) - 16
							&& playerX[byte5] >= screenImageLeft
							&& playerX[byte5] <= (screenImageLeft + width) - 16) {
						byte byte18 = opponentArrayStart;
						boolean flag3 = byte18 >= 0;
						while (flag3) {
							if (playerHealth[byte18] >= 0
									&& playerY[byte18] >= screenImageTop
									&& playerY[byte18] <= (screenImageTop + height) - 16
									&& playerX[byte18] >= screenImageLeft
									&& playerX[byte18] <= (screenImageLeft + width) - 16) {
								int j1 = playerX[byte18] - playerX[byte5];
								int k3 = playerY[byte18] - playerY[byte5];
								if (TargetAhead(byte21, j1, k3)
										&& HasClearView(playerX[byte5] + 8,
												playerY[byte5] + 8, j1, k3)) {
									byte byte12 = GetDirection(j1, k3);
									byte byte20 = NextUnit(byte5);
									boolean flag5;
									for (flag5 = true; flag5 && byte20 != byte5; byte20 = NextUnit(byte20)) {
										int k1 = playerX[byte20]
												- playerX[byte5];
										int l3 = playerY[byte20]
												- playerY[byte5];
										switch (byte12) {
										case 0: // '\0'
											flag5 = k1 > 0 || l3 > k1 + 8
													|| l3 < k1 - 8;
											break;

										case 1: // '\001'
											flag5 = l3 > 0 || k1 > 8 || k1 < -8;
											break;

										case 2: // '\002'
											flag5 = k1 < 0 || k1 + l3 > 8
													|| k1 + l3 < -8;
											break;

										case 3: // '\003'
											flag5 = k1 > 0 || l3 > 8 || l3 < -8;
											break;

										case 5: // '\005'
											flag5 = k1 < 0 || l3 > 8 || l3 < -8;
											break;

										case 6: // '\006'
											flag5 = k1 > 0 || k1 + l3 > 8
													|| k1 + l3 < -8;
											break;

										case 7: // '\007'
											flag5 = l3 < 0 || k1 > 8 || k1 < -8;
											break;

										case 8: // '\b'
											flag5 = k1 < 0 || l3 > k1 + 8
													|| l3 < k1 - 8;
											break;
										}
									}

									if (flag5) {
										if (clockTicks > playerNextShot[byte5]) {
											playerDirection[byte5] = byte12;
											StartProjectile(byte5, (byte) 99);
											playerNextShot[byte5] = clockTicks + 10;
											flag6 = false;
										}
										break;
									}
								}
							}
							byte18 = playerArrayNext[byte18];
							if (byte18 < 0)
								flag3 = false;
						}
					}
					if (flag6) {
						byte byte16 = ((byte) (TileBlockData[MapData[playerY[byte5] + 8 >> 4][playerX[byte5] + 8 >> 4]] != 3 ? 2
								: 1));
						int l6;
						int i7;
						if (byte23 == 0) {
							if (playerAimX[0] >= 0) {
								l6 = playerAimX[0];
								i7 = playerAimY[0];
							} else if (takeCover) {
								l6 = playerX[byte5];
								i7 = playerY[byte5];
							} else if (playerDirection[currentPlayer] == 3) {
								l6 = playerX[currentPlayer] + 16;
								i7 = playerY[currentPlayer] + 16;
							} else if (playerDirection[currentPlayer] == 5) {
								l6 = playerX[currentPlayer] - 16;
								i7 = playerY[currentPlayer] - 16;
							} else if (playerDirection[currentPlayer] < 3) {
								l6 = playerX[currentPlayer] - 16;
								i7 = playerY[currentPlayer] + 16;
							} else {
								l6 = playerX[currentPlayer] + 16;
								i7 = playerY[currentPlayer] - 16;
							}
						} else if (playerAimX[1] >= 0) {
							l6 = playerAimX[1];
							i7 = playerAimY[1];
						} else if (takeCover) {
							l6 = playerX[byte5];
							i7 = playerY[byte5];
						} else if (playerDirection[currentPlayer] == 3) {
							l6 = playerX[currentPlayer] + 16;
							i7 = playerY[currentPlayer] - 16;
						} else if (playerDirection[currentPlayer] == 5) {
							l6 = playerX[currentPlayer] - 16;
							i7 = playerY[currentPlayer] + 16;
						} else if (playerDirection[currentPlayer] < 3) {
							l6 = playerX[currentPlayer] + 16;
							i7 = playerY[currentPlayer] + 16;
						} else {
							l6 = playerX[currentPlayer] - 16;
							i7 = playerY[currentPlayer] - 16;
						}
						byte byte13 = 4;
						if (i7 < playerY[byte5]) {
							int k5 = MovePlayerUp(byte5, Math.min(byte16,
									playerY[byte5] - i7));
							if (k5 > 0) {
								byte13 -= 3;
								playerY[byte5] -= k5;
							}
						} else if (i7 > playerY[byte5]) {
							int l5 = MovePlayerDown(byte5, Math.min(byte16, i7
									- playerY[byte5]));
							if (l5 > 0) {
								byte13 += 3;
								playerY[byte5] += l5;
							}
						}
						if (l6 < playerX[byte5]) {
							int i6 = MovePlayerLeft(byte5, Math.min(byte16,
									playerX[byte5] - l6));
							if (i6 > 0) {
								byte13--;
								playerX[byte5] -= i6;
							}
						} else if (l6 > playerX[byte5]) {
							int j6 = MovePlayerRight(byte5, Math.min(byte16, l6
									- playerX[byte5]));
							if (j6 > 0) {
								byte13++;
								playerX[byte5] += j6;
							}
						}
						if (byte13 == 4) {
							if (takeCover) {
								playerDirection[byte5] = 1;
							} else {
								playerDirection[byte5] = playerDirection[currentPlayer];
								if (l6 == playerX[byte5]
										&& i7 == playerY[byte5])
									playerAimX[byte23] = playerAimY[byte23] = -1;
								else if (flag7 && byte23 == 1
										&& playerAimX[1] >= 0) {
									playerAimX[0] = playerAimX[1] = playerAimY[0] = playerAimY[1] = -1;
								} else {
									flag7 = playerAimX[byte23] >= 0;
									int l1 = playerX[byte5];
									int i4 = playerY[byte5];
									if (i7 < i4 - 16 && i4 >= 16) {
										if ((i4 & 0xf) == 0)
											SetAvoidanceAimUpAndDown(l1 >> 4,
													i4 >> 4, -1, byte23);
									} else if (l6 < l1 - 16 && l1 >= 16) {
										if ((l1 & 0xf) == 0)
											SetAvoidanceAimLeftAndRight(
													l1 >> 4, i4 >> 4, -1,
													byte23);
									} else if (l6 > l1 + 16 && l1 <= 144) {
										if ((l1 & 0xf) == 0)
											SetAvoidanceAimLeftAndRight(
													l1 >> 4, i4 >> 4, 1, byte23);
									} else if (i7 > i4 + 16 && i4 <= 2016
											&& (i4 & 0xf) == 0)
										SetAvoidanceAimUpAndDown(l1 >> 4,
												i4 >> 4, 1, byte23);
								}
							}
							playerMoving[byte5] = false;
						} else {
							playerDirection[byte5] = byte13;
							playerMoving[byte5] = true;
						}
					}
				}
				byte5 = NextUnit(byte5);
				byte23++;
			}
			for (byte byte6 = opponentArrayStart; byte6 >= 0; byte6 = playerArrayNext[byte6])
				if (playerHealth[byte6] >= 0
						&& playerY[byte6] >= screenImageTop
						&& playerY[byte6] <= (screenImageTop + height) - 16
						&& playerX[byte6] >= screenImageLeft
						&& playerX[byte6] <= (screenImageLeft + width) - 16) {
					byte byte22 = playerDirection[byte6];
					boolean flag4 = true;
					boolean flag8 = false;
					byte byte19 = currentPlayer;
					while (flag4) {
						if (playerHealth[byte19] >= 0
								&& (playerType[byte19] != 3 || playerSpecial[byte19] < clockTicks)
								&& playerY[byte19] >= screenImageTop
								&& playerY[byte19] <= (screenImageTop + height) - 16
								&& playerX[byte19] >= screenImageLeft
								&& playerX[byte19] <= (screenImageLeft + width) - 16) {
							int i2 = playerX[byte19] - playerX[byte6];
							int j4 = playerY[byte19] - playerY[byte6];
							flag8 = true;
							if (TargetAhead(byte22, i2, j4)
									&& HasClearView(playerX[byte6] + 8,
											playerY[byte6] + 8, i2, j4)) {
								byte byte14 = GetDirection(i2, j4);
								if (clockTicks > playerNextShot[byte6]) {
									playerDirection[byte6] = byte14;
									StartProjectile(byte6, (byte) 99);
									playerNextShot[byte6] = clockTicks + 20;
								} else {
									MoveEnemy(byte6, byte14);
								}
								break;
							}
						}
						byte19 = NextUnit(byte19);
						if (byte19 == currentPlayer) {
							if (flag8
									&& (Math.abs(random.nextInt()) % 10 < 1 || !MoveEnemy(
											byte6, playerDirection[byte6]))) {
								int j7 = Math.abs(random.nextInt()) % 10;
								if (j7 < 4)
									MoveEnemy(byte6,
											(byte) (8 - playerDirection[byte6]));
								else if (j7 < 6)
									MoveEnemy(byte6, (byte) 7);
								else if (j7 < 8)
									MoveEnemy(byte6, (byte) (j7 * 2 - 6));
								else
									MoveEnemy(byte6, (byte) (j7 * 2 - 13));
							}
							flag4 = false;
						}
					}
				}

			StartSimpleTimer((short) 0, 50);
		} else if (word0 == 1) {
			frameCount++;
			if (frameCount == 1) {
				GetSpecialImage("nGame");
				StartSimpleTimer((short) 1, 2000);
			} else if (frameCount == 2)
				StartSimpleTimer((short) 1, 3000);
			else
				GoToMainMenu(true);
		} else if (word0 == 2) {
			if (frameCount == 2)
				frameCount = 0;
			else
				frameCount++;
			StartSimpleTimer((short) 2, 100);
		} else if (word0 == 3)
			allowKeyPresses = true;
		else if (word0 == 4) {
			GetSpecialImage("select");
			GetGameImages(false);
			screenType = 10;
		}
		repaint();
	}

	private byte NextUnit(byte byte0) {
		return NextUnitRecurse(byte0, byte0);
	}

	private byte NextUnitRecurse(byte byte0, byte byte1) {
		byte byte2 = playerArrayNext[byte0];
		if (byte2 == opponentArrayStart)
			byte2 = playerArrayStart;
		if (playerHealth[byte2] >= 0 || byte2 == byte1)
			return byte2;
		else
			return NextUnitRecurse(byte2, byte1);
	}

	private void StartProjectile(byte byte0, byte byte1) {
		if (projectileArrayUnusedStart >= 0) {
			byte byte2 = StartProjectileCommonCode(byte1);
			projectileDirection[byte2] = playerDirection[byte0];
			if (byte1 == 1) {
				byte byte3 = opponentArrayStart;
				byte byte4 = -1;
				int i = 0x5f5e0ff;
				int j = playerX[byte0];
				int k = playerY[byte0];
				for (; byte3 >= 0; byte3 = playerArrayNext[byte3])
					if (playerHealth[byte3] >= 0
							&& playerY[byte3] >= screenImageTop
							&& playerY[byte3] <= (screenImageTop + height) - 16
							&& playerX[byte3] >= screenImageLeft
							&& playerX[byte3] <= (screenImageLeft + width) - 16) {
						int l = playerX[byte3] - j;
						int i1 = playerY[byte3] - k;
						int j1;
						if (l > 0)
							j1 = l;
						else
							j1 = -l;
						if (i1 > 0)
							j1 += i1;
						else
							j1 -= i1;
						if (j1 < i && HasClearView(j + 8, k + 8, l, i1)) {
							byte4 = byte3;
							i = j1;
						}
					}

				if (byte4 >= 0) {
					playerDirection[byte0] = GetDirection(playerX[byte4] - j,
							playerY[byte4] - k);
					DoPlayerDamage(byte4, (byte) 2);
				}
				projectileX[byte2] = playerX[byte0]
						+ gunFlareX[playerDirection[byte0]];
				projectileY[byte2] = playerY[byte0]
						+ gunFlareY[playerDirection[byte0]];
			} else {
				projectileX[byte2] = playerX[byte0] + 8;
				projectileY[byte2] = playerY[byte0] + 8;
			}
		}
	}

	private byte StartProjectileCommonCode(byte byte0) {
		byte byte1 = projectileArrayUnusedStart;
		projectileArrayUnusedStart = projectileArrayNext[byte1];
		projectileArrayNext[byte1] = projectileArrayUsedStart;
		projectileArrayUsedStart = byte1;
		projectileType[byte1] = byte0;
		projectileTime[byte1] = 0;
		return byte1;
	}

	private boolean TargetAhead(byte byte0, int i, int j) {
		switch (byte0) {
		case 0: // '\0'
			return i <= -j;

		case 1: // '\001'
			return j <= 0;

		case 2: // '\002'
			return i >= j;

		case 3: // '\003'
			return i <= 0;

		case 5: // '\005'
			return i >= 0;

		case 6: // '\006'
			return j >= i;

		case 7: // '\007'
			return j >= 0;

		case 8: // '\b'
			return i >= -j;

		case 4: // '\004'
		default:
			return false;
		}
	}

	private boolean HasClearView(int i, int j, int k, int l) {
		if (k < 0) {
			i += k;
			j += l;
			k = -k;
			l = -l;
		}
		int i1 = i >> 4;
		int j1 = j >> 4;
		byte byte0;
		if (l >= 0)
			byte0 = 1;
		else
			byte0 = -1;
		if (k == 0) {
			int l1 = j + l >> 4;
			if (TileBlockData[MapData[j1][i1]] <= 1)
				return false;
			for (; j1 != l1; j1 += byte0)
				if (TileBlockData[MapData[j1][i1]] <= 1)
					return false;

		} else {
			int j2 = (l << 10) / k;
			int k2 = (j << 10) - j2 * i;
			int l2 = i + k;
			boolean flag = true;
			while (flag) {
				int k1 = i1 + 1 << 4;
				int i2;
				if (k1 >= l2) {
					i2 = j + l >> 4;
					flag = false;
				} else {
					i2 = k1 * j2 + k2 >> 14;
				}
				if (TileBlockData[MapData[j1][i1]] <= 1)
					return false;
				while (j1 != i2) {
					j1 += byte0;
					if (TileBlockData[MapData[j1][i1]] <= 1)
						return false;
				}
				i1 = k1 >> 4;
			}
		}
		return true;
	}

	private byte GetDirection(int i, int j) {
		if (i == 0)
			return ((byte) (j <= 0 ? 1 : 7));
		int k = (j << 10) / i;
		byte byte0;
		if (k > 2472)
			byte0 = 7;
		else if (k > 424)
			byte0 = 8;
		else if (k > -424)
			byte0 = 5;
		else if (k > -2472)
			byte0 = 2;
		else
			byte0 = 1;
		if (i > 0)
			return byte0;
		else
			return (byte) (8 - byte0);
	}

	private boolean MoveEnemy(byte byte0, byte byte1) {
		byte byte2 = ((byte) (TileBlockData[MapData[playerY[byte0] + 8 >> 4][playerX[byte0] + 8 >> 4]] != 3 ? 2
				: 1));
		if (byte1 < 3) {
			int i = MovePlayerUp(byte0, byte2);
			if (i > 0)
				playerY[byte0] -= i;
			else
				byte1 += 3;
		} else if (byte1 > 5) {
			int j = MovePlayerDown(byte0, byte2);
			if (j > 0)
				playerY[byte0] += j;
			else
				byte1 -= 3;
		}
		if (byte1 % 3 == 0) {
			int k = MovePlayerLeft(byte0, byte2);
			if (k > 0)
				playerX[byte0] -= k;
			else
				byte1++;
		} else if (byte1 % 3 == 2) {
			int l = MovePlayerRight(byte0, byte2);
			if (l > 0)
				playerX[byte0] += l;
			else
				byte1--;
		}
		if (byte1 == 4) {
			playerMoving[byte0] = false;
			return false;
		} else {
			playerDirection[byte0] = byte1;
			playerMoving[byte0] = true;
			return true;
		}
	}

	private void SetHidingAims(byte byte0) {
		byte byte1 = NextUnit(byte0);
		for (byte byte2 = -1; byte1 != byte0; byte2 = (byte) (-byte2)) {
			SetHidingAimOneSide(byte2, byte1);
			byte1 = NextUnit(byte1);
		}

	}

	private void SetHidingAimOneSide(byte byte0, byte byte1) {
		int i1 = (byte0 + 1) / 2;
		int l1 = playerY[byte1] >> 4;
		for (int i2 = 0; i2 <= 1; i2++) {
			int l;
			if (byte0 == -1) {
				int j1 = playerX[byte1] >> 4;
				if (l1 >= 1 && TileBlockData[MapData[l1 - 1][j1]] == 0
						&& StoreHidingAim(i1, l1 << 4, j1 << 4))
					return;
				if (l1 >= 2 && TileBlockData[MapData[l1 - 2][j1]] == 0
						&& StoreHidingAim(i1, l1 - 1 << 4, j1 << 4))
					return;
				l = j1 - 1;
			} else {
				int k1 = playerX[byte1] + 15 >> 4;
				if (l1 >= 1 && TileBlockData[MapData[l1 - 1][k1]] == 0
						&& StoreHidingAim(i1, l1 << 4, k1 << 4))
					return;
				if (l1 >= 2 && TileBlockData[MapData[l1 - 2][k1]] == 0
						&& StoreHidingAim(i1, l1 - 1 << 4, k1 << 4))
					return;
				l = k1 + 1;
			}
			if (l1 >= 1 && l >= 0 && l < 11
					&& TileBlockData[MapData[l1 - 1][l]] == 0
					&& StoreHidingAim(i1, l1 << 4, l << 4))
				return;
			if (l1 >= 2 && l >= 0 && l < 11
					&& TileBlockData[MapData[l1 - 2][l]] == 0
					&& StoreHidingAim(i1, l1 - 1 << 4, l << 4))
				return;
			for (int i = 0; i < 4 && l >= 0 && l < 11;) {
				int j = 0;
				int k = l1 - 3;
				if (k < 0)
					k = 0;
				boolean flag = false;
				for (; j < 7 && k < 128; k++) {
					if (TileBlockData[MapData[k][l]] == 0)
						flag = true;
					else if (flag) {
						if (StoreHidingAim(i1, k << 4, l << 4))
							return;
						flag = false;
					}
					j++;
				}

				i++;
				l += byte0;
				if (flag)
					i = 99;
			}

			byte0 = (byte) (-byte0);
		}

		playerAimX[i1] = -1;
		playerAimY[i1] = -1;
	}

	private boolean StoreHidingAim(int i, int j, int k) {
		if (i == 1 && playerAimX[0] == k && playerAimY[0] == j) {
			return false;
		} else {
			playerAimX[i] = k;
			playerAimY[i] = j;
			return true;
		}
	}

	private void SetAvoidanceAimUpAndDown(int i, int j, int k, byte byte0) {
		int l = j + k;
		if (TileBlockData[MapData[l][i]] > 1) {
			playerAimX[byte0] = i << 4;
			playerAimY[byte0] = l << 4;
		} else {
			boolean aflag[] = { true, true };
			boolean flag = true;
			for (int i1 = 1; flag; i1++) {
				if (aflag[0])
					if (i < i1 || TileBlockData[MapData[j][i - i1]] <= 1) {
						aflag[0] = false;
						if (!aflag[1])
							flag = false;
					} else if (TileBlockData[MapData[l][i - i1]] > 1) {
						playerAimX[byte0] = i - i1 << 4;
						playerAimY[byte0] = l << 4;
						flag = false;
					}
				if (aflag[1])
					if (i >= 11 - i1 || TileBlockData[MapData[j][i + i1]] <= 1) {
						aflag[1] = false;
						if (!aflag[0])
							flag = false;
					} else if (TileBlockData[MapData[l][i + i1]] > 1) {
						playerAimX[byte0] = i + i1 << 4;
						playerAimY[byte0] = l << 4;
						flag = false;
					}
			}

		}
	}

	private void SetAvoidanceAimLeftAndRight(int i, int j, int k, byte byte0) {
		int l = i + k;
		if (TileBlockData[MapData[j][l]] > 1) {
			playerAimX[byte0] = l << 4;
			playerAimY[byte0] = j << 4;
		} else {
			boolean aflag[] = { true, true };
			boolean flag = true;
			for (int i1 = 1; flag; i1++) {
				if (aflag[0])
					if (j < i1 || TileBlockData[MapData[j - i1][i]] <= 1) {
						aflag[0] = false;
						if (!aflag[1])
							flag = false;
					} else if (TileBlockData[MapData[j - i1][l]] > 1) {
						playerAimX[byte0] = l << 4;
						playerAimY[byte0] = j - i1 << 4;
						flag = false;
					}
				if (aflag[1])
					if (j >= 128 - i1 || TileBlockData[MapData[j + i1][i]] <= 1) {
						aflag[1] = false;
						if (!aflag[0])
							flag = false;
					} else if (TileBlockData[MapData[j + i1][l]] > 1) {
						playerAimX[byte0] = l << 4;
						playerAimY[byte0] = j + i1 << 4;
						flag = false;
					}
			}

		}
	}

	private void MoveScreenUp(int i) {
		if (screenImageTop > 0) {
			if (i > 3)
				i = 3;
			screenImageTop -= i;
			if (screenImageTop < 0)
				screenImageTop = 0;
			if (screenImageDiscontinuity == 0) {
				screenImageDiscontinuity += screenImageHeight;
				screenSplit -= screenImageHeight;
			}
			if (screenImageHeight - screenImageDiscontinuity < screenSplit
					- screenImageTop) {
				screenImageDiscontinuity -= 16;
				int j = screenImageTop >> 4;
				for (int k = 10; k >= 0; k--) {
					screenImageGraphics.setClip(k << 4,
							screenImageDiscontinuity, 16, 16);
					screenImageGraphics.drawImage(tilesImage, k
							- TileTextureData[MapData[j][k]] << 4,
							screenImageDiscontinuity, 20);
				}

			}
		}
	}

	private void MoveScreenDown(int i) {
		if (screenImageTop < 2048 - height) {
			if (i > 3)
				i = 3;
			screenImageTop += i;
			if (screenImageTop > 2048 - height)
				screenImageTop = 2048 - height;
			if (screenImageDiscontinuity == screenImageHeight) {
				screenImageDiscontinuity -= screenImageHeight;
				screenSplit += screenImageHeight;
			}
			if (screenImageDiscontinuity < (screenImageTop + height)
					- screenSplit) {
				screenImageDiscontinuity += 16;
				int j = screenImageTop + height >> 4;
				for (int k = 10; k >= 0; k--) {
					screenImageGraphics.setClip(k << 4,
							screenImageDiscontinuity - 16, 16, 16);
					screenImageGraphics.drawImage(tilesImage, k
							- TileTextureData[MapData[j][k]] << 4,
							screenImageDiscontinuity - 16, 20);
				}

			}
		}
	}

	private void MoveScreenLeft(int i) {
		if (screenImageLeft > 0) {
			if (i > 3)
				i = 3;
			screenImageLeft -= i;
			if (screenImageLeft < 0)
				screenImageLeft = 0;
		}
	}

	private void MoveScreenRight(int i) {
		if (screenImageLeft < 176 - width) {
			if (i > 3)
				i = 3;
			screenImageLeft += i;
			if (screenImageLeft > 176 - width)
				screenImageLeft = 176 - width;
		}
	}

	private int MovePlayerUp(byte byte0, int i) {
		if (playerY[byte0] >= i) {
			if ((playerY[byte0] & 0xf) < i) {
				boolean flag = false;
				boolean flag1 = false;
				for (int i1 = 0; i1 <= 15; i1 += 15) {
					int j = MapData[(playerY[byte0] >> 4) - 1][playerX[byte0]
							+ i1 >> 4];
					if (TileBlockData[j] == 1) {
						if (byte0 < 3 && playerType[byte0] == 3) {
							if (flag)
								Infiltrate(byte0, true);
							else
								flag = true;
						} else {
							return playerY[byte0] & 0xf;
						}
					} else {
						if (TileBlockData[j] == 0)
							return playerY[byte0] & 0xf;
						if (TileBlockData[j] == 5) {
							if (byte0 < 3 && playerType[byte0] == 3)
								if (flag1)
									Infiltrate(byte0, false);
								else
									flag1 = true;
						} else if (byte0 == currentPlayer) {
							j = TileSpecialData[j];
							if (j >= 0)
								PickUpItem(byte0, (byte) j);
						}
					}
				}

			}
			for (byte byte1 = playerArrayStart; byte1 >= 0 && i > 0; byte1 = playerArrayNext[byte1])
				if (byte1 != byte0 && playerHealth[byte1] >= 0) {
					int k = playerX[byte0] - playerX[byte1];
					if (k > -16 && k < 16) {
						int l = playerY[byte0] - i - playerY[byte1];
						if (l > -16 && l < 16) {
							i = playerY[byte0] - playerY[byte1] - 16;
							CheckCollision(byte0, byte1);
						}
					}
				}

			return i;
		} else {
			return playerY[byte0];
		}
	}

	private int MovePlayerDown(byte byte0, int i) {
		if (playerY[byte0] + i <= 2032) {
			if ((1 - playerY[byte0] & 0xf) <= i) {
				boolean flag = false;
				boolean flag1 = false;
				for (int i1 = 0; i1 <= 15; i1 += 15) {
					int j = MapData[playerY[byte0] + i + 15 >> 4][playerX[byte0]
							+ i1 >> 4];
					if (TileBlockData[j] == 1) {
						if (byte0 < 3 && playerType[byte0] == 3) {
							if (flag)
								Infiltrate(byte0, true);
							else
								flag = true;
						} else {
							return -playerY[byte0] & 0xf;
						}
					} else {
						if (TileBlockData[j] == 0)
							return -playerY[byte0] & 0xf;
						if (TileBlockData[j] == 5) {
							if (byte0 < 3 && playerType[byte0] == 3)
								if (flag1)
									Infiltrate(byte0, false);
								else
									flag1 = true;
						} else if (byte0 == currentPlayer) {
							j = TileSpecialData[j];
							if (j >= 0)
								PickUpItem(byte0, (byte) j);
						}
					}
				}

			}
			for (byte byte1 = playerArrayStart; byte1 >= 0 && i > 0; byte1 = playerArrayNext[byte1])
				if (byte1 != byte0 && playerHealth[byte1] >= 0) {
					int k = playerX[byte0] - playerX[byte1];
					if (k > -16 && k < 16) {
						int l = playerY[byte1] - playerY[byte0] - i;
						if (l > -16 && l < 16) {
							i = playerY[byte1] - playerY[byte0] - 16;
							CheckCollision(byte0, byte1);
						}
					}
				}

			return i;
		} else {
			return 2032 - playerY[byte0];
		}
	}

	private int MovePlayerLeft(byte byte0, int i) {
		if (playerX[byte0] >= i) {
			if ((playerX[byte0] & 0xf) < i) {
				boolean flag = false;
				boolean flag1 = false;
				for (int i1 = 0; i1 <= 15; i1 += 15) {
					int j = MapData[playerY[byte0] + i1 >> 4][(playerX[byte0] >> 4) - 1];
					if (TileBlockData[j] == 1) {
						if (byte0 < 3 && playerType[byte0] == 3) {
							if (flag)
								Infiltrate(byte0, true);
							else
								flag = true;
						} else {
							return playerX[byte0] & 0xf;
						}
					} else {
						if (TileBlockData[j] == 0)
							return playerX[byte0] & 0xf;
						if (TileBlockData[j] == 5) {
							if (byte0 < 3 && playerType[byte0] == 3)
								if (flag1)
									Infiltrate(byte0, false);
								else
									flag1 = true;
						} else if (byte0 == currentPlayer) {
							j = TileSpecialData[j];
							if (j >= 0)
								PickUpItem(byte0, (byte) j);
						}
					}
				}

			}
			for (byte byte1 = playerArrayStart; byte1 >= 0 && i > 0; byte1 = playerArrayNext[byte1])
				if (byte1 != byte0 && playerHealth[byte1] >= 0) {
					int k = playerY[byte0] - playerY[byte1];
					if (k > -16 && k < 16) {
						int l = playerX[byte0] - i - playerX[byte1];
						if (l > -16 && l < 16) {
							i = playerX[byte0] - playerX[byte1] - 16;
							CheckCollision(byte0, byte1);
						}
					}
				}

			return i;
		} else {
			return playerX[byte0];
		}
	}

	private int MovePlayerRight(byte byte0, int i) {
		if (playerX[byte0] + i <= 160) {
			if ((1 - playerX[byte0] & 0xf) <= i) {
				boolean flag = false;
				boolean flag1 = false;
				for (int i1 = 0; i1 <= 15; i1 += 15) {
					int j = MapData[playerY[byte0] + i1 >> 4][playerX[byte0]
							+ i + 15 >> 4];
					if (TileBlockData[j] == 1) {
						if (byte0 < 3 && playerType[byte0] == 3) {
							if (flag)
								Infiltrate(byte0, true);
							else
								flag = true;
						} else {
							return -playerX[byte0] & 0xf;
						}
					} else {
						if (TileBlockData[j] == 0)
							return -playerX[byte0] & 0xf;
						if (TileBlockData[j] == 5) {
							if (byte0 < 3 && playerType[byte0] == 3)
								if (flag1)
									Infiltrate(byte0, false);
								else
									flag1 = true;
						} else if (byte0 == currentPlayer) {
							j = TileSpecialData[j];
							if (j >= 0)
								PickUpItem(byte0, (byte) j);
						}
					}
				}

			}
			for (byte byte1 = playerArrayStart; byte1 >= 0 && i > 0; byte1 = playerArrayNext[byte1])
				if (byte1 != byte0 && playerHealth[byte1] >= 0) {
					int k = playerY[byte0] - playerY[byte1];
					if (k > -16 && k < 16) {
						int l = playerX[byte1] - playerX[byte0] - i;
						if (l > -16 && l < 16) {
							i = playerX[byte1] - playerX[byte0] - 16;
							CheckCollision(byte0, byte1);
						}
					}
				}

			return i;
		} else {
			return 160 - playerX[byte0];
		}
	}

	private void CheckCollision(byte byte0, byte byte1) {
		if (byte0 == currentPlayer && playerType[byte0] == 4 && byte1 < 3
				&& playerHealth[byte0] > 0
				&& clockTicks > playerNextShot[byte0]) {
			playerHealth[byte1] += playerHealth[byte0];
			playerNextShot[byte0] = clockTicks + 60;
			if (playerHealth[byte1] > INITIAL_HEALTH[playerType[byte1]])
				playerHealth[byte1] = INITIAL_HEALTH[playerType[byte1]];
			playerShowStatus[byte1] = clockTicks + 60;
		}
	}

	private void PickUpItem(byte byte0, byte byte1) {
		switch (specialType[byte1]) {
		default:
			break;

		case 4: // '\004'
			playerProtected[byte0] = clockTicks + 180;
			break;

		case 5: // '\005'
			if (playerHealth[byte0] < INITIAL_HEALTH[playerType[byte0]]) {
				playerShowStatus[byte0] = clockTicks + 60;
				playerHealth[byte0]++;
				UpdateStatusBar();
			} else {
				return;
			}
			break;

		case 6: // '\006'
			if (playerType[byte0] != 4
					&& playerType[byte0] != 3
					&& playerSpecial[byte0] < INITIAL_AMMO[WEAPON_TYPE[playerType[byte0]]]) {
				playerSpecial[byte0] = INITIAL_AMMO[WEAPON_TYPE[playerType[byte0]]];
				UpdateStatusBar();
			} else {
				return;
			}
			break;

		case 7: // '\007'
			endLevel = true;
			return;
		}
		TileSpecialData[MapData[specialY[byte1] >> 4][specialX[byte1] >> 4]] = -1;
		byte byte2 = specialArrayPrevious[byte1];
		if (byte2 < 0)
			specialArrayStart = specialArrayNext[byte1];
		else
			specialArrayNext[byte2] = specialArrayNext[byte1];
		byte2 = specialArrayNext[byte1];
		if (byte2 >= 0)
			specialArrayPrevious[byte2] = specialArrayPrevious[byte1];
	}

	private void Infiltrate(byte byte0, boolean flag) {
		if (playerInBuilding[byte0]) {
			playerInBuilding[byte0] = false;
			if (clockTicks < playerSpecial[byte0])
				SetInvisibility(byte0, false);
		} else {
			playerInBuilding[byte0] = true;
			if (flag)
				SetInvisibility(byte0, true);
		}
	}

	private void DoExplosion(byte byte0, int i) {
		PlaySound(explosionSound);
		int j = projectileX[byte0];
		int k = projectileY[byte0];
		CheckPlayerDamage(j + 16, j - 32, k + 16, k - 32, (byte) 2);
		for (byte byte1 = specialArrayExplodableStart; byte1 >= 0; byte1 = specialArrayNext[byte1]) {
			byte byte2 = specialType[byte1];
			if (byte2 >= 0 && k + 24 >= specialY[byte1]
					&& k - 24 <= specialY[byte1] + SPECIAL_GFX_H[byte2]
					&& j + 24 >= specialX[byte1]
					&& j - 24 <= specialX[byte1] + SPECIAL_GFX_W[byte2]) {
				specialDamage[byte1] -= i;
				if (specialDamage[byte1] <= 0) {
					int l = specialX[byte1] >> 4;
					int i1 = specialY[byte1] >> 4;
					for (int j1 = (SPECIAL_GFX_W[byte2] >> 4) - 1; j1 >= 0; j1--) {
						for (int k1 = (SPECIAL_GFX_H[byte2] >> 4) - 1; k1 >= 0; k1--) {
							byte byte3 = MapData[i1 + k1][l + j1];
							TileSpecialData[byte3] = -1;
							TileBlockData[byte3] = 4;
						}

					}

					specialType[byte1] = (byte) (-1 - byte2);
				}
			}
		}

		projectileType[byte0] = 10;
		projectileTime[byte0] = 0;
	}

	private boolean CheckPlayerDamage(int i, int j, int k, int l, byte byte0) {
		boolean flag = false;
		for (byte byte1 = playerArrayStart; byte1 >= 0;) {
			if (playerHealth[byte1] >= 0 && playerX[byte1] <= i
					&& playerX[byte1] > j && playerY[byte1] <= k
					&& playerY[byte1] > l) {
				DoPlayerDamage(byte1, byte0);
				flag = true;
			}
			byte1 = playerArrayNext[byte1];
			if (byte1 == opponentArrayStart) {
				i += 2;
				j -= 2;
				k += 2;
				l -= 2;
			}
		}

		return flag;
	}

	private void DoPlayerDamage(byte byte0, byte byte1) {
		if (byte0 >= 3 || clockTicks > playerProtected[byte0]) {
			playerHealth[byte0] -= byte1;
			if (playerHealth[byte0] < -1)
				playerHealth[byte0] = -1;
			if (byte0 < 3)
				playerShowStatus[byte0] = clockTicks + 60;
			if (byte0 == currentPlayer) {
				if (playerHealth[byte0] < 0) {
					currentPlayer = NextUnit(byte0);
					playerDirection[currentPlayer] = 1;
				}
				UpdateStatusBar();
			}
		}
	}

	private void SetMenuScreenType(short word0) {
		System.gc();
		menuGraphicHeight = (short) (4 + largeFontHeight + 3);
		menuLineHeight = fontHeight;
		int i = largeFontHeight + 4;
		if (word0 >= 100)
			SetMenuScreenParseText("h" + (word0 - 100), "");
		else if (word0 == 7 || word0 == 8) {
			if (missionTime[missionNumberCurrent] > 0)
				SetMenuScreenParseText("b" + missionNumberCurrent,
						"~ ~Best time: " + missionTime[missionNumberCurrent]
								/ 10 + "." + missionTime[missionNumberCurrent]
								% 10);
			else
				SetMenuScreenParseText("b" + missionNumberCurrent, "");
		} else if (word0 == 11) {
			menuStringArray = null;
			System.gc();
			int j = (int) ((System.currentTimeMillis() - startTime) / 100L);
			if (missionTime[missionNumberCurrent] == 0
					|| j < missionTime[missionNumberCurrent]) {
				missionTime[missionNumberCurrent] = j;
				writeRecords();
				SetMenuScreenParseText("d" + missionNumberCurrent,
						"~ ~New best time: " + j / 10 + "." + j % 10);
			} else {
				SetMenuScreenParseText("d" + missionNumberCurrent, "~ ~Time: "
						+ j / 10 + "." + j % 10);
			}
		} else if (word0 == 16) {
			menuGraphicHeight = (short) (4 + titleImage.getHeight() + 3);
			SetMenuScreenParseText("end", "");
		} else {
			menuLineHeight = largeFontHeight;
			if (word0 == 3) {
				menuStringArray = Strings.helpTitles;
				clockTicks = 0;
			} else if (word0 == 1) {
				menuStringArray = (new String[] { "Play", "Options", "Help",
						"Exit" });
				menuGraphicHeight = (short) (4 + titleImage.getHeight() + 3);
			} else if (word0 == 6) {
				menuStringArray = new String[missionNumberMax + 1];
				for (int k = 0; k <= missionNumberMax; k++)
					menuStringArray[k] = Strings.missionTitle[missionNumberMax
							- k];

				menuGraphicHeight = (short) (4 + titleImage.getHeight() + 3);
			} else if (word0 == 4)
				menuStringArray = (new String[] {
						soundOn ? "Sound: ON" : "Sound: OFF", "Reset Game" });
			else if (word0 == 5)
				menuStringArray = (new String[] {
						soundOn ? "Sound: ON" : "Sound: OFF",
						"Restart Mission", "Main Menu" });
			menuSelection = 1;
		}
		menuLength = (short) menuStringArray.length;
		screenType = word0;
		menuStart = 1;
		menuLines = (short) Math.min((height - menuGraphicHeight - i)
				/ menuLineHeight, menuLength);
		int l = height - menuGraphicHeight - i - menuLines * menuLineHeight >> 1;
		if (l > 0)
			menuGraphicHeight += l;
		System.gc();
	}

	private void SetMenuScreenParseText(String s, String s1) {
		System.gc();
		Object obj = null;
		Object obj1 = null;
		int j = 0;
		byte abyte0[] = new byte[860];
		String s2 = "";
		try {
			InputStream inputstream = getClass()
					.getResourceAsStream(s + ".txt");
			DataInputStream datainputstream = new DataInputStream(inputstream);
			for (int i = datainputstream.read(); i != -1 && i != 10 && i != 13; i = datainputstream
					.read()) {
				abyte0[j] = (byte) i;
				j++;
			}

			s2 = new String(abyte0, 0, j);
			datainputstream.close();
			inputstream.close();
		} catch (Exception exception) {
			System.out.println("Exception: " + exception.toString());
		}
		s2 = s2 + s1;
		Vector vector = ParseString(s2);
		menuStringArray = null;
		System.gc();
		menuStringArray = new String[vector.size() - 1];
		for (int k = vector.size() - 2; k >= 0; k--)
			menuStringArray[k] = s2.substring(((Integer) vector.elementAt(k))
					.intValue(),
					((Integer) vector.elementAt(k + 1)).intValue() - 1);

		menuSelection = 0;
	}

	private Vector ParseString(String s) {
		Vector vector = new Vector();
		int i = 0;
		boolean flag = false;
		int k = s.length();
		int l = width - 14 - 8;
		while (i < k) {
			vector.addElement(new Integer(i));
			int j = i;
			int i1;
			for (i1 = 0; i1 < l && i < k && s.charAt(i) != '~'; i++)
				i1 += font.charWidth(s.charAt(i));

			if (i1 >= l) {
				int j1;
				for (j1 = --i; j1 > j && s.charAt(j1) != ' '; j1--)
					;
				if (j1 > j)
					i = j1 + 1;
			} else {
				i++;
			}
		}
		vector.addElement(new Integer(i));
		return vector;
	}

	private COD cod;
	private static final short LEFT_SOFTKEY = -6;
	private static final short RIGHT_SOFTKEY = -7;
	private static final byte pagemargin = 4;
	private static final int ROWMAX = 128;
	private static final int COLMAX = 11;
	private static final int MAP_HEIGHT = 2048;
	private static final int MAP_WIDTH = 176;
	private static final int SCREEN_MARGIN = 22;
	private static final byte SCREEN_SPEED_MAX = 3;
	private static final byte PLAYER_MAX = 3;
	private static final byte OPPONENT_MAX = 42;
	private static final byte SPECIAL_MAX = 40;
	private static final byte SQUARE_MAX = 30;
	private static final byte PROJECTILE_MAX = 20;
	private static final byte PROJECTILE_TIME_MAX_BULLET = 10;
	private static final byte PROJECTILE_TIME_MAX_GRENADE = 8;
	private static final byte PROJECTILE_TIME_MAX_EXPLOSIVES = 38;
	private static final byte PROJECTILE_TIME_MAX_RIFLE = 2;
	private static final byte EXPLOSION_FRAMES = 4;
	private static final byte DEATH_FRAMES = 30;
	private static final byte SHOT_DELAY_AXIS = 20;
	private static final byte SHOT_DELAY_AI = 10;
	private static final byte SHOT_DELAY_GRENADE = 10;
	private static final byte SHOT_DELAY_RIFLE = 10;
	private static final byte SHOT_DELAY_EXPLOSIVES = 10;
	private static final byte HEALING_DELAY = 60;
	private static final int FLAK_JACKET_TIME = 180;
	private static final byte INITIAL_AMMO[] = { 10, 5, 5 };
	private static final byte INITIAL_HEALTH[] = { 2, 1, 3, 2, 3 };
	private static final byte SHOW_STATUS_TIME = 60;
	private static final byte STATUS_BAR_WIDTH = 66;
	private static final byte STATUS_BAR_HEIGHT = 14;
	private static final byte SHOW_ORDERS_TIME = 50;
	private static final int INVISIBILITY_TIME = 180;
	private static final short screen_splash = 0;
	private static final short screen_menu = 1;
	private static final short screen_play = 2;
	private static final short screen_rules_menu = 3;
	private static final short screen_options = 4;
	private static final short screen_options_ingame = 5;
	private static final short screen_mission = 6;
	private static final short screen_briefing = 7;
	private static final short screen_briefing_no_back = 8;
	private static final short screen_squad_select = 9;
	private static final short screen_squad_confirm = 10;
	private static final short screen_end_level = 11;
	private static final short screen_player_dead = 12;
	private static final short screen_player_dead_extra = 13;
	private static final short screen_options_ingame_extra = 14;
	private static final short screen_reset_confirm = 15;
	private static final short screen_exit = 16;
	private static final short timer_loop = 0;
	private static final short timer_splash = 1;
	private static final short timer_menu_anim = 2;
	private static final short timer_suspend_key_presses = 3;
	private static final short timer_player_dead = 4;
	private static final int DELAY_LOOP = 50;
	private static final int DELAY_SPLASH_ACTIVISION = 2500;
	private static final int DELAY_SPLASH_NGAME = 2000;
	private static final int DELAY_SPLASH_TITLE = 3000;
	private static final int DELAY_ANIM = 100;
	private static final int DELAY_SUSPEND = 750;
	private int height;
	private int width;
	private int halfHeight;
	private int halfWidth;
	private short fontHeight;
	private short largeFontHeight;
	private Font font;
	private Font largeFont;
	private Timer timer;
	private SimpleTimerTask simpletimertask;
	private short screenType;
	private byte missionNumberCurrent;
	private byte missionNumberMax;
	private static final byte MISSION_COUNT = 8;
	private boolean endLevel;
	private Image tilesImage;
	private Image unitImage;
	private Image screenImage;
	private Image specialImage;
	private Image symbolsImage;
	private Image statusBar;
	private Image titleImage;
	private Image arrowsImage;
	private short menuStart;
	private short menuLines;
	private short menuGraphicHeight;
	private short menuLineHeight;
	private short menuLength;
	private short menuSelection;
	private String menuStringArray[];
	private Graphics screenImageGraphics;
	private int screenImageLeft;
	private int screenImageTop;
	private int screenImageHeight;
	private int screenSplit;
	private int screenImageDiscontinuity;
	private byte projectileArrayNext[];
	private byte projectileArrayUsedStart;
	private byte projectileArrayUnusedStart;
	private int projectileX[];
	private int projectileY[];
	private byte projectileDirection[];
	private byte projectileTime[];
	private byte projectileType[];
	private byte opponentArrayStart;
	private byte playerArrayNext[];
	private byte playerArrayStart;
	private byte currentPlayer;
	private boolean takeCover;
	private int playerX[];
	private int playerY[];
	private byte playerDirection[];
	private boolean playerMoving[];
	private byte playerHealth[];
	private int playerNextShot[];
	private int playerAimX[];
	private int playerAimY[];
	private byte playerType[];
	private int playerSpecial[];
	private int playerProtected[];
	private boolean playerInBuilding[];
	private int playerShowStatus[];
	private byte specialArrayNext[];
	private byte specialArrayPrevious[];
	private byte specialArrayStart;
	private byte specialArrayExplodableStart;
	private int specialX[];
	private int specialY[];
	private byte specialType[];
	private byte specialDamage[];
	private byte squarePlayer[];
	private byte squareRow[];
	private byte squareCol[];
	private byte squareMax;
	private int currentKeyCode;
	private int nextKeyCode;
	private byte frameCount;
	private int clockTicks;
	private int showOrders;
	private boolean soundOn;
	private boolean allowKeyPresses;
	private Random random;
	private Sound introSound;
	private Sound fireSound;
	private Sound pickupSound;
	private Sound secondarySound;
	private Sound explosionSound;
	private Sound unitSound;
	private Sound winSound;
	private Sound loseSound;
	private int missionTime[];
	private long startTime;
	private byte MapData[][];
	private byte TileTextureData[];
	private byte TileBlockData[];
	private byte TileSpecialData[];
	private static final byte PLAYER_INFANTRY = 0;
	private static final byte PLAYER_SNIPER = 1;
	private static final byte PLAYER_ENGINEER = 2;
	private static final byte PLAYER_COMMANDO = 3;
	private static final byte PLAYER_MEDIC = 4;
	private static final byte SPECIAL_TANK = 0;
	private static final byte SPECIAL_JEEP = 1;
	private static final byte SPECIAL_V2 = 2;
	private static final byte SPECIAL_DOOR = 3;
	private static final byte SPECIAL_FLAK_JACKET = 4;
	private static final byte SPECIAL_MEDIPACK = 5;
	private static final byte SPECIAL_AMMUNITION = 6;
	private static final byte SPECIAL_END = 7;
	private static final byte SPECIAL_BACK_DOOR = 8;
	private static final int SPECIAL_GFX_X[] = { 0, 80, 112, 112, 128, 128, 32 };
	private static final byte SPECIAL_GFX_Y[] = { 0, 0, 0, 32, 0, 16, 32 };
	private static final byte SPECIAL_GFX_W[] = { 64, 32, 16, 32, 16, 16, 16,
			16, 32 };
	private static final byte SPECIAL_GFX_H[] = { 32, 32, 32, 16, 16, 16, 16,
			16, 16 };
	private static final byte CRATER_GFX_X[] = { 0, 64, 96, 96 };
	private static final byte CRATER_GFX_Y[] = { 0, 0, 16, 0 };
	private static final byte CRATER_GFX_W[] = { 64, 32, 16, 32 };
	private static final byte CRATER_GFX_H[] = { 32, 32, 32, 16 };
	private static final byte CRATER_DRAW_Y[] = { 0, 0, 16, 0 };
	private static final byte EXPLOSIVES_GFX_X = 48;
	private static final byte EXPLOSIVES_GFX_Y = 32;
	private static final byte FLAG_GFX_X = 0;
	private static final byte FLAG_GFX_Y = 32;
	private static final byte SECONDARY_GFX_X = 64;
	private static final byte FLAK_JACKET_GFX_X = 82;
	private static final byte DAGGER_GFX_X = 72;
	private static final byte DAGGER_GFX_Y = 6;
	private static final byte GUNFLARE_GFX_X = 74;
	private static final byte GUNFLARE_GFX_Y = 15;
	private static final byte PROJECTILE_BULLET = 99;
	private static final byte PROJECTILE_GRENADE = 0;
	private static final byte PROJECTILE_RIFLE = 1;
	private static final byte PROJECTILE_EXPLOSIVES = 2;
	private static final byte PROJECTILE_EXPLOSION = 10;
	private static final byte DAMAGE_BULLET = 1;
	private static final byte DAMAGE_RIFLE = 2;
	private static final byte DAMAGE_EXPLOSION = 2;
	private static final byte TILE_BLOCKED_IMPASSABLE = 0;
	private static final byte TILE_BLOCKED_DOOR = 1;
	private static final byte TILE_BLOCKED_WALK_UNDER = 2;
	private static final byte TILE_BLOCKED_WATER = 3;
	private static final byte TILE_BLOCKED_CLEAR = 4;
	private static final byte TILE_BLOCKED_BACK_DOOR = 5;
	private static final byte WEAPON_TYPE[] = { 0, 1, 2, 0, -1 };
	private static final byte bulletDeltaX[] = { -4, 0, 4, -6, 0, 6, -4, 0, 4 };
	private static final byte bulletDeltaY[] = { -4, -6, -4, 0, 0, 0, 4, 6, 4 };
	private static final byte bulletClipDeltaX[] = { 11, 1, 0, 13, 0, 0, 11, 1,
			0 };
	private static final byte bulletClipDeltaY[] = { 11, 13, 11, 1, 0, 1, 0, 0,
			0 };
	private static final byte bulletImageDeltaX[] = { 101, 86, 98, 93, 0, 80,
			109, 86, 90 };
	private static final byte bulletImageDeltaY[] = { 48, 45, 48, 38, 0, 38,
			37, 32, 37 };
	private static final byte bulletClipWidth[] = { 11, 3, 11, 13, 0, 13, 11,
			3, 11 };
	private static final byte bulletClipHeight[] = { 11, 13, 11, 3, 0, 3, 11,
			13, 11 };
	private static final byte bulletEndDeltaX[] = { -11, 0, 11, -13, 0, 13,
			-11, 0, 11 };
	private static final byte bulletEndDeltaY[] = { -11, -13, -11, 0, 0, 0, 11,
			13, 11 };
	private static final byte gunFlareX[] = { 0, 8, 15, 0, 0, 15, 0, 8, 15 };
	private static final byte gunFlareY[] = { 5, 0, 5, 9, 0, 9, 10, 11, 10 };

}
