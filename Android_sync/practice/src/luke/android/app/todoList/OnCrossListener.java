package luke.android.app.todoList;
public interface OnCrossListener {
	
	/**
	 * Crossing event, someone has performed a cross/uncross action over the given position
	 * @param position List position where crossing action happened
	 * @param crossed True if crossing action, false if uncrossing
	 */
	void onCross(int position, boolean crossed);
}
