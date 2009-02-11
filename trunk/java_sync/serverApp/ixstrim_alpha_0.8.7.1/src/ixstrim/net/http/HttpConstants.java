package ixstrim.net.http;

import ixstrim.Version;

/**
 * @author <a href="mailto:yaroslav.malyshev@ngs.ru">Yaroslav Malyshev</a>
 * Date: Apr 8, 2003
 */
public interface HttpConstants {

    public static final String METHOD_POST = "POST";
    public static final String METHOD_GET = "GET";

    public static final String PROTOCOL_HTTP09 = "HTTP/0.9";
    public static final String PROTOCOL_HTTP11 = "HTTP/1.1";
    public static final String PROTOCOL_HTTP10 = "HTTP/1.0";

    public static final String RESPONSE_CODE_100 = "100 Continue";
    public static final String RESPONSE_CODE_101 = "101 Switching Protocols";

    public static final String RESPONSE_CODE_200 = "200 OK";
    public static final String RESPONSE_CODE_201 = "201 Created";
    public static final String RESPONSE_CODE_202 = "202 Accepted";
    public static final String RESPONSE_CODE_203 = "203 Non-Authoritative Information";
    public static final String RESPONSE_CODE_204 = "204 No Content";
    public static final String RESPONSE_CODE_205 = "205 Reset Content";
    public static final String RESPONSE_CODE_206 = "206 Partial Content";

    public static final String RESPONSE_CODE_300 = "300 Multiple Choices";
    public static final String RESPONSE_CODE_301 = "301 Moved Permanently";
    public static final String RESPONSE_CODE_302 = "302 Found";
    public static final String RESPONSE_CODE_303 = "303 See Other";
    public static final String RESPONSE_CODE_304 = "304 Not Modified";
    public static final String RESPONSE_CODE_305 = "305 Use Proxy";
    public static final String RESPONSE_CODE_307 = "307 Temporary Redirect";

    public static final String RESPONSE_CODE_400 = "400 Bad Request";
    public static final String RESPONSE_CODE_401 = "401 Unauthorized";
    public static final String RESPONSE_CODE_402 = "402 Payment Required";
    public static final String RESPONSE_CODE_403 = "403 Forbidden";
    public static final String RESPONSE_CODE_404 = "404 Not Found";
    public static final String RESPONSE_CODE_405 = "405 Method Not Allowed";
    public static final String RESPONSE_CODE_406 = "406 Not Acceptable";
    public static final String RESPONSE_CODE_407 = "407 Proxy Authentication Required";
    public static final String RESPONSE_CODE_408 = "408 Request Time-out";
    public static final String RESPONSE_CODE_409 = "409 Conflict";
    public static final String RESPONSE_CODE_410 = "410 Gone";
    public static final String RESPONSE_CODE_411 = "411 Length Required";
    public static final String RESPONSE_CODE_412 = "412 Precondition Failed";
    public static final String RESPONSE_CODE_413 = "413 Request Entity Too Large";
    public static final String RESPONSE_CODE_414 = "414 Request-URI Too Large";
    public static final String RESPONSE_CODE_415 = "415 Unsupported Media Type";
    public static final String RESPONSE_CODE_416 = "416 Requested range not satisfiable";
    public static final String RESPONSE_CODE_417 = "417 Expectation Failed";

    public static final String RESPONSE_CODE_500 = "500 Internal HttpServer Error";
    public static final String RESPONSE_CODE_501 = "501 Not Implemented";
    public static final String RESPONSE_CODE_502 = "502 Bad Gateway";
    public static final String RESPONSE_CODE_503 = "503 Service Unavailable";
    public static final String RESPONSE_CODE_504 = "504 Gateway Time-out";
    public static final String RESPONSE_CODE_505 = "505 HTTP Version not supported";


    public static final String HEADER_CONTENT_LENGTH = "content-length";
    public static final String HEADER_CONTENT_TYPE = "content-type";
    public static final String HEADER_CONNECTION = "connection";
    public static final String HEADER_TRANSFER_ENCODING = "transfer-encoding";
    public static final String HEADER_LOCATION = "location";
    public static final String HEADER_COOKIE = "cookie";
    public static final String HEADER_SERVER = "server";
    public static final String HEADER_CONTENT_DISPOSITION = "content-disposition";


    public static final String HEADER_VALUE_KEEP_ALIVE = "keep-alive";
    public static final String HEADER_VALUE_CLOSE = "close";
    public static final String HEADER_VALUE_CHUNKED = "chunked";
    public static final String HEADER_VALUE_TEXT_HTML = "text/html";
    public static final String HEADER_VALUE_X_WWW_FORM_URLENCODED = "application/x-www-form-urlencoded";
    public static final String HEADER_VALUE_MULTIPART_FORM_DATA = "multipart/form-data";

    public static final String HEADER_VALUE_SERVER_IXSTRIM = Version.getSignature();

}