//: builder:BuildMedia.java
// Example of the Builder pattern
package builder;
import java.util.*;
import junit.framework.*;

// Different "representations" of media:
class Media extends ArrayList {}
class Book extends Media {}
class Magazine extends Media {}
class WebSite extends Media {}

// ... contain different kinds of media items:
class MediaItem {
  private String s;
  public MediaItem(String s) { this.s = s; }
  public String toString() { return s; }
}
class Chapter extends MediaItem {
  public Chapter(String s) { super(s); }
}
class Article extends MediaItem {
  public Article(String s) { super(s); }
}
class WebItem extends MediaItem {
  public WebItem(String s) { super(s); }
}

// ... but use the same basic construction steps:
class MediaBuilder {
  public void buildBase() {}
  public void addMediaItem(MediaItem item) {}
  public Media getFinishedMedia() { return null; }
}

class BookBuilder extends MediaBuilder {
  private Book b;
  public void buildBase() {
    System.out.println("Building book framework");
    b = new Book();
  }
  public void addMediaItem(MediaItem chapter) {
    System.out.println("Adding chapter " + chapter);
    b.add(chapter);
  }
  public Media getFinishedMedia() { return b; }
}  

class MagazineBuilder extends MediaBuilder {
  private Magazine m;
  public void buildBase() {
    System.out.println("Building magazine framework");
    m = new Magazine();
  }
  public void addMediaItem(MediaItem article) {
    System.out.println("Adding article " + article);
    m.add(article);
  }
  public Media getFinishedMedia() { return m; }
}  

class WebSiteBuilder extends MediaBuilder {
  private WebSite w;
  public void buildBase() {
    System.out.println("Building web site framework");
    w = new WebSite();
  }
  public void addMediaItem(MediaItem webItem) {
    System.out.println("Adding web item " + webItem);
    w.add(webItem);
  }
  public Media getFinishedMedia() { return w; }
}  

class MediaDirector { // a.k.a. "Context"
  private MediaBuilder mb;
  public MediaDirector(MediaBuilder mb) {
    this.mb = mb; // Strategy-ish
  }
  public Media produceMedia(List input) {
    mb.buildBase();
    for(Iterator it = input.iterator(); it.hasNext();)
      mb.addMediaItem((MediaItem)it.next());
    return mb.getFinishedMedia();
  }
};

public class BuildMedia extends TestCase {
  private List input = Arrays.asList(new MediaItem[] {
    new MediaItem("item1"), new MediaItem("item2"),
    new MediaItem("item3"), new MediaItem("item4"),
  });
  public void testBook() {
    MediaDirector buildBook = 
      new MediaDirector(new BookBuilder());
    Media book = buildBook.produceMedia(input);
    String result = "book: " + book;
    System.out.println(result);
    assertEquals(result, 
      "book: [item1, item2, item3, item4]");
  }
  public void testMagazine() {
    MediaDirector buildMagazine = 
      new MediaDirector(new MagazineBuilder());
    Media magazine = buildMagazine.produceMedia(input);
    String result = "magazine: " + magazine;
    System.out.println(result);
    assertEquals(result, 
      "magazine: [item1, item2, item3, item4]");
  }
  public void testWebSite() {
    MediaDirector buildWebSite = 
      new MediaDirector(new WebSiteBuilder());
    Media webSite = buildWebSite.produceMedia(input);
    String result = "web site: " + webSite;
    System.out.println(result);
    assertEquals(result, 
      "web site: [item1, item2, item3, item4]");
  }
  public static void main(String[] args) {
    junit.textui.TestRunner.run(BuildMedia.class);
  }
} ///:~
