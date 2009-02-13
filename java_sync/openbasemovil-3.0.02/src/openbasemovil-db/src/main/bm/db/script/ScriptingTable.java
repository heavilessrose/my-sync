package bm.db.script;
/* -----------------------------------------------------------------------------
    OpenBaseMovil Database Library
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

import bm.core.CoreConstants;
import bm.db.Row;
import bm.db.RowSet;
import bm.db.Table;
import bm.vm.Instance;
import bm.vm.NativeClass;
import bm.vm.VirtualMachine;
import bm.vm.VirtualMachineException;
import bm.vm.lang.Conversor;
/*
 * File Information
 *
 * Created on       : 23-oct-2007 22:11:47
 * Created by       : narciso
 * Last modified by : $Author$
 * Last modified on : $Date$
 * Revision         : $Revision$
 */

/**
 * Scripting class to access tables from script.
 *
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision$
 */
public class ScriptingTable
    extends NativeClass
{
    /*
    Pendientes de implementar


    public int[] findRaw( final String fieldName, final Object value )
    public int[] findRawFuzzy( final String fieldName, final String value )
    public TableStatus getTableStatus()
    public void rebuildModifiedIndex()
    public TableInfo getTableInfo()
    public void unmarkSentRecords()
    public long getSize()
    public long getTotalSize()
    public long getCacheHits()
    public long getCacheMisses()
    public long getAverageFindTime()
    public RowSet getModifiedRecords()
    public FieldInfo getFieldInfo( final int index )
    public FieldInfo getFieldInfo( final String name )

    public synchronized void save( final Row row )
    public void remove( final Row row )

     */

    private static final int GET_NAME           = 0;
    private static final int GET_PARENT         = 1;
    private static final int FETCH              = 2;
    private static final int OPEN               = 3;
    private static final int CLOSE              = 4;
    private static final int OPEN_TREE          = 5;
    private static final int CLOSE_TREE         = 6;
    private static final int DROP               = 7;
    private static final int CREATE_INDEX       = 8;
    private static final int FIND               = 9;
    private static final int CREATE_ROW         = 10;
    private static final int FIND_FUZZY         = 11;
    private static final int FIND_ALL           = 12;
    private static final int REMOVE             = 13;
    private static final int HAS_FIELD          = 14;

    static
    {
        addMethod( "getName", GET_NAME, 0 );
        addMethod( "getParent", GET_PARENT, 0 );
        addMethod( "fetch", FETCH, 1 );
        addMethod( "open", OPEN, 0 );
        addMethod( "close", CLOSE, 0 );
        addMethod( "openTree", OPEN_TREE, 0 );
        addMethod( "closeTree", CLOSE_TREE, 0 );
        addMethod( "drop", DROP, 0 );
        addMethod( "createIndex", CREATE_INDEX, 5 );
        addMethod( "find", FIND, 2 );
        addMethod( "createRow", CREATE_ROW, 0 );
        addMethod( "findFuzzy", FIND_FUZZY, 2 );
        addMethod( "findAll", FIND_ALL, 0 );
        addMethod( "remove", REMOVE, 0 );
        addMethod( "hasField", HAS_FIELD, 1 );
    }

    /**
     * Constructor.
     *
     * @param vm virtual machine
     */
    public ScriptingTable( final VirtualMachine vm )
    {
        super( vm );
        name = "Table";
    }

    /**
     * Invoke the method.
     *
     * @param target target instance on which to run
     * @param args   method arguments
     * @return return value
     * @throws bm.vm.VirtualMachineException on errors
     */
    public Instance invoke(
            Instance target, Instance[] args
    )
            throws VirtualMachineException
    {
        final int id = checkMethod( methodName, args );
        if( id != -1 )
        {
            final Table table = (Table) target.getAttachment( "table" );
            if( table != null )
            {
                try
                {
                    switch( id )
                    {
                        case GET_NAME           :
                            return Conversor.toInstance(
                                    getVirtualMachine(),
                                    table.getName()
                            );

                        case GET_PARENT         :
                            return (Instance) target.getAttachment( "parent" );

                        case FETCH              :
                            return sendRow(
                                    table.fetch(
                                            Conversor.toInteger(
                                                    args[0].get( "value" )
                                            ).intValue()
                                    ),
                                    target
                            );

                        case OPEN               :
                            table.open();
                            break;

                        case CLOSE              :
                            table.close();
                            break;

                        case OPEN_TREE          :
                            table.openTree();
                            break;

                        case CLOSE_TREE         :
                            table.closeTree();
                            break;

                        case DROP               :
                            table.drop();
                            break;

                        case CREATE_INDEX       :
                            table.createIndex(
                                    Conversor.toString( args[0] ),
                                    Conversor.toInteger( args[1] ).intValue(),
                                    Conversor.mapToStringArray( args[2] ),
                                    Conversor.toInteger( args[3] ).byteValue(),
                                    Conversor.toBoolean( args[4] )
                            );
                            break;

                        case FIND               :
                            return sendRowSet(
                                    table.find(
                                            Conversor.toString( args[0] ),
                                            args[1].get( "value" )
                                    ),
                                    target
                            );

                        case CREATE_ROW         :
                            return sendRow( table.createRow(), target );

                        case FIND_FUZZY         :
                            return sendRowSet(
                                    table.findFuzzy(
                                            Conversor.toString( args[0] ),
                                            Conversor.toString( args[1] )
                                    ),
                                    target
                            );

                        case FIND_ALL           :
                            return sendRowSet( table.findAll(), target );

                        case REMOVE             :
                            table.remove();
                            break;

                        case HAS_FIELD          :
                            return Conversor.toInstance(
                                    getVirtualMachine(),
                                    table.hasField( Conversor.toString(
                                            args[0]
                                    ) ) ?
                                        CoreConstants.TRUE :
                                        CoreConstants.FALSE
                            );
                    }
                }
                catch( Exception e )
                {
                    throw new VirtualMachineException( 0, e.getMessage() );
                }
            }
        }
        return null;
    }

    private Instance sendRow( final Row row, final Instance target )
            throws VirtualMachineException
    {
        final Instance r = getVirtualMachine().newInstance( "Row" );
        r.setAttachment( "row", row );
        r.setAttachment( "table", target );
        return r;
    }

    private Instance sendRowSet( final RowSet rowSet, final Instance target )
            throws VirtualMachineException
    {
        final Instance r = getVirtualMachine().newInstance( "RowSet" );
        r.setAttachment( "rowset", rowSet );
        r.setAttachment( "table", target );
        return r;
    }
}
