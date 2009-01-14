package io.files.util;

import java.io.*;

/**
 * 在目录中穿行, 并根据strategy(策略设计模式)处理这些文件
 * 
 * @author WangYinghua
 * 
 */
public class ProcessFiles {
	// 内嵌接口, 所以要实现此类必须提供相应的处理策略
	public interface Strategy {
		void process(File file);
	}

	private Strategy strategy;
	private String ext;

	public ProcessFiles(Strategy strategy, String ext) {
		this.strategy = strategy;
		this.ext = ext;
	}

	public void start(String[] args) {
		try {
			if (args.length == 0)
				processDirectoryTree(new File("."));
			else
				for (String arg : args) {
					File fileArg = new File(arg);
					if (fileArg.isDirectory())
						processDirectoryTree(fileArg);
					else {
						// Allow user to leave off extension:
						if (!arg.endsWith("." + ext))
							arg += "." + ext;
						strategy.process(new File(arg).getCanonicalFile());
					}
				}
		} catch (IOException e) {
			throw new RuntimeException(e);
		}
	}

	public void processDirectoryTree(File root) throws IOException {
		for (File file : Directory.walk(root.getAbsolutePath(), ".*\\." + ext))
			strategy.process(file.getCanonicalFile());
	}

	// Demonstration of how to use it:
	public static void main(String[] args) {
		new ProcessFiles(new ProcessFiles.Strategy() {
			// 实现回调函数process
			public void process(File file) {
				System.out.println(file);
			}
		}, "java").start(args);
	}
} /* (Execute to see output) */// :~