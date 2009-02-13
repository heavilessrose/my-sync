package org.bouncycastle.util.test;

public class SimpleTestResult
    implements TestResult
{
    private boolean   success;
    private String    message;
    private Exception exception;

    public SimpleTestResult(
        boolean success,
        String  message)
    {
        this.success = success;
        this.message = message;
    }

    public SimpleTestResult(
        boolean     success,
        String          message,
        Exception    exception)
    {
        this.success = success;
        this.message = message;
        this.exception = exception;
    }
    
    public boolean isSuccessful()
    {
        return success;
    }

    public String toString()
    {
        return message;
    }
    
    public Exception getException()
    {
        return exception;
    }
}
