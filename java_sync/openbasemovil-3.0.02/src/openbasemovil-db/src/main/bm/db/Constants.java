package bm.db;
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

/*
 * File Information
 *
 * Created on       : 14-sep-2006 10:10:07
 * Created by       : narciso
 * Last modified by : $Author:narciso $
 * Last modified on : $Date:2007-02-04 00:37:40 +0100 (dom, 04 feb 2007) $
 * Revision         : $Revision:2 $
 */

/**
 * Constants for the bm db package.
 *
 * @author <a href="mailto:narciso@elondra.org">Narciso Cerezo</a>
 * @version $Revision:2 $
 */
public class Constants
{
    //--------------------------------------------------------------------------
    // Misc constants
    //--------------------------------------------------------------------------

    // Start size for arrays when reading records from recordstores
    public static final int START_SIZE      = 128;
    public static final int GROWTH_FACTOR   = 128;

    public static final String REMOTE_ID       = "remote_id";

    //--------------------------------------------------------------------------
    // Field types
    //--------------------------------------------------------------------------
    public static final byte FT_DISABLED     = -1;
    public static final byte FT_STRING       = 0;
    public static final byte FT_INT          = 1;
    public static final byte FT_LONG         = 2;
    public static final byte FT_SHORT        = 3;
    public static final byte FT_FIXED_POINT  = 4;
    public static final byte FT_DATE         = 5;
    public static final byte FT_BOOLEAN      = 6;
    public static final byte FT_BLOB         = 7;
    public static final byte FT_IMAGE        = 8;
    public static final byte FT_BVLOB        = 9;

    //--------------------------------------------------------------------------
    // Multiplexation
    //--------------------------------------------------------------------------
    public static final int    MAX_MULTIPLEXED_BLOB_ROWS    = 4;
    public static final int    MULTIPLEXER_SECURITY_FACTOR  = 2;

    //--------------------------------------------------------------------------
    // Database structure record types
    //--------------------------------------------------------------------------
    /**
     * Record Type: table information.
     */
    public static final byte DBRT_TABLE_INFO     = 0;
    /**
     * Record Type: table status.
     */
    public static final byte DBRT_TABLE_STATUS   = 1;


    public static final int MAX_TABLE_NAME_LENGTH = 26;

    // *************************************************************************
    // Constants used by the commercial version
    // *************************************************************************

    public static final String[] DOTS = { " [|]", " [/]", " [-]", " [\\]" };

    //--------------------------------------------------------------------------
    // Misc constants
    //--------------------------------------------------------------------------
    // Memory check boundaries for table synchronization
    public static final int LOW_MEM         = 60 * 1024;
    public static final int FORCE_GC_COUNT  = 100;
    public static final int CHECK_MEM_COUNT = 20;

    //--------------------------------------------------------------------------
    // Download Types
    //--------------------------------------------------------------------------
    public static final Integer DT_DATABASE_STRUCTURE           = new Integer( 2 );
    public static final Integer DT_WHOLE_DATABASE               = new Integer( 3 );

    //--------------------------------------------------------------------------
    // Downloaded Data Section Types
    //--------------------------------------------------------------------------
    public static final byte DBDATA_NONE            = 0;
    public static final byte DBDATA_FULL_TABLE      = 1;
    public static final byte DBDATA_TABLE_UPDATE    = 2;
    public static final byte DBDATA_INDEX           = 3;
    public static final byte DBDATA_END             = 127;

    //--------------------------------------------------------------------------
    // Donwload status
    //--------------------------------------------------------------------------
    public static final int DOWNLOAD_NOT_READY          = -1;
    public static final int INVALID_TICKET              = -2;
    public static final int ERROR_PREPARING_DOWNLOAD    = -3;

    //--------------------------------------------------------------------------
    // Table Data sections
    //--------------------------------------------------------------------------
    public static final int     ROW_END     = -1;
    public static final int     ROW_DELETED = 0;
    public static final int     ROW_UPDATED = 1;

    //--------------------------------------------------------------------------
    // Status codes for updates from the device
    //--------------------------------------------------------------------------
    public static final int RC_OK                   = 0;
    public static final int RC_DUPLICATE_RECORD     = 1;
    public static final int RC_UPDATE_NOT_POSSIBLE  = 2;
    public static final int RC_DELETED_RECORD       = 3;
    public static final int RC_DELETE_NOT_POSSIBLE  = 4;
    public static final int RC_INVALID_RECORD       = 5;

    //--------------------------------------------------------------------------
    // Synchronization process status
    //--------------------------------------------------------------------------
    public static final short SS_NONE               = 0;
    public static final short SS_CHANGES_GENERATED  = 1;
    public static final short SS_CHANGES_SENT       = 2;
    public static final short SS_CHANGES_FIXED      = 3;
    public static final short SS_PROCESS_PENDING    = 4;
    public static final short SS_REQUEST_UPDATE     = 5;
    public static final short SS_PROCESS_UPDATE     = 6;
    public static final short SS_FINISHED           = 100;

    //--------------------------------------------------------------------------
    // Synchronization temporary files
    //--------------------------------------------------------------------------
    public static final String CHANGES_FILE         = "changes_file_tmp";
    public static final String RECORDS_FILE         = "records_file_tmp";

    //--------------------------------------------------------------------------
    // Retry wait times
    //--------------------------------------------------------------------------
    public static final long    MAX_WAIT_TIME       = 15 * 60 * 1000;
    public static final long    INITIAL_WAIT_PERIOD = 1000;
    public static final long    INCREASE_WAIT       = 5000;
    public static final long    MAX_WAIT_PERIOD     = 60000;

    //--------------------------------------------------------------------------
    // Login errors
    //--------------------------------------------------------------------------
    public static final int SC_INVALID_USER_PASSWD = -1;
    public static final int SC_ERROR = -2;
    public static final int SC_INVALID_APPLICATION_FOR_USER = -3;

    //--------------------------------------------------------------------------
    // Common preferences
    //--------------------------------------------------------------------------
    public static final String USER_FULL_NAME   = "user.fullName";
    public static final String USER_CUSTOM_ID   = "user.customId";
    public static final String USER_ID          = "user.id";
    public static final String DEVICE_ID        = "device.id";
    public static final String DEVICE_NUMBER    = "device.num";
    public static final String USER_PASSWORD    = "user.password";

    public static final String DP_IS_COLOR          = "isColor";
    public static final String DP_MEMORY            = "memory";
    public static final String DP_NUM_COLORS        = "numColors";
    public static final String DP_VIBRATION         = "vibration";
    public static final String DP_PROTOCOL_VERSION  = "retina.client.version";
    public static final String DP_PHONE_NUMBER      = "phone.number";

    // *************************************************************************
    // Error Codes
    // *************************************************************************

    // Database header
    public static final int ERR_DB_HEADER_SERIALIZE             = 10001;
    public static final int ERR_DB_HEADER_DESERIALIZE           = 10002;
    // Database Tools
    public static final int ERR_DBT_DOWNLOAD_STRUCTURE          = 10011;
    public static final int ERR_DBT_GET_RELATED_TICKET          = 10012;
    public static final int ERR_DBT_PROCESS_DELETED             = 10013;
    public static final int ERR_DBT_DESERIALIZE_MODIFIED_ROW    = 10014;
    public static final int ERR_DBT_FIND_UPD_ROW                = 10015;
    public static final int ERR_DBT_CORRUPTED_INDEX             = 10016;
    public static final int ERR_DBT_UPDATE_INDEXES              = 10017;
    // Database Initializer
    public static final int ERR_DBI_DOWNLOAD_STRUCTURE          = 10021;
    public static final int ERR_DBI_INITIALIZE_IN               = 10022;
    public static final int ERR_DBI_UNEXPECTED_END_OF_SECTION   = 10023;
    public static final int ERR_DBI_PROCESS_TABLE               = 10024;
    public static final int ERR_DBI_DESERIALIZE_MODIFIED_ROW    = 10025;
    public static final int ERR_DBI_PROCESS_INDEX               = 10026;
    public static final int ERR_DBI_WAIT_DOWNLOAD               = 10027;
    public static final int ERR_DBI_DOWNLOAD_WHOLE_DB           = 10028;
    public static final int ERR_DBI_RUN                         = 10029;

    // Database
    public static final int ERR_DB_UPDATE_STATUS                = 10031;
    public static final int ERR_DB_SAVE_DBHEADER                = 10032;
    public static final int ERR_DB_LOAD_STRUCTURE               = 10033;
    // Table Status
    public static final int ERR_TS_SERIALIZE                    = 10041;
    public static final int ERR_TS_DESERIALIZE                  = 10042;
    // Field Info
    public static final int ERR_FI_SERIALIZE                    = 10051;
    public static final int ERR_FI_DESERIALIZE                  = 10052;
    // Index Info
    public static final int ERR_II_SERIALIZE                    = 10061;
    public static final int ERR_II_DESERIALIZE                  = 10062;
    // Index Node
    public static final int ERR_INODE_SERIALIZE                 = 10071;
    public static final int ERR_INODE_DESERIALIZE               = 10072;
    // Index Node Key
    public static final int ERR_INK_SERIALIZE                   = 10081;
    public static final int ERR_INK_DESERIALIZE                 = 10082;
    // Sorted Int Array
    public static final int ERR_SIA_SERIALIZE                   = 10091;
    public static final int ERR_SIA_DESERIALIZE                 = 10092;
    // Table Info
    public static final int ERR_TI_SERIALIZE                    = 10120;
    public static final int ERR_TI_DESERIALIZE                  = 10121;
    // Index
    public static final int ERR_IDX_GET_NODE                    = 10141;
    public static final int ERR_IDX_SAVE_NODE                   = 10142;
    public static final int ERR_IDX_DROP_NODE                   = 10143;
    public static final int ERR_IDX_INSERT_NODE                 = 10144;
    public static final int ERR_IDX_FIND                        = 10145;
    public static final int ERR_IDX_FIND_FUZZY                  = 10146;
    // Sorted Index
    public static final int ERR_SIDX_REBUILD                    = 10151;
    public static final int ERR_SIDX_LOAD_HEADER                = 10152;
    public static final int ERR_SIDX_LOAD_SEGMENT               = 10153;
    public static final int ERR_SIDX_WRITE_HEADER               = 10154;
    public static final int ERR_SIDX_CHECKUPDATE                = 10155;
    public static final int ERR_SIDX_FIND                       = 10156;
    // Table
    public static final int ERR_TBL_REBUILD_MODIFIED_INDEX      = 10201;
    public static final int ERR_TBL_STORE_UPDATE                = 10202;
    public static final int ERR_TBL_STORE_ADD                   = 10203;
    public static final int ERR_TBL_GET_ROW_FROM_STORE          = 10204;
    public static final int ERR_TBL_MARK_SENT                   = 10205;
    public static final int ERR_TBL_UPDATE_INDEX                = 10206;
    public static final int ERR_TBL_UPDATE_MOD_INDEX            = 10207;
    public static final int ERR_TBL_UPDATE_INDEXES              = 10208;
    public static final int ERR_TBL_FIND_RAW                    = 10209;
    public static final int ERR_TBL_DROP_ROW                    = 10210;
    public static final int ERR_TBL_FIND_ALL                    = 10211;
    public static final int ERR_TBL_SAVE_ROW                    = 10212;
    public static final int ERR_TBL_REMOVE_ROW                  = 10213;
    public static final int ERR_TBL_UNMARK                      = 10214;

    // Row
    public static final int ERR_ROW_SERIALIZE                   = 10231;
    public static final int ERR_ROW_DESERIALIZE                 = 10232;
    public static final int ERR_ROW_SERIALIZE_FIELDS            = 10233;
    public static final int ERR_ROW_DESERIALIZE_FIELDS          = 10234;
    public static final int ERR_ROW_SERIALIZE_FIELD             = 10235;
    public static final int ERR_ROW_DESERIALIZE_FIELD           = 10236;
    public static final int ERR_ROW_SERIALIZE_HEADER            = 10237;
    public static final int ERR_ROW_DESERIALIZE_HEADER          = 10238;
    public static final int ERR_ROW_CHECK_TYPE                  = 10239;
    // Preferences
    public static final int ERR_PRF_LOAD                        = 10241;
    public static final int ERR_PRF_GET                         = 10242;
    public static final int ERR_PRF_SET                         = 10243;
    public static final int ERR_PRF_SET_DATA                    = 10244;
    public static final int ERR_PRF_UNSET                       = 10245;
    public static final int ERR_PRF_WRITE_DIRECTORY             = 10246;
    public static final int ERR_PRF_DROP_RESTART                = 10247;

    // DatabaseSynchronizer
    public static final int ERR_DBS_DOWNLOAD_STRUCTURE          = 10251;
    public static final int ERR_DBS_UPDATE_TABLE_INFO           = 10252;
    public static final int ERR_DBS_CHECK_STRUCTURE             = 10253;
    public static final int ERR_DBS_BUILD_CHANGE_FILE           = 10254;
    public static final int ERR_DBS_PROCESS_CHANGE_CONFIRMATION = 10255;
    public static final int ERR_DBS_RUN                         = 10256;

    // UpdateRequest
    public static final int ERR_UPR_SERIALIZE_TABLE_REQUEST     = 10261;
    public static final int ERR_UPR_DESERIALIZE_TABLE_REQUEST   = 10262;
    public static final int ERR_UPR_SERIALIZE                   = 10263;
    public static final int ERR_UPR_DESERIALIZE                 = 10264;

    // IndexKey
    public static final int ERR_IDK_SERIALIZE                   = 10500;
    public static final int ERR_IDK_DESERIALIZE                 = 10501;

    // LoginInfo
    public static final int ERR_LIF_SAVE                        = 10600;
    public static final int ERR_LIF_LOAD                        = 10601;

    // PersistentMap
    public static final int ERR_PM_SAVE_DIR                     = 10700;
    public static final int ERR_PM_CLEAR                        = 10701;
    public static final int ERR_PM_GET_NODB                     = 10702;
    public static final int ERR_PM_GET_NOTBL                    = 10703;
    public static final int ERR_PM_GET                          = 10704;
    public static final int ERR_PM_SET                          = 10705;

    // Database Backup
    public static final int ERR_DBK_FILE_IO                     = 10800;
    public static final int ERR_DBK_RS                          = 10801;

    // Database Restore
    public static final int ERR_DBR_FILE_IO                     = 10900;
    public static final int ERR_DBR_RS                          = 10901;
    public static final int ERR_DBR_RS_CLOSE                    = 10902;
    public static final int ERR_DBR_RS_DELETE                   = 10903;
}
