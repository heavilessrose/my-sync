public class StringsTest {

	public static void main(String[] args) {

		String str = "网络科技破what are you doing now? my dear xiaoZi 我ieh水利科技上帝发哦死立刻节省电费了科技时代佛iuqw老师夸奖对方.空间是垃圾哦 ";
		char[] chars = str.toCharArray();
		System.out.println(new String(chars, 2, 5));
		System.out.println(str.substring(2, 5));
		/////////
		System.out.println(str.indexOf("are", 2));
		
	}
}
