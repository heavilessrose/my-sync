package bm.vm.lang;

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

import bm.core.io.SerializerOutputStream;
import bm.core.io.SerializationException;
import bm.core.io.SerializerInputStream;
/*
 * File Information
 *
 * Created on       : 10-oct-2007 17:30:52
 * Created by       : narciso
 * Last modified by : $Author$
 * Last modified on : $Date$
 * Revision         : $Revision$
 */

/**
 * An operator that requires to operands.
 *
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision$
 */
public abstract class BinaryOperator
        extends Command
{
    protected Expression    operand1;
    protected Expression    operand2;

    public void setOperand1( final Expression operand1 )
    {
        this.operand1 = operand1;
    }

    public void setOperand2( final Expression operand2 )
    {
        this.operand2 = operand2;
    }

    /**
     * Serializes command common properties, must be called by subclasses.
     *
     * @param out output stream
     * @throws bm.core.io.SerializationException
     *          on errors
     */
    public synchronized void serialize( SerializerOutputStream out )
            throws SerializationException
    {
        super.serialize( out );
        out.writeByte( (byte) 1 ); // version
        out.writeBoolean( operand1 == null );
        if( operand1 != null )
        {
            operand1.serialize( out );
        }
        out.writeBoolean( operand2 == null );
        if( operand2 != null )
        {
            operand2.serialize( out );
        }
    }

    /**
     * Read command common properties, must be called by subclasses.
     *
     * @param in input stream
     * @throws bm.core.io.SerializationException
     *          on errors
     */
    public synchronized void deserialize( SerializerInputStream in )
            throws SerializationException
    {
        super.deserialize( in );
        in.readByte(); // skip version
        boolean isNull = in.readBoolean();
        if( !isNull )
        {
            operand1 = new Expression();
            operand1.setParent( this );
            operand1.deserialize( in );
        }
        else
        {
            operand1 = null;
        }
        isNull = in.readBoolean();
        if( !isNull )
        {
            operand2 = new Expression();
            operand2.setParent( this );
            operand2.deserialize( in );
        }
        else
        {
            operand2 = null;
        }
    }
}
