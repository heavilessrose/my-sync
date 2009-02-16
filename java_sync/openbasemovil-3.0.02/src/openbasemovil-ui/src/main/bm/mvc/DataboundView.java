package bm.mvc;

/*
 * File Information
 *
 * Created on       : 21-mar-2007 0:40:56
 * Created by       : narciso
 * Last modified by : $Author$
 * Last modified on : $Date$
 * Revision         : $Revision$
 */

/**
 * Databound view common methods.
 * 
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision$
 */
public interface DataboundView {
	/**
	 * Get the field resolver.
	 * 
	 * @return field resolver
	 */
	FieldResolver getResolver();

	/**
	 * Set the field resolver.
	 * 
	 * @param resolver
	 *            resolver
	 */
	void setResolver(FieldResolver resolver);
}
