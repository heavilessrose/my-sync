package luke.games.engines.pLib.math;

import luke.games.engines.gLib.GMath;

public strictfp class Vector2f implements Vector2f_interface {
	/** (X分量)x,y垂直 */
	public float x;
	/** (Y分量)x,y垂直 */
	public float y;

	/**
	 * 构造一空向量
	 */
	public Vector2f() {
	}

	/**
	 * @see pLib.math.Vector2f_interface#getX()
	 */
	public float getX() {
		return x;
	}

	/**
	 * @see pLib.math.Vector2f_interface#getY()
	 */
	public float getY() {
		return y;
	}

	/**
	 * 根据另一个向量构造向量
	 * 
	 * @param other
	 *            要拷贝到这个向量的另一个向量
	 */
	public Vector2f(Vector2f_interface other) {
		this(other.getX(), other.getY());
	}

	/**
	 * 构造一个新向量
	 * 
	 * @param x
	 *            x轴分量
	 * @param y
	 *            y轴分量
	 */
	public Vector2f(float x, float y) {
		this.x = x;
		this.y = y;
	}

	/**
	 * 设置这个向量的值
	 * 
	 * @param other
	 *            设置到这个向量的向量
	 */
	public void set(Vector2f_interface other) {
		set(other.getX(), other.getY());
	}

	/**
	 * 设置向量的值
	 * 
	 * @param x
	 *            x轴分量
	 * @param y
	 *            y轴分量
	 */
	public void set(float x, float y) {
		this.x = x;
		this.y = y;
	}

	/**
	 * 构造这个向量的反向量
	 * 
	 * @return 反向量
	 */
	public Vector2f negate() {
		return new Vector2f(-x, -y);
	}

	/**
	 * @see pLib.math.Vector2f_interface#dot
	 */
	public float dot(Vector2f_interface other) {
		return (x * other.getX()) + (y * other.getY());
	}

	/**
	 * 向量相加,只是分量相加
	 * 
	 * @param v
	 *            要相加的向量
	 */
	public void add(Vector2f_interface v) {
		x += v.getX();
		y += v.getY();
	}

	/**
	 * 向量相加(只是分量相加),返回新的向量,参与相加的两个向量不变
	 * 
	 * @param a
	 *            加数
	 * @param b
	 *            被加数
	 * @return 相加后所得向量
	 */
	public static Vector2f add(Vector2f a, Vector2f_interface b) {
		Vector2f newV = (Vector2f) a.cloneV();
		newV.add(b);
		return newV;
	}

	/**
	 * 向量相减(只是分量相减)
	 * 
	 * @param v
	 *            要相减的向量
	 */
	public void sub(Vector2f_interface v) {
		x -= v.getX();
		y -= v.getY();
	}

	/**
	 * 向量相减(只是分量相减),返回新的向量,参与相减的两个向量不变
	 * 
	 * @param a
	 *            减数
	 * @param b
	 *            被减数
	 * @return 相减后所得向量
	 */
	public static Vector2f sub(Vector2f a, Vector2f_interface b) {
		Vector2f newV = (Vector2f) a.cloneV();
		newV.sub(b);
		return newV;
	}

	/**
	 * 数乘/缩放
	 * 
	 * @param a
	 *            缩放比例
	 */
	public void scale(float rate) {
		x *= rate;
		y *= rate;
	}

	/**
	 * 向量数乘/缩放,返回新的向量,参与数乘/缩放的两个向量不变
	 * 
	 * @param a
	 *            缩放比例
	 * @return 数乘/缩放后所得向量
	 */
	public static Vector2f scale(Vector2f v, float scales) {
		Vector2f newV = (Vector2f) v.cloneV();
		newV.scale(scales);
		return newV;
	}

	/**
	 * 单位化(归一化))
	 * 
	 */
	public void normalise() {
		float l = length();

		if (l == 0)
			return;

		x /= l;
		y /= l;
	}

	/**
	 * 单位化(归一化))
	 * 
	 * @param v
	 *            要单位化的向量
	 * @return 返回单位向量,本向量不变
	 */
	public static Vector2f normalise(Vector2f v) {
		Vector2f newV = (Vector2f) v.cloneV();
		float l = newV.length();

		if (!(l == 0)) {
			newV.x /= l;
			newV.y /= l;
		}
		return newV;
	}

	/**
	 * 模的平方
	 * 
	 * @return 模的平方
	 */
	public float lengthSquared() {
		return (x * x) + (y * y);
	}

	/**
	 * @see pLib.math.Vector2f_interface#length()
	 */
	public float length() {
		return (float) GMath.sqrt((int) lengthSquared());
	}

	/**
	 * 两向量点之间距离
	 * 
	 * @param other
	 *            The other point we're measuring to
	 * @return The distance to the other point
	 */
	public float distance(Vector2f_interface other) {
		return (float) GMath.sqrt((int) distanceSquared(other));
	}

	/**
	 * Get the distance squared from this point to another(两点之间距离平方)
	 * 
	 * @param other
	 *            The other point we're measuring to
	 * @return The distance to the other point
	 */
	public float distanceSquared(Vector2f_interface other) {
		float dx = other.getX() - getX();
		float dy = other.getY() - getY();

		return (dx * dx) + (dy * dy);
	}

	/**
	 * 求此向量的正交向量
	 * <p>
	 * 原理:两向量正交的充要条件为 a.b==0
	 * 
	 * @param isShun
	 *            是否顺时针方向旋转所得的正交向量
	 */
	public Vector2f crossCut(Vector2f other, boolean isShun) {
		if (isShun)
			return new Vector2f(-y, x);
		else
			return new Vector2f(y, -x);
	}

	/**
	 * 本向量在other向量上的投影长
	 * 
	 * @param other
	 * @return 投影长
	 */
	public float getProjectLength(Vector2f other) {
		float len = 0;
		other.normalise();
		len = dot(other);
		return len;
	}

	/**
	 * 本向量在other向量上的投影向量
	 * 
	 * @param other
	 * @return 投影向量
	 */
	public Vector2f getProjectVector(Vector2f other) {
		other.normalise();
		return new Vector2f(getProjectLength(other) * other.x,
				getProjectLength(other) * other.y);
	}

	/**
	 * 投影 ???
	 * 
	 * @param b
	 *            要投影到的向量
	 * @param result
	 *            投影后所得向量
	 */
	public void projectOntoUnit(Vector2f_interface b, Vector2f result) {
		float dp = b.dot(this);

		result.x = dp * b.getX();
		result.y = dp * b.getY();
	}

	/**
	 * Compare two vectors allowing for a (small) error as indicated by the
	 * delta. Note that the delta is used for the vector's components
	 * separately, i.e. any other vector that is contained in the square box
	 * with sides 2*delta and this vector at the center is considered equal.
	 * (每个分量可以有delta的误差)
	 * 
	 * @param other
	 *            The other vector to compare this one to
	 * @param delta
	 *            The allowed error
	 * @return True if this vector is equal to other, with a tolerance defined
	 *         by delta
	 */
	public boolean equalsDelta(Vector2f_interface other, float delta) {
		return (other.getX() - delta < x && other.getX() + delta > x
				&& other.getY() - delta < y && other.getY() + delta > y);

	}

	/**
	 * 计算此向量与另一向量的夹角
	 * 
	 * @param other
	 *            另一向量
	 * @return 夹角
	 */
	// public float angleWith(Vector2f other) {
	// float angelWith;//夹角
	//
	// return angelWith;
	// }
	/** 克隆这个向量 */
	public Vector2f_interface cloneV() {
		Vector2f newV = new Vector2f();
		newV.x = x;
		newV.y = y;
		return newV;
	}

	/**
	 * @see java.lang.Object#toString()
	 */
	public String toString() {
		return "[Vec " + x + "," + y + " (" + length() + ")]";
	}
}
