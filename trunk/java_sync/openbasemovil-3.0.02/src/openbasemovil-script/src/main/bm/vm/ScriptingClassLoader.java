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

import bm.core.io.SerializationException;
import bm.core.io.SerializerInputStream;
import bm.vm.sys.Map;
import bm.vm.sys.ScriptingObject;
import bm.vm.sys.VMSystem;
import bm.vm.sys.Void;

import java.io.InputStream;
import java.util.Hashtable;
import java.util.Vector;
/*
 * File Information
 *
 * Created on       : 07-oct-2007 1:06:45
 * Created by       : narciso
 * Last modified by : $Author$
 * Last modified on : $Date$
 * Revision         : $Revision$
 */

/**
 * Loads classes from a certain storage.<br/>
 * This default implementation can be substituted at runtime with other class
 * loaders.<br/>
 * This default implementation looks for classes as resources bundled with the
 * .jar file, stored in a folder called <i>vm</i> located at the root directory
 * of the .jar file.<br/>
 * A subclass of this default loader MUST always default to this class load
 * method when it can not find a class, as this class has also the ability of
 * loading system classes.
 *
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision$
 */
public class ScriptingClassLoader
{
    /**
     * Some classes can be accessed both by name or by alias.
     */
    private Hashtable aliases;
    /**
     * System classes, like int, double, etc.
     */
    private Hashtable systemClasses;
    /**
     * Cache of already loaded real scripting clasess.
     */
    private Hashtable loadedClasses = new Hashtable( 10 );

    /**
     * Clasess that are loading now, for managing dependencies.
     */
    private Vector loading = new Vector( 10 );
    /**
     * Proxies for classes that are currently loading, to manage cyclic
     * dependencies of classes provinding proxies that are resolved when both
     * classes are fully loaded.
     */
    private Hashtable proxies = new Hashtable( 10 );

    /**
     * The virtual machine for this class loader.
     */
    protected VirtualMachine vm;

    /**
     * Create a new class loader for a virtual machine.
     *
     * @param vm virtual machine
     */
    public ScriptingClassLoader( final VirtualMachine vm )
    {
        this.vm = vm;
        systemClasses = new Hashtable( 9 );
        addSystemClass( "boolean" );
        addSystemClass( "String" );
        addSystemClass( "byte" );
        addSystemClass( "short" );
        addSystemClass( "int" );
        addSystemClass( "long" );
        addSystemClass( "double" );
        systemClasses.put( "map", new Map( vm ) );
        systemClasses.put( "system", new VMSystem( vm ) );
        systemClasses.put( "object", new ScriptingObject( vm ) );
        systemClasses.put( "void", new Void( vm ) );
        aliases = new Hashtable( 9 );
        aliases.put( "sys.Boolean", "boolean" );
        aliases.put( "sys.String", "String" );
        aliases.put( "sys.Byte", "byte" );
        aliases.put( "sys.Short", "short" );
        aliases.put( "sys.Integer", "int" );
        aliases.put( "sys.Long", "long" );
        aliases.put( "sys.Double", "double" );
        aliases.put( "sys.Map", "map" );
        aliases.put( "sys.System", "system" );
        aliases.put( "sys.Object", "object" );
        aliases.put( "sys.Void", "void" );
    }

    /**
     * Add a native class to the class loader.
     *
     * @param name class name
     * @param clazz class
     */
    public void addNativeClass( final String name, final NativeClass clazz )
    {
        systemClasses.put( name, clazz );
    }

    /**
     * Remove a native class from the class loader.
     *
     * @param name class name
     */
    public void removeNativeClass( final String name )
    {
        Object clazz = systemClasses.get( name );
        if( clazz != null && clazz instanceof NativeClass )
        {
            systemClasses.remove( name );
        }
    }

    /**
     * Get a class given its name or alias.
     *
     * @param name name or alias
     * @return class
     * @throws ScriptingClassNotFoundException if the class cant be found
     */
    public ScriptingClass load( final String name )
            throws ScriptingClassNotFoundException
    {
        if( aliases.containsKey( name ) )
        {
            return (ScriptingClass) systemClasses.get( aliases.get( name ) );
        }
        else if( systemClasses.containsKey( name ) )
        {
            return (ScriptingClass) systemClasses.get( name );
        }
        else if( loadedClasses.containsKey( name ) )
        {
            return (ScriptingClass) loadedClasses.get( name );
        }
        else
        {
            return loadNonSystemClass( name );
        }
    }

    /**
     * Get the alias for a given class name.
     *
     * @param name class name
     * @return alias or null if class has no alias
     */
    public String getAlias( final String name )
    {
        return (String) aliases.get( name );
    }

    /**
     * Load a class from an input stream.
     *
     * @param is input stream
     * @param name class name
     * @return the class
     * @throws SerializationException on errors loading class
     */
    protected ScriptingClass load( final InputStream is, final String name )
            throws SerializationException
    {
        startLoad( name );
        final SerializerInputStream in = new SerializerInputStream( is );
        final ScriptingClass clazz = new ScriptingClass( vm );
        clazz.deserialize( in );
        if( proxies.containsKey( name ) )
        {
            final ScriptingClassProxy proxy =
                    (ScriptingClassProxy) proxies.get( name );
            proxy.setClazz( clazz );
        }
        loadedClasses.put( name, clazz );
        endLoad( name );
        return clazz;
    }

    /**
     * Starts loading a class.
     *
     * @param name class name
     */
    protected void startLoad( final String name )
    {
        if( !loading.contains( name ) )
        {
            loading.addElement( name );
        }
    }

    /**
     * End loading a class. Removes also the proxy if it was needed.
     *
     * @param name class name
     */
    protected synchronized void endLoad( final String name )
    {
        loading.removeElement( name );
        if( proxies.containsKey( name ) )
        {
            proxies.remove( name );
        }
    }

    /**
     * Check if a class is loading.
     *
     * @param name class name
     * @return true if so
     */
    protected boolean isLoading( final String name )
    {
        return loading.contains( name );
    }

    /**
     * Get the virtual machine for this class loader.
     *
     * @return virtual machine
     */
    public VirtualMachine getVirtualMachine()
    {
        return vm;
    }

    private void addSystemClass( final String name )
    {
        final ScriptingClass sysClass = new ScriptingClass( vm );
        sysClass.name = name;
        sysClass.systemClass = true;
        sysClass.properties = new Hashtable( 1 );
        sysClass.properties.put( "value", sysClass );
        systemClasses.put( sysClass.name, sysClass );
    }

    private ScriptingClass loadNonSystemClass( final String name )
            throws ScriptingClassNotFoundException
    {
        if( isLoading( name ) )
        {
            ScriptingClassProxy proxy = (ScriptingClassProxy) proxies.get( name );
            if( proxy == null )
            {
                proxy = new ScriptingClassProxy( vm );
                proxies.put( name, proxy );
            }
            return proxy;
        }
        else
        {
            InputStream is = null;
            try
            {
                is = getClass().getResourceAsStream( "/vm/" + name + ".bmc" );
                if( is != null )
                {
                    return load( is, name );
                }
                else
                {
                    throw new ScriptingClassNotFoundException( 0, name );
                }
            }
            catch( Exception e )
            {
                throw new ScriptingClassNotFoundException( 0, name );
            }
            finally
            {
                if( is != null ) try{ is.close(); }catch( Exception e ){}
                endLoad( name );
            }
        }
    }

}
