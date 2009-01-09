这是一个拼音定位器  
    1：能查询拼音包括多音。
    2：根据拼音定位信息 
    3:不修改你的任何代码 就能给你的程序里面所有的ComboBox加上拼音定位功能。！
    
运行方式： 安装运行环境 
           将“o.jar”  加入你的classpath
           示例1： run.bat  
           使用2： JAVA -classpath "./o.jar"  wmw.ui.ToolKit  yourclass 
               //yourclass为你的程序启动类       
               例如：JAVA  HelloWord //以前的运行方式
                     JAVA  wmw.ui.ToolKit HelloWord //现在的运行方式

//UI使用方式  
  public static void main(String[] args) {
    try {
      //可以自己实现一个UItool       //设置UI
      wmw.ui.ToolKit.StartUI(new wmw.ui.UItool());      
      wmw.ui.ToolKit.SetTimeOut(1000);//超时 重新定位 
    }
    catch (Exception e) {
      e.printStackTrace();
    }

  }
拼音类使用
  pinying.pybean pybean = new pinying.pybean(); //第一步: 得到处理对象
  int intlist0[] = pybean.getIndexByHanzi(Item, Skey); //第二步 得到排序后的字符数组下标
  例如 String[] tem=new String[]{"123大家","大家"，"hhhh"}  
       int intlist0[] = pybean.getIndexByHanzi(Item, "dj")
       则：intlist0[] ={1，0，2} 
//根据单个汉字得到拼音(包括多音)
String getpinying(String inhanzhi) 
//根据汉字串得到拼音(包括多音)
String getSomepiny(String inString)
//根据汉字串得到声母(包括多音)
String[] getSomeSM(String[] inString) 
//根据汉字串数组  和  拼音串(拼音首字符)    得到按拼音串排序的汉字串数组
public String[] getListByHanzi(String[] inString, String piny) 

//根据拼音串数组  和  拼音串(拼音首字符)    得到按拼音串排序的汉字串数组
public String[] getListByPiny(String[] inString, String piny) 
//根据拼音串数组,拼音数组和 拼音串(拼音首字符)    得到按拼音串排序的汉字串数组
 public String[] getListByPiny(String[] inString, String[] py, String piny) 
//根据汉字串数组  和  拼音串(拼音首字符)    得到按拼音串排序的汉字串数组的下标
  public int[] getIndexByHanzi(String[] inString, String piny) 
//根据字符串得到拼音串
  public String getSomeSM(String inString) 


注意：：：
      *你可以不使用   pinying.pybean  而是自己模仿  pinying.pybean写一个类来处理。调用stringall类的方法就可以了。
      *wmwpy.properties 是   stringall串行化后的对象。
       所有wmwpy.properties 和stringall.class， 放入相同的classpath中就可以了。
       不要编译stringall.java.  因为你编译后的class文件版本和wmwpy.properties的class版本可能不同(classid)。     