package com.sun.j2me.blueprints.smartticket.shared.midp.model;

import java.io.*;
import com.sun.j2me.blueprints.smartticket.shared.midp.ApplicationException;

public class SeatingPlan {

	public static final byte AVAILABLE = 0;
	public static final byte UNAVAILABLE = 1;
	public static final byte BOOKED = 2;
	public static final byte AISLE = 9;
	private byte[] seatMap;
	private int rowLength;

	public SeatingPlan(byte[] seatMap, int rowLength) {
		this.seatMap = seatMap;
		this.rowLength = rowLength;

		return;
	}

	public byte[] getSeatMap() {
		return seatMap;
	}

	public int getRowCount() {
		return seatMap.length / rowLength;
	}

	public int getRowLength() {
		return rowLength;
	}

	public byte getStatusOf(int rowNumber, int seatNumber) {
		return seatMap[rowNumber * rowLength + seatNumber];
	}

	public void setStatusOf(int rowNumber, int seatNumber, byte status) {
		seatMap[rowNumber * rowLength + seatNumber] = status;

		return;
	}

	public int getBookedSeatsCount() {
		int bookedSeatCount = 0;

		for (int i = 0; i != seatMap.length; i++) {
			if (seatMap[i] == BOOKED) {
				bookedSeatCount++;
			}
		}

		return bookedSeatCount;
	}

	public Seat[] getBookedSeats() {
		Seat[] bookedSeats = new Seat[getBookedSeatsCount()];
		int j = 0;

		for (int i = 0; i < seatMap.length; i++) {
			if (seatMap[i] == BOOKED) {
				bookedSeats[j++] = new Seat(i / rowLength, i % rowLength);
			}
		}

		return bookedSeats;
	}

	public void serialize(DataOutputStream dataStream) throws ApplicationException {
		try {
			dataStream.writeInt(seatMap.length);
			dataStream.write(seatMap, 0, seatMap.length);
			dataStream.writeInt(rowLength);

			return;
		} catch (IOException ioe) {
			throw new ApplicationException(ioe);
		}
	}

	public static SeatingPlan deserialize(DataInputStream dataStream) throws ApplicationException {
		try {
			byte[] data = new byte[dataStream.readInt()];

			dataStream.readFully(data);

			return new SeatingPlan(data, dataStream.readInt());
		} catch (IOException ioe) {
			throw new ApplicationException(ioe);
		}
	}

	public String toString() {
		StringBuffer buf = new StringBuffer();

		for (int i = 0; i < seatMap.length; i++) {
			buf.append(String.valueOf(seatMap[i]));

			if ((i % rowLength) == (rowLength - 1)) {
				buf.append('\n');
			}
		}

		return buf.toString();
	}

	public static class Seat {

		private int rowNumber;
		private int seatNumber;

		public Seat(int rowNumber, int seatNumber) {
			this.rowNumber = rowNumber;
			this.seatNumber = seatNumber;

			return;
		}

		private Seat() {
		}

		public int getRowNumber() {
			return rowNumber;
		}

		public int getSeatNumber() {
			return seatNumber;
		}

		public void serialize(DataOutputStream dataStream) throws ApplicationException {
			try {
				dataStream.writeInt(rowNumber);
				dataStream.writeInt(seatNumber);

				return;
			} catch (IOException ioe) {
				throw new ApplicationException(ioe);
			}
		}

		public static SeatingPlan.Seat deserialize(DataInputStream dataStream)
				throws ApplicationException {
			try {
				Seat seat = new Seat();

				seat.rowNumber = dataStream.readInt();
				seat.seatNumber = dataStream.readInt();

				return seat;
			} catch (IOException ioe) {
				throw new ApplicationException(ioe);
			}
		}

	}

}
