package org.bouncycastle.crypto.test;

import org.bouncycastle.crypto.Digest;
import org.bouncycastle.crypto.digests.RIPEMD320Digest;
import org.bouncycastle.util.encoders.Hex;
import org.bouncycastle.util.test.SimpleTestResult;
import org.bouncycastle.util.test.Test;
import org.bouncycastle.util.test.TestResult;

/**
 * RIPEMD320 Digest Test
 */
public class RIPEMD320DigestTest
    implements Test
{
    final static String[] messages = {
        "",
        "a",
        "abc",
        "message digest",
        "abcdefghijklmnopqrstuvwxyz",
        "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq",
        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789",
        "12345678901234567890123456789012345678901234567890123456789012345678901234567890"
    };

    final static String[] digests = {
        "22d65d5661536cdc75c1fdf5c6de7b41b9f27325ebc61e8557177d705a0ec880151c3a32a00899b8",
        "ce78850638f92658a5a585097579926dda667a5716562cfcf6fbe77f63542f99b04705d6970dff5d",
        "de4c01b3054f8930a79d09ae738e92301e5a17085beffdc1b8d116713e74f82fa942d64cdbc4682d",
        "3a8e28502ed45d422f68844f9dd316e7b98533fa3f2a91d29f84d425c88d6b4eff727df66a7c0197",
        "cabdb1810b92470a2093aa6bce05952c28348cf43ff60841975166bb40ed234004b8824463e6b009",
        "d034a7950cf722021ba4b84df769a5de2060e259df4c9bb4a4268c0e935bbc7470a969c9d072a1ac",
        "ed544940c86d67f250d232c30b7b3e5770e0c60c8cb9a4cafe3b11388af9920e1b99230b843c86a4",
        "557888af5f6d8ed62ab66945c6d2a0a47ecd5341e915eb8fea1d0524955f825dc717e4a008ab2d42"
    };

    final static String million_a_digest = "bdee37f4371e20646b8b0d862dda16292ae36f40965e8c8509e63d1dbddecc503e2b63eb9245bb66";

    public static boolean arraysEqual(byte[] a, byte[] b)
    {
        if (a == b) return true;
        if (a.length != b.length) return false;

        for (int i = 0; i < a.length; i++)
        {
            if (a[i] != b[i]) return false;
        }

        return true;
    }
        
    public String getName()
    {
        return "RIPEMD320";
    }

    public TestResult perform()
    {
        Digest digest = new RIPEMD320Digest();
        byte[] resBuf = new byte[digest.getDigestSize()];

        for (int i = 0; i < messages.length; i++)
        {
            byte[] m = messages[i].getBytes();
            digest.update(m, 0, m.length);
            digest.doFinal(resBuf, 0);

            if (!arraysEqual(resBuf, Hex.decode(digests[i])))
            {
                return new SimpleTestResult(false, getName() + ": Vector " + i + " failed");
            }
        }

        //
        // test 2
        //
        byte[] m = messages[messages.length-1].getBytes();

        digest.update(m, 0, m.length/2);

        // clone the Digest
        Digest d = new RIPEMD320Digest((RIPEMD320Digest)digest);

        digest.update(m, m.length/2, m.length - m.length/2);
        digest.doFinal(resBuf, 0);

        if (!arraysEqual(resBuf, Hex.decode(digests[digests.length-1])))
        {
            return new SimpleTestResult(false,
                "RIPEMD320 failing clone test"
                + System.getProperty("line.separator")
                + "    expected: " + digests[digests.length-1]
                + System.getProperty("line.separator")
                + "    got     : " + new String(Hex.encode(resBuf)));
        }

        d.update(m, m.length/2, m.length - m.length/2);
        d.doFinal(resBuf, 0);

        if (!arraysEqual(resBuf, Hex.decode(digests[digests.length-1])))
        {
            return new SimpleTestResult(false,
                "RIPEMD320 failing clone test - part 2"
                + System.getProperty("line.separator")
                + "    expected: " +  digests[digests.length-1]
                + System.getProperty("line.separator")
                + "    got     : " + new String(Hex.encode(resBuf)));
        }

        for (int i = 0; i < 1000000; i++)
        {
            digest.update((byte)'a');
        }
        digest.doFinal(resBuf, 0);

        if (!arraysEqual(resBuf, Hex.decode(million_a_digest)))
        {
            return new SimpleTestResult(false, getName() + ": Million a's failed");
        }

        return new SimpleTestResult(true, getName() + ": Okay");
    }

    public static void main(
        String[]    args)
    {
        RIPEMD320DigestTest test = new RIPEMD320DigestTest();
        TestResult          result = test.perform();

        System.out.println(result);
    }
}
