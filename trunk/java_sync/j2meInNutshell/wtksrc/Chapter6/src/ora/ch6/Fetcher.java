package ora.ch6;

import java.io.InputStream;
import java.io.IOException;
import java.io.OutputStream;
import javax.microedition.io.Connector;
import javax.microedition.io.HttpConnection;

public class Fetcher {

    private static final String BASE_URL = "http://www.amazon.com";
    private static final String QUERY_URL = BASE_URL +
                                "/exec/obidos/search-handle-form/0";

    private static final int MAX_REDIRECTS = 5;

    // Fetches the title, ranking and review count
    // for a book with a given ISBN.
    public static boolean fetch(BookInfo info) throws IOException {
        InputStream is = null;
        OutputStream os = null;
        HttpConnection conn = null;
        int redirects = 0;
        try {
            String isbn = info.getIsbn();
            String query = "index=books&field-keywords=" + isbn + "\r\n";
            String requestMethod = HttpConnection.POST;
            String name = QUERY_URL;

            while (redirects < MAX_REDIRECTS) {
                conn = (HttpConnection)Connector.open(name,
                                                    Connector.READ_WRITE);

                // Send the ISBN number to perform the query
                conn.setRequestMethod(requestMethod);
                conn.setRequestProperty("Connection", "Close");
                if (requestMethod.equals(HttpConnection.POST)) {
                    conn.setRequestProperty("Content-Type",
                                "application/x-www-form-urlencoded");
                    os = conn.openOutputStream();
                    os.write(query.getBytes());
                    os.close();
                    os = null;                    
                }

                // Read the response from the server
                is = conn.openInputStream();
                int code = conn.getResponseCode();

                // If we get a redirect, try again at the new location
                if ((code >= HttpConnection.HTTP_MOVED_PERM &&
                        code <= HttpConnection.HTTP_SEE_OTHER) ||
                        code == HttpConnection.HTTP_TEMP_REDIRECT) {
                    // Get the URL of the new location (always absolute)
                    name = conn.getHeaderField("Location");
                    is.close();
                    conn.close();
                    is = null;
                    conn = null;

                    if (++redirects > MAX_REDIRECTS) {
                        // Too many redirects - give up.
                        break;
                    }

                    // Choose the appropriate request method
                    requestMethod = HttpConnection.POST;
                    if (code == HttpConnection.HTTP_MOVED_TEMP ||
                            code == HttpConnection.HTTP_SEE_OTHER) {
                        requestMethod = HttpConnection.GET;
                    }
                    continue;
                }
                String type = conn.getType();
                if (code == HttpConnection.HTTP_OK &&
                                    type.equals("text/html")) {
                    info.setFromInputStream(is);
                    return true;
                }
            }
        } catch (Throwable t) {
            System.out.println(t);
        } finally {
            if (is != null) {
                try {
                    is.close();
                } catch (IOException ex) {
                }
            }
            if (os != null) {
                try {
                    os.close();
                } catch (IOException ex) {
                }
            }
            if (conn != null) {
                try {
                    conn.close();
                } catch (IOException ex) {
                }
            }            
        }
        return false;
    }
}
