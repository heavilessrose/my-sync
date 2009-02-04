package com.srijeeb.web;

import java.io.BufferedInputStream;
import java.io.BufferedReader;
import java.io.ByteArrayOutputStream;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

import javax.servlet.Servlet;
import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import com.srijeeb.dto.DTOMediaRetrieveIn;
import com.srijeeb.dto.DTOMediaRetrieveOut;
import com.srijeeb.dto.DTOMediaStoreIn;
import com.srijeeb.dto.DTOMediaStoreOut;

/**
 * @author Srijeeb Roy
 *
 * TODO To change the template for this generated type comment go to
 * Window - Preferences - Java - Code Style - Code Templates
 */
public class MMAServlet extends HttpServlet implements Servlet {

	String fileLocationBase = null;

	/* (non-Java-doc)
	 * @see javax.servlet.http.HttpServlet#HttpServlet()
	 */
	public MMAServlet() {
		super();
	}

	/* (non-Java-doc)
	 * @see javax.servlet.http.HttpServlet#doGet(HttpServletRequest arg0, HttpServletResponse arg1)
	 */
	protected void doGet(HttpServletRequest req, HttpServletResponse resp) throws ServletException, IOException {
		// TODO Auto-generated method stub
		doProcess(req, resp);
	}

	/* (non-Java-doc)
	 * @see javax.servlet.http.HttpServlet#doPost(HttpServletRequest arg0, HttpServletResponse arg1)
	 */
	protected void doPost(HttpServletRequest req, HttpServletResponse resp) throws ServletException, IOException {
		// TODO Auto-generated method stub
		doProcess(req, resp);
	}

	/* (non-Java-doc)
	 * @see javax.servlet.Servlet#init(ServletConfig arg0)
	 */
	public void init() throws ServletException {
		fileLocationBase = this.getInitParameter("fileLocation");
		System.out.println("fileLocationBase=" + fileLocationBase);
	}

	public void doProcess(HttpServletRequest req, HttpServletResponse resp) {


		System.out.println("Inside doProcess...");
		resp.setContentType("application/octet-stream");

		try {
			int contentLength = handleCall(
					   req.getInputStream(),
					   resp.getOutputStream(),
					   req,
					   resp);

			resp.setContentLength(contentLength);

		}catch(Exception e) {
			e.printStackTrace();
			resp.setStatus(HttpServletResponse.SC_INTERNAL_SERVER_ERROR);
		}


	}
	public int handleCall(
			InputStream in,
			OutputStream out,
			HttpServletRequest req,
			HttpServletResponse resp)
			throws Exception {

		System.out.println("Inside handleCall...");
		DataInputStream call = new DataInputStream(in);
		DataOutputStream result = new DataOutputStream(out);

		byte method = -1;
		ByteArrayOutputStream bufferedResult =
			new ByteArrayOutputStream();
		DataOutputStream successfulResult =
			new DataOutputStream(bufferedResult);
		method = call.readByte();
		if ( method == 1 ) {//Store Media
			System.out.println("Going to store Media...");
			DTOMediaStoreIn inDTO = DTOMediaStoreIn.deserialize(call);
			DTOMediaStoreOut outDTO = storeMedia(inDTO);
			outDTO.serialize(successfulResult);
		}
		else if ( method == 2 ) {//Retrieve Media
			System.out.println("Going to retrieve Media...");
			DTOMediaRetrieveIn inDTO = DTOMediaRetrieveIn.deserialize(call);
			DTOMediaRetrieveOut outDTO = retrieveMedia(inDTO);
			outDTO.serialize(successfulResult);
		}
		successfulResult.flush();

		byte[] bufferedBytes = bufferedResult.toByteArray();

		result.write(bufferedBytes, 0, bufferedBytes.length);

		return result.size();
	}

	/**
	 * @param inDTO
	 * @return
	 */
	private DTOMediaStoreOut storeMedia(DTOMediaStoreIn inDTO) {

		System.out.println("Inside Media Store...");
		DTOMediaStoreOut ret = new DTOMediaStoreOut();
		ret.successFlag = "X";
		if ( inDTO != null ) {

			String currentNo = getCurrentNo();

			FileOutputStream aFileOutputStream ;
			try {
				aFileOutputStream = new FileOutputStream(fileLocationBase + "/" + currentNo + "." + inDTO.extension);
				aFileOutputStream.write(inDTO.inpByteArray);
				aFileOutputStream.close();
				ret.successFlag = "S";
				System.out.println("Successfully stored the content");
			} catch (FileNotFoundException e) {
				e.printStackTrace();
			} catch (IOException e) {
				e.printStackTrace();
			}
		}

		return ret;
	}

	private synchronized String getCurrentNo() {

		try {
			BufferedReader br = new BufferedReader( new FileReader(fileLocationBase + "/SEQUENCE.txt" ));
			String line = br.readLine();
			if ( line == null ) {
				line = "0";
			}
			br.close();

			int seq = Integer.parseInt(line.trim());

			FileWriter fw = new FileWriter(fileLocationBase + "/SEQUENCE.txt");
			fw.write(String.valueOf(seq + 1));
			fw.close();

			return String.valueOf(seq + 1);

		} catch (FileNotFoundException e) {
			try {
				//e.printStackTrace();
				FileWriter fw = new FileWriter(fileLocationBase + "/SEQUENCE.txt");
				fw.write("0");
				fw.close();
				return "0";
			} catch (IOException e1) {
				e1.printStackTrace();
			}
		} catch (IOException e) {
			e.printStackTrace();
		}
		return null;

	}
	/**
	 * @param inDTO
	 * @return
	 */
	private DTOMediaRetrieveOut retrieveMedia(DTOMediaRetrieveIn inDTO) {
		System.out.println("Inside media retrieve...");
		DTOMediaRetrieveOut ret = new DTOMediaRetrieveOut();
		ret.successFlag = "X";

		if ( inDTO != null ) {
			try {

				String ext = findFileExtension(inDTO.mediaId);
				BufferedInputStream bf = new BufferedInputStream( new FileInputStream(fileLocationBase + "/" + inDTO.mediaId + "." + ext));
				int available = bf.available();
				byte[] bytes = new byte[available];
				bf.read(bytes);
				bf.close();
				ret.outByteArray = bytes;
				ret.successFlag = "S";
				ret.contentType = findContentType(ext);
				System.out.println("Successfully Read the content");

			} catch (FileNotFoundException e) {
				e.printStackTrace();
			} catch (IOException e) {
				e.printStackTrace();
			}
		}

		return ret;
	}

	private String findFileExtension(String mediaId) {
		try {

			File aFile = new File(fileLocationBase);
			if ( aFile.isDirectory()) {

				File[] filesInDirectory = aFile.listFiles();
				if ( filesInDirectory != null && filesInDirectory.length > 0 ) {

					for ( int i=0; i < filesInDirectory.length; i++) {
						String name = filesInDirectory[i].getName();
						if ( name != null ) {
							int indexOfDot = name.indexOf(".");
							if ( indexOfDot != -1 ) {
								String fileNameWithoutExt = name.substring(0,indexOfDot);
								if ( fileNameWithoutExt.equals(mediaId)) {

									return name.substring(indexOfDot+1,name.length() );
								}

							}

						}
					}
				}
			}

		}catch(Exception e) {
			e.printStackTrace();
		}
		return "";

	}


    private String findContentType(String ext) {
		String ct = "";
		if (ext.equals("mpg") || ext.equals("avi") ) {
			ct = "video/mpeg";
		} else if (ext.equals("3gp")) {
			ct = "video/3gpp";
		} else if (ext.equals("wav")) {
			ct = "audio/x-wav";
		} else if (ext.equals("gif")) {
			ct = "image/gif";
		} else if (ext.equals("png")) {
			ct = "image/png";
		}

		return ct;
    }
}