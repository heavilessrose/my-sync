package luke.java.practice.httpServer;

import java.io.IOException;
import java.net.InetSocketAddress;
import java.nio.channels.SelectionKey;
import java.nio.channels.Selector;
import java.nio.channels.spi.SelectorProvider;
import java.nio.channels.ServerSocketChannel;
import java.nio.channels.SocketChannel;
import java.util.Iterator;
import java.util.Set;
import java.util.concurrent.*;

public class MinioHttpd extends MinioCommon {
	// Global Constants - Default Port
	public static final int DEFAULT_PORT = 8080;
	public static final int MAX_REACTOR = 20;

	private int port;
	private Selector acceptSelector;
	private MinioReactor[] reactors;
	private int index;
	private ServerSocketChannel serverChannel;
	private ThreadPoolExecutor reactorPool;
	private ThreadPoolExecutor processPool;

	// Default Contructor
	public MinioHttpd() {
		port = DEFAULT_PORT;
		index = 0;
		reactors = new MinioReactor[MAX_REACTOR];
	}

	// Contructor with prefered port
	public MinioHttpd(int port_) {
		this();
		port = port_;
	}

	// return a new Handler object [Overidable]
	protected MinioHandler newHandler(ThreadPoolExecutor pool_,
			SocketChannel channel_) throws IOException {
		return new MinioHandler(pool_, channel_);
	}

	// return a new Selector object
	protected Selector newSelector() throws IOException {
		return SelectorProvider.provider().openSelector();
	}

	// return a new ThreadPoolExecutor
	protected ThreadPoolExecutor newReactorThreadPoolExecutor() {
		return new ThreadPoolExecutor(MAX_REACTOR, MAX_REACTOR, 50000L,
				TimeUnit.MILLISECONDS, new LinkedBlockingQueue<Runnable>());
	}

	// return a new ThreadPoolExecutor
	protected ThreadPoolExecutor newProcessThreadPoolExecutor() {
		return new ThreadPoolExecutor(THREAD_POOL_SIZE, THREAD_POOL_SIZE,
				50000L, TimeUnit.MILLISECONDS,
				new LinkedBlockingQueue<Runnable>());
	}

	// Destructor
	public void finalize() throws IOException {
		log(" disposing...");
		serverChannel.close();
		// selector.close();
	}

	// start up service
	public void start() throws IOException {
		log(NOTE, "starting...");
		acceptSelector = newSelector();
		serverChannel = ServerSocketChannel.open();
		serverChannel.configureBlocking(false);
		// serverChannel.socket().setReuseAddress( true );
		serverChannel.socket().bind(new InetSocketAddress(port));
		SelectionKey sk = serverChannel.register(acceptSelector,
				SelectionKey.OP_ACCEPT);
		sk.attach(new MinioAcceptor());
		processPool = newProcessThreadPoolExecutor();
		Thread t = new Thread(new MinioReactor(acceptSelector));
		t.setDaemon(false);
		t.start();
		startReactorPool();
		log(NOTE, "binded");
	}

	private void startReactorPool() throws IOException {
		reactorPool = newReactorThreadPoolExecutor();
		int i;
		for (i = 0; i < MAX_REACTOR; i++) {
			reactors[i] = new MinioReactor(newSelector());
			reactorPool.execute(reactors[i]);
		}
		log(i + " reactors started");
	}

	// Minio Reactor
	public class MinioReactor implements Runnable {
		private Selector activeSelector;
		private ConcurrentLinkedQueue<MinioHandler> registrants;

		public MinioReactor(Selector selector_) {
			activeSelector = selector_;
			registrants = new ConcurrentLinkedQueue<MinioHandler>();
		}

		public void register(MinioHandler handler) {
			registrants.add(handler);
			activeSelector.wakeup();
		}

		public void run() {
			SelectionKey key = null;
			MinioHandler handler = null;
			try {
				while (!Thread.interrupted()) {
					log(100, activeSelector + " wait for select");
					int n = activeSelector.select(); // blocking here
					while (null != (handler = registrants.poll())) {
						key = handler.getChannel().register(activeSelector,
								SelectionKey.OP_READ, handler);
						handler.assignKey(key);
						log(101, "registered " + key);
					}
					log(100, activeSelector + "selected " + n + " key(s)");
					if (n <= 0) {
						log(101, " select 0 !!!");
						continue;
					}
					Set selected = activeSelector.selectedKeys();
					Iterator i = selected.iterator();
					while (i.hasNext()) {
						key = (SelectionKey) i.next();
						i.remove();
						dispatch(key);
						key = null;
					}
					selected.clear();
				}
			} catch (Exception e) {
				log(ERRO, e.getMessage());
			}
		}

		private void dispatch(SelectionKey sk) {
			Runnable r = (Runnable) sk.attachment();
			if (r != null)
				r.run();
		}
	}

	// Minio Channel Acceptor
	private class MinioAcceptor implements Runnable {
		public void run() {
			try {
				SocketChannel c = serverChannel.accept();
				if (c != null) {
					log(SPEC, "Socket accepted - "
							+ c.socket().getInetAddress().getHostAddress());
					reactors[index].register(newHandler(processPool, c));
				}
				index = (index + 1) % MAX_REACTOR;
			} catch (Exception e) {
				log(ERRO, e.getMessage());
			}
		}
	}

	public static void main(String[] args) {

		System.out.println("MinioHttpd 1.0 (C) 2007 Hei Chan");
		System.out.println("(command line option: [-p<port>] [-v])");

		MinioHttpd miniohttpd = new MinioHttpd(8081);
		miniohttpd.setLogLevel(200);

		try {
			miniohttpd.start();
		} catch (IOException e) {
			log(ERRO, e.getMessage());
		}

	}

}
