package ixstrim.net.http.file;

import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.RandomAccessFile;
import java.io.File;

import ixstrim.event.ServerErrorEvent;
import ixstrim.event.EventDispatcher;
import ixstrim.context.ApplicationContext;
import ixstrim.util.cache.Cache;
import ixstrim.util.cache.ExpirableFIFOCache;


/**
 * @author <a href="mailto:yaroslav.malyshev@ngs.ru">Yaroslav Malyshev</a>
 * Date: Apr 3, 2003
 */
public class ChunkedFileManager {

    private static final Cache cache = new ExpirableFIFOCache(100, 1000);

    private EventDispatcher eventDispatcher
            = ApplicationContext.getDefaultApplicationContext().getEventDispatcher();


    private static ChunkedFileManager instance;

    private int chunkSize;

    public ChunkedFileManager(int chunkSize) {
        this.chunkSize = chunkSize;
    }

    public int getChunkSize() {
        return chunkSize;
    }

    public byte[] getChunk(File filename, int chunkNumber) {
        ChunkKey key = new ChunkKey(filename, chunkNumber);
        byte[] chunk = (byte[])cache.get(key);
        if(chunk == null) {
            byte[] bb = loadChunk(filename, chunkNumber);
            if(bb != null) {
                cache.put(key, bb);
                return bb;
            } else {
                return null;
            }
        } else {
            return chunk;
        }
    }

    public static ChunkedFileManager getFileManager() {
        if(instance == null) {
            synchronized(ChunkedFileManager.class) {
                if(instance == null) {
                    int blockSize = 4096;
                    instance = new ChunkedFileManager(blockSize);
                }
            }
        }
        return instance;
    }

    private byte[] loadChunk(File filename, int chunkNumber) {
        try {
            RandomAccessFile raf = new RandomAccessFile(filename, "r");
            try {
                int fileLength = (int)raf.length();
                int currentChunkStart = chunkNumber * chunkSize;
                if(currentChunkStart >= fileLength) return null;
                int currentChunkSize = Math.min(chunkSize, fileLength - currentChunkStart);
                byte[] bb = new byte[currentChunkSize];
                raf.seek(currentChunkStart);
                raf.readFully(bb, 0, currentChunkSize);
                return bb;
            } finally {
                raf.close();
            }
        } catch(FileNotFoundException e) {
            return null;
        } catch(IOException e) {
            eventDispatcher.errorOccured(new ServerErrorEvent(this,e));
            return null;
        }
    }

    private static class ChunkKey {
        private File filename;
        private int chunkNumber;

        ChunkKey(File filename, int chunkNumber) {
            this.filename = filename;
            this.chunkNumber = chunkNumber;
        }

        public boolean equals(Object o) {
            if(o instanceof ChunkKey) {
                ChunkKey chunkKey = (ChunkKey)o;
                if(filename.equals(chunkKey.filename)
                        && chunkNumber == chunkKey.chunkNumber
                ) {
                    return true;
                }
            }
            return false;
        }

        public int hashCode() {
            return filename.hashCode() + chunkNumber;
        }
    }
}
