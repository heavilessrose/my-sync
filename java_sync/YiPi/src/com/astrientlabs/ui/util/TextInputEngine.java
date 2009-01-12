/*
 * Copyright (C) 2006 Astrient Labs, LLC
 * 
 * Astrient Labs, LLC 
 * www.astrientlabs.com 
 * rashid@astrientlabs.com
 * Rashid Mayes 2006
 */
package com.astrientlabs.ui.util;

import javax.microedition.lcdui.Canvas;

import com.astrientlabs.prefs.PropertyMap;

public class TextInputEngine implements Runnable
{    
    public static final int EVENT_IDLE = 0;
    public static final int EVENT_DELETE = 1;
    public static final int EVENT_CURSOR_FLASH = 2;
    public static final int EVENT_CURSOR_FORWARD = 3;
    public static final int EVENT_CURSOR_BACK = 4;
    public static final int EVENT_APPEND = 5;
    
    public static final int MODE_SINGLETAP = 0;
    public static final int MODE_MULTITAP = 1;
    
    private StringBuffer buffer = new StringBuffer();
    
    private TextInputListener textInputListener;
    private int maxTextChars;
    private boolean cursorMoved = false;
    private boolean idle = true;
    private boolean uppercase = false;
    private boolean running = false;
    private boolean hasFocus = false;
    public int keyTimeout;
    public int idleTimeout;
    private int caretPosition = -1;
    private int charIndex;    
    private int cursorFlashRate; 
    private long lastEventTimestamp = 0;
    private long lastCycle;
    private int mode = MODE_MULTITAP;

    private int lastKeyCode = Integer.MIN_VALUE;
    
    private PropertyMap keyMap;
    private String chars;
    
    
    public TextInputEngine(TextInputListener textInputListener, int idleTimeout, int keyTimeout, int cursorFlashRate, int maxTextChars)
    {
        this.textInputListener = textInputListener;
        this.maxTextChars = maxTextChars;
        this.idleTimeout = idleTimeout;
        this.keyTimeout = keyTimeout;
        this.cursorFlashRate = cursorFlashRate;
        
        String platform = System.getProperty("microedition.platform");
        if ( platform != null )
        {
            if ( platform.startsWith("NokiaE61") || platform.startsWith("NokiaE62") )
            {
                keyMap = new PropertyMap("nokiae61");
                keyMap.initialize();
            }
        }
        
        if ( keyMap == null )
        {
            keyMap = new PropertyMap("keymap");
            keyMap.put(String.valueOf(Canvas.KEY_NUM0).intern()," 0");
            keyMap.put(String.valueOf(Canvas.KEY_NUM1).intern(),".,?!@1");
            keyMap.put(String.valueOf(Canvas.KEY_NUM2).intern(),"abc2");
            keyMap.put(String.valueOf(Canvas.KEY_NUM3).intern(),"def3");
            keyMap.put(String.valueOf(Canvas.KEY_NUM4).intern(),"ghi4");
            keyMap.put(String.valueOf(Canvas.KEY_NUM5).intern(),"jkl5");
            keyMap.put(String.valueOf(Canvas.KEY_NUM6).intern(),"mno6");
            keyMap.put(String.valueOf(Canvas.KEY_NUM7).intern(),"pqrs7");
            keyMap.put(String.valueOf(Canvas.KEY_NUM8).intern(),"tuv8");
            keyMap.put(String.valueOf(Canvas.KEY_NUM9).intern(),"wxyz9");
            keyMap.put(String.valueOf(Canvas.KEY_STAR).intern(),".,?!\"-()@/:_;+&%*=<>\u00A3\u20AC$\u00A5\u00A4[]{}\\~^`\u00A1\u00BF\u00A7#| ");           
        }
        
        setMode(keyMap.getInt("mode",MODE_MULTITAP));
    }
       
    public void onFocusEnter()
    {
        hasFocus = true;
        caretPosition=buffer.length()-1;
        
        //cursorMoved = true;
        lastKeyCode = 0;
        charIndex = 0;
        idle = true;
        uppercase = false;
    }
    
    public void onFocusExit()
    {
        onFocusEnter();
        hasFocus = false;
    }
    

    protected void mark()
    {
        lastEventTimestamp = System.currentTimeMillis();
        idle = false;
    }
    
    public boolean keyPressed(int keyCode)
    {
        
        if ( (chars = keyMap.get(String.valueOf(keyCode).intern())) != null )
        {       
            if ( mode == MODE_SINGLETAP || (lastKeyCode != keyCode || ((lastEventTimestamp + keyTimeout) < System.currentTimeMillis())) )
            {
                lastKeyCode = keyCode;
                charIndex = 0;

                if (!cursorMoved && buffer.length() < maxTextChars)
                {
                    caretPosition++;
                    cursorMoved = true;
                }

                if (caretPosition == buffer.length() && buffer.length() < maxTextChars)
                {
                    buffer.append(' ');
                }       
            }

            if ( uppercase )
            {
                buffer.setCharAt(caretPosition,Character.toUpperCase(chars.charAt((charIndex++) % chars.length())));
            }
            else
            {
                buffer.setCharAt(caretPosition,chars.charAt((charIndex++) % chars.length()));
            }

            textInputListener.update(this, EVENT_APPEND);
            cursorMoved = false;
            mark();
            
            return true;
        }
        else if ( keyCode == Canvas.KEY_POUND )
        {
            uppercase = !uppercase;
            return true;
        }
        
        return false;
    }

    public boolean keyReleased(int keyCode)
    {
        return false;
    }

    public boolean keyRepeated(int keyCode)
    {
        return keyPressed(keyCode);
    }

    
    public void moveCursor(boolean forward)
    {
        if ( forward )
        {
            if ( caretPosition < (buffer.length()-1) )
            {
                caretPosition++;
                textInputListener.update(this,EVENT_CURSOR_FORWARD);
            }
        }
        else
        {
            if ( caretPosition > 0 )
            {
                caretPosition--;
                textInputListener.update(this,EVENT_CURSOR_BACK);
            }
        }
        
        mark();
    }
    
    
    public  void delete()       
    {
        if ( caretPosition > 0 && buffer.length() > 0 )
        {
            buffer.deleteCharAt(--caretPosition);
            textInputListener.update(this,EVENT_DELETE);
        }
        mark();
    }
      


    public void run()
    {
        if ( !running && hasFocus )
        {
            try
            {
                running = true;
                
                long currentTs = System.currentTimeMillis();
                if (mode == MODE_SINGLETAP || currentTs >= (lastEventTimestamp+keyTimeout) )
                {
                    if ( !cursorMoved && buffer.length() < maxTextChars)
                    {
                         cursorMoved = true;
                         caretPosition++;
                         if ( caretPosition == buffer.length() ) buffer.append(' ');
                         
                         textInputListener.update(this, EVENT_CURSOR_FORWARD);
                    }
                }

                if ( !idle && (currentTs >= (lastEventTimestamp+idleTimeout)) )
                {
                    textInputListener.update(this,EVENT_IDLE);
                    idle = true;  
                }
                
                
                if ( currentTs > (lastCycle+cursorFlashRate))
                {
                    lastCycle = currentTs;
                    textInputListener.update(this, EVENT_CURSOR_FLASH);
                }
            }
            finally
            {
                running = false;
            }      
        }
    }
    
    
    public void setText(String text)
    {
        buffer.setLength(0);
        buffer.append(text);
        onFocusEnter();
    }
    
    public String getText()
    {
        return buffer.toString();
    }

    public int getCaretPosition()
    {
        return caretPosition;
    }
    
    public int getMode()
    {
        return mode;
    }
    
    public void setMode(int mode)
    {
        this.mode = mode;
    }
}
