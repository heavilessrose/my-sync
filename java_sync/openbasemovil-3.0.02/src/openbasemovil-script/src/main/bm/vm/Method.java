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

import bm.core.io.Serializable;
/*
 * File Information
 *
 * Created on       : 23-oct-2007 14:22:41
 * Created by       : narciso
 * Last modified by : $Author$
 * Last modified on : $Date$
 * Revision         : $Revision$
 */

/**
 * A method in a class.
 *
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision$
 */
public interface Method
        extends Serializable
{
    /**
     * Get the class to which this method belongs.
     * @return scripting class
     */
    ScriptingClass getClazz();

    /**
     * Set the class to which this method belongs.
     *
     * @param clazz scripting class
     */
    void setClazz( ScriptingClass clazz );

    /**
     * Invoke the method.
     *
     * @param target target instance on which to run
     * @param args method arguments
     * @return return value
     * @throws VirtualMachineException on errors
     */
    Instance invoke(
            Instance      target,
            Instance[]    args
    )
            throws VirtualMachineException;

    /**
     * Get the method name.
     *
     * @return method name
     */
    String getMethodName();

    /**
     * Set the method name.
     *
     * @param name name
     */
    void setMethodName( String name );
}
