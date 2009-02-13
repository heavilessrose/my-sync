package com.basemovil.vc.view.form;
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

import bm.core.io.SerializerOutputStream;
import bm.core.io.SerializationException;
import com.basemovil.vc.ViewCompilerException;
import com.basemovil.vc.ViewCompiler;
/*
 * File Information
 *
 * Created on       : 17-oct-2007 22:05:50
 * Created by       : narciso
 * Last modified by : $Author$
 * Last modified on : $Date$
 * Revision         : $Revision$
 */

/**
 * A money item.
 *
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision$
 */
public class Money
        extends Text
{
    protected int       decimals;

    public Money()
    {
        type = MONEY;
    }

    public int getDecimals()
    {
        return decimals;
    }

    public void setDecimals( final int decimals )
    {
        this.decimals = decimals;
    }

    public void store( final SerializerOutputStream out )
            throws ViewCompilerException
    {
        super.store( out );

        try
        {
            out.writeInt( decimals );
        }
        catch( SerializationException e )
        {
            throw new ViewCompilerException( e );
        }
    }
}
