package luke.java.practice.net.socket.TCP.blockingIO.chatApp;

/*************************************************************************
 *  Compilation:  javac ChatClient.java
 *  Execution:    java ChatClient name host
 *  Dependencies: In.java Out.java
 *
 *  Connects to host server on port 4444, enables an interactive
 *  chat client.
 *  
 *  % java ChatClient alice localhost
 *
 *  % java ChatClient bob localhost
 *  
 *************************************************************************/

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Container;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.net.Socket;

import javax.swing.JFrame;
import javax.swing.JScrollPane;
import javax.swing.JTextArea;
import javax.swing.JTextField;

import luke.java.practice.net.socket.TCP.blockingIO.echoApp.In;
import luke.java.practice.net.socket.TCP.blockingIO.echoApp.Out;

public class ChatClient extends JFrame implements ActionListener {

	private String screenName;

	// GUI stuff
	private JTextArea enteredText = new JTextArea(10, 32);
	private JTextField typedText = new JTextField(32);

	// socket for connection to chat server
	private Socket socket;

	// for writing to and reading from the server
	private Out out;
	private In in;

	public ChatClient(String screenName, String hostName) {

		// connect to server
		try {
			socket = new Socket(hostName, 4444);
			out = new Out(socket);
			in = new In(socket);
		} catch (Exception ex) {
			ex.printStackTrace();
		}
		this.screenName = screenName;

		// close output stream - this will cause listen() to stop and exit
		addWindowListener(new WindowAdapter() {
			public void windowClosing(WindowEvent e) {
				out.close();
				// in.close();
				// try { socket.close(); }
				// catch (Exception ioe) { ioe.printStackTrace(); }
			}
		});

		// create GUI stuff
		enteredText.setEditable(false);
		enteredText.setBackground(Color.LIGHT_GRAY);
		typedText.addActionListener(this);

		Container content = getContentPane();
		content.add(new JScrollPane(enteredText), BorderLayout.CENTER);
		content.add(typedText, BorderLayout.SOUTH);

		// display the window, with focus on typing box
		setTitle("Chat Client 1.0: [" + screenName + "]");
		setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		pack();
		typedText.requestFocusInWindow();
		setVisible(true);

	}

	// process TextField after user hits Enter
	public void actionPerformed(ActionEvent e) {
		out.println("[" + screenName + "]: " + typedText.getText());
		typedText.setText("");
		typedText.requestFocusInWindow();
	}

	// listen to socket and print everything that server broadcasts
	public void listen() {
		String s;
		while ((s = in.readLine()) != null) {
			enteredText.insert(s + "\n", enteredText.getText().length());
			enteredText.setCaretPosition(enteredText.getText().length());
		}
		out.close();
		in.close();
		try {
			socket.close();
		} catch (Exception e) {
			e.printStackTrace();
		}
		System.err.println("Closed client socket");
	}

	public static void main(String[] args) {
		ChatClient client = new ChatClient("Luke", "127.0.0.1");
		client.listen();
	}
}
