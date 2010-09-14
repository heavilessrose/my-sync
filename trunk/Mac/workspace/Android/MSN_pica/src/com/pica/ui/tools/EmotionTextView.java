package com.pica.ui.tools;

import java.util.Hashtable;
import java.util.Vector;
import com.pica.msn.MsnUtil;
import com.pica.msn.R;
import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Paint;
import android.graphics.Rect;
import android.util.AttributeSet;
import android.view.View;

public class EmotionTextView extends View{
	
	/**支持表情 仅一行 有省略号*/
	public static byte Emotion_Yes_OneLine_Yes=1;
	/**不支持表情 仅一行 有省略号*/
	public static byte Emotion_No_OneLine_Yes=2;
	/**支持表情 多行 */
	public static byte Emotion_Yes_OneLine_No=3;
	/**不支持表情 多行*/
	public static byte Emotion_No_OneLine_No=4; 
	/**支持表情 仅一行 有省略号 有时间戳*/
	public static byte Emotion_Yes_OneLine_Yes_TimeStamp_Yes=5; 
	/**支持表情 仅一行 有省略号 有时间戳 聊天记录*/
	public static byte Emotion_Yes_OneLine_Yes_TimeStamp_Yes_History_Yes=6;
	public String showText;
	public Paint paint;

	/**起始绘制的x,y坐标*/
	int startX,startY;
	/**该页面的宽度*/
 	private int width;
 	/**是否超过一行*/
	boolean isMoreOneLine=false;
	
    /**行距*/
    int distance;
    
    byte type;
	
    /**
     * 
     * @param context <br>
     * @param attrs<br>
     * @param float textSize 字体大小<br>
     * @param int color 颜色<br>
     * @param isShowEmotion 是否显示表情<br>
     */
	public EmotionTextView(Context context, AttributeSet attrs,float textSize,int color ,byte type) 
	{
		super(context, attrs);     
        paint = new Paint();
        paint.setTextSize(textSize);
        paint.setColor(color);
        paint.setAntiAlias(true);
        this.type=type;
	}
	
	public Paint getCurrentPaint()
	{
		return paint;
	}

	/**
	 * 将字符串赋给该控件
	 * @param showText 字符串
	 * @param startX 起始x
	 * @param startY 起始y
	 * @param width  屏幕宽度
	 * @return 高度
	 */
	public int setShowText(String showText,int startX,int startY,int width)
	{
		this.startX=startX;
		this.startY=startY;
		this.width=width;
		this.showText=showText;
		if(this.type==Emotion_Yes_OneLine_No)
			return initEmotion();
		else if(this.type==Emotion_No_OneLine_No)
			return init();
		else if(this.type==Emotion_Yes_OneLine_Yes)
			return initSplitEmotion();
		else if(this.type==Emotion_Yes_OneLine_Yes_TimeStamp_Yes)
			return initSplitEmotionTime();
		else if(this.type==Emotion_Yes_OneLine_Yes_TimeStamp_Yes_History_Yes)
			return initSplitEmotionTimeHistory();
		else
			return -1;
	}

    public int initSplitEmotion()
    {
    	return appendSplitEmotionTextItem(showText, null, 0x000000, (byte)0, ITEM_TYPE_SPLIT_EMOTION, null, startX, startY);
    }
	
    public int initSplitEmotionTime()
    {
    	String imid = showText.substring(0, showText.lastIndexOf("(")-3);
    	String timeStamp = showText.substring(showText.lastIndexOf("(")+1, showText.length()-1);
    	return appendSplitEmotionTextTimeItem(imid,null, 0x000000, (byte)0, ITEM_TYPE_SPLIT_EMOTION_TIME, null, startX, startY, timeStamp);
    }
    
    public int initSplitEmotionTimeHistory()
    {
    	String imid = showText.substring(0, showText.lastIndexOf("("));
    	String timeStamp = showText.substring(showText.lastIndexOf("(")+1, showText.lastIndexOf(")"));
    	return appendSplitEmotionTextTimeItem(imid,null, 0x000000, (byte)0, ITEM_TYPE_SPLIT_EMOTION_TIME, null, startX, startY, timeStamp);
    }
	public int init()
	{
		return appendTextItem(showText, null, 0x000000, (byte)0, ITEM_TYPE_TEXT, null, startX, startY,(byte)-1);
	}
	
	/**
	 * 考虑有图标的情况
	 * @return
	 */
	public int  initEmotion()
	{
		return appendTextItem(showText, null, 0x000000, (byte)0, ITEM_TYPE_TEXT_EMOTION, null, startX, startY,(byte)-1 );
	}
	
	public static byte VCENTER_SPACE = 4; //各元素的行间距

	public static byte HCENTER_SPACE = 4; //各元素的列间距
	
	//****************************
	//页面上元素的类型
	//****************************
	public static final byte ITEM_TYPE_TEXT = 0x01; //文本元素
	public static final byte ITEM_TYPE_TEXT_EMOTION = 0x09; //带表情的文本 
	public static final byte ITEM_TYPE_SPLIT_EMOTION=0x21;//带省略号的一行表情文本
	public static final byte ITEM_TYPE_SPLIT_EMOTION_TIME = 0x22;//带有省略号和时间戳的一行表情文本 
	
	/**有表情的字符串 解析后装载的对象*/
	public Object item[];
	
	
	/**
	 * 添加一个文本类的元素，包括文本元素、回车元素、分隔线元素、超链接元素
	 * @param text 元素的文本内容
	 * @param url 所绘制的元素
	 * @param type 所添加元素的类型
	 * @param color 文本的颜色
	 * @param targetType 链接类型
	 * @param obj  备用附加属性
	 * @param x 元素位于页面位置的横坐标
	 * @param y 元素位于页面位置的纵坐标
	 * @param byte index 将元素添加至具体位置 
	 */
	synchronized public int appendTextItem(String text, String url, int color,
			byte targetType, byte type, Object obj, int x, int y,byte index)
	{
		
		Rect rect=new Rect();
		paint.getTextBounds(text, 0, text.length(), rect);
		item = new Object[15];
		item[ITEM_TYPE] = new Byte(type);
		item[ITEM_LINEID] = new Integer(0);
		int textWidth=(int)paint.measureText(text);
		item[ITEM_HEIGHT] = new Integer(rect.height());
		item[ITEM_STARTX] = new Integer(x);
		item[ITEM_STARTY] = new Integer(y);
		item[ITEM_ISONELINE] = new Boolean(true);
		item[ITEM_LINENUM] = new Integer(1);
		item[ITEM_HAVEFOCUS] = new Boolean(false);
		item[ITEM_ISFOCUSITEM] = new Boolean(false);
		item[ITEM_COLOR] = new Integer(color);
		item[ITEM_CONTENT] = text;
		item[ITEM_URL] = url;
		item[ITEM_TARGET_TYPE] = new Byte(targetType);
		item[ITEM_OBJECT] = obj;
		if (type == ITEM_TYPE_TEXT_EMOTION)//混合文本
		{
			int len =textWidth;// Constant.msnFont.stringWidth(text);
			item[ITEM_WIDTH] = new Integer(len);

			//#ifdef support_enough_memory
			 Object[] ob=MsnUtil.splitNewEmotionText(text, this.width - 2* HCENTER_SPACE-x, paint,false);
			//#else
			//# Object[] ob=MsnUtil.splitEmotionText(text, width - 2* HCENTER_SPACE,paint);
			//#endif
			
			item[ITEM_CONTENT] = ob[0];

			int lnum = ((Vector) item[ITEM_CONTENT]).size();

			if (lnum > 1)
			{
				item[ITEM_ISONELINE] = new Boolean(false);
				item[ITEM_WIDTH] = new Integer(width- x - 2 * HCENTER_SPACE);
			}
			else
				item[ITEM_ISONELINE] = new Boolean(true);

			item[ITEM_LINENUM] = new Integer(lnum);
			item[ITEM_HEIGHT] = new Integer(Math.max(rect.height(),emotionHeight));
		} 
		else
		{
			if (text.equals("") || text.length() == 0)
				item[ITEM_WIDTH] = new Integer(1);
			else
			{
				int len = textWidth;
				
				//#ifdef support_enough_memory
					item[ITEM_CONTENT] = MsnUtil.splitNewText(text,width - 2 * HCENTER_SPACE-x, paint);
				//#else	
				//#	item[ITEM_CONTENT] = MsnUtil.splitText(text, width- 2 * HCENTER_SPACE, paint);
				//#endif
				
				int num=((String[]) item[ITEM_CONTENT]).length;
				if(num>1)
				{
					item[ITEM_WIDTH] = new Integer(width - x - 2 * HCENTER_SPACE);
					item[ITEM_ISONELINE] = new Boolean(false);
					item[ITEM_LINENUM] =new Integer(num);
				}
				else 
				{
					item[ITEM_CONTENT] = text;
					item[ITEM_WIDTH] = new Integer(len);
					item[ITEM_LINENUM] = new Integer(1);
				}
				
//				if (len > this.width - 2 * HCENTER_SPACE)
//				{
//					item[ITEM_WIDTH] = new Integer(len);
//					item[ITEM_ISONELINE] = new Boolean(false);
//					// 为了计算简便 只有英文时候才考虑断行问题
//					//if (Model.languageType == Constant.language_english_type)
//					
//					//#ifdef support_enough_memory
//						item[ITEM_CONTENT] = MsnUtil.splitNewText(text,width - 2 * HCENTER_SPACE-x, paint);
//					//#else	
//					//#	item[ITEM_CONTENT] = MsnUtil.splitText(text, width- 2 * HCENTER_SPACE, paint);
//					//#endif	
//					item[ITEM_LINENUM] = new Integer(
//							((String[]) item[ITEM_CONTENT]).length);
//				} 
//				else
//				{
//					item[ITEM_WIDTH] = new Integer(len);
//					item[ITEM_LINENUM] = new Integer(1);
//				}
			}
		}

		return (((Integer)item[ITEM_LINENUM]).intValue()*(((Integer)item[ITEM_HEIGHT]).intValue() + VCENTER_SPACE));
	}
	
	/**
	 * 添加一个带表情的字符串 超长则添加省略号
	 * @param text 元素的文本内容
	 * @param url 所绘制的元素
	 * @param type 所添加元素的类型
	 * @param color 文本的颜色
	 * @param targetType 链接类型
	 * @param obj  备用附加属性
	 * @param x 元素位于页面位置的横坐标
	 * @param y 元素位于页面位置的纵坐标
	 */
	synchronized public int appendSplitEmotionTextItem(String text, String url, int color,
			byte targetType, byte type, Object obj, int x, int y)
	{
		Rect rect=new Rect();
		paint.getTextBounds(text, 0, text.length(), rect);
		int textWidth=(int)paint.measureText(text);
		item = new Object[15];

		item[ITEM_TYPE] = new Byte(type);
		item[ITEM_LINEID] = new Integer(0);
		item[ITEM_HEIGHT] = new Integer(rect.height());

		item[ITEM_STARTX] = new Integer(x);
		item[ITEM_STARTY] = new Integer(y);
		item[ITEM_ISONELINE] = new Boolean(true);
		item[ITEM_LINENUM] = new Integer(1);
		item[ITEM_HAVEFOCUS] = new Boolean(false);
		item[ITEM_ISFOCUSITEM] = new Boolean(false);
		item[ITEM_COLOR] = new Integer(color);
		item[ITEM_CONTENT] = text;
		item[ITEM_URL] = url;
		item[ITEM_TARGET_TYPE] = new Byte(targetType);


		item[ITEM_OBJECT] = obj;


		if (type == ITEM_TYPE_SPLIT_EMOTION)//
		{
			int len =textWidth;
			item[ITEM_WIDTH] = new Integer(len);
			
			String omit="...";
			int omitWidth=(int)paint.measureText(omit);
			item[ITEM_CONTENT] =MsnUtil.getNewVisibleStringAt(width-omitWidth,paint, text);
			item[ITEM_ISONELINE] = new Boolean(true);
			item[ITEM_LINENUM] = new Integer(1);
			item[ITEM_HEIGHT] = new Integer(Math.max(rect.height(),emotionHeight));	
		}

		return (((Integer)item[ITEM_LINENUM]).intValue()*(((Integer)item[ITEM_HEIGHT]).intValue() + VCENTER_SPACE));
	}
	
	/**
	 * 添加一个带表情的字符串 超长则添加省略号和时间戳
	 * @param text 元素的文本内容
	 * @param url 所绘制的元素
	 * @param type 所添加元素的类型
	 * @param color 文本的颜色
	 * @param targetType 链接类型
	 * @param obj  备用附加属性
	 * @param x 元素位于页面位置的横坐标
	 * @param y 元素位于页面位置的纵坐标
	 */
	synchronized public int appendSplitEmotionTextTimeItem(String text, String url, int color,
			byte targetType, byte type, Object obj, int x, int y, String timeStamp)
	{
		Rect rect=new Rect();
		paint.getTextBounds(text, 0, text.length(), rect);
		int textWidth=(int)paint.measureText(text);
		item = new Object[15];

		item[ITEM_TYPE] = new Byte(type);
		item[ITEM_LINEID] = new Integer(0);
		item[ITEM_HEIGHT] = new Integer(rect.height());

		item[ITEM_STARTX] = new Integer(x);
		item[ITEM_STARTY] = new Integer(y);
		item[ITEM_ISONELINE] = new Boolean(true);
		item[ITEM_LINENUM] = new Integer(1);
		item[ITEM_HAVEFOCUS] = new Boolean(false);
		item[ITEM_ISFOCUSITEM] = new Boolean(false);
		item[ITEM_COLOR] = new Integer(color);
		item[ITEM_CONTENT] = text;
		item[ITEM_URL] = url;
		item[ITEM_TARGET_TYPE] = new Byte(targetType);


		item[ITEM_OBJECT] = obj;

		if (type == ITEM_TYPE_SPLIT_EMOTION_TIME)//
		{
			int len =textWidth;
			item[ITEM_WIDTH] = new Integer(len);
			
			String omit="... 说: ("+timeStamp+")";
			int omitWidth=(int)paint.measureText(omit);
			item[ITEM_CONTENT] =MsnUtil.getNewVisibleStringAt(width-omitWidth,paint, text, timeStamp);
			item[ITEM_ISONELINE] = new Boolean(true);
			item[ITEM_LINENUM] = new Integer(1);
			item[ITEM_HEIGHT] = new Integer(Math.max(rect.height(),emotionHeight));	
		}

		return (((Integer)item[ITEM_LINENUM]).intValue()*(((Integer)item[ITEM_HEIGHT]).intValue() + VCENTER_SPACE));
	}
	
	/***/
	private void drawEmotionTextItem(Canvas canvas,  Object[] item)
	{
		try
		{
		int startX = ((Integer) item[ITEM_STARTX]).intValue();
		int startY = ((Integer) item[ITEM_STARTY]).intValue();
		Vector vector = (Vector) item[ITEM_CONTENT];
		int itemHeight = ((Integer) item[ITEM_HEIGHT]).intValue();

			for (int i = 0; i < vector.size(); i++)
			{
	//			if (startY > this.viewY - this.y
	//					&& startY - itemHeight < this.viewY - this.y
	//							+ this.viewHeight) // 为提高效率，视图外的行不绘制
	//			{
	//				g.setColor(((Integer) item[ITEM_COLOR]).intValue());
	//				Face.drawEmotionText(g, (Vector) vector.elementAt(i), startX
	//						+ this.x, startY + this.y, Graphics.BOTTOM
	//						| Graphics.LEFT);
			
				MsnUtil.drawEmotionText(this,canvas, (Vector) vector.elementAt(i), startX, startY , paint);

				startY = startY + itemHeight + VCENTER_SPACE;
			}
		}catch(Exception e)
		{
			e.printStackTrace();
		}
	}
	
	
	/**
	 * 绘制文本元素
	 * @param g Graphics对象
	 * @param item 所绘制的元素
	 */
	private void drawTextItem(Canvas canvas, Object[] item) 
	{
		
		//g.setColor(item.ITEM_COLOR);
		//g.setColor(((Integer) item[ITEM_COLOR]).intValue());
//		if (item.ITEM_ISONELINE)
//			g.drawString((String) item.ITEM_CONTENT,
//					item.ITEM_STARTX /*- this.viewX*/
//							+ this.x, item.ITEM_STARTY
//							/*- this.viewY*/ + this.y, Graphics.BOTTOM
//							| Graphics.LEFT);
		
		if (((Boolean) item[ITEM_ISONELINE]).booleanValue())
//			g.drawString(((String) item[ITEM_CONTENT]),
//					((Integer) item[ITEM_STARTX]).intValue() /*- this.viewX*/
//							+ this.x, ((Integer) item[ITEM_STARTY]).intValue() + this.y, Graphics.BOTTOM
//							| Graphics.LEFT);
			canvas.drawText(((String) item[ITEM_CONTENT]), ((Integer) item[ITEM_STARTX]).intValue(), ((Integer) item[ITEM_STARTY]).intValue() ,  paint);
			
		else
			drawLongText(canvas, item);

	}
	
	/**
	 * 绘制多行的文本或超链
	 * @param g Graphics对象
	 * @param item 所绘制的元素
	 */
	private void drawLongText(Canvas canvas,  Object[] item) 
	{
		int startX = ((Integer) item[ITEM_STARTX]).intValue();
		int startY = ((Integer) item[ITEM_STARTY]).intValue();
		int itemHeight = ((Integer) item[ITEM_HEIGHT]).intValue();
		String[] str = (String[]) item[ITEM_CONTENT];
		
		byte itemType;
		for (int i = 0; i < str.length; i++) 
		{
			{
			     
				itemType = ((Byte) item[ITEM_TYPE]).byteValue();            
				
				canvas.drawText(str[i],startX,startY,paint);
			}
			startY = startY + itemHeight + VCENTER_SPACE;
		}
	}
	
	private void drawSplitEmotionTextItem(Canvas canvas,  Object[] item) 
	{
		try
		{
			Vector vector = (Vector) item[ITEM_CONTENT];
			int startX = ((Integer) item[ITEM_STARTX]).intValue();
			int startY = ((Integer) item[ITEM_STARTY]).intValue();
			MsnUtil.drawEmotionText(this,canvas, vector, startX, startY , paint);
		} 
		catch (Exception e)
		{
			e.printStackTrace();
		}
		
	}
	
	
    @Override
    public void onDraw(Canvas canvas) 
    {
    	super.onDraw(canvas);
    	
    	paint(canvas);

    }
    
    public void paint(Canvas canvas)
    {
		if(this.type==Emotion_Yes_OneLine_No||this.type==Emotion_No_OneLine_No)
		{
	    	switch (((Byte)item[ITEM_TYPE]).byteValue()) 
			{
			case ITEM_TYPE_TEXT:
				drawTextItem(canvas, item);
				break;
				
			case ITEM_TYPE_TEXT_EMOTION:
				drawEmotionTextItem(canvas, item);
				break;
			}
		}
	    else
	    {
	    	if(this.type==Emotion_Yes_OneLine_Yes || this.type == Emotion_Yes_OneLine_Yes_TimeStamp_Yes ||
	    			this.type == Emotion_Yes_OneLine_Yes_TimeStamp_Yes_History_Yes)	
	    	{
	    		drawSplitEmotionTextItem(canvas, item);
	    	}
	    }

	}
    
    public static Hashtable faceHashtable; 
    
    public static int[] emotion_index=
    {
    	R.drawable.e_1,R.drawable.e_1,R.drawable.e_2,R.drawable.e_2,R.drawable.e_2,R.drawable.e_2,
    	R.drawable.e_3,R.drawable.e_3,R.drawable.e_4,R.drawable.e_4,R.drawable.e_4,R.drawable.e_4,
    	R.drawable.e_5,R.drawable.e_5,R.drawable.e_5,R.drawable.e_5,
    	R.drawable.e_6,R.drawable.e_6,R.drawable.e_7,R.drawable.e_7,R.drawable.e_8,R.drawable.e_8,R.drawable.e_8,R.drawable.e_8,
    	R.drawable.e_9,R.drawable.e_9,R.drawable.e_10,R.drawable.e_10,
    	R.drawable.e_11,R.drawable.e_12,R.drawable.e_12,R.drawable.e_13,R.drawable.e_13,R.drawable.e_14,R.drawable.e_15,
    	R.drawable.e_16,R.drawable.e_17,R.drawable.e_18,R.drawable.e_19,R.drawable.e_20,
    	R.drawable.e_21,R.drawable.e_22,R.drawable.e_23,R.drawable.e_24,R.drawable.e_24,R.drawable.e_25,R.drawable.e_25,
    	R.drawable.e_26,R.drawable.e_26,R.drawable.e_26,R.drawable.e_26,R.drawable.e_26,R.drawable.e_26,R.drawable.e_26,R.drawable.e_26,
    	R.drawable.e_26,R.drawable.e_26,R.drawable.e_26,R.drawable.e_26,R.drawable.e_26,R.drawable.e_26,R.drawable.e_26,
    	R.drawable.e_27,R.drawable.e_28,R.drawable.e_29,R.drawable.e_30,R.drawable.e_31,
    	R.drawable.e_32,R.drawable.e_33,R.drawable.e_34,R.drawable.e_34,R.drawable.e_35,
    	R.drawable.e_36,R.drawable.e_37,R.drawable.e_37,R.drawable.e_38,R.drawable.e_38,
    	R.drawable.e_39,R.drawable.e_39,R.drawable.e_40,R.drawable.e_40,R.drawable.e_40
    };  
    
    public static int[] mThumbIds = {
    		R.drawable.ee_1,R.drawable.ee_2,R.drawable.ee_3,R.drawable.ee_4,
    		R.drawable.ee_5,R.drawable.ee_6,R.drawable.ee_7,R.drawable.ee_8,
    		R.drawable.ee_9,R.drawable.ee_10,R.drawable.ee_11,R.drawable.ee_12,
    		R.drawable.ee_13,R.drawable.ee_14,R.drawable.ee_15,R.drawable.ee_16,
    		R.drawable.ee_17,R.drawable.ee_18,R.drawable.ee_19,R.drawable.ee_20,
    		R.drawable.ee_21,R.drawable.ee_22,R.drawable.ee_23,R.drawable.ee_24,
    		R.drawable.ee_25,R.drawable.ee_26,R.drawable.ee_27,R.drawable.ee_28,
    		R.drawable.ee_29,R.drawable.ee_30,R.drawable.ee_31,R.drawable.ee_32,
    		R.drawable.ee_33,R.drawable.ee_34,R.drawable.ee_35,R.drawable.ee_36,
    		R.drawable.ee_37,R.drawable.ee_38,R.drawable.ee_39,R.drawable.ee_40,
    };
    
    public static String[] emotion_face=
    {
    	 ":)", ":D", ";)",":-o", ":P", 
    	 "(H)",  ":@", ":S", ":$", ":(",
    	 ":'(", ":|", "(a)", "8o|", "8-|", 
    	 "+o(","<:o)","|-)","*-)",":-#",
    	 ":-*","^o)","8-)","(l)","(u)",
    	 "*9mil","(@)","(&)","(sn)","(bah)",
    	 "(S)","(*)","(#)","(R)","({)",
    	 "(})","(k)","(F)","(W)","(0)"
     };
    
    
    public static String[] emotion_type=
    {
    	":)", ":-)", ":d", ":D", ":-d", ":-D", ";)", ";-)", ":o", ":-o", ":O", ":-O", ":p", ":-p", ":P", ":-P",
    	"(h)", "(H)",  ":@", ":-@",  ":s", ":-s", ":S", ":-S", ":$", ":-$",":(", ":-(",
    	 ":'(", ":|", ":-|", "(a)","(A)","8o|","8-|",
    	 "+o(","<:o)","|-)","*-)",":-#",
    	 ":-*","^o)","8-)","(l)","(L)","(u)","(U)",
    	 "*9mil","*red+u","*unicef","*wwf","*oxfam","*one","*care","*mssoc","*help","*hsus","*bgca","*sierra","*acs","*mod","*naf","(@)","(&)","(sn)","(bah)",
    	 "(S)","(*)","(#)","(R)","(r)","({)",
    	 "(})","(k)","(K)","(F)","(f)","(W)","(w)","(0)","(o)","(O)"
     };
    
    public static int emotionWidth;
    public static int emotionHeight;
	public static void initImage()
	{
		// 初始化哈希表
		faceHashtable = new Hashtable();
		for (int i = emotion_type.length; i-- > 0;)
		{
			faceHashtable.put(emotion_type[i], new Integer(emotion_index[i]));
		}
	}
 
	
	//********************
	// page 页面参数
	//*********************
	/**元素类型 0x00*/
	public static final byte ITEM_TYPE = 0x00; //元素类型
	/**元素的行号 0x01*/
	private static final byte ITEM_LINEID = 0x01; //元素的行号
	/**元素高度 0x02*/
	private static final byte ITEM_HEIGHT = 0x02; //元素高度
	/**元素宽度 0x03*/
	private static final byte ITEM_WIDTH = 0x03; //元素宽度
	/**元素起始横坐标 0x04*/
	public static final byte ITEM_STARTX = 0x04; //元素起始横坐标
	/**元素起始纵坐标 0x05*/
	private static final byte ITEM_STARTY = 0x05; //元素起始纵坐标
	/**元素是否为单行 0x06*/
	private static final byte ITEM_ISONELINE = 0x06; //元素是否为单行
	/**元素所占行数 0x07*/
	private static final byte ITEM_LINENUM = 0x07; //元素所占行数
	/**元素是否具有焦点 0x08*/
	private static final byte ITEM_HAVEFOCUS = 0x08; //元素是否具有焦点 
	/**元素能否具有焦点 0x09*/
	private static final byte ITEM_ISFOCUSITEM = 0x09; //元素能否具有焦点
	/**元素的颜色 0x0a*/
	private static final byte ITEM_COLOR = 0x0a; //元素的颜色
	
	//****************************
	//文本和超链元素 图像 属性索引
	//****************************
	/**元素的文本内容 0x0b*/
	public static final byte ITEM_CONTENT = 0x0b; //元素的文本内容
	/**超链所指向的URL 0x0c*/
	private static final byte ITEM_URL = 0x0c; //超链所指向的URL
	/**目标类型 0x0d*/
	private static final byte ITEM_TARGET_TYPE = 0x0d; //目标类型
	/**备用附加属性 0x0e*/
	private static final byte ITEM_OBJECT = 0x0e; //备用附加属性

	
	//****************************
	//图片元素属性索引
	//****************************
	/**图片对象 0x0b*/
	public static final byte ITEM_PICTURE = 0x0b; //图片对象
	/**备用附加属性 0x0d*/
	public static final byte ITEM_IMAGE_OBJECT = 0x0d;  //备用附加属性
	/**0x10*/
	public static final byte ITEM_IMAGE_TARGET_TYPE = 0x10;
	/**统计包 0x12*/
	public static final byte ITEM_ADVIST=0x12;//统计包

	
	//****************************
	//表单控件共有属性索引
	//****************************
	/**控件的标题 0x0b*/
	public static final byte ITEM_LABEL = 0x0b; //控件的标题
	/**控件的标题数组 0x0c*/
	public static final byte ITEM_LABEL_CONTENT=0x0c;//控件的标题数组
	/**控件默认显示的内容 0x0d*/
	public static final byte ITEM_TEXT = 0x0d; //控件默认显示的内容
	/**输入框的宽度 0x0e*/
	public static final byte ITEM_BOXWIDTH = 0x0e; //输入框的宽度
	/**表单id 0x0f*/
	public static final byte ITEM_FORMID = 0x0f; //表单id Integer
	/**表单控件所要提交的变量名 0x10*/
	public static final byte ITEM_VALUENAME = 0x10; //表单控件所要提交的变量名
	/**表单控件所要提交的值 0x11*/
	public static final byte ITEM_VALUE = 0x11; //表单控件所要提交的值

	
	//****************************
	//文本输入控件独有属性索引
	//****************************

	/**最大允许输入的字符数 0x12*/
	private static final byte ITEM_MAXSIZE = 0x12; //最大允许输入的字符数
	/**输入的格式限制 0x13*/
	public static final byte ITEM_CONSTRAINTS = 0x13; //输入的格式限制
	/**所引用的高级界面控件 0x14*/
	public static final byte ITEM_TEXTBOX = 0x14; //所引用的高级界面控件

	
	//****************************
	//复选框控件独有属性索引
	//****************************
	/**复选框控件所引用的对象 0x0d*/
    public static final byte ITEM_SELECTED_OBJECT = 0x0d; //复选框控件所引用的对象	
	/**选中状态标志 0x0e*/
	public static final byte  ITEM_SELECTEDFLAG = 0x0e;//选中状态标志

	
	//****************************
	//下拉列表控件独有属性索引
	//****************************
	/**老索引 0x0d*/
	public static final byte ITEM_OLD_SELECT = 0x0d; //老索引
    /**所引用的高级界面控件 0x13*/
	public static final byte ITEM_CHOICEGROUP = 0x13; //所引用的高级界面控件
	/**变量名数组 0x12*/
	public static final byte ITEM_VALUEGROUP = 0x12;        //变量名数组

	
	//*****************************
	//表单提交按钮元素属性索引
    //*****************************
	/**0x0b*/
	private static final byte ITEM_CAPTION = 0x0b;
	/**0x0c*/
	public static final byte ITEM_SUBMITFORMID = 0x0c;
	/**0x0d*/
	public static final byte ITEM_SUBMITURL = 0x0d;
	/**提交的方法 post/get 0x0e*/
	public static final byte ITEM_SUBMIT_METHOD=0x0e;//提交的方法 post/get
	/**0x0f*/
	public static final byte ITEM_SUBMIT_TYPE=0x0f;
	/**0x12*/
	public static final byte ITEM_SUBMIT_DATA=0x12;
}


