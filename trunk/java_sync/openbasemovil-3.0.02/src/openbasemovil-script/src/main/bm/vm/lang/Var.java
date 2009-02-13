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

import bm.vm.*;
import bm.core.io.SerializerOutputStream;
import bm.core.io.SerializationException;
import bm.core.io.SerializerInputStream;
/*
 * File Information
 *
 * Created on       : 09-oct-2007 23:38:59
 * Created by       : narciso
 * Last modified by : $Author$
 * Last modified on : $Date$
 * Revision         : $Revision$
 */

/**
 * Variable declaration.
 *
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision$
 */
public class Var
    extends Command
{
    protected String    name;
    protected String    className;

    public String getName()
    {
        return CommandFactory.VAR;
    }

    /**
     * Get the name of the class to be used for serialization/deserialization
     * of complex/nested objects.
     *
     * @return class name
     */
    public String getSerializableClassName()
    {
        return "bm.vm.lang.Var";
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
        out.writeString( name );
        out.writeString( className );
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
        name = in.readString();
        className = in.readString();
    }

    /**
     * Run the command.<br/>
     * The default implementation runs children sequentially. If a child
     * that terminates execution is found, the value returned by the run method
     * of that child is immedately returned. Otherwise, the method returns null
     * when all children have been executed.
     *
     * @return the value returned by a termination children or null
     * @throws VirtualMachineException on errors
     */
    public Instance run()
            throws VirtualMachineException
    {
        getContext().declare( name, className );
        return null;
    }
}
