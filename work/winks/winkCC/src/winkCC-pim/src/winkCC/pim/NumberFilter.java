package winkCC.pim;

import java.util.Hashtable;

// 通过此过滤器得到  软件要操作的电话号码
public interface NumberFilter {
	public String filter(Hashtable numbers);
}
