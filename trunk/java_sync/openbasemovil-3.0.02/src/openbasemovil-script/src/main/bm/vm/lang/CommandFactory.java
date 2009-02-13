package bm.vm.lang;

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

import java.util.Hashtable;
/*
 * File Information
 *
 * Created on       : 11-oct-2007 0:12:55
 * Created by       : narciso
 * Last modified by : $Author$
 * Last modified on : $Date$
 * Revision         : $Revision$
 */

/**
 * Creates command in a safe way even when obfuscated.
 *
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision$
 */
public class CommandFactory
{
    public static final String AND          = "&";
    public static final String BLOCK        = "{";
    public static final String DIV          = "/";
    public static final String COMP         = ">";
    public static final String EVAL         = "$";
    public static final String EXPR         = "%";
    public static final String IF           = "?";
    public static final String INVOCATION   = ".";
    public static final String MINUS        = "-";
    public static final String MULT         = "*";
    public static final String NOT          = "!";
    public static final String OR           = "|";
    public static final String PLUS         = "+";
    public static final String RETURN       = "<";
    public static final String SET          = "=";
    public static final String VAR          = "#";
    public static final String WHILE        = "(";
    public static final String GET          = "_";
    public static final String NEW          = "N";

    private static final Hashtable classes = new Hashtable( 13 );
    static
    {
        addCommand( new And() );
        addCommand( new Block() );
        addCommand( new Div() );
        addCommand( new Comparator( (byte) 0 ) );
        addCommand( new Eval() );
        addCommand( new Expression() );
        addCommand( new If() );
        addCommand( new Invocation() );
        addCommand( new Minus() );
        addCommand( new Mult() );
        addCommand( new Not() );
        addCommand( new Or() );
        addCommand( new Plus() );
        addCommand( new Return() );
        addCommand( new Set() );
        addCommand( new Var() );
        addCommand( new While() );
        addCommand( new Get() );
        addCommand( new New() );
    }

    private static void addCommand( final Command command )
    {
        classes.put( command.getName(), command.getClass() );
    }

    public static Command createCommand( final String name )
            throws IllegalAccessException,
                   InstantiationException
    {
        final Class clazz = (Class) classes.get( name );
        if( clazz != null )
        {
            return (Command) clazz.newInstance();
        }
        else
        {
            throw new InstantiationException( "Invalid command" );
        }
    }
}
