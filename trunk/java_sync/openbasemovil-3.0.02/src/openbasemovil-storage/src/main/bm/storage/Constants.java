package bm.storage;
/* -----------------------------------------------------------------------------
    OpenBaseMovil Storage Library
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
 * Created on       : 07-dic-2006 23:32:34
 * Created by       : narciso
 * Last modified by : $Author$
 * Last modified on : $Date$
 * Revision         : $Revision$
 */

/**
 * Constants for the bm.rs package.
 *
 * @author <a href="mailto:narciso@elondra.org">Narciso Cerezo</a>
 * @version $Revision$
 */
public class Constants
{
    //--------------------------------------------------------------------------
    // Error Codes
    //--------------------------------------------------------------------------

    public static final int MAX_MULTIPLEXED_ROW_SIZE = 8192;
    public static final int MAX_MULTIPLEXED_ROWS = 100;

    // Store
    public static final int ERR_STR_OPEN                        = 15100;
    public static final int ERR_STR_ADD_RECORD                  = 15101;
    public static final int ERR_STR_FLUSH                       = 15102;
    public static final int ERR_STR_DELETE_RECORD               = 15103;
    public static final int ERR_STR_GET_VALID_OFFSETS           = 15104;
    public static final int ERR_STR_ENUMERATE                   = 15105;
    public static final int ERR_STR_DELETE                      = 15106;
    public static final int ERR_STR_CLOSE                       = 15107;
    public static final int ERR_STR_DROP                        = 15108;
    public static final int ERR_STR_PACK_COPY                   = 15109;
    public static final int ERR_STR_GET_RECORD_SIZE             = 15110;
    public static final int ERR_STR_LOAD_RECORD                 = 15111;
    public static final int ERR_STR_SET_RECORD                  = 15112;

    // Record
    public static final int ERR_REC_ADD_OVERFLOW                = 15150;
    public static final int ERR_REC_DELETE_OVERFLOW             = 15151;
    public static final int ERR_REC_GET_OVERFLOW                = 15152;
    public static final int ERR_REC_INVALID_RECORD_DATA         = 15153;

    // StoreEnumeration
    public static final int ERR_STE_NEXT_ID                     = 15200;
    public static final int ERR_STE_FEED                        = 15201;

    // StoreInfo
    public static final int STI_FLUSH                           = 15250;

}
