/*
 * Created on 05-Apr-2006
 * 
 * Author: Georgi R. Georgiev
 * Copyright: Georgi R. Georgiev
 *     This file is part of MobiDict

    MobiDict is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    MobiDict is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Foobar; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

 */
package dict.search;

import javax.microedition.lcdui.Alert;

/**
 * This is an interface to report messages and end tasks to the GUI layer
 * 
 * @author Georgi R. Georgiev
 */
public interface Callback {
    /**
     * Report alert to the gui layer
     * @param alert alert to be shown
     */
    public void reportAlert(Alert alert);
    /**
     * Report that the task is ended normally
     */
    public void reportEndTask();
}
