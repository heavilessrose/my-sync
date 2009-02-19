package ora.ch4;

import java.io.IOException;
import java.util.Calendar;
import java.util.Date;
import javax.microedition.lcdui.Alert;
import javax.microedition.lcdui.AlertType;
import javax.microedition.lcdui.Choice;
import javax.microedition.lcdui.ChoiceGroup;
import javax.microedition.lcdui.Command;
import javax.microedition.lcdui.CommandListener;
import javax.microedition.lcdui.DateField;
import javax.microedition.lcdui.Display;
import javax.microedition.lcdui.Displayable;
import javax.microedition.lcdui.Form;
import javax.microedition.lcdui.Gauge;
import javax.microedition.lcdui.Image;
import javax.microedition.lcdui.ImageItem;
import javax.microedition.lcdui.Item;
import javax.microedition.lcdui.ItemStateListener;
import javax.microedition.lcdui.List;
import javax.microedition.lcdui.Screen;
import javax.microedition.lcdui.StringItem;
import javax.microedition.lcdui.TextField;
import javax.microedition.midlet.MIDlet;

public class ItemMIDlet extends MIDlet 
        implements CommandListener, ItemStateListener {

    // The MIDlet's Display object
    private Display display;
        
    // Flag indicating first call of startApp
    protected boolean started;
    
    // Exit command
    private Command exitCommand;
    
    // Back to examples list command
    private Command backCommand;
    
    // The example selection list
    private List examplesList;
    
    // The Screens used to demonstrate different Items
    private Screen[] screens;
    
    // The example names. Used to populate the list.
    private String[] examples = {
        "StringItem", "TextField", "DateField",
        "ImageItem", "Gauge", "ChoiceGroup",
        "List", "Alert", "Sounds"
    };
    
    // List of alert types
    private AlertType[] alertTypes = new AlertType[] {
        AlertType.ALARM, AlertType.CONFIRMATION, AlertType.ERROR,
        AlertType.INFO, AlertType.WARNING, null
    };
    
    private String[] alertTypeNames = new String[] {
        "ALARM", "CONFIRMATION", "ERROR", "INFO", "WARNING", "None"
    };

    protected void startApp() {
        if (!started) {
            started = true;
            display = Display.getDisplay(this);
            
            // Create the common commands
            createCommands();
            
            // Create the screens
            createScreens();
            
            // Create the list of examples
            createList();
            
            // Start with the List
            display.setCurrent(examplesList);
        }
    }

    protected void pauseApp() {
    }

    protected void destroyApp(boolean unconditional) {
    }

    public void commandAction(Command c, Displayable d) {
        if (d == examplesList) {
            // New example selected
            int index = examplesList.getSelectedIndex();
            display.setCurrent(screens[index]);
        } else if (c == exitCommand) {
            // Exit. No need to call destroyApp
            // because it is empty.
            notifyDestroyed();
        } else if (c == backCommand) {
            // Go back to main selection list
            display.setCurrent(examplesList);
        } else if (c == List.SELECT_COMMAND) {
            // Selection made in the IMPLICIT LIST
            handleChoiceSelection((Choice)d);
        }
    }
    
    public void itemStateChanged(Item item) {        
        if (item instanceof TextField) {
            System.out.println("Text field content: <" +
                            ((TextField)item).getString() + ">");
        } else if (item instanceof DateField) {
            DateField df = (DateField)item;
            Date date = df.getDate();
            if (date != null) {
                Calendar cal = Calendar.getInstance();
                cal.setTime(date);
                System.out.println("Date field content set to " + date);                
            } else {
                System.out.println("Date field content set to null");
            }
        } else if (item instanceof Gauge) {
            int value = ((Gauge)item).getValue();
            System.out.println("Gauge value set to " + value);
        } else if (item instanceof ChoiceGroup) {
            handleChoiceSelection((Choice)item);
        } else {
            System.out.println("Item state changed in " + item);
        }
    }    
    
    private void createCommands() {
        exitCommand = new Command("Exit", Command.EXIT, 0);
        backCommand = new Command("Back", Command.BACK, 1);
    }
    
    private void createList() {
        examplesList = new List("Select Example", List.IMPLICIT);
        for (int i = 0; i < examples.length; i++) {
            examplesList.append(examples[i], null);
        } 
        examplesList.setCommandListener(this);
    }
    
    private void createScreens() {
        screens = new Screen[examples.length];
        screens[0] = createStringsForm();
        screens[1] = createTextFieldForm();
        screens[2] = createDateFieldForm();
        screens[3] = createImageItemForm();
        screens[4] = createGaugeForm();
        screens[5] = createChoiceGroupForm();
        screens[6] = createListExample();
        screens[7] = createAlertForm();
        screens[8] = createSoundsForm();
    }

    private void addCommands(Displayable d) {
        d.addCommand(exitCommand);
        d.addCommand(backCommand);
        d.setCommandListener(this);
        if (d instanceof Form) {
            ((Form)d).setItemStateListener(this);
        }
    }
    
    // Example for StringItem
    private Form createStringsForm() {
        Form form = new Form("StringItem");
        
        form.append(new StringItem("State ", "OK"));
        form.append(new StringItem(null, "No label\n"));
        form.append(new StringItem(null, "Line\nbreak"));
        form.append(new StringItem("Label", "Text."));
        form.append(new StringItem("Label2 ", "Text2."));
        
        addCommands(form);
        return form;
    }
    
    // Example for TextField
    private Form createTextFieldForm() {
        Form form = new Form("TextField");
        
        form.append(new TextField("Any", null, 8, TextField.ANY));
        form.append(new TextField("Phone", "1234567890", 10, TextField.PHONENUMBER));
        form.append(new TextField("Number", "12345", 8, TextField.NUMERIC));
        form.append(new TextField("Password", null, 8, TextField.PASSWORD | TextField.NUMERIC));
        
        addCommands(form);
        return form;
    }
    
    // Example for DateField
    private Form createDateFieldForm() {
        Form form = new Form("DateField");
        
        // Get Calendar for the epoch date and time
        Calendar baseCal = Calendar.getInstance();
        Date baseDate = new Date(0);
        baseCal.setTime(baseDate);
        
        // Get Calendar for now and use the epoch
        // values to reset the date to the epoch.
        Calendar cal = Calendar.getInstance();
        Date now = new Date();       
        cal.setTime(now);
        cal.set(Calendar.YEAR, baseCal.get(Calendar.YEAR));
        cal.set(Calendar.MONTH, baseCal.get(Calendar.MONTH));
        cal.set(Calendar.DATE, baseCal.get(Calendar.DATE));
        
        DateField timeOnly = new DateField("Time", DateField.TIME);
        DateField dateOnly = new DateField("Date", DateField.DATE);
        DateField both = new DateField("Both", DateField.DATE_TIME);
        
        timeOnly.setDate(cal.getTime());
        dateOnly.setDate(now);
        both.setDate(now);
        
        form.append(timeOnly);
        form.append(dateOnly);
        form.append(both);
       
        addCommands(form);
        return form;
    }
    
    // Example for ImageItem
    private Form createImageItemForm() {
        Form form = new Form("ImageItem");
        
        try {
            Image red = Image.createImage("/ora/ch4/resources/red.png");
            Image blue = Image.createImage("/ora/ch4/resources/blue.png");

            // ImageItems with labels
            // (1)
            form.append(new ImageItem("Center", red, ImageItem.LAYOUT_CENTER, null));
            
            // (2)
            form.append(new ImageItem("Left", red, ImageItem.LAYOUT_LEFT, null));
            
            // (3)
            form.append(new ImageItem("Right", red, ImageItem.LAYOUT_RIGHT, null));
            
            // (4)
            form.append(new ImageItem("Default", red, ImageItem.LAYOUT_DEFAULT, null));
            
            // ImageItems with no labels
            // (5)
            form.append(new ImageItem(null, blue, 
                    ImageItem.LAYOUT_NEWLINE_BEFORE | 
                    ImageItem.LAYOUT_CENTER | 
                    ImageItem.LAYOUT_NEWLINE_AFTER, null));
            
            // (6)
            form.append(new ImageItem(null, blue, 
                    ImageItem.LAYOUT_NEWLINE_BEFORE | 
                    ImageItem.LAYOUT_DEFAULT | 
                    ImageItem.LAYOUT_NEWLINE_AFTER, null));
            
            // (7)
            form.append(new ImageItem(null, blue, 
                    ImageItem.LAYOUT_NEWLINE_BEFORE | 
                    ImageItem.LAYOUT_RIGHT | 
                    ImageItem.LAYOUT_NEWLINE_AFTER, null));
            
            // (8)
            form.append(new ImageItem(null, blue, ImageItem.LAYOUT_DEFAULT, null)); 
            form.append(new ImageItem(null, blue, ImageItem.LAYOUT_DEFAULT, null)); 
        } catch (IOException ex) {
            form.append("Failed to load images");
        }
        
        addCommands(form);
        return form;
    }
    
    // Example for Gauge
    private Form createGaugeForm() {
        Form form = new Form("Gauge");
        
        form.append(new Gauge(null, true, 100, 50));
        form.append(new Gauge(null, true, 100, 25));
        form.append(new Gauge(null, false, 100, 50));
        
        addCommands(form);
        return form;
    }
    
    // Example for ChoiceGroup
    private Form createChoiceGroupForm() {
        Form form = new Form("ChoiceGroup");
        
        try {
            Image red = Image.createImage("/ora/ch4/resources/red.png");
            Image green = Image.createImage("/ora/ch4/resources/green.png");
            Image blue = Image.createImage("/ora/ch4/resources/blue.png");

            // Exclusive choice group
            String[] strings = new String[] { "Red", "Green", "Blue" };
            Image[] images = new Image[] { red, green, blue };
            ChoiceGroup exGroup = new ChoiceGroup("Choose one", ChoiceGroup.EXCLUSIVE,
                                                        strings, images);
            form.append(exGroup);

            // Multiple choice group
            ChoiceGroup multiGroup = new ChoiceGroup("Choose any", ChoiceGroup.MULTIPLE);
            form.append(multiGroup);
            multiGroup.append("Use SSL", null);
            multiGroup.append("Reconnect on failure", null);
            multiGroup.append("Enable tracing", null);
        } catch (IOException ex) {
            form.append("Failed to load images");
        }
        
        addCommands(form);
        return form;
    }
    
    // Example for List
    private Screen createListExample() {
        List list = new List("List", List.IMPLICIT);

        try {
            Image red = Image.createImage("/ora/ch4/resources/red.png");
            Image green = Image.createImage("/ora/ch4/resources/green.png");
            Image blue = Image.createImage("/ora/ch4/resources/blue.png");
            
            list.append("Red", red);
            list.append("Green", green);
            list.append("Blue", blue);
        } catch (IOException ex) {
            Form form = new Form("Error");
            form.append("Failed to load images");
            return form;
        }
        
        addCommands(list);
        return list;
    }
    
    // Example for Alert
    private Screen createAlertForm() {
        final Form form = new Form("Alert");
        
        // A ChoiceGroup that allows the user to choose between
        // a modal Alert or an Alert with a finite timeout.
        final ChoiceGroup limitGroup = new ChoiceGroup("Timeout", Choice.EXCLUSIVE);
        limitGroup.append("Forever", null);
        limitGroup.append("Bounded 5s", null);
        form.append(limitGroup);
        
        // A Gauge that allows the user to choose the 
        // timeout for the Alert. This appears only if
        // the "Bounded" option is selected.
        final Gauge gauge = new Gauge(null, true, 30, 5);
        
        // A ChoiceGroup that lets the user pick the 
        // type of Alert
        final ChoiceGroup group = createAlertChoiceGroup(true); 
        form.append(group);  
        
        // A Command that is used to display the Alert
        final Command okCommand = new Command("OK", Command.OK, 0);
        
        // Add CommandListener for back/exit commands
        // and the OK command, which is specific to this Form
        form.addCommand(okCommand);
        form.addCommand(exitCommand);
        form.addCommand(backCommand);
        form.setCommandListener(new CommandListener() {
            public void commandAction(Command c, Displayable d) {
                if (c == okCommand) {
                    // Create and display the Alert
                    
                    // Get the alert type from the 
                    // second ChoiceGroup
                    int selectedType = group.getSelectedIndex();
                    AlertType alertType = alertTypes[selectedType];
                    String name = alertTypeNames[selectedType];
                    
                    // Get the timeout. This is FOREVER if the
                    // first item in the first ChoiceGroup is
                    // selected. Otherwise it is the value in
                    // the Gauge
                    int timeout = Alert.FOREVER;
                    String timeoutString = "none";
                    if (limitGroup.isSelected(1)) {
                        // "Bounded" selected - get the timeout.
                        timeout = gauge.getValue() * 1000;
                        timeoutString = gauge.getValue() + "s";                        
                    }
                    
                    // Create the Alert and set the timeout
                    Alert alert = new Alert("Alert!",
                        "Alert type: " + name + "\nTimeout = " + timeoutString,
                        null, alertType);
                    alert.setTimeout(timeout);
                    
                    // Display the Alert.
                    display.setCurrent(alert);                               
                } else {
                    // Delegate others to the usual commandAction
                    ItemMIDlet.this.commandAction(c, d);
                }                
            }
        });
        
        // Set our own ItemStateListener
        form.setItemStateListener(new ItemStateListener() {
            public void itemStateChanged(Item item) {
                if (item == limitGroup) {
                    if (limitGroup.getSelectedIndex() == 0) {
                        // Get rid of the Gauge
                        form.delete(1);
                    } else {
                        // Add the Gauge
                        form.insert(1, gauge);
                    }
                } else if (item == gauge) {               
                    int value = gauge.getValue();
                    limitGroup.set(1, "Bounded " + value + "s", null);
                }
            }
        });
        return form;
    }
    
        
    // Example for Sounds
    private Screen createSoundsForm() {
        final Form form = new Form("Sounds");
        
        // A ChoiceGroup that lets the user pick the 
        // type of sound
        final ChoiceGroup group = createAlertChoiceGroup(false); 
        form.append(group);  

        addCommands(form);
        
        // Set our own ItemStateListener
        form.setItemStateListener(new ItemStateListener() {
            public void itemStateChanged(Item item) {
                // Get the alert type from the ChoiceGroup
                int selectedType = group.getSelectedIndex();
                AlertType alertType = alertTypes[selectedType];
                boolean result = alertType.playSound(display);
                System.out.println("A sound was " + (result ? "" : "not ")
                                    + "played.");
                
            }
        });
        return form;
    }
    
    // Handles the selection for a Choice 
    private void handleChoiceSelection(Choice choice) {
        int count = choice.size();
        boolean[] states = new boolean[count];
        int selCount = choice.getSelectedFlags(states);
        if (selCount > 0) {
            System.out.println("Selected items:");
            for (int i = 0; i < count; i++) {
                if (states[i]) {
                    System.out.println("\t" + choice.getString(i));
                }
            }
        } else {
            System.out.println("No selected items.");
        }
        int selectedIndex = choice.getSelectedIndex();
        System.out.println("Selected index is " + selectedIndex);        
    }
    
    // Creates a ChoiceGroup containing a set of Alert types
    private ChoiceGroup createAlertChoiceGroup(boolean includeNone) {
        ChoiceGroup group = new ChoiceGroup("Alert Type", Choice.EXCLUSIVE);
        int count = includeNone ? alertTypeNames.length : alertTypeNames.length - 1;
        for (int i = 0; i < count; i++) {
            group.append(alertTypeNames[i], null);
        }
        return group;
    }
}
