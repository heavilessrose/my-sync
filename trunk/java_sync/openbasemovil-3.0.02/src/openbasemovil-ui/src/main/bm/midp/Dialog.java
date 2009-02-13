/*
 * Copyright (c) 2006 Elondra S.L. All Rights Reserved.
 */
package bm.midp;
/* -----------------------------------------------------------------------------
    OpenBaseMovil User Interface Library
    Copyright (C) 2004-2008 Elondra S.L.

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.
    If not, see <a href="http://www.gnu.org/licenses">http://www.gnu.org/licenses</a>.
----------------------------------------------------------------------------- */

import bm.core.Application;
import bm.core.ResourceManager;
import bm.core.mvc.AbstractView;
import bm.core.tools.Tools;

import javax.microedition.lcdui.*;

/*
 * File Information
 *
 * Created on       : 03-jun-2005 11:15:08
 * Created by       : narciso
 * Last modified by : $Author: narciso $
 * Last modified on : $Date: 2006/01/03 13:17:46 $
 * Revision         : $Revision: 1.2 $
 */

/**
 * Utility class to handle custom dialogs.
 *
 * @author <a href="mailto:narciso@elondra.org">Narciso Cerezo</a>
 * @version $Revision: 1.2 $
 */
public class Dialog
    extends Canvas
{
    public static final int OK      = 1;
    public static final int YES_NO  = 2;

    private static Font buttonFont = Font.getFont(
            Font.FACE_PROPORTIONAL,
            Font.STYLE_BOLD,
            Font.SIZE_SMALL
    );
    private static Font titleFont = Font.getFont(
            Font.FACE_PROPORTIONAL,
            Font.STYLE_BOLD,
            Font.SIZE_LARGE
    );
    private static Font textFont = Font.getFont(
            Font.FACE_PROPORTIONAL,
            Font.STYLE_PLAIN,
            Font.SIZE_SMALL
    );

    private static int backgroundColor = 0xFFFFFF;
    private static int titleColor = 0;
    private static int textColor;
    private static int buttonBackground = 0x00496E;
    private static int selectedButtonBackground = 0x85B0C6;
    private static int buttonColor = 0xDFDFDF;
    private static int selectedButtonColor = 0xFFFFFF;

    public static Font getTitleFont()
    {
        return titleFont;
    }

    public static synchronized void setTitleFont( final Font titleFont )
    {
        Dialog.titleFont = titleFont;
    }

    public static Font getTextFont()
    {
        return textFont;
    }

    public static synchronized void setTextFont( final Font textFont )
    {
        Dialog.textFont = textFont;
    }

    public static Font getButtonFont()
    {
        return buttonFont;
    }

    public static void setButtonFont( final Font buttonFont )
    {
        Dialog.buttonFont = buttonFont;
    }

    public static int getBackgroundColor()
    {
        return backgroundColor;
    }

    public static void setBackgroundColor( final int backgroundColor )
    {
        Dialog.backgroundColor = backgroundColor;
    }

    public static int getTitleColor()
    {
        return titleColor;
    }

    public static void setTitleColor( final int titleColor )
    {
        Dialog.titleColor = titleColor;
    }

    public static int getTextColor()
    {
        return textColor;
    }

    public static void setTextColor( final int textColor )
    {
        Dialog.textColor = textColor;
    }

    public static int getButtonBackground()
    {
        return buttonBackground;
    }

    public static void setButtonBackground( final int buttonBackground )
    {
        Dialog.buttonBackground = buttonBackground;
    }

    public static int getButtonColor()
    {
        return buttonColor;
    }

    public static void setButtonColor( final int buttonColor )
    {
        Dialog.buttonColor = buttonColor;
    }

    public static int getSelectedButtonBackground()
    {
        return selectedButtonBackground;
    }

    public static void setSelectedButtonBackground( final int selectedButtonBackground )
    {
        Dialog.selectedButtonBackground = selectedButtonBackground;
    }

    public static int getSelectedButtonColor()
    {
        return selectedButtonColor;
    }

    public static void setSelectedButtonColor( final int selectedButtonColor )
    {
        Dialog.selectedButtonColor = selectedButtonColor;
    }

    private Displayable current;
    private Display     display;
    private String      title;
    private String      message;
    private int         dialogType;
    private AlertType   alertType;
    private int         selected;
    Boolean     result = null;

    public Dialog( final String title, final String message )
    {
        init( title, message, YES_NO, AlertType.CONFIRMATION );
    }


    /**
     * Constructor.
     *
     * @param title alert title
     * @param message alert message
     * @param dialogType dialog type
     * @param alertType alert type
     */
    public Dialog(
            final String    title,
            final String    message,
            final int       dialogType,
            final AlertType alertType
    )
    {
        init( title, message, dialogType, alertType );
    }

    private void init( final String     title,
                       final String     message,
                       final int        dialogType,
                       final AlertType  alertType
    )
    {
        display = Application.getManager().getDisplay();
        current = display.getCurrent();
        this.title = title;
        this.message = message;
        this.dialogType = dialogType;
        this.alertType = alertType;
        setFullScreenMode( true );
    }

    private int paint(
            final Graphics  g,
            final String[]  text,
            final int       y,
            final Font      font,
            final int       color,
            final boolean   center
    )
    {
        final int length = text.length;
        final int baseLine = font.getBaselinePosition();
        final int lineHeight = font.getHeight() + 1;

        int align, x;
        if( center )
        {
            x = getWidth() / 2;
            align = Graphics.HCENTER | Graphics.BASELINE;
        }
        else
        {
            x = 1;
            align = Graphics.LEFT | Graphics.BASELINE;
        }
        g.setFont( font );
        g.setColor( color );
        int y0 = y;
        for( int i = 0; i < length; i++ )
        {
            if( text[i] != null )
            {
                g.drawString( text[i], x, y0 + baseLine, align );
                y0 += lineHeight;
            }
        }
        return y0;
    }

    protected void paint( Graphics graphics )
    {
        final Font buttonFont = Dialog.buttonFont;
        final Font titleFont = Dialog.titleFont;
        final Font textFont = Dialog.textFont;

        AbstractView.touchLastAccess();
        Graphics g = graphics;
        Image buffer = null;
        int availHeight = getHeight() - buttonFont.getHeight() - 5;
        if( !isDoubleBuffered() )
        {
            buffer = Image.createImage( getWidth(), getHeight() );
            g = buffer.getGraphics();
        }
        g.setColor( backgroundColor );
        g.fillRect( 0, 0, getWidth(), getHeight() );

        int maxLines = availHeight / ( titleFont.getHeight() + 1);
        if( maxLines < 4 )
        {
            maxLines = 3;
        }
        final String[] titleLines = Tools.splitInLines(
                title,
                titleFont,
                titleFont,
                getWidth() - 2,
                maxLines,
                true
        );
        int y = paint( g, titleLines, 1, titleFont, titleColor, true );
        y += 2;

        availHeight -= titleLines.length * ( titleFont.getHeight() + 1);
        maxLines = availHeight / ( textFont.getHeight() + 1);
        final String[] messageLines = Tools.splitInLines(
                message,
                textFont,
                textFont,
                getWidth() - 2,
                maxLines,
                true
        );
        paint( g, messageLines, y, textFont, textColor, false );

        final int buttonHeight = buttonFont.getHeight() + 2;
        final int buttonY = getHeight() - buttonHeight + 1 +
                            buttonFont.getBaselinePosition();
        if( dialogType == OK )
        {
            g.setColor( selectedButtonBackground );
            g.fillRect( 0, getHeight() - buttonHeight, getWidth(), buttonHeight );

            g.setColor( selectedButtonColor );
            g.setFont( buttonFont );
            g.drawString(
                    ResourceManager.getResource( "global.Ok" ),
                    1,
                    buttonY,
                    Graphics.LEFT | Graphics.BASELINE
            );
        }
        else
        {
            g.setColor( selected == 0 ? selectedButtonBackground : buttonBackground );
            g.fillRect( 0, getHeight() - buttonHeight, getWidth() / 2, buttonHeight );

            g.setColor( selected == 0 ? selectedButtonColor : buttonColor );
            g.setFont( buttonFont );
            g.drawString(
                    ResourceManager.getResource( "global.Yes" ),
                    1,
                    buttonY,
                    Graphics.LEFT | Graphics.BASELINE
            );

            g.setColor( selected == 1 ? selectedButtonBackground : buttonBackground );
            g.fillRect( getWidth() / 2, getHeight() - buttonHeight, getWidth() / 2, buttonHeight );

            g.setColor( selected == 1 ? selectedButtonColor : buttonColor );
            g.setFont( buttonFont );

            final String no = ResourceManager.getResource( "global.No" );
            final int buttonX = getWidth() - 1 - buttonFont.stringWidth( no );
            g.drawString(
                    no,
                    buttonX,
                    buttonY,
                    Graphics.LEFT | Graphics.BASELINE
            );
        }

        if( !isDoubleBuffered() )
        {
            graphics.drawImage( buffer, 0, 0, Graphics.LEFT | Graphics.TOP );
        }
    }


    protected void keyPressed( int keyCode )
    {
        switch( keyCode )
        {
            case -6:
            case 49:
                result = new Boolean( true );
                break;

            case -7:
            case 51:
                if( dialogType == YES_NO )
                {
                    result = new Boolean( false );
                }
                break;

            default:
                int action = getGameAction( keyCode );
                switch( action )
                {
                    case FIRE:
                        if( dialogType == YES_NO )
                        {
                            result = new Boolean( selected == 0 );
                        }
                        else
                        {
                            result = new Boolean( true );
                        }
                        break;

                    case LEFT:
                    case RIGHT:
                        if( dialogType == YES_NO )
                        {
                            selected++;
                            if( selected > 1 )
                            {
                                selected = 0;
                            }
                            repaint();
                        }
                        break;
                }

        }
    }

    private class Waiter
        extends Thread
    {
        public void run()
        {
            while( result == null )
            {
                try
                {
                    sleep( 500 );
                }
                catch( InterruptedException e )
                {
                }
            }
        }
    }

    /**
     * Utitliy method that displays a confirmation alert.<br/>
     * It shows a modal alert and waits until the user presses yes o no, then
     * the method returns.
     *
     * @return true if the user pressed yes, false if the user pressed no
     * @noinspection FieldRepeatedlyAccessedInMethod
     */
    public boolean confirm()
    {
        final Waiter waiter = new Waiter();
        display.setCurrent( this );
        waiter.start();
        try
        {
            waiter.join();
        }
        catch( InterruptedException e )
        {
        }
        if( current != null )
        {
            display.setCurrent( current );
        }
        return result != null && result.booleanValue();
    }

}
