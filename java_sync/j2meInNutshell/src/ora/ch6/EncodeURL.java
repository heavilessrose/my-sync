package ora.ch6;

/**
 * A class that encodes URL parameter values
 * for MIDP devices.
 */
public class EncodeURL {

    // The characters that do not need to
    // be converted.
    private static final String noEncode =
        "abcdefghijklmnopqrstuvwxyz" +
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ" +
        "0123456789.*-_";

    // Mapping value values 0 through 15 to the
    // corresponding hexadecimal character.
    private static final char[] hexDigits = {
        '0', '1', '2', '3', '4', '5', '6', '7',
        '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'
    };

    // Encodes the given string as required for
    // use in a URL query string or POST data.
    public static String encode(String src) {
        StringBuffer result = new StringBuffer(src.length());
        int count = src.length();
        for (int i = 0; i < count; i++) {
            char c = src.charAt(i);
            if (noEncode.indexOf(c) != -1) {
                // This is a character that does not
                // need to be encoded
                result.append(c);
                continue;
            }

            // Space is converted to '+'
            if (c == ' ') {
                result.append('+');
                continue;
            }

            // The remaining characters must be converted to
            // '%XY' where 'XY' is the hexadecimal value of
            // the character itself.
            result.append('%');
            result.append(hexDigits[(c >> 4) & 0xF]);
            result.append(hexDigits[c & 0xF]);
        }
        return result.toString();
    }
}