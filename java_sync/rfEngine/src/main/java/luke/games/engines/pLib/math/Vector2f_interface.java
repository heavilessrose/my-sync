package luke.games.engines.pLib.math;

public interface Vector2f_interface {
	/** @return 向量的X轴分量 */
	public float getX();

	/** @return 向量的Y轴分量 */
	public float getY();

	/** @return 向量的模 */
	public float length();

	/** @return 模的平方 */
	public float lengthSquared();

	/**
	 * 两向量点之间的距离
	 * 
	 * @param other
	 *            另一向量
	 * @return 两向量之间的距离
	 */
	public float distance(Vector2f_interface other);

	/**
	 * 两向量点之间距离的平方
	 * 
	 * @param other
	 *            另一向量
	 * @return 两向量点之间距离的平方
	 */
	public float distanceSquared(Vector2f_interface other);

	/**
	 * @param other
	 *            要点乘的向量
	 * @return 向量的点积
	 */
	public float dot(Vector2f_interface other);

	/**
	 * 投影这个向量到另一个
	 * 
	 * @param b
	 *            投影到的向量
	 * @param result
	 *            投影后的向量
	 */
	public void projectOntoUnit(Vector2f_interface b, Vector2f result);

	public Vector2f_interface cloneV();

	public String toString();

}
