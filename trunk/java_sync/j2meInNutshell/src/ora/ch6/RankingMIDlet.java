package ora.ch6;

import java.io.IOException;
import javax.microedition.lcdui.Alert;
import javax.microedition.lcdui.AlertType;
import javax.microedition.lcdui.Command;
import javax.microedition.lcdui.CommandListener;
import javax.microedition.lcdui.Display;
import javax.microedition.lcdui.Displayable;
import javax.microedition.lcdui.Form;
import javax.microedition.lcdui.StringItem;
import javax.microedition.lcdui.TextField;
import javax.microedition.midlet.MIDlet;
import javax.microedition.midlet.MIDletStateChangeException;


public class RankingMIDlet extends MIDlet
                        implements CommandListener, Runnable {

    private Command exitCommand;
    private Command okCommand;
    private Command cancelCommand;
    private Command newCommand;
    private Display display;
    private TextField isbnField;
    private StringItem isbnDisplay;
    private StringItem titleDisplay;
    private StringItem rankingDisplay;
    private StringItem reviewDisplay;
    private Form isbnForm;
    private Form searchForm;
    private Form resultForm;
    private BookInfo searchBookInfo;
    private Thread searchThread;

    protected void startApp() throws MIDletStateChangeException {
        if (display == null) {
            initialize();
            display.setCurrent(isbnForm);
        }
    }

    protected void pauseApp() {
    }

    protected void destroyApp(boolean unconditional)
                        throws MIDletStateChangeException {

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
                searchForBook(new BookInfo(isbn));
            }
        } else if (cmd == cancelCommand) {
            searchThread = null;
            isbnField.setString(null);
            display.setCurrent(isbnForm);
        } else if (cmd == newCommand) {
            isbnField.setString(null);
            display.setCurrent(isbnForm);
        }
    }

    public void searchForBook(BookInfo info) {
        searchBookInfo = info;
        isbnDisplay.setText(info.getIsbn().trim());
        display.setCurrent(searchForm);
        searchThread = new Thread(this);
        searchThread.start();
    }

    public void run() {
        try {
            boolean found = Fetcher.fetch(searchBookInfo);
            if (searchThread == Thread.currentThread()) {
                if (found && searchBookInfo.getTitle() != null) {
                    titleDisplay.setText(searchBookInfo.getTitle());
                    rankingDisplay.setText(
                        searchBookInfo.getRanking() == 0 ? "" :
                                String.valueOf(searchBookInfo.getRanking()));
                    reviewDisplay.setText(
                        searchBookInfo.getReviews() == 0 ? "" :
                                String.valueOf(searchBookInfo.getReviews()));
                    display.setCurrent(resultForm);
                } else {
                    Alert alert = new Alert("Book not found", null,
                                        null, AlertType.ERROR);
                    alert.setTimeout(Alert.FOREVER);
                    alert.setString("No book with ISBN " +
                                        searchBookInfo.getIsbn() +
                                        " was found.");
                    isbnField.setString(null);
                    display.setCurrent(alert, isbnForm);
                }
            }
        } catch (Throwable ex) {
            if (searchThread == Thread.currentThread()) {
                Alert alert = new Alert("Search Failed", null,
                                        null, AlertType.ERROR);
                alert.setTimeout(Alert.FOREVER);
                alert.setString("Search failed:\n" + ex.getMessage());
                isbnField.setString(null);
                display.setCurrent(alert, isbnForm);
            }
        }
    }

    private void initialize() {
        display = Display.getDisplay(this);

        exitCommand = new Command("Exit", Command.EXIT, 0);
        okCommand = new Command("OK", Command.OK, 0);
        cancelCommand = new Command("Cancel", Command.CANCEL, 0);
        newCommand = new Command("New", Command.SCREEN, 1);

        isbnForm = new Form("Book Query");
        isbnForm.append("Enter an ISBN and press OK:");
        isbnField = new TextField("", null, 10, TextField.ANY);
        isbnForm.append(isbnField);
        isbnForm.addCommand(okCommand);
        isbnForm.addCommand(exitCommand);

        searchForm = new Form("Book Search");
        searchForm.append("Searching for ISBN\n");
        isbnDisplay = new StringItem(null, null);
        searchForm.append(isbnDisplay);
        searchForm.append("\nPlease wait....");
        searchForm.addCommand(cancelCommand);

        resultForm = new Form("Search Results");
        titleDisplay = new StringItem("Book title: ", null);
        rankingDisplay = new StringItem("Ranking:    ", null);
        reviewDisplay = new StringItem("Reviews:    ", null);
        resultForm.append(titleDisplay);
        resultForm.append(rankingDisplay);
        resultForm.append(reviewDisplay);
        resultForm.addCommand(newCommand);
        resultForm.addCommand(exitCommand);

        // Register for events from all of the forms
        isbnForm.setCommandListener(this);
        searchForm.setCommandListener(this);
        resultForm.setCommandListener(this);
    }
}