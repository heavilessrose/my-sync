package org.bouncycastle.crypto.test;

import org.bouncycastle.crypto.engines.CAST5Engine;
import org.bouncycastle.crypto.params.KeyParameter;
import org.bouncycastle.util.encoders.Hex;
import org.bouncycastle.util.test.Test;
import org.bouncycastle.util.test.TestResult;

/**
 * cast tester - vectors from http://www.ietf.org/rfc/rfc2144.txt
 */
public class CAST5Test
    extends CipherTest
{
    static Test[]  tests = {
        new BlockCipherVectorTest(0, new CAST5Engine(),
            new KeyParameter(Hex.decode("0123456712345678234567893456789A")),
            "0123456789ABCDEF", 
            "238B4FE5847E44B2"),
        new BlockCipherVectorTest(0, new CAST5Engine(),
            new KeyParameter(Hex.decode("01234567123456782345")),
            "0123456789ABCDEF", 
            "EB6A711A2C02271B"),
        new BlockCipherVectorTest(0, new CAST5Engine(),
            new KeyParameter(Hex.decode("0123456712")),
            "0123456789ABCDEF", 
            "7Ac816d16E9B302E"),
            };

    CAST5Test()
    {
        super(tests);
    }

    public String getName()
    {
        return "CAST5";
    }

    public static void main(
        String[]    args)
    {
        CAST5Test       test = new CAST5Test();
        TestResult      result = test.perform();

        System.out.println(result);
    }
}
