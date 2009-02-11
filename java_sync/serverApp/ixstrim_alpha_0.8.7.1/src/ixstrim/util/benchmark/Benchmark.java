package ixstrim.util.benchmark;

import java.io.IOException;
import java.net.InetSocketAddress;
import java.net.URL;
import java.nio.ByteBuffer;
import java.nio.channels.SelectionKey;
import java.nio.channels.Selector;
import java.nio.channels.SocketChannel;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.Set;

/**
 * @author <a href="mailto:yaroslav.malyshev@ngs.ru">Yaroslav Malyshev</a>
 * Date: Apr 8, 2003
 */
public class Benchmark implements Runnable {

    private static int[] responsesCount;
    private static long[] netErrors;
    private static long startTime;
    private static long finishTime;
    private static int threads = 1;
    private static int clients = 10;
    private static long maxTime = 10000;

    private byte[] request;
    private String host;
    private int port;
    private int connectionsCount;
    private Selector selector;
    private int number;


    public static void main(String args[]) throws Exception {
        if(args.length == 0) {
            System.err.println("Use pasametrs : <URL> [<clients> [<time>]]");
            System.exit(0);
        }
        URL url = new URL(args[0]);
        String host = url.getHost();
        int port = url.getPort();
        if(port <= 0) port = url.getDefaultPort();
        String uri = url.getPath() + (url.getQuery() == null ? "" : "?" + url.getQuery());
        try {
            clients = Integer.parseInt(args[1]);
        } catch(Exception e) {
        }
        try {
            maxTime = Integer.parseInt(args[2]) * 1000;
        } catch(Exception e) {
        }

        if(clients < threads) threads = clients;

        System.err.println("\nTesting URL - http://" + host + ":" + port + uri + " Time : " + maxTime / 1000 + " sec");
        System.err.print("Connecting...");
        ArrayList list = new ArrayList();
        responsesCount = new int[threads];
        netErrors = new long[threads];
        int clientsSum = 0;
        for(int i = 0; i < responsesCount.length; i++) {
            int mClients = clients / threads;
            if(i == responsesCount.length - 1) mClients = clients - mClients * (threads - 1);
            clientsSum += mClients;
            Benchmark m = new Benchmark(host, port, mClients,
                    "GET " + uri + " HTTP/1.1\r\n" +
                    "Connection:keep-alive\r\n" +
                    "Host:" + host + ":" + port + "\r\n\r\n", i);
            m.connect();
            list.add(m);
        }
        System.gc();
        System.err.println(clientsSum + " clients started in " + threads + " threads");
        startTime = System.currentTimeMillis();
        finishTime = startTime + maxTime;
        for(int i = 0; i < list.size(); i++) {
            new Thread((Benchmark)list.get(i)).start();
        }
    }

    public static synchronized void finished() {
        if(startTime == 0) return;
        long workTime = System.currentTimeMillis() - startTime;
        long sum = 0;
        for(int i = 0; i < responsesCount.length; i++) sum += responsesCount[i];
        long errSum = 0;
        for(int i = 0; i < netErrors.length; i++) errSum += netErrors[i];
        System.out.println("Clients:" + clients + " RPS:" + (sum * 1000 / workTime) + " Requests:" + sum + " Errors:" + errSum);
        System.err.println("Clients:" + clients + " RPS:" + (sum * 1000 / workTime) + " Requests:" + sum + " Errors:" + errSum);
        startTime = 0;
    }

    public Benchmark(String host, int port, int connectionsCount, String request, int number) {
        this.request = request.getBytes();
        this.host = host;
        this.port = port;
        this.connectionsCount = connectionsCount;
        this.number = number;
    }


    public void connect() throws IOException {
        selector = Selector.open();
        for(int i = 0; i < connectionsCount;) {
            try {
                SocketChannel ch = SocketChannel.open(new InetSocketAddress(host, port));
                ch.configureBlocking(false);
                SelectionKey key = ch.register(selector, SelectionKey.OP_READ);
                key.attach(new Filter(number,responsesCount));
                ch.write(ByteBuffer.wrap(Benchmark.this.request));
                i++;
                if(i % 20 == 0) System.err.print(".");
            } catch(IOException e) {
                System.err.print("*");
            }
        }
    }

    public void run() {
        try {
            while(true) {
                if(System.currentTimeMillis() > finishTime) break;
                selector.select(100);
                Set set = selector.selectedKeys();
                if(set.size() == 0){
                    try {
                        Thread.sleep(5);
                    } catch(InterruptedException e) {}
                    continue;
                }
                Iterator iterator = set.iterator();
                while(iterator.hasNext()) {
                    SelectionKey key = (SelectionKey)iterator.next();
                    try {
                        if(key.isReadable()) {
                            Filter f = (Filter)key.attachment();
                            SocketChannel ch = (SocketChannel)key.channel();
                            long oldCount = responsesCount[number];
                            try {
                                while(f.filter(ch)) ;
                                if(oldCount != responsesCount[number]) ch.write(ByteBuffer.wrap(Benchmark.this.request));
                            } catch(IOException e) {
                                netErrors[number]++;
                                System.err.println("Net err (" + e.getMessage() + ")");
                                ch.close();
                            }
                        }
                    } catch(Exception e) {
                        e.printStackTrace();
                    }
                }
                set.clear();
            }
            finished();
            Set set = selector.keys();
            Iterator iterator = set.iterator();
            while(iterator.hasNext()) {
                SelectionKey key = (SelectionKey)iterator.next();
                SocketChannel ch = (SocketChannel)key.channel();
                ch.close();
            }
        } catch(IOException e) {
            e.printStackTrace();
        }
    }
}