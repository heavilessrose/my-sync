package java.math.test;

import java.math.BigInteger;

import org.bouncycastle.util.test.*;


public class BigIntegerTest
    implements Test
{
    public String getName()
    {
        return "BigInteger";
    }

    public TestResult perform()
    {
	    BigInteger n1, n2, r1;
	
	    // test division where the difference in bit length of the dividend and divisor is 32 bits 
	    n1 = new BigInteger("54975581388");
	    n2 = new BigInteger("10");
	    r1 = n1.divide(n2);
	    if (!r1.toString(10).equals("5497558138"))
	    {
	        return new SimpleTestResult(false, "BigInteger: failed Divide Test");
	    }
	    
		return new SimpleTestResult(true, "BigInteger: Okay");
    }

    public static void main(
        String[]    args)
    {
        BigIntegerTest         test = new BigIntegerTest();
        TestResult      result = test.perform();

        System.out.println(result);
    }
}

