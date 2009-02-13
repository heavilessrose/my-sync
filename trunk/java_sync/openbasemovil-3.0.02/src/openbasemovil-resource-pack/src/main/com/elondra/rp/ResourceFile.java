package com.elondra.rp;
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

/*
 * File Information
 *
 * Created on       : 08-mar-2007 0:32:50
 * Created by       : narciso
 * Last modified by : $Author$
 * Last modified on : $Date$
 * Revision         : $Revision$
 */

/**
 * A resource file.
 *
 * @author <a href="mailto:narciso@elondra.org">Narciso Cerezo</a>
 * @version $Revision$
 */
public class ResourceFile
{
    private String file;
    private String encoding = "UTF-8";

    public ResourceFile()
    {
    }

    public ResourceFile( final String file )
    {
        this.file = file;
    }

    public ResourceFile( final String file, final String encoding )
    {
        this.file = file;
        this.encoding = encoding;
    }

    public String getFile()
    {
        return file;
    }

    public void setFile( final String file )
    {
        this.file = file;
    }

    public String getEncoding()
    {
        return encoding;
    }

    public void setEncoding( final String encoding )
    {
        this.encoding = encoding;
    }
}
