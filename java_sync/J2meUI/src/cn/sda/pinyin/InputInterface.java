package cn.sda.pinyin;

/**
 *
 * @author Administrator
 */
public interface InputInterface {
   abstract void InputFuncChar(char inChar, boolean sbc);
   abstract void InputVisibleString(String inString);
   abstract void InputVisibleChar(char inChar) ;
}
