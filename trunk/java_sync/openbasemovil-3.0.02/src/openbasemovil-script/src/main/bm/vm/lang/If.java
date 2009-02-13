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
import bm.vm.VirtualMachineException;
import bm.vm.Context;
/*
 * File Information
 *
 * Created on       : 05-oct-2007 17:44:12
 * Created by       : narciso
 * Last modified by : $Author$
 * Last modified on : $Date$
 * Revision         : $Revision$
 */

/**
 * An if/else command.
 *
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision$
 */
public class If
    extends Command
{
    protected Block       ifBranch;
    protected Block       elseBranch;
    protected Expression  condition;

    /**
     * Get the name of the class to be used for serialization/deserialization
     * of complex/nested objects.
     *
     * @return class name
     */
    public String getSerializableClassName()
    {
        return "bm.vm.lang.If";
    }

    public String getName()
    {
        return CommandFactory.IF;
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
        out.writeByte( (byte) 1 ); // Version
        ifBranch.serialize( out );
        out.writeBoolean( elseBranch == null );
        if( elseBranch != null )
        {
            elseBranch.serialize( out );
        }
        condition.serialize( out );
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
        ifBranch = new Block();
        ifBranch.setParent( this );
        ifBranch.deserialize( in );
        final boolean isNull = in.readBoolean();
        if( !isNull )
        {
            elseBranch = new Block();
            elseBranch.setParent( this );
            elseBranch.deserialize( in );
        }
        else
        {
            elseBranch = null;
        }
        condition =  new Expression();
        condition.setParent( this );
        condition.deserialize( in );
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
        final Context context = getContext();
        condition.setContext( context );
        final Instance cond = condition.run();
        Instance retval = null;
        if( cond != null && Conversor.toBoolean( cond ) )
        {
            ifBranch.setContext( context );
            retval = ifBranch.run();
            terminated = ifBranch.isTerminated();
        }
        else if( elseBranch != null )
        {
            elseBranch.setContext( context );
            retval = elseBranch.run();
            terminated = elseBranch.isTerminated();
        }
        return retval;
    }
}
