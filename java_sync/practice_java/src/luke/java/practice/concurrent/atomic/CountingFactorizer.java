package luke.java.practice.concurrent.atomic;

import java.math.BigInteger;
import java.util.concurrent.atomic.AtomicLong;

import luke.java.practice.concurrent.annotation.ThreadSafe;

@ThreadSafe
public class CountingFactorizer implements Servlet {
	private final AtomicLong count = new AtomicLong(0);

	public long getCount() {
		return count.get();
	}

	public void service(ServletRequest req, ServletResponse resp) {
		BigInteger i = extractFromRequest(req);
		BigInteger[] factors = factor(i);
		count.incrementAndGet();
		encodeIntoResponse(resp, factors);
	}
}
