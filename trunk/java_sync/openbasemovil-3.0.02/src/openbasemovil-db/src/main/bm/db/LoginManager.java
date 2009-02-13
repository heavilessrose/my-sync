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

import bm.core.log.Log;
import bm.core.log.LogFactory;
import bm.core.tools.MD5Digest;

/*
 * File Information
 *
 * Created on       : 08-mar-2007 17:33:00
 * Created by       : narciso
 * Last modified by : $Author$
 * Last modified on : $Date$
 * Revision         : $Revision$
 */

/**
 * Manages login and user information.
 *
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision$
 */
public class LoginManager
{
    private static final Log log = LogFactory.getLog( "LoginManager" );

    private static LoginInfo    loginInfo;

    /**
     * Saves the given password as the password for offline use of the application.<br/>
     * The password is encrypted and stored in persistent storage for future use.
     * @param password the password
     * @throws DBException on error storing the password
     */
    public static void storeEncryptedPassword( final String password )
            throws DBException
    {
        MD5Digest md5 = new MD5Digest();
        final byte[] data = password.getBytes();
        md5.update( data, 0, data.length );
        final byte[] digest = new byte[16];
        md5.doFinal( digest, 0 );
        final LoginInfo info = getLoginInfo();
        info.password = digest;
        info.save();
    }

    /**
     * Check if the given password matches the encrypted stored one.
     *
     * @param challenge challenge password
     * @return true if they match, false if they don't or no stored password
     * @throws DBException on errors getting the stored password
     */
    public static boolean checkOfflinePassword( final String challenge )
            throws DBException
    {
        final LoginInfo info = getLoginInfo();
        final byte[] stored = info.password;
        if( stored != null )
        {
            if( stored.length == 16 )
            {
                MD5Digest md5 = new MD5Digest();
                final byte[] data = challenge.getBytes();
                md5.update( data, 0, data.length );
                final byte[] digest = new byte[16];
                md5.doFinal( digest, 0 );
                for( int i = 0; i < 16; i++ )
                {
                    if( stored[i] != digest[i] )
                    {
                        return false;
                    }
                }
                return true;
            }
            else
            {
                info.password = null;
                info.save();
                return false;
            }
        }
        else
        {
            return false;
        }
    }

    private static LoginInfo getLoginInfo()
    {
        if( loginInfo == null )
        {
            loadLoginInfo();
        }
        return loginInfo;
    }

    /**
     * Reload login info from store.
     */
    public static synchronized void reload()
    {
        loginInfo = null;
        loadLoginInfo();
    }

    /** @noinspection FieldRepeatedlyAccessedInMethod,JavaDoc*/
    private static synchronized void loadLoginInfo()
    {
        if( loginInfo == null )
        {
            loginInfo = new LoginInfo();
            try
            {
                loginInfo.load();
            }
            catch( DBException e )
            {
                log.error( e );
            }
        }
    }

    /**
     * Get and parse device number from preferences.
     *
     * @return device number
     */
    public static long getDeviceNumber()
    {
        final LoginInfo info = getLoginInfo();
        return info.deviceNumber != null ?
               info.deviceNumber.longValue() :
               1;
    }

    /**
     * Get the device global id.
     *
     * @return device global id
     */
    public static String getDeviceId()
    {
        return getLoginInfo().deviceId;
    }

    /**
     * Get and parse user id from preferences.
     *
     * @return device number
     */
    public static long getUserId()
    {
        final LoginInfo info = getLoginInfo();
        return info.userId != null ?
               info.userId.longValue() :
               -1;
    }

    /**
     * Get user full name.
     * @return user full name
     */
    public static String getUserFullName()
    {
        return getLoginInfo().userFullName;
    }

    /**
     * Get company name.
     * @return company name
     */
    public static String getCompanyName()
    {
        return getLoginInfo().companyName;
    }

    /**
     * Get user custom id.
     *
     * @return user custom id
     */
    public static String getUserCustomId()
    {
        return getLoginInfo().userCustomId;
    }

    /**
     * Get the encrypted password for this user.
     * @return encrypted password
     */
    public static byte[] getEncryptedPassword()
    {
        return getLoginInfo().password;
    }
}
