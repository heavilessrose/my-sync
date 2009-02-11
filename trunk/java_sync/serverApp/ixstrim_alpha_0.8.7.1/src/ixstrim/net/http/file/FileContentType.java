package ixstrim.net.http.file;

import java.util.HashMap;

/**
 * @author <a href="mailto:yaroslav.malyshev@ngs.ru">Yaroslav Malyshev</a>
 * Date: Apr 3, 2003
 */
public class FileContentType {

    private static HashMap map = new HashMap();

    static {
        map.put(".aif", "audio/x-aiff");
        map.put(".aiff", "audio/x-aiff");
        map.put(".aifc", "audio/x-aiff");
        map.put(".ai", "application/postscript");
        map.put(".au", "audio/basic");
        map.put(".asc", "text/plain");
        map.put(".asf", "video/x-ms-asf");
        map.put(".asx", "video/x-ms-asf");
        map.put(".avi", "video/x-msvideo");
        map.put(".bin", "application/octet-stream");
        map.put(".bcpio", "application/x-bcpio");
        map.put(".bmp", "image/bmp");
        map.put(".class", "application/octet-stream");
        map.put(".cpt", "application/mac-compactpro");
        map.put(".css", "text/css");
        map.put(".cpio", "application/x-cpio");
        map.put(".csh", "application/x-csh");
        map.put(".cdf", "application/x-netcdf");
        map.put(".dms", "application/octet-stream");
        map.put(".doc", "application/msword");
        map.put(".dcr", "application/x-director");
        map.put(".dir", "application/x-director");
        map.put(".dxr", "application/x-director");
        map.put(".dvi", "application/x-dvi");
        map.put(".exe", "application/octet-stream");
        map.put(".eps", "application/postscript");
        map.put(".etx", "text/x-setext");
        map.put(".gtar", "application/x-gtar");
        map.put(".gif", "image/gif");
        map.put(".gz", "application/octet-stream");
        map.put(".hdml", "text/x-hdml");
        map.put(".hqx", "application/mac-binhex40");
        map.put(".html", "text/html");
        map.put(".htm", "text/html");
        map.put(".hdf", "application/x-hdf");
        map.put(".ief", "image/ief");
        map.put(".ice", "x-conference/x-cooltalk");
        map.put(".java", "text/plain");
        map.put(".jav", "text/plain");
        map.put(".js", "application/x-javascript");
        map.put(".jpeg", "image/jpeg");
        map.put(".jpg", "image/jpeg");
        map.put(".jpe", "image/jpeg");
        map.put(".kar", "audio/midi");
        map.put(".latex", "application/x-latex");
        map.put(".lha", "application/octet-stream");
        map.put(".lhz", "application/octet-stream");
        map.put(".mid", "audio/midi");
        map.put(".mpeg", "video/mpeg");
        map.put(".mpg", "video/mpeg");
        map.put(".mpe", "video/mpeg");
        map.put(".mov", "video/quicktime");
        map.put(".movie", "video/x-sgi-movie");
        map.put(".mpga", "audio/mpeg");
        map.put(".mp2", "audio/mpeg");
        map.put(".mp3", "audio/mpeg");
        map.put(".man", "application/x-troff-man");
        map.put(".me", "application/x-troff-me");
        map.put(".ms", "application/x-troff-ms");
        map.put(".nc", "application/x-netcdf");
        map.put(".oda", "application/oda");
        map.put(".pdf", "application/pdf");
        map.put(".ps", "application/postscript");
        map.put(".ppt", "application/vnd.ms-powerpoint");
        map.put(".png", "image/png");
        map.put(".pgn", "application/x-chess-pgn");
        map.put(".pnm", "image/x-portable-anymap");
        map.put(".pbm", "image/x-portable-bitmap");
        map.put(".pgm", "image/x-portable-graymap");
        map.put(".ppm", "image/x-portable-pixmap");
        map.put(".qt", "video/quicktime");
        map.put(".rtf", "application/rtf");
        map.put(".ram", "audio/x-pn-realaudio");
        map.put(".rm", "audio/x-pn-realaudio");
        map.put(".rpm", "audio/x-pn-realaudio-plugin");
        map.put(".ra", "audio/x-realaudio");
        map.put(".ras", "image/x-cmu-raster");
        map.put(".rgb", "image/x-rgb");
        map.put(".rtx", "text/richtext");
        map.put(".rtf", "text/rtf");
        map.put(".smi", "application/smil");
        map.put(".smil", "application/smil");
        map.put(".sml", "application/smil");
        map.put(".skp", "application/x-koan");
        map.put(".skd", "application/x-koan");
        map.put(".skt", "application/x-koan");
        map.put(".skm", "application/x-koan");
        map.put(".src", "application/x-wais-source");
        map.put(".sh", "application/x-sh");
        map.put(".shar", "application/x-shar");
        map.put(".swf", "application/x-shockwave-flash");
        map.put(".sit", "application/x-stuffit");
        map.put(".spl", "application/x-futuresplash");
        map.put(".sv4cpio", "application/x-sv4cpio");
        map.put(".sv4crc", "application/x-sv4crc");
        map.put(".snd", "audio/basic");
        map.put(".sgml", "text/sgml");
        map.put(".sgm", "text/sgml");
        map.put(".tgz", "application/octet-stream");
        map.put(".tar", "application/x-tar");
        map.put(".tcl", "application/x-tcl");
        map.put(".tex", "application/x-tex");
        map.put(".texinfo", "application/x-texinfo");
        map.put(".texi", "application/x-texinfo");
        map.put(".t", "application/x-troff");
        map.put(".tr", "application/x-troff");
        map.put(".roff", "application/x-troff");
        map.put(".tiff", "image/tiff");
        map.put(".tif", "image/tiff");
        map.put(".txt", "text/plain");
        map.put(".tsv", "text/tab-separated-values");
        map.put(".ustar", "application/x-ustar");
        map.put(".vcd", "application/x-cdlink");
        map.put(".vrml", "model/vrml");
        map.put(".wav", "audio/x-wav");
        map.put(".wax", "audio/x-ms-wax");
        map.put(".wrl", "model/vrml");
        map.put(".wma", "audio/x-ms-wma");
        map.put(".wml", "text/vnd.wap.wml");
        map.put(".wmls", "text/vnd.wap.wmlscript");
        map.put(".wmlc", "application/vnd.wap.wmlc");
        map.put(".wmlsc", "application/vnd.wap.wmlscript");
        map.put(".wbmp", "image/vnd.wap.wbmp");
        map.put(".xls", "application/vnd.ms-excel");
        map.put(".xbm", "image/x-xbitmap");
        map.put(".xpm", "image/x-xpixmax");
        map.put(".xwd", "image/x-xwindowdump");
        map.put(".xml", "text/xml");
        map.put(".zip", "application/zip");
        map.put(".z", "application/octet-stream");
    }


    public static String getContentType(String filename) {
        int i = filename.lastIndexOf('.');
        if(i != -1) {
            String ext = filename.substring(i).toLowerCase();
            String contentType = (String)map.get(ext);
            if(contentType != null) return contentType;
        }
        return "application/octet-stream";
    }
}
