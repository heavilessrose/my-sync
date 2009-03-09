package org.kalmeo.kuix.widget;

import org.kalmeo.kuix.core.KuixConstants;

/**
 * This class represents a drag and drop container. <br>
 * <br>
 * <strong>For further informations, visit the <a
 * href="http://www.kalmeo.org/files/kuix/widgetdoc/index.html"
 * target="new">Kuix widgets reference page</a></strong>.
 * 
 * @author bbeaulant
 */
public class DragAndDropContainer extends Widget {

	/**
	 * Construct a {@link DragAndDropContainer}
	 */
	public DragAndDropContainer() {
		super(KuixConstants.DRAG_AND_DROP_CONTAINER_WIDGET_TAG);
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see org.kalmeo.kuix.widget.Widget#processPointerEvent(byte, int, int)
	 */
	public boolean processPointerEvent(byte type, int x, int y) {

		if (type == KuixConstants.POINTER_DRAGGED_EVENT_TYPE) {
			getDesktop().setDraggedWidget(getChild(), x, y);
			markAsValidate();
			return true;
		}

		if (type == KuixConstants.POINTER_DROPPED_EVENT_TYPE) {
			Widget draggedWidget = getDesktop().removeDraggedWidget(false);
			if (draggedWidget != null) {
				add(draggedWidget);
			}
			return true;
		}

		return false;
	}

}
