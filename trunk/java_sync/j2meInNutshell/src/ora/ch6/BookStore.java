package ora.ch6;

import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import javax.microedition.rms.RecordComparator;
import javax.microedition.rms.RecordEnumeration;
import javax.microedition.rms.RecordFilter;
import javax.microedition.rms.RecordListener;
import javax.microedition.rms.RecordStore;
import javax.microedition.rms.RecordStoreException;

// A class that implements a persistent store
// of books, keyed by ISBN.
public class BookStore implements RecordComparator, RecordFilter {

    // The name of the record store used to hold books
    private static final String STORE_NAME = "BookStore";

    // The record store itself
    private RecordStore store;

    // ISBN to be used during a filter operation
    private String searchISBN;

    // Creates a bookstore and opens it
    public BookStore() {
        try {
            store = RecordStore.openRecordStore(STORE_NAME, true);
        } catch (RecordStoreException ex) {
            System.err.println(ex);
        }
    }

    // Closes the bookstore
    public void close() throws RecordStoreException {
        if (store != null) {
            store.closeRecordStore();
        }
    }

    // Gets the number of books in the book store
    public int getBookCount() throws RecordStoreException {
        if (store != null) {
            return store.getNumRecords();
        }
        return 0;
    }

    // Adds a listener to the book store
    public void addRecordListener(RecordListener l) {
        if (store != null) {
            store.addRecordListener(l);
        }
    }

    // Removes a listener from the book store
    public void removeRecordListener(RecordListener l) {
        if (store != null) {
            store.removeRecordListener(l);
        }
    }

    // Gets a sorted list of all of the books in
    // the store.
    public RecordEnumeration getBooks() throws RecordStoreException {
        if (store != null) {
            return store.enumerateRecords(null, this, false);
        }
        return null;
    }

    
    // Gets a BookInfo from a store record
    // given its ISBN
    public BookInfo getBookInfo(String isbn) throws RecordStoreException,
                                                    IOException {
        BookInfo bookInfo = null;
        searchISBN = isbn; 
        
        // Look for a book with the given ISBN
        RecordEnumeration enum = store.enumerateRecords(
                                        this, null, false);
        
        // If found, get its identifier and
        // fetch its BookInfo object
        if (enum.numRecords() > 0) {
            int id = enum.nextRecordId();
            bookInfo = getBookInfo(id);
        }
        
        // Release the enumeration
        enum.destroy();
        
        return bookInfo;
    }
        
    // Gets a BookInfo from a store record
    // given its record identifier
    public BookInfo getBookInfo(int id) throws RecordStoreException,
                                                    IOException {
        byte[] bytes = store.getRecord(id);
        DataInputStream is = new DataInputStream(
                            new ByteArrayInputStream(bytes));

        String isbn = is.readUTF();
        BookInfo info = new BookInfo(isbn);
        info.id = id;
        info.title = is.readUTF();
        info.ranking = is.readInt();
        info.reviews = is.readInt();
        info.lastRanking = is.readInt();
        info.lastReviews = is.readInt();

        return info;
    }
    
    // Adds an entry to the store or modifies the existing
    // entry if a matching ISBN exists.
    public void saveBookInfo(BookInfo bookInfo)
                                throws IOException, RecordStoreException {
        if (store != null) {
            searchISBN = bookInfo.getIsbn();
            RecordEnumeration enum = store.enumerateRecords(
                                        this, null, false);
            if (enum.numRecords() > 0) {
                // A matching record exists. Set the id
                // of the BookInfo to match the existing record
                bookInfo.id = enum.nextRecordId();
                byte[] bytes = toByteArray(bookInfo);
                store.setRecord(bookInfo.id, bytes, 0, bytes.length);
            } else {
                // Create a new record
                bookInfo.id = store.getNextRecordID();
                byte[] bytes = toByteArray(bookInfo);
                store.addRecord(bytes, 0, bytes.length);
            }

            // Finally, destroy the RecordEnumeration
            enum.destroy();
        }
    }

    // Deletes the entry for a book from the store
    public void deleteBook(BookInfo bookInfo) throws RecordStoreException {
        if (store != null) {
            store.deleteRecord(bookInfo.id);
        }
    }

    // RecordComparator implementation
    public int compare(byte[] book1, byte[] book2) {
        try {
            DataInputStream stream1 =
                new DataInputStream(new ByteArrayInputStream(book1));
            DataInputStream stream2 =
                new DataInputStream(new ByteArrayInputStream(book2));

            // Match based on the ISBN, but sort based on the title.
            String isbn1 = stream1.readUTF();
            String isbn2 = stream2.readUTF();
            if (isbn1.equals(isbn2)) {
                return RecordComparator.EQUIVALENT;
            }
            String title1 = stream1.readUTF();
            String title2 = stream2.readUTF();
            int result = title1.compareTo(title2);
            if (result == 0) {
                return RecordComparator.EQUIVALENT;
            }
            return result < 0 ? RecordComparator.PRECEDES :
                                RecordComparator.FOLLOWS;
        } catch (IOException ex) {
            return RecordComparator.EQUIVALENT;
        }
    }

    // RecordFilter implementation
    public boolean matches(byte[] book) {
        if (searchISBN != null) {
            try {
                DataInputStream stream =
                    new DataInputStream(new ByteArrayInputStream(book));

                // Match based on the ISBN.
                return searchISBN.equals(stream.readUTF());
            } catch (IOException ex) {
                System.err.println(ex);
            }
        }

        // Default is not to match
        return false;
    }
    
    // Writes a record into a byte array.
    private byte[] toByteArray(BookInfo bookInfo) throws IOException {
        ByteArrayOutputStream baos = new ByteArrayOutputStream();
        DataOutputStream os = new DataOutputStream(baos);

        os.writeUTF(bookInfo.isbn);
        os.writeUTF(bookInfo.title == null ? "" : bookInfo.title);
        os.writeInt(bookInfo.ranking);
        os.writeInt(bookInfo.reviews);
        os.writeInt(bookInfo.lastRanking);
        os.writeInt(bookInfo.lastReviews);

        return baos.toByteArray();
    }
}