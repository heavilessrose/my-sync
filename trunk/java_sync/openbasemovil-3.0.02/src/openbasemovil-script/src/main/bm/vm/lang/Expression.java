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

import bm.core.io.SerializerOutputStream;
import bm.core.io.SerializationException;
import bm.core.io.SerializerInputStream;
import bm.vm.Instance;
import bm.vm.Constants;
import bm.vm.VirtualMachineException;
/*
 * File Information
 *
 * Created on       : 05-oct-2007 17:37:16
 * Created by       : narciso
 * Last modified by : $Author$
 * Last modified on : $Date$
 * Revision         : $Revision$
 */

/**
 * An expression is a command that can have a fixed value or return a value
 * that comes from the execution of it's children.
 *
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision$
 */
public class Expression
    extends Command
{
    protected Instance      value;
    protected Command       command;

    /**
     * Get the name of the class to be used for serialization/deserialization
     * of complex/nested objects.
     *
     * @return class name
     */
    public String getSerializableClassName()
    {
        return "bm.vm.lang.Expression";
    }

    public String getName()
    {
        return CommandFactory.EXPR;
    }

    /**
     * Serializes command.
     *
     * @param out output stream
     * @throws bm.core.io.SerializationException on errors
     */
    public synchronized void serialize( SerializerOutputStream out )
            throws SerializationException
    {
        super.serialize( out );
        out.writeByte( (byte) 1 ); // version
        out.writeBoolean( value == null );
        if( value != null )
        {
            out.writeString( value.getScriptingClass().getName() );
            value.serialize( out );
        }
        out.writeBoolean( command == null );
        if( command != null )
        {
            out.writeString( command.getName() );
            command.serialize( out );
        }
    }

    /**
     * Read command.
     *
     * @param in input stream
     * @throws bm.core.io.SerializationException on errors
     */
    public synchronized void deserialize( SerializerInputStream in )
            throws SerializationException
    {
        super.deserialize( in );
        in.readByte(); // skip version
        boolean isNull = in.readBoolean();
        if( !isNull )
        {
            final String className = in.readString();
            try
            {
                value = getVirtualMachine().newInstance( className );
            }
            catch( VirtualMachineException e )
            {
                throw new SerializationException(
                        Constants.ERR_EXPR_CLASS_NOT_FOUND,
                        e
                );
            }
            value.deserialize( in );
        }
        else
        {
            value = null;
        }
        isNull = in.readBoolean();
        if( !isNull )
        {
            try
            {
                command = CommandFactory.createCommand( in.readString() );
                command.setParent( this );
                command.deserialize( in );
            }
            catch( Exception e )
            {
                throw new SerializationException(
                        Constants.ERR_EXPR_CREATE_COMMAND,
                        e
                );
            }
        }
        else
        {
            command = null;
        }
    }

    /**
     * Run the command.<br/>
     * The default implementation runs children sequentially. If a child
     * that terminates execution is found, the value returned by the run method
     * of that child is immedately returned. Otherwise, the method returns null
     * when all children have been executed.
     *
     * @return the value returned by a termination children or null
     */
    public Instance run()
            throws VirtualMachineException
    {
        if( command != null )
        {
            command.setContext( getContext() );
            return command.run();
        }
        else
        {
            return value;
        }
    }
}
