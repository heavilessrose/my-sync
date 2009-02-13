package bm.vm.comp;

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

import bm.vm.MethodImpl;
import bm.vm.ScriptingClass;
import bm.vm.lang.Block;

import java.util.Vector;
/*
 * File Information
 *
 * Created on       : 09-oct-2007 23:20:44
 * Created by       : narciso
 * Last modified by : $Author$
 * Last modified on : $Date$
 * Revision         : $Revision$
 */

/**
 * Method decorator for compilation.
 *
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision$
 */
public class CompMethod
    extends MethodImpl
{
    public CompMethod( final ScriptingClass clazz )
    {
        super( clazz );
    }

    public void addArgument( final String name, final ScriptingClass argClass )
    {
        if( argNames == null )
        {
            argNames = new Vector( 10 );
            argClasses = new Vector( 10 );
        }
        argNames.addElement( name );
        argClasses.addElement( argClass );
    }

    public void setBody( final Block body )
    {
        this.body = body;
    }
}
