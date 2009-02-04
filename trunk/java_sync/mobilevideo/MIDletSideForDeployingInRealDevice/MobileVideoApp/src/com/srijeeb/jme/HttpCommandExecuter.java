package com.srijeeb.jme;

import java.util.*;
import java.io.*;
import javax.microedition.io.*;
import com.srijeeb.dto.*;



public class HttpCommandExecuter {


	private static HttpCommandExecuter aHttpCommandExecuter = new HttpCommandExecuter();
	private HttpCommandExecuter(){
	}

	public static HttpCommandExecuter getInstance() {
		return aHttpCommandExecuter;
	}


	private static String serviceURL = "";

	public static void setServiceURL(String serviceURL_) {
		serviceURL = serviceURL_;
	}

	public HttpConnection openConnection() throws IOException {

        try {

            System.out.println("ServiceURL=" + serviceURL);
            HttpConnection connection =
                (HttpConnection) Connector.open(serviceURL);

            connection.setRequestProperty("User-Agent",
                                          System.getProperty("microedition.profiles"));
            connection.setRequestProperty("Content-Type",
                                          "application/octet-stream");
            connection.setRequestMethod(HttpConnection.POST);

            System.out.println("Connection Established...");

            return connection;
        } catch (IOException ioe) {

            ioe.printStackTrace();
            throw ioe;
        }
        catch (Exception e) {
            e.printStackTrace();
            return null;
        }
	}

    public void closeConnection(HttpConnection connection,
                         DataOutputStream outputStream,
                         DataInputStream inputStream) {

        if (outputStream != null) {
            try {
                outputStream.close();
            } catch (IOException ioe) {}    // Ignored
            catch(Exception e){}//Ignored
        }

        if (inputStream != null) {
            try {
                inputStream.close();
            } catch (IOException ioe) {}    // Ignored
             catch(Exception e){}//Ignored
        }

        if (connection != null) {
            try {
                connection.close();
            } catch (IOException ioe) {}    // Ignored
             catch(Exception e){}//Ignored
        }

        return;
    }


    public DataOutputStream openConnectionOutputStream(HttpConnection connection)
            throws IOException {
        try {
            return connection.openDataOutputStream();
        } catch (IOException ioe) {

           ioe.printStackTrace();
            throw ioe;
        }
        catch (Exception e) {
            e.printStackTrace();
            return null;
        }
    }

    public DataInputStream openConnectionInputStream(HttpConnection connection)
            throws Exception {
        try {
            int responseCode = connection.getResponseCode();
            System.out.println("Response Code: " + responseCode);
            if (responseCode == HttpConnection.HTTP_OK
                    || responseCode == HttpConnection.HTTP_CREATED) {

                DataInputStream inputStream = connection.openDataInputStream();

				return inputStream;

            }
        } catch (IOException ioe) {

            ioe.printStackTrace();
            throw ioe;
        }

        catch (Exception e) {
            e.printStackTrace();

            return null;
        }
        return null;
    }


    public synchronized DTOMediaStoreOut executeHttp(DTOMediaStoreIn input) throws Exception {

 		HttpConnection connection = null;
 		DataOutputStream outputStream = null;
 		DataInputStream inputStream = null;
 		try {
 			connection = openConnection();
 			outputStream = openConnectionOutputStream(connection);
 			outputStream.writeByte(1);
 			input.serialize(outputStream);
 			outputStream.close();
 			inputStream = openConnectionInputStream(connection);
 			DTOMediaStoreOut output = DTOMediaStoreOut.deserialize(inputStream);
 			return output;
 		} catch (IOException ioe) {
 			throw ioe;
 		}
 		finally {
 			closeConnection(connection, outputStream, inputStream);
 		}

	}



    public synchronized DTOMediaRetrieveOut executeHttp(DTOMediaRetrieveIn input) throws Exception {

 		HttpConnection connection = null;
 		DataOutputStream outputStream = null;
 		DataInputStream inputStream = null;
 		try {
 			connection = openConnection();
 			outputStream = openConnectionOutputStream(connection);
 			outputStream.writeByte(2);
 			input.serialize(outputStream);
 			outputStream.close();
 			inputStream = openConnectionInputStream(connection);
 			DTOMediaRetrieveOut output = DTOMediaRetrieveOut.deserialize(inputStream);
 			return output;
 		} catch (IOException ioe) {
 			throw ioe;
 		}
 		finally {
 			closeConnection(connection, outputStream, inputStream);
 		}

	}

}