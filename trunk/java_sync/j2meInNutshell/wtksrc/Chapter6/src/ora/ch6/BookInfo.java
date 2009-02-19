package ora.ch6;

import java.io.InputStream;
import java.io.IOException;

/**
 * A class that represents a book listing
 * at an online book set, including the number
 * of reviews for the book and its sales ranking.
 */
public class BookInfo {

    int    id;          // Used when persisting
    String isbn;        // The book ISBN
    String title;       // The book title
    int    reviews;     // Number of reviews
    int    ranking;     // Current ranking
    int    lastReviews; // Last review count
    int    lastRanking;  // Last ranking

    public BookInfo(String isbn) {
        this.isbn = isbn;
    }

    public String getIsbn() {
        return isbn;
    }

    public String getTitle() {
        return title;
    }

    public int getReviews() {
        return reviews;
    }

    public int getRanking() {
        return ranking;
    }

    public int getLastReviews() {
        return lastReviews;
    }

    public int getLastRanking() {
        return lastRanking;
    }

    // Installs details parsed from an input stream
    public void setFromInputStream(InputStream is) {
        // Use an InputHelper to search the input
        InputHelper helper = new InputHelper(is);
        try {

            // Default new values to current values
            int newRanking = this.ranking;
            int newReviews = this.reviews;

            boolean found = helper.moveAfterString("buying info: ");
            if (!found) {
                return;
            }

            // Gather the title from the rest of this line
            StringBuffer titleBuffer = helper.getRestOfLine();

            // Look for the number of reviews
            found = helper.moveAfterString("Based on ");
            if (!found) {
                return;
            }

            // Gather the number of reviews from the current location
            String reviewString = helper.gatherNumber();

            // Look for the sales rank
            found = helper.moveAfterString("Sales Rank: ");
            if (!found) {
                return;
            }

            // Gather the number from the current location
            String rankingString = helper.gatherNumber();

            // Having safely found everything, set the new title
            title = titleBuffer.toString().trim();

            // Now convert the reviews and ranking to integers.
            // If they fail to convert, just leave the existing
            // values.
            try {
                newRanking = Integer.parseInt(rankingString);
            } catch (NumberFormatException ex) {
            }

            if (newRanking != ranking) {
                lastRanking = ranking;
                ranking = newRanking;
                if (lastRanking == 0) {
                    // First time, set last and current
                    // to the same value
                    lastRanking = ranking;
                }
            }

            try {
                newReviews = Integer.parseInt(reviewString);
            } catch (NumberFormatException ex) {
            }

            if (newReviews != reviews) {
                lastReviews = reviews;
                reviews = newReviews;
                if (lastReviews == 0) {
                    // First time, set last and current
                    // to the same value
                    lastReviews = reviews;
                }
            }
        } catch (IOException ex) {
        } finally {
            // Allow garbage collection
            helper.dispose();
            helper = null;
        }
    }
}