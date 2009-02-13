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

import bm.vm.lang.Block;
import bm.core.tools.StreamParser;
import bm.core.tools.ParserException;
/*
 * File Information
 *
 * Created on       : 19-oct-2007 20:50:03
 * Created by       : narciso
 * Last modified by : $Author$
 * Last modified on : $Date$
 * Revision         : $Revision$
 */

/**
 * The &quot;var&quot; keyword.
 *
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision$
 */
public class VarKeyword
    extends Keyword
{
    public VarKeyword()
    {
        name = "var";
    }

    public void parse(
            final ClassCompiler compiler,
            final StreamParser  parser,
            final Block         parent
    )
            throws CompilerException
    {
        final CompVar command = new CompVar();
        try
        {
            String token = parser.next( ClassCompiler.WS, false, false, false );
            compiler.getClass( token );
            command.setClassName( token );

            token = parser.next( ";", false, false, false );
            command.setName( token );

            parent.add( command );
        }
        catch( ParserException e )
        {
            throw new CompilerException( e );
        }
    }
}
