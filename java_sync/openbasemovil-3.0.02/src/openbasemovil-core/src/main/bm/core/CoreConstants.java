package bm.core;
/* -----------------------------------------------------------------------------
    OpenBaseMovil Core Library, foundation of the OpenBaseMovil database and tools
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
 * Created on       : 08-dic-2006 13:22:09
 * Created by       : narciso
 * Last modified by : $Author: narciso_cerezo $
 * Last modified on : $Date: 2008-01-16 23:55:21 +0000 (mié, 16 ene 2008) $
 * Revision         : $Revision: 6 $
 */

/**
 * Core package constants.
 *
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision: 6 $
 */
public class CoreConstants
{
    //--------------------------------------------------------------------------
    // Misc constants
    //--------------------------------------------------------------------------
    public static final int SIMPLE_CACHE_DEFAULT_SIZE = 10;
    public static final String DEFAULT_ENCODING = "UTF-8";
    // Convenience constants to replace missing Boolean.TRUE and Boolean.FALSE
    public static final Boolean TRUE    = new Boolean( true );
    public static final Boolean FALSE   = new Boolean( false );

    //--------------------------------------------------------------------------
    // Error Codes
    //--------------------------------------------------------------------------

    // Properties
    public static final int ERR_PRP_SERIALIZE                       = 1021;
    public static final int ERR_PRP_DESERIALIZE                     = 1022;
    // Tools
    public static final int ERR_TOL_SECURE_ALLOC                    = 1041;
    public static final int ERR_TOL_INVALID_SERIALIZABLE_OBJECT     = 1042;
    // SequentialFileOutputStream
    public static final int ERR_SFO_OPEN                            = 1051;
    // RandomAccessInputStream
    public static final int ERR_RAIS_READ_HEADER                    = 1061;
    public static final int ERR_RAIS_CONSTRUCTOR                    = 1062;
    public static final int ERR_RAIS_INIT                           = 1063;
    public static final int ERR_RAIS_SEEK                           = 1064;
    // SerializerOutputStream
    public static final int ERR_SOS                                 = 1080;
    // SerializerInputStream
    public static final int ERR_SIS                                 = 1090;

    //--------------------------------------------------------------------------
    // Common Views IDs
    //--------------------------------------------------------------------------
    public static final int HELP_VIEW_ID        = 10;
    public static final int PROGRESS_VIEW_ID    = 20;

    //--------------------------------------------------------------------------
    // ResourceManager
    //--------------------------------------------------------------------------
    public static final String PROP_RESOURCE_LOCALE = "resource.locale";
    public static final String MICROEDITION_LOCALE  = "microedition.locale";
    public static final String USER_LANGUAGE        = "user.language";
    public static final String USER_COUNTRY         = "user.country";
    public static final String DEFAULT_LOCALE       = "es_ES";

    //--------------------------------------------------------------------------
    // Object types for SerializerStreams
    //--------------------------------------------------------------------------
    public static final byte SERIALIZABLE  = 0;
    public static final byte STRING        = 1;
    public static final byte LONG          = 2;
    public static final byte BYTE          = 3;
    public static final byte BOOLEAN       = 4;
    public static final byte INTEGER       = 5;
    public static final byte SHORT         = 6;
    public static final byte ARRAY         = 7;
    public static final byte MAP           = 8;
    public static final byte VECTOR        = 9;
    public static final byte BOB           = 10;
    public static final byte DATE          = 11;
    public static final int MAX_BLOB_SIZE = 128000;
}
