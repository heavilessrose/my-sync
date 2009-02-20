package bm.core.math;

import bm.core.ResourceManager;
import bm.core.io.Serializable;
import bm.core.io.SerializationException;
import bm.core.io.SerializerInputStream;
import bm.core.io.SerializerOutputStream;
import bm.core.tools.Tools;

/*
 * File Information
 *
 * Created on       : 2005/05/30
 * Created by       : narciso
 * Last modified by : $Author:narciso $
 * Last modified on : $Date:2007-02-03 19:34:57 +0100 (s谩b, 03 feb 2007) $
 * Revision         : $Revision:4 $
 */

/**
 * 小数的定点数表示.Represents a decimal fixed point number, targeted to CLDC 1.0
 * platforms.<br/>
 * 
 * All operations on an instance of FixedPoint change the underlying value, so
 * it's not Immutable. The reason is to prevent the creation of unneeded objects
 * which is not recommended for J2ME as causes performance problems.<br/>
 * 
 * Be aware that you can always expect more rounding problems with fixed point
 * numbers than with floating point numbers.<br/>
 * This class allows the parsing and formatting of numbers using different
 * decimal separator and grouping separator characters, so it is well suited for
 * internationzalizable applications.<br/>
 * 
 * @author <a href="mailto:narciso@elondra.com">Narciso Cerezo</a>
 * @version $Revision:4 $
 */
public class FixedPoint implements Serializable {
	public static final FixedPoint ZERO = new FixedPoint(0);
	public static final FixedPoint ONE = new FixedPoint(1);

	/**
	 * Commodity definition to prevent excesive object creation.
	 */
	public static final Character DOT = new Character('.');
	/**
	 * Commodity definition to prevent excesive object creation.
	 */
	public static final Character COMMA = new Character(',');

	private static final int MAX_PRECISION = 18;

	private static char decimalSeparator = ',';
	private static Character groupingSeparator;
	private static String moneySymbol = "Y";
	private static boolean moneySymbolAfter = true;

	static {
		reloadLanguageSettings();
	}

	/**
	 * Fixed point value hold as a long.
	 */
	private long value;
	/**
	 * Precision on the value. Max 18.
	 */
	private short precision;
	/**
	 * Commodity value to avoid excesive calculation, holds 10 power precission.
	 */
	private transient long factor = 1;
	private static final short DEFAULT_MONEY_PRECISION = (short) 2;

	public FixedPoint() {
	}

	/**
	 * Constructor.
	 * 
	 * @param value
	 *            fixed point value with a precission of 0 digits
	 */
	public FixedPoint(final long value) {
		this.value = value;
	}

	/**
	 * Constructor.
	 * 
	 * @param value
	 *            fixed point value with the given precission
	 * @param precision
	 *            precission for the value, greater than or equal to 0
	 */
	public FixedPoint(final long value, final short precision) {
		this.value = value;
		this.precision = precision > 0 ? precision < MAX_PRECISION ? precision
				: MAX_PRECISION : 0;
		this.factor = Tools.power(10, precision);
	}

	/**
	 * Get the name of the class to be used for serialization/deserialization of
	 * complex/nested objects.
	 * 
	 * @return class name
	 */
	public String getSerializableClassName() {
		return "bm.core.math.FixedPoint";
	}

	/**
	 * Get the precission of this number.
	 * 
	 * @return precission
	 */
	public short getPrecision() {
		return precision;
	}

	/**
	 * Set the precission of this value. Stored value will change accordingly
	 * mantaining the same real value. If the precission is less than the actual
	 * one, the stored value is rounded to the new one.
	 * 
	 * @param precision
	 *            new precission, greater than or equal to 0
	 * @return a fixed point with the new fixed precision
	 * @noinspection FieldRepeatedlyAccessedInMethod
	 */
	public FixedPoint setPrecision(final int precision) {
		if (precision != this.precision && precision >= 0) {
			final FixedPoint result = clone();
			if (precision > result.precision) {
				result.value *= Tools.power(10, precision - result.precision);
			} else {
				result.value = round(precision);
			}
			result.precision = (short) precision;
			if (precision < 0) {
				result.precision = 0;
			} else if (precision > MAX_PRECISION) {
				result.precision = MAX_PRECISION;
			}
			result.factor = Tools.power(10, precision);
			return result;
		} else {
			return this;
		}
	}

	/**
	 * Add operation.
	 * 
	 * @param number
	 *            FixedPoint number to add
	 * @return new FixedPoint object with result
	 */
	public FixedPoint add(final FixedPoint number) {
		// Stupid error when directly summed, but not using a variable !!!!
		final FixedPoint result = clone();
		// If we inline this variable it fails!!!!!!
		// noinspection UnnecessaryLocalVariable
		final long fixed = result.fixPrecission(number);
		result.value += fixed;
		return result;
	}

	/**
	 * Add operation.
	 * 
	 * @param value
	 *            a real value (not a fixed point value as a long)
	 * @return new FixedPoint object with result
	 */
	public FixedPoint add(final long value) {
		final FixedPoint result = clone();
		result.value += (value * factor);
		return result;
	}

	/**
	 * Add operation.
	 * 
	 * @param value
	 *            a real value (not a fixed point value as an int)
	 * @return new FixedPoint object with result
	 */
	public FixedPoint add(final int value) {
		final FixedPoint result = clone();
		result.value += (value * factor);
		return result;
	}

	/**
	 * Substraction operation.
	 * 
	 * @param number
	 *            FixedPoint number to substract
	 * @return new FixedPoint object with result
	 */
	public FixedPoint sub(final FixedPoint number) {
		final FixedPoint result = clone();
		// If we inline this variable it fails!!!!!!
		// noinspection UnnecessaryLocalVariable
		final long fixedValue = result.fixPrecission(number);
		result.value -= fixedValue;
		return result;
	}

	/**
	 * Substraction operation.
	 * 
	 * @param value
	 *            a real value (not a fixed point value as a long)
	 * @return new FixedPoint object with result
	 */
	public FixedPoint sub(final long value) {
		final FixedPoint result = clone();
		result.value -= (value * factor);
		return result;
	}

	/**
	 * Substraction operation.
	 * 
	 * @param value
	 *            a real value (not a fixed point value as an int)
	 * @return new FixedPoint object with result
	 */
	public FixedPoint sub(final int value) {
		final FixedPoint result = clone();
		result.value -= (value * factor);
		return result;
	}

	/**
	 * Division operation.
	 * 
	 * @param number
	 *            FixedPoint number as divisor
	 * @return new FixedPoint object with result
	 */
	public FixedPoint div(final FixedPoint number) {
		final FixedPoint result = clone();
		if (number.getRawValue() != 0) {
			result.value *= factor;
			// If we inline this variable it fails!!!!!!
			// noinspection UnnecessaryLocalVariable
			final long fixedValue = result.fixPrecission(number);
			result.value /= fixedValue;
		} else {
			throw new ArithmeticException(ResourceManager
					.getResource("fixedPoint.divisionByZero"));
		}
		return result;
	}

	/**
	 * Division operation.
	 * 
	 * @param value
	 *            a real value as divisor (not a fixed point value as a long)
	 * @return new FixedPoint object with result
	 */
	public FixedPoint div(final long value) {
		final FixedPoint result = setPrecision(precision + 1);
		if (value != 0) {
			result.value /= value;
			return result.setPrecision(precision);
		} else {
			throw new ArithmeticException(ResourceManager
					.getResource("fixedPoint.divisionByZero"));
		}
	}

	/**
	 * Division operation.
	 * 
	 * @param value
	 *            a real value as divisor (not a fixed point value as an int)
	 * @return new FixedPoint object with result
	 */
	public FixedPoint div(final int value) {
		return div((long) value);
	}

	/**
	 * Multiplication operation.
	 * 
	 * @param number
	 *            FixedPoint number to multiply
	 * @return new FixedPoint object with result
	 */
	public FixedPoint mult(final FixedPoint number) {
		final FixedPoint result = clone();
		final long fixedValue = result.fixPrecission(number);
		result.value = round(value * fixedValue, (short) (precision * 2),
				precision);
		return result;
	}

	/**
	 * Multiplication operation.
	 * 
	 * @param value
	 *            a real value (not a fixed point value as a long)
	 * @return new FixedPoint object with result
	 */
	public FixedPoint mult(final long value) {
		final FixedPoint result = clone();
		result.value *= value;
		return result;
	}

	/**
	 * Multiplication operation.
	 * 
	 * @param value
	 *            a real value (not a fixed point value as an int)
	 * @return new FixedPoint object with result
	 */
	public FixedPoint mult(final int value) {
		final FixedPoint result = clone();
		result.value *= value;
		return result;
	}

	/**
	 * Return the integer part of the number as a long. Is the same as
	 * truncating to 0 decimals.
	 * 
	 * @return truncated long value of this number
	 */
	public long trunc() {
		return value / factor;
	}

	/**
	 * Truncate the number to the given number of decimals.<br/>
	 * This mehtod is similar to setPrecission, but makes no rounding.
	 * 
	 * @param precision
	 *            number of decimals
	 * @return truncated number or this number if current precission is the same
	 */
	public FixedPoint trunc(final int precision) {
		if (precision != this.precision && precision >= 0) {
			final FixedPoint result = clone();
			if (precision > result.precision) {
				result.value *= Tools.power(10, precision - result.precision);
			} else {
				result.value /= Tools.power(10, result.precision - precision);
			}
			result.precision = (short) precision;
			if (precision < 0) {
				result.precision = 0;
			} else if (precision > MAX_PRECISION) {
				result.precision = MAX_PRECISION;
			}
			result.factor = Tools.power(10, precision);
			return result;
		} else {
			return this;
		}
	}

	/**
	 * Get the String representation of this number.
	 * 
	 * @param decimalSeparator
	 *            the character to use as decimal separator
	 * @param groupingSeparator
	 *            the character to use as grouping separator, if null no
	 *            grouping separation will be done. For commodity you can use
	 *            the DOT and COMMA constants on this class
	 * @return textual representation of this number
	 */
	public String toString(final char decimalSeparator,
			final Character groupingSeparator) {
		return toString(value, precision, decimalSeparator, groupingSeparator);
	}

	/**
	 * Get the String representation of this number, performing the requested
	 * rounding.
	 * 
	 * @param decimals
	 *            digits to round to, if precission is greater, otherwise no
	 *            rounding is done
	 * @param decimalSeparator
	 *            the character to use as decimal separator
	 * @param groupingSeparator
	 *            the character to use as grouping separator, if null no
	 *            grouping separation will be done. For commodity you can use
	 *            the DOT and COMMA constants on this class
	 * @return textual representation of this number
	 */
	public String toString(final int decimals, final char decimalSeparator,
			final Character groupingSeparator) {
		return setPrecision(decimals).toString(decimalSeparator,
				groupingSeparator);
	}

	/**
	 * Get the String representation of this number, performing the requested
	 * rounding.
	 * 
	 * @param decimals
	 *            digits to round to, if precission is greater, otherwise no
	 *            rounding is done
	 * @return textual representation of this number
	 */
	public String toString(final int decimals) {
		return setPrecision(decimals).toString(decimalSeparator,
				groupingSeparator);
	}

	/**
	 * Get the String representation of this number, using the default
	 * decimalSeparator and groupingSeparator.
	 * 
	 * @return textual representation of this number
	 */
	public String toString() {
		return toString(value, precision, decimalSeparator, groupingSeparator);
	}

	/**
	 * Get the String representation of this number, using the default
	 * decimalSeparator and no grouping.
	 * 
	 * @return textual representation of this number
	 */
	public String toStringNoGrouping() {
		return toString(value, precision, decimalSeparator, null);
	}

	/**
	 * Get the String representation of this number, using the default
	 * decimalSeparator and groupingSeparator, and appending the default money
	 * symbol.
	 * 
	 * @param decimals
	 *            decimals
	 * @return textual representation of this number
	 */
	public String toMoneyString(final int decimals) {
		return toMoneyString(decimals, decimalSeparator, groupingSeparator,
				moneySymbol, moneySymbolAfter);
	}

	/**
	 * Get the String representation of this number, using the default
	 * decimalSeparator and groupingSeparator, and appending the default money
	 * symbol.
	 * 
	 * @return textual representation of this number
	 */
	public String toMoneyString() {
		return toMoneyString(DEFAULT_MONEY_PRECISION);
	}

	/**
	 * Get the String representation of this number, using the default
	 * decimalSeparator and groupingSeparator, and appending the default money
	 * symbol.
	 * 
	 * @param decimals
	 *            number of decimals to round to
	 * @param decimalSeparator
	 *            decimal separator, mandatory
	 * @param groupingSeparator
	 *            grouping separator, can be null
	 * @param moneySymbol
	 *            money symbol to use
	 * @param moneySymbolAfter
	 *            whether the symbols goes before of after the number
	 * @return textual representation of this number
	 */
	public String toMoneyString(final int decimals,
			final char decimalSeparator, final Character groupingSeparator,
			final String moneySymbol, final boolean moneySymbolAfter) {
		if (moneySymbolAfter) {
			return setPrecision(decimals).toString(decimalSeparator,
					groupingSeparator)
					+ moneySymbol;
		} else {
			return moneySymbol
					+ setPrecision(decimals).toString(decimalSeparator,
							groupingSeparator);
		}
	}

	/**
	 * Get the String representation of this number, using the default
	 * decimalSeparator and groupingSeparator, and appending the default money
	 * symbol.
	 * 
	 * @param decimalSeparator
	 *            decimal separator, mandatory
	 * @param groupingSeparator
	 *            grouping separator, can be null
	 * @param moneySymbol
	 *            money symbol to use
	 * @param moneySymbolAfter
	 *            whether the symbols goes before of after the number
	 * @return textual representation of this number
	 */
	public String toMoneyString(final char decimalSeparator,
			final Character groupingSeparator, final String moneySymbol,
			final boolean moneySymbolAfter) {
		if (moneySymbolAfter) {
			return toString(value, DEFAULT_MONEY_PRECISION, decimalSeparator,
					groupingSeparator)
					+ moneySymbol;
		} else {
			return moneySymbol
					+ toString(value, DEFAULT_MONEY_PRECISION,
							decimalSeparator, groupingSeparator);
		}
	}

	/**
	 * Compare this number to another FixedPoint number.
	 * 
	 * @param number
	 *            number to compare to
	 * @return 0 if both are equal, 1 if this number is greater than number, and
	 *         -1 if number is greater than this one
	 */
	public int compareTo(final FixedPoint number) {
		final long thisValue;
		final long numberValue;
		// local variable to improve speed on CDC
		final short precision = this.precision;
		final long value = this.value;

		if (precision == number.precision) {
			thisValue = value;
			numberValue = number.value;
		} else if (precision > number.precision) {
			thisValue = value;
			numberValue = number.value
					* Tools.power(10, precision - number.precision);
		} else {
			numberValue = number.value;
			thisValue = value * Tools.power(10, number.precision - precision);
		}

		if (thisValue == numberValue) {
			return 0;
		} else if (thisValue > numberValue) {
			return 1;
		} else {
			return -1;
		}
	}

	/**
	 * Clone this object.
	 * 
	 * @return a new object with the same properties
	 */
	public FixedPoint clone() {
		return new FixedPoint(value, precision);
	}

	/**
	 * Test if the given object is a FixedPoint number and is equal to this one.
	 * 
	 * @param o
	 *            object to test
	 * @return true if both objects represent the same number
	 * @throws ClassCastException
	 *             if object is not a FixedPoint instance
	 */
	public boolean equals(final Object o) {
		return this == o || !(o == null || getClass() != o.getClass())
				&& toString().equals(o.toString());

	}

	/**
	 * Calculate this number's hash code.
	 * 
	 * @return hash code
	 */
	public int hashCode() {
		int result;
		result = (int) (value ^ (value >>> 32));
		result = 29 * result + (int) precision;
		return result;
	}

	/**
	 * Write object status to stream.
	 * 
	 * @param out
	 *            output stream
	 * 
	 * @throws bm.core.io.SerializationException
	 *             on errors
	 */
	public void serialize(final SerializerOutputStream out)
			throws SerializationException {
		FixedPoint.serialize(out, this, true);
	}

	/**
	 * Read object status from stream.
	 * 
	 * @param in
	 *            input stream
	 * 
	 * @throws bm.core.io.SerializationException
	 *             on errors
	 */
	public void deserialize(final SerializerInputStream in)
			throws SerializationException {
		final FixedPoint value = FixedPoint.deserialize(in, true);
		this.value = value.value;
		this.precision = value.precision;
		this.factor = value.factor;
	}

	public static void reloadLanguageSettings() {
		String resource = ResourceManager
				.getResource("fixedPoint.decimalSeparator");
		if (resource != null) {
			decimalSeparator = resource.charAt(0);
		}
		resource = ResourceManager.getResource("fixedPoint.groupingSeparator");
		if (resource != null) {
			groupingSeparator = new Character(resource.charAt(0));
		} else {
			groupingSeparator = null;
		}
		resource = ResourceManager.getResource("fixedPoint.moneySimbol");
		if (resource != null) {
			moneySymbol = resource;
		}
		resource = ResourceManager.getResource("fixedPoint.moneySimbolAfter");
		if (resource != null) {
			moneySymbolAfter = resource.toLowerCase().equals("true");
		}
	}

	/**
	 * Serialize a FixedPoint number to an output stream.
	 * 
	 * @param out
	 *            output
	 * @param number
	 *            number
	 * @param nullable
	 *            if can be null
	 * @throws SerializationException
	 *             on errors
	 */
	public static void serialize(final SerializerOutputStream out,
			final FixedPoint number, final boolean nullable)
			throws SerializationException {
		if (nullable) {
			out.writeBoolean(number == null);
		}
		if (number != null) {
			out.writeLong(number.value);
			out.writeShort(number.precision);
		}
	}

	/**
	 * Deserialize a FixedPoint number from an input stream.
	 * 
	 * @param in
	 *            input
	 * @param nullable
	 *            if it can be null
	 * @return FixedPoint number or null
	 * @throws bm.core.io.SerializationException
	 *             on errors
	 */
	public static FixedPoint deserialize(final SerializerInputStream in,
			final boolean nullable) throws SerializationException {
		if (nullable) {
			if (in.readBoolean()) {
				return null;
			}
		}
		final FixedPoint fp = new FixedPoint(0);
		fp.value = in.readLong();
		fp.precision = in.readShort();
		if (fp.precision < 0) {
			fp.precision = 0;
		} else if (fp.precision > MAX_PRECISION) {
			fp.precision = MAX_PRECISION;
		}
		fp.factor = Tools.power(10, fp.precision);
		return fp;
	}

	/**
	 * Constructs a new FixedPoint number parsing it from a String source.
	 * 
	 * @param source
	 *            string representation
	 * @param decimalSeparator
	 *            decimal separator to use
	 * @return parsed FixedPoint number, if the source is null or empty returns
	 *         ZERO.
	 * @throws NumberFormatException
	 *             if the string does not represent a valid number
	 */
	public static FixedPoint parse(final String source,
			final char decimalSeparator) throws NumberFormatException {
		if (source == null || source.trim().length() == 0) {
			return ZERO;
		}
		int sign = 1;
		int startPosition = 0;
		final char[] characters = source.toCharArray();
		if (characters[0] == '-') {
			sign = -1;
			startPosition = 1;
		}
		short precision = -1;
		long value = 0;
		final int length = characters.length;
		for (int i = startPosition; i < length; i++) {
			if (characters[i] == decimalSeparator) {
				if (precision == -1) {
					precision++;
				}
			} else if (characters[i] == '.' || characters[i] == ',') {
				// Dot or comma after decimal separator has been detected mean
				// nothing
			} else if (characters[i] >= '0' && characters[i] <= '9') {
				value = (value * 10) + (characters[i] - '0');
				if (precision >= 0) {
					precision++;
				}
			} else {
				throw new NumberFormatException();
			}
		}
		if (sign == -1) {
			value *= -1;
		}
		return new FixedPoint(value, precision);
	}

	/**
	 * Constructs a new FixedPoint number parsing it from a String source, using
	 * the default decimal separator.
	 * 
	 * @param source
	 *            string representation
	 * @return parsed FixedPoint number.
	 * @throws NumberFormatException
	 *             if the string does not represent a valid number
	 */
	public static FixedPoint parse(final String source)
			throws NumberFormatException {
		return parse(source, decimalSeparator);
	}

	/**
	 * Get the raw value stored on this number.
	 * 
	 * @return the raw value
	 */
	long getRawValue() {
		return value;
	}

	/**
	 * 四舍五入 a fixed point number.
	 * 
	 * @param rawValue
	 *            raw fixed point value to round
	 * @param precision
	 *            current precision
	 * @param decimals
	 *            new precision
	 * @return rounded number
	 */
	private static long round(final long rawValue, final short precision,
			final short decimals) {
		if (decimals >= precision) {
			return rawValue;
		}
		long current = Math.abs(rawValue);
		int positions = precision;
		while (positions > decimals) {
			final long fraction = current % 10;
			current /= 10;
			if (fraction >= 5) {
				current++;
			}
			positions--;
		}
		return rawValue > 0 ? current : current * -1;
	}

	/**
	 * Round this number to a new precission.
	 * 
	 * @param decimals
	 *            new precission
	 * @return rounded raw fixed point number
	 */
	private long round(final int decimals) {
		return round(value, precision, (short) decimals);
	}

	/**
	 * Get the String representation of this number, performing the requested
	 * rounding.
	 * 
	 * @param number
	 *            raw fixed point number to represent
	 * @param precission
	 *            number of decimals to use
	 * @param decimalSeparator
	 *            the character to use as decimal separator
	 * @param groupingSeparator
	 *            the character to use as grouping separator, if null no
	 *            grouping separation will be done. For commodity you can use
	 *            the DOT and COMMA constants on this class
	 * @return textual representation of this number
	 */
	private static String toString(final long number, final short precission,
			final char decimalSeparator, final Character groupingSeparator) {
		final long abs;
		final String sign;
		if (number >= 0) {
			sign = "";
			abs = number;
		} else {
			abs = -number;
			sign = "-";
		}
		final String base = Long.toString(abs);

		final String decimal;
		String integer;
		if (precission > 0) {
			final int splitPoint = base.length() - precission;
			if (splitPoint >= 0) {
				decimal = base.substring(splitPoint);
				if (splitPoint > 0) {
					integer = base.substring(0, splitPoint);
				} else {
					integer = "0";
				}
			} else {
				integer = "0";
				decimal = Tools.repeat("0", precission - base.length()) + base;
			}
		} else {
			decimal = "";
			integer = base;
		}
		if (groupingSeparator != null) {
			final StringBuffer buffer = new StringBuffer();
			final char[] chars = integer.toCharArray();
			final int length = chars.length;
			int count = 0;
			for (int i = length - 1; i >= 0; i--) {
				buffer.insert(0, chars[i]);
				count++;
				if (count == 3) {
					count = 0;
					if (i > 0 && chars[i - 1] != '-') {
						buffer.insert(0, groupingSeparator);
					}
				}
			}
			integer = buffer.toString();
		}
		if (precission > 0) {
			return sign + integer + decimalSeparator + decimal;
		} else {
			return sign + integer;
		}
	}

	/**
	 * For operations with other FixedPoint numbers gets a value with the right
	 * precission, and if the number has a greater precission sets the
	 * precission of this instance to it, so the result always has the highest
	 * precission of the two.
	 * 
	 * @param number
	 *            source fixed point number
	 * @return raw fixed point value
	 * @noinspection FieldRepeatedlyAccessedInMethod
	 */
	private long fixPrecission(final FixedPoint number) {
		if (number.precision > precision) {
			final FixedPoint fp = setPrecision(number.precision);
			factor = fp.factor;
			value = fp.value;
			precision = fp.precision;
			return number.value;
		} else if (number.precision < precision) {
			return number.value * Tools.power(10, precision - number.precision);
		} else {
			return number.value;
		}
	}

	/**
	 * Get the default decimal separator. For parse and toString methods wich do
	 * not get it as a parameter. Default value is a comma.
	 * 
	 * @return default decimal separator
	 */
	public static char getDecimalSeparator() {
		return decimalSeparator;
	}

	/**
	 * Set the default decimal separator.
	 * 
	 * @param decimalSeparator
	 *            new decimal separator
	 */
	public static synchronized void setDecimalSeparator(
			final char decimalSeparator) {
		FixedPoint.decimalSeparator = decimalSeparator;
	}

	/**
	 * Geth the default grouping separator. For toString methods wich do no get
	 * it as a parameter. Default value is null (no grouping separator).
	 * 
	 * @return default grouping separator
	 */
	public static Character getGroupingSeparator() {
		return groupingSeparator;
	}

	/**
	 * Set the default grouping separator.
	 * 
	 * @param groupingSeparator
	 *            new grouping separator
	 */
	public static synchronized void setGroupingSeparator(
			final Character groupingSeparator) {
		FixedPoint.groupingSeparator = groupingSeparator;
	}

	/**
	 * Get the default money symbol. This symbol is used in the toMoneyString
	 * methods.
	 * 
	 * @return default money symbol
	 */
	public static String getMoneySymbol() {
		return moneySymbol;
	}

	/**
	 * Set the default money symbol.
	 * 
	 * @param moneySymbol
	 *            new money symbol
	 */
	public static synchronized void setMoneySymbol(final String moneySymbol) {
		FixedPoint.moneySymbol = moneySymbol;
	}

	/**
	 * Check if the money symbol goes after the quantity, if false it goes
	 * before it.
	 * 
	 * @return if the symbol goes after the quantity
	 */
	public static boolean isMoneySymbolAfter() {
		return moneySymbolAfter;
	}

	/**
	 * Set if the money symbol goes after the quantity.
	 * 
	 * @param moneySymbolAfter
	 *            if true it goes after, if false it goes before.
	 */
	public static synchronized void setMoneySymbolAfter(
			final boolean moneySymbolAfter) {
		FixedPoint.moneySymbolAfter = moneySymbolAfter;
	}
}
