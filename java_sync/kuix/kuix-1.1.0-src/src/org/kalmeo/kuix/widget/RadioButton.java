package org.kalmeo.kuix.widget;

import org.kalmeo.kuix.core.KuixConstants;

/**
 * This class represents a radio button. <br>
 * <br>
 * <strong>For further informations, visit the <a
 * href="http://www.kalmeo.org/files/kuix/widgetdoc/index.html"
 * target="new">Kuix widgets reference page</a></strong>.
 * 
 * @author bbeaulant
 */
public class RadioButton extends CheckBox {

	// The associated string value
	private String value;

	/**
	 * Construct a {@link RadioButton}
	 */
	public RadioButton() {
		super(KuixConstants.RADIO_BUTTON_WIDGET_TAG);
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see org.kalmeo.kuix.widget.CheckBox#setAttribute(java.lang.String,
	 * java.lang.String)
	 */
	public boolean setAttribute(String name, String value) {
		if (KuixConstants.VALUE_ATTRIBUTE.equals(name)) {
			setValue(value);
			return true;
		}
		return super.setAttribute(name, value);
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see org.kalmeo.kuix.widget.Widget#getAttribute(java.lang.String)
	 */
	public Object getAttribute(String name) {
		if (KuixConstants.VALUE_ATTRIBUTE.equals(name)) {
			return getValue();
		}
		return super.getAttribute(name);
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see org.kalmeo.kuix.widget.CheckBox#setSelected(boolean)
	 */
	public void setSelected(boolean selected) {
		internalSetSelected(selected, true);
	}

	/**
	 * Used to intercept selection change and propagate the event to the ragio
	 * groupg according to <code>propagateToRadioGroup</code> value.
	 * 
	 * @param selected
	 * @param propagateToRadioGroup
	 */
	protected void internalSetSelected(boolean selected,
			boolean propagateToRadioGroup) {
		if (propagateToRadioGroup) {
			RadioGroup group = getRadioGroup();
			if (group != null) {
				group.setSelectedRadioButton(this, true);
			} else {
				super.setSelected(selected);
			}
		} else {
			super.setSelected(selected);
		}
	}

	/**
	 * @return the value
	 */
	public String getValue() {
		return value;
	}

	/**
	 * @param value
	 *            the value to set
	 */
	public void setValue(String value) {
		this.value = value;
		RadioGroup radioGroup = getRadioGroup();
		if (radioGroup != null) {
			radioGroup.processNewRadioButtonValue(this);
		}
	}

	/**
	 * @return the group
	 */
	public RadioGroup getRadioGroup() {
		if (parent instanceof RadioGroup) {
			return (RadioGroup) parent;
		}
		return null;
	}

}
