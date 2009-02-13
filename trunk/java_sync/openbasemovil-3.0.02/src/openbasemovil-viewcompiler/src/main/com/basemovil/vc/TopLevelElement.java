package com.basemovil.vc;
/* -----------------------------------------------------------------------------
    OpenBaseMovil View Compiler, generates the binary form of views from
    an XML file.
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

import bm.db.index.Index;

/*
 * File Information
 *
 * Created on       : 17-oct-2007 18:09:07
 * Created by       : narciso
 * Last modified by : $Author$
 * Last modified on : $Date$
 * Revision         : $Revision$
 */

/**
 * Top level elements represent the objects that are directly nested under
 * the view compiler: iconSet, controller, and views
 *
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision$
 */
public abstract class TopLevelElement
{
    protected ViewCompiler parent;

    public ViewCompiler getParent()
    {
        return parent;
    }

    public void setParent( final ViewCompiler parent )
    {
        this.parent = parent;
    }

    /**
     * Store the binary element definition in the index.
     *
     * @param index index
     * @throws ViewCompilerException on errors
     */
    public abstract void store( final Index index )
            throws ViewCompilerException;
}
