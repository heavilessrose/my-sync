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

import bm.core.CoreConstants;
import bm.vm.Instance;
import bm.vm.ScriptingClassNotFoundException;
import bm.vm.VirtualMachineException;
import bm.vm.Context;
/*
 * File Information
 *
 * Created on       : 05-oct-2007 17:45:18
 * Created by       : narciso
 * Last modified by : $Author$
 * Last modified on : $Date$
 * Revision         : $Revision$
 */

/**
 * Performs a logical or of two operands, using a lazy evaluation algorithm.
 *
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision$
 */
public class Or
    extends BinaryOperator
{

    public String getName()
    {
        return CommandFactory.OR;
    }

    /**
     * Get the name of the class to be used for serialization/deserialization
     * of complex/nested objects.
     *
     * @return class name
     */
    public String getSerializableClassName()
    {
        return "bm.vm.lang.Or";
    }

    /**
     * Run the command.<br/>
     *
     * @return the value returned by a termination children or null
     * @throws VirtualMachineException on errors
     */
    public Instance run()
            throws VirtualMachineException
    {
        if( operand1 == null || operand2 == null )
        {
            throw new IllegalArgumentException( "And needs exactly two arguments" );
        }
        else
        {
            final Context context = getContext();
            operand1.setContext( context );
            operand2.setContext( context );
            Boolean value;
            if( Conversor.toBoolean( operand1.run() ) )
            {
                value = CoreConstants.TRUE;
            }
            else
            {
                value = Conversor.toBoolean( operand2.run() ) ?
                        CoreConstants.TRUE :
                        CoreConstants.FALSE;
            }
            Instance instance = null;
            try
            {
                instance = getVirtualMachine().newInstance( "boolean" );
                instance.set( "value", value );
            }
            catch( ScriptingClassNotFoundException e )
            {
            }
            return instance;
        }
    }
}
