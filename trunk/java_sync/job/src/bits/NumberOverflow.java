package bits;

/** 数值溢出 */
public class NumberOverflow {
	public static void main(String[] args) {

		// i设置为1000000，在乘方时Java发现结果（1000000000000）已经超出了int基本数据类型的最大范围（2147483647），
		// 于是作了默认的类型提升（type promotion），中间结果做为long类型存放，返回结果时目标数据类型int不能够容纳下结果，
		// 于是根据Java的基础类型的变窄转换（Narrowing
		// primitiveconversion）规则，把结果宽于int类型宽度的部分全部丢弃，也就是只取结果的低32位，于是就得到了上面的结果。

		// 下面用一个十六进制表示的例子阐释这个问题
		// int i3 = 1000000;
		// System.out.println (Long.toHexString (i3*i3).toUpperCase());
		// System.out.println (Long.toHexString (i3*i3).toUpperCase());
		// System.out.println (Integer.toHexString (i3*i3).toUpperCase());
		// System.out.println ((int)i3*i3);
		// ---------------------------------------------------
		// FFFFFFFFD4A51000
		// 1000000000000
		// D4A51000
		// -727379968
		// ---------------------------------------------------

		int i = 1000000;
		System.out.println(i * i);
	}
}
