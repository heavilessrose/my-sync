package ora.ch6;

import java.io.InterruptedIOException;
import javax.microedition.io.Connector;
import javax.microedition.io.Datagram;
import javax.microedition.io.DatagramConnection;
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

public class TimeMIDlet extends MIDlet
                    implements CommandListener, Runnable {

    private Display display;
    private Form addressForm;
    private Form connectForm;
    private Form displayForm;
    private Command backCommand;
    private Command exitCommand;
    private Command okCommand;
    private StringItem messageLabel;
    private TextField serverName;

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
        DatagramConnection conn = null;
        display.setCurrent(connectForm);

        try {
            // Build the name string for the Connector open method
            String server = serverName.getString();
            String name = "datagram://" + server + ":" + 13;
            conn = (DatagramConnection)Connector.open(name,
                                    Connector.READ_WRITE, false);

            // Build and send an empty datagram
            Datagram dg = conn.newDatagram(10);
            dg.setData("Hello".getBytes(), 0, 5);
            conn.send(dg);

            // Receive the reply
            Datagram rdg = conn.newDatagram(512);
            conn.receive(rdg);
            messageLabel.setText(new String(rdg.getData(), 0, rdg.getLength()));
            display.setCurrent(displayForm);

        } catch (InterruptedIOException iex) {
            display.callSerially(new Runnable() {
                public void run() {
                    Alert alert = new Alert("No Reply",
                        "No reply was received.\n" +
                        "Please check the server address and try again.", null,
                        AlertType.ERROR);
                    alert.setTimeout(Alert.FOREVER);
                    display.setCurrent(alert, addressForm);
                }
            });
            return;
        } catch (Exception ex) {
            display.callSerially(new Runnable() {
                public void run() {
                    Alert alert = new Alert("Invalid Address",
                        "The supplied address is invalid\n" +
                        "Please correct it and try again.", null,
                        AlertType.ERROR);
                    alert.setTimeout(Alert.FOREVER);
                    display.setCurrent(alert, addressForm);
                }
            });
            return;
        } catch (Error err) {
            System.out.println(err);
            err.printStackTrace();
        }
    }

    private void initialize() {
        display = Display.getDisplay(this);

        // Commands
        exitCommand = new Command("Exit", Command.EXIT, 0);
        okCommand = new Command("OK", Command.OK, 0);
        backCommand = new Command("Back", Command.BACK, 0);

        // The address form
        addressForm = new Form("Network Time");
        serverName = new TextField("Time Server name:", "tock.usno.navy.mil",
                                            256, TextField.ANY);
        addressForm.append(serverName);
        addressForm.addCommand(okCommand);
        addressForm.addCommand(exitCommand);
        addressForm.setCommandListener(this);

        // The connect form
        connectForm = new Form("Sending");
        messageLabel = new StringItem(null,
                    "Sending the datagram...\nPlease wait.");
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