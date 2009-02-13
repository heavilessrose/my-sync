package bm.core.tools;

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

import java.util.Enumeration;
import java.util.Vector;

/*
 * File Information
 *
 * Created on       : 30-oct-2007 23:37:37
 * Created by       : narciso
 * Last modified by : $Author$
 * Last modified on : $Date$
 * Revision         : $Revision$
 */

/**
 * A union of two or more enumerations that look like a single one.
 *
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision$
 */
public class EnumerationUnion
    implements Enumeration
{
    private Vector  elements = new Vector( 10 );
    private int     current;

    public void add( final Enumeration i )
    {
        elements.addElement( i );
    }

    public boolean hasMoreElements()
    {
        if( current <= elements.size() )
        {
            final Enumeration i = (Enumeration) elements.elementAt( current );
            if( i.hasMoreElements() )
            {
                return true;
            }
            else
            {
                current++;
                return hasMoreElements();
            }
        }
        else
        {
            return false;
        }
    }

    public Object nextElement()
    {
        if( current <= elements.size() )
        {
            final Enumeration i = (Enumeration) elements.elementAt( current );
            if( i.hasMoreElements() )
            {
                return i.nextElement();
            }
            else
            {
                current++;
                return nextElement();
            }
        }
        else
        {
            return null;
        }
    }
}