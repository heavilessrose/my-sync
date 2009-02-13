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
 * Created on       : 19-oct-2007 21:30:55
 * Created by       : narciso
 * Last modified by : $Author$
 * Last modified on : $Date$
 * Revision         : $Revision$
 */

/**
 * Synthetic block keyword.
 *
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision$
 */
public class BlockKeyword
    extends Keyword
{

    public BlockKeyword()
    {
        name = "block";
    }

    public void parse(
            final ClassCompiler compiler,
            final StreamParser  parser,
            final Block         parent
    )
            throws CompilerException
    {
        final Block block = new Block();
        while( true )
        {
            String token;
            try
            {
                token = parser.next( ClassCompiler.WS + "(", false, true, true );
            }
            catch( ParserException e )
            {
                throw new CompilerException( e );
            }
            if( token == null )
            {
                throw new CompilerException(
                        "Unexpected end of file",
                        parser.getStartLine(),
                        parser.getStartChar()
                );
            }
            if( token.equals( "}" ) )
            {
                parent.add( block );
                return;
            }
            else
            {
                final Keyword keyword = compiler.getKeyword( token );
                if( keyword != null )
                {
                    keyword.parse( compiler, parser, block );
                }
                else
                {
                    parser.pushback( token );
                    block.add( compiler.parseExpression( ";" ) );
                }
            }
        }
    }
}
