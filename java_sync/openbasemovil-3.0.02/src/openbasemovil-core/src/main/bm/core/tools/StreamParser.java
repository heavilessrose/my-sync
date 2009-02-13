package bm.core.tools;

/* -----------------------------------------------------------------------------
    bmScript Scripting language for Mobile Devices
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

import java.io.Reader;
import java.io.IOException;
/*
 * File Information
 *
 * Created on       : 19-oct-2007 14:25:57
 * Created by       : narciso
 * Last modified by : $Author$
 * Last modified on : $Date$
 * Revision         : $Revision$
 */

/**
 * The StreamParser reads tokens from a stream, but in such a way that every
 * new token read can be delimited by different characters. The parser also
 * supports comments in the stream at the user request.
 *
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision$
 */
public class StreamParser
{
    public static final String WHITESPACE = "\r\n\t ";

    private static final byte   INITIAL         = 0;
    private static final byte   COMMENT_START   = 1;
    private static final byte   LINE_COMMENT    = 2;
    private static final byte   BLOCK_COMMENT   = 3;
    private static final byte   COMMENT_END     = 4;
    private static final byte   ESCAPE          = 5;

    private PushBackReader in;
    private int            startLine;
    private int            startChar;
    private int            endLine;
    private int            endChar;
    private char           lastDelimiter;

    public StreamParser( final Reader in )
    {
        this.in = in instanceof PushBackReader ?
                  (PushBackReader) in :
                  new PushBackReader( in );
    }

    public int getStartLine()
    {
        return startLine;
    }

    public int getStartChar()
    {
        return startChar;
    }

    public int getEndLine()
    {
        return endLine;
    }

    public int getEndChar()
    {
        return endChar;
    }

    public char getLastDelimiter()
    {
        return lastDelimiter;
    }

    public int nextChar()
            throws ParserException
    {
        int c;
        try
        {
            c = in.read();
        }
        catch( IOException e )
        {
            throw new ParserException(
                    "Error reading input",
                    startLine,
                    startChar
            );
        }
        endChar++;
        startChar = endChar;
        if( c == '\n' )
        {
            endLine++;
            startLine = endLine;
            startChar = endChar = 0;
        }
        return c;
    }

    /**
     * Read the next token from the stream, using WHITESPACE as delimiters,
     * stripping whitespace, not allowing comments and not reusing delimiters.
     *
     * @return token
     * @throws ParserException on errors
     */
    public String next()
            throws ParserException
    {
        return next( WHITESPACE, false, false, false );
    }

    public String next(
            final String    delimiters,
            final boolean   keepWhitespace,
            final boolean   allowComments,
            final boolean   reuseDelimiter
    )
            throws ParserException
    {
        return next(
                delimiters,
                keepWhitespace,
                allowComments,
                reuseDelimiter,
                false
        );
    }

    public String next(
            final String    delimiters,
            final boolean   keepWhitespace,
            final boolean   allowComments,
            final boolean   reuseDelimiter,
            final boolean   processEscape
    )
            throws ParserException
    {
        try
        {
            final StringBuffer token = new StringBuffer();
            startLine = endLine;
            startChar = endChar;
            int c = in.read();
            endChar++;
            int status = INITIAL;
            while( c != -1 )
            {
                if( c == '\n' )
                {
                    endLine++;
                    endChar = 0;
                }
                switch( status )
                {
                    case INITIAL:
                        if( delimiters.indexOf( c ) > -1 )
                        {
                            lastDelimiter = (char) c;
                            if( isWhiteSpace( c ) )
                            {
                                if( token.length() > 0 )
                                {
                                    if( reuseDelimiter )
                                    {
                                        in.revert( (char) c );
                                    }
                                    return token.toString();
                                }
                            }
                            else
                            {
                                if( reuseDelimiter )
                                {
                                    in.revert( (char) c );
                                }
                                return token.toString();
                            }
//                            if( token.length() > 0 )
//                            {
//                                if( reuseDelimiter )
//                                {
//                                    in.revert( (char) c );
//                                }
//                                return token.toString();
//                            }
//                            else if( isWhiteSpace( c ) && keepWhitespace )
//                            {
//                                token.append( (char) c );
//                            }
                        }
                        else if( processEscape && c == '\\' )
                        {
                            status = ESCAPE;
                        }
                        else if( allowComments && c == '/' )
                        {
                            status = COMMENT_START;
                        }
                        else if( isWhiteSpace( c ) )
                        {
                            if( keepWhitespace )
                            {
                                token.append( (char) c );
                            }
                        }
                        else
                        {
                            token.append( (char) c );
                        }
                        break;

                    case ESCAPE:
                        switch( c )
                        {
                            case 'n':
                                token.append( '\n' );
                                break;

                            case 'r':
                                token.append( '\r' );
                                break;

                            case 't':
                                token.append( '\t' );
                                break;

                            case 'b':
                                token.append( '\b' );
                                break;

                            case 'f':
                                token.append( '\f' );
                                break;

                            default:
                                token.append( (char) c );
                                break;
                        }
                        status = INITIAL;
                        break;

                    case COMMENT_START:
                        if( c == '/' )
                        {
                            status = LINE_COMMENT;
                        }
                        else if( c == '*' )
                        {
                            status = BLOCK_COMMENT;
                        }
                        else
                        {
                            status = INITIAL;
                            token.append( '/' ).append( (char) c );
                        }
                        break;

                    case LINE_COMMENT:
                        if( c == '\n' )
                        {
                            status = INITIAL;
                        }
                        break;

                    case BLOCK_COMMENT:
                        if( c == '*' )
                        {
                            status = COMMENT_END;
                        }
                        break;

                    case COMMENT_END:
                        if( c == '/' )
                        {
                            status = INITIAL;
                        }
                        else
                        {
                            status = BLOCK_COMMENT;
                        }
                        break;

                }
                c = in.read();
            }
            return token.length() > 0 ? token.toString() : null;
        }
        catch( IOException e )
        {
            throw new ParserException(
                    "Error reading input",
                    startLine,
                    startChar
            );
        }
    }

    public static boolean isValidIdentifier( final String s )
    {
        if( s == null || s.trim().length() == 0 )
        {
            return false;
        }
        if( !isIdentifierStart( s.charAt( 0 ) ) )
        {
            return false;
        }
        final int length = s.length();
        for( int i = 1; i < length; i++ )
        {
            if( !isIdentifierPart( s.charAt( i ) ) )
            {
                return false;
            }
        }
        return true;
    }

    public static boolean isIdentifierPart( final char c )
    {
        return isLowerCase( c ) || isUpperCase( c ) || c == '$' ||
               isDigit( c ) || c == '-' || c == '_';
    }

    public static boolean isDigit( final char c )
    {
        return c >= '0' && c <= '9';
    }

    public static boolean isIdentifierStart( final char c )
    {
        return isLowerCase( c ) || isUpperCase( c ) || c == '_';
    }

    public static boolean isLowerCase( final char c )
    {
        return c >= 'a' && c <= 'z';
    }

    public static boolean isUpperCase( final char c )
    {
        return c >= 'A' && c <= 'Z';
    }

    public static boolean isWhiteSpace( final int c )
    {
        return WHITESPACE.indexOf( c ) > -1;
    }

    public static boolean isWhiteSpace( final char c )
    {
        return WHITESPACE.indexOf( c ) > -1;
    }

    public void pushback( final int c )
    {
        in.revert( (char) c );
        if( c == '\n' )
        {
            endLine--;
            endChar = startChar;
        }
        else
        {
            endChar--;
        }
    }

    public void pushback( final String token )
    {
        if( token != null && token.length() > 0 )
        {
            final char[] chars = token.toCharArray();
            final int length = chars.length;
            for( int i = length - 1; i >= 0; i-- )
            {
                pushback( chars[i] );
            }
        }
    }

    public void skipWhiteSpace()
            throws ParserException
    {
        int c = nextChar();
        while( c != -1 )
        {
            if( !isWhiteSpace( c ) )
            {
                in.revert( (char) c );
                return;
            }
            else
            {
                c = nextChar();
            }
        }
        throw new ParserException(
                "Unexpected end of file",
                getStartLine(),
                getStartChar()
        );
    }

    public void searchNext( final char chr )
            throws ParserException
    {
        int c = nextChar();
        while( c != -1 )
        {
            if( c == chr )
            {
                break;
            }
            else if( !isWhiteSpace( c ) )
            {
                throw new ParserException(
                        "Unexpected character: " + ((char) c),
                        getStartLine(),
                        getStartChar()
                );
            }
            else
            {
                c = nextChar();
            }
        }
        if( c == -1 )
        {
            throw new ParserException(
                    "Unexpected end of file",
                    getStartLine(),
                    getStartChar()
            );
        }
    }

    public char searchNextChar( final String str )
            throws ParserException
    {
        int c = nextChar();
        while( c != -1 )
        {
            if( str.indexOf( c ) > -1 )
            {
                return (char) c;
            }
            else if( !isWhiteSpace( c ) )
            {
                throw new ParserException(
                        "Unexpected character: " + ((char) c),
                        getStartLine(),
                        getStartChar()
                );
            }
            else
            {
                c = nextChar();
            }
        }
        throw new ParserException(
                "Unexpected end of file",
                getStartLine(),
                getStartChar()
        );
    }

    public int getLastChar()
    {
        return in.getLastChar();
    }
}
