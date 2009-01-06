import java.util.Vector;

import javax.microedition.lcdui.Canvas;
import javax.microedition.lcdui.Font;
import javax.microedition.lcdui.Graphics;

import utils.FontUtil;

public class MainCanvas extends Canvas {

	int x = 10;
	int y = 10;
	int width = 50;
	int height = 200;
	String str = "网络科技破what are you doing now? my dear xiaoZi 我ieh水利科技上帝发哦死立刻节省电费了科技时代佛iuqw老师夸奖对方.空间是垃圾哦                          456";
	String[] lines = null;
	Font font = Font.getDefaultFont();

	MainCanvas() {
		init();
	}

	protected void init() {
		String[] lines = FontUtil.splitByBox(font, str, box_w - space * 2, false);
		strLinesCount = lines.length;
		boxLinesCount = FontUtil.getVerticalCount(font, box_h, space);
		//		slide_h = (boxLinesCount / strLinesCount) * box_h;
	}

	protected void update() {

	}

	// string box 
	int box_x = 20;
	int box_y = 20;
	int start_y = box_y;
	int space = 2;
	int box_w = 100;
	int box_h = 80;
	int eachLineHeight = font.getHeight() + space;
	int boxLinesCount = 0; // 方格中能显示的行数
	int strLinesCount = 0; // 字符串分行后的行数

	// slide 
	//	int slide_start_y = box_y;
	//	int slide_w = 10;
	//	int slide_h = 0;
	//	int slide_step = 0;

	public void paint(Graphics g) {

		g.setColor(200, 200, 200);
		g.fillRect(0, 0, getWidth(), getHeight());
		g.setColor(0, 0, 0);

		FontUtil
				.drawStringInBox(g, font, str, box_x, box_y, start_y, box_w, box_h, space, Graphics.TOP | Graphics.LEFT);
	}

	public final void keyPressed(int keyCode) {
		System.out.println("keycode: " + keyCode);
		switch (keyCode) {
		case UP:
		case -1:
			if (start_y < box_y)
				start_y += eachLineHeight;
			repaint(box_x, box_y, box_w, box_h);
			break;
		case DOWN:
		case -2:
			if (start_y > box_y - eachLineHeight * (strLinesCount - boxLinesCount)
					&& box_h < (strLinesCount * eachLineHeight))
				start_y -= eachLineHeight;
			repaint(box_x, box_y, box_w, box_h);
			break;
		default:

			break;
		}
	}

}
