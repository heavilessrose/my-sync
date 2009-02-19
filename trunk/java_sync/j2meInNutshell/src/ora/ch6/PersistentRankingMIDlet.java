package ora.ch6;

import java.io.IOException;
import java.util.Vector;
import javax.microedition.lcdui.Alert;
import javax.microedition.lcdui.AlertType;
import javax.microedition.lcdui.Command;
import javax.microedition.lcdui.CommandListener;
import javax.microedition.lcdui.Display;
import javax.microedition.lcdui.Displayable;
import javax.microedition.lcdui.Form;
import javax.microedition.lcdui.List;
import javax.microedition.lcdui.Screen;
import javax.microedition.lcdui.StringItem;
import javax.microedition.lcdui.TextField;
import javax.microedition.midlet.MIDlet;
import javax.microedition.midlet.MIDletStateChangeException;
import javax.microedition.rms.RecordEnumeration;
import javax.microedition.rms.RecordListener;
import javax.microedition.rms.RecordStore;
import javax.microedition.rms.RecordStoreException;


public class PersistentRankingMIDlet extends MIDlet
                implements CommandListener, RecordListener, Runnable {

    private Command exitCommand;
    private Command okCommand;
    private Command cancelCommand;
    private Command newCommand;
    private Command checkCommand;
    private Command detailsCommand;
    private Command backCommand;
    private Command deleteCommand;
    private Display display;
    private TextField isbnField;
    private StringItem isbnDisplay;
    private StringItem titleDisplay;
    private StringItem rankingDisplay;
    private StringItem reviewDisplay;
    private StringItem checkTitle;
    private Form isbnForm;
    private Form searchForm;
    private Form resultForm;
    private Form checkForm;
    private List bookList;
    private Vector bookInfoList;
    private Thread searchThread;
    private BookStore bookStore;
    private BookInfo searchBookInfo;

    protected void startApp() throws MIDletStateChangeException {
        if (display == null) {
            initialize();

            // If there are any books in the
            // book store, display the list.
            // Otherwise, start with the ISBN form.
            display.setCurrent(getSelectionScreen());
        }
    }

    protected void pauseApp() {
    }

    protected void destroyApp(boolean unconditional)
                        throws MIDletStateChangeException {
        // Close the book store
        if (bookStore != null) {
            try {
                bookStore.removeRecordListener(this);
                bookStore.close();
            } catch (RecordStoreException ex) {
            }
        }
    }

    public void commandAction(Command cmd, Displayable d) {
        if (cmd == exitCommand) {
            try {
                destroyApp(true);
            } catch (MIDletStateChangeException ex) {
            }
            notifyDestroyed();
        } else if (cmd == okCommand) {
            String isbn = isbnField.getString().trim();
            if (!isbn.equals("")) {
                isbnDisplay.setText(isbn);
                display.setCurrent(searchForm);
                searchForBook(new BookInfo(isbn));
            }
        } else if (cmd == cancelCommand) {
            searchThread = null;
            isbnField.setString(null);
            display.setCurrent(getSelectionScreen());
        } else if (cmd == newCommand) {
            isbnField.setString(null);
            display.setCurrent(isbnForm);
        } else if (cmd == detailsCommand || cmd == List.SELECT_COMMAND) {
            int index = bookList.getSelectedIndex();
            searchBookInfo = (BookInfo)bookInfoList.elementAt(index);
            isbnDisplay.setText(searchBookInfo.getIsbn());
            showBookInfo(searchBookInfo);
        } else if (cmd == deleteCommand) {
            int index = bookList.getSelectedIndex();
            BookInfo bookInfo = (BookInfo)bookInfoList.elementAt(index);
            try {
                bookStore.deleteBook(bookInfo);
            } catch (RecordStoreException ex) {
                System.out.println("Delete failed: " + ex);
            }
        } else if (cmd == checkCommand) {
            String isbn = searchBookInfo.getIsbn();
            checkTitle.setText(searchBookInfo.getTitle());
            display.setCurrent(checkForm);
            searchForBook(searchBookInfo);
        } else if (cmd == backCommand) {
            display.setCurrent(getSelectionScreen());
        }
    }

    public void searchForBook(BookInfo info) {
        searchBookInfo = info;
        searchThread = new Thread(this);
        searchThread.start();
    }

    public void recordAdded(RecordStore recordStore, int recordId) {
        // Update the book list
        populateBookList();
    }

    public void recordChanged(RecordStore recordStore, int recordId) {
        // Update the book list
        populateBookList();
    }

    public void recordDeleted(RecordStore recordStore, int recordId) {
        // Update the book list
        populateBookList();
    }

    public void run() {
        try {
            boolean found = Fetcher.fetch(searchBookInfo);
            if (searchThread == Thread.currentThread()) {
                if (found && searchBookInfo.getTitle() != null) {
                    // Display the book details
                    showBookInfo(searchBookInfo);

                    // Add the new book to the book store
                    bookStore.saveBookInfo(searchBookInfo);
                } else {
                    Alert alert = new Alert("Book not found", null,
                                        null, AlertType.ERROR);
                    alert.setTimeout(Alert.FOREVER);
                    alert.setString("No book with ISBN " +
                                        searchBookInfo.getIsbn() +
                                        " was found.");
                    isbnField.setString(null);
                    display.setCurrent(alert, getSelectionScreen());
                }
            }
        } catch (Throwable ex) {
            if (searchThread == Thread.currentThread()) {
                Alert alert = new Alert("Search Failed", null,
                                        null, AlertType.ERROR);
                alert.setTimeout(Alert.FOREVER);
                alert.setString("Search failed:\n" + ex.getMessage());
                isbnField.setString(null);
                display.setCurrent(alert, getSelectionScreen());
            }
        }
    }

    // Shows book details on the result screen
    private void showBookInfo(BookInfo info) {
        titleDisplay.setText(info.getTitle());

        int ranking = info.getRanking();
        int lastRanking = info.getLastRanking();
        int change = ranking - lastRanking;
        String rankingText =
                ranking == 0 ? "" :
                    String.valueOf(ranking);
        if (change > 0) {
            rankingText += ", down by " + change;
        } else if (change < 0) {
            rankingText += ", UP by " + (-change);
        }
        rankingDisplay.setText(rankingText);

        int reviews = info.getReviews();
        int lastReviews = info.getLastReviews();
        change = reviews - lastReviews;
        String reviewText =
            reviews == 0 ? "" :
                    String.valueOf(reviews);
        if (change > 0) {
            reviewText += ", up by " + change;
        } 
        reviewDisplay.setText(reviewText);

        display.setCurrent(resultForm);
    }

    // If there are any books in the
    // book store, display the list.
    // Otherwise, start with the ISBN form.
    private Screen getSelectionScreen() {
        return
            bookInfoList.isEmpty() ? (Screen)isbnForm : (Screen)bookList;
    }

    // Populates the list of books
    private void populateBookList() {
        // Clear out any existing content
        int count = bookList.size();
        for (int i = 0; i < count; i++) {
            bookList.delete(0);
        }
        bookInfoList.removeAllElements();

        // Add an entry for each book in the store
        try {
            RecordEnumeration enum = bookStore.getBooks();
            while (enum.hasNextElement()) {
                int id = enum.nextRecordId();
                BookInfo info = bookStore.getBookInfo(id);
                bookInfoList.addElement(info);
                String title = info.getTitle();
                if (title == null || title.equals("")) {
                    title = info.getIsbn();
                }
                bookList.append(title, null);
            }
            enum.destroy();
        } catch (Exception ex) {
            // Just leave an empty list.
        }

        // The ISBN list should have an exit command
        // only if the List screen is empty.
        isbnForm.removeCommand(exitCommand);
        if (bookInfoList.isEmpty()) {
            isbnForm.addCommand(exitCommand);
        }
    }

    private void initialize() {
        display = Display.getDisplay(this);

        // Open the book store
        bookStore = new BookStore();
 
        exitCommand = new Command("Exit", Command.EXIT, 0);
        okCommand = new Command("OK", Command.OK, 0);
        cancelCommand = new Command("Cancel", Command.CANCEL, 0);
        newCommand = new Command("New", Command.SCREEN, 1);
        detailsCommand = new Command("Details", Command.OK, 0);
        checkCommand = new Command("Check", Command.OK, 0);
        backCommand = new Command("Back", Command.CANCEL, 1);
        deleteCommand = new Command("Delete", Command.SCREEN, 1);

        bookList = new List("Books", List.IMPLICIT);
        bookList.addCommand(detailsCommand);
        bookList.addCommand(newCommand);
        bookList.addCommand(deleteCommand);
        bookList.addCommand(exitCommand);
        bookInfoList = new Vector();

        isbnForm = new Form("Book Query");
        isbnForm.append("Enter an ISBN and press OK:");
        isbnField = new TextField("", null, 10, TextField.ANY);
        isbnForm.append(isbnField);
        isbnForm.addCommand(okCommand);
        isbnForm.addCommand(exitCommand);
        isbnForm.addCommand(backCommand);

        searchForm = new Form("Book Search");
        searchForm.append("Searching for ISBN\n");
        isbnDisplay = new StringItem(null, null);
        searchForm.append(isbnDisplay);
        searchForm.append("\nPlease wait....");
        searchForm.addCommand(cancelCommand);

        checkForm = new Form("Details Update");
        checkForm.append("Getting details for\n");
        checkTitle = new StringItem(null, null);
        checkForm.append(checkTitle);
        checkForm.append(" from amazon.com\nPlease wait....");
        checkForm.addCommand(cancelCommand);

        resultForm = new Form("Search Results");
        titleDisplay = new StringItem("Book title: ", null);
        rankingDisplay = new StringItem("Ranking:    ", null);
        reviewDisplay = new StringItem("Reviews:    ", null);
        resultForm.append(titleDisplay);
        resultForm.append(rankingDisplay);
        resultForm.append(reviewDisplay);
        resultForm.addCommand(backCommand);
        resultForm.addCommand(checkCommand);

        // Register for events from all of the forms
        isbnForm.setCommandListener(this);
        searchForm.setCommandListener(this);
        resultForm.setCommandListener(this);
        bookList.setCommandListener(this);

        // Listen for changes in the content of the book store
        bookStore.addRecordListener(this);

        // Install the books held in the record store
        populateBookList();
    }
}