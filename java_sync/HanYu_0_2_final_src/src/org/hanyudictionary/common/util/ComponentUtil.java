package org.hanyudictionary.common.util;

import java.awt.Component;
import java.awt.Dimension;
import java.awt.Toolkit;


public class ComponentUtil {
	public static void setComponentBoundsToCenterScreen(Component component) {
		Dimension screen = Toolkit.getDefaultToolkit().getScreenSize();
		int x = (int)(screen.getWidth() - component.getHeight()) / 2;
		int y = (int)(screen.getHeight() - component.getHeight()) / 2;
		component.setBounds(x, y, component.getWidth(), component.getHeight());
	}
}
