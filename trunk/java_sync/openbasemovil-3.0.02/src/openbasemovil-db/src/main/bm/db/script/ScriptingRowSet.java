package bm.db.script;

import bm.vm.NativeClass;
import bm.vm.Instance;
import bm.vm.VirtualMachineException;
import bm.vm.VirtualMachine;
import bm.vm.lang.Conversor;
import bm.db.Row;
import bm.db.RowSet;
import bm.core.CoreConstants;

/*
 * File Information
 *
 * Created on       : 23-oct-2007 22:34:29
 * Created by       : narciso
 * Last modified by : $Author$
 * Last modified on : $Date$
 * Revision         : $Revision$
 */

/**
 * Scripting interface to the RowSet class.
 * 
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision$
 */
public class ScriptingRowSet extends NativeClass {
	/*
	 * public void applyFilter( final RowFilter filter ) public void
	 * clearFilter() public void packFilter() public boolean isFiltered()
	 * 
	 * public void sort( final String fieldName ) public void sort( final String
	 * fieldName, final int mode ) public void sort( final int fieldIndex )
	 * public void sort( final int fieldIndex, final int mode ) public void
	 * openSource() public void closeSource() public boolean hasNext() public
	 * boolean hasPrevious() public boolean next() public boolean previous()
	 * public int size() public int realSize() public Row getCurrent() public
	 * int getCurrentRecordId() public Row getRowAt( final int position ) public
	 * int getRecordIdAt( final int position ) public void beforeFirst() public
	 * void afterLast() public int getPosition() public void goTo( final int
	 * newIndex ) public void release()
	 */

	private static final int SORT = 0;
	private static final int SORT_EX = 1;
	private static final int OPEN_SOURCE = 2;
	private static final int CLOSE_SOURCE = 3;
	private static final int APPLY_FILTER = 4;
	private static final int CLEAR_FILTER = 5;
	private static final int HAS_NEXT = 12;
	private static final int HAS_PREVIOUS = 13;
	private static final int NEXT = 14;
	private static final int PREVIOUS = 15;
	private static final int SIZE = 16;
	private static final int REAL_SIZE = 17;
	private static final int GET_CURRENT = 18;
	private static final int GET_CURRENT_RID = 19;
	private static final int GET_ROW_AT = 20;
	private static final int GET_RECORD_ID_AT = 21;
	private static final int PACK_FILTER = 22;
	private static final int IS_FILTERED = 23;
	private static final int BEFORE_FIRST = 24;
	private static final int AFTER_LAST = 25;
	private static final int GET_POSITION = 26;
	private static final int GO_TO = 27;
	private static final int RELEASE = 28;

	static {
		addMethod("sort", SORT, 1);
		addMethod("sortEx", SORT_EX, 2);
		addMethod("openSource", OPEN_SOURCE, 0);
		addMethod("closeSource", CLOSE_SOURCE, 0);
		addMethod("hasNext", HAS_NEXT, 0);
		addMethod("hasPrevious", HAS_PREVIOUS, 0);
		addMethod("next", NEXT, 1);
		addMethod("previous", PREVIOUS, 0);
		addMethod("size", SIZE, 0);
		addMethod("realSize", REAL_SIZE, 0);
		addMethod("getCurrent", GET_CURRENT, 0);
		addMethod("getCurrentRecordId", GET_CURRENT_RID, 0);
		addMethod("getRowAt", GET_ROW_AT, 1);
		addMethod("getRecordIdAt", GET_RECORD_ID_AT, 1);
		addMethod("beforeFirst", BEFORE_FIRST, 0);
		addMethod("afterLast", AFTER_LAST, 0);
		addMethod("getPosition", GET_POSITION, 0);
		addMethod("goTo", GO_TO, 1);
		addMethod("release", RELEASE, 0);
	}

	/**
	 * Constructor.
	 * 
	 * @param vm
	 *            virtual machine
	 */
	public ScriptingRowSet(final VirtualMachine vm) {
		super(vm);
		name = "RowSet";
	}

	/**
	 * Invoke the method.
	 * 
	 * @param target
	 *            target instance on which to run
	 * @param args
	 *            method arguments
	 * @return return value
	 * @throws bm.vm.VirtualMachineException
	 *             on errors
	 */
	public Instance invoke(Instance target, Instance[] args)
			throws VirtualMachineException {
		final int id = checkMethod(methodName, args);
		if (id != -1) {
			final RowSet rowSet = (RowSet) target.getAttachment("rowset");
			if (rowSet != null) {
				try {
					switch (id) {
					case SORT: {
						final Object locator = ScriptingRow.getLocator(args[0]);
						if (locator instanceof String) {
							rowSet.sort((String) locator);
						} else {
							rowSet.sort(((Integer) locator).intValue());
						}
					}
						break;

					case SORT_EX: {
						final Object locator = ScriptingRow.getLocator(args[0]);
						if (locator instanceof String) {
							rowSet.sort((String) locator, Conversor.toInteger(
									args[1].get("value")).intValue());
						} else {
							rowSet.sort(((Integer) locator).intValue());
						}
					}
						break;

					case OPEN_SOURCE:
						rowSet.openSource();
						break;

					case CLOSE_SOURCE:
						rowSet.closeSource();
						break;

					case HAS_NEXT:
						return Conversor.toInstance(getVirtualMachine(), rowSet
								.hasNext() ? CoreConstants.TRUE
								: CoreConstants.FALSE);

					case HAS_PREVIOUS:
						return Conversor.toInstance(getVirtualMachine(), rowSet
								.hasPrevious() ? CoreConstants.TRUE
								: CoreConstants.FALSE);

					case NEXT:
						return Conversor.toInstance(getVirtualMachine(), rowSet
								.next() ? CoreConstants.TRUE
								: CoreConstants.FALSE);

					case PREVIOUS:
						return Conversor.toInstance(getVirtualMachine(), rowSet
								.previous() ? CoreConstants.TRUE
								: CoreConstants.FALSE);

					case SIZE:
						return Conversor.toInstance(getVirtualMachine(),
								new Integer(rowSet.size()));

					case REAL_SIZE:
						return Conversor.toInstance(getVirtualMachine(),
								new Integer(rowSet.realSize()));

					case GET_CURRENT:
						return sendRow(rowSet.getCurrent(), (Instance) target
								.getAttachment("table"));

					case GET_CURRENT_RID:
						return Conversor.toInstance(getVirtualMachine(),
								new Integer(rowSet.getCurrentRecordId()));

					case GET_ROW_AT:
						return sendRow(rowSet.getRowAt(Conversor.toInteger(
								args[0].get("value")).intValue()),
								(Instance) target.getAttachment("table"));

					case GET_RECORD_ID_AT:
						return Conversor.toInstance(getVirtualMachine(),
								new Integer(rowSet.getRecordIdAt(Conversor
										.toInteger(args[0].get("value"))
										.intValue())));

					case BEFORE_FIRST:
						rowSet.beforeFirst();
						break;

					case AFTER_LAST:
						rowSet.afterLast();
						break;

					case GET_POSITION:
						return Conversor.toInstance(getVirtualMachine(),
								new Integer(rowSet.getPosition()));

					case GO_TO:
						rowSet.goTo(Conversor.toInteger(args[0].get("value"))
								.intValue());
						break;

					case RELEASE:
						rowSet.release();
						break;
					}
				} catch (Exception e) {
					throw new VirtualMachineException(0, e.getMessage());
				}
			}
		}
		return null;
	}

	private Instance sendRow(final Row row, final Instance table)
			throws VirtualMachineException {
		final Instance r = getVirtualMachine().newInstance("Row");
		r.setAttachment("row", row);
		r.setAttachment("table", table);
		return r;
	}
}
