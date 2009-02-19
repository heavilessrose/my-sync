package org.hanyudictionary.dao;

import java.util.List;

/**
 * Dictionary data access object interface
 * @author Ken
 */
public interface DictionaryDAO {
	public List findWord(String searchWord) throws Exception;
}
