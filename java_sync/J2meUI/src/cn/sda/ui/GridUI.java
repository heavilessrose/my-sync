package cn.sda.ui;

import cn.sda.enterprise.*;
import cn.sda.event.DrawGridCellEvent;
import cn.sda.event.GridCellClickEvent;
import cn.sda.event.KeybordEvent;
import cn.sda.event.PointerEvent;
import javax.microedition.lcdui.Graphics;
import java.util.Vector;

/**
 * @author not attributable
 * @version 1.0
 */
public class GridUI extends BaseControl {
    //数据集合
    private DataSetUI DataSet = null;
    private DataSetUI oldDataSet = null;
    //各列宽度
    private Vector ColumnsList;
    //边框
    private int borderStyle = ConstsUI.bsNone;

    //隐藏选定内容
    private boolean hideSelection = true;
    //滚动边框(多行)
    private int scrollBars = ConstsUI.srNone;
    //边框线颜色
    private int borderColor = ConstsUI.clBlack;
    //滚动条颜色
    private int FixedColor = ConstsUI.clBtnFace;
    //选中后字体颜色
    private int selectedFontColor = ConstsUI.clWhite;
    //选中行后，行的背景色
    private int selectedBackColor = ConstsUI.clFocusShadow;
    private int lostFocusSelectedBackColor = ConstsUI.clGray;
    //统计行背景色
    private int footRowColor = ConstsUI.clWhite;
    //内部变量
    //左边指针显示的宽度
    private int FixedWidth = 15;
    //表格头的高度,行高
    private int RowHeight = 15;
    //屏幕当前显示起始行
    private int StartRow = 0;
    //屏幕当前显示起始列
    private int StartCol = 0;
    //屏幕当前结束行
    private int EndRow = 0;
    //是否为最后一行
    private boolean IsEndRow = false;
    //是否为最后一列
    private boolean IsEndCol = false;
    //当前指针位置
    private int Cursor = 0;
    //滚动条宽度
    private int barwidth = 10;
    //缺省列宽
    private int DefaultColWidth = 50;
    //行数，行最大长度
    private int maxLineLenght = 0;
    //水平开始显示位置
    private int startLeft = 0;
    //变动前位置
    private int oldStartLine = 0;
    private int oldStartLeft = 0;
    //水平滚条位置
    private int HSLeft = 0;
    private int HSTop = 0;
    private int HSWidth = 0;
    private int HSHeight = 0;
    //垂直滚动条位置
    private int VSLeft = 0;
    private int VSTop = 0;
    private int VSWidth = 0;
    private int VSHeight = 0;
    //点击的滚动条的位置
    private int oldScrollPointx = 0;
    private int oldScrollPointy = 0;
    //按下滚动条
    private boolean isscrollbarpointdown = false;
    //记录按下的滚动条0:水平，1：垂直的
    private byte scrollbardownHV = 0;

    //统计内容显示
    private boolean showFootRow = false;
    //点的排序字段
    private String sortFieldName = "";
    //排序方式
    private int sortType = DataSetUI.sortAsc;
    //单元格单击事件
    private GridCellClickEvent OnGridCellClick = null;
    //绘单元格
    private DrawGridCellEvent onDrawGridCell = null;
    //单元格
    private Cell cell = null;
    //各列信息
    public class Column {
        //统计类型
        public static final int ftNone = 0;       //不显示
        public static final int ftAvg = 1;        //平均
        public static final int ftCount = 2;      //行数
        public static final int ftStaticText = 3; //固定文字
        public static final int ftSum = 4;        //求和
        public static final int ftFieldValue = 5; //当前值
        //标题文字对齐方式
        private int titleAlignType = ConstsUI.alignLeft;
        //内容文字对齐方式
        private int dataAlignType = ConstsUI.alignLeft;
        //统计内容对齐方式
        private int footAlignType = ConstsUI.alignLeft;
        //统计内容格式
        private String footDisplayFormat = "";
        //字段标题
        private String Title = "";
        //字段名
        private String FieldName = "";
        //列宽度
        private int ColWidth = 60;
        //格式表达式(主要针对浮点数)
        private String DisplayFormat = "";
        //统计内容
        private String footValue = "";
        //统计类型
        private int footType = ftNone;
        //构造
        public Column() {
        }

        public Column(String fieldName) {
            this.FieldName = fieldName;
        }

        public Column(String fieldName, String title) {
            this.FieldName = fieldName;
            this.Title = title;
        }

        public Column(String fieldName, String title, int colWidth) {
            this.FieldName = fieldName;
            this.Title = title;
            this.ColWidth = colWidth;
        }

        public Column(String fieldName, String title, int colWidth, String displayFormat) {
            this.FieldName = fieldName;
            this.Title = title;
            this.ColWidth = colWidth;
            this.DisplayFormat = displayFormat;
        }

        public int getColWidth() {
            return ColWidth;
        }

        public void setColWidth(int ColWidth) {
            this.ColWidth = ColWidth;
        }

        public String getDisplayFormat() {
            return DisplayFormat;
        }

        public void setDisplayFormat(String DisplayFormat) {
            this.DisplayFormat = DisplayFormat;
        }

        public String getFieldName() {
            return FieldName;
        }

        public void setFieldName(String FieldName) {
            this.FieldName = FieldName;
        }

        public String getTitle() {
            return Title;
        }

        public void setTitle(String Title) {
            this.Title = Title;
        }

        public int getFootType() {
            return footType;
        }

        public void setFootType(int footType) {
            this.footType = footType;
        }

        public String getFootValue() {
            return footValue;
        }

        public void setFootValue(String footValue) {
            this.footValue = footValue;
        }

        public int getDataAlignType() {
            return dataAlignType;
        }

        public void setDataAlignType(int dataAlignType) {
            this.dataAlignType = dataAlignType;
        }

        public int getTitleAlignType() {
            return titleAlignType;
        }

        public void setTitleAlignType(int titleAlignType) {
            this.titleAlignType = titleAlignType;
        }

        public int getFootAlignType() {
            return footAlignType;
        }

        public void setFootAlignType(int footAlignType) {
            this.footAlignType = footAlignType;
        }

        public String getFootDisplayFormat() {
            return footDisplayFormat;
        }

        public void setFootDisplayFormat(String footDisplayFormat) {
            this.footDisplayFormat = footDisplayFormat;
        }
    }

    //对列的处理
    //增加
    private void internalAddColumn(Column newColumn) {
        if (!ColumnsList.contains(newColumn)) {
            ColumnsList.addElement(newColumn);
        }
    }

    public void addColumn(Column newColumn) {
        internalAddColumn(newColumn);
    }

    private Column internalAddColumn() {
        Column cl = new Column();
        ColumnsList.addElement(cl);
        return cl;
    }

    public Column addColumn() {
        return internalAddColumn();
    }
    //构造
    public Column newColumn() {
        return new Column();
    }

    public Column newColumn(String fieldName) {
        return new Column(fieldName);
    }

    public Column newColumn(String fieldName, String title) {
        return new Column(fieldName, title);
    }

    public Column newColumn(String fieldName, String title, int colWidth) {
        return new Column(fieldName, title, colWidth);
    }

    public Column newColumn(String fieldName, String title, int colWidth, String displayFormat) {
        return new Column(fieldName, title, colWidth, displayFormat);
    }
    //删除
    private void internalDeleteColumn(int columnIndex) {
        if ((columnIndex > -1) && (columnIndex < ColumnsList.size())) {
            ColumnsList.removeElementAt(columnIndex);
        }
    }

    public void deleteColumn(int columnIndex) {
        internalDeleteColumn(columnIndex);
    }

    private void internalDeleteColumn(String columnFieldName) {
        Column cl = null;
        for (int i = 0; i < ColumnsList.size(); i++) {
            cl = (Column) ColumnsList.elementAt(i);
            if (cl.FieldName.equals(columnFieldName)) {
                ColumnsList.removeElementAt(i);
                break;
            }
        }
    }

    public void deleteColumn(String columnFieldName) {
        internalDeleteColumn(columnFieldName);
    }
    //获取
    public int getColumnsCount() {
        return ColumnsList.size();
    }

    private Column internalGetColumn(int columnIndex) {
        Column result = null;
        if ((columnIndex > -1) && (columnIndex < ColumnsList.size())) {
            result = (Column) ColumnsList.elementAt(columnIndex);
        }
        return result;
    }

    public Column getColumn(int columnIndex) {
        return internalGetColumn(columnIndex);
    }

    private Column internalGetColumn(String columnFieldName) {
        Column cl = null;
        for (int i = 0; i < ColumnsList.size(); i++) {
            cl = (Column) ColumnsList.elementAt(i);
            if (cl.FieldName.equals(columnFieldName)) {
                break;
            }
        }
        return cl;
    }

    public Column getColumn(String columnFieldName) {
        return internalGetColumn(columnFieldName);
    }

    //单元格
    public class Cell {

        private int foreColor = 0;
        private int backColor = 0;
        private String fieldValue = "";
        private String fieldName = "";
        private int rowIndex = 0;

        public int getBackColor() {
            return backColor;
        }

        public void setBackColor(int backColor) {
            this.backColor = backColor;
        }

        public int getForeColor() {
            return foreColor;
        }

        public void setForeColor(int foreColor) {
            this.foreColor = foreColor;
        }

        public String getFieldName() {
            return fieldName;
        }

        public String getFieldValue() {
            return fieldValue;
        }

        public void setFieldValue(String fieldValue) {
            this.fieldValue = fieldValue;
        }

        public int getRowIndex() {
            return rowIndex;
        }
    }

    public GridUI() {
        super();
        internalSDAGrid();
    }

    private void internalSDAGrid() {

        setVisible(true);
        //各列
        ColumnsList = new Vector();
        setLeft(0);
        setTop(0);
        setBorderStyle(ConstsUI.bsFixedSingle);
        super.setWidth(60);
        //高度根据字体高度来
        setHeight(getFont().getHeight() + 4);
        super.setBackColor(ConstsUI.clWhite);
        //单元格
        cell = new Cell();
        this.onPointerPressed = new PointerEvent() {

            public void Event(BaseControl ctrl, int x, int y) {
                doPointerPressed(x, y);
            }
        };
        this.onPointerReleased = new PointerEvent() {

            public void Event(BaseControl ctrl, int x, int y) {
                doPointerReleased(x, y);
            }
        };
        this.onKeyUp = new KeybordEvent() {

            public void Event(BaseControl ctrl, int keyCode) {
                doKeyUp(keyCode);
            }
        };
    }
    //画
    public void paint() {
        internalPaint();
        PaintChilds();
    }

    private void internalPaint() {
        if (!IsCanPaint()) {
            return;
        }
        Graphics g = form.getGraphics();
        //可视下才画
        if (isVisible()) {
            //初始化引用变量
            int thisWidth = getWidth();
            int thisHeight = getHeight();
            int HBarWidth = getHBarWidth();
            int VBarHeight = getVBarHeight();
            int rowHeight = getRowHeight();
            maxLineLenght = getAllWidth();
            //对齐方式处理后，画组件
            //设置字体
            g.setFont(getFont());
            SetClip(g);
            //外框
            if (!isTransparent()) {
                g.setColor(backColor);
                fillRect(g, 0, 0, thisWidth, thisHeight);
            }
            //fixed           
            g.setColor(FixedColor);
            fillRect(g, 0, 0, FixedWidth, thisHeight);
            fillRect(g, 0, 0, thisWidth, rowHeight);
            g.setColor(borderColor);
            drawRect(g, 0, 0, FixedWidth, thisHeight);
            drawRect(g, 0, 0, thisWidth, rowHeight);
            //是否显示统计行
            int footheight = showFootRow ? rowHeight : 0;
            //画横线，结合行高
            int H = rowHeight;
            if (DataSet != null) {
                while (H < thisHeight - RowHeight - footheight) {
                    H += rowHeight;
                    drawLine(g, 0, 0 + H, thisWidth, H);
                }
            }
            //画统计行
            if (showFootRow) {
                g.setColor(FixedColor);
                fillRect(g, 0, thisHeight - barwidth - rowHeight, FixedWidth, rowHeight);
                g.setColor(footRowColor);
                fillRect(g, FixedWidth + 1, thisHeight - barwidth - rowHeight, thisWidth, rowHeight);
                g.setColor(borderColor);
                drawRect(g, 0, thisHeight - barwidth - rowHeight, thisWidth, rowHeight);
                drawRect(g, 0, thisHeight - barwidth - rowHeight, FixedWidth, rowHeight);
            }
            //边框
            if (borderStyle == ConstsUI.bsFixed3D) {
                g.setColor(ConstsUI.clGray);
                drawLine(g, 0, 0, thisWidth, 0);
                drawLine(g, 0, 0, 0, thisHeight);
                g.setColor(ConstsUI.clBlack);
                drawLine(g, 1, 1, thisWidth - 1, 1);
                drawLine(g, 1, 1, 1, thisHeight - 1);
                g.setColor(ConstsUI.clBtnFace);
                drawLine(g, 2, thisHeight - 1, thisWidth - 2, thisHeight - 1);
                drawLine(g, thisWidth - 1, 2, thisWidth - 1, thisHeight - 1);
                g.setColor(ConstsUI.clWhite);
                drawLine(g, 1, thisHeight, thisWidth, thisHeight);
                drawLine(g, thisWidth, 1, thisWidth, thisHeight);
            }
            if (borderStyle == ConstsUI.bsFixedSingle) {
                //边框
                g.setColor(this.getBorderColor());
                drawRect(g, 0, 0, thisWidth, thisHeight);
            }
            //如果没数据就停止
            if (DataSet.getRecordCount() == 0) {
                DataSet.RecNo = 0;
            }
            //Title
            int V = FixedWidth;
            Column ct;
            //输出区域
            int sWidth = 0;
            int sHeight = 0;
            if (DataSet != null) {
                Cursor = DataSet.RecNo;
                if (StartRow > Cursor) {
                    StartRow = Cursor;
                }
                int maxRowCount = getMaxRowsNum();
                if (StartRow + maxRowCount - 1 < Cursor) {
                    StartRow = Cursor - maxRowCount + 1;
                }
                for (int i = StartCol; i < getColumnsCount(); i++) {
                    ct = getColumn(i);
                    if (V + ct.ColWidth > thisWidth) {
                        int imwidth = thisWidth - V;
                        sWidth = imwidth - 1;
                        sHeight = rowHeight - 1;
                    } else {
                        sWidth = ct.ColWidth - 1;
                        sHeight = rowHeight - 1;
                    }
                    g.setColor(FixedColor);
                    SetClip(g, V + 1, 1, sWidth, sHeight);
                    fillRect(g, V + 1, 1, sWidth, sHeight);
                    g.setColor(foreColor);
                    if (ct.titleAlignType == ConstsUI.alignLeft) {
                        drawString(g, ct.Title, V + 1, 1);
                    } else if (ct.titleAlignType == ConstsUI.alignCenter) {
                        drawString(g, ct.Title, V + 1 + (sWidth - getFont().stringWidth(ct.Title)) / 2, 1);
                    } else {
                        drawString(g, ct.Title, V + 1 + sWidth - getFont().stringWidth(ct.Title), 1);
                    }
                    //画排序符号
                    if (ct.FieldName.equals(sortFieldName)) {
                        g.setColor(borderColor);
                        if (sortType == DataSetUI.sortAsc) {
                            //升序
                            fillTriangle(g, V + ct.ColWidth - 5, rowHeight / 2 - 2,
                                    V + ct.ColWidth - 9, rowHeight / 2 + 2,
                                    V + ct.ColWidth - 1, rowHeight / 2 + 2);
                        } else {
                            fillTriangle(g, V + ct.ColWidth - 9, rowHeight / 2 - 2,
                                    V + ct.ColWidth - 1, rowHeight / 2 - 2, V + ct.ColWidth - 5, rowHeight / 2 + 2);
                        }
                    }
                    //统计字段
                    if (showFootRow) {
                        //列统计信息
                        SetClip(g, V + 1, thisHeight - barwidth - rowHeight, sWidth, sHeight);
                        switch (ct.footType) {
                            case Column.ftNone: {
                                //不显示任何内容
                                ct.footValue = "";
                                break;
                            }
                            case Column.ftCount: {
                                //行数统计
                                if (!ct.footDisplayFormat.equals("")) {
                                    ct.footValue = Tools.formatFloat(ct.footDisplayFormat, DataSet.getRecordCount());
                                } else {
                                    ct.footValue = String.valueOf(DataSet.getRecordCount());
                                }
                                break;
                            }
                            case Column.ftStaticText: {
                                //显示已经设置的内容
                                break;
                            }
                            case Column.ftFieldValue: {
                                //显示当前行的内容
                                if (!ct.footDisplayFormat.equals("")) {
                                    ct.footValue = Tools.formatFloat(ct.footDisplayFormat, Double.parseDouble(DataSet.getRow().getValueByFieldName(ct.FieldName)));
                                } else {
                                    ct.footValue = DataSet.getRow().getValueByFieldName(ct.FieldName);
                                }
                                break;
                            }
                            case Column.ftAvg: {
                                //计算平均值
                                double avg = 0.0;
                                String st;
                                for (int j = 0; j < DataSet.getRecordCount(); j++) {
                                    st = DataSet.getRow(j).getValueByFieldName(ct.FieldName);
                                    if ((st == null) || (st.equals(""))) {
                                        st = "0";
                                    }
                                    avg += Double.parseDouble(st);
                                }
                                if (!ct.footDisplayFormat.equals("")) {
                                    if (DataSet.getRecordCount() > 0) {
                                        ct.footValue = Tools.formatFloat(ct.footDisplayFormat, avg / DataSet.getRecordCount());
                                    } else {
                                        ct.footValue = "";
                                    }
                                } else {
                                    ct.footValue = String.valueOf(avg / DataSet.getRecordCount());
                                }
                                break;
                            }
                            case Column.ftSum: {
                                //汇总
                                double sum = 0.0;
                                String st;
                                for (int j = 0; j < DataSet.getRecordCount(); j++) {
                                    st = DataSet.getRow(j).getValueByFieldName(ct.FieldName);
                                    if (st == null) {
                                        st = "0";
                                    }
                                    sum += Double.parseDouble(st);
                                }
                                if (!ct.footDisplayFormat.equals("")) {
                                    ct.footValue = Tools.formatFloat(ct.footDisplayFormat, sum);
                                } else {
                                    ct.footValue = String.valueOf(sum);
                                }
                                break;
                            }
                        }
                        if (ct.footAlignType == ConstsUI.alignLeft) {
                            drawString(g, ct.footValue, V + 1, thisHeight - barwidth - rowHeight + 1);
                        } else if (ct.footAlignType == ConstsUI.alignCenter) {
                            drawString(g, ct.footValue, V + 1 + (sWidth - getFont().stringWidth(ct.footValue)) / 2, thisHeight - barwidth - rowHeight + 1);
                        } else {
                            drawString(g, ct.footValue, V + 1 + sWidth - getFont().stringWidth(ct.footValue), thisHeight - barwidth - rowHeight + 1);
                        }
                    }

                    SetClip(g);
                    V += ct.ColWidth;
                    //画线
                    drawLine(g, V, 0, V, thisHeight);
                    if (V > thisWidth) {
                        IsEndCol = false;
                        break;
                    } else {
                        IsEndCol = true;
                    }
                }
                //绘数据
                DataSetUI.Row row;
                for (int i = StartRow;
                        i < DataSet.getRecordCount();
                        i++) {
                    V = FixedWidth;
                    row = DataSet.getRow(i);
                    SetClip(g, V, (i - StartRow + 1) * rowHeight, thisWidth - V, rowHeight);
                    if (Cursor == i) {
                        g.setColor(selectedBackColor);
                    } else {
                        g.setColor(backColor);
                    }
                    fillRect(g, V, (i - StartRow + 1) * rowHeight, thisWidth - V, rowHeight);
                    g.setColor(borderColor);
                    drawRect(g, V, (i - StartRow + 1) * rowHeight, thisWidth - V, rowHeight);
                    for (int j = StartCol;
                            j < getColumnsCount();
                            j++) {
                        ct = getColumn(j);
                        //从当前起始列开始画
                        int imw;
                        int imh;
                        if (V + ct.ColWidth > thisWidth) {
                            imw = thisWidth - V;
                        } else {
                            imw = ct.ColWidth;
                        }
                        imh = rowHeight;
                        String drawstr = row.getValueByFieldName(ct.FieldName);
                        if (drawstr == null) {
                            drawstr = "";
                        }
                        //格式化
                        if (!ct.DisplayFormat.equals("")) {
                            if (drawstr.equals("")) {
                                drawstr = "0";
                            }
                            drawstr = Tools.formatFloat(ct.DisplayFormat, Double.parseDouble(drawstr));
                        }
                        cell.fieldValue = drawstr;
                        cell.fieldName = ct.FieldName;
                        cell.rowIndex = i;
                        //cell.backColor
                        if (Cursor == i) {
                            if (isFoucsed()) {
                                cell.backColor = selectedBackColor;
                                cell.foreColor = selectedFontColor;
                            } else {
                                cell.backColor = lostFocusSelectedBackColor;
                                cell.foreColor = selectedFontColor;
                            }
                        } else {
                            cell.backColor = backColor;
                            cell.foreColor = foreColor;
                            //////////事件
                            if (this.onDrawGridCell != null) {
                                onDrawGridCell.Event(cell);
                            }
                        }
                        g.setColor(cell.backColor);
                        fillRect(g, V, (i - StartRow + 1) * rowHeight, imw, imh);
                        g.setColor(borderColor);
                        drawRect(g, V, (i - StartRow + 1) * rowHeight, imw, imh);
                        ///////////////
                        g.setColor(cell.foreColor);
                        if (ct.dataAlignType == ConstsUI.alignLeft) {
                            drawString(g, drawstr, V + 2, (i - StartRow + 1) * rowHeight + 1);
                        } else if (ct.dataAlignType == ConstsUI.alignCenter) {
                            drawString(g, drawstr, V + 1 + (ct.ColWidth - getFont().stringWidth(drawstr)) / 2, (i - StartRow + 1) * rowHeight + 1);
                        } else {
                            drawString(g, drawstr, V + 1 + ct.ColWidth - getFont().stringWidth(drawstr) - 1, (i - StartRow + 1) * rowHeight + 1);
                        }
                        //向后移动
                        V += ct.ColWidth;
                        //判断
                        if (V > thisWidth) {
                            break;
                        }
                    }
                    if ((i - StartRow + 2) * rowHeight > thisHeight - barwidth - footheight - rowHeight) {
                        if (i == DataSet.getRecordCount() - 1) {
                            IsEndRow = true;
                            EndRow = i;
                        } else {
                            IsEndRow = false;
                            EndRow = i;
                            break;
                        }
                    } else {
                        IsEndRow = true;
                        EndRow = i;
                    }
                }
                //调整指针
                if (Cursor < StartRow) {
                    Cursor = StartRow;
                    DataSet.doDataChangeScroll(Cursor);
                }
                if (Cursor > EndRow) {
                    Cursor = EndRow;
                    DataSet.doDataChangeScroll(Cursor);
                }
                //指针,画三角
                int Curpos = Cursor - StartRow;
                SetClip(g, 0, rowHeight * (Curpos + 1), FixedWidth, rowHeight * (Curpos + 2));
                g.setColor(foreColor);
                fillTriangle(g, FixedWidth / 2 - 2, rowHeight * (Curpos + 1) + rowHeight / 2 - 4,
                        FixedWidth / 2 - 2, rowHeight * (Curpos + 1) + rowHeight / 2 + 4,
                        FixedWidth / 2 + 2, rowHeight * (Curpos + 1) + rowHeight / 2);
            }
            //滚动条
            if (borderStyle == ConstsUI.bsFixed3D) {
                SetClip(g, 2, 2, thisWidth - 2, thisHeight - 2);
            } else {
                SetClip(g, 1, 1, thisWidth - 1, thisHeight - 1);
            }
            if ((scrollBars == ConstsUI.srHorizontal) || (scrollBars == ConstsUI.srBoth)) {
                g.setColor(backColor);
                fillRect(g, 0, thisHeight - barwidth, thisWidth, barwidth);
                //水平
                g.setColor(FixedColor);
                fillRect(g, 0, thisHeight - barwidth, barwidth, barwidth);
                fillRect(g, HBarWidth - barwidth, thisHeight - barwidth, barwidth, barwidth);
                g.setColor(getBorderColor());
                drawRect(g, 0, thisHeight - barwidth, barwidth, barwidth);
                drawRect(g, HBarWidth - barwidth, thisHeight - barwidth, barwidth, barwidth);
                //滚动块
                g.setColor(backColor);
                fillRect(g, barwidth, thisHeight - barwidth, HBarWidth - 2 * barwidth, barwidth);
                g.setColor(this.getBorderColor());
                drawRect(g, barwidth, thisHeight - barwidth, HBarWidth - 2 * barwidth, barwidth);
                //块大小
                int pwidth = HBarWidth - 2 * barwidth;

                if (maxLineLenght > HBarWidth - 4) {
                    pwidth = ((HBarWidth - 4) * (HBarWidth - 2 * barwidth)) / maxLineLenght;
                }
                startLeft = getForeWidth();
                //计算块位置
                int ppos = 0;
                if (maxLineLenght > 0) {
                    ppos = (startLeft * (HBarWidth - 2 * barwidth)) / maxLineLenght;
                }
                //修正位置
                if (ppos + pwidth > HBarWidth - 2 * barwidth) {
                    ppos = HBarWidth - 2 * barwidth - pwidth;
                }
                if (maxLineLenght - startLeft - (HBarWidth - 4) < getFont().charWidth('x')) {
                    ppos = HBarWidth - 2 * barwidth - pwidth;
                }
                //画块
                g.setColor(FixedColor);
                fillRect(g, barwidth + ppos, thisHeight - barwidth, pwidth, barwidth);
                g.setColor(getBorderColor());
                drawRect(g, barwidth + ppos, thisHeight - barwidth, pwidth, barwidth);
                //记录大小
                HSLeft = barwidth + ppos;
                HSTop = thisHeight - barwidth;
                HSWidth = pwidth;
                HSHeight = barwidth;
                //画块上的线
                int tpos = HSWidth / 2 + HSLeft;
                drawRect(g, tpos, HSTop + 2, 0, HSHeight - 4);
                if (tpos - 2 > HSLeft) {
                    drawRect(g, tpos - 2, HSTop + 2, 0, HSHeight - 4);
                }
                if (tpos + 2 < HSLeft + HSWidth) {
                    drawRect(g, tpos + 2, HSTop + 2, 0, HSHeight - 4);
                }
                //三角
                g.setColor(getBorderColor());
                fillTriangle(g, barwidth / 2 - 2, thisHeight - barwidth / 2,
                        barwidth / 2 + 2, thisHeight - barwidth / 2 - 4,
                        barwidth / 2 + 2, thisHeight - barwidth / 2 + 4);
                fillTriangle(g, HBarWidth - barwidth + barwidth / 2 + 2, thisHeight - barwidth / 2,
                        HBarWidth - barwidth + barwidth / 2 - 2, thisHeight - barwidth / 2 - 4,
                        HBarWidth - barwidth + barwidth / 2 - 2, thisHeight - barwidth / 2 + 4);
            }
            if ((scrollBars == ConstsUI.srVertical) || (scrollBars == ConstsUI.srBoth)) {
                //垂直
                g.setColor(FixedColor);
                fillRect(g, thisWidth - barwidth, 0, barwidth, barwidth);
                fillRect(g, thisWidth - barwidth, VBarHeight - barwidth, barwidth, barwidth);
                g.setColor(getBorderColor());
                drawRect(g, thisWidth - barwidth, 0, barwidth, barwidth);
                drawRect(g, thisWidth - barwidth, VBarHeight - barwidth, barwidth, barwidth);
                //滚动块
                g.setColor(backColor);
                fillRect(g, thisWidth - barwidth, barwidth, barwidth, VBarHeight - 2 * barwidth);
                g.setColor(getBorderColor());
                drawRect(g, thisWidth - barwidth, barwidth, barwidth, VBarHeight - 2 * barwidth);
                //块大小
                int pheight = VBarHeight - 2 * barwidth;
                int allHeight = getAllHeight();
                int valwidth = (VBarHeight - footheight - rowHeight) / rowHeight * rowHeight;
                if (allHeight > valwidth) {
                    pheight = (valwidth * (VBarHeight - 2 * barwidth) / allHeight);
                }
                //计算块位置
                int ppos = 0;
                if (allHeight > 0) {
                    ppos = (StartRow * getRowHeight() * (VBarHeight - 2 * barwidth)) / allHeight;
                }
                //修正位置
                if (ppos + pheight > VBarHeight - 2 * barwidth) {
                    ppos = VBarHeight - 2 * barwidth - pheight;
                }
                //画块
                g.setColor(FixedColor);
                fillRect(g, thisWidth - barwidth, barwidth + ppos, barwidth, pheight);
                g.setColor(getBorderColor());
                drawRect(g, thisWidth - barwidth, barwidth + ppos, barwidth, pheight);
                //记录大小
                VSLeft = thisWidth - barwidth;
                VSTop = barwidth + ppos;
                VSWidth = barwidth;
                VSHeight = pheight;
                //画块上的线
                int tpos = VSHeight / 2 + VSTop;
                drawRect(g, VSLeft + 2, tpos, barwidth - 4, 0);
                if (tpos - 2 > VSTop) {
                    drawRect(g, VSLeft + 2, tpos - 2, VSWidth - 4, 0);
                }
                if (tpos + 2 < VSTop + VSHeight) {
                    drawRect(g, VSLeft + 2, tpos + 2, VSWidth - 4, 0);
                }
                //三角
                g.setColor(getBorderColor());
                fillTriangle(g, thisWidth - barwidth + barwidth / 2, barwidth / 2 - 2,
                        thisWidth - barwidth + barwidth / 2 - 4, barwidth / 2 + 2,
                        thisWidth - barwidth + barwidth / 2 + 4, barwidth / 2 + 2);
                fillTriangle(g, thisWidth - barwidth + barwidth / 2 - 4, VBarHeight - barwidth + barwidth / 2 - 2,
                        thisWidth - barwidth + barwidth / 2 + 4, VBarHeight - barwidth + barwidth / 2 - 2,
                        thisWidth - barwidth + barwidth / 2, VBarHeight - barwidth / 2 + 2);
            }
            /*
            //焦点            
            SetClip(g);
            if (isFoucsed()) {
                g.setColor(SDAConsts.clFocusShadow);
                drawRect(g, 1, 1, thisWidth - 2, thisHeight - 2);
            }
             */ 
        }
    }

    public int getSelectedFontColor() {
        return selectedFontColor;
    }

    public void setSelectedFontColor(int selectedFontColor) {
        this.selectedFontColor = selectedFontColor;
        paint();
    }

    public int getBorderStyle() {
        return borderStyle;
    }

    public void setBorderStyle(int borderStyle) {
        this.borderStyle = borderStyle;
        paint();
    }

    public boolean isHideSelection() {
        return hideSelection;
    }

    public void setHideSelection(boolean hideSelection) {
        this.hideSelection = hideSelection;
    }

    public int getScrollBars() {
        return scrollBars;
    }

    public void setScrollBars(int scrollBars) {
        this.scrollBars = scrollBars;
        paint();
    }

    public int getScrollBarWidth() {
        return barwidth;
    }

    public void setScrollBarWidth(int barwidth) {
        this.barwidth = barwidth;
        paint();
    }

    private int getCursor() {
        return Cursor;
    }

    private void setCursor(int Cursor) {
        this.Cursor = Cursor;
    }

    public int getDefaultColWidth() {
        return DefaultColWidth;
    }

    public void setDefaultColWidth(int DefaultColWidth) {
        this.DefaultColWidth = DefaultColWidth;
        paint();
    }

    public int getFixedWidth() {
        return FixedWidth;
    }

    public void setFixedWidth(int FixedWidth) {
        this.FixedWidth = FixedWidth;
        paint();
    }

    public int getFootRowColor() {
        return footRowColor;
    }

    public void setFootRowColor(int footRowColor) {
        this.footRowColor = footRowColor;
        paint();
    }

    public int getRowHeight() {
        int fontheight = getFont().getHeight() + 2;
        if (RowHeight < fontheight) {
            RowHeight = fontheight;
        }
        return RowHeight;
    }

    public void setRowHeight(int RowHeight) {
        //根据字体大小判断
        int fontheight = getFont().getHeight() + 2;
        if (fontheight > RowHeight) {
            this.RowHeight = fontheight;
        } else {
            this.RowHeight = RowHeight;
        }
        paint();
    }

    public int getBorderColor() {
        return borderColor;
    }

    public void setBorderColor(int borderColor) {
        this.borderColor = borderColor;
        paint();
    }

    public int getFixedColor() {
        return FixedColor;
    }

    public void setFixedColor(int FixedColor) {
        this.FixedColor = FixedColor;
        paint();
    }

    private int getForeWidth() {
        int foreWidth = 0;
        for (int i = 0; i < StartCol; i++) {
            foreWidth += ((Column) ColumnsList.elementAt(i)).ColWidth;
        }
        return foreWidth;
    }

    public boolean isShowFootRow() {
        return showFootRow;
    }

    public void setShowFootRow(boolean showFootRow) {
        this.showFootRow = showFootRow;
        paint();
    }

    public int getSelectedBackColor() {
        return selectedBackColor;
    }

    public void setSelectedBackColor(int selectedBackColor) {
        this.selectedBackColor = selectedBackColor;
        paint();
    }

    public int getLostFocusSelectedBackColor() {
        return lostFocusSelectedBackColor;
    }

    public void setLostFocusSelectedBackColor(int lostFocusSelectedBackColor) {
        this.lostFocusSelectedBackColor = lostFocusSelectedBackColor;
    }

    private int getAllWidth() {
        int allWidth = 0;
        for (int i = 0; i < ColumnsList.size(); i++) {
            allWidth += ((Column) ColumnsList.elementAt(i)).ColWidth;
        }
        return allWidth;
    }

    private int getForeHeight() {
        int foreHeight = getRowHeight() * StartRow;
        return foreHeight;
    }

    private int getAllHeight() {
        int allHeight = 0;
        if (DataSet != null) {
            allHeight = getRowHeight() * DataSet.getRecordCount();
        }
        return allHeight;
    }

    private int getMaxRowsNum() {
        int rowHeight = getRowHeight();
        return (getVBarHeight() - rowHeight - (showFootRow ? rowHeight : 0)) / rowHeight;
    }
    //读取数据填充各列表
    public void setDataSet(DataSetUI DataSet) {
        interalSetDataSet(DataSet);
    }

    private void interalSetDataSet(DataSetUI DataSet) {
        //设置观察
        if (DataSet == null) {
            if (oldDataSet != null) {
                oldDataSet.setWatchControl(this, false);
            }
            this.DataSet = null;
            oldDataSet = null;
            return;
        } else {
            oldDataSet = DataSet;
            this.DataSet = DataSet;
            oldDataSet.setWatchControl(this, true);
        }
        //数据
        DataSetUI.Field fd = null;
        if ((DataSet != null) && (ColumnsList.size() == 0)) {
            for (int i = 0; i < DataSet.getFieldsCount(); i++) {
                fd = DataSet.getField(i);
                Column ct = new Column();
                ct.ColWidth = DefaultColWidth;
                ct.FieldName = fd.getFiledName();
                ct.Title = ct.FieldName;
                ct.DisplayFormat = "";
                ColumnsList.addElement(ct);
            }
        }
        Cursor = 0;
        StartCol = 0;
        StartRow = 0;
        EndRow = 0;
        //是否为最后一行
        IsEndRow = false;
        //是否为最后一列
        IsEndCol = false;
        //数据集合
        DataSet.RecNo = 0;
        DataSet.doDataChangeScroll(DataSet.RecNo);
        paint();
    }

    private int getHBarWidth() {
        int swidth = 0;
        if (scrollBars == ConstsUI.srHorizontal) {
            swidth = getWidth();
        }
        if (scrollBars == ConstsUI.srBoth) {
            swidth = getWidth() - barwidth;
        }
        return swidth;
    }

    private int getVBarHeight() {
        int sheight = 0;
        if (scrollBars == ConstsUI.srVertical) {
            sheight = getHeight();
        }
        if (scrollBars == ConstsUI.srBoth) {
            sheight = getHeight() - barwidth;
        }
        return sheight;
    }

    //处理事件的执行
    //点箭头滚动内容
    private void doPointerPressed(int x, int y) {
        if ((DataSet == null) || (DataSet.getRecordCount() == 0)) {
            return;
        }
        int posx = screenXToClient(x);
        int posy = screenYToClient(y);
        int thisWidth = getWidth();
        int thisHeight = getHeight();
        int VBarHeight = getVBarHeight();
        int HBarWidth = getHBarWidth();
        int fontHeight = getFont().getHeight() + 2;
        int rowHeight = getRowHeight();
        //如果点了外面
        if (!InClientRect(posx, posy, 0, 0, thisWidth, thisHeight)) {
        //popVisible = false;
        }
        //确定点击了滚动条区域
        if ((scrollBars == ConstsUI.srHorizontal) || (scrollBars == ConstsUI.srBoth)) {
            //判断是否点击了左箭头
            if (InClientRect(posx, posy, 0, thisHeight - barwidth, barwidth, barwidth)) {
                //向右滚动
                if (StartCol > 0) {
                    StartCol -= 1;
                }
            } else //右箭头
            if (InClientRect(posx, posy, HBarWidth - barwidth, thisHeight - barwidth, barwidth, barwidth)) {
                if (!IsEndCol) {
                    StartCol += 1;
                }
            } else //滚动条
            if (InClientRect(posx, posy, HSLeft, HSTop, HSWidth, HSHeight)) {
                //记录点击的滚动条位置
                oldScrollPointx = posx;
                oldScrollPointy = posy;
                isscrollbarpointdown = true;
                scrollbardownHV = 0;
                oldStartLeft = startLeft;
            } else {
                if (InClientRect(posx, posy, 0, thisHeight - barwidth, HBarWidth, barwidth)) {
                    //计算滚动块要到位置
                    int tpos = posx > HSLeft ? (posx - HSWidth) : (posx);
                    int oldcol = StartCol;
                    //计算StartLeft                    
                    startLeft = ((tpos - barwidth) * (maxLineLenght)) / (HBarWidth - 2 * barwidth);
                    int len = 0;
                    int index = 0;
                    while (startLeft > len) {
                        len += getColumn(index).ColWidth;
                        index++;
                    }
                    StartCol = index;
                    if (oldcol == StartCol) {
                        StartCol = posx > HSLeft ? StartCol + 1 : StartCol - 1;
                    }
                }
            }
        }

        if ((scrollBars == ConstsUI.srVertical) || (scrollBars == ConstsUI.srBoth)) {
            //只有垂直滚动条
            //判断是否点击了上箭头
            if (InClientRect(posx, posy, thisWidth - barwidth, 0, barwidth, barwidth)) {
                //向下滚动
                if (Cursor > StartRow) {
                    Cursor -= 1;
                    DataSet.RecNo = Cursor;
                    DataSet.doDataChangeScroll(DataSet.RecNo);
                    return;
                }
                //向上滚动
                if (StartRow > 0) {
                    StartRow -= 1;
                    Cursor = StartRow;
                    DataSet.RecNo = Cursor;
                    DataSet.doDataChangeScroll(DataSet.RecNo);
                }
            } else //下箭头
            if (InClientRect(posx, posy, thisWidth - barwidth, VBarHeight - barwidth, barwidth, barwidth)) {
                //向上滚动
                if (Cursor < EndRow) {
                    Cursor += 1;
                    DataSet.RecNo = Cursor;
                    DataSet.doDataChangeScroll(DataSet.RecNo);
                    return;
                }
                if (!IsEndRow) {
                    StartRow += 1;
                    Cursor = EndRow + 1;
                    DataSet.RecNo = Cursor;
                } else {
                    Cursor = EndRow;
                    DataSet.RecNo = Cursor;
                }
                DataSet.doDataChangeScroll(DataSet.RecNo);
            } else //滚动条
            if (InClientRect(posx, posy, VSLeft, VSTop, VSWidth, VSHeight)) {
                //记录位置
                oldScrollPointx = posx;
                oldScrollPointy = posy;
                isscrollbarpointdown = true;
                scrollbardownHV = 1;
                oldStartLine = StartRow;
            } else {
                if (InClientRect(posx, posy, thisWidth - barwidth, 0, barwidth, VBarHeight)) {
                    //空白的
                    //计算滚动块要到位置
                    int tpos = posy > VSTop ? (posy - VSHeight) : (posy);
                    //计算StartLine
                    int oldline = StartRow;
                    StartRow = ((tpos - barwidth) * (getAllHeight())) / (VBarHeight - 2 * barwidth) / fontHeight;
                    if (oldline == StartRow) {
                        StartRow = posy > VSTop ? StartRow + 1 : StartRow - 1;
                    }
                    if (DataSet.RecNo < StartRow) {
                        DataSet.RecNo = StartRow;
                    }
                    int maxRowsNum = getMaxRowsNum();
                    if (DataSet.RecNo > StartRow + maxRowsNum - 1) {
                        DataSet.RecNo = StartRow + maxRowsNum - 1;
                    }
                }
            }
        }
        //点了滚动条外       

        if (InClientRect(posx, posy, 0, rowHeight, HBarWidth, VBarHeight - rowHeight - (showFootRow ? rowHeight : 0))) {
            //计算y位置计算Cursor位置
            Cursor = posy / rowHeight - 1 + StartRow;
            if (Cursor < StartRow) {
                Cursor = StartRow;
            }
            if (Cursor > EndRow) {
                Cursor = EndRow;
            }
            DataSet.RecNo = Cursor;
            DataSet.doDataChangeScroll(Cursor);
            //触发点击cell事件
            if (OnGridCellClick != null) {
                OnGridCellClick.Event(DataSet.getRow(Cursor));
            }
        }
        //点了标题条，判断排序的
        if (InClientRect(posx, posy, FixedWidth, 0, thisWidth - barwidth - FixedWidth, rowHeight)) {
            int pos = 0;
            int id = StartCol;
            Column cl = null;
            while (pos < posx - FixedWidth) {
                cl = getColumn(id);
                id++;
                pos += cl.ColWidth;
            }
            if (DataSet.getField(cl.FieldName).getFiledType() != DataSetUI.FieldAudoID) {
                if (sortType == DataSetUI.sortAsc) {
                    sortType = DataSetUI.sortDesc;
                } else {
                    sortType = DataSetUI.sortAsc;
                }
                sortFieldName = cl.FieldName;
                DataSet.sortDataSet(cl.FieldName, sortType);
            }
        }

    }
    //拖动事件处理
    private void doPointerReleased(int x, int y) {
        int posx = screenXToClient(x);
        int posy = screenYToClient(y);
        int VBarHeight = getVBarHeight();
        int HBarWidth = getHBarWidth();
        //根据点击的位置,判断滚动的多少
        if ((scrollBars == ConstsUI.srHorizontal) || ((scrollBars == ConstsUI.srBoth) && (scrollbardownHV == 0))) {
            if (isscrollbarpointdown) {
                int stepx = posx - oldScrollPointx;
                //根据滚动多少来重新定位
                //计算滚动块要到位置
                int tpos = HSLeft + stepx;
                HSLeft = tpos < barwidth ? barwidth : tpos;
                HSLeft = HSLeft + HSWidth > HBarWidth - barwidth ? HBarWidth - barwidth - HSWidth : HSLeft;
                //计算StartLeft
                startLeft = ((HSLeft - barwidth) * (maxLineLenght)) / (HBarWidth - 2 * barwidth);
                if (oldStartLeft != startLeft) {
                    oldScrollPointx = posx;
                    oldStartLeft = startLeft;
                }
                int oldcol = StartCol;
                int len = 0;
                int index = 0;
                while (startLeft > len) {
                    len += getColumn(index).ColWidth;
                    index++;
                }
                StartCol = index;
                if (oldcol == StartCol) {
                    if (!IsEndCol) {
                        StartCol = stepx > 0 ? StartCol + 1 : StartCol - 1;
                    }
                }
                if (StartCol < 0) {
                    StartCol = 0;
                }
            }
        }
        if ((scrollBars == ConstsUI.srVertical) || ((scrollBars == ConstsUI.srBoth) && (scrollbardownHV == 1))) {
            if (isscrollbarpointdown) {
                int stepy = posy - oldScrollPointy;
                //根据滚动多少来重新定位
                //计算滚动块要到位置
                int tpos = VSTop + stepy;
                VSTop = tpos < barwidth ? barwidth : tpos;
                VSTop = VSTop + VSHeight > VBarHeight - barwidth ? VBarHeight - barwidth - VSHeight : VSTop;
                //计算StartLine
                StartRow = ((VSTop - barwidth) * (getAllHeight())) / (VBarHeight - 2 * barwidth) / getFont().getHeight();
                if (oldStartLine == StartRow) {
                    if (!IsEndRow) {
                        StartRow = stepy > 0 ? StartRow + 1 : StartRow - 1;
                    }
                    oldStartLine = StartRow;
                } else {
                    if ((stepy > 0) && (StartRow < oldStartLine)) {
                        StartRow = oldStartLine;
                    }
                    oldStartLine = StartRow;
                }
                if (StartRow < 0) {
                    StartRow = 0;
                }
                if (DataSet.RecNo < StartRow) {
                    DataSet.RecNo = StartRow;
                }
                int maxRowsNum = getMaxRowsNum();
                if (DataSet.RecNo > StartRow + maxRowsNum - 1) {
                    DataSet.RecNo = StartRow + maxRowsNum - 1;
                }
            }
        }
        isscrollbarpointdown = false;
    }
    //键盘事件处理
    private void doKeyUp(int keyCode) {
        String key = form.getKeyName(keyCode).toUpperCase();
        try {
            if (key.equals(ConstsUI.KEY_UP)) {
                if (DataSet != null) {
                    if (Cursor > StartRow) {
                        Cursor -= 1;
                        DataSet.RecNo = Cursor;
                        DataSet.doDataChangeScroll(DataSet.RecNo);
                        return;
                    }
                    //向上滚动
                    if (StartRow > 0) {
                        StartRow -= 1;
                        Cursor = StartRow;
                        DataSet.RecNo = Cursor;
                        DataSet.doDataChangeScroll(DataSet.RecNo);
                    }
                }
            }
            if (key.equals(ConstsUI.KEY_DOWN)) {
                if (DataSet != null) {
                    if (Cursor < EndRow) {
                        Cursor += 1;
                        DataSet.RecNo = Cursor;
                        DataSet.doDataChangeScroll(DataSet.RecNo);
                        return;
                    }
                    if (!IsEndRow) {
                        StartRow += 1;
                        Cursor = EndRow + 1;
                        DataSet.RecNo = Cursor;
                    } else {
                        Cursor = EndRow;
                        DataSet.RecNo = Cursor;
                    }
                    DataSet.doDataChangeScroll(DataSet.RecNo);
                }
            }
            if (key.equals(ConstsUI.KEY_LEFT)) {
                if (StartCol > 0) {
                    StartCol -= 1;
                }
            }
            if (key.equals(ConstsUI.KEY_RIGHT)) {
                if (!IsEndCol) {
                    StartCol += 1;
                }
            }
        } finally {
            repaintControl();
        }
    }
    //点击单元格事件处理   
    public void setOnGridCellClick(GridCellClickEvent OnGridCellClick) {
        this.OnGridCellClick = OnGridCellClick;
    }

    public void setOnDrawGridCell(DrawGridCellEvent onDrawGridCell) {
        this.onDrawGridCell = onDrawGridCell;
    }

    protected boolean canDownTabNext() {
        boolean result = false;
        if (DataSet != null) {
            if (DataSet.RecNo == DataSet.getRecordCount() - 1) {
                result = true;
            }
        } else {
            result = false;
        }
        return result;
    }

    protected boolean canLeftTabPrior() {
        return false;
    }

    protected boolean canRightTabNext() {
        return false;
    }

    protected boolean canUpTabPrior() {
        boolean result = false;
        if (DataSet != null) {
            if (DataSet.RecNo == 0 || DataSet.RecNo == -1) {
                result = true;
            }
        } else {
            result = false;
        }
        return result;
    }
    //焦点
}
