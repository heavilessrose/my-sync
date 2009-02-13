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
 * Created on       : 19-oct-2007 21:46:10
 * Created by       : narciso
 * Last modified by : $Author$
 * Last modified on : $Date$
 * Revision         : $Revision$
 */

/**
 * The &quot;while&quot; keyword.
 *
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision$
 */
public class WhileKeyword
    extends Keyword
{
    public WhileKeyword()
    {
        name = "while";
    }

    public void parse(
            final ClassCompiler compiler,
            final StreamParser  parser,
            final Block         parent
    )
            throws CompilerException
    {
        final CompWhile command = new CompWhile();
        try
        {
            final int c = parser.nextChar();
            if( c != '(' )
            {
                throw new CompilerException(
                        "( expected",
                        parser.getStartLine(),
                        parser.getStartChar()
                );
            }

            command.setCondition( compiler.parseExpression( ")" ) );
            parser.searchNext( '{' );
            final Block body = new Block();

            final BlockKeyword keyword = new BlockKeyword();
            keyword.parse( compiler, parser, body );

            command.setBody( body );
            parent.add( command );
        }
        catch( ParserException e )
        {
            throw new CompilerException( e );
        }
    }
}
