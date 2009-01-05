package luke.games.engines.gLib.test;

import javax.microedition.lcdui.*;
import luke.games.engines.gLib.GGraphics;

final public class ScreenLogo implements SubScreen {

	static int LogoState = 0;
	static int FontSize = Font.SIZE_SMALL;

	public ScreenLogo() {
		GameMIDlet.TIMER_PRE_FRAME = 33;
		ScreenController.KeyEnable = true;
		ScreenController.LogicEnable = true;
		ScreenController.RenderEnable = true;
		ScreenController.Timer = 0;
		GGraphics.setFont(FontSize);
	}

	public void Logic() {
		if (ScreenController.IsKeyDown(GAME_KEY_1)) {
			ScreenController.ChangeSubSreen(SCREEN_TYPE_MAIN, "关卡演示");
		}
		if (ScreenController.IsKeyDown(GAME_KEY_2)) {
			ScreenController.ChangeSubSreen(SCREEN_TYPE_MAIN2, "程序演示");
		}
		if (ScreenController.IsKeyDown(GAME_KEY_B)) {
			GameMIDlet.Exit = true;
		}
	}

	public void Render(Graphics g) {
		// g.setColor(255,255,255);
		// g.fillRect(0,0,200,200);
		GGraphics.drawString(ScreenController.ScreenGraphics, "按 1 进入关卡演示", 8, 8,
				0xff00ff00, 0xff000000, GGraphics.TEXT_STYLE_AROUND);
		GGraphics.drawString(ScreenController.ScreenGraphics, "按 2 进入程序演示", 8, 20,
				0xff00ff00, 0xff000000, GGraphics.TEXT_STYLE_AROUND);
	}
}