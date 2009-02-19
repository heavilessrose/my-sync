package ora.ch6;

import java.io.InputStream;
import java.io.IOException;
import java.io.OutputStream;
import javax.microedition.io.Connector;
import javax.microedition.io.StreamConnection;
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

public class SocketMIDlet extends MIDlet
                    implements CommandListener, Runnable {

    private Display display;
    private Form addressForm;
    private Form connectForm;
    private Form displayForm;
    private TextField serverName;
    private TextField serverPort;
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
        OutputStream os = null;
        StreamConnection socket = null;

        try {
            String server = serverName.getString();
            String port = serverPort.getString();
            String name = "socket://" + server + ":" + port;
            socket = (StreamConnection)Connector.open(name, Connector.READ_WRITE);
        } catch (Exception ex) {
            Alert alert = new Alert("Invalid Address",
                        "The supplied address is invalid\n" +
                        "Please correct it and try again.", null,
                        AlertType.ERROR);
            alert.setTimeout(Alert.FOREVER);
            display.setCurrent(alert, addressForm);
            return;
        }

        try {
            // Send a message to the server
            String request = "GET / HTTP/1.0\n\n";

            os = socket.openOutputStream();
            os.write(request.getBytes());
            os.close();

            // Read the server's reply, up to a maximum
            // of 128 bytes.
            is = socket.openInputStream();
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
            socket.close();
            display.setCurrent(displayForm);
        } catch (IOException ex) {
            Alert alert = new Alert("I/O Error",
                        "An error occurred while communicating with the server.",
                        null, AlertType.ERROR);
            alert.setTimeout(Alert.FOREVER);
            display.setCurrent(alert, addressForm);
            return;
        } finally {
            // Close open streams and the socket
            try {
                if (is != null) {
                    is.close();
                    is = null;
                }
            } catch (IOException ex1) {
            }
            try {
                if (os != null) {
                    os.close();
                    os = null;
                }
            } catch (IOException ex1) {
            }
            try {
                if (socket != null) {
                    socket.close();
                    socket = null;
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
        addressForm = new Form("Socket Client");
        serverName = new TextField("Server name:", "", 256, TextField.ANY);
        serverPort = new TextField("Server port:", "", 8, TextField.NUMERIC);
        addressForm.append(serverName);
        addressForm.append(serverPort);
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