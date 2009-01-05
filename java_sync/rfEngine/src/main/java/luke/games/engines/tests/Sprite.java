package luke.games.engines.tests;

/** 通用精灵接口 */
public abstract class Sprite {
	/** 精灵能量(发射炮弹能量等级减一) */
	public int energy = 0;
	public int curEnergyLevel = 0;
	public static final int[] ENERGY_LEVEL = { 0, 1, 2, 3 };

	/** 精灵方位(坐标) */
	public int x = 0;
	public int y = 0;

	/** 得到方位坐标 */
	public int getX() {
		return this.x;
	}

	public int getY() {
		return this.y;
	}

	/** 设置方位坐标 */
	public void setX(int newX) {
		this.x = newX;
	}

	public void setY(int newY) {
		this.y = newY;
	}

	/** 精灵速度 */
	public int velocity = 0;

	/** 得到精灵速度 */
	public int getV() {
		return this.velocity;
	}

	/** 设置精灵速度 */
	public void setV(int newV) {
		this.velocity = newV;
	}

	/** 运动方向 */
	public static int curDirect = 0;
	public static final int DIR_LEFT = 0;
	public static final int DIR_RIGHT = 1;
	public static final int DIR_UP = 2;
	public static final int DIR_DOWN = 3;
	public static final int[] directions = { DIR_LEFT, DIR_RIGHT, DIR_UP,
			DIR_DOWN };

	/** 得到精灵方向 */
	public int getDir() {
		return this.curDirect;
	}

	/** 设置精灵方向 */
	public void setDir(int newDir) {
		this.curDirect = newDir;
	}
}
