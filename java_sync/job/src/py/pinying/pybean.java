package py.pinying;

/**
 * <p>Title: </p>
 * <p>Description: </p>
 * <p>Copyright: Copyright (c) 2004</p>
 * <p>Company: ���ڳ������������ </p>
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
      System.out.println("eerr����");
      eerr.printStackTrace();
    }
  }

  public pybean() {
  }
  java.util.Hashtable haallSomeSM=new java.util.Hashtable();

  public static void main(String[] args) {
    pybean pybean1 = new pybean();
  }
//���ݵ������ֵõ�ƴ��(��������)
  public String getpinying(String inhanzhi) {
    return inO.getpinying(inhanzhi);
  }

  //���ݺ��ִ��õ�ƴ��(��������)
  public String getSomepiny(String inString) {
    return inO.getStringpiny(inString);
  }

  //���ݺ��ִ��õ���ĸ(��������)
  public String[] getSomeSM(String[] inString) {
    String sarray[] = new String[inString.length];
for (int k = 0; k < inString.length; k++) {
  sarray[k] = inO.getStringpiny(inString[k]);
}
return sarray;



//    return inO.getSomeSM(inString);
  }
  //���ݺ��ִ�����  ��  ƴ����(ƴ�����ַ�)    �õ���ƴ��������ĺ��ִ�����
  public String[] getListByHanzi(String[] inString, String piny) {
    if(piny==""){return inString;}
    return inO.getListByHanzi(inString, piny);
  }

  //����ƴ��������  ��  ƴ����(ƴ�����ַ�)    �õ���ƴ��������ĺ��ִ�����
  public String[] getListByPiny(String[] inString, String piny) {
    return inO.getListByPiny(inString, piny);
  }
   //����ƴ��������,ƴ������� ƴ����(ƴ�����ַ�)    �õ���ƴ��������ĺ��ִ�����
 public String[] getListByPiny(String[] inString, String[] py, String piny) {
    return  inO.getListByPiny(inString,  py,  piny);
 }
  //���ݺ��ִ�����  ��  ƴ����(ƴ�����ַ�)    �õ���ƴ��������ĺ��ִ�������±�
  public int[] getIndexByHanzi(String[] inString, String piny) {
    return inO.getIndexByPiny(inO.getSomeSM(inString), piny);
  }

  //����ƴ��������  ��  ƴ����(ƴ�����ַ�)    �õ���ƴ��������ĺ��ִ�������±�
//  public int[] getIndexByPiny(String[] inString, String piny) {
//    return inO.getIndexByPiny(inString, piny);
//  }
//�����ַ����õ�ƴ����
  public String getSomeSM(String inString) {
    return inO.getStringSM(inString);
  }


}
