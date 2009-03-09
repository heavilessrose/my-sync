package org.kalmeo.util.resource;

import java.io.IOException;
import java.io.InputStream;
import java.util.Hashtable;

import javax.microedition.lcdui.Image;

/**
 * @author bbeaulant
 */
public class ImageManager {

	// The ImageManager static instance
	public static final ImageManager instance = new ImageManager();

	// Image cache
	private final Hashtable images = new Hashtable();

	/**
	 * Retrieve an {@link Image} instance from the specified <code>path</code>.
	 * 
	 * @param path
	 * @return an {@link Image} instance
	 */
	public Image getImage(String path) {

		// Check path validity
		if ((path == null) || (path.length() == 0)) {
			return null;
		}

		// Check if image is already cached
		if (images.containsKey(path)) {
			return (Image) images.get(path);
		}

		// Load the requested image
		Image image = null;
		try {
			image = Image.createImage(path);
		} catch (IOException ioe) {
		}
		if (image == null) {
			try {
				InputStream is = getClass().getResourceAsStream(path);
				if (is != null) {
					byte[] imageData = new byte[is.available()];
					is.read(imageData, 0, imageData.length);
					image = Image.createImage(imageData, 0, imageData.length);
					is.close();
				}
			} catch (Throwable e) {
			}
		}

		// Add image instance to cache
		if (image != null) {
			images.put(path, image);
		} else {
			System.err.println("Error loading : " + path);
		}

		return image;
	}

	/**
	 * Release an image instance from cache.
	 * 
	 * @param path
	 */
	public void releaseImage(String path) {
		if (images.containsKey(path)) {
			images.remove(path);
		}
	}

	/**
	 * Release all cached image instances.
	 */
	public void releaseAll() {
		images.clear();
		System.gc();
	}

}
