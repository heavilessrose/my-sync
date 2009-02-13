package bm.mvc;
/* -----------------------------------------------------------------------------
    OpenBaseMovil User Interface Library
    Copyright (C) 2004-2008 Elondra S.L.

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.
    If not, see <a href="http://www.gnu.org/licenses">http://www.gnu.org/licenses</a>.
----------------------------------------------------------------------------- */

import bm.core.Application;
import bm.core.ErrorHandler;
import bm.core.ResourceManager;
import bm.core.log.Log;
import bm.core.log.LogFactory;
import bm.core.math.FixedPoint;
import bm.core.mvc.*;
import bm.core.tools.LocalFormatTools;
import bm.db.DBException;
import bm.db.Row;
import bm.db.RowSet;
import bm.storage.RSException;
import bm.storage.RecordStoreFullException;
import bm.ui.def.view.form.*;

import javax.microedition.lcdui.*;
import java.io.IOException;
import java.util.Date;
import java.util.Enumeration;
import java.util.Hashtable;
import java.util.Vector;
/*
 * File Information
 *
 * Created on       : 10-mar-2007 14:52:31
 * Created by       : narciso
 * Last modified by : $Author$
 * Last modified on : $Date$
 * Revision         : $Revision$
 */

/**
 * Data bound forms built configuration files.<br/>
 * A configuration file can contain one or more form definitions, and multiple
 * files can be registered.<br/>
 * A configuration file is actually a properties file, with groups of properties
 * following a syntax:<br/>
 * A form is identified by it's name, and that name is also the first part
 * of all the properties associated to it.<br/>
 * A form can have a number of properties:<br/>
 * <dl>
 *  <dt>id</dt>
 *  <dd>The view id</dd>
 *  <dt>title</dt>
 *  <dd>The tile of the form, it can be a text or the name of a resource</dd>
 *  <dt>readOnly</dt>
 *  <dd>If the form is a readonly one, can be &quot;true&quot; or &quot;false&quot;</dd>
 *  <dt>itemCount</dt>
 *  <dd>The number of items in the form</dd>
 *  <dt>saveOnAccept</dt>
 *  <dd>If the data should be actually saved in the table or just the row
 * updated, by default true. Only has meaning when the view is not readOnly
 * and not browsable.</dd>
 * </dl>
 * Each item is defined using the form name, followed by the item index (from
 * 0 to itemCount - 1), and then the item properties (required in bold):
 * <dl>
 *  <dt><b>name</b></dt>
 *  <dd>item name</dd>
 *  <dt><b>type</b></dt>
 *  <dd>item type, one of: text, date, time, timestamp, gauge, spacer, image,
 * choice, money, percent, integer, fixedpoint</dd>
 *  <dt>label</dt>
 *  <dd>item label, can be empty, a text or a resource name.</dd>
 *  <dt>labelExtra</dt>
 *  <dd>item label extra text to append to label, can be empty or a text.</dd>
 *  <dt>layout</dt>
 *  <dd>item layout as an integer, by default Item.LAYOUT_2.</dd>
 *  <dt>bind</dt>
 *  <dd>name of the field to bind to, empty if not bound</dd>
 *  <dt>readOnly</dt>
 *  <dd>if the field is read only</dd>
 *  <dt>text</dt>
 *  <dd>a fixed text for not bound items</dd>
 *  <dt>size</dt>
 *  <dd>size for editable fields (text, money, percent, integer, fixedpoint)</dd>
 *  <dt>constraints</dt>
 *  <dd>constraints for editable fields (text, money, percent, integer, fixedpoint)</dd>
 *  <dt>appearance</dt>
 *  <dd>appearance mode for readOnly fields and images</dd>
 *  <dt>maxValue</dt>
 *  <dd>maximum value for gauges, by default 100</dd>
 *  <dt>altText</dt>
 *  <dd>optional alternative text for images</dd>
 *  <dt>minWidth</dt>
 *  <dd>minimum width for spacers, by default 1</dd>
 *  <dt>minHeight</dt>
 *  <dd>minimum height for spacers, by default 1</dd>
 *  <dt>wrap</dt>
 *  <dd>for choices, if item text should wrap</dd>
 *  <dt>choiceType</dt>
 *  <dd>for choices, the type: exclusive, multiple, popup</dd>
 *  <dt>decimals</dt>
 *  <dd>number of decimals for decimal types (money, percent, fixedpoint)</dd>
 * </dl>
 *
 * For example:<br/>
 * <pre>
 * customerDetail.title=customer.detail.Title
 * customerDetail.itemCount=10
 * customerDetail.readOnly=true
 * customerDetail.0.name=commercialName
 * customerDetail.0.label=customer.searchBox.CommercialName
 * customerDetail.0.labelExtra=:
 * customerDetail.0.layout=16896
 * customerDetail.0.bind=cust_name
 * customerDetail.0.readOnly=true
 * customerDetail.0.type=text
 * ...
 * </pre>
 *
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision$
 */
public class DataFormView
    extends AbstractView
        implements RowSetBrowser.Listener,
                   ItemStateListener,
                   DataboundView
{
    private static final Log log = LogFactory.getLog( "DataFormView" );

    public static final String[] NOKIA_FULL_KEYBOARD = new String[] { "e61", "e70" };

    // -------------------------------------------------------------------------
    // Constants
    // -------------------------------------------------------------------------


    // -------------------------------------------------------------------------
    // Static elements
    // -------------------------------------------------------------------------
    static final Hashtable forms = new Hashtable( 10 );

    // -------------------------------------------------------------------------
    // Instance elements
    // -------------------------------------------------------------------------

    private Row             row;
    private RowSetBrowser   browser;
    private String          name;
    private Hashtable       boundItems = new Hashtable( 10 );
    private Hashtable       items;
    private Vector          changedItems = new Vector( 10 );
    private boolean         browsable;
    private FormDefinition  formDef;
    private boolean         changed;
    private int             viewId;
    private FieldResolver   resolver;
    private FormDef         def;

    // -------------------------------------------------------------------------
    // Public methods
    // -------------------------------------------------------------------------

    /**
     * Constructor.
     *
     * @param controller view controller
     * @param name view name
     * @param browsable if it will be used with a RowSet
     * @param formDef form definition
     */
    public DataFormView(
            final Controller    controller,
            final String        name,
            final boolean       browsable,
            final FormDef       formDef
    )
    {
        this( controller, name, browsable, null, formDef );
    }

    /**
     * Constructor.
     *
     * @param controller view controller
     * @param name view name
     * @param browsable if it will be used with a RowSet
     * @param readOnly if the form should be created as readonly overriding definition
     * @param formDef form definition
     */
    public DataFormView(
            final Controller    controller,
            final String        name,
            final boolean       browsable,
            final boolean       readOnly,
            final FormDef       formDef
    )
    {
        this(
                controller,
                name,
                browsable,
                new Boolean( readOnly ),
                formDef
        );
    }

    /**
     * Constructor.
     *
     * @param controller view controller
     * @param name view name
     * @param browsable if it will be used with a RowSet
     * @param readOnly if the form should be created as readonly overriding definition
     * @param formDef form definition
     */
    public DataFormView(
            final Controller    controller,
            final String        name,
            final boolean       browsable,
            final Boolean       readOnly,
            final FormDef       formDef
    )
    {
        super( controller );
        this.name = name;
        this.browsable = browsable;
        this.def = formDef;
        try
        {
            build( readOnly );
        }
        catch( IOException e )
        {
            log.error( e );
            ErrorHandler.handleError( this, e );
        }
    }

    /**
     * Get the field resolver.
     *
     * @return field resolver
     */
    public FieldResolver getResolver()
    {
        return resolver;
    }

    /**
     * Set the field resolver.
     *
     * @param resolver resolver
     */
    public void setResolver( final FieldResolver resolver )
    {
        this.resolver = resolver;
    }

    Object resolve( final Row row, final String fieldName )
    {
        try
        {
            if( row.getParent().hasField( fieldName ) )
            {
                return row.getField( fieldName );
            }
            else if( resolver != null )
            {
                return resolver.resolve( this, fieldName, row );
            }
            else
            {
                return null;
            }
        }
        catch( Exception e )
        {
            log.error( e );
            ErrorHandler.handleError( this, e );
            return null;
        }
    }

    /**
     * Check if there're pending changes to be saved.
     *
     * @return true if there're changes
     */
    public boolean isChanged()
    {
        return changed;
    }

    /**
     * Default implementation, calls controller's onRelease and clears
     * attached objects.
     */
    public void release()
    {
        super.release();
        try
        {
            final RowSetBrowser browser = this.browser;
            if( browser != null && browser.getRowSet() != null )
            {
                browser.getRowSet().release();
                browser.setRowSet( null );
            }
        }
        catch( Exception e )
        {
            log.error( e );
        }
        System.gc();
        Thread.yield();
    }

    /**
     * Default implementation of commandAction, calls
     * AbstractController.commandAction methods choosing the right one depending
     * on the displayable type.
     *
     * @param command     command to execute
     * @param displayable displayable item
     */
    public void commandAction(
            final Command command,
            final Displayable displayable
    )
    {
        if( !handleAction( command ) )
        {
            if( !formDef.isReadOnly() )
            {
                final ControllerEvent event = AbstractController.buildEvent(
                        this,
                        command
                );
                if( event.getCode() == ControllerEvent.ACCEPT )
                {
                    save();
                    caller.show();
                    release();
                }
                else if( event.getCode() == ControllerEvent.CANCEL )
                {
                    cancel();
                    caller.show();
                    release();
                }
                else
                {
                    controller.handle( event );
                }
            }
            else
            {
                AbstractController.commandAction( this, command );
            }
        }
    }

    /**
     * Bind a RowSet.
     *
     * @param rowSet the rowset
     * @throws RSException on errors
     * @throws DBException on errors
     * @throws RecordStoreFullException if no space left
     */
    public void bind( final RowSet rowSet )
            throws RSException,
                   DBException,
                   RecordStoreFullException
    {
        if( browsable )
        {
            browser.setRowSet( rowSet );
        }
    }

    /**
     * Get the current bound row (either directly or through a RowSet).
     *
     * @return the row
     */
    public Row getRow()
    {
        return row;
    }

    /**
     * Bind a row.
     *
     * @param row the row
     */
    public void bind( final Row row )
    {
        if( !browsable )
        {
            rebind( row );
        }
    }

    /**
     * Set the currently focused item.
     *
     * @param name item name
     */
    public void setFocusAt( final String name )
    {
        setFocusAt( getItem( name ) );
    }

    /**
     * Set the currently focused item.
     *
     * @param index item index
     */
    public void setFocusAt( final int index )
    {
        setFocusAt( getItem( index ) );
    }

    private void setFocusAt( final Item item )
    {
        focusedItem = item;
        show();
    }

    /**
     * Get the value of a field.<br/>
     * The value returned depends on the object type:<br/>
     * <table>
     * <tr>
     *  <th>field type</th>
     *  <th>return type</th>
     * </tr>
     * <tr>
     *  <td>text</td>
     *  <td>String</td>
     * </tr>
     * <tr>
     *  <td>date, time, timestamp</td>
     *  <td>Date</td>
     * </tr>
     * <tr>
     *  <td>gauge, integer</td>
     *  <td>Integer</td>
     * </tr>
     * <tr>
     *  <td>choice</td>
     *  <td>not implemented</td>
     * </tr>
     * <tr>
     *  <td>money, percent, fixedpoint</td>
     *  <td>FixedPoint</td>
     * </tr>
     * </table>
     * @param name field name
     * @return Object or null if no such a field
     */
    public Object getValue( final String name )
    {
        final FieldDefinition field = formDef.get( name );
        if( field != null )
        {
            return field.getValue( (Item) items.get( name ) );
        }
        else
        {
            return null;
        }
    }

    /**
     * Get an item from the form.
     *
     * @param index item index as declared in the configuration file.
     * @return item
     */
    public Item getItem( final int index )
    {
        return ((Form) displayable).get( browsable ? index + 1 : index );
    }

    /**
     * Get an item by it's name, as declared in the configuration file.
     *
     * @param name item name
     * @return item
     */
    public Item getItem( final String name )
    {
        return (Item) items.get( name );
    }

    /**
     * Get a TextField item from the from.
     *
     * @param index item index as declared in the configuration file.
     * @return item
     */
    public TextField getTextField( final int index )
    {
        return (TextField) ((Form) displayable).get( browsable ? index + 1 : index );
    }

    /**
     * Get a TextField item from the form.
     *
     * @param name item name
     * @return item
     */
    public TextField getTextField( final String name )
    {
        return (TextField) items.get( name );
    }

    /**
     * Get the contents of a fixedpoint, money or percent field as a FixedPoint.
     * @param name field name
     * @return null if the field does not exists, a FixedPoint otherwise
     */
    public FixedPoint getFixedPointField( final String name )
    {
        final TextField field = (TextField) items.get( name );
        if( field != null )
        {
            return FixedPoint.parse( field.getString().replace( ',', '.' ), '.' );
        }
        else
        {
            return null;
        }
    }

    /**
     * Get a StringItem item from the from.
     *
     * @param index item index as declared in the configuration file.
     * @return item
     */
    public StringItem getStringItem( final int index )
    {
        return (StringItem) ((Form) displayable).get( browsable ? index + 1 : index );
    }

    /**
     * Get a StringItem item from the form.
     *
     * @param name item name
     * @return item
     */
    public StringItem getStringItem( final String name )
    {
        return (StringItem) items.get( name );
    }

    /**
     * Get a ImageItem item from the from.
     *
     * @param index item index as declared in the configuration file.
     * @return item
     */
    public ImageItem getImageItem( final int index )
    {
        return (ImageItem) ((Form) displayable).get( browsable ? index + 1 : index );
    }

    /**
     * Get a ImageItem item from the form.
     *
     * @param name item name
     * @return item
     */
    public ImageItem getImageItem( final String name )
    {
        return (ImageItem) items.get( name );
    }

    /**
     * Get a Gauge item from the from.
     *
     * @param index item index as declared in the configuration file.
     * @return item
     */
    public Gauge getGauge( final int index )
    {
        return (Gauge) ((Form) displayable).get( browsable ? index + 1 : index );
    }

    /**
     * Get a Gauge item from the form.
     *
     * @param name item name
     * @return item
     */
    public Gauge getGauge( final String name )
    {
        return (Gauge) items.get( name );
    }

    /**
     * Get a DateField item from the from.
     *
     * @param index item index as declared in the configuration file.
     * @return item
     */
    public DateField getDateField( final int index )
    {
        return (DateField) ((Form) displayable).get( browsable ? index + 1 : index );
    }

    /**
     * Get a DateField item from the form.
     *
     * @param name item name
     * @return item
     */
    public DateField getDateField( final String name )
    {
        return (DateField) items.get( name );
    }

    /**
     * Get a ChoiceGroup item from the from.
     *
     * @param index item index as declared in the configuration file.
     * @return item
     */
    public ChoiceGroup getChoiceGroup( final int index )
    {
        return (ChoiceGroup) ((Form) displayable).get( browsable ? index + 1 : index );
    }

    /**
     * Get a ChoiceGroup item from the form.
     *
     * @param name item name
     * @return item
     */
    public ChoiceGroup getChoiceGroup( final String name )
    {
        return (ChoiceGroup) items.get( name );
    }

    /**
     * Listener for item state changes.
     * @param item the item
     */
    public void itemStateChanged( Item item )
    {
        if( boundItems.containsKey( item ) )
        {
            changed = true;
            if( !changedItems.contains( item ) )
            {
                changedItems.addElement( item );
            }
        }
    }

    /**
     * Called when the user request a movement, prior to making it.
     *
     * @return true if the movement should be allowed, false otherwise.
     */
    public boolean beforeMove()
    {
        return formDef.isReadOnly() || !changed;
    }

    /**
     * Listener for the RowSetBrowser.
     */
    public void positionChanged()
    {
        try
        {
            final Row row = browser.getRowSet().getCurrent();

            if( row != null )
            {
                rebind( row );
                final Display display = Application.getManager().getDisplay();
                display.setCurrent( displayable );
                display.setCurrentItem( browser );
            }
        }
        catch( Exception e )
        {
            log.error( e );
        }
    }

    /**
     * Each view must have an id that identifies it.
     *
     * @return view id
     */
    public int getId()
    {
        return viewId;
    }

    public void setId( final int viewId )
    {
        this.viewId = viewId;
    }

    // -------------------------------------------------------------------------
    // Private methods
    // -------------------------------------------------------------------------

    private void build( final Boolean readOnlyOverride )
            throws IOException
    {
        final String name = this.name;
        FormDefinition formDef = (FormDefinition) forms.get( name );
        if( formDef == null )
        {
            formDef = new FormDefinition( name, readOnlyOverride, def );
            forms.put( name, formDef );
        }
        this.formDef = formDef;
        viewId = formDef.getId();

        final Form form = new Form( formDef.getTitle() );

        if( browsable )
        {
            final RowSetBrowser browser = new RowSetBrowser( null );
            browser.setUsedForSelection( formDef.isReadOnly() );
            browser.setItemCommandListener( this );
            browser.setListener( this );
            form.append( browser );
            this.browser = browser;
        }

        items = new Hashtable( formDef.size() );
        //noinspection MethodCallInLoopCondition
        for( final Enumeration i = formDef.fields(); i.hasMoreElements(); )
        {
            final FieldDefinition field = (FieldDefinition) i.nextElement();
            final Item item = field.create();
            form.append( item );
            item.setItemCommandListener( this );
            if( field.getBind() != null )
            {
                boundItems.put( item, field );
            }
            items.put( field.getName(), item );
        }
        if( formDef.isDefaultButtons() )
        {
            if( formDef.isReadOnly() )
            {
                form.addCommand( Application.getCommandFactory().backCommand() );
            }
            else
            {
                form.setItemStateListener( this );
                form.addCommand( Application.getCommandFactory().okCommand() );
                form.addCommand( Application.getCommandFactory().cancelCommand() );
            }
        }
        form.setCommandListener( this );
        displayable = form;
        focusedItem = form.get( 0 );
    }


    public void cancel()
    {
        try
        {
            changed = false;
            changedItems.removeAllElements();
            final Row row = browsable ?
                            browser.getRowSet().getCurrent() :
                            this.row;
            rebind( row );
        }
        catch( Exception e )
        {
            log.error( e );
            ErrorHandler.handleError( this, e );
        }
    }

    public void save()
    {
        try
        {
            final Row row = browsable ?
                            browser.getRowSet().getCurrent() :
                            this.row;
            //noinspection MethodCallInLoopCondition
            for( final Enumeration i = changedItems.elements(); i.hasMoreElements(); )
            {
                final Item item = (Item) i.nextElement();
                final FieldDefinition field = (FieldDefinition) boundItems.get( item );
                if( field != null && row.getParent().hasField( field.getBind() ) )
                {
                    row.setField( field.getBind(), field.getValue( item ) );
                }
            }
            if( browsable || formDef.isSaveOnAccept() )
            {
                row.getParent().save( row );
            }
            changed = false;
            changedItems.removeAllElements();
        }
        catch( Exception e )
        {
            log.error( e );
            ErrorHandler.handleError( this, e );
        }
    }

    private void rebind( final Row row )
    {
        this.row = row;
        if( row != null )
        {
            //noinspection MethodCallInLoopCondition
            for( final Enumeration i = boundItems.keys(); i.hasMoreElements(); )
            {
                final Item item = (Item) i.nextElement();
                final FieldDefinition field = (FieldDefinition)
                        boundItems.get( item );
                field.bind( row, item );
            }
        }
    }

    private class FormDefinition
    {
        private int         id;
        private String      name;
        private String      title;
        private boolean     readOnly;
        private boolean     saveOnAccept;
        private boolean     defaultButtons;
        private Hashtable   fieldsByName;
        private Vector      fields;

        /** @noinspection FieldRepeatedlyAccessedInMethod,JavaDoc*/
        public FormDefinition(
                final String    name,
                final Boolean   readOnlyOverride,
                final FormDef   def
        )
        {
            this.name = name;
            this.id = def.getId();
            title = ResourceManager.getResource( def.getTitle() );
            final Vector items = def.getItems();
            final int itemCount = items.size();
            if( readOnlyOverride == null )
            {
                readOnly = def.isReadOnly();
            }
            else
            {
                readOnly = readOnlyOverride.booleanValue();
            }
            saveOnAccept =
                    def.getSaveOnAccept() == null ||
                    def.getSaveOnAccept().booleanValue();
            defaultButtons =
                    def.getDefaultButtons() == null ||
                    def.getDefaultButtons().booleanValue();
            fields = new Vector( itemCount );
            fieldsByName = new Hashtable( itemCount );
            for( int i = 0; i < itemCount; i++ )
            {
                final ItemDef fd = (ItemDef) items.elementAt( i );
                final FieldDefinition field = new FieldDefinition( fd, readOnly );
                fields.addElement( field );
                fieldsByName.put( field.getName(), field );
            }
        }

        public int getId()
        {
            return id;
        }

        public boolean isSaveOnAccept()
        {
            return saveOnAccept;
        }

        public String getName()
        {
            return name;
        }

        public String getTitle()
        {
            return title;
        }

        public boolean isReadOnly()
        {
            return readOnly;
        }

        public boolean isDefaultButtons()
        {
            return defaultButtons;
        }

        public int size()
        {
            return fields.size();
        }

        public Enumeration fields()
        {
            return fields.elements();
        }

        public FieldDefinition get( final int index )
        {
            return (FieldDefinition) fields.elementAt( index );
        }

        public FieldDefinition get( final String name )
        {
            return (FieldDefinition) fieldsByName.get( name );
        }
    }

    private class FieldDefinition
    {
        private ItemDef def;

        private String  bind;
        private boolean readOnly;
        private int     size;

        /** @noinspection FieldRepeatedlyAccessedInMethod,JavaDoc*/
        public FieldDefinition( final ItemDef def, final boolean formReadOnly )
        {
            this.def = def;

            size = 0;
            readOnly = true;
            switch( def.getItemType() )
            {
                case ItemDef.TEXT:
                case ItemDef.INTEGER:
                    bind = ((TextDef) def).getBind();
                    readOnly = formReadOnly || ((TextDef) def).isReadOnly();
                    size = ((TextDef) def).getSize();
                    break;

                case ItemDef.DATE:
                case ItemDef.TIME:
                case ItemDef.TIMESTAMP:
                    bind = ((DateDef) def).getBind();
                    readOnly = formReadOnly || ((DateDef) def).isReadOnly();
                    break;

                case ItemDef.GAUGE:
                    bind = ((GaugeDef) def).getBind();
                    readOnly = formReadOnly || ((GaugeDef) def).isReadOnly();
                    break;

                case ItemDef.IMAGE:
                    bind = ((ImageDef) def).getBind();
                    break;

                case ItemDef.CHOICE:
                    bind = ((ChoiceDef) def).getBind();
                    break;

                case ItemDef.FIXED_POINT:
                case ItemDef.MONEY:
                case ItemDef.PERCENT:
                    bind = ((FixedPointDef) def).getBind();
                    readOnly = formReadOnly || ((FixedPointDef) def).isReadOnly();
                    size = ((FixedPointDef) def).getSize();
                    break;
            }
        }

        public String getName()
        {
            return def.getName();
        }

        public String getBind()
        {
            return bind;
        }

        public boolean isReadOnly()
        {
            return readOnly;
        }

        public byte getType()
        {
            return (byte) def.getItemType();
        }

        public int getSize()
        {
            return size;
        }

        public int getMaxValue()
        {
            return def.getItemType() == ItemDef.GAUGE ?
                   ((GaugeDef) def).getMaxValue() :
                   0;
        }

        public int getChoiceType()
        {
            return def.getItemType() == ItemDef.CHOICE ?
                   ((ChoiceDef) def).getChoiceType() :
                    0;
        }

        public Item create() throws IOException
        {
            switch( def.getItemType() )
            {
                case ItemDef.TEXT:
                    return createText();

                case ItemDef.DATE:
                    return createDate( DateField.DATE );

                case ItemDef.TIME:
                    return createDate( DateField.TIME );

                case ItemDef.TIMESTAMP:
                    return createDate( DateField.DATE_TIME );

                case ItemDef.GAUGE:
                    return createGauge();

                case ItemDef.IMAGE:
                    return createImage();

                case ItemDef.SPACER:
                    return createSpacer();

                case ItemDef.CHOICE:
                    return createChoice();

                case ItemDef.MONEY:
                case ItemDef.PERCENT:
                case ItemDef.FIXED_POINT:
                    return createItem( TextField.DECIMAL );
//                    if( !DeviceInfo.checkPlatform( NOKIA_FULL_KEYBOARD ) )
//                    {
//                        return createItem( TextField.DECIMAL );
//                    }
//                    else
//                    {
//                        return createItem( TextField.ANY );
//                    }

                case ItemDef.INTEGER:
                    return createItem( TextField.NUMERIC );

                case ItemDef.BUTTON:
                    return createButton();

                default:
                    return null;
            }
        }

        public boolean equals( final Object o )
        {
            if( this == o )
            {
                return true;
            }
            if( o == null || getClass() != o.getClass() )
            {
                return false;
            }

            final FieldDefinition that = (FieldDefinition) o;

            //noinspection RedundantIfStatement
            if( !def.getName().equals( that.getName() ) )
            {
                return false;
            }

            return true;
        }

        public int hashCode()
        {
            return def.getName().hashCode();
        }

        private Item createButton()
                throws IOException
        {
            Item item;
            final ButtonDef buttonDef = (ButtonDef) def;

            if( buttonDef.getImage() != null )
            {
                final Image img = Image.createImage(
                        getClass().getResourceAsStream( buttonDef.getImage() )
                );
                item = new ImageItem( null, img, Item.BUTTON, null );
            }
            else
            {
                item = new StringItem(
                        null,
                        ResourceManager.getResource( buttonDef.getLabel() ),
                        Item.BUTTON
                );
            }
            Command cmd = null;
            switch( buttonDef.getButtonType() )
            {
                case ButtonDef.BUTTON_TYPE_BACK:
                    cmd = Application.getCommandFactory().backCommand();
                    break;

                case ButtonDef.BUTTON_TYPE_CANCEL:
                    cmd = Application.getCommandFactory().cancelCommand();
                    break;

                case ButtonDef.BUTTON_TYPE_EXIT:
                    cmd = Application.getCommandFactory().exitCommand();
                    break;

                case ButtonDef.BUTTON_TYPE_HELP:
                    cmd = Application.getCommandFactory().helpCommand();
                    break;

                case ButtonDef.BUTTON_TYPE_HOME:
                    cmd = Application.getCommandFactory().homeCommand();
                    break;

                case ButtonDef.BUTTON_TYPE_OK:
                    cmd = Application.getCommandFactory().okCommand();
                    break;

                case ButtonDef.BUTTON_TYPE_STOP:
                    cmd = Application.getCommandFactory().stopCommand();
                    break;

                case ButtonDef.BUTTON_TYPE_CUSTOM:
                    cmd = Application.getCommandFactory().getCommand(
                            buttonDef.getCode()
                    );
                    break;
            }
            if( cmd == null )
            {
                cmd = new Command(
                        ResourceManager.getResource( buttonDef.getLabel() ),
                        Command.ITEM,
                        1
                );
            }
            addAction(
                    new Action( getName(), cmd, buttonDef.getCode(), item ),
                    true
            );
            item.setDefaultCommand( cmd );
            return item;
        }

        private Item createItem( final int constraints )
        {
            final TextDef textDef = (TextDef) def;
            Item item;
            final String label =
                    ResourceManager.getResource( textDef.getLabel() ) +
                    (
                            textDef.getLabelExtra() != null ?
                                textDef.getLabelExtra() :
                                ""
                    );
            if( readOnly )
            {
                item = new StringItem(
                        label,
                        textDef.getText(),
                        textDef.getAppearance()
                );
            }
            else
            {
                item = new TextField(
                        label,
                        textDef.getText(),
                        size,
                        constraints
                );
            }
            item.setLayout( textDef.getLayout() );
            return item;
        }

        private Item createChoice()
        {
            final ChoiceDef choiceDef = (ChoiceDef) def;
            final String label =
                    ResourceManager.getResource( choiceDef.getLabel() ) +
                    (
                            choiceDef.getLabelExtra() != null ?
                                choiceDef.getLabelExtra() :
                                ""
                    );

            final ChoiceGroup item = new ChoiceGroup(
                    label,
                    choiceDef.getChoiceType()
            );
            if( choiceDef.isWrap() )
            {
                item.setFitPolicy( Choice.TEXT_WRAP_ON );
            }
            item.setLayout( def.getLayout() );

            final Vector options = choiceDef.getOptions();
            final int count = options.size();
            for( int i = 0; i < count; i++ )
            {
                final Option option = (Option) options.elementAt( i );
                //noinspection ConstantConditions
                item.append(
                        ResourceManager.getResource( option.getLabel() ),
                        null
                );
            }
            return item;
        }

        private Item createSpacer()
        {
            final SpacerDef spacerDef = (SpacerDef) def;
            final Spacer item = new Spacer(
                    spacerDef.getMinWidth(),
                    spacerDef.getMinHeight()
            );
            item.setLayout( def.getLayout() );
            return item;
        }

        private Item createImage()
        {
            final ImageDef imageDef = (ImageDef) def;
            final String label =
                    ResourceManager.getResource( imageDef.getLabel() ) +
                    (
                            imageDef.getLabelExtra() != null ?
                                imageDef.getLabelExtra() :
                                ""
                    );
            return new ImageItem(
                    label,
                    null,
                    def.getLayout(),
                    ResourceManager.getResource( imageDef.getAltText() ),
                    imageDef.getAppearance()
            );
        }

        private Item createGauge()
        {
            final GaugeDef gaugeDef = (GaugeDef) def;
            final String label =
                    ResourceManager.getResource( gaugeDef.getLabel() ) +
                    (
                            gaugeDef.getLabelExtra() != null ?
                                gaugeDef.getLabelExtra() :
                                ""
                    );
            final Gauge item = new Gauge(
                    label,
                    !readOnly,
                    gaugeDef.getMaxValue(),
                    0
            );
            item.setLayout( def.getLayout() );
            return item;
        }

        private Item createDate( final int type )
        {
            final DateDef dateDef = (DateDef) def;
            final String label =
                    ResourceManager.getResource( dateDef.getLabel() ) +
                    (
                            dateDef.getLabelExtra() != null ?
                                dateDef.getLabelExtra() :
                                ""
                    );
            final Item item;
            if( !readOnly )
            {
                item = new DateField( label, type );
            }
            else
            {
                item = new StringItem( label, "" );
            }
            item.setLayout( def.getLayout() );
            return item;
        }

        private Item createText()
        {
            return createItem( ((TextDef) def).getConstraints() );
        }

        public void bind( final Row row, final Item item )
        {
            switch( def.getItemType() )
            {
                case ItemDef.TEXT:
                    bindText( row, item );
                    break;

                case ItemDef.DATE:
                case ItemDef.TIME:
                case ItemDef.TIMESTAMP:
                    bindDate( row, item );
                    break;

                case ItemDef.GAUGE:
                    bindGauge( row, item );
                    break;

                case ItemDef.IMAGE:
                    bindImage( row, item );
                    break;

                case ItemDef.CHOICE:
                    bindChoice( row, item );
                    break;

                case ItemDef.MONEY:
                    bindFixedPoint( row, item, true );
                    break;

                case ItemDef.PERCENT:
                    bindPercent( row, item );
                    break;

                case ItemDef.INTEGER:
                    bindInteger( row, item );
                    break;

                case ItemDef.FIXED_POINT:
                    bindFixedPoint( row, item, false );
                    break;
            }
        }

        private void bindInteger( final Row row, final Item item )
        {
            final Object raw = resolve( row, bind );
            String value = raw != null ? raw.toString() : "";
            if( readOnly )
            {
                ((StringItem) item).setText( value );
            }
            else
            {
                ((TextField) item).setString( value );
            }
        }

        private void bindPercent( final Row row, final Item item )
        {
            final Object raw = resolve( row, bind );
            String value;
            if( raw != null )
            {
                if( raw instanceof FixedPoint )
                {
                    value = ((FixedPoint) raw).toString(
                                ((FixedPointDef) def).getDecimals()
                    ) + (readOnly ? "%" : "");
                }
                else
                {
                    value = "";
                }
            }
            else
            {
                value = "";
            }
            if( readOnly )
            {
                ((StringItem) item).setText( value );
            }
            else
            {
                ((TextField) item).setString( value );
            }
        }

        private void bindFixedPoint( final Row row, final Item item, final boolean money )
        {
            final Object raw = resolve( row, bind );
            final boolean readOnly = this.readOnly;
            String value;
            if( raw != null )
            {
                if( raw instanceof FixedPoint )
                {
                    if( money )
                    {
                        final String symbol = FixedPoint.getMoneySymbol();
                        final boolean symbolAfter = FixedPoint.isMoneySymbolAfter();
                        value = ((FixedPoint) raw).toMoneyString(
                                ((FixedPointDef) def).getDecimals(),
                                readOnly ? FixedPoint.getDecimalSeparator() : '.',
                                readOnly ? FixedPoint.getGroupingSeparator() : null,
                                readOnly ? symbol : "",
                                symbolAfter
                        );
                    }
                    else
                    {
                        value = ((FixedPoint) raw).toString(
                                ((FixedPointDef) def).getDecimals(),
                                readOnly ? FixedPoint.getDecimalSeparator() : '.',
                                readOnly ? FixedPoint.getGroupingSeparator() : null
                        );
                    }
                }
                else
                {
                    value = "";
                }
            }
            else
            {
                value = "";
            }
            if( readOnly )
            {
                ((StringItem) item).setText( value );
            }
            else
            {
                ((TextField) item).setString( value );
            }
        }

        private void bindChoice( final Row row, final Item item )
        {
            // ToDo: implement
        }

        private void bindImage( final Row row, final Item item )
        {
            final Object raw = resolve( row, bind );
            if( raw != null )
            {
                if( raw instanceof byte[] )
                {
                    ((ImageItem) item).setImage( Image.createImage(
                            (byte[]) raw,
                            0,
                            ((byte[]) raw).length
                    ) );
                }
                else
                {
                    ((ImageItem) item).setImage( null );
                }
            }
            else
            {
                ((ImageItem) item).setImage( null );
            }
        }

        private void bindGauge( final Row row, final Item item )
        {
            final Object raw = resolve( row, bind );
            if( raw != null )
            {
                int i = 0;
                if( raw instanceof Byte )
                {
                    i = ((Byte) raw).byteValue();
                }
                else if( raw instanceof Short )
                {
                    i = ((Short) raw).shortValue();
                }
                else if( raw instanceof Integer )
                {
                    i = ((Integer) raw).intValue();
                }
                else if( raw instanceof Long )
                {
                    i = (int) ((Long) raw).longValue();
                }
                ((Gauge) item).setValue( i );
            }
            else
            {
                ((Gauge) item).setValue( 0 );
            }
        }

        private void bindDate( final Row row, final Item item )
        {
            final Object raw = resolve( row, bind );
            if( raw != null )
            {
                if( raw instanceof Date )
                {
                    final Date date =(Date) raw;
                    if( readOnly )
                    {
                        String value;
                        switch( def.getItemType() )
                        {
                            case ItemDef.DATE:
                                value = LocalFormatTools.formatDate( date );
                                break;

                            case ItemDef.TIME:
                                value = LocalFormatTools.formatTime( date );
                                break;

                            default:
                                value = LocalFormatTools.formatTimestamp( date );
                        }
                        ((StringItem) item).setText( value );
                    }
                    else
                    {
                        ((DateField) item).setDate( date );
                    }
                }
                else
                {
                    ((DateField) item).setDate( null );
                }
            }
            else
            {
                if( !readOnly )
                {
                    ((DateField) item).setDate( null );
                }
                else
                {
                    ((StringItem) item).setText( "" );
                }
            }
        }

        private void bindText( final Row row, final Item item )
        {
            final Object raw = resolve( row, bind );
            String value;
            if( raw != null )
            {
                value = raw.toString();
                if( value == null )
                {
                    value = "";
                }
            }
            else
            {
                value = "";
            }
            if( readOnly )
            {
                ((StringItem) item).setText( value );
            }
            else
            {
                ((TextField) item).setString( value );
            }
        }

        public Object getValue( final Item item )
        {
            switch( def.getItemType() )
            {
                case ItemDef.TEXT:
                    return ((TextField) item).getString();

                case ItemDef.DATE:
                case ItemDef.TIME:
                case ItemDef.TIMESTAMP:
                    return ((DateField) item).getDate();

                case ItemDef.GAUGE:
                    return new Integer( ((Gauge) item).getValue() );

                case ItemDef.CHOICE:
                    return null; // ToDo: implement

                case ItemDef.MONEY:
                    return FixedPoint.parse(
                            ((TextField) item).getString().replace( ',', '.' ),
                            '.'
                    );

                case ItemDef.PERCENT:
                    return FixedPoint.parse(
                            ((TextField) item).getString().replace( ',', '.' ),
                            '.'
                    );

                case ItemDef.FIXED_POINT:
                    return FixedPoint.parse(
                            ((TextField) item).getString().replace( ',', '.' ),
                            '.'
                    );

                case ItemDef.INTEGER:
                    return new Integer( Integer.parseInt(
                            ((TextField) item).getString()
                    ) );

                default:
                    return null;
            }
        }
    }
}
