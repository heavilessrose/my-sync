package com.winksi.j2me.cc.model.main;

/**
 * DIY数据 模型
 * @author Jerry
 */
public class DiyDataModel {

	/** 
	 * 单个块大小 50k 
	 * 需求: 
	 * JPG在30k之内的可直接上传；JPG大于30k小于500k的，终端本地分别上传图像，由平台进行转换；如大于500k，将提示并操作失败.
	 */
	public static final int _singleBlockLength = 1024 * 50;
	/** 本地DIY图片文件数据 */
	private byte[] _fileData;
	private byte[] _greetingMsgData;
	private byte[] _numData;
	/** 完整文件名(含后缀) */
	private String _fullName;
	/** 文件名(不含后缀) */
	private String _fileName;
	/**
	 *  文件类型
	 *  1:jpg, 2:png, 3:gif
	 */
	private String _fileType;
	/** 图片文件的一个唯一标识 */
	private String _fileId;
	private int _total_block = 0;
	private int _block_index = 0;
	/** 本地DIY图片文件数据长度 */
	private int _total_length = 0;

	public DiyDataModel() {

	}

	public void init() {
		_fileData = null;
		_greetingMsgData = null;
		_numData = null;
		_fullName = null;
		_fileName = null;
		_fileType = null;
		_fileId = null;
		_total_block = 0;
		_block_index = 0;
		_total_length = 0;
	}

	/** 设置本地DIY图片数据 */
	public void setFileData(byte[] b) {
		this._fileData = b;
		_total_length = _fileData.length;
		int block_sum = _fileData.length / _singleBlockLength;
		//        System.out.println("file set size : "+filedata.length);
		//        System.out.println("block size : "+blocksize);
		if (block_sum > 1) {
			_total_block = block_sum + 1; // ?
		} else {
			_total_block = 1;
		}
		_block_index = 1;
	}

	/** 设置问候语 */
	public void setMsgData(byte[] b) {
		this._greetingMsgData = b;
	}

	/** 定制DIY号码 */
	public void setNumberData(byte[] b) {
		this._numData = b;
	}

	/** 设置一个图片的完整文件名 */
	public void setFullFileName(String s) {
		this._fullName = s;
		_fileName = _fullName.substring(0, _fullName.lastIndexOf('.'));
		//        System.out.println("file name : "+filename);
		String filetypename = (_fullName.substring(_fullName.lastIndexOf('.') + 1)).toLowerCase();
		//        System.out.println("filetypename : "+filetypename);
		if (filetypename.equals("jpeg") || filetypename.equals("jpg")) {
			_fileType = "1";
		} else if (filetypename.equals("png")) {
			_fileType = "2";
		} else if (filetypename.equals("gif")) {
			_fileType = "6";
		}
		//        filetype=fullname.substring(fullname.indexOf('.')+1);
	}

	public byte[] getFileData() {
		return this._fileData;
	}

	public int getTotalLength() {
		return this._total_length;
	}

	public int getTotalBlock() {
		return this._total_block;
	}

	public int getIndexBlock() {
		return this._block_index;
	}

	public byte[] getMsgData() {
		return this._greetingMsgData;
	}

	public byte[] getNumberData() {
		return this._numData;
	}

	public String getFileName() {
		return this._fileName;
	}

	public String getFileType() {
		return this._fileType;
	}

	public void setFileID(String id) {
		this._fileId = id;
	}

	public String getFileID() {
		return this._fileId;
	}

}
