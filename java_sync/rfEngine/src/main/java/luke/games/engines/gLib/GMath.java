package luke.games.engines.gLib;

import luke.games.engines.gLib.GMath;

/**
 * 附加的数学方法
 */
public class GMath {
	/**
	 * 两非负数相乘<br>
	 * add by luke, to be optimized. 07.12.19
	 * 
	 * @param a
	 * @param d
	 */
	int mul(int a, int d) {
		int r = 0;
		int p = 0;
		while (a > 0) {
			if ((a & 1) != 0) {// a 为奇数时
				r += d << p;
			}
			p++;
			a = a >> 1;
		}
		return r;
	}

	/**
	 * 求平方根
	 * 
	 * @param i
	 *            定义域
	 * @return 值域
	 */
	final public static int sqrt(int i) {
		int l = 0;
		for (int k = 0x100000; k != 0; k >>= 2) {
			int j = l + k;
			l >>= 1;
			if (j <= i) {
				i -= j;
				l += k;
			}
		}
		return l;
	}

	/** 圆周率的256倍定点数。 */
	final static public int PI256 = 804;

	/**
	 * sine*256 angle table 0~90
	 */
	private static final short[] SINES = { 0, 4, 8, 13, 17, 22, 26, 31, 35, 40,
			44, 48, 53, 57, 61, 66, 70, 74, 79, 83, 87, 91, 95, 100, 104, 108,
			112, 116, 120, 124, 127, 131, 135, 139, 143, 146, 150, 154, 157,
			161, 164, 167, 171, 174, 177, 181, 184, 187, 190, 193, 196, 198,
			201, 204, 207, 209, 212, 214, 217, 219, 221, 223, 226, 228, 230,
			232, 233, 235, 237, 238, 240, 242, 243, 244, 246, 247, 248, 249,
			250, 251, 252, 252, 253, 254, 254, 255, 255, 255, 255, 255, 256 };

	/**
	 * 正旋
	 * 
	 * @param angle
	 *            范围0~360
	 * @return 256倍数
	 */
	final public static int sinTimes256(int angle) {
		int d = angle < 0 ? -1 : 1;
		angle = Math.abs(angle % 360); // 360 degrees
		if (angle <= 90) {
			return SINES[angle] * d;
		} else if (angle <= 180) {
			return SINES[180 - angle] * d;
		} else if (angle <= 270) {
			return -SINES[angle - 180] * d;
		} else {
			return -SINES[360 - angle] * d;
		}
	}

	/**
	 * 余旋
	 * 
	 * @param angle
	 *            范围0~360
	 * @return 256倍数
	 */
	final public static int cosTimes256(int angle) {
		return sinTimes256(angle + 90); // i.e. add 90 degrees
	}

	/**
	 * 正切
	 * 
	 * @param angle
	 *            范围0~360
	 * @return 256倍数
	 */
	final public static int tangentTimes256(int angle) {
		return sinTimes256(angle) * 256 / cosTimes256(angle);
	}

	/**
	 * 余切
	 * 
	 * @param angle
	 *            范围0~360
	 * @return 256倍数
	 */
	final public static int cotangentTimes256(int angle) {
		return cosTimes256(angle) * 256 / sinTimes256(angle);
	}

	/**
	 * 对一个整型数组排序
	 * 
	 * @param list
	 *            要排序的数组
	 * @param above
	 *            if above<0 then min->max else max->min
	 */
	final public static void sequence(int[] list, int above) {
		int temp, max, min;
		boolean tag = true;
		for (int i = list.length - 1; i >= 0; i--) {
			for (int j = 0; j < i; j++) {
				if (above < 0) {
					if (list[j] < list[j + 1]) {
						temp = list[j];
						list[j] = list[j + 1];
						list[j + 1] = temp;
						tag = true;
					}
				} else {
					if (list[j] > list[j + 1]) {
						temp = list[j];
						list[j] = list[j + 1];
						list[j + 1] = temp;
						tag = true;
					}
				}
			}
			if (tag == false)
				break;
		}
	}

	/**
	 * 矢量相加
	 * 
	 * @param px
	 * @param py
	 * @param qx
	 * @param qy
	 * @return v[]
	 */
	final public static int[] vectorAdd(int px, int py, int qx, int qy) {
		return (new int[] { px + qx, py + qy });
	}

	/**
	 * 矢量相减
	 * 
	 * @param px
	 * @param py
	 * @param qx
	 * @param qy
	 * @return v[]
	 */
	final public static int[] vectorSub(int px, int py, int qx, int qy) {
		return (new int[] { px - qx, py - qy });
	}

	/**
	 * 矢量相乘<br>
	 * 在三角坐标系中: 若 P × Q > 0 , 则P在Q的顺时针方向。 若 P × Q < 0 , 则P在Q的逆时针方向。 若 P × Q = 0 ,
	 * 则P与Q共线，但可能同向也可能反向<br>
	 * 在屏幕坐标系中: 若 P × Q > 0 , 则P在Q的逆时针方向。 若 P × Q < 0 , 则P在Q的顺时针方向。 若 P × Q = 0 ,
	 * 则P与Q共线，但可能同向也可能反向
	 * 
	 * @param p
	 * @param q
	 * @return +-
	 */
	final public static int vectorMul(int[] p, int[] q) {
		return (p[0] * q[1] - p[1] * q[0]);
	}

	/**
	 * 判断2条直线段是否相撞<br>
	 * 屏幕坐标系: 判断P1P2跨立Q1Q2的依据是： ((Q2-Q1)×(P1-Q1))*((P2-Q1)×(Q2-Q1)) >= 0 <br>
	 * 同理判断Q1Q2跨立P1P2的依据是： ((P2-P1)×(Q1-P1))*((Q2-P1)×(P2-P1)) >= 0
	 * 
	 * @param p1x
	 *            line 1
	 * @param p1y
	 * @param p2x
	 * @param p2y
	 * @param q1x
	 *            line 2
	 * @param q1y
	 * @param q2x
	 * @param q2y
	 * @return false:true
	 */
	final static public boolean cdLine(int p1x, int p1y, int p2x, int p2y,
			int q1x, int q1y, int q2x, int q2y) {
		int p1[] = GMath.vectorSub(q2x, q2y, q1x, q1y);// 1
		// int p2[] = GameMath.vectorSub(p1x,p1y,q1x,q1y);//2
		// int p3[] = GameMath.vectorSub(p2x,p2y,q1x,q1y);//3
		// int p4[] = GameMath.vectorSub(q2x,q2y,q1x,q1y);//4=1
		int p5[] = GMath.vectorSub(p2x, p2y, p1x, p1y);// 5
		// int p6[] = GameMath.vectorSub(q1x,q1y,p1x,p1y);//6
		// int p7[] = GameMath.vectorSub(q2x,q2y,p1x,p1y);//7
		// int p8[] = GameMath.vectorSub(p2x,p2y,p1x,p1y);//8=5
		if (GMath.vectorMul(p1, GMath.vectorSub(p1x, p1y, q1x, q1y))
				* GMath.vectorMul(GMath.vectorSub(p2x, p2y, q1x, q1y), p1) >= 0
				&& GMath.vectorMul(p5, GMath.vectorSub(q1x, q1y, p1x, p1y))
						* GMath.vectorMul(GMath.vectorSub(q2x, q2y, p1x, p1y),
								p5) >= 0) {
			return true;
		}
		return false;
	}

	/**
	 * 判断2个矩形是否相撞<br>
	 * 原理: 两个矩形中心点在x方向的距离的绝对值小于等于矩形宽度和的二分之一，同时y方向的距离的绝对值小于等于矩形高度和的二分之一<br>
	 * 
	 * 
	 * @param x1
	 *            矩形1的左上角的坐标
	 * @param y1
	 * @param w1
	 * @param h1
	 * 
	 * @param x2
	 *            矩形2的左上角的坐标
	 * @param y2
	 * @param w2
	 * @param h2
	 * @return false:true 是否相撞
	 */
	final static public boolean cdRect(int x1, int y1, int w1, int h1, int x2,
			int y2, int w2, int h2) {
		// if( Math.abs(((sx<<8)+(sw<<7))-((dx<<8)+(dw<<7))) < (sw<<7)+(dw<<7)
		// &&
		// Math.abs(((sy<<8)+(sh<<7))-((dy<<8)+(dh<<7))) < (sh<<7)+(dh<<7)
		// ){
		// return true;
		// }
		if (Math.abs(((x1 << 1) + w1) - ((x2 << 1) + w2)) < (w1 + w2)
				&& Math.abs(((y1 << 1) + h1) - ((y2 << 1) + h2)) < (h1 + h2)) {
			return true;
		}
		return false;
	}

	/**
	 * 判断2个圆是否相撞
	 * 
	 * @param x1
	 * @param y1
	 * @param r1
	 * @param x2
	 * @param y2
	 * @param r2
	 * @return false:true
	 */
	final static public boolean cdRound(int x1, int y1, int r1, int x2, int y2,
			int r2) {
		int x, y, z;
		x = Math.abs(x1 - x2);
		y = Math.abs(y1 - y2);
		z = sqrt(x * x + y * y);
		if (z <= r1 + r2) {
			return true;
		}
		return false;
	}

	/**
	 * 判断点和线段是否相交
	 * 
	 * @param qx
	 * @param qy
	 * @param p1x
	 * @param p1y
	 * @param p2x
	 * @param p2y
	 * @return false:true
	 */
	final static public boolean cdPointLine(int qx, int qy, int p1x, int p1y,
			int p2x, int p2y) {
		// ：( Q - P1 ) × ( P2 - P1 ) = 0
		if (vectorMul(vectorSub(qx, qy, p1x, p1y),
				vectorSub(p2x, p2y, p1x, p1y)) == 0) {
			if (Math.min(p1x, p2x) <= qx && Math.max(p1x, p2x) >= qx
					&& Math.min(p1y, p2y) <= qy && Math.max(p1y, p2y) >= qy) {
				return true;
			}
		}
		return false;
	}

	/**
	 * 判断2个点是否相撞
	 * 
	 * @param x1
	 * @param y1
	 * @param x2
	 * @param y2
	 * @return false:true
	 */
	final static public boolean cdPoint(int x1, int y1, int x2, int y2) {
		if (x1 == x2 && y1 == y2)
			return true;
		return false;
	}
}
