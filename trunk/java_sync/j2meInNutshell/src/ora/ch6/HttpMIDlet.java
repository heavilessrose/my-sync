package ora.ch6;

import java.io.InputStream;
import java.io.IOException;
import java.io.OutputStream;
import javax.microedition.io.Connector;
import javax.microedition.io.HttpConnection;
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

public class HttpMIDlet extends MIDlet
                    implements CommandListener, Runnable {

    private Display display;
    private Form addressForm;
    private Form connectForm;
    private Form displayForm;
    private TextField serverURL;
    private StringItem messageLabel;
    private StringItem errorLabel;
    private Command okCommand;
    private Command exitCommand;
    private Command backCommand;

    protected void startApp() throws MIDletStateChangeException {
        if (display == null) {
            initialize();
            display.setCurrent(addressForm);
        }
    }

    protected void pauseApp() {
    }

    protected void destroyApp(boolean unconditional)
                        throws MIDletStateChangeException {
    }

    public void commandAction(Command cmd, Displayable d) {
        if (cmd == okCommand) {
            Thread t = new Thread(this);
            t.start();
            display.setCurrent(connectForm);
        } else if (cmd == backCommand) {
            display.setCurrent(addressForm);
        } else if (cmd == exitCommand) {
            try {
                destroyApp(true);
            } catch (MIDletStateChangeException ex) {
            }
            notifyDestroyed();
        }
    }

    public void run() {
        InputStream is = null;
        HttpConnection conn = null;

        try {
            String url = serverURL.getString();
            if (!url.startsWith("http://") && 
			!url.startsWith("https://")) {
                url = "http://" + url;
            }
            conn = (HttpConnection)Connector.open(url, Connector.READ_WRITE);
        } catch (Exception ex) {
		System.out.println(ex);
		ex.printStackTrace();
            Alert alert = new Alert("Invalid Address",
                        "The supplied address is invalid\n" +
                        "Please correct it and try again.", null,
                        AlertType.ERROR);
            alert.setTimeout(Alert.FOREVER);
            display.setCurrent(alert, addressForm);
            return;
        }

        try {
            // Fetch the required page, reading up
            // to a maximum of 128 bytes
            if (conn.getResponseCode() == HttpConnection.HTTP_OK) {
                is = conn.openInputStream();
                final int MAX_LENGTH = 128;
                byte[] buf = new byte[MAX_LENGTH];
                int total = 0;
                while (total < MAX_LENGTH) {
                    int count = is.read(buf, total, MAX_LENGTH - total);
                    if (count < 0) {
                        break;
                    }
                    total += count;
                }
                is.close();
                String reply = new String(buf, 0, total);
                messageLabel.setText(reply);
            } else {
                messageLabel.setText("Failed: error " + conn.getResponseCode() +
                                "\n" + conn.getResponseMessage());
            }
            
            // Get the response code and print all the headers
            System.out.println("Response code = " + conn.getResponseCode());
            System.out.println("Response message = " + conn.getResponseMessage());
            for (int i = 0; ; i++) {
                String key = conn.getHeaderFieldKey(i);
                String value = conn.getHeaderField(i);
                if (key == null) {
                    // Reached last header
                    break;
                }
                System.out.println(key + " = " + value);
            }
            conn.close();
            display.setCurrent(displayForm);
        } catch (IOException ex) {
		System.out.println(ex);
		ex.printStackTrace();
            Alert alert = new Alert("I/O Error",
                        "An error occurred while communicating with the server.",
                        null, AlertType.ERROR);
            alert.setTimeout(Alert.FOREVER);
            display.setCurrent(alert, addressForm);
            return;
        } finally {
            // Close open streams
           try {
                if (is != null) {
                    is.close();
                    is = null;
                }
            } catch (IOException ex1) {
            }
            try {
                if (conn != null) {
                    conn.close();
                    conn = null;
                }
            } catch (IOException ex1) {
            }
        }
    }

    private void initialize() {
        display = Display.getDisplay(this);

        // Commands
        exitCommand = new Command("Exit", Command.EXIT, 0);
        okCommand = new Command("OK", Command.OK, 0);
        backCommand = new Command("Back", Command.BACK, 0);

        // The address form
        addressForm = new Form("HTTP Client");
        serverURL = new TextField("URL:", "", 256, TextField.ANY);
        addressForm.append(serverURL);
        addressForm.addCommand(okCommand);
        addressForm.addCommand(exitCommand);
        addressForm.setCommandListener(this);

        // The connect form
        connectForm = new Form("Connecting");
        messageLabel = new StringItem(null, "Connecting...\nPlease wait.");
        connectForm.append(messageLabel);
        connectForm.addCommand(backCommand);
        connectForm.setCommandListener(this);

        // The display form
        displayForm = new Form("Server Reply");
        messageLabel = new StringItem(null, null);
        displayForm.append(messageLabel);
        displayForm.addCommand(backCommand);
        displayForm.setCommandListener(this);
    }
}