package com.basemovil.vc;

import bm.db.index.Index;
import bm.core.io.SerializerOutputStream;

import java.io.ByteArrayOutputStream;
/*
 * File Information
 *
 * Created on       : 17-oct-2007 18:14:26
 * Created by       : narciso
 * Last modified by : $Author$
 * Last modified on : $Date$
 * Revision         : $Revision$
 */

/**
 * Controller bean.
 *
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision$
 */
public class Controller
    extends TopLevelElement
{
    private String id;
    private String className;

    public String getId()
    {
        return id;
    }

    public void setId( final String id )
    {
        this.id = id;
    }

    public String getClassName()
    {
        return className;
    }

    public void setClassName( final String className )
    {
        this.className = className;
    }


    /**
     * Store the binary element definition in the index.
     *
     * @param index index
     * @throws com.basemovil.vc.ViewCompilerException
     *          on errors
     */
    public void store( final Index index )
            throws ViewCompilerException
    {
        try
        {
            final ByteArrayOutputStream baos = new ByteArrayOutputStream();
            final SerializerOutputStream out = new SerializerOutputStream( baos );

            out.writeByte( (byte) 1 );
            out.writeString( className );
            out.flush();
            out.close();

            index.insertObject( "controller." + id, baos.toByteArray() );
        }
        catch( Exception e )
        {
            throw new ViewCompilerException( e );
        }
    }
}
