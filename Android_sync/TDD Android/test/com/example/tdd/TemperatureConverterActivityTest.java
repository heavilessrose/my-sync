package com.example.tdd;

import java.util.HashMap;
import java.util.Map;

import junit.framework.TestCase;

public class TemperatureConverterActivityTest extends TestCase {
	public void testCelsiusToFahrenheit() {
		// fail("Not implemented yet");
		for (int c : conversionTable.keySet()) {
			int f = conversionTable.get(c);
			String msg = "" + c + "C -> " + f + "F";
			assertEquals(msg, f, TemperatureConverter.celsiusToFahrenheit(c));
		}
	}

	/** 测试是否在出现小于绝对0度的情况时抛出了异常 */
	public void testInvalidCelsiusTemperature() {
		// fail("Not implemented yet");
		try {
			int f = TemperatureConverter.celsiusToFahrenheit(-274);
		} catch (RuntimeException ex) {
			if (ex.getMessage().contains("below absolute zero")) {
				return;
			} else {
				fail("Undetected temperature below absolute zero: "
						+ ex.getMessage());
			}
		}
		fail("Undetected temperature below absolute zero: no exception generated");
	}

	private static final Map<Integer, Integer> conversionTable = new HashMap<Integer, Integer>();

	static { // initialize (c, f) pairs
		conversionTable.put(0, 32);
		conversionTable.put(100, 212);
		conversionTable.put(-1, 30);
		conversionTable.put(-100, -148);
		conversionTable.put(32, 90);
		conversionTable.put(-40, -40);
		conversionTable.put(-273, -459);
	}
}
