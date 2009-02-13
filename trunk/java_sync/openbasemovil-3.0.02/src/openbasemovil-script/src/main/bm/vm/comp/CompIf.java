package bm.vm.comp;

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

import bm.vm.lang.If;
import bm.vm.lang.Expression;
import bm.vm.lang.Block;
/*
 * File Information
 *
 * Created on       : 11-oct-2007 19:56:02
 * Created by       : narciso
 * Last modified by : $Author$
 * Last modified on : $Date$
 * Revision         : $Revision$
 */

/**
 * Decorator for compilation.
 *
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision$
 */
public class CompIf
    extends If
{
    public void setCondition( final Expression condition )
    {
        this.condition = condition;
    }

    public void setIfBranch( final Block ifBranch )
    {
        this.ifBranch = ifBranch;
    }

    public void setElseBranch( final Block elseBranch )
    {
        this.elseBranch = elseBranch;
    }
}
