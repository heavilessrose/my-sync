public class StringsTest {

	public static void main(String[] args) {

		String str = "����Ƽ���what are you doing now? my dear xiaoZi ��iehˮ���Ƽ��ϵ۷�Ŷ�����̽�ʡ����˿Ƽ�ʱ����iuqw��ʦ�佱�Է�.�ռ�������Ŷ ";
		char[] chars = str.toCharArray();
		System.out.println(new String(chars, 2, 5));
		System.out.println(str.substring(2, 5));
		/////////
		System.out.println(str.indexOf("are", 2));
		
	}
}
