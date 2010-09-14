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
	
	/**֧�ֱ��� ��һ�� ��ʡ�Ժ�*/
	public static byte Emotion_Yes_OneLine_Yes=1;
	/**��֧�ֱ��� ��һ�� ��ʡ�Ժ�*/
	public static byte Emotion_No_OneLine_Yes=2;
	/**֧�ֱ��� ���� */
	public static byte Emotion_Yes_OneLine_No=3;
	/**��֧�ֱ��� ����*/
	public static byte Emotion_No_OneLine_No=4; 
	/**֧�ֱ��� ��һ�� ��ʡ�Ժ� ��ʱ���*/
	public static byte Emotion_Yes_OneLine_Yes_TimeStamp_Yes=5; 
	/**֧�ֱ��� ��һ�� ��ʡ�Ժ� ��ʱ��� �����¼*/
	public static byte Emotion_Yes_OneLine_Yes_TimeStamp_Yes_History_Yes=6;
	public String showText;
	public Paint paint;

	/**��ʼ���Ƶ�x,y����*/
	int startX,startY;
	/**��ҳ��Ŀ��*/
 	private int width;
 	/**�Ƿ񳬹�һ��*/
	boolean isMoreOneLine=false;
	
    /**�о�*/
    int distance;
    
    byte type;
	
    /**
     * 
     * @param context <br>
     * @param attrs<br>
     * @param float textSize �����С<br>
     * @param int color ��ɫ<br>
     * @param isShowEmotion �Ƿ���ʾ����<br>
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
	 * ���ַ��������ÿؼ�
	 * @param showText �ַ���
	 * @param startX ��ʼx
	 * @param startY ��ʼy
	 * @param width  ��Ļ���
	 * @return �߶�
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
	 * ������ͼ������
	 * @return
	 */
	public int  initEmotion()
	{
		return appendTextItem(showText, null, 0x000000, (byte)0, ITEM_TYPE_TEXT_EMOTION, null, startX, startY,(byte)-1 );
	}
	
	public static byte VCENTER_SPACE = 4; //��Ԫ�ص��м��

	public static byte HCENTER_SPACE = 4; //��Ԫ�ص��м��
	
	//****************************
	//ҳ����Ԫ�ص�����
	//****************************
	public static final byte ITEM_TYPE_TEXT = 0x01; //�ı�Ԫ��
	public static final byte ITEM_TYPE_TEXT_EMOTION = 0x09; //��������ı� 
	public static final byte ITEM_TYPE_SPLIT_EMOTION=0x21;//��ʡ�Ժŵ�һ�б����ı�
	public static final byte ITEM_TYPE_SPLIT_EMOTION_TIME = 0x22;//����ʡ�Ժź�ʱ�����һ�б����ı� 
	
	/**�б�����ַ��� ������װ�صĶ���*/
	public Object item[];
	
	
	/**
	 * ���һ���ı����Ԫ�أ������ı�Ԫ�ء��س�Ԫ�ء��ָ���Ԫ�ء�������Ԫ��
	 * @param text Ԫ�ص��ı�����
	 * @param url �����Ƶ�Ԫ��
	 * @param type �����Ԫ�ص�����
	 * @param color �ı�����ɫ
	 * @param targetType ��������
	 * @param obj  ���ø�������
	 * @param x Ԫ��λ��ҳ��λ�õĺ�����
	 * @param y Ԫ��λ��ҳ��λ�õ�������
	 * @param byte index ��Ԫ�����������λ�� 
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
		if (type == ITEM_TYPE_TEXT_EMOTION)//����ı�
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
//					// Ϊ�˼����� ֻ��Ӣ��ʱ��ſ��Ƕ�������
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
	 * ���һ����������ַ��� ���������ʡ�Ժ�
	 * @param text Ԫ�ص��ı�����
	 * @param url �����Ƶ�Ԫ��
	 * @param type �����Ԫ�ص�����
	 * @param color �ı�����ɫ
	 * @param targetType ��������
	 * @param obj  ���ø�������
	 * @param x Ԫ��λ��ҳ��λ�õĺ�����
	 * @param y Ԫ��λ��ҳ��λ�õ�������
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
	 * ���һ����������ַ��� ���������ʡ�Ժź�ʱ���
	 * @param text Ԫ�ص��ı�����
	 * @param url �����Ƶ�Ԫ��
	 * @param type �����Ԫ�ص�����
	 * @param color �ı�����ɫ
	 * @param targetType ��������
	 * @param obj  ���ø�������
	 * @param x Ԫ��λ��ҳ��λ�õĺ�����
	 * @param y Ԫ��λ��ҳ��λ�õ�������
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
			
			String omit="... ˵: ("+timeStamp+")";
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
	//							+ this.viewHeight) // Ϊ���Ч�ʣ���ͼ����в�����
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
	 * �����ı�Ԫ��
	 * @param g Graphics����
	 * @param item �����Ƶ�Ԫ��
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
	 * ���ƶ��е��ı�����
	 * @param g Graphics����
	 * @param item �����Ƶ�Ԫ��
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
		// ��ʼ����ϣ��
		faceHashtable = new Hashtable();
		for (int i = emotion_type.length; i-- > 0;)
		{
			faceHashtable.put(emotion_type[i], new Integer(emotion_index[i]));
		}
	}
 
	
	//********************
	// page ҳ�����
	//*********************
	/**Ԫ������ 0x00*/
	public static final byte ITEM_TYPE = 0x00; //Ԫ������
	/**Ԫ�ص��к� 0x01*/
	private static final byte ITEM_LINEID = 0x01; //Ԫ�ص��к�
	/**Ԫ�ظ߶� 0x02*/
	private static final byte ITEM_HEIGHT = 0x02; //Ԫ�ظ߶�
	/**Ԫ�ؿ�� 0x03*/
	private static final byte ITEM_WIDTH = 0x03; //Ԫ�ؿ��
	/**Ԫ����ʼ������ 0x04*/
	public static final byte ITEM_STARTX = 0x04; //Ԫ����ʼ������
	/**Ԫ����ʼ������ 0x05*/
	private static final byte ITEM_STARTY = 0x05; //Ԫ����ʼ������
	/**Ԫ���Ƿ�Ϊ���� 0x06*/
	private static final byte ITEM_ISONELINE = 0x06; //Ԫ���Ƿ�Ϊ����
	/**Ԫ����ռ���� 0x07*/
	private static final byte ITEM_LINENUM = 0x07; //Ԫ����ռ����
	/**Ԫ���Ƿ���н��� 0x08*/
	private static final byte ITEM_HAVEFOCUS = 0x08; //Ԫ���Ƿ���н��� 
	/**Ԫ���ܷ���н��� 0x09*/
	private static final byte ITEM_ISFOCUSITEM = 0x09; //Ԫ���ܷ���н���
	/**Ԫ�ص���ɫ 0x0a*/
	private static final byte ITEM_COLOR = 0x0a; //Ԫ�ص���ɫ
	
	//****************************
	//�ı��ͳ���Ԫ�� ͼ�� ��������
	//****************************
	/**Ԫ�ص��ı����� 0x0b*/
	public static final byte ITEM_CONTENT = 0x0b; //Ԫ�ص��ı�����
	/**������ָ���URL 0x0c*/
	private static final byte ITEM_URL = 0x0c; //������ָ���URL
	/**Ŀ������ 0x0d*/
	private static final byte ITEM_TARGET_TYPE = 0x0d; //Ŀ������
	/**���ø������� 0x0e*/
	private static final byte ITEM_OBJECT = 0x0e; //���ø�������

	
	//****************************
	//ͼƬԪ����������
	//****************************
	/**ͼƬ���� 0x0b*/
	public static final byte ITEM_PICTURE = 0x0b; //ͼƬ����
	/**���ø������� 0x0d*/
	public static final byte ITEM_IMAGE_OBJECT = 0x0d;  //���ø�������
	/**0x10*/
	public static final byte ITEM_IMAGE_TARGET_TYPE = 0x10;
	/**ͳ�ư� 0x12*/
	public static final byte ITEM_ADVIST=0x12;//ͳ�ư�

	
	//****************************
	//���ؼ�������������
	//****************************
	/**�ؼ��ı��� 0x0b*/
	public static final byte ITEM_LABEL = 0x0b; //�ؼ��ı���
	/**�ؼ��ı������� 0x0c*/
	public static final byte ITEM_LABEL_CONTENT=0x0c;//�ؼ��ı�������
	/**�ؼ�Ĭ����ʾ������ 0x0d*/
	public static final byte ITEM_TEXT = 0x0d; //�ؼ�Ĭ����ʾ������
	/**�����Ŀ�� 0x0e*/
	public static final byte ITEM_BOXWIDTH = 0x0e; //�����Ŀ��
	/**��id 0x0f*/
	public static final byte ITEM_FORMID = 0x0f; //��id Integer
	/**���ؼ���Ҫ�ύ�ı����� 0x10*/
	public static final byte ITEM_VALUENAME = 0x10; //���ؼ���Ҫ�ύ�ı�����
	/**���ؼ���Ҫ�ύ��ֵ 0x11*/
	public static final byte ITEM_VALUE = 0x11; //���ؼ���Ҫ�ύ��ֵ

	
	//****************************
	//�ı�����ؼ�������������
	//****************************

	/**�������������ַ��� 0x12*/
	private static final byte ITEM_MAXSIZE = 0x12; //�������������ַ���
	/**����ĸ�ʽ���� 0x13*/
	public static final byte ITEM_CONSTRAINTS = 0x13; //����ĸ�ʽ����
	/**�����õĸ߼�����ؼ� 0x14*/
	public static final byte ITEM_TEXTBOX = 0x14; //�����õĸ߼�����ؼ�

	
	//****************************
	//��ѡ��ؼ�������������
	//****************************
	/**��ѡ��ؼ������õĶ��� 0x0d*/
    public static final byte ITEM_SELECTED_OBJECT = 0x0d; //��ѡ��ؼ������õĶ���	
	/**ѡ��״̬��־ 0x0e*/
	public static final byte  ITEM_SELECTEDFLAG = 0x0e;//ѡ��״̬��־

	
	//****************************
	//�����б�ؼ�������������
	//****************************
	/**������ 0x0d*/
	public static final byte ITEM_OLD_SELECT = 0x0d; //������
    /**�����õĸ߼�����ؼ� 0x13*/
	public static final byte ITEM_CHOICEGROUP = 0x13; //�����õĸ߼�����ؼ�
	/**���������� 0x12*/
	public static final byte ITEM_VALUEGROUP = 0x12;        //����������

	
	//*****************************
	//���ύ��ťԪ����������
    //*****************************
	/**0x0b*/
	private static final byte ITEM_CAPTION = 0x0b;
	/**0x0c*/
	public static final byte ITEM_SUBMITFORMID = 0x0c;
	/**0x0d*/
	public static final byte ITEM_SUBMITURL = 0x0d;
	/**�ύ�ķ��� post/get 0x0e*/
	public static final byte ITEM_SUBMIT_METHOD=0x0e;//�ύ�ķ��� post/get
	/**0x0f*/
	public static final byte ITEM_SUBMIT_TYPE=0x0f;
	/**0x12*/
	public static final byte ITEM_SUBMIT_DATA=0x12;
}


