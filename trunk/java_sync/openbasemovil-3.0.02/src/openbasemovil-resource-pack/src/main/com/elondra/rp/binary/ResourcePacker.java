package com.elondra.rp.binary;
/* -----------------------------------------------------------------------------
    OpenBaseMovil ResourcePacker, packs and checks resource files
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

import org.apache.tools.ant.Task;
import org.apache.tools.ant.BuildException;
import com.elondra.rp.ResourceFile;
import com.elondra.rp.PackException;

/*
 * File Information
 *
 * Created on       : 08-mar-2007 0:28:25
 * Created by       : narciso
 * Last modified by : $Author$
 * Last modified on : $Date$
 * Revision         : $Revision$
 */

/**
 * Resource packer ant decorator.
 *
 * @author <a href="mailto:narciso@elondra.org">Narciso Cerezo</a>
 * @version $Revision$
 */
public class ResourcePacker
    extends Task
{
    private Packer packer = new Packer();

    public String getOut()
    {
        return packer.getOut();
    }

    public void setOut( final String out )
    {
        packer.setOut( out );
    }

    public String getMode()
    {
        return packer.getMode();
    }

    public void setMode( final String mode )
    {
        packer.setMode( mode );
    }

    public void setOrder( final int order )
    {
        packer.setOrder( order );
    }

    public int getOrder()
    {
        return packer.getOrder();
    }

    public void addConfigured( ResourceFile file )
    {
        packer.addConfigured( file );
    }

    public void setFiles( final String files )
    {
        packer.configure( files );
    }

    public void execute()
            throws BuildException
    {
        try
        {
            packer.execute();
        }
        catch( PackException e )
        {
            throw new BuildException( e );
        }
    }
}
