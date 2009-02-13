package bm.storage;

import java.util.Random;
import java.util.List;
import java.util.Vector;
import java.util.StringTokenizer;
import java.io.*;
/*
 * File Information
 *
 * Created on       : 30-abr-2007 16:02:05
 * Created by       : narciso
 * Last modified by : $Author$
 * Last modified on : $Date$
 * Revision         : $Revision$
 */

/**
 * Generate, save and load test loads for StoreTest.
 *
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision$
 */
public class StoreTestLoad
{
    public static final byte OP_ADD  = 0;
    public static final byte OP_MODIFY = 1;
    public static final byte OP_DELETE = 2;

    private List operations = new Vector( 1000 );

    public static void main( final String[] args )
    {
        new StoreTestLoad().generate( args[0] );
    }

    private void writeAdd( final PrintWriter out, final int size, final int content )
    {
        out.println( "add," + size + "," + content );
    }

    private void writeModify(
            final PrintWriter   out,
            final int           recordId,
            final int           size,
            final int           content
    )
    {
        out.println( "set," + recordId + "," + size + "," + content );
    }

    private void writeDelete( final PrintWriter out, final int recordId )
    {
        out.println( "delete," + recordId );
    }

    public void generate( final String name )
    {
        final Random random = new Random( System.currentTimeMillis() );
        final int size = 1000 + random.nextInt( 3000 );
        Writer writer = null;
        PrintWriter out = null;
        try
        {
            writer = new FileWriter( name );
            out = new PrintWriter( writer );

            for( int i = 0; i < size; i++ )
            {
                writeAdd( out, 100 + random.nextInt( 50 ), random.nextInt( 128 ) );
            }

            final int operations = 400 + random.nextInt( 1000 );
            for( int i = 0; i < operations; i++ )
            {
                final byte operation = (byte) ( 1 + random.nextInt( 2 ) );
                switch( operation )
                {
                    case OP_MODIFY:
                    {
                        final int recordId = 1 + random.nextInt( size );
                        writeModify(
                                out,
                                recordId,
                                100 + random.nextInt( 50 ),
                                random.nextInt( 128 )
                        );
                        writeModify(
                                out,
                                recordId,
                                100 + random.nextInt( 50 ),
                                random.nextInt( 128 )
                        );
                    }
                        break;

                    case OP_DELETE:
                    {
                        final int recordId = 1 + random.nextInt( size );
                        writeDelete( out, recordId );
                        writeModify(
                                out,
                                recordId,
                                100 + random.nextInt( 50 ),
                                random.nextInt( 128 )
                        );
                        writeDelete( out, recordId );
                    }
                        break;
                }
            }

            for( int i = 0; i < (100 + random.nextInt( 100 )); i++ )
            {
                writeAdd( out, 100 + random.nextInt( 50 ), random.nextInt( 128 ) );
            }

            out.println( "end" );
        }
        catch( Exception e )
        {
            e.printStackTrace();
        }
        finally
        {
            if( out != null ) try{ out.close(); }catch( Exception e ){}
            if( writer != null ) try{ writer.close(); }catch( Exception e ){}
        }
    }

    public List getOperations()
    {
        return operations;
    }

    public void load( final InputStream is )
            throws IOException
    {
        InputStreamReader in = null;
        try
        {
            in = new InputStreamReader( is );
            load( in );
        }
        finally
        {
            if( in != null ) try{ in.close(); }catch( Exception e ){}
        }
    }

    public void load( final Reader reader )
            throws IOException
    {
        BufferedReader in = reader instanceof BufferedReader ?
                            (BufferedReader) reader :
                            new BufferedReader( reader );
        Operation op = new Operation( in );
        while( op.code != -1 )
        {
            operations.add( op );
            op = new Operation( in );
        }
    }

    public void load( final String name )
            throws IOException
    {
        FileReader reader = null;
        try
        {
            reader = new FileReader( name );
            load( reader );
        }
        finally
        {
            if( reader != null ) try{ reader.close(); }catch( Exception e ){}
        }
    }

    public class Operation
    {
        byte    code;
        int     size;
        byte    content;
        int     recordId;

        public Operation( final BufferedReader in )
                throws IOException
        {
            final StringTokenizer stk = new StringTokenizer( in.readLine(), "," );
            String token = stk.nextToken();
            if( token.equalsIgnoreCase( "end" ) )
            {
                code = -1;
            }
            else if( token.equalsIgnoreCase( "add" ) )
            {
                code = OP_ADD;
                size = Integer.parseInt( stk.nextToken() );
                content = Integer.valueOf( stk.nextToken() ).byteValue();
            }
            else if( token.equalsIgnoreCase( "set" ) )
            {
                code = OP_MODIFY;
                recordId = Integer.parseInt( stk.nextToken() );
                size = Integer.parseInt( stk.nextToken() );
                content = Integer.valueOf( stk.nextToken() ).byteValue();
            }
            else if( token.equalsIgnoreCase( "delete" ) )
            {
                code = OP_DELETE;
                recordId = Integer.parseInt( stk.nextToken() );
            }            
        }
    }
}
