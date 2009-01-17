package cn.sda.ui;

import java.util.Vector;
import javax.microedition.lcdui.Graphics;
import javax.microedition.lcdui.Font;

/**
 * 
 * @author Administrator
 */
public class SDAChart extends SDABaseControl {

	private int borderColor = SDAConsts.clBlack;
	private int scaleLineColor = SDAConsts.clFocusShadow;
	private String titleText = "";
	private int titleTextAlign = SDAConsts.alignCenter;
	private boolean showTitleText = false;
	private String footText = "";
	private int footTextAlign = SDAConsts.alignCenter;
	private boolean showFootText = false;
	private String axisText = "";
	private int axisTextAlign = SDAConsts.alignCenter;
	private boolean showAxisText = false;
	// 柱图宽度
	private int barChartWidth = 8;
	// 参考线颜色
	private int refLineColor = SDAConsts.clFocusShadow;
	private boolean showRefLine = true;
	// 列表(所有的series里面的x值都要各自排序)
	private Vector seriesList = null;
	// 坐标排序
	private Vector xValueList = null;
	private Vector yValueList = null;
	// Y最大值
	private double maxYInt = 0;
	// 最值记录
	private double maxXValue = 0;
	private double minXValue = 0;
	private double maxYValue = 0;
	private double minYValue = 0;
	// 实际像素开始位置和区域大小
	private int yStartPos = 0;
	private int xStartPos = 0;
	private int yLen = 0;
	private int xLen = 0;

	public SDAChart() {
		super();
		visible = true;
		width = 200;
		height = 160;
		backColor = SDAConsts.clWhite;
		seriesList = new Vector();
		xValueList = new Vector();
		yValueList = new Vector();
	}

	public void paint() {
		if (!IsCanPaint()) {
			return;
		}
		Graphics g = form.getGraphics();
		SetClip(g);
		g.setFont(getFont());
		// 变量
		int thisWidth = getWidth();
		int thisHeight = getHeight();
		// 背景
		if (!transparent) {
			g.setColor(backColor);
			fillRect(g, 0, 0, thisWidth, thisHeight);
		}
		// 外框
		g.setColor(borderColor);
		drawRect(g, 0, 0, thisWidth, thisHeight);
		// 画标尺
		paintScale(g, canPaintScale());
		// 画数据
		for (int i = 0; i < seriesList.size(); i++) {
			if (((SDAChartSeries) seriesList.elementAt(i)).getChartType() == SDAChartSeries.ctLine) {
				paintLineSeries(g, (SDAChartSeries) seriesList.elementAt(i));
			}
			if (((SDAChartSeries) seriesList.elementAt(i)).getChartType() == SDAChartSeries.ctBar) {
				paintBarSeries(g, (SDAChartSeries) seriesList.elementAt(i));
			}
			if (((SDAChartSeries) seriesList.elementAt(i)).getChartType() == SDAChartSeries.ctPie) {
				paintPieSeries(g, (SDAChartSeries) seriesList.elementAt(i));
			}
			if (((SDAChartSeries) seriesList.elementAt(i)).getChartType() == SDAChartSeries.ctPoint) {
				paintPointSeries(g, (SDAChartSeries) seriesList.elementAt(i));
			}
		}
		PaintChilds();
	}

	// 判断是否可以画标尺
	private boolean canPaintScale() {
		boolean result = true;
		for (int i = 0; i < seriesList.size(); i++) {
			if (((SDAChartSeries) seriesList.elementAt(i)).getChartType() == SDAChartSeries.ctPie) {
				result = false;
				break;
			}
		}
		return result;
	}

	// 排序
	private void sortValue() {
		// 各自Series排序(对x方向升序)
		SDAChartSeries series = null;
		SDAChartSeries.PointData pd = null;
		SDAChartSeries.PointData pd1 = null;
		int num = 0;
		for (int i = 0; i < seriesList.size(); i++) {
			series = (SDAChartSeries) seriesList.elementAt(i);
			num = series.getDataCount();
			for (int j = 0; j < num; j++) {
				pd = (SDAChartSeries.PointData) series.getData(j);
				for (int k = j + 1; k < num; k++) {
					pd1 = (SDAChartSeries.PointData) series.getData(k);
					if (pd.xValue > pd1.xValue) {
						series.dataList.setElementAt(pd, k);
						series.dataList.setElementAt(pd1, j);
						pd = (SDAChartSeries.PointData) series.getData(j);
					}
				}
			}
		}
		// 所有x方向排序(升序)
		xValueList.removeAllElements();
		for (int i = 0; i < seriesList.size(); i++) {
			series = (SDAChartSeries) seriesList.elementAt(i);
			for (int j = 0; j < series.getDataCount(); j++) {
				pd = (SDAChartSeries.PointData) series.getData(j);
				xValueList.addElement(pd);
			}
		}
		for (int i = 0; i < xValueList.size(); i++) {
			pd = (SDAChartSeries.PointData) xValueList.elementAt(i);
			for (int j = i + 1; j < xValueList.size(); j++) {
				pd1 = (SDAChartSeries.PointData) xValueList.elementAt(j);
				if (pd.xValue > pd1.xValue) {
					xValueList.setElementAt(pd, j);
					xValueList.setElementAt(pd1, i);
					pd = (SDAChartSeries.PointData) xValueList.elementAt(i);
				}
			}
		}
		// 所有y方向排序(升序)
		yValueList.removeAllElements();
		for (int i = 0; i < seriesList.size(); i++) {
			series = (SDAChartSeries) seriesList.elementAt(i);
			for (int j = 0; j < series.getDataCount(); j++) {
				pd = (SDAChartSeries.PointData) series.getData(j);
				yValueList.addElement(pd);
			}
		}
		for (int i = 0; i < yValueList.size(); i++) {
			pd = (SDAChartSeries.PointData) yValueList.elementAt(i);
			for (int j = i + 1; j < yValueList.size(); j++) {
				pd1 = (SDAChartSeries.PointData) yValueList.elementAt(j);
				if (pd.yValue > pd1.yValue) {
					yValueList.setElementAt(pd, j);
					yValueList.setElementAt(pd1, i);
					pd = (SDAChartSeries.PointData) yValueList.elementAt(i);
				}
			}
		}
	}

	// 乘方运算
	private double getMulValue(int miValue) {
		double result = 1;
		if (miValue > 0) {
			for (int i = 0; i < miValue; i++) {
				result *= 10;
			}
		}
		if (miValue < 0) {
			for (int i = 0; i > miValue; i--) {
				result /= 10;
			}
		}
		return result;
	}

	// 计算y方向标尺刻度
	private double calYScale() {
		Font ft = getFont();
		// 计算显示刻度(排序后)
		double maxValue = yValueList.size() > 0 ? ((SDAChartSeries.PointData) yValueList
				.elementAt(yValueList.size() - 1)).yValue
				: 0;
		double minValue = yValueList.size() > 0 ? ((SDAChartSeries.PointData) yValueList
				.elementAt(0)).yValue
				: 0;
		if (maxValue < 0) {
			maxValue = 0;
		}
		if (minValue > 0) {
			minValue = 0;
			// 判断是否可以容纳的下文字(注意两边的空间)
		}
		int sint = (showTitleText ? ft.getHeight() : 0)
				+ (showFootText ? ft.getHeight() : 0);
		// 首先判断1
		double max = (int) (maxValue / 1) * 1;
		double min = (int) (minValue / 1) * 1;
		maxYInt = max;
		// 两边空余一个
		int sideInt = (int) ((maxValue > max ? 1 : 0) + (minValue < min ? 1 : 0));
		// 所有的数量
		int allInt = (int) (max - min + sideInt);
		// 最小单元
		double minUnit = 1;
		if (ft.getHeight() * allInt >= getHeight() - sint) {
			// 分割稠密(稀化)
			for (int i = 0; i < 64; i++) {
				minUnit = getMulValue(i);
				max = (int) (maxValue / minUnit) * minUnit;
				min = (int) (minValue / minUnit) * minUnit;
				sideInt = (int) ((maxValue > max ? 1 : 0) + (minValue < min ? 1
						: 0));
				// 所有的数量
				allInt = (int) ((max - min + sideInt) / minUnit);
				if (ft.getHeight() * allInt >= getHeight() - sint) {
					minUnit = 2 * getMulValue(i);
					max = (int) (maxValue / minUnit) * minUnit;
					min = (int) (minValue / minUnit) * minUnit;
					sideInt = (int) ((maxValue > max ? 1 : 0) + (minValue < min ? 1
							: 0));
					// 所有的数量
					allInt = (int) ((max - min + sideInt) / minUnit);
					if (ft.getHeight() * allInt >= getHeight() - sint) {
						minUnit = 5 * getMulValue(i);
						max = (int) (maxValue / minUnit) * minUnit;
						min = (int) (minValue / minUnit) * minUnit;
						sideInt = (int) ((maxValue > max ? 1 : 0) + (minValue < min ? 1
								: 0));
						// 所有的数量
						allInt = (int) ((max - min + sideInt) / minUnit);
						if (ft.getHeight() * allInt >= getHeight() - sint) {
							continue;
						} else {
							break;
						}
					} else {
						break;
					}
				} else {
					break;
				}
			}
			// 获取到合适的
		} else {
			// 分割太稀(稠化)
			int iInt = 0;
			int mulInt = 1;
			for (int i = -1; i > -64; i--) {
				iInt = i;
				mulInt = 5;
				minUnit = 5 * getMulValue(i);
				max = (int) (maxValue / minUnit) * minUnit;
				min = (int) (minValue / minUnit) * minUnit;
				sideInt = (int) ((maxValue > max ? 1 : 0) + (minValue < min ? 1
						: 0));
				// 所有的数量
				allInt = (int) ((max - min + sideInt) / minUnit);
				if (ft.getHeight() * allInt < getHeight() - sint) {
					mulInt = 2;
					minUnit = 2 * getMulValue(i);
					max = (int) (maxValue / minUnit) * minUnit;
					min = (int) (minValue / minUnit) * minUnit;
					sideInt = (int) ((maxValue > max ? 1 : 0) + (minValue < min ? 1
							: 0));
					// 所有的数量
					allInt = (int) ((max - min + sideInt) / minUnit);
					if (ft.getHeight() * allInt < getHeight() - sint) {
						mulInt = 1;
						minUnit = getMulValue(i);
						max = (int) (maxValue / minUnit) * minUnit;
						min = (int) (minValue / minUnit) * minUnit;
						sideInt = (int) ((maxValue > max ? 1 : 0) + (minValue < min ? 1
								: 0));
						// 所有的数量
						allInt = (int) ((max - min + sideInt) / minUnit);
						if (ft.getHeight() * allInt < getHeight() - sint) {
							continue;
						} else {
							break;
						}
					} else {
						break;
					}
				} else {
					break;
				}
			}
			if (mulInt == 5) {
				mulInt = 1;
			}
			if (mulInt == 2) {
				mulInt = 5;
			}
			minUnit = mulInt * getMulValue(iInt + 1);
		}
		maxYValue = maxValue > max ? max + minUnit : max;
		minYValue = minValue < min ? min - minUnit : min;
		return minUnit;
	}

	// 计算x方向标尺刻度
	private double calXScale() {
		Font ft = getFont();
		// 计算显示刻度(排序后)
		double maxValue = xValueList.size() > 0 ? ((SDAChartSeries.PointData) xValueList
				.elementAt(xValueList.size() - 1)).xValue
				: 0;
		double minValue = xValueList.size() > 0 ? ((SDAChartSeries.PointData) xValueList
				.elementAt(0)).xValue
				: 0;
		// 判断是否可以容纳的下文字(注意两边的空间)
		int sint = (showAxisText ? ft.charWidth('x') : 0)
				+ (ft.stringWidth(String.valueOf(maxYInt)));
		// 首先判断1
		double max = (int) (maxValue / 1) * 1;
		double min = (int) (minValue / 1) * 1;
		// 两边空余一个
		int sideInt = (int) ((maxValue > max ? 1 : 0) + (minValue < min ? 1 : 0));
		// 所有的数量
		int allInt = (int) (max - min + sideInt);
		// 最小单元
		double minUnit = 1;
		if (ft.stringWidth(String.valueOf(max)) * allInt >= getWidth() - sint) {
			// 分割稠密(稀化)
			for (int i = 0; i < 64; i++) {
				minUnit = getMulValue(i);
				max = (int) (maxValue / minUnit) * minUnit;
				min = (int) (minValue / minUnit) * minUnit;
				sideInt = (int) ((maxValue > max ? 1 : 0) + (minValue < min ? 1
						: 0));
				// 所有的数量
				allInt = (int) ((max - min + sideInt) / minUnit);
				if (ft.stringWidth(String.valueOf(max)) * allInt >= getWidth()
						- sint) {
					minUnit = 2 * getMulValue(i);
					max = (int) (maxValue / minUnit) * minUnit;
					min = (int) (minValue / minUnit) * minUnit;
					sideInt = (int) ((maxValue > max ? 1 : 0) + (minValue < min ? 1
							: 0));
					// 所有的数量
					allInt = (int) ((max - min + sideInt) / minUnit);
					if (ft.stringWidth(String.valueOf(max)) * allInt >= getWidth()
							- sint) {
						minUnit = 5 * getMulValue(i);
						max = (int) (maxValue / minUnit) * minUnit;
						min = (int) (minValue / minUnit) * minUnit;
						sideInt = (int) ((maxValue > max ? 1 : 0) + (minValue < min ? 1
								: 0));
						// 所有的数量
						allInt = (int) ((max - min + sideInt) / minUnit);
						if (ft.stringWidth(String.valueOf(max)) * allInt >= getWidth()
								- sint) {
							continue;
						} else {
							break;
						}
					} else {
						break;
					}
				} else {
					break;
				}
			}
			// 获取到合适的
		} else {
			// 分割太稀(稠化)
			int iInt = 0;
			int mulInt = 1;
			for (int i = -1; i > -64; i--) {
				iInt = i;
				mulInt = 5;
				minUnit = 5 * getMulValue(i);
				max = (int) (maxValue / minUnit) * minUnit;
				min = (int) (minValue / minUnit) * minUnit;
				sideInt = (int) ((maxValue > max ? 1 : 0) + (minValue < min ? 1
						: 0));
				// 所有的数量
				allInt = (int) ((max - min + sideInt) / minUnit);
				if (ft.stringWidth(String.valueOf(max)) * allInt < getWidth()
						- sint) {
					mulInt = 2;
					minUnit = 2 * getMulValue(i);
					max = (int) (maxValue / minUnit) * minUnit;
					min = (int) (minValue / minUnit) * minUnit;
					sideInt = (int) ((maxValue > max ? 1 : 0) + (minValue < min ? 1
							: 0));
					// 所有的数量
					allInt = (int) ((max - min + sideInt) / minUnit);
					if (ft.stringWidth(String.valueOf(max)) * allInt < getWidth()
							- sint) {
						mulInt = 1;
						minUnit = getMulValue(i);
						max = (int) (maxValue / minUnit) * minUnit;
						min = (int) (minValue / minUnit) * minUnit;
						sideInt = (int) ((maxValue > max ? 1 : 0) + (minValue < min ? 1
								: 0));
						// 所有的数量
						allInt = (int) ((max - min + sideInt) / minUnit);
						if (ft.stringWidth(String.valueOf(max)) * allInt < getWidth()
								- sint) {
							continue;
						} else {
							break;
						}
					} else {
						break;
					}
				} else {
					break;
				}
			}
			if (mulInt == 5) {
				mulInt = 1;
			}
			if (mulInt == 2) {
				mulInt = 5;
			}
			minUnit = mulInt * getMulValue(iInt + 1);
		}
		maxXValue = maxValue > max ? max + minUnit : max;
		minXValue = minValue < min ? min - minUnit : min;
		return minUnit;
	}

	// 画标尺
	private void paintScale(Graphics g, boolean canPaint) {
		int thisWidth = getWidth();
		int thisHeight = getHeight();
		Font ft = getFont();
		// 排序
		sortValue();
		// Y方向
		double yScale = calYScale();
		// 画y刻度
		yLen = thisHeight
				- (showFootText ? 2 * ft.getHeight() + 4 : ft.getHeight() + 4)
				- (showTitleText ? ft.getHeight() : 0);
		int id = 0;
		double scaleValue = minYValue;
		// y值最大的宽度
		int maxYValueWidth = ft.stringWidth(String.valueOf(maxYValue)) > ft
				.stringWidth(String.valueOf(minYValue)) ? ft.stringWidth(String
				.valueOf(maxYValue)) : ft
				.stringWidth(String.valueOf(minYValue));
		int scaleLeft = maxYValueWidth + (showAxisText ? ft.getHeight() : 0);
		int scaleStart = thisHeight
				- (showFootText ? 2 * ft.getHeight() + 4 : ft.getHeight() + 4);
		int scalepos = 0;
		int selfLen = 0;
		if (canPaint) {
			while (scaleValue <= maxYValue) {
				scalepos = (int) (yLen * id / ((maxYValue - minYValue) / yScale));
				if (scaleStart - scalepos - ft.getHeight() / 2 + 1 >= 0) {
					selfLen = maxYValueWidth
							- ft.stringWidth(String.valueOf(scaleValue));
					drawString(g, String.valueOf(scaleValue), showAxisText ? ft
							.getHeight()
							+ selfLen : selfLen, scaleStart - scalepos
							- ft.getHeight() / 2 + 1);
				}
				if (showRefLine) {
					if (scaleValue != minYValue) {
						g.setColor(refLineColor);
						g.setStrokeStyle(Graphics.DOTTED);
						drawLine(g, scaleLeft, scaleStart - scalepos,
								thisWidth, scaleStart - scalepos);
						g.setColor(foreColor);
						g.setStrokeStyle(Graphics.SOLID);
					}
				}
				// 画刻度
				drawLine(g, scaleLeft, scaleStart - scalepos, scaleLeft + 4,
						scaleStart - scalepos);
				scaleValue += yScale;
				id++;
			}
		}
		// 竖线
		if (canPaint) {
			drawLine(g, scaleLeft + 4, showTitleText ? ft.getHeight() : 0,
					scaleLeft + 4, scaleStart);
		}
		yStartPos = scaleStart;
		// X方向
		double xScale = calXScale();
		// 画x刻度
		// ys = (int) ((thisWidth - (scaleLeft + 4)) / ((maxXValue - minXValue)
		// / xScale));
		xLen = thisWidth - (scaleLeft + 4);
		id = 0;
		scaleValue = minXValue;
		int scaleTop = showFootText ? 2 * ft.getHeight() + 4
				: ft.getHeight() + 4;
		// x值
		if (canPaint) {
			while (scaleValue <= maxXValue) {
				scalepos = (int) (xLen * id / ((maxXValue - minXValue) / xScale));
				if (scaleValue < maxXValue) {
					drawString(g, String.valueOf(scaleValue), scaleLeft + 4
							+ scalepos
							- (ft.stringWidth(String.valueOf(scaleValue))) / 2,
							thisHeight - scaleTop + 4);
				}
				// 画刻度
				if (showRefLine) {
					if (scaleValue != minXValue) {
						g.setColor(refLineColor);
						g.setStrokeStyle(Graphics.DOTTED);
						drawLine(g, scaleLeft + 4 + scalepos, thisHeight
								- scaleTop, scaleLeft + 4 + scalepos, yStartPos
								- yLen);
						g.setColor(foreColor);
						g.setStrokeStyle(Graphics.SOLID);
					}
				}
				drawLine(g, scaleLeft + 4 + scalepos, thisHeight - scaleTop,
						scaleLeft + 4 + scalepos, thisHeight - scaleTop + 4);
				scaleValue += xScale;
				id++;
			}
		}
		// 横线
		if (canPaint) {
			drawLine(g, scaleLeft + 4, getHeight() - scaleTop, getWidth(),
					getHeight() - scaleTop);
		}
		xStartPos = scaleLeft + 4;
		// xLen = thisWidth - xStartPos;
		SetClip(g);
		// 外框
		if (canPaint) {
			drawRect(g, xStartPos, yStartPos - yLen, xLen, yLen);
		}
		// 文字
		if (canPaint) {
			if (showAxisText) {
				int textWidth = axisText.length() * ft.getHeight();
				int fontHeight = ft.getHeight();
				int startPos = (getHeight() - textWidth) / 2 + 1;
				char ch = 0;
				for (int i = 0; i < axisText.length(); i++) {
					ch = axisText.charAt(i);
					drawString(g, String.valueOf(ch), (fontHeight - ft
							.charWidth(ch)) / 2, startPos + i * fontHeight);
				}
			}
		}
		if (canPaint) {
			if (showFootText) {
				int textWidth = ft.stringWidth(footText);
				int fontHeight = ft.getHeight();
				int startPos = (thisWidth - textWidth) / 2 + 1;
				drawString(g, footText, startPos, thisHeight - fontHeight);
			}
		}
		if (showTitleText) {
			int textWidth = ft.stringWidth(titleText);
			int startPos = (thisWidth - textWidth) / 2 + 1;
			drawString(g, titleText, startPos, 0);
		}
	}

	// 画series线
	private void paintLineSeries(Graphics g, SDAChartSeries series) {
		SDAChartSeries.PointData pd = null;
		SDAChartSeries.PointData pd1 = null;
		Font ft = getFont();
		int fontHeight = ft.getHeight();
		int x1 = 0, y1 = 0, x2 = 0, y2 = 0;
		int mack = series.getMarksBackColor();
		byte type = series.getMarkValueType();
		String value = "";
		series.calPercent();
		for (int i = 0; i < series.getDataCount() - 1; i++) {
			pd = series.getData(i);
			x1 = getXPos(pd.xValue);
			y1 = getYPos(pd.yValue);
			pd1 = series.getData(i + 1);
			x2 = getXPos(pd1.xValue);
			y2 = getYPos(pd1.yValue);
			// 线
			g.setColor(series.getLineColor());
			drawLine(g, x1, y1, x2, y2);
			// 画点
			if (series.isShowPoint()) {
				g.setColor(pd.color);
				fillRect(g, x1 - 2, y1 - 2, 4, 4);
				g.setColor(foreColor);
				drawRect(g, x1 - 2, y1 - 2, 4, 4);
			}
			// 画标识
			if (series.isShowMarks()) {
				if (type == SDAChartSeries.svNone) {
					value = "";
				}
				if (type == SDAChartSeries.svValue) {
					value = String.valueOf(pd.yValue);
				}
				if (type == SDAChartSeries.svXValue) {
					value = String.valueOf(pd.xValue);
				}
				if (type == SDAChartSeries.svPercent) {
					value = pd.percent;
				}
				if (type == SDAChartSeries.svXYValue) {
					value = pd.xValue + "/" + pd.yValue;
				}
				if (type == SDAChartSeries.svXValuePercent) {
					value = pd.xValue + "/" + pd.percent;
				}
				if (type == SDAChartSeries.svLabel) {
					value = pd.label;
				}
				if (type == SDAChartSeries.svLabelValue) {
					value = pd.label + "/" + pd.yValue;
				}
				if (type == SDAChartSeries.svLabelPercent) {
					value = pd.label + "/" + pd.percent;
				}
				if (value.length() > 0) {
					g.setColor(foreColor);
					drawLine(g, x1, y1, x1, y1 - 6);
					g.setColor(mack);
					fillRect(g, x1 - (ft.stringWidth(value)) / 2 + 1, y1 - 6
							- fontHeight, ft.stringWidth(value) + 3, fontHeight);
					g.setColor(foreColor);
					drawRect(g, x1 - (ft.stringWidth(value)) / 2 + 1, y1 - 6
							- fontHeight, ft.stringWidth(value) + 3, fontHeight);
					drawString(g, value, x1 - (ft.stringWidth(value)) / 2 + 3,
							y1 - 6 - fontHeight);
				}
			}
		}
		if (pd1 != null) {
			if (series.isShowPoint()) {
				g.setColor(pd1.color);
				fillRect(g, x2 - 2, y2 - 2, 4, 4);
				g.setColor(foreColor);
				drawRect(g, x2 - 2, y2 - 2, 4, 4);
			}
			if (series.isShowMarks()) {
				if (type == SDAChartSeries.svNone) {
					value = "";
				}
				if (type == SDAChartSeries.svValue) {
					value = String.valueOf(pd1.yValue);
				}
				if (type == SDAChartSeries.svXValue) {
					value = String.valueOf(pd1.xValue);
				}
				if (type == SDAChartSeries.svPercent) {
					value = pd1.percent;
				}
				if (type == SDAChartSeries.svXYValue) {
					value = pd1.xValue + "/" + pd1.yValue;
				}
				if (type == SDAChartSeries.svXValuePercent) {
					value = pd1.xValue + "/" + pd1.percent;
				}
				if (type == SDAChartSeries.svLabel) {
					value = pd1.label;
				}
				if (type == SDAChartSeries.svLabelValue) {
					value = pd1.label + "/" + pd1.yValue;
				}
				if (type == SDAChartSeries.svLabelPercent) {
					value = pd1.label + "/" + pd1.percent;
				}
				if (value.length() > 0) {
					g.setColor(foreColor);
					drawLine(g, x2, y2, x2, y2 - 6);
					g.setColor(mack);
					fillRect(g, x2 - (ft.stringWidth(value)) / 2 + 1, y2 - 6
							- fontHeight, ft.stringWidth(value) + 3, fontHeight);
					g.setColor(foreColor);
					drawRect(g, x2 - (ft.stringWidth(value)) / 2 + 1, y2 - 6
							- fontHeight, ft.stringWidth(value) + 3, fontHeight);
					drawString(g, value, x2 - (ft.stringWidth(value)) / 2 + 3,
							y2 - 6 - fontHeight);
				}
			}
		}
	}

	// 散点图
	private void paintPointSeries(Graphics g, SDAChartSeries series) {
		SDAChartSeries.PointData pd = null;
		Font ft = getFont();
		int fontHeight = ft.getHeight();
		int x1 = 0, y1 = 0;
		int mack = series.getMarksBackColor();
		byte type = series.getMarkValueType();
		String value = "";
		series.calPercent();
		for (int i = 0; i < series.getDataCount(); i++) {
			pd = series.getData(i);
			x1 = getXPos(pd.xValue);
			y1 = getYPos(pd.yValue);
			// 画点
			if (series.isShowPoint()) {
				g.setColor(pd.color);
				fillRect(g, x1 - 2, y1 - 2, 4, 4);
				g.setColor(foreColor);
				drawRect(g, x1 - 2, y1 - 2, 4, 4);
			}
			// 画标识
			if (series.isShowMarks()) {
				if (type == SDAChartSeries.svNone) {
					value = "";
				}
				if (type == SDAChartSeries.svValue) {
					value = String.valueOf(pd.yValue);
				}
				if (type == SDAChartSeries.svXValue) {
					value = String.valueOf(pd.xValue);
				}
				if (type == SDAChartSeries.svPercent) {
					value = pd.percent;
				}
				if (type == SDAChartSeries.svXYValue) {
					value = pd.xValue + "/" + pd.yValue;
				}
				if (type == SDAChartSeries.svXValuePercent) {
					value = pd.xValue + "/" + pd.percent;
				}
				if (type == SDAChartSeries.svLabel) {
					value = pd.label;
				}
				if (type == SDAChartSeries.svLabelValue) {
					value = pd.label + "/" + pd.yValue;
				}
				if (type == SDAChartSeries.svLabelPercent) {
					value = pd.label + "/" + pd.percent;
				}
				if (value.length() > 0) {
					g.setColor(foreColor);
					drawLine(g, x1, y1, x1, y1 - 6);
					g.setColor(mack);
					fillRect(g, x1 - (ft.stringWidth(value)) / 2 + 1, y1 - 6
							- fontHeight, ft.stringWidth(value) + 3, fontHeight);
					g.setColor(foreColor);
					drawRect(g, x1 - (ft.stringWidth(value)) / 2 + 1, y1 - 6
							- fontHeight, ft.stringWidth(value) + 3, fontHeight);
					drawString(g, value, x1 - (ft.stringWidth(value)) / 2 + 3,
							y1 - 6 - fontHeight);
				}
			}
		}
	}

	// 柱图
	private void paintBarSeries(Graphics g, SDAChartSeries series) {
		SDAChartSeries.PointData pd = null;
		Font ft = getFont();
		int fontHeight = ft.getHeight();
		int x1 = 0, y1 = 0, y2 = getYPos(0);
		int mack = series.getMarksBackColor();
		byte type = series.getMarkValueType();
		String value = "";
		series.calPercent();
		SetClip(g, xStartPos + 1, yStartPos - yLen + 1, xLen - 1, yLen - 1);
		for (int i = 0; i < series.getDataCount(); i++) {
			pd = series.getData(i);
			x1 = getXPos(pd.xValue);
			y1 = getYPos(pd.yValue);
			// 柱
			if (y1 > y2) {
				g.setColor(pd.color);
				fillRect(g, x1 - barChartWidth / 2, y2, barChartWidth, y1 - y2);
				g.setColor(foreColor);
				drawRect(g, x1 - barChartWidth / 2, y2, barChartWidth, y1 - y2);
			} else {
				g.setColor(pd.color);
				fillRect(g, x1 - barChartWidth / 2, y1, barChartWidth, y2 - y1);
				g.setColor(foreColor);
				drawRect(g, x1 - barChartWidth / 2, y1, barChartWidth, y2 - y1);
			}
			// 画标识
			if (series.isShowMarks()) {
				if (type == SDAChartSeries.svNone) {
					value = "";
				}
				if (type == SDAChartSeries.svValue) {
					value = String.valueOf(pd.yValue);
				}
				if (type == SDAChartSeries.svXValue) {
					value = String.valueOf(pd.xValue);
				}
				if (type == SDAChartSeries.svPercent) {
					value = pd.percent;
				}
				if (type == SDAChartSeries.svXYValue) {
					value = pd.xValue + "/" + pd.yValue;
				}
				if (type == SDAChartSeries.svXValuePercent) {
					value = pd.xValue + "/" + pd.percent;
				}
				if (type == SDAChartSeries.svLabel) {
					value = pd.label;
				}
				if (type == SDAChartSeries.svLabelValue) {
					value = pd.label + "/" + pd.yValue;
				}
				if (type == SDAChartSeries.svLabelPercent) {
					value = pd.label + "/" + pd.percent;
				}
				if (value.length() > 0) {
					if (y1 < y2) {
						g.setColor(foreColor);
						drawLine(g, x1, y1, x1, y1 - 6);
						g.setColor(mack);
						fillRect(g, x1 - (ft.stringWidth(value)) / 2 + 1, y1
								- 6 - fontHeight, ft.stringWidth(value) + 3,
								fontHeight);
						g.setColor(foreColor);
						drawRect(g, x1 - (ft.stringWidth(value)) / 2 + 1, y1
								- 6 - fontHeight, ft.stringWidth(value) + 3,
								fontHeight);
						drawString(g, value, x1 - (ft.stringWidth(value)) / 2
								+ 3, y1 - 6 - fontHeight);
					} else {
						g.setColor(foreColor);
						drawLine(g, x1, y1, x1, y1 + 6);
						g.setColor(mack);
						fillRect(g, x1 - (ft.stringWidth(value)) / 2 + 1,
								y1 + 6, ft.stringWidth(value) + 3, fontHeight);
						g.setColor(foreColor);
						drawRect(g, x1 - (ft.stringWidth(value)) / 2 + 1,
								y1 + 6, ft.stringWidth(value) + 3, fontHeight);
						drawString(g, value, x1 - (ft.stringWidth(value)) / 2
								+ 3, y1 + 6);
					}
				}
			}
		}
	}

	// 饼图
	private void paintPieSeries(Graphics g, SDAChartSeries series) {
		SDAChartSeries.PointData pd = null;
		Font ft = getFont();
		int fontHeight = ft.getHeight();
		int x1 = 0, y1 = 0;
		int mack = series.getMarksBackColor();
		byte type = series.getMarkValueType();
		String value = "";
		series.calPercent();
		int pieTop = showTitleText ? fontHeight + 8 : 8;
		int pieWidth = getWidth() - 8
				- (series.isShowPieLegend() ? series.getPieLegendWidth() : 0);
		int pieHeight = getHeight() - pieTop;
		SetClip(g, 1, pieTop - 2, width, height);
		double startAngle = 0;
		double angle = 0;
		g.setColor(foreColor);
		fillArc(g, pieWidth / 2 + 4, pieHeight / 2 + pieTop - 2, pieWidth + 2,
				pieHeight + 2, 0, 360);
		// 图例
		if (series.isShowPieLegend()) {
			g.setColor(foreColor);
			drawRect(g, pieWidth + 8, pieTop, series.getPieLegendWidth() - 1,
					series.getDataCount() * fontHeight);
		}
		for (int i = 0; i < series.getDataCount(); i++) {
			pd = series.getData(i);
			x1 = getXPos(pd.xValue);
			y1 = getYPos(pd.yValue);
			// 角度
			angle = 360 * pd.dpercent / 100;
			// 饼图
			g.setColor(pd.color);
			fillArc(g, pieWidth / 2 + 4, pieHeight / 2 + pieTop - 2, pieWidth,
					pieHeight, (int) startAngle, (int) angle + 1);
			startAngle += angle;
			// 画标识
			if (series.isShowPieLegend()) {
				if (type == SDAChartSeries.svNone) {
					value = "";
				}
				if (type == SDAChartSeries.svValue) {
					value = String.valueOf(pd.yValue);
				}
				if (type == SDAChartSeries.svXValue) {
					value = String.valueOf(pd.xValue);
				}
				if (type == SDAChartSeries.svPercent) {
					value = pd.percent;
				}
				if (type == SDAChartSeries.svXYValue) {
					value = pd.xValue + "/" + pd.yValue;
				}
				if (type == SDAChartSeries.svXValuePercent) {
					value = pd.xValue + "/" + pd.percent;
				}
				if (type == SDAChartSeries.svLabel) {
					value = pd.label;
				}
				if (type == SDAChartSeries.svLabelValue) {
					value = pd.label + "/" + pd.yValue;
				}
				if (type == SDAChartSeries.svLabelPercent) {
					value = pd.label + "/" + pd.percent;
				}
				if (value.length() > 0) {
					g.setColor(pd.color);
					fillRect(g, pieWidth + 10, pieTop + 1 + i * fontHeight
							+ (fontHeight - 8) / 2, 8, 8);
					g.setColor(foreColor);
					drawRect(g, pieWidth + 10, pieTop + 1 + i * fontHeight
							+ (fontHeight - 8) / 2, 8, 8);
					g.setColor(foreColor);
					drawString(g, value, pieWidth + 20, pieTop + 1 + i
							* fontHeight);
				}
			}
		}
	}

	// 根据点得到实际位置
	private int getXPos(double x) {
		return (int) (xStartPos + (x - minXValue) * xLen
				/ (maxXValue - minXValue));
	}

	private int getYPos(double y) {
		return (int) (yStartPos - (y - minYValue) * yLen
				/ (maxYValue - minYValue)) + 1;
	}

	// 图表增加删除
	public void addSeries(SDAChartSeries series) {
		if (!seriesList.contains(series)) {
			seriesList.addElement(series);
			series.setChart(this);
			repaintControl();
		}
	}

	public void removeSeries(SDAChartSeries series) {
		if (seriesList.contains(series)) {
			seriesList.removeElement(series);
			repaintControl();
		}
	}

	public void removeSeries(int index) {
		if (index > -1 && index < seriesList.size()) {
			seriesList.removeElementAt(index);
			repaintControl();
		}
	}

	// 属性
	public String getAxisText() {
		return axisText;
	}

	public void setAxisText(String axisText) {
		this.axisText = axisText;
	}

	public int getAxisTextAlign() {
		return axisTextAlign;
	}

	public void setAxisTextAlign(int axisTextAlign) {
		this.axisTextAlign = axisTextAlign;
	}

	public int getBorderColor() {
		return borderColor;
	}

	public void setBorderColor(int borderColor) {
		this.borderColor = borderColor;
	}

	public String getFootText() {
		return footText;
	}

	public void setFootText(String footText) {
		this.footText = footText;
	}

	public int getFootTextAlign() {
		return footTextAlign;
	}

	public void setFootTextAlign(int footTextAlign) {
		this.footTextAlign = footTextAlign;
	}

	public int getScaleLineColor() {
		return scaleLineColor;
	}

	public void setScaleLineColor(int scaleLineColor) {
		this.scaleLineColor = scaleLineColor;
	}

	public String getTitleText() {
		return titleText;
	}

	public void setTitleText(String titleText) {
		this.titleText = titleText;
	}

	public int getTitleTextAlign() {
		return titleTextAlign;
	}

	public void setTitleTextAlign(int titleTextAlign) {
		this.titleTextAlign = titleTextAlign;
	}

	public boolean isShowAxisText() {
		return showAxisText;
	}

	public void setShowAxisText(boolean showAxisText) {
		this.showAxisText = showAxisText;
	}

	public boolean isShowFootText() {
		return showFootText;
	}

	public void setShowFootText(boolean showFootText) {
		this.showFootText = showFootText;
	}

	public boolean isShowTitleText() {
		return showTitleText;
	}

	public void setShowTitleText(boolean showTitleText) {
		this.showTitleText = showTitleText;
	}

	public int getRefLineColor() {
		return refLineColor;
	}

	public void setRefLineColor(int refLineColor) {
		this.refLineColor = refLineColor;
	}

	public int getBarChartWidth() {
		return barChartWidth;
	}

	public void setBarChartWidth(int barChartWidth) {
		this.barChartWidth = barChartWidth;
	}
}
