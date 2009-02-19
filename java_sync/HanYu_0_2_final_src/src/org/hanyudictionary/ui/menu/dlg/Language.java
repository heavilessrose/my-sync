package org.hanyudictionary.ui.menu.dlg;

import java.io.Serializable;

public class Language implements Serializable {
	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	private String name;
	private String locale;
	public String getLocale() {
		return locale;
	}
	public void setLocale(String locale) {
		this.locale = locale;
	}
	public String getName() {
		return name;
	}
	public void setName(String name) {
		this.name = name;
	}
	
	public String toString(){
		return name;
	}
	
	public boolean equals(Object o){
		if (o instanceof Language){
			Language lang = (Language) o;
			if (this.name.equals(lang.name) && this.locale.equals(lang.locale))
				return locale.equals(((Language)o).locale);
			else 
				return false;
		} 
		return false;
	}
}
