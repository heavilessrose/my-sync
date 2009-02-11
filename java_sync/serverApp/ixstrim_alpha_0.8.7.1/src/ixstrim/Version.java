package ixstrim;

/**
 * @author <a href="mailto:yaroslav.malyshev@ngs.ru">Yaroslav Malyshev</a>
 * Date: Apr 17, 2003
 */
public class Version {

    private static String version = "alpha 0.8.7.1";
    private static String productName = "IxStrim server";
    private static String signature = productName + " " + version;

    public static String getVersion() {
        return version;
    }

    public static String getProductName() {
        return productName;
    }

    public static String getSignature() {
        return signature;
    }


}
