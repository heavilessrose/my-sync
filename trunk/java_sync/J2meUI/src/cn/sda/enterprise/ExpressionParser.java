package cn.sda.enterprise;

import java.util.Hashtable;
import java.util.Vector;

/**
 * 简单的表达式解析程序
 * 
 * @author Administrator
 * @VarsList Hashtable -变量列表，计算的时候从中获取变量值
 * @Expression StringBuffer -表达式
 */
public class ExpressionParser {

	public Hashtable VarsList;
	private String Expression;
	private int LeftBracketIndex = 0;
	// 返回结果
	private double calResult = 0.0;
	// 4种错误类型
	public static final int SYNTAX_ERROR = 0; // 语法错误

	public static final int UNBALPARENS_ERROR = 1; // 括号没有结束错误

	public static final int NOEXP_ERROR = 2; // 表达式为空错误

	public static final int DIVBYZERO_ERROR = 3; // 被0除错误
	// 针对4种错误类型定义的4个错误提示

	public static final String[] ERROR_MESSAGES = { "Syntax Error",
			"Unbalanced Parentheses", "No Expression Present",
			"Division by Zero" };

	public ExpressionParser() {
		internalSDAExpression();
	}

	private void internalSDAExpression() {
		VarsList = new Hashtable();
		LeftBracketIndex = 0;
	}

	/**
	 * @param String
	 *            exp -表达式字符串
	 */
	public void setExpression(String exp) {
		internalSetExpression(exp);
	}

	private void internalSetExpression(String exp) {
		Expression = exp;
		// 分析变量
		getVars();
	}

	/**
	 * @return double -计算表达式返回数值
	 */
	public double calculate() throws Exception {
		return internalCalculate();
	}

	private double internalCalculate() throws Exception {
		try {
			parseExp(Expression);
			return calResult;
		} catch (Exception e) {
			throw new Exception(e.getMessage());
		}
	}

	/**
	 * 对表达式进行解析
	 * 
	 * @param String
	 *            exp:要解析的表达式
	 */
	private void parseExp(String exp) throws Exception {
		// 递归解析
		// 左括号位置
		int leftpos = -1;
		// 右括号位置
		int rightpos = -1;
		// 括号区域的最大层
		int maxLayer = 0;
		// 当前指针位置
		int pos = 0;
		while (pos < exp.length()) {
			// 首先判断括号，来分段递归处理
			char ch = exp.charAt(pos);
			if (ch == '(') {
				LeftBracketIndex++;
				if (LeftBracketIndex > maxLayer) {
					maxLayer = LeftBracketIndex;
					leftpos = pos;
					rightpos = exp.indexOf(")", leftpos);
					if (rightpos == -1) {
						handleError(SYNTAX_ERROR);
					}
				}
			}
			if (ch == ')') {
				LeftBracketIndex--;
			}
			pos++;
		}
		// 循环完毕后，查看是否有内容可以得到
		StringBuffer calExp = new StringBuffer();
		if (leftpos > -1) {
			calExp.append(exp.substring(leftpos + 1, rightpos));
		} else {
			// 没有括号，就是简单表达式
			calExp.append(exp);
		}
		// 将无括号的最简单的表达式分离
		Vector vlist = new Vector();
		pos = 0;
		String element = "";
		char ch;
		while (pos < calExp.length()) {
			ch = calExp.charAt(pos);
			if (isDelim(ch)) {
				if (element.length() > 0) {
					vlist.addElement(element);
					element = "";
				}
				element = String.valueOf(ch);
			}
			if (isLetter(ch) || (Character.isDigit(ch)) || ch == '.') {
				element += ch;
				pos++;
				continue;
			}
			if (element.length() > 0) {
				vlist.addElement(element);
				element = "";
			}
			pos++;
		}
		if (element.length() > 0) {
			vlist.addElement(element);
			element = "";
		}
		// 根据＋-*/优先级进行运算
		int index = -1;
		String x1 = "";
		String x2 = "";
		double x3 = 0.0;
		String[] op = { "*", "/", "+", "-" };
		for (int i = 0; i < op.length; i++) {
			while (vlist.contains(op[i])) {
				index = vlist.indexOf(op[i]);
				if (index > 0) {
					x1 = (String) vlist.elementAt(index - 1);
					x2 = (String) vlist.elementAt(index + 1);
					x3 = baseCal(x1, x2, op[i]);
					calResult = x3;
					// 替换
					vlist.setElementAt(String.valueOf(x3), index);
					// 删除
					vlist.removeElementAt(index + 1);
					vlist.removeElementAt(index - 1);
				}
			}
		}
		// 执行完毕了，用值替换原表达式的值
		if (leftpos > -1) {
			exp = exp.substring(0, leftpos) + (String) vlist.elementAt(0)
					+ exp.substring(rightpos + 1, exp.length());
			if (exp.length() > 0) {
				parseExp(exp);
			}
		}
	}

	private double baseCal(String bx1, String bx2, String operator)
			throws Exception {
		// 查看是不是变量
		double x1 = 0.0;
		double x2 = 0.0;
		if (isLetter(bx1.charAt(0))) {
			x1 = Double.parseDouble(getParamValue(bx1));
		} else {
			x1 = Double.parseDouble(bx1);
		}
		if (isLetter(bx2.charAt(0))) {
			x2 = Double.parseDouble(getParamValue(bx2));
		} else {
			x2 = Double.parseDouble(bx2);
		}
		// 最小运算
		double result = 0;
		if (operator.equals("+")) {
			result = x1 + x2;
		}
		if (operator.equals("-")) {
			result = x1 - x2;
		}
		if (operator.equals("*")) {
			result = x1 * x2;
		}
		if (operator.equals("/")) {
			if (x2 == 0) {
				handleError(DIVBYZERO_ERROR);
			} else {
				result = x1 / x2;
			}
		}
		return result;
	}

	/**
	 * 处理异常情况
	 */
	private void handleError(int errorType) throws Exception {
		// 遇到异常情况时，根据错误类型，取得异常提示信息，将提示信息封装在异常中抛出
		throw new Exception(ERROR_MESSAGES[errorType]);
	}

	/**
	 * 对表达式需要的参数赋值
	 * 
	 * @param String
	 *            paramName:参数名
	 * @param String
	 *            paramValue:参数值
	 */
	public void setParamValue(String paramName, String paramValue) {
		internalSetParamValue(paramName, paramValue);
	}

	private void internalSetParamValue(String paramName, String paramValue) {
		if (VarsList.containsKey(paramName)) {
			VarsList.put(paramName, paramValue);
		}
	}

	/**
	 * 获取所有的变量
	 */
	private void getVars() {
		// 清除
		VarsList.clear();
		// 临时列表
		Vector vt = new Vector();
		// 过滤查找所有变量
		int pos = 0;
		String element = "";
		char ch;
		while (pos < Expression.length()) {
			ch = Expression.charAt(pos);
			if (isLetter(ch)) {
				element += ch;
				pos++;
				continue;
			}
			if (Character.isDigit(ch)) {
				if (element.length() > 0) {
					element += ch;
					pos++;
					continue;
				}
			}
			if (element.length() > 0) {
				vt.addElement(element);
				element = "";
			}
			pos++;
		}
		if (element.length() > 0) {
			vt.addElement(element);
			element = "";
		}
		// 处理重复的
		for (int i = 0; i < vt.size(); i++) {
			Object object = vt.elementAt(i);
			if (!VarsList.containsKey(object)) {
				VarsList.put(object, "");
			}
		}
		vt = null;
	}

	/**
	 * 从参数列表中获取指定参数的值
	 * 
	 * @param String
	 *            paramName:参数名
	 * @return String :返回指定参数名的值(以字符串表示)
	 */
	private String getParamValue(String paramName) {
		if (VarsList.containsKey(paramName)) {
			return (String) VarsList.get(paramName);
		} else {
			// 没有发现
			return null;
		}
	}

	private boolean isDelim(char c) {
		if (("+-*/()".indexOf(c) != -1)) {
			return true;
		}
		return false;
	}

	private boolean isLetter(char ch) {
		if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || (ch == '_')) {
			return true;
		}
		return false;
	}
}
