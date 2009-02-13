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

import bm.vm.Instance;
import bm.vm.VirtualMachineException;
import bm.vm.ScriptingClassNotFoundException;
import bm.core.CoreConstants;
/*
 * File Information
 *
 * Created on       : 05-oct-2007 17:46:17
 * Created by       : narciso
 * Last modified by : $Author$
 * Last modified on : $Date$
 * Revision         : $Revision$
 */

/**
 * Perform a logical negation.
 *
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision$
 */
public class Not
    extends UnaryOperator
{

    public String getName()
    {
        return CommandFactory.NOT;
    }

    /**
     * Get the name of the class to be used for serialization/deserialization
     * of complex/nested objects.
     *
     * @return class name
     */
    public String getSerializableClassName()
    {
        return "bm.vm.lang.Not";
    }

    /**
     * Run the command.<br/>
     *
     * @return the value returned by a termination children or null
     * @throws bm.vm.VirtualMachineException on errors
     */
    public Instance run()
            throws VirtualMachineException
    {
        if( operand1 == null )
        {
            throw new IllegalArgumentException( "Operator needs exactly one argument" );
        }
        else
        {
            operand1.setContext( getContext() );
            Boolean value = Conversor.toBoolean( operand1.run() ) ?
                                CoreConstants.FALSE :
                                CoreConstants.TRUE;
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
