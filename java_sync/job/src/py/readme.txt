����һ��ƴ����λ��  
    1���ܲ�ѯƴ������������
    2������ƴ����λ��Ϣ 
    3:���޸�����κδ��� ���ܸ���ĳ����������е�ComboBox����ƴ����λ���ܡ���
    
���з�ʽ�� ��װ���л��� 
           ����o.jar��  �������classpath
           ʾ��1�� run.bat  
           ʹ��2�� JAVA -classpath "./o.jar"  wmw.ui.ToolKit  yourclass 
               //yourclassΪ��ĳ���������       
               ���磺JAVA  HelloWord //��ǰ�����з�ʽ
                     JAVA  wmw.ui.ToolKit HelloWord //���ڵ����з�ʽ

//UIʹ�÷�ʽ  
  public static void main(String[] args) {
    try {
      //�����Լ�ʵ��һ��UItool       //����UI
      wmw.ui.ToolKit.StartUI(new wmw.ui.UItool());      
      wmw.ui.ToolKit.SetTimeOut(1000);//��ʱ ���¶�λ 
    }
    catch (Exception e) {
      e.printStackTrace();
    }

  }
ƴ����ʹ��
  pinying.pybean pybean = new pinying.pybean(); //��һ��: �õ��������
  int intlist0[] = pybean.getIndexByHanzi(Item, Skey); //�ڶ��� �õ��������ַ������±�
  ���� String[] tem=new String[]{"123���","���"��"hhhh"}  
       int intlist0[] = pybean.getIndexByHanzi(Item, "dj")
       ��intlist0[] ={1��0��2} 
//���ݵ������ֵõ�ƴ��(��������)
String getpinying(String inhanzhi) 
//���ݺ��ִ��õ�ƴ��(��������)
String getSomepiny(String inString)
//���ݺ��ִ��õ���ĸ(��������)
String[] getSomeSM(String[] inString) 
//���ݺ��ִ�����  ��  ƴ����(ƴ�����ַ�)    �õ���ƴ��������ĺ��ִ�����
public String[] getListByHanzi(String[] inString, String piny) 

//����ƴ��������  ��  ƴ����(ƴ�����ַ�)    �õ���ƴ��������ĺ��ִ�����
public String[] getListByPiny(String[] inString, String piny) 
//����ƴ��������,ƴ������� ƴ����(ƴ�����ַ�)    �õ���ƴ��������ĺ��ִ�����
 public String[] getListByPiny(String[] inString, String[] py, String piny) 
//���ݺ��ִ�����  ��  ƴ����(ƴ�����ַ�)    �õ���ƴ��������ĺ��ִ�������±�
  public int[] getIndexByHanzi(String[] inString, String piny) 
//�����ַ����õ�ƴ����
  public String getSomeSM(String inString) 


ע�⣺����
      *����Բ�ʹ��   pinying.pybean  �����Լ�ģ��  pinying.pybeanдһ��������������stringall��ķ����Ϳ����ˡ�
      *wmwpy.properties ��   stringall���л���Ķ���
       ����wmwpy.properties ��stringall.class�� ������ͬ��classpath�оͿ����ˡ�
       ��Ҫ����stringall.java.  ��Ϊ�������class�ļ��汾��wmwpy.properties��class�汾���ܲ�ͬ(classid)��     