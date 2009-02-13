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

import bm.vm.lang.Invocation;
import bm.vm.lang.Expression;

import java.util.Vector;
/*
 * File Information
 *
 * Created on       : 13-oct-2007 2:57:13
 * Created by       : narciso
 * Last modified by : $Author$
 * Last modified on : $Date$
 * Revision         : $Revision$
 */

/**
 * Decorator for compilation.
 *
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision$
 */
public class CompInvocation
    extends Invocation
{
    public void setMethodName( final String methodName )
    {
        this.methodName = methodName;
    }

    public void setTarget( final Expression target )
    {
        this.target = target;
    }

    public void setTargetClass( final String className )
    {
        this.targetClass = className;
    }

    public void addArgument( final Expression argument )
    {
        if( args == null )
        {
            args = new Vector( 1 );
        }
        args.addElement( argument );
    }
}
