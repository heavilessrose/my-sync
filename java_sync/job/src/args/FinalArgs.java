package args;
public class FinalArgs {
	public static void main(String hh[]) {
		LoginInfo login = new LoginInfo();
		login.setPassword("1235");
		login.setUserName("mygod");
		System.out.println("username:" + login.getUserName() + ",password:"
				+ login.getPassword());

		checkLoginInfo(login);

		System.out.println("username:" + login.getUserName() + ",password:"
				+ login.getPassword());
	}

	public static void checkLoginInfo(final LoginInfo login) {
		// login = new LoginInfo(); // �޷��޸Ĵ������ı���, ԭʼ����Ҳ����
		login.setUserName("yun"); // �����޸ı����е�����
	}

}

class LoginInfo {
	String pass;
	String name;

	public void setPassword(String string) {
		this.pass = string;
	}

	public String getPassword() {
		return this.pass;
	}

	public String getUserName() {
		return this.name;
	}

	public void setUserName(String string) {
		this.name = string;
	}

}