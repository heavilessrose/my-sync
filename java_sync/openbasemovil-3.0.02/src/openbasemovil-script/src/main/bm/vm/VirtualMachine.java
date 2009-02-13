package bm.vm;

import bm.core.io.SerializableFactory;
import bm.vm.lang.*;

import java.util.Vector;

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

/*
 * File Information
 *
 * Created on       : 07-oct-2007 1:26:39
 * Created by       : narciso
 * Last modified by : $Author$
 * Last modified on : $Date$
 * Revision         : $Revision$
 */

/**
 * The BaseMovil Script Virtual Machine.
 *
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision$
 */
public class VirtualMachine
{
    private ScriptingClassLoader    classLoader = new ScriptingClassLoader( this );
    private Context                 context     = new Context();

    /**
     * Registers serializable classes in the virtual machine library into the
     * SerializableFactory.
     */
    public static void registerSerializableClasses()
    {
        SerializableFactory.register( "bm.vm.lang.And", new And().getClass() );
        SerializableFactory.register( "bm.vm.lang.Block", new Block().getClass() );
        SerializableFactory.register( "bm.vm.lang.Comparator", new Comparator().getClass() );
        SerializableFactory.register( "bm.vm.lang.Div", new Div().getClass() );
        SerializableFactory.register( "bm.vm.lang.Eval", new Eval().getClass() );
        SerializableFactory.register( "bm.vm.lang.Expression", new Expression().getClass() );
        SerializableFactory.register( "bm.vm.lang.Get", new Get().getClass() );
        SerializableFactory.register( "bm.vm.lang.If", new If().getClass() );
        SerializableFactory.register( "bm.vm.lang.Invocation", new Invocation().getClass() );
        SerializableFactory.register( "bm.vm.lang.Minus", new Minus().getClass() );
        SerializableFactory.register( "bm.vm.lang.Mult", new Mult().getClass() );
        SerializableFactory.register( "bm.vm.lang.New", new New().getClass() );
        SerializableFactory.register( "bm.vm.lang.Not", new Not().getClass() );
        SerializableFactory.register( "bm.vm.lang.Or", new Or().getClass() );
        SerializableFactory.register( "bm.vm.lang.Plus", new Plus().getClass() );
        SerializableFactory.register( "bm.vm.lang.Return", new Return().getClass() );
        SerializableFactory.register( "bm.vm.lang.Set", new Set().getClass() );
        SerializableFactory.register( "bm.vm.lang.Var", new Var().getClass() );
        SerializableFactory.register( "bm.vm.lang.While", new While().getClass() );
    }

    /**
     * Create a virtual machine with the default class loader.
     */
    public VirtualMachine()
    {
        context.virtualMachine = this;
        try
        {
            context.declare( "system", "system" );
            context.set( "system", newInstance( "system" ) );
        }
        catch( VirtualMachineException e )
        {
        }
    }

    /**
     * Create a virtual machine with a custom class loader.
     *
     * @param classLoader class loader
     */
    public VirtualMachine( final ScriptingClassLoader classLoader )
    {
        this();
        this.classLoader = classLoader;
    }

    /**
     * Get this virtual machine class loader.
     *
     * @return class loader
     */
    public ScriptingClassLoader getClassLoader()
    {
        return classLoader;
    }

    /**
     * Set the class loader.
     *
     * @param classLoader class loader
     */
    public void setClassLoader( final ScriptingClassLoader classLoader )
    {
        this.classLoader = classLoader;
    }

    /**
     * Get the vm context.
     *
     * @return context
     */
    public Context getContext()
    {
        return context;
    }

    /**
     * Set the vm context.
     *
     * @param context context
     */
    public void setContext( final Context context )
    {
        this.context = context;
        context.virtualMachine = this;
        if( !context.contains( "system" ) )
        {
            try
            {
                context.set( "system", newInstance( "system" ) );
            }
            catch( VirtualMachineException e )
            {
            }
        }
    }

    /**
     * Runs a given class by invoking its <i>main</i> method.
     *
     * @param className class name
     * @return the value returned by the main method
     * @throws ScriptingClassNotFoundException if the class can't be found
     * @throws InvocationException if the main method throws an exception
     */
    public Instance run( final String className )
            throws VirtualMachineException
    {
        return newInstance( className ).invoke( "main", null );
    }

    public Instance newInstance( final String className )
            throws VirtualMachineException
    {
        final ScriptingClass clazz = classLoader.load( className );
        final Instance instance = new Instance( clazz, this );
        invokeConstructors( clazz, instance );
        return instance;
    }

    private void invokeConstructors(
            final ScriptingClass clazz,
            final Instance instance
    )
            throws VirtualMachineException
    {
        final Method constructor = clazz.getMethod( clazz.getName() );
        if( constructor != null )
        {
            constructor.invoke( instance, null );
        }
        final Vector ancestors = clazz.getAncestors();
        if( ancestors != null )
        {
            final int size = ancestors.size();
            for( int i = 0; i < size; i++ )
            {
                final ScriptingClass ancestor = (ScriptingClass)
                        ancestors.elementAt( i );
                invokeConstructors( ancestor, instance );
            }
        }
    }
}
