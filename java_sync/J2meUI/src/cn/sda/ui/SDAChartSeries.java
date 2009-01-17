package cn.sda.ui;

import java.util.Enumeration;
import java.util.Random;
import java.util.Vector;

/**
 * 
 * @author Administrator
 */
public class SDAChartSeries {

	// 值类型
	public static final byte svNone = 0;
	public static final byte svValue = 1;
	public static final byte svPercent = 2;
	public static final byte svXValue = 3;
	public static final byte svXYValue = 4;
	public static final byte svXValuePercent = 5;
	public static final byte svLabel = 6;
	public static final byte svLabelValue = 7;
	public static final byte svLabelPercent = 8;
	// 图形类型
	public static final byte ctLine = 0;
	public static final byte ctPie = 1;
	public static final byte ctBar = 2;
	public static final byte ctPoint = 3;
	private String title = "";
	private boolean showMarks = false;
	private byte markValueType = svNone;
	private int marksBackColor = SDAConsts.clBtnFace;
	private int lineColor = SDAConsts.clBlack;
	private boolean showPoint = true;
	private int percentDecNum = 2;
	// 饼图图例宽度
	private int pieLegendWidth = 60;
	private boolean showPieLegend = true;
	// 图形类型
	private byte chartType = ctLine;
	// 数据存储
	protected Vector dataList = null;
	// 图表类
	private SDAChart chart = null;
	// 为点的颜色建立一个随机机制
	private Random rd = null;

	// 点数据类
	public class PointData {

		protected double xValue = 0;
		protected double yValue = 0;
		protected String label = "";
		protected String percent = "0.0";
		protected double dpercent = 0.0;
		int r = rd.nextInt();
		int g = rd.nextInt();
		int b = rd.nextInt();
		protected int color = r * g * b;

		public int getColor() {
			return color;
		}

		public String getLabel() {
			return label;
		}

		public String getPercent() {
			return percent;
		}

		public double getXValue() {
			return xValue;
		}

		public double getYValue() {
			return yValue;
		}
	}

	public SDAChartSeries() {
		dataList = new Vector();
		rd = new Random(255);
	}

	public int getMarksBackColor() {
		return marksBackColor;
	}

	public void setMarksBackColor(int marksBackColor) {
		this.marksBackColor = marksBackColor;
	}

	public boolean isShowMarks() {
		return showMarks;
	}

	public void setShowMarks(boolean showMarks) {
		this.showMarks = showMarks;
	}

	public String getTitle() {
		return title;
	}

	public void setTitle(String title) {
		this.title = title;
	}

	public byte getMarkValueType() {
		return markValueType;
	}

	public void setMarkValueType(byte markValueType) {
		this.markValueType = markValueType;
	}

	// 处理数据
	public void addData(double xValue, double yValue) {
		PointData pd = new PointData();
		pd.xValue = xValue;
		pd.yValue = yValue;
		dataList.addElement(pd);
	}

	public void addData(double xValue, double yValue, String label) {
		PointData pd = new PointData();
		pd.xValue = xValue;
		pd.yValue = yValue;
		pd.label = label;
		dataList.addElement(pd);
	}

	public void addData(double xValue, double yValue, int color) {
		PointData pd = new PointData();
		pd.xValue = xValue;
		pd.yValue = yValue;
		pd.color = color;
		dataList.addElement(pd);
	}

	public void addData(double xValue, double yValue, String label, int color) {
		PointData pd = new PointData();
		pd.xValue = xValue;
		pd.yValue = yValue;
		pd.color = color;
		pd.label = label;
		dataList.addElement(pd);
	}

	public void removeData(int index) {
		if (index > -1 && index < dataList.size()) {
			dataList.removeElementAt(index);
		}
	}

	public void clear() {
		dataList.removeAllElements();
	}

	// 读数据
	public PointData getData(int index) {
		PointData pd = null;
		if (index > -1 && index < dataList.size()) {
			pd = (PointData) dataList.elementAt(index);
		}
		return pd;
	}

	public Enumeration getDataList() {
		return dataList.elements();
	}

	public int getDataCount() {
		return dataList.size();
	}

	// 属性
	public int getLineColor() {
		return lineColor;
	}

	public void setLineColor(int lineColor) {
		this.lineColor = lineColor;
	}

	protected void setChart(SDAChart chart) {
		this.chart = chart;
	}

	public boolean isShowPoint() {
		return showPoint;
	}

	public void setShowPoint(boolean showPoint) {
		this.showPoint = showPoint;
	}

	public int getPercentDecNum() {
		return percentDecNum;
	}

	public void setPercentDecNum(int percentDecNum) {
		this.percentDecNum = percentDecNum;
	}

	public byte getChartType() {
		return chartType;
	}

	public void setChartType(byte chartType) {
		this.chartType = chartType;
	}

	public int getPieLegendWidth() {
		return pieLegendWidth;
	}

	public void setPieLegendWidth(int pieLegendWidth) {
		this.pieLegendWidth = pieLegendWidth;
	}

	public boolean isShowPieLegend() {
		return showPieLegend;
	}

	public void setShowPieLegend(boolean showPieLegend) {
		this.showPieLegend = showPieLegend;
	}

	// 计算百分比
	protected void calPercent() {
		// 总数
		double total = 0;
		PointData pd = null;
		for (int i = 0; i < dataList.size(); i++) {
			pd = (PointData) dataList.elementAt(i);
			total += Math.abs(pd.yValue);
		}
		// 计算
		double per = 0.0;
		String p1 = "0.0";
		for (int i = 0; i < dataList.size(); i++) {
			pd = (PointData) dataList.elementAt(i);
			per = Math.abs(pd.yValue) * 100 / total;
			pd.dpercent = per;
			p1 = String.valueOf(per);
			if ((p1.indexOf(".") > 0)
					&& (p1.length() - p1.indexOf(".") - 1 >= percentDecNum)) {
				pd.percent = p1.substring(0, p1.indexOf(".") + percentDecNum
						+ 1)
						+ "%";
			} else {
				pd.percent = p1 + "%";
			}
		}
	}
}
