package org.hanyudictionary.dao;
/**
 * Factory class for DAO (Translator choose dao from this)
 * @author Ken
 */
public abstract class DAOFactory {
	
	/* For search from file DAO*/
	public static DictionaryDAO getCEDefDAOUseFile(){
		return new org.hanyudictionary.dao.file.CEDefDAO();
	}
	
	public static DictionaryDAO getECDefDAOUseFile(){
		return new org.hanyudictionary.dao.file.ECDefDAO();
	}
	
	public static DictionaryDAO getPYDAOUseFile(){
		return new org.hanyudictionary.dao.file.PYDefDAO();
	}
}
