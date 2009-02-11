package se.rupy.http;

import java.io.*;
import java.net.*;
import java.text.*;
import java.util.*;

import java.nio.channels.*;

/**
 * A tiny HTTP daemon. The whole server is non-static so that you can launch
 * multiple contained HTTP servers in one application on different ports.
 * 
 * @author marc
 */

public class Daemon implements Runnable {
	public Properties properties;
	public boolean verbose, debug, host;

	int threads, timeout, cookie, delay, size, port;

	private HashMap archive, service, session;
	private Chain workers, queue;
	private Selector selector;
	private String pass;

	/**
	 * Use this to start the daemon from your application. The parameters below
	 * should be in the properties argument.
	 * 
	 * @param host (false)
	 *            if you want to host multiple domains on one rupy server you need
	 *            to enable hosts, and add host attributes to your deploy jar 
	 *            manifest files.
	 * @param pass
	 *            the pass used to deploy services via HTTP POST or null/"" to
	 *            disable remote hot-deploy
	 * @param port (8000)
	 *            which TCP port
	 * @param threads (5)
	 *            how many worker threads, the daemon also starts one selector
	 *            and one heartbeat thread.
	 * @param timeout (5 minutes)
	 *            session timeout in seconds or 0 to disable sessions
	 * @param cookie (4 characters)
	 *            session key length; default and minimum is 4, > 10 can be
	 *            considered secure
	 * @param delay (5 seconds)
	 *            time in seconds before started event gets dropped due to
	 *            inactivity. Increase this if your users will download 
	 *            content with a 'open/save/cancel' + save location dialog, 
	 *            since this will timeout otherwise.
	 * @param size (1024 bytes)
	 *            IO buffer size, should be proportional to the data sizes
	 *            received/sent by the server currently this is input/output-
	 *            buffer, chunk-buffer, post-body-max and header-max lengths! :P
	 * @param verbose
	 */
	public Daemon(Properties properties) {
		this.properties = properties;

		threads = Integer.parseInt(properties.getProperty("threads", "5"));
		cookie = Integer.parseInt(properties.getProperty("cookie", "4"));
		port = Integer.parseInt(properties.getProperty("port", "8000"));
		timeout = Integer.parseInt(properties.getProperty("timeout", "300")) * 1000;
		delay = Integer.parseInt(properties.getProperty("delay", "5")) * 1000;
		size = Integer.parseInt(properties.getProperty("size", "1024"));

		verbose = properties.getProperty("verbose", "false").toLowerCase()
		.equals("true");
		debug = properties.getProperty("debug", "false").toLowerCase().equals(
		"true");
		host = properties.getProperty("host", "false").toLowerCase().equals(
		"true");

		if (!verbose) {
			debug = false;
		}

		archive = new HashMap();
		service = new HashMap();
		session = new HashMap();

		workers = new Chain();
		queue = new Chain();

		try {
			new Heart();

			int threads = Integer.parseInt(properties.getProperty("threads",
			"5"));

			for (int i = 0; i < threads; i++) {
				workers.add(new Worker(this, i));
			}

			new Thread(this).start();
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	HashMap session() {
		return session;
	}

	Selector selector() {
		return selector;
	}

	void chain(Deploy.Archive archive) throws Exception {
		Deploy.Archive old = (Deploy.Archive) this.archive.get(archive.name());

		if (old != null) {
			Iterator it = old.service().iterator();

			while (it.hasNext()) {
				Service service = (Service) it.next();

				try {
					service.destroy();
				} catch (Exception e) {
					e.printStackTrace();
				}
			}
		}

		Iterator it = archive.service().iterator();

		while (it.hasNext()) {
			Service service = (Service) it.next();
			add(archive.chain(), service);
		}

		this.archive.put(archive.name(), archive);
	}

	public void add(Service service) throws Exception {
		add(this.service, service);
	}

	void add(HashMap map, Service service) throws Exception {
		StringTokenizer paths = new StringTokenizer(service.path(), ":");

		while (paths.hasMoreTokens()) {
			String path = paths.nextToken();
			Chain chain = (Chain) map.get(path);

			if (chain == null) {
				chain = new Chain();
				map.put(path, chain);
			}

			Service old = (Service) chain.put(service);

			if (old != null) {
				throw new Exception(service.getClass().getName()
						+ " with path '" + path + "' and index ["
						+ service.index() + "] is conflicting with "
						+ old.getClass().getName()
						+ " for the same path and index.");
			}

			if (verbose)
				System.out.println(path + padding(path) + chain);

			try {
				service.create();
			} catch (Exception e) {
				e.printStackTrace();
			}
		}
	}

	String padding(String path) {
		StringBuffer buffer = new StringBuffer();
		
		for(int i = 0; i < 10 - path.length(); i++) {
			buffer.append(' ');
		}
		
		return buffer.toString();
	}
	
	void verify(Deploy.Archive archive) throws Exception {
		Iterator it = archive.chain().keySet().iterator();

		while (it.hasNext()) {
			String path = (String) it.next();
			Chain chain = (Chain) archive.chain().get(path);

			for (int i = 0; i < chain.size(); i++) {
				Service service = (Service) chain.get(i);

				if (i != service.index()) {
					this.archive.remove(archive.name());
					throw new Exception(service.getClass().getName()
							+ " with path '" + path + "' has index ["
							+ service.index() + "] which is too high.");
				}
			}
		}
	}

	Deploy.Stream content(Query query) {
		if(host) {
			return content(query.header("host"), query.path());
		}
		else {
			return content(query.path());
		}
	}
	
	Deploy.Stream content(String path) {
		return content("content", path);
	}
		
	Deploy.Stream content(String host, String path) {
		File file = new File("app" + File.separator + host + File.separator + path);

		if(file.exists() && !file.isDirectory()) {
			return new Deploy.Big(file);
		}
		
		/*
		synchronized (this.archive) {
			Iterator it = this.archive.values().iterator();

			while (it.hasNext()) {
				Deploy.Archive archive = (Deploy.Archive) it.next();
				Deploy.Stream stream = (Deploy.Stream) archive.content().get(
						path);
				
				if (stream != null) {
					return stream;
				}
			}
		}
		*/
		
		return null;
	}

	Chain chain(Query query) {
		if(host) {
			return chain(query.header("host"), query.path());
		}
		else {
			return chain(query.path());
		}
	}
	
	public Chain chain(String path) {
		return chain("content", path);
	}
	
	public Chain chain(String host, String path) {
		synchronized (this.service) {
			Chain chain = (Chain) this.service.get(path);

			if (chain != null) {
				return chain;
			}
		}

		synchronized (this.archive) {
			Iterator it = this.archive.values().iterator();

			while (it.hasNext()) {
				Deploy.Archive archive = (Deploy.Archive) it.next();
				
				if (archive.host().equals(host)) {
					Chain chain = (Chain) archive.chain().get(path);

					if (chain != null) {
						return chain;
					}
				}
			}
		}

		return null;
	}

	synchronized Event next(Worker worker) {
		synchronized (this.queue) {
			if (queue.size() > 0) {
				if (debug)
					System.out.println("worker " + worker.index()
							+ " found work " + queue);

				return (Event) queue.remove(0);
			}
		}
		return null;
	}

	public void run() {
		String pass = properties.getProperty("pass", "");

		try {
			selector = Selector.open();
			ServerSocketChannel server = ServerSocketChannel.open();
			server.socket().bind(new InetSocketAddress(port));
			server.configureBlocking(false);
			server.register(selector, SelectionKey.OP_ACCEPT);

			DecimalFormat decimal = (DecimalFormat) DecimalFormat.getInstance();
			decimal.applyPattern("#.##");

			if (verbose)
				System.out.println("daemon started\n" + "- pass       \t"
						+ pass + "\n" + "- port       \t" + port + "\n"
						+ "- worker(s)  \t" + threads + " thread"
						+ (threads > 1 ? "s" : "") + "\n" + "- session    \t"
						+ cookie + " characters\n" + "- timeout    \t"
						+ decimal.format((double) timeout / 60000) + " minute"
						+ (timeout / 60000 > 1 ? "s" : "") + "\n"
						+ "- IO timeout \t" + delay / 1000 + " second"
						+ (delay / 1000 > 1 ? "s" : "") + "\n"
						+ "- IO buffer  \t" + size + " bytes\n"
						+ "- debug      \t" + debug);

			if (pass != null && pass.length() > 0) {
				add(new Deploy("app" + File.separator, pass));

				File[] app = new File(Deploy.path).listFiles(new Filter());

				if (app != null) {
					for (int i = 0; i < app.length; i++) {
						Deploy.deploy(this, app[i]);
					}
				}
			}

			if (properties.getProperty("test", "false").toLowerCase().equals(
			"true"))
				test();
		} catch (Exception e) {
			e.printStackTrace();
			System.exit(1);
		}

		int index = 0;
		Event event = null;
		SelectionKey key = null;

		while (true) {
			try {
				selector.select();
				Iterator it = selector.selectedKeys().iterator();

				while (it.hasNext()) {
					key = (SelectionKey) it.next();
					it.remove();

					if (key.isValid()) {
						if (key.isAcceptable()) {
							// TODO: Event pool?
							event = new Event(this, key, index++);
							event.log("accept ---");
						} else if (key.isReadable() || key.isWritable()) {
							key.interestOps(0);

							event = (Event) key.attachment();
							Worker worker = event.worker();

							if (debug) {
								if (key.isReadable())
									event.log("read ---");
								if (key.isWritable())
									event.log("write ---");
							}

							if (worker == null) {
								employ(event);
							} else {
								worker.wakeup();
							}
						}
					}
				}
			} catch (Exception e) {
				/*
				 * Here we get mostly ClosedChannelExceptions and
				 * java.io.IOException: 'Too many open files' when the server is
				 * taking a beating. Better to drop connections than to drop the
				 * server.
				 */
				event.disconnect(e);
			}
		}
	}

	void queue(Event event) {
		synchronized (this.queue) {
			queue.add(event);
		}
		
		if (debug)
			System.out.println("queue " + queue.size());
	}
	
	synchronized void employ(Event event) {
		if(queue.size() > 0) {
			queue(event);
			return;
		}
		
		workers.reset();
		Worker worker = (Worker) workers.next();

		if (worker == null) {
			queue(event);
			return;
		}

		while (worker.busy()) {
			worker = (Worker) workers.next();

			if (worker == null) {
				queue(event);
				return;
			}
		}

		if (debug)
			System.out.println("worker " + worker.index() + " hired. (" + queue.size() + ")");

		event.worker(worker);
		worker.event(event);
		worker.wakeup();
	}

	class Filter implements FilenameFilter {
		public boolean accept(File dir, String name) {
			if (name.endsWith(".jar")) {
				return true;
			}

			return false;
		}
	}

	class Heart implements Runnable {
		Heart() {
			new Thread(this).start();
		}

		public void run() {
			while (true) {
				try {
					Thread.sleep(1000);

					synchronized (session) {
						Iterator it = session.values().iterator();

						while (it.hasNext()) {
							Session se = (Session) it.next();

							if (System.currentTimeMillis() - se.date() > timeout) {
								it.remove();
								se.remove();

								if (debug)
									System.out.println("session timeout "
											+ se.key());
							}
						}
					}
				} catch (Exception e) {
					e.printStackTrace();
				}
			}
		}
	}

	public static void main(String[] args) {
		Properties properties = new Properties();

		for (int i = 0; i < args.length; i++) {
			String flag = args[i];
			String value = null;

			if (flag.startsWith("-") && ++i < args.length) {
				value = args[i];

				if (value.startsWith("-")) {
					i--;
					value = null;
				}
			}

			if (value == null) {
				properties.put(flag.substring(1).toLowerCase(), "true");
			} else {
				properties.put(flag.substring(1).toLowerCase(), value);
			}
		}

		if (properties.getProperty("help", "false").toLowerCase()
				.equals("true")) {
			System.out.println("Usage: java -jar http.jar -verbose");
			return;
		}

		new Daemon(properties);
	}

	/*
	 * Test cases are performed in parallel with one worker thread, in order to
	 * detect synchronous errors.
	 */
	void test() throws Exception {
		System.out.println("Parallel testing begins in one second:");
		System.out.println("- OP_READ, OP_WRITE and selector wakeup.");
		System.out.println("- Asynchronous non-blocking reply.");
		System.out.println("- Session creation and timeout.");
		System.out.println("- Exception handling.");
		System.out.println("Estimated duration: ~2 sec.");
		System.out.println("             ---o---");

		try {
			Thread.sleep(1000);
		} catch (InterruptedException e) {
		}

		add(new Test.Service("/io"));
		add(new Test.Service("/async"));
		add(new Test.Service("/error"));

		new Thread(new Test("localhost:" + port + "/io",
				new File(Test.original))).start();
		new Thread(new Test("localhost:" + port + "/async", null)).start();
		new Thread(new Test("localhost:" + port + "/error", null)).start();
	}
}
