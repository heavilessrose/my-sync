package com.example.tdd;

public class TemperatureConverter {
	public static final int ABSOLUTE_ZERO_C = -273;

	public static int celsiusToFahrenheit(int celsius) {
		if (celsius < ABSOLUTE_ZERO_C) {
			throw new RuntimeException("Invalid temperature: " + celsius
					+ " below absolute zero");
		}
		return (int) Math.round(celsius * 1.8 + 32);
	}
}
