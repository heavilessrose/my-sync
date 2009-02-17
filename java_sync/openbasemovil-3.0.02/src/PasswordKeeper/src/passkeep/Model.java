package passkeep;

import bm.core.io.SerializationException;
import bm.core.tools.Blowfish;
import bm.db.*;
import bm.db.index.Index;
import bm.storage.RSException;
import bm.storage.RecordStoreFullException;

import java.util.Date;

/*
 * File Information
 *
 * Created on       : 27-jul-2005 12:09:44
 * Created by       : narciso
 * Last modified by : $Author: narciso $
 * Last modified on : $Date: 2005/11/09 23:41:22 $
 * Revision         : $Revision: 1.3 $
 */

/**
 * Business method repository.
 * 
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision: 1.3 $
 */
public final class Model {
	// Define the table name as constants
	public static final String TBL_CATEGORIES = "categories";
	public static final String TBL_PASSWORDS = "passwords";

	// Define the field names as constants
	// Field names

	// This is the name of the built-in primary key of every table
	public static final String FLD_REMOTE_ID = "REMOTE_ID";

	// Sample field definitions
	public static final String FLD_CAT_NAME = "name";
	public static final String FLD_CAT_DESCRIPTION = "description";
	public static final String FLD_CAT_PARENT_ID = "parent_id";

	public static final String FLD_PASS_TITLE = "title";
	public static final String FLD_PASS_PASSWORD = "password";
	public static final String FLD_PASS_CATEGORY_ID = "category_id";
	public static final String FLD_PASS_USER_NAME = "user_name";
	public static final String FLD_PASS_URL = "url";
	public static final String FLD_PASS_COMMENT = "comment";
	public static final String FLD_PASS_EXPIRES = "exprires";

	// Specific constants
	public static final String CONFIG_ACTIVE_LOCALE = "config.activeLocale";

	public static final String APPLICATION_NAME = "PasswordKeeper";
	public static final String VIRTUAL_DATABASE_NAME = "passkeep";

	// static fields
	private static String[] versionInfo;
	private static Database database;
	private static Blowfish cipher = new Blowfish();

	// private constants
	private Model() {
	}

	public static String[] getVersionInfo() {
		return versionInfo;
	}

	public static void setVersionInfo(final String[] versionInfo) {
		Model.versionInfo = versionInfo;
	}

	public static synchronized Database getDatabase() throws DBException,
			RecordStoreFullException, RSException, SerializationException {
		if (database == null) {
			database = Database.connect(VIRTUAL_DATABASE_NAME);
			if (database.getVersionMajor() == 0) {
				database.drop();
				database = Database.create(VIRTUAL_DATABASE_NAME);

				// Create the categories table
				final Table categories = new Table(TBL_CATEGORIES);
				categories.addColumn(FLD_CAT_NAME, Constants.FT_STRING, 32,
						false, null);
				categories.addColumn(FLD_CAT_DESCRIPTION, Constants.FT_STRING,
						128);
				categories.addColumn(FLD_CAT_PARENT_ID, Constants.FT_LONG, 0,
						true, "categories");
				categories.createIndex("ix_cat_parent", 50, FLD_CAT_PARENT_ID,
						Index.KT_LONG, false);
				database.createTable(categories);

				// Create the passwords table
				final Table passwords = new Table(TBL_PASSWORDS);
				passwords.addColumn(FLD_PASS_TITLE, Constants.FT_STRING, 32,
						false, null);
				passwords.addColumn(FLD_PASS_PASSWORD, Constants.FT_BLOB, 0,
						false, null);
				passwords.addColumn(FLD_PASS_CATEGORY_ID, Constants.FT_LONG, 0,
						false, TBL_CATEGORIES);
				passwords
						.addColumn(FLD_PASS_USER_NAME, Constants.FT_STRING, 32);
				passwords.addColumn(FLD_PASS_URL, Constants.FT_STRING, 256);
				passwords.addColumn(FLD_PASS_COMMENT, Constants.FT_STRING, 128);
				passwords.addColumn(FLD_PASS_EXPIRES, Constants.FT_DATE);
				passwords.createIndex("ix_pass_category", 30,
						FLD_PASS_CATEGORY_ID, Index.KT_LONG, false);
				passwords.createFullTextIndex("ix_pass_text", new String[] {
						FLD_PASS_TITLE, FLD_PASS_USER_NAME, FLD_PASS_COMMENT });
				database.createTable(passwords);

				database.setVersionMajor((short) 1);
			} else {
				database.start();
			}
		}
		return database;
	}

	public static RowSet getRootCategories() throws DBException,
			SerializationException, RecordStoreFullException {
		return database.getTable(TBL_CATEGORIES).find(FLD_CAT_PARENT_ID, null);
	}

	public static RowSet getChildCategories(final Long parentId)
			throws DBException, SerializationException,
			RecordStoreFullException {
		return database.getTable(TBL_CATEGORIES).find(FLD_CAT_PARENT_ID,
				parentId);
	}

	public static RowSet findEntriesByCategory(final long id)
			throws DBException, SerializationException,
			RecordStoreFullException {
		return database.getTable(TBL_PASSWORDS).find(FLD_PASS_CATEGORY_ID,
				new Long(id));
	}

	public static Row findCategoryById(final Long parentId) throws DBException,
			SerializationException, RecordStoreFullException, RSException {
		return lookUpEntity(database.getTable(TBL_CATEGORIES), parentId
				.longValue());
	}

	public static void removeCategoryTree(final Row category)
			throws DBException, SerializationException,
			RecordStoreFullException, RSException {
		final RowSet childCategories = getChildCategories(new Long(category
				.getId()));
		if (childCategories.size() > 0) {
			// A category with child categories can not have entries
			// use recursion to remove child categories
			while (childCategories.next()) {
				removeCategoryTree(childCategories.getCurrent());
			}
			// And finally remove the category itself
			category.remove();
		} else {
			final RowSet childEntries = findEntriesByCategory(category.getId());
			if (childEntries.size() > 0) {
				// Remove child entries
				while (childEntries.next()) {
					childEntries.getCurrent().remove();
				}
				// And finally remove the category itself
				category.remove();
			} else {
				// The category is empty, simply remove it
				category.remove();
			}
		}
	}

	public static Row lookUpEntity(final Table table, final long entityId)
			throws DBException, SerializationException,
			RecordStoreFullException, RSException {
		final RowSet rowSet = table.find(FLD_REMOTE_ID, new Long(entityId));
		if (rowSet != null && rowSet.size() > 0) {
			rowSet.next();
			return rowSet.getCurrent();
		} else {
			return null;
		}
	}

	public static void createCategory(final Long parentId, final String name,
			final String description) throws DBException,
			SerializationException, RecordStoreFullException, RSException {
		final Table categories = database.getTable(TBL_CATEGORIES);
		final Row row = categories.createRow();
		row.setField(FLD_CAT_NAME, name);
		row.setField(FLD_CAT_DESCRIPTION, description);
		row.setField(FLD_CAT_PARENT_ID, parentId);
		row.save();
	}

	public static RowSet findEntries(final String searchText)
			throws DBException, SerializationException,
			RecordStoreFullException {
		return database.getTable(TBL_PASSWORDS).findFuzzy(
				FLD_PASS_TITLE + "+" + FLD_PASS_USER_NAME + "+"
						+ FLD_PASS_COMMENT, searchText);
	}

	public static void createEntry(final String title, final String password,
			final long categoryId, final String userName, final String url,
			final String comment, final Date expires) throws DBException,
			SerializationException, RecordStoreFullException, RSException {
		final Table entries = database.getTable(TBL_PASSWORDS);
		final Row row = entries.createRow();
		row.setField(FLD_PASS_TITLE, title);
		cipher.init(true, AppState.getPassword().getBytes());
		row.setField(FLD_PASS_PASSWORD, cipher.encrypt(password.getBytes()));
		row.setField(FLD_PASS_CATEGORY_ID, new Long(categoryId));
		if (userName != null && userName.trim().length() > 0) {
			row.setField(FLD_PASS_USER_NAME, userName);
		}
		if (url != null && url.trim().length() > 0) {
			row.setField(FLD_PASS_URL, url);
		}
		if (comment != null && comment.trim().length() > 0) {
			row.setField(FLD_PASS_COMMENT, comment);
		}
		if (expires != null) {
			row.setField(FLD_PASS_EXPIRES, expires);
		}
		row.save();
	}

	public static String decryptPassword(final byte[] blob) {
		cipher.init(false, AppState.getPassword().getBytes());
		return cipher.decryptString(blob);
	}

	public static void saveEntry(final Row row, final String clearPassword)
			throws DBException, SerializationException,
			RecordStoreFullException, RSException {
		cipher.init(true, AppState.getPassword().getBytes());
		row.setField(FLD_PASS_PASSWORD, cipher
				.encrypt(clearPassword.getBytes()));
		row.save();
	}
}
