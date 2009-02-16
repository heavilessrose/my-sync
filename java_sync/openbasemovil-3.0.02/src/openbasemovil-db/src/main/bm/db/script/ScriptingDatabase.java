package bm.db.script;

import bm.vm.*;
import bm.vm.lang.Conversor;
import bm.db.Database;
import bm.db.Table;

import java.util.Hashtable;

/*
 * File Information
 *
 * Created on       : 23-oct-2007 21:47:05
 * Created by       : narciso
 * Last modified by : $Author$
 * Last modified on : $Date$
 * Revision         : $Revision$
 */

/**
 * Scripting class to access databases from script.
 * 
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision$
 */
public class ScriptingDatabase extends NativeClass {
	/**
	 * Prepare a class loader to be able to access databases and related
	 * classes: Database, Table, RowSet, Row.
	 * 
	 * @param classLoader
	 *            the class loader
	 */
	public static void prepareClassLoader(final ScriptingClassLoader classLoader) {
		final VirtualMachine vm = classLoader.getVirtualMachine();
		classLoader.addNativeClass("Database", new ScriptingDatabase(vm));
		classLoader.addNativeClass("Table", new ScriptingTable(vm));
		classLoader.addNativeClass("RowSet", new ScriptingRowSet(vm));
		classLoader.addNativeClass("Row", new ScriptingRow(vm));
	}

	private static final int CONNECT = 0;
	private static final int GET_NAME = 1;
	private static final int GET_TABLE = 2;
	private static final int GET_VER_MAJOR = 3;
	private static final int GET_VER_MINOR = 4;
	private static final int CREATE = 5;
	private static final int START = 6;
	private static final int DROP = 7;
	private static final int CREATE_TABLE = 8;
	private static final int PACK = 9;
	private static final int REPAIR = 10;
	private static final int IS_DAMAGED = 11;
	private static final int OPEN = 12;
	private static final int CLOSE = 13;
	private static final int SHUTDOWN = 14;
	private static final int SANITY_CHECK = 15;

	static {
		addMethod("connect", CONNECT, 1);
		addMethod("getName", GET_NAME, 0);
		addMethod("getTable", GET_TABLE, 1);
		addMethod("getVersionMajor", GET_VER_MAJOR, 0);
		addMethod("getVersionMinor", GET_VER_MINOR, 0);
		addMethod("create", CREATE, 1);
		addMethod("start", START, 0);
		addMethod("drop", DROP, 0);
		addMethod("createTable", CREATE_TABLE, 1);
		addMethod("pack", PACK, 0);
		addMethod("repair", REPAIR, 0);
		addMethod("isDamaged", IS_DAMAGED, 0);
		addMethod("open", OPEN, 0);
		addMethod("close", CLOSE, 0);
		addMethod("shutdown", SHUTDOWN, 0);
		addMethod("sanityCheck", SANITY_CHECK, 0);
	}

	public ScriptingDatabase(final VirtualMachine vm) {
		super(vm);
		name = "Database";
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
			Database db = (Database) target.getAttachment("db");
			try {
				switch (id) {
				case CONNECT:
					if (db == null) {
						db = Database.connect(Conversor.toString(args[0]));
						target.setAttachment("db", db);
					}
					return null;

				case CREATE:
					if (db == null) {
						db = Database.create(Conversor.toString(args[0]));
						target.setAttachment("db", db);
					}
					return null;

				case GET_TABLE:
					if (db != null) {
						final String tableName = Conversor.toString(args[0]);
						final Table table = db.getTable(tableName);
						if (table != null) {
							Hashtable tables = (Hashtable) target
									.getAttachment("tables");
							if (tables == null) {
								tables = new Hashtable(10);
								target.setAttachment("tables", tables);
							}
							Instance t = (Instance) tables.get(tableName);
							if (t == null) {
								t = getVirtualMachine().newInstance("Table");
								t.setAttachment("table", table);
								t.setAttachment("parent", target);
								tables.put(tableName, t);
							}
							return t;
						} else {
							return null;
						}
					} else {
						return null;
					}

				case CREATE_TABLE:
					if (db != null) {
						final Table table = (Table) args[0]
								.getAttachment("table");
						db.createTable(table);
						Hashtable tables = (Hashtable) target
								.getAttachment("tables");
						if (tables == null) {
							tables = new Hashtable(10);
							target.setAttachment("tables", tables);
						}
						Instance t = (Instance) tables.get(table.getName());
						if (t == null) {
							t = getVirtualMachine().newInstance("Table");
							t.setAttachment("table", table);
							t.setAttachment("parent", target);
							tables.put(table.getName(), t);
						}
						return t;
					} else {
						return null;
					}

				case START:
					if (db != null) {
						db.start();
					} else {
						return null;
					}

				case PACK:
					if (db != null) {
						db.pack();
					} else {
						return null;
					}

				case DROP:
					if (db != null) {
						db.drop();
					} else {
						return null;
					}

				case REPAIR:
					if (db != null) {
						db.repair();
					} else {
						return null;
					}

				case IS_DAMAGED:
					return db != null ? Conversor.toInstance(
							getVirtualMachine(), new Boolean(db.isDamaged()))
							: null;

				case OPEN:
					if (db != null) {
						db.open();
					} else {
						return null;
					}

				case CLOSE:
					if (db != null) {
						db.close();
					} else {
						return null;
					}

				case SHUTDOWN:
					if (db != null) {
						db.shutdown();
					} else {
						return null;
					}

				case SANITY_CHECK:
					if (db != null) {
						db.sanityCheck();
					} else {
						return null;
					}

				case GET_NAME:
					return db != null ? Conversor.toInstance(
							getVirtualMachine(), db.getName()) : null;

				case GET_VER_MAJOR:
					return db != null ? Conversor.toInstance(
							getVirtualMachine(),
							new Short(db.getVersionMajor())) : null;

				case GET_VER_MINOR:
					return db != null ? Conversor.toInstance(
							getVirtualMachine(),
							new Short(db.getVersionMinor())) : null;

				default:
					return null;
				}
			} catch (Exception e) {
				throw new VirtualMachineException(0, e.getMessage());
			}
		} else {
			return null;
		}
	}
}
