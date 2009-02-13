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
import bm.vm.Context;
import bm.vm.InvocationException;
import bm.core.io.SerializerOutputStream;
import bm.core.io.SerializationException;
import bm.core.io.SerializerInputStream;
import bm.core.CoreConstants;

/*
 * File Information
 *
 * Created on       : 12-oct-2007 1:52:58
 * Created by       : narciso
 * Last modified by : $Author$
 * Last modified on : $Date$
 * Revision         : $Revision$
 */

/**
 * Evaluates a variable or property reference.
 *
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision$
 */
public class Eval
    extends Command
{
    public String   item;

    /**
     * Get the name of the class to be used for serialization/deserialization
     * of complex/nested objects.
     *
     * @return class name
     */
    public String getSerializableClassName()
    {
        return "bm.vm.lang.Eval";
    }

    public String getName()
    {
        return CommandFactory.EVAL;
    }

    /**
     * Serializes command common properties, must be called by subclasses.
     *
     * @param out output stream
     * @throws bm.core.io.SerializationException
     *          on errors
     */
    public synchronized void serialize( SerializerOutputStream out )
            throws SerializationException
    {
        super.serialize( out );
        out.writeByte( (byte) 1 ); // version
        out.writeString( item );
    }

    /**
     * Read command common properties, must be called by subclasses.
     *
     * @param in input stream
     * @throws bm.core.io.SerializationException
     *          on errors
     */
    public synchronized void deserialize( SerializerInputStream in )
            throws SerializationException
    {
        super.deserialize( in );
        in.readByte(); // skip version
        item = in.readString();
    }

    /**
     * Run the command.<br/>
     *
     * @return the value returned by a termination command or null
     * @throws bm.vm.VirtualMachineException on errors
     */
    public Instance run()
            throws VirtualMachineException
    {
        if( item == null || item.equals( "null" ) )
        {
            return null;
        }
        else if( item.equals( "true" ) )
        {
            final Instance instance =
                    getVirtualMachine().newInstance( "boolean" );
            instance.set( "value", CoreConstants.TRUE );
            return instance;
        }
        else if( item.equals( "false" ) )
        {
            final Instance instance =
                    getVirtualMachine().newInstance( "boolean" );
            instance.set( "value", CoreConstants.FALSE );
            return instance;
        }
        else
        {
            final Context context = getContext();
            if( context.contains( item ) )
            {
                return Conversor.toInstance( context, context.get( item ) );
            }
            else
            {
                final Instance thisInstance = (Instance) context.get( "this" );
                if( thisInstance.getScriptingClass().hasProperty( item ) )
                {
                    return Conversor.toInstance(
                            context,
                            thisInstance.get( item )
                    );
                }
                else
                {
                    throw new InvocationException(
                            0,
                            "No variable or property with this name: " + item
                    );
                }
            }
        }
    }
}
