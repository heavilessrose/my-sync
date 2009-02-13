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

import bm.vm.ScriptingClass;
import bm.vm.VirtualMachine;
import bm.vm.Method;
import bm.vm.ScriptingClassNotFoundException;

import java.util.Hashtable;
import java.util.Vector;
/*
 * File Information
 *
 * Created on       : 09-oct-2007 22:49:30
 * Created by       : narciso
 * Last modified by : $Author$
 * Last modified on : $Date$
 * Revision         : $Revision$
 */

/**
 * Decorator to enable construction of classes by compiler.
 *
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision$
 */
public class CompScriptingClass
    extends ScriptingClass
{
    public CompScriptingClass( final VirtualMachine vm )
    {
        super( vm );
    }

    public void setName( final String name )
    {
        this.name = name;
    }

    public void addProperty( final String name, final ScriptingClass clazz )
    {
        if( properties == null )
        {
            properties = new Hashtable( 1 );
        }
        properties.put( name, clazz );
    }

    public void addMethod( final String name, final Method method )
    {
        if( methods == null )
        {
            methods = new Hashtable( 1 );
        }
        methods.put( name, method );
    }

    public void addSuperClass( final String className )
            throws ScriptingClassNotFoundException
    {
        if( ancestors == null )
        {
            ancestors = new Vector( 1 );
        }
        ancestors.addElement( getClassLoader().load( className ) );
    }
}
