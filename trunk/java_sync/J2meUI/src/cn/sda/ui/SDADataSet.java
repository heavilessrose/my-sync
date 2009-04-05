package cn.sda.ui;

import cn.sda.enterprise.*;
import cn.sda.event.DataFilterEvent;
import cn.sda.event.DataScrollChangeEvent;
import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.util.Enumeration;
import java.util.Vector;
import javax.microedition.rms.RecordEnumeration;
import javax.microedition.rms.RecordStore;

/**
 * 
 * @author Administrator
 */
// 数据集合类
public class SDADataSet {
	// rms
	private DataSetRMS dataRms = null;
	// 字段类型
	public static final byte FieldBoolean = 0;
	public static final byte FieldByte = 1;
	public static final byte FieldShort = 2;
	public static final byte FieldInteger = 3;
	public static final byte FieldLong = 4;
	public static final byte FieldFloat = 5;
	public static final byte FieldDouble = 6;
	public static final byte FieldString = 7;
	public static final byte FieldAudoID = 8;
	// 排序方式
	public static final int sortDesc = 0;
	public static final int sortAsc = 1;
	// 字段集合
	private Vector Fields = null;
	// 可见值集合
	private Vector Rows = null;
	// 所有值集合
	private Vector allRows = null;
	// delta记录
	private Vector deltaFields = null;
	private Vector deltaRows = null;
	// Key
	private String keyName = "";
	// 数据集合指针位置
	public int RecNo = -1;
	// 表达式计算器
	private ExpressionParser exp;
	// 记录merge
	private boolean recChangeLog = false;
	// 是否打开自动ID计算
	private boolean autoID = false;
	// 需要刷新的表格列表
	private Vector dataControlList = null;
	// 滚动事件
	private DataScrollChangeEvent onDataScrollChange = null;
	// 数据执行过滤的事件
	private DataFilterEvent onDataFilter = null;
	// 对链接组件通知变动
	private boolean enableControls = true;

	// 字段描述
	public class Field {
		// 字段名
		private String FiledName = "";
		// 字段类型
		private byte FiledType = FieldString;
		// 是否为计算字段
		private boolean CalField = false;
		// 计算字段表达式
		private String CalExpression = "";

		public Field() {
		}

		public Field(String fieldName) {
			this.FiledName = fieldName;
		}

		public Field(String fieldName, byte fieldType) {
			this.FiledName = fieldName;
			if ((fieldType > -1) && (fieldType < 8)) {
				this.FiledType = fieldType;
			}
		}

		public Field(String fieldName, byte fieldType, boolean isCalField,
				String CalExpression) {
			this.FiledName = fieldName;
			if ((fieldType > -1) && (fieldType < 8)) {
				this.FiledType = fieldType;
			}
			this.CalField = isCalField;
			this.CalExpression = CalExpression;
		}

		public String getCalExpression() {
			return CalExpression;
		}

		public void setCalExpression(String CalExpression) {
			if (isCanEditFields()) {
				this.CalExpression = CalExpression;
			} else {
				System.out.println("Can not change Expression.");
			}
		}

		public String getFiledName() {
			return FiledName;
		}

		public void setFiledName(String FiledName) {
			this.FiledName = FiledName;
		}

		public int getFiledType() {
			return FiledType;
		}

		public void setFiledType(byte FiledType) {
			if (isCanEditFields()) {
				this.FiledType = FiledType;
			} else {
				System.out.println("Can not change FiledType.");
			}
		}

		public boolean isCalFiled() {
			return CalField;
		}

		public void setCalFiled(boolean isCalFiled) {
			if (isCanEditFields()) {
				this.CalField = isCalFiled;
			} else {
				System.out.println("Can not change CalField.");
			}
		}
	}

	// 列构造方法
	public Field newField() {
		return new Field();
	}

	public Field newField(String fieldName) {
		return new Field(fieldName);
	}

	public Field newField(String fieldName, byte fieldType) {
		return new Field(fieldName, fieldType);
	}

	public Field newField(String fieldName, byte fieldType, boolean isCalField,
			String CalExpression) {
		return new Field(fieldName, fieldType, isCalField, CalExpression);
	}

	// 行数据
	public class Row {

		private Vector valueList;
		private boolean visible = true; // 是否可见（过滤的时候需要）

		private void internalRow() {
			valueList = new Vector();
			// 增加空值
			for (int i = 0; i < Fields.size(); i++) {
				valueList.addElement(null);
			}
		}

		public Row() {
			internalRow();
		}

		public boolean isVisible() {
			return visible;
		}

		public void setVisible(boolean visible) {
			this.visible = visible;
			// 设置是否在Rows
			if (!visible) {
				if (isInRows()) {
					Rows.removeElement(this);
					setAutoID();
					refreshDataControl();
				}
			} else {
				if (!isInRows()) {
					Rows.addElement(this);
					setAutoID();
					refreshDataControl();
				}
			}
		}

		// 获取指定字段值
		public String getValueByFieldName(String fieldName) {
			return internalGetValueByFieldName(fieldName);
		}

		private String internalGetValueByFieldName(String fieldName) {
			Field fd;
			String result = null;
			for (int i = 0; i < Fields.size(); i++) {
				fd = (Field) Fields.elementAt(i);
				if (fd.FiledName.equals(fieldName)) {
					result = (String) valueList.elementAt(i);
					break;
				}
			}
			return result;
		}

		public String getValueByFieldIndex(int fieldIndex) {
			return internalGetValueByFieldIndex(fieldIndex);
		}

		private String internalGetValueByFieldIndex(int fieldIndex) {
			if ((fieldIndex > -1) && (fieldIndex < valueList.size())) {
				// 得到字段以后就从当前的行中获取值
				return (String) valueList.elementAt(fieldIndex);
			} else {
				return null;
			}
		}

		public Object getObjectByFieldIndex(int fieldIndex) {
			return internalGetObjectByFieldIndex(fieldIndex);
		}

		private Object internalGetObjectByFieldIndex(int fieldIndex) {
			if ((fieldIndex > -1) && (fieldIndex < valueList.size())) {
				// 得到字段以后就从当前的行中获取值
				return valueList.elementAt(fieldIndex);
			} else {
				return null;
			}
		}

		public void setValueByFieldName(String fieldName, String fieldValue) {
			internalSetValueByFieldName(fieldName, fieldValue);
		}

		private void internalSetValueByFieldName(String fieldName,
				String fieldValue) {
			Field fd;
			for (int i = 0; i < Fields.size(); i++) {
				fd = (Field) Fields.elementAt(i);
				if ((fd.FiledName.equals(fieldName))
						&& (fd.FiledType != SDADataSet.FieldAudoID)) {
					valueList.setElementAt(fieldValue, i);
					// 计算
					try {
						calField(this);
					} catch (Exception e) {
					}
					if (isInRows()) {
						refreshDataControl();
					}
					break;
				}
			}
		}

		public void setValueByFieldIndex(int fieldIndex, String fieldValue) {
			internalSetValueByFieldIndex(fieldIndex, fieldValue);
		}

		private void internalSetValueByFieldIndex(int fieldIndex,
				String fieldValue) {
			if ((fieldIndex > -1) && (fieldIndex < valueList.size())) {
				Field fd = null;
				fd = (Field) Fields.elementAt(fieldIndex);
				if (fd.FiledType != SDADataSet.FieldAudoID) {
					valueList.setElementAt(fieldValue, fieldIndex);
					// 计算
					try {
						calField(this);
					} catch (Exception e) {
					}
					if (isInRows()) {
						refreshDataControl();
					}
				}
			}
		}

		// 判断是否在Rows中
		private boolean isInRows() {
			return Rows.indexOf(this) > -1;
		}
	}

	// RMS操作类
	private class DataSetRMS {

		public DataSetRMS() {
		}

		public RecordStore OpenRS(String tableName) {
			RecordStore rs = null;
			if (tableName.length() > 32) {
				return null;
			}
			try {
				rs = RecordStore.openRecordStore(tableName, true);
				return rs;
			} catch (Exception e) {
				return null;
			}
		}

		// 写入数据库
		public void WriteDataSet(String tableName) {
			// 写入（先删除）
			deleteTable(tableName);
			// 打开空的
			RecordStore rs = OpenRS(tableName);
			ByteArrayOutputStream bos = null;
			DataOutputStream dos = null;
			Field fd = null;
			Row row = null;
			byte[] SendByte = null;
			// 创建
			bos = new ByteArrayOutputStream();
			dos = new DataOutputStream(bos);
			try {
				// 写入字段数
				dos.writeShort((short) Fields.size());
				// 写入字段
				for (int i = 0; i < Fields.size(); i++) {
					fd = (Field) Fields.elementAt(i);
					// 字段名
					dos.writeUTF(fd.FiledName);
					// 类型
					dos.writeByte(fd.FiledType);
					// 是否计算字段
					dos.writeBoolean(fd.CalField);
					// 表达式
					dos.writeUTF(fd.CalExpression);
				}
				SendByte = bos.toByteArray();
				rs.addRecord(SendByte, 0, SendByte.length);
				bos.close();
				dos.close();
				// 写入具体内容
				for (int i = 0; i < allRows.size(); i++) {
					bos = null;
					dos = null;
					bos = new ByteArrayOutputStream();
					dos = new DataOutputStream(bos);
					row = (Row) allRows.elementAt(i);
					dos.writeBoolean(row.visible);
					for (int j = 0; j < Fields.size(); j++) {
						dos.writeUTF(row.getValueByFieldIndex(j));
					}
					// 写入
					SendByte = bos.toByteArray();
					rs.addRecord(SendByte, 0, SendByte.length);
					bos.close();
					dos.close();
					bos = null;
					dos = null;
				}
				rs.closeRecordStore();
			} catch (Exception e) {
			}
		}

		public void deleteTable(String tableName) {
			try {
				RecordStore.deleteRecordStore(tableName);
			} catch (Exception e) {
			}
		}

		// 装载
		public void loadTable(String tableName) {
			// 清空当前记录
			internalClearFields();
			// id数组
			int[] arrayID = null;
			// 读新记录
			RecordStore rs = OpenRS(tableName);
			try {
				if (rs.getNumRecords() == 0) {
					rs.closeRecordStore();
					return;
				}
			} catch (Exception e) {
			}
			ByteArrayInputStream bis = null;
			DataInputStream dis = null;
			byte[] recByte = null;
			try {
				RecordEnumeration rem = rs.enumerateRecords(null, null, false);
				// 记录的顺序要重新排布
				arrayID = new int[rem.numRecords()];
				int index = 0;
				while (rem.hasNextElement()) {
					arrayID[index] = rem.nextRecordId();
					index++;
				}
				boolean asc = arrayID[0] < arrayID[arrayID.length - 1] ? true
						: false;
				// 读字段
				recByte = rs.getRecord(arrayID[asc ? 0 : arrayID.length - 1]);
				bis = new ByteArrayInputStream(recByte);
				dis = new DataInputStream(bis);
				// 字段数目
				short fieldNum = dis.readShort();
				for (int i = 0; i < fieldNum; i++) {
					Field fd = new Field();
					fd.FiledName = dis.readUTF();
					fd.FiledType = dis.readByte();
					fd.CalField = dis.readBoolean();
					fd.CalExpression = dis.readUTF();
					Fields.addElement(fd);
				}
				bis.close();
				dis.close();
				bis = null;
				dis = null;
				// 读具体内容
				for (int i = 1; i < arrayID.length; i++) {
					recByte = rs.getRecord(arrayID[asc ? i : arrayID.length - i
							- 1]);
					bis = new ByteArrayInputStream(recByte);
					dis = new DataInputStream(bis);
					Row row = new Row();
					row.visible = dis.readBoolean();
					for (int j = 0; j < Fields.size(); j++) {
						String ok = dis.readUTF();
						row.setValueByFieldIndex(j, ok);
					}
					allRows.addElement(row);
					if (row.visible)
						Rows.addElement(row);
					dis.close();
					bis.close();
					bis = null;
					dis = null;
				}
				rem.destroy();
				rs.closeRecordStore();
			} catch (Exception e) {
			}
		}
	}

	// 行构造
	public Row newRow() {
		return new Row();
	}

	public SDADataSet() {
		internalSDADataSet();
	}

	private void internalSDADataSet() {
		Fields = new Vector();
		Rows = new Vector();
		allRows = new Vector();
		exp = new ExpressionParser();
		recChangeLog = false;
		deltaFields = new Vector();
		deltaRows = new Vector();
		dataControlList = new Vector();
		dataRms = new DataSetRMS();
	}

	// 从数据库加载
	public void loadFromRms(String tableName) {
		dataRms.loadTable(tableName);
		setAutoID();
		refreshDataControl();
	}

	// 保存到数据库
	public void saveToRms(String tableName) {
		dataRms.WriteDataSet(tableName);
	}

	public void showAllRecord() {
		enableControls = false;
		Row row = null;
		for (int i = 0; i < allRows.size(); i++) {
			row = (Row) allRows.elementAt(i);
			if (!row.visible) {
				row.setVisible(true);
			}
		}
		enableControls();
	}

	public void setFiltered() {
		First();
		enableControls = false;
		// 执行过滤
		Row row = null;
		for (int i = 0; i < allRows.size(); i++) {
			row = (Row) allRows.elementAt(i);
			row.setVisible(true);
			doDataFilter(row);
		}
		enableControls();
	}

	public boolean isAutoID() {
		return autoID;
	}

	public void setAutoID(boolean autoID) {
		this.autoID = autoID;
		setAutoID();
		refreshDataControl();
	}

	// 对观察表格进行中断处理
	public void disableControls() {
		enableControls = false;
	}

	public void enableControls() {
		enableControls = true;
		refreshDataControl();
	}

	// 刷新组件
	private void refreshDataControl() {
		if (enableControls) {
			for (int i = 0; i < dataControlList.size(); i++) {
				SDABaseControl ctrl = (SDABaseControl) dataControlList
						.elementAt(i);
				if (ctrl != null) {
					ctrl.paint();
				}
			}
		}
	}

	// 需要刷新的表格
	protected void setWatchControl(SDABaseControl control, boolean setWatch) {
		if (setWatch) {
			if (!dataControlList.contains(control)) {
				dataControlList.addElement(control);
			}
		} else {
			int id = dataControlList.indexOf(control);
			if (id > -1) {
				dataControlList.removeElementAt(id);
			}
		}
	}

	// 字段操作处理
	// 如果已经存在row,字段将不能够再操作
	private boolean isCanEditFields() {
		boolean result = false;
		if (recChangeLog) {
			result = false;
		} else {
			result = true;
		}
		if (Rows.size() > 0) {
			result = false;
		} else {
			result = true;
		}
		return result;
	}

	// 如果没有字段，就不能够增加字段
	private boolean isCanAddRow() {
		if (Fields.size() < 1) {
			return false;
		} else {
			return true;
		}
	}

	// 开始记录delta,设置以后，结构不允许变动
	public void setDeltaRecord(boolean canRecord, String keyFieldName) {
		internalSetDeltaRecord(canRecord, keyFieldName);
	}

	private void internalSetDeltaRecord(boolean canRecord, String keyFieldName) {
		if (recChangeLog && canRecord) {
			return;
		}
		recChangeLog = canRecord;
		if (recChangeLog) {
			// 判断结构中有无该key
			if (!Fields.contains(keyFieldName)) {
				System.out.println("Can not find the keyFieldName:\""
						+ keyFieldName + "\"");
				return;
			}
			// 记录当前数据信息到临时结构
			keyName = keyFieldName;
			copyToDelta();
		} else {
			// 删除临时结构内容
			deleteFromDelta();
		}
	}

	// 获取delpta内容
	public String getDelta() {
		return internalGetDelta();
	}

	private String internalGetDelta() {
		// 比较新旧内容，计算出差异
		StringBuffer DeltaXml = new StringBuffer();
		DeltaXml.append("<?xml version=\"1.0\" encoding=\"UTF-8\" ?>");
		DeltaXml.append("<DataSet>");
		// 循环当前Rows
		Row row = null;
		Field fd = null;
		String keyValue = "";
		for (int i = 0; i < Rows.size(); i++) {
			row = (Row) Rows.elementAt(i);
			keyValue = row.getValueByFieldName(keyName);
			if (keyValue == null) {
				// 新增的
				DeltaXml.append("<D us=\"Insert\">");
				for (int j = 0; j < row.valueList.size(); j++) {
					fd = (Field) Fields.elementAt(j);
					DeltaXml.append("<" + fd.FiledName + "><![CDATA["
							+ String.valueOf(row.valueList.elementAt(j))
							+ "]]></" + fd.FiledName + ">");
				}
				DeltaXml.append("</D>");
			}
			// 查找旧的里面有无新Key值,有就比较值，不同就增，没有也增
			Row oldrow = getDeltaRow(keyName, keyValue);
			if (oldrow != null) {
				for (int j = 0; j < Fields.size(); j++) {
					Row newrow = (Row) Rows.elementAt(i);
					if (!newrow.valueList.elementAt(j).equals(
							oldrow.valueList.elementAt(j))) {
						fd = (Field) Fields.elementAt(j);
						DeltaXml.append("<D us=\"Modify\">");
						for (int k = 0; k < newrow.valueList.size(); k++) {
							fd = (Field) Fields.elementAt(k);
							DeltaXml.append("<"
									+ fd.FiledName
									+ "><![CDATA["
									+ String.valueOf(newrow.valueList
											.elementAt(k)) + "]]></"
									+ fd.FiledName + ">");
						}
						DeltaXml.append("</D>");
						break;
					}
				}
			} else {
				// 增加
				DeltaXml.append("<D us=\"Insert\">");
				for (int j = 0; j < row.valueList.size(); j++) {
					fd = (Field) Fields.elementAt(j);
					DeltaXml.append("<" + fd.FiledName + "><![CDATA["
							+ String.valueOf(row.valueList.elementAt(j))
							+ "]]></" + fd.FiledName + ">");
				}
				DeltaXml.append("</D>");
			}
		}
		// 删除的
		for (int i = 0; i < deltaRows.size(); i++) {
			row = (Row) deltaRows.elementAt(i);
			keyValue = row.getValueByFieldName(keyName);
			if (getRow(keyName, keyValue) == null) {
				// 删除的
				DeltaXml.append("<D us=\"Delete\">");
				DeltaXml.append("<" + keyName + "><![CDATA[" + keyValue
						+ "]]></" + keyName + ">");
				DeltaXml.append("</D>");
			}
		}
		DeltaXml.append("</DataSet>");
		return DeltaXml.toString();
	}

	// 拷贝信息
	public Field copyField(Field fd) {
		return internalCopyField(fd);
	}

	private Field internalCopyField(Field fd) {
		if (fd == null) {
			return null;
		}
		Field CopyField = new Field(fd.FiledName, fd.FiledType, fd.CalField,
				fd.CalExpression);
		return CopyField;
	}

	public Row copyRow(Row row) {
		return internalCopyRow(row);
	}

	private Row internalCopyRow(Row row) {
		if (row == null) {
			return null;
		}
		Row CopyRow = new Row();
		// 拷贝记录
		for (int i = 0; i < row.valueList.size(); i++) {
			String st = String.valueOf(row.valueList.elementAt(i));
			CopyRow.valueList.addElement(st);
		}
		return CopyRow;
	}

	// 拷贝数据到delta记录结构
	private void copyToDelta() {
		// Fields拷贝
		for (int i = 0; i < Fields.size(); i++) {
			deltaFields.addElement(copyField((Field) Fields.elementAt(i)));
		}
		// row拷贝
		for (int i = 0; i < Rows.size(); i++) {
			deltaRows.addElement(copyRow((Row) Rows.elementAt(i)));
		}
	}

	// 删除临时结构的数据
	private void deleteFromDelta() {
		// Fields删除
		deltaFields.removeAllElements();
		// 行删除
		deltaRows.removeAllElements();
		System.gc();
	}

	// 增加
	public void addFiled(Field field) {
		internalAddFiled(field);
	}

	private void internalAddFiled(Field field) {
		if (isCanEditFields()) {
			if (!Fields.contains(field)) {
				Fields.addElement(field);
			}
		} else {
			System.out.println("Can not edit Fields.");
		}
	}

	// 清除字段
	public void close() {
		internalClearFields();
	}

	private void internalClearFields() {
		Fields.removeAllElements();
		// 也得清除数据
		for (int i = 0; i < allRows.size(); i++) {
			((Row) allRows.elementAt(i)).valueList.removeAllElements();
		}
		allRows.removeAllElements();
		Rows.removeAllElements();
		refreshDataControl();
		System.gc();
	}

	// 删除字段
	public void deleteField(int fieldIndex) {
		internalDeleteField(fieldIndex);
	}

	private void internalDeleteField(int fieldIndex) {
		if (isCanEditFields()) {
			if ((fieldIndex > -1) && (fieldIndex < Fields.size())) {
				Fields.removeElementAt(fieldIndex);
			}
		} else {
			System.out.println("Can not edit Fields.");
		}
	}

	public void deleteField(String fieldName) {
		internalDeleteField(fieldName);
	}

	private void internalDeleteField(String fieldName) {
		if (isCanEditFields()) {
			for (int i = 0; i < Fields.size(); i++) {
				Field fd = (Field) Fields.elementAt(i);
				if (fd.FiledName.equals(fieldName)) {
					Fields.removeElementAt(i);
					fd = null;
					break;
				}
			}
		} else {
			System.out.println("Can not edit Fields.");
		}
	}

	public void deleteField(Field field) {
		internalDeleteField(field);
	}

	private void internalDeleteField(Field field) {
		if (isCanEditFields()) {
			int index = Fields.indexOf(field);
			if (index > -1) {
				Fields.removeElementAt(index);
			}
		} else {
			System.out.println("Can not edit Fields.");
		}
	}

	public Field getField(int fieldIndex) {
		return internalGetField(fieldIndex);
	}

	private Field internalGetField(int fieldIndex) {
		Field fd = null;
		if ((fieldIndex > -1) && (fieldIndex < Fields.size())) {
			fd = (Field) Fields.elementAt(fieldIndex);
		}
		return fd;
	}

	public int getFieldIndex(String fieldName) {
		return internalGetFieldIndex(fieldName);
	}

	private int internalGetFieldIndex(String fieldName) {
		Field fd = null;
		int result = -1;
		for (int i = 0; i < Fields.size(); i++) {
			fd = (Field) Fields.elementAt(i);
			if (fd.FiledName.equals(fieldName)) {
				result = i;
			}
		}
		return result;
	}

	public int getFieldsCount() {
		return Fields.size();
	}

	public Field getField(String fieldName) {
		return internalGetField(fieldName);
	}

	private Field internalGetField(String fieldName) {
		Field fd = null;
		for (int i = 0; i < Fields.size(); i++) {
			fd = (Field) Fields.elementAt(i);
			if (fd.FiledName.equals(fieldName)) {
				break;
			}
		}
		return fd;
	}

	// 获取资料
	// 记录数
	public int getRecordCount() {
		return Rows.size();
	}

	// 当前记录号
	public int getRecordNo() {
		return RecNo;
	}

	public void setRecordNo(int RecNo) {
		internalSetRecordNo(RecNo);
	}

	private void internalSetRecordNo(int RecNo) {
		if (Rows.size() > 0) {
			if ((Rows.size() >= RecNo) && (RecNo > -1)) {
				this.RecNo = RecNo;
				refreshDataControl();
			}
		}
	}

	protected void doDataChangeScroll(int recNo) {
		if (onDataScrollChange != null) {
			Row row = getRow(recNo);
			onDataScrollChange.Event(row);
		}
	}

	protected void doDataChangeScroll(Row row) {
		if (onDataScrollChange != null) {
			onDataScrollChange.Event(row);
		}
	}

	// 过滤
	protected void doDataFilter(Row row) {
		if (onDataFilter != null) {
			onDataFilter.Event(row);
		}
	}

	// 导航
	public Row First() {
		return internalFirst();
	}

	private Row internalFirst() {
		if (Rows.size() > 0) {
			Row row = null;
			RecNo = 0;
			row = (Row) Rows.elementAt(RecNo);
			doDataChangeScroll(row);
			refreshDataControl();
			return row;
		} else {
			return null;
		}
	}

	public Row Prior() {
		return internalPrior();
	}

	private Row internalPrior() {
		if (Rows.size() > 0) {
			if (RecNo > 0) {
				Row row = null;
				RecNo--;
				refreshDataControl();
				row = (Row) Rows.elementAt(RecNo);
				doDataChangeScroll(row);
				return row;
			} else {
				return null;
			}
		} else {
			return null;
		}
	}

	public Row Next() {
		return internalNext();
	}

	private Row internalNext() {
		if (Rows.size() > 0) {
			if (RecNo < Rows.size() - 1) {
				Row row = null;
				RecNo++;
				refreshDataControl();
				row = (Row) Rows.elementAt(RecNo);
				doDataChangeScroll(row);
				return row;
			} else {
				return null;
			}
		} else {
			return null;
		}
	}

	public Row Last() {
		return internalLast();
	}

	private Row internalLast() {
		if (Rows.size() > 0) {
			RecNo = Rows.size() - 1;
			refreshDataControl();
			Row row = null;
			row = (Row) Rows.elementAt(RecNo);
			doDataChangeScroll(row);
			return row;
		} else {
			return null;
		}
	}

	public boolean isEof() {
		return RecNo == Rows.size() - 1;
	}

	// 删除
	public boolean deleteRow() {
		return internalDeleteRow();
	}

	private boolean internalDeleteRow() {
		if (allRows.size() > 0) {
			Row row = (Row) Rows.elementAt(RecNo);
			Rows.removeElementAt(RecNo);
			if (RecNo > Rows.size() - 1) {
				RecNo--;
				refreshDataControl();
				doDataChangeScroll(RecNo);
			}
			allRows.removeElement(row);
			row = null;
			return true;
		} else {
			return false;
		}
	}

	// 判断是否有自动序号字段，有就自动写入序号
	private void setAutoID() {
		if (!autoID) {
			return;
		}
		Field fd = null;
		Row row = null;
		for (int i = 0; i < Rows.size(); i++) {
			row = (Row) Rows.elementAt(i);
			for (int j = 0; j < Fields.size(); j++) {
				fd = (Field) Fields.elementAt(j);
				if (fd.FiledType == FieldAudoID) {
					row.valueList.setElementAt(String.valueOf(i + 1), j);
				}
			}
		}
	}

	// 增加
	public void addRow(Row row) {
		internalAddRow(row);
	}

	private void internalAddRow(Row row) {
		if (isCanAddRow()) {
			if (!allRows.contains(row)) {
				allRows.addElement(row);
				if (row.visible) {
					Rows.addElement(row);
					// 计算
					try {
						calField(row);
					} catch (Exception e) {
					}
					// 计算自动ID
					setAutoID();
					refreshDataControl();
				}
			}
		} else {
			System.out.println("Can not add row.");
		}
	}

	public void addRow(String[] stringArray) {
		internalAddRow(stringArray);
	}

	private void internalAddRow(String[] stringArray) {
		if (isCanAddRow()) {
			Row row = new Row();
			for (int i = 0; i < stringArray.length; i++) {
				row.setValueByFieldIndex(i, stringArray[i]);
			}
			addRow(row);
		} else {
			System.out.println("Can not add row.");
		}
	}

	public void deleteRow(Row row) {
		internalDeleteRow(row);
	}

	private void internalDeleteRow(Row row) {
		if (Rows.contains(row)) {
			Rows.removeElement(row);
			allRows.removeElement(row);
			if (RecNo > Rows.size() - 1) {
				RecNo--;
				refreshDataControl();
				Row srow = null;
				srow = getRow(RecNo);
				doDataChangeScroll(srow);
			}
			row = null;
			// ID
			setAutoID();
		}
	}

	public void deleteRow(int recNo) {
		internalDeleteRow(recNo);
	}

	private void internalDeleteRow(int recNo) {
		if ((recNo > -1) && (recNo < Rows.size())) {
			Row row = (Row) Rows.elementAt(recNo);
			Rows.removeElementAt(recNo);
			allRows.removeElement(row);
			row = null;
			if (RecNo > Rows.size() - 1) {
				RecNo--;
				refreshDataControl();
				Row srow = null;
				srow = getRow(RecNo);
				doDataChangeScroll(srow);
			}
			// ID
			setAutoID();
		}
	}

	public Row getRow() {
		return internalGetRow();
	}

	private Row internalGetRow() {
		if (RecNo > -1) {
			return (Row) Rows.elementAt(RecNo);
		} else {
			return null;
		}
	}

	public Row getRow(int recNo) {
		return internalGetRow(recNo);
	}

	private Row internalGetRow(int recNo) {
		if (Rows.size() > 0) {
			if ((recNo > -1) && (recNo < Rows.size())) {
				return (Row) Rows.elementAt(recNo);
			} else {
				return null;
			}
		} else {
			return null;
		}
	}

	public Row getRow(String fieldName, String fieldValue) {
		return internalGetRow(fieldName, fieldValue);
	}

	private Row internalGetRow(String fieldName, String fieldValue) {
		if (!Fields.contains(fieldName)) {
			return null;
		} else {
			int index = getFieldIndex(fieldName);
			Row result = null;
			if (index > -1) {
				for (int i = 0; i < Rows.size(); i++) {
					Row row = (Row) Rows.elementAt(i);
					if ((row.valueList.elementAt(index)).equals(fieldValue)) {
						result = row;
						break;
					}
				}
			}
			return result;
		}
	}

	private Row getDeltaRow(String fieldName, String fieldValue) {
		if (!deltaFields.contains(fieldName)) {
			return null;
		} else {
			int index = getFieldIndex(fieldName);
			Row result = null;
			if (index > -1) {
				for (int i = 0; i < deltaRows.size(); i++) {
					Row row = (Row) deltaRows.elementAt(i);
					if ((row.valueList.elementAt(index)).equals(fieldValue)) {
						result = row;
						break;
					}
				}
			}
			return result;
		}
	}

	// 处理计算字段
	private void calField(Row row) throws Exception {
		Field fd;
		for (int i = 0; i < Fields.size(); i++) {
			fd = (Field) Fields.elementAt(i);
			if (fd.CalField) {
				exp.setExpression(fd.CalExpression);
				Enumeration vars = exp.VarsList.keys();
				while (vars.hasMoreElements()) {
					String fieldName = (String) vars.nextElement();
					exp.setParamValue(fieldName, row
							.getValueByFieldName(fieldName));
				}
				// 计算
				try {
					double value = exp.calculate();
					row.setValueByFieldIndex(i, String.valueOf(value));
				} catch (Exception e) {
					throw new Exception(e.getMessage());
				}
			}
		}
	}

	// 对内容排序(暂时实现单字段排序,排序的时候要判断字段类型)
	public void sortDataSet(String fieldName, int sortType) {
		internalSortDataSet(fieldName, sortType);
		doDataChangeScroll(RecNo);
	}

	private void internalSortDataSet(String fieldName, int sortType) {
		// 新列
		Vector tempRows = new Vector();
		Enumeration list = Rows.elements();
		while (list.hasMoreElements()) {
			tempRows.addElement(list.nextElement());
		}
		Rows.removeAllElements();
		// 行
		Row currRow = null;
		// 列
		int fieldType = getField(fieldName).FiledType;
		int fieldID = getFieldIndex(fieldName);
		// 两个比较值
		String tmpVal = null;
		String currVal = null;
		Row tmpRow = null;
		while (!tempRows.isEmpty()) {
			tmpRow = (Row) tempRows.elementAt(0);
			tmpVal = tmpRow.getValueByFieldIndex(fieldID);
			for (int i = 1; i < tempRows.size(); i++) {
				currRow = (Row) tempRows.elementAt(i);
				currVal = currRow.getValueByFieldIndex(fieldID);
				if ((currVal == null) || (currVal.equals(""))) {
					continue;
				}
				if ((tmpVal == null) || (tmpVal.equals(""))) {
					tmpRow = currRow;
					tmpVal = currVal;
					continue;
				}
				switch (fieldType) {
				case FieldDouble:
				case FieldByte:
				case FieldFloat:
				case FieldInteger:
				case FieldLong:
				case FieldShort: {
					if (Double.parseDouble(currVal) > Double
							.parseDouble(tmpVal)) {
						tmpRow = currRow;
						tmpVal = currVal;
						continue;
					}
				}
				case FieldString:
				case FieldBoolean: {
					// 比较
					if (currVal.compareTo(tmpVal) > 0) {
						tmpRow = currRow;
						tmpVal = currVal;
						continue;
					}
				}
				}
			}
			tempRows.removeElement(tmpRow);
			if (Rows.isEmpty() || (sortType == sortDesc)) {
				Rows.addElement(tmpRow);
			} else if (sortType == sortAsc) {
				Rows.insertElementAt(tmpRow, 0);
			}
		}
		// 自动ID
		setAutoID();
		// 刷新
		refreshDataControl();
	}

	// 数据集合的装载
	public void fillDataSet(byte[] dataBuffer) {
		internalFillDataSet(dataBuffer);
	}

	private void internalFillDataSet(byte[] dataBuffer) {
		internalClearFields();
		ByteArrayInputStream bis = new ByteArrayInputStream(dataBuffer);
		DataInputStream dis = new DataInputStream(bis);
		try {
			// 字段区
			int FieldsLen = dis.readInt();
			byte[] fieldbyte = new byte[FieldsLen];
			dis.read(fieldbyte);
			ByteArrayInputStream fieldbis = new ByteArrayInputStream(fieldbyte);
			DataInputStream fielddis = new DataInputStream(fieldbis);
			while (fielddis.available() > 0) {
				// 读长度
				int fieldLen = fielddis.readByte();
				byte[] fb = new byte[fieldLen];
				if (fieldLen > 0) {
					fielddis.read(fb);
					Fields.addElement(new String(fb, "UTF-8"));
				}
			}
			fieldbis.close();
			fielddis.close();
			// 内容区
			int valuesLen = 0;
			while (dis.available() > 0) {
				valuesLen = dis.readInt();
				byte[] valuebyte = new byte[valuesLen];
				dis.read(valuebyte);
				ByteArrayInputStream valuebis = new ByteArrayInputStream(
						valuebyte);
				DataInputStream valuedis = new DataInputStream(valuebis);
				Vector valueVector = new Vector();
				while (valuedis.available() > 0) {
					// 读长度
					int valueLen = valuedis.readInt();
					byte[] fb = new byte[valueLen];
					if (valueLen > 0) {
						valuedis.read(fb);
						String fv = new String(fb, "UTF-8");
						valueVector.addElement(fv);
					} else {
						valueVector.addElement("");
					}
				}
				// 加到列表
				Rows.addElement(valueVector);
				// 关闭
				valuebis.close();
				valuedis.close();
			}
			if (Rows.size() > 0) {
				RecNo = 0;
				refreshDataControl();
				doDataChangeScroll(RecNo);
			}
		} catch (Exception e) {
		} finally {
			try {
				bis.close();
				dis.close();
			} catch (Exception e) {
			}
		}
		// ID
		setAutoID();
	}

	// 内容滚动事件
	public void setOnDataScrollChange(DataScrollChangeEvent onDataScrollChange) {
		this.onDataScrollChange = onDataScrollChange;
	}

	// 过滤
	public void setOnDataFilter(DataFilterEvent onDataFilter) {
		this.onDataFilter = onDataFilter;
	}
}
