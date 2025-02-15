package winkCC.core.store.winksBook;

import java.util.Hashtable;

/**
 * 彩像电话簿.
 * 
 * @author WangYinghua
 * 
 */
public class WinksBook {

	/**
	 * 内存中缓存的彩像电话簿. key: name, value: WinksContacts对象.
	 */
	private Hashtable _allWinksContacts = new Hashtable();

	/**
	 * 获取彩像电话簿中所有联系人.
	 * 
	 * @return
	 */
	public Hashtable getAllWinksContacts() {
		return _allWinksContacts;
	}

	/**
	 * 根据电话号码取得一个联系人.
	 * 
	 * @param no
	 *            电话号码.
	 * @return
	 */
	public WinksContact getWinksContact(String no) {
		return null;
	}

	/**
	 * 初始化. 解析并载入彩像电话簿,利用彩像电话簿完成数据完整性检查等.<br>
	 * 1> 搜索手机所有号码, 如此号码不存在则在彩像电话簿中删除该联系人.<br>
	 * 2> 如果丢失该联系人的彩像文件则删除该联系人绑定的彩像ID, 等待下次更新下载彩像时重新下载丢失的彩像文件.
	 */
	public void init() {
		parseAndLoadWinksBook();
	}

	/**
	 * 验证彩像电话簿中一个联系人的数据完整性.<br>
	 * 1> 搜索手机所有号码, 如此号码不存在则在彩像电话簿中删除该联系人.<br>
	 * 2> 如果丢失该联系人的彩像文件则删除该联系人绑定的彩像ID, 等待下次更新下载彩像时重新下载丢失的彩像文件.
	 * 
	 * 
	 * @param wContact
	 *            彩像电话簿中的联系人.
	 * @return
	 */
	public boolean verifyWinksContact(WinksContact wContact) {
		return false;
	}

	/**
	 * 验证整个彩像电话簿.
	 */
	public void verifyIntegrality() {

	}

	/**
	 * 创建彩像电话簿.<br>
	 * 软件第一次运行由用户选择生成彩像电话簿 或校验完整性后重新生成彩像电话簿.
	 */
	public void createWinksBook(Hashtable wContact) {

		writeWinksBookFile();
	}

	/**
	 * 给彩像电话簿中添加联系人.<br>
	 * 实际上只是加入到缓存中并未立即更改彩像电话簿文件. 应在适当的时候将更改写入文件.
	 * 
	 * @param wContact
	 */
	public void addWinksContact(WinksContact wContact) {

	}

	/**
	 * 删除彩像电话簿中一个联系人.<br>
	 * 实际上只是加入到缓存中并未立即更改彩像电话簿文件. 应在适当的时候将更改写入文件.
	 */
	public void deleteWinksContact(WinksContact wContact) {

	}

	/**
	 * 将彩像联系人列表写入到彩像电话簿xml文件.
	 */
	public void writeWinksBookFile() {

	}

	/**
	 * 解析并将彩像电话簿载入内存.
	 */
	public void parseAndLoadWinksBook() {

	}

	//-------------------------------------------------------
	/**
	 * 添加保存一个彩像文件.
	 * 
	 * @param data
	 */
	public void addCC(byte[] data) {

	}

	/**
	 * 删除一个彩像文件
	 * 
	 * @param ccname
	 */
	public void deleteCC(String ccname) {

	}

	/**
	 * 释放资源.并提交改变到彩像电话簿文件.
	 */
	public void dispose() {

		writeWinksBookFile();
	}

}
