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

import bm.core.io.Serializable;
import bm.core.io.SerializationException;
import bm.core.io.SerializerInputStream;
import bm.core.io.SerializerOutputStream;
import bm.vm.*;

/*
 * File Information
 *
 * Created on       : 05-oct-2007 16:35:52
 * Created by       : narciso
 * Last modified by : $Author$
 * Last modified on : $Date$
 * Revision         : $Revision$
 */

/**
 * The Command is the base for any script command. This is an abstract class
 * that contains the semantics for command nesting and execution.
 *
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision$
 */
public abstract class Command
    implements Serializable
{
    protected boolean   terminateExcution;

    /**
     * Signals that the block ended through the action of a terminating child,
     * such as a return statement.
     */
    protected transient boolean terminated;

    protected Method method;
    protected Command   parent;
    protected Context   context;

    public abstract String getName();

    /**
     * Serializes command common properties, must be called by subclasses.
     *
     * @param out output stream
     * @throws bm.core.io.SerializationException on errors
     */
    public synchronized void serialize( SerializerOutputStream out )
            throws SerializationException
    {
        out.writeByte( (byte) 1 ); // version
        out.writeBoolean( terminateExcution );
    }

    /**
     * Read command common properties, must be called by subclasses.
     *
     * @param in input stream
     * @throws bm.core.io.SerializationException on errors
     */
    public synchronized void deserialize( SerializerInputStream in )
            throws SerializationException
    {
        in.readByte(); // skip version
        terminateExcution = in.readBoolean();
    }

    /**
     * Get parent command.
     *
     * @return parent
     */
    public Command getParent()
    {
        return parent;
    }

    /**
     * Set parent command.
     *
     * @param parent parent
     */
    public void setParent( final Command parent )
    {
        this.parent = parent;
    }

    VirtualMachine getVirtualMachine()
    {
        final Method method = getMethod();
        if( method != null )
        {
            return method.getClazz().getVirtualMachine();
        }
        else
        {
            return null;
        }
    }

    /**
     * Get owner method.
     *
     * @return method
     */
    public Method getMethod()
    {
        if( method != null )
        {
            return method;
        }
        else if( parent != null )
        {
            return parent.getMethod();
        }
        else
        {
            return null;
        }
    }

    /**
     * Set owner method.
     *
     * @param method method
     */
    public void setMethod( final Method method )
    {
        this.method = method;
    }

    /**
     * Get command context.<br/>
     *
     * @return nearest context in the command chain
     */
    public Context getContext()
    {
        return context != null ?
                   context :
                   parent != null ?
                        parent.getContext() :
                        null;
    }

    /**
     * Set command context.
     *
     * @param context context
     */
    public void setContext( final Context context )
    {
        this.context = context;
    }

    /**
     * Run the command.<br/>
     *
     * @return the value returned by a termination command or null
     * @throws VirtualMachineException on errors
     */
    public abstract Instance run()
            throws VirtualMachineException;

    /**
     * Check if the command comes from a chain of execution termination.
     *
     * @return true if so
     */
    public boolean isTerminated()
    {
        return terminated;
    }
}
