package gbk;
import java.io.InputStream;

public class HGB2312 {

    private byte[] map = new byte[15228];

    private byte[] buffer;
    private int index;

    public HGB2312() throws Exception {
        InputStream is = getClass().getResourceAsStream("/gb2u.dat");
        is.read(map);
        is.close();
    }

    public String gb2utf8(byte[] gb) throws Exception {
        buffer = new byte[gb.length + gb.length / 2 + 3];
        index = 0;
        int c, h, l, ind;
        for (int i = 0; i < gb.length;) {
            if (gb[i] >= 0) {
                fillBuffer(gb[i++]);
            } else {
                h = 256 + gb[i++];
                l = 256 + gb[i++];
                h = h - 0xA0 - 1;
                l = l - 0xA0 - 1;
                if (h < 9) {
                    ind = (h * 94 + l) << 1;
                    c = (byte2Int(map[ind]) << 8 | byte2Int(map[ind + 1]));
                    fillBuffer(c);
                } else if (h >= 9 && h <= 14) {
                    fillBuffer(0);
                } else if (h > 14) {
                    h -= 6;
                    ind = (h * 94 + l) << 1;
                    c = (byte2Int(map[ind]) << 8 | byte2Int(map[ind + 1]));
                    fillBuffer(c);
                } else {
                    fillBuffer(0);
                }
            }
        }
        // ind = index - 2;
        // h = (byte) ((ind >> 8) & 0x7F);
        // l = (byte) (ind & 0xFF);
        // buffer[0] = h;
        // buffer[1] = l;

        return new String(buffer, 0, index, "UTF-8");
    }

    private void fillBuffer(int value) {
        if (value <= 0x0000007F) {
            buffer[index++] = (byte) value;
        } else if (value >= 0x00000080 && value <= 0x000007FF) {
            byte b1 = (byte) (0x60 | (value >> 6));
            byte b2 = (byte) (0x80 | (value & 0x3F));
            buffer[index++] = b1;
            buffer[index++] = b2;
        } else if (value >= 0x00000800 && value <= 0x0000FFFF) {
            byte b1 = (byte) (0xE0 | (value >> 12));
            byte b2 = (byte) (0x80 | ((value >> 6) & 0x3F));
            byte b3 = (byte) (0x80 | (value & 0x3F));
            buffer[index++] = b1;
            buffer[index++] = b2;
            buffer[index++] = b3;
        } else if (value >= 0x00010000 && value <= 0x001FFFFF) {
            byte b1 = (byte) (0x1E | (value >> 18));
            byte b2 = (byte) (0x80 | ((value >> 12) & 0x3F));
            byte b3 = (byte) (0x80 | ((value >> 6) & 0x3F));
            byte b4 = (byte) (0x80 | (value & 0x3F));
            buffer[index++] = b1;
            buffer[index++] = b2;
            buffer[index++] = b3;
            buffer[index++] = b4;
        } else if (value >= 0x00200000 && value <= 0x03FFFFFF) {
            byte b1 = (byte) (0x3E | (value >> 24));
            byte b2 = (byte) (0x80 | ((value >> 18) & 0x3F));
            byte b3 = (byte) (0x80 | ((value >> 12) & 0x3F));
            byte b4 = (byte) (0x80 | ((value >> 6) & 0x3F));
            byte b5 = (byte) (0x80 | (value & 0x3F));
            buffer[index++] = b1;
            buffer[index++] = b2;
            buffer[index++] = b3;
            buffer[index++] = b4;
            buffer[index++] = b5;
        } else if (value >= 0x04000000 && value <= 0x7FFFFFFF) {
            byte b1 = (byte) (0x7E | (value >> 30));
            byte b2 = (byte) (0x80 | ((value >> 24) & 0x3F));
            byte b3 = (byte) (0x80 | ((value >> 18) & 0x3F));
            byte b4 = (byte) (0x80 | ((value >> 12) & 0x3F));
            byte b5 = (byte) (0x80 | ((value >> 6) & 0x3F));
            byte b6 = (byte) (0x80 | (value & 0x3F));
            buffer[index++] = b1;
            buffer[index++] = b2;
            buffer[index++] = b3;
            buffer[index++] = b4;
            buffer[index++] = b5;
            buffer[index++] = b6;
        }
    }

    private int byte2Int(byte b) {
        if (b < 0) {
            return 256 + b;
        } else {
            return b;
        }
    }
}
