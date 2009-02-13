package bm.vm;

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

import java.util.Hashtable;
/*
 * File Information
 *
 * Created on       : 05-oct-2007 16:53:07
 * Created by       : narciso
 * Last modified by : $Author$
 * Last modified on : $Date$
 * Revision         : $Revision$
 */

/**
 * Context for execution.<br/>
 * Context can be nested, and each context has it's own set of variables in
 * an environment map.<br/>
 * When a variable is requested from a context, if that variable exists within
 * the context's own environment that is returned, otherwise it's searched
 * through the context chain.
 *
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision$
 */
public class Context
{
    /**
     * Environment.
     */
    protected Hashtable         environment = new Hashtable( 5 );
    protected Hashtable         declared = new Hashtable( 5 );
    protected Context           parent;
    protected VirtualMachine    virtualMachine;

    /**
     * Create an empty context.
     */
    public Context()
    {
    }

    /**
     * Create a context with the given environment.
     *
     * @param environment environment.
     */
    public Context( final Hashtable environment )
    {
        this.environment = environment;
    }

    /**
     * Create a context within a parent context.
     *
     * @param parent parent context
     */
    public Context( final Context parent )
    {
        this.parent = parent;
    }

    /**
     * Create a context within a parent context with the given environment.
     *
     * @param parent parent context
     * @param environment environment
     */
    public Context( final Context parent, final Hashtable environment )
    {
        this.parent = parent;
        this.environment = environment;
    }

    /**
     * Get the VirtualMachine that this context belongs to.
     *
     * @return virtual machine
     */
    public VirtualMachine getVirtualMachine()
    {
        if( virtualMachine != null )
        {
            return virtualMachine;
        }
        else if( parent != null )
        {
            return parent.getVirtualMachine();
        }
        else
        {
            return null;
        }
    }

    /**
     * Declare a variable. Variables must be declared before they can be asigned
     * a value.
     *
     * @param name variable name
     * @param className class name
     * @throws ScriptingClassNotFoundException if class can't be found
     */
    public synchronized void declare( final String name, final String className )
            throws ScriptingClassNotFoundException
    {
        if( !declared.containsKey( name ) )
        {
            final ScriptingClass clazz =
                    getVirtualMachine().getClassLoader().load( className );
            declared.put( name, clazz );
        }
    }

    /**
     * Declare a variable. Variables must be declared before they can be asigned
     * a value.
     *
     * @param name variable name
     * @param clazz class
     */
    public synchronized void declare( final String name, final ScriptingClass clazz )
    {
        if( !declared.containsKey( name ) )
        {
            declared.put( name, clazz );
        }
    }

    /**
     * Get the class of a variable.
     *
     * @param name variable name
     * @return ScriptingClass
     * @throws InvocationException if variable is undeclared
     */
    public ScriptingClass getVariableClass( final String name )
            throws InvocationException
    {
        final Context ctx = getDeclaringContext( name );
        if( ctx != null )
        {
            return (ScriptingClass) ctx.declared.get( name );
        }
        else
        {
            throw new InvocationException( 0, "Undeclared variable: " + name );
        }
    }

    /**
     * Check if a variable is declared within this context.
     *
     * @param name variable name
     * @return true if so
     */
    public boolean isDelcared( final String name )
    {
        return declared.containsKey( name );
    }

    /**
     * Check if a variable is declared in this context or any other context
     * in the hierarchy.
     *
     * @param name name
     * @return true if so
     */
    public boolean contains( final String name )
    {
        return declared.containsKey( name ) ||
               parent != null && parent.contains( name );
    }

    private Context getDeclaringContext( final String name )
    {
        if( declared.containsKey( name ) )
        {
            return this;
        }
        else if( parent != null )
        {
            return parent.getDeclaringContext( name );
        }
        else
        {
            return null;
        }
    }

    /**
     * Set/unset a variable.
     *
     * @param key variable name (case insensitive)
     * @param value value, if null unsets the variable
     * @throws InvocationException on errors
     */
    public synchronized void set( final String key, final Object value )
            throws InvocationException
    {
        if( key != null )
        {
            final Context ctx = getDeclaringContext( key );
            if( ctx != null )
            {
                if( value != null )
                {
                    ctx.environment.put( key, value );
                }
                else
                {
                    ctx.environment.remove( key );
                }
            }
            else
            {
                throw new InvocationException( 0, "Undeclared variable: " + key );
            }
        }
        else
        {
            throw new IllegalArgumentException( "key can't be null" );
        }
    }

    /**
     * Get a variable value.
     *
     * @param key variable name (case insensitive).
     * @return value or null if not defined.
     */
    public Object get( final String key )
    {
        if( key == null )
        {
            throw new IllegalArgumentException( "key can't be null" );
        }
        if( environment.containsKey( key ) )
        {
            return environment.get( key );
        }
        else if( parent != null )
        {
            return parent.get( key );
        }
        else
        {
            return null;
        }
    }
}
