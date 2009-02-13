/*
 * Copyright (c) 2006 Elondra S.L. All Rights Reserved.
 */
package bm.core.tools;
/* -----------------------------------------------------------------------------
    OpenBaseMovil Core Library, foundation of the OpenBaseMovil database and tools
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
import bm.core.ResourceManager;

import java.util.Calendar;
import java.util.Date;
import java.util.TimeZone;

/*
 * File Information
 *
 * Created on       : 24-may-2005 17:02:14
 * Created by       : narciso
 * Last modified by : $Author:narciso $
 * Last modified on : $Date:2007-02-03 19:34:57 +0100 (sáb, 03 feb 2007) $
 * Revision         : $Revision:4 $
 */

/**
 * Localized formatting tools.<br/>
 * Default formats are taken from the ResourceManager using these keys:<br/>
 * date format: <code>format.date</code><br/>
 * time format: <code>format.time</code><br/>
 * <br/>
 * Month names are taken from the ResourceManager using the key
 * <code>format.month.<i>MonthNumber</i></code>
 * where <i>MonthNumber</i> ranges from 1 to 12<br/>
 * Day names are taken from the ResourceManager using the key
 * <code>format.weekday.<i>DayNumber</i></code>
 * where <i>DayNumber</i> ranges from 1 to 7 as returned by
 * Calendar.MONTH, beign 1 Sunday, 2 Monday, etc.<br/>
 * Format is similar to the J2SE SimpleDateFormat:<br/>
 * <table>
 *  <tr>
 *      <th>Literal</th>
 *      <th>Meaning</th>
 *  </tr>
 *  <tr>
 *      <td>d</td>
 *      <td>Day of month (1-31), one digit</td>
 *  </tr>
 *  <tr>
 *      <td>dd</td>
 *      <td>Day of month (01-31), two digits</td>
 *  </tr>
 *  <tr>
 *      <td>ddd</td>
 *      <td>First three letters of day of week</td>
 *  </tr>
 *  <tr>
 *      <td>dddd</td>
 *      <td>Day of week</td>
 *  </tr>
 *  <tr>
 *      <td>M</td>
 *      <td>Month (1-12), one digit</td>
 *  </tr>
 *  <tr>
 *      <td>MM</td>
 *      <td>Month (01-12), two digits</td>
 *  </tr>
 *  <tr>
 *      <td>MMM</td>
 *      <td>First three letter of month name</td>
 *  </tr>
 *  <tr>
 *      <td>MMMM</td>
 *      <td>Month name</td>
 *  </tr>
 *  <tr>
 *      <td>yy</td>
 *      <td>Year, last two digits</td>
 *  </tr>
 *  <tr>
 *      <td>yyyy</td>
 *      <td>Year, with century, two digits</td>
 *  </tr>
 *  <tr>
 *      <td>h</td>
 *      <td>Hour, 0-24, one digit</td>
 *  </tr>
 *  <tr>
 *      <td>hh</td>
 *      <td>Hour, 00-24, two digits</td>
 *  </tr>
 *  <tr>
 *      <td>m</td>
 *      <td>Minute, 0-59, one digit</td>
 *  </tr>
 *  <tr>
 *      <td>mm</td>
 *      <td>Minute, 00-59, two digits</td>
 *  </tr>
 *  <tr>
 *      <td>s</td>
 *      <td>Seconds, 0-59, one digit</td>
 *  </tr>
 *  <tr>
 *      <td>ss</td>
 *      <td>Seconds, 00-59, two digits</td>
 *  </tr>
 * </table>
 * All other characters are interpreted as literals.<br/>
 * Strings surrounded by single quotes are taken as literals.
 *
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision:4 $
 */
public final class LocalFormatTools
{
    private static final char[] KEYS = { 'd', 'M', 'y', 'h', 'm', 's' };
    private static final String[] TOKENS = {
            "d",
            "dd",
            "ddd",
            "dddd",
            "M",
            "MM",
            "MMM",
            "MMMM",
            "yy",
            "yyyy",
            "h",
            "hh",
            "m",
            "mm",
            "s",
            "ss"
    };
    private static final int[] FORMAT = {
            Calendar.DAY_OF_MONTH,
            Calendar.DAY_OF_MONTH,
            Calendar.DAY_OF_WEEK,
            Calendar.DAY_OF_WEEK,
            Calendar.MONTH,
            Calendar.MONTH,
            Calendar.MONTH,
            Calendar.MONTH,
            Calendar.YEAR,
            Calendar.YEAR,
            Calendar.HOUR_OF_DAY,
            Calendar.HOUR_OF_DAY,
            Calendar.MINUTE,
            Calendar.MINUTE,
            Calendar.SECOND,
            Calendar.SECOND
    };

    /**
     * Get a fixed local date, adding to the current time the GMT offset for
     * the default TimeZone.
     * @return local date
     */
    public static Date getLocalDate()
    {
        return new Date(
                System.currentTimeMillis() +
                TimeZone.getDefault().getRawOffset()
        );
    }

    /**
     * Format a given time in milliseconds as a number of hours, minutes and
     * seconds suitable for showing an ellapsed time, i.e. not showing hours
     * and minutes if they're zero.
     * @param milliseconds time ellapsed in milliseconds
     * @param units if true use unit letters (h,m,s), if false just separate using colons
     * @return formatted time string, using two digits for minutes and seconds
     */
    public static String formatEllapsedTime(
            final long      milliseconds,
            final boolean   units
    )
    {
        final long timeInSeconds = milliseconds / 1000;
        long seconds = timeInSeconds % 60;
        long minutes = ((timeInSeconds - seconds) / 60) % 60;
        long hours = timeInSeconds / 3600;

        final StringBuffer buffer = new StringBuffer();

        if( hours > 0 )
        {
            buffer.append( hours ).append( units ? "h " : ":" );
        }
        if( minutes > 0 )
        {
            if( minutes < 10 )
            {
                buffer.append( "0" );
            }
            buffer.append( minutes ).append( units ? "m " : ":" );
        }
        if( seconds < 10 )
        {
            buffer.append( "0" );
        }
        buffer.append( seconds ).append( units ? "s " : ":" );
        return buffer.toString();
    }

    /**
     * Format a date using the given format string, adjusting the time to
     * local time.
     *
     * @param date date
     * @param format format string
     * @return formatted date string
     */
    public static String format( final Date date, final String format )
    {
        return format( date, format, true );
    }

    /**
     * Format a date using the given format string.
     *
     * @param date date
     * @param format format string
     * @param adjustLocalTime adjust the time to the local time
     * @return formatted date string
     */
    public static String format(
            final Date      date,
            final String    format,
            final boolean   adjustLocalTime
    )
    {
        final Calendar cal = Calendar.getInstance();
        if( adjustLocalTime )
        {
            cal.setTime( new Date(
                    date.getTime() + TimeZone.getDefault().getRawOffset()
            ) );
        }
        else
        {
            cal.setTime( date );
        }
        final StringBuffer result = new StringBuffer();
        final StringBuffer token = new StringBuffer();
        final char[] str = format.toCharArray();
        final int length = str.length;
        boolean literal = false;
        for( int i = 0; i < length; i++ )
        {
            if( literal )
            {
                if( str[i] == '\'' )
                {
                    literal = false;
                    result.append( token.toString() );
                    token.delete( 0, token.length() );
                }
                else
                {
                    token.append( str[i] );
                }
            }
            else
            {
                if( str[i] == '\'' )
                {
                    if( token.length() > 0 )
                    {
                        result.append( evaluate( cal, token.toString() ) );
                        token.delete( 0, token.length() );
                    }
                    literal = true;
                }
                else if( isKey( str[i] ) )
                {
                    if( token.length() == 0 || token.charAt( 0 ) == str[i] )
                    {
                        token.append( str[i] );
                    }
                    else
                    {
                        result.append( evaluate( cal, token.toString() ) );
                        token.delete( 0, token.length() );
                        token.append( str[i] );
                    }
                }
                else
                {
                    if( token.length() > 0 )
                    {
                        result.append( evaluate( cal, token.toString() ) );
                        token.delete( 0, token.length() );
                        result.append( str[i] );
                    }
                    else
                    {
                        result.append( str[i] );
                    }
                }
            }
        }
        if( token.length() > 0 )
        {
            if( literal )
            {
                result.append( token );
            }
            else
            {
                result.append( evaluate( cal, token.toString() ) );
            }
        }
        return result.toString();
    }

    /**
     * Format a date as a string using the defaut format. Adjusts the date to
     * local date.
     *
     * @param date date to format
     * @return formatted date
     */
    public static String formatDate( final Date date )
    {
        return format( date, ResourceManager.getResource( "format.date" ) );
    }

    /**
     * Format a date as a time string using the defaut format. Adjust the date
     * to local date.
     *
     * @param date date to format
     * @return formatted time
     */
    public static String formatTime( final Date date )
    {
        return format( date, ResourceManager.getResource( "format.time" ) );
    }

    /**
     * Format a date as a date-time usgin the default formats. Adjusts the date
     * to local date.
     *
     * @param date date
     * @return date time string
     */
    public static String formatTimestamp( final Date date )
    {
        return format(
                date,
                ResourceManager.getResource( "format.date" ) + " " +
                    ResourceManager.getResource( "format.time" )
        );
    }

    private static String evaluate( final Calendar cal, final String token )
    {
        final String[] tokens = TOKENS;
        final int[] format = FORMAT;
        final int count = tokens.length;
        for( int i = 0; i < count; i++ )
        {
            if( tokens[i].equals( token ) )
            {
                int element = cal.get( format[i] );
                String value = Integer.toString( element );
                switch( format[i] )
                {
                    case Calendar.YEAR:
                        if( token.length() == 2 )
                        {
                            element = element % 100;
                            return element < 10 ?
                                    "0" + element :
                                    Integer.toString( element );
                        }
                        else
                        {
                            return value;
                        }

                    case Calendar.MONTH:
                        value = Integer.toString( element + 1 );
                        switch( token.length() )
                        {
                            case 1:
                                return value;

                            case 2:
                                return value.length() == 2 ? value : "0" + value;

                            case 3:
                                return ResourceManager.getResource(
                                        "format.month." + value
                                ).substring( 0, 3 ).toLowerCase();

                            default:
                                return ResourceManager.getResource(
                                        "format.month." + value
                                );
                        }

                    case Calendar.DAY_OF_WEEK:
                        switch( token.length() )
                        {
                            case 3:
                                return ResourceManager.getResource(
                                        "format.weekday." + value
                                ).substring( 0, 3 ).toLowerCase();

                            default:
                                return ResourceManager.getResource(
                                        "format.weekday." + value
                                );
                        }

                    default:
                        return pad( token, value );
                }
            }
        }
        return token;
    }

    private static String pad( final String token, final String value )
    {
        if( token.length() == 1 )
        {
            return value;
        }
        else
        {
            return value.length() == 2 ? value : "0" + value;
        }
    }

    private static boolean isKey( final char c )
    {
        final char[] keys = KEYS;
        final int count = keys.length;
        for( int i = 0; i < count; i++ )
        {
            if( keys[i] == c )
            {
                return true;
            }
        }
        return false;
    }
}
