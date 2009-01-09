package py.pinying;

/**
 * <p>Title: </p>
 * <p>Description: </p>
 * <p>Copyright: Copyright (c) 2004</p>
 * <p>Company: 深圳尘日软件工作室 </p>
 * @author  wangsong
 * @version 1.1
 */
import java.io.*;
import java.io.InputStream;
import java.util.PropertyResourceBundle;
import java.util.ResourceBundle;

public class pybean {
  public static stringall inO;
  static {
    try {
      java.io.InputStream ins = pybean.class.getResourceAsStream("wmwpy.properties");
      java.io.ObjectInputStream objint = new java.io.ObjectInputStream(ins);
      inO = (stringall) objint.readObject();
    }
    catch (Exception eerr) {
      System.out.println("eerr出错");
      eerr.printStackTrace();
    }
  }

  public pybean() {
  }
  java.util.Hashtable haallSomeSM=new java.util.Hashtable();

  public static void main(String[] args) {
    pybean pybean1 = new pybean();
  }
//根据单个汉字得到拼音(包括多音)
  public String getpinying(String inhanzhi) {
    return inO.getpinying(inhanzhi);
  }

  //根据汉字串得到拼音(包括多音)
  public String getSomepiny(String inString) {
    return inO.getStringpiny(inString);
  }

  //根据汉字串得到声母(包括多音)
  public String[] getSomeSM(String[] inString) {
    String sarray[] = new String[inString.length];
for (int k = 0; k < inString.length; k++) {
  sarray[k] = inO.getStringpiny(inString[k]);
}
return sarray;



//    return inO.getSomeSM(inString);
  }
  //根据汉字串数组  和  拼音串(拼音首字符)    得到按拼音串排序的汉字串数组
  public String[] getListByHanzi(String[] inString, String piny) {
    if(piny==""){return inString;}
    return inO.getListByHanzi(inString, piny);
  }

  //根据拼音串数组  和  拼音串(拼音首字符)    得到按拼音串排序的汉字串数组
  public String[] getListByPiny(String[] inString, String piny) {
    return inO.getListByPiny(inString, piny);
  }
   //根据拼音串数组,拼音数组和 拼音串(拼音首字符)    得到按拼音串排序的汉字串数组
 public String[] getListByPiny(String[] inString, String[] py, String piny) {
    return  inO.getListByPiny(inString,  py,  piny);
 }
  //根据汉字串数组  和  拼音串(拼音首字符)    得到按拼音串排序的汉字串数组的下标
  public int[] getIndexByHanzi(String[] inString, String piny) {
    return inO.getIndexByPiny(inO.getSomeSM(inString), piny);
  }

  //根据拼音串数组  和  拼音串(拼音首字符)    得到按拼音串排序的汉字串数组的下标
//  public int[] getIndexByPiny(String[] inString, String piny) {
//    return inO.getIndexByPiny(inString, piny);
//  }
//根据字符串得到拼音串
  public String getSomeSM(String inString) {
    return inO.getStringSM(inString);
  }


}
