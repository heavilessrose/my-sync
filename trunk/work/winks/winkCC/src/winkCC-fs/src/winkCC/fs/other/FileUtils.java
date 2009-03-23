package winkCC.fs.other;

import java.io.IOException;

import javax.microedition.io.Connector;
import javax.microedition.io.file.FileConnection;

public class FileUtils {

	public void readFile(String fileName) {

	}

	public void writeFile(String fileName) {

	}

	public void openFile(String fileName) throws IOException {
		FileConnection fc = (FileConnection) Connector.open("file:///" + fileName);
	}

	public void closeFile(String fileName) {

	}

	public void deleteFile(String fileName) {

	}

	public void isDir(String fileName) {

	}

	public void traverseDir(String fileName) {

	}

	public void getProperties(String fileName) {

	}
}
