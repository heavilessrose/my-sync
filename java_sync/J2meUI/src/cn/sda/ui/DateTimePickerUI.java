package cn.sda.ui;

import cn.sda.event.BaseEditTextChangeEvent;
import cn.sda.event.KeybordEvent;
import cn.sda.event.NotifyEvent;
import cn.sda.event.PointerEvent;
import java.util.Calendar;
import java.util.TimeZone;
import javax.microedition.lcdui.Graphics;
import javax.microedition.lcdui.Font;

/**
 *
 * @author Administrator
 */
public class DateTimePickerUI extends BaseControl {

    private int borderColor = ConstsUI.clBlack;
    private int arrowWidth = 15;
    private int arrowBackColor = ConstsUI.clBtnFace;
    private int selectedFontColor = ConstsUI.clWhite;
    private int selectedBackColor = ConstsUI.clFocusShadow;
    //日期
    private int Year = 0;
    private int Month = 0;
    private int Day = 0;
    private int Hour = 0;
    private int Minute = 0;
    private int Second = 0;
    //格式
    private int dateKind = ConstsUI.dkDate;
    //当前选中类型(Year:0,Month:1,Day:2,Hour:3,Minute:4,Second:5)
    private int selectType = -1;
    private int selectStart = 0;
    private int selectLen = 0;
    private String selectText = "";
    //日期
    Calendar cl = null;
    //当前时间   
    private String dateTime = "";
    //编辑框
    private BaseEdit NumEdit = null;
    //是否可以编辑
    private boolean canEdit = false;

    public DateTimePickerUI() {
        super();
        cl = Calendar.getInstance(TimeZone.getDefault());
        //初始化
        Year = cl.get(Calendar.YEAR);
        Month = cl.get(Calendar.MONTH) + 1;
        Day = cl.get(Calendar.DATE);
        Hour = cl.get(Calendar.HOUR_OF_DAY);
        Minute = cl.get(Calendar.MINUTE);
        Second = cl.get(Calendar.SECOND);
        dateTime = String.valueOf(Year) + "-" + String.valueOf(Month) + "-" + String.valueOf(Day);
        //可视属性
        visible = true;
        width = 100;
        height = 20;
        backColor = ConstsUI.clWhite;
        NumEdit = new BaseEdit();
        NumEdit.setBorderStyle(ConstsUI.bsFixedSingle);
        NumEdit.backColor = ConstsUI.clFocusShadow;
        NumEdit.foreColor = ConstsUI.clWhite;
        NumEdit.visible = false;
        setCanEdit(false);
        AddControl(NumEdit);
        NumEdit.setOnLostFocused(new NotifyEvent() {

            public void Event(BaseControl ctrl) {
                //失去焦点，隐藏
                NumEdit.cursorCol = 0;
                internalPaint();
            }
        });
        NumEdit.setOnFocused(new NotifyEvent() {

            public void Event(BaseControl ctrl) {
                //设置输入法
                form.Application.inputPanel.setImeType(InputPanel.imDigit);
                form.repaintControl();
            }
        });
        //内容变化
        NumEdit.setOnTextChange(new BaseEditTextChangeEvent() {

            public void Event(BaseEdit edit) {
                //文字变化
                int value = 0;
                try {
                    value = Integer.parseInt(NumEdit.text);
                    setSelectValue(value, false);
                } catch (Exception e) {
                    setSelectValue(Integer.parseInt(selectText), false);
                }
            }
        });
        setOnKeyUp(new KeybordEvent() {

            public void Event(BaseControl ctrl, int keyCode) {
                //按下
                doKeyUp(keyCode);
            }
        });
        setOnPointerPressed(new PointerEvent() {

            public void Event(BaseControl ctrl, int x, int y) {
                //触屏
                doPointerPress(x, y);
            }
        });
    }

    private void setTab(boolean tab) {
        tabStop = tab;
    }

    public int getArrowWidth() {
        return arrowWidth;
    }

    public void setArrowWidth(int arrowWidth) {
        this.arrowWidth = arrowWidth;
        repaintControl();
    }

    public int getBorderColor() {
        return borderColor;
    }

    public void setBorderColor(int borderColor) {
        this.borderColor = borderColor;
        repaintControl();
    }

    public int getArrowBackColor() {
        return arrowBackColor;
    }

    public void setArrowBackColor(int arrowBackColor) {
        this.arrowBackColor = arrowBackColor;
        repaintControl();
    }

    //日期显示
    private void setDateTime() {
        setSelectTypePos(selectType);
        Year = cl.get(Calendar.YEAR);
        Month = cl.get(Calendar.MONTH) + 1;
        Day = cl.get(Calendar.DAY_OF_MONTH);
        Hour = cl.get(Calendar.HOUR_OF_DAY);
        Minute = cl.get(Calendar.MINUTE);
        Second = cl.get(Calendar.SECOND);
        if (dateKind == ConstsUI.dkDate) {
            dateTime = String.valueOf(Year) + "-" + String.valueOf(Month) + "-" + String.valueOf(Day);
        }
        if (dateKind == ConstsUI.dkTime) {
            dateTime = String.valueOf(Hour) + ":" + String.valueOf(Minute) + ":" + String.valueOf(Second);
        }
        if (dateKind == ConstsUI.dkDateTime) {
            dateTime = String.valueOf(Year) + "-" + String.valueOf(Month) + "-" + String.valueOf(Day) + " " +
                    String.valueOf(Hour) + ":" + String.valueOf(Minute) + ":" + String.valueOf(Second);
        }
        repaintControl();
    }

    public String getDate() {
        return String.valueOf(Year) + "-" + String.valueOf(Month) + "-" + String.valueOf(Day);
    }

    public String getTime() {
        return String.valueOf(Hour) + ":" + String.valueOf(Minute) + ":" + String.valueOf(Second);
    }

    public String getDateTime() {
        return String.valueOf(Year) + "-" + String.valueOf(Month) + "-" + String.valueOf(Day) + " " +
                String.valueOf(Hour) + ":" + String.valueOf(Minute) + ":" + String.valueOf(Second);
    }

    public void setDate(int year, int month, int day) {
        setYear(year);
        setMonth(month);
        setDay(day);
    }

    public void setTime(int hour, int minute, int second) {
        setHour(hour);
        setMinute(minute);
        setSecond(second);
    }

    public void setDateTime(int year, int month, int day, int hour, int minute, int second) {
        setDate(year, month, day);
        setTime(hour, minute, second);
    }

    public int getDay() {
        return Day;
    }

    public void setDay(int Day) {
        try {
            cl.set(Calendar.DAY_OF_MONTH, Day);
            this.Day = Day;
            setDateTime();
        } catch (Exception e) {
        }
    }

    public int getHour() {
        return Hour;
    }

    public void setHour(int Hour) {
        try {
            cl.set(Calendar.HOUR_OF_DAY, Hour);
            this.Hour = Hour;
            setDateTime();
        } catch (Exception e) {

        }
    }

    public int getMinute() {
        return Minute;
    }

    public void setMinute(int Minute) {
        try {
            cl.set(Calendar.MINUTE, Minute);
            this.Minute = Minute;
            setDateTime();
        } catch (Exception e) {
        }
    }

    public int getMonth() {
        return Month;
    }

    public void setMonth(int Month) {
        try {
            cl.set(Calendar.MONTH, Month);
            this.Month = Month;
            setDateTime();
        } catch (Exception e) {
        }
    }

    public int getSecond() {
        return Second;
    }

    public void setSecond(int Second) {
        try {
            cl.set(Calendar.SECOND, Second);
            this.Second = Second;
            setDateTime();
        } catch (Exception e) {
        }
    }

    public int getYear() {
        return Year;
    }

    public void setYear(int Year) {
        try {
            cl.set(Calendar.YEAR, Year);
            this.Year = Year;
            setDateTime();
        } catch (Exception e) {
        }
    }

    public int getDateKind() {
        return dateKind;
    }

    public void setDateKind(int dateKind) {
        this.dateKind = dateKind;
        if (dateKind == ConstsUI.dkTime) {
            selectType = 3;
        } else {
            selectType = 0;
        }
        setSelectTypePos(selectType);
        setDateTime();
    }

    public int getSelectedBackColor() {
        return selectedBackColor;
    }

    public void setSelectedBackColor(int selectedBackColor) {
        this.selectedBackColor = selectedBackColor;
    }

    public int getSelectedFontColor() {
        return selectedFontColor;
    }

    public void setSelectedFontColor(int selectedFontColor) {
        this.selectedFontColor = selectedFontColor;
    }

    //获取修改类型位置
    private void setSelectTypePos(int selectType) {
        int result = 0;
        Font ft = getFont();
        int yearLen = ft.stringWidth(String.valueOf(Year));
        int monthLen = ft.stringWidth(String.valueOf(Month));
        int dayLen = ft.stringWidth(String.valueOf(Day));
        int hourLen = ft.stringWidth(String.valueOf(Hour));
        int minuteLen = ft.stringWidth(String.valueOf(Minute));
        int secondLen = ft.stringWidth(String.valueOf(Second));
        if (dateKind == ConstsUI.dkDate || dateKind == ConstsUI.dkDateTime) {
            switch (selectType) {
                case 0:
                    result = 0;
                    selectLen = yearLen;
                    selectText = String.valueOf(Year);
                    break;
                case 1:
                    result = yearLen + ft.charWidth('-');
                    selectLen = monthLen;
                    selectText = String.valueOf(Month);
                    break;
                case 2:
                    result = yearLen + monthLen + ft.stringWidth("--");
                    selectLen = dayLen;
                    selectText = String.valueOf(Day);
                    break;
            }
        }
        if (dateKind == ConstsUI.dkDateTime) {
            switch (selectType) {
                case 3:
                    result = yearLen + monthLen + dayLen + ft.stringWidth("-- ");
                    selectLen = hourLen;
                    selectText = String.valueOf(Hour);
                    break;
                case 4:
                    result = yearLen + monthLen + dayLen + hourLen + ft.stringWidth("-- :");
                    selectLen = minuteLen;
                    selectText = String.valueOf(Minute);
                    break;
                case 5:
                    result = yearLen + monthLen + dayLen + hourLen + minuteLen + ft.stringWidth("-- ::");
                    selectLen = secondLen;
                    selectText = String.valueOf(Second);
                    break;
            }
        }
        if (dateKind == ConstsUI.dkTime) {
            switch (selectType) {
                case 3:
                    result = 0;
                    selectLen = hourLen;
                    selectText = String.valueOf(Hour);
                    break;
                case 4:
                    result = hourLen + ft.charWidth(':');
                    selectLen = minuteLen;
                    selectText = String.valueOf(Minute);
                    break;
                case 5:
                    result = hourLen + minuteLen + ft.stringWidth("::");
                    selectLen = secondLen;
                    selectText = String.valueOf(Second);
                    break;
            }
        }
        selectStart = result;
    }

    public void paint() {
        if (IsCanPaint()) {
            internalPaint();
            PaintChilds();
        }
    }

    private void internalPaint() {
        //画
        Graphics g = form.getGraphics();
        g.setFont(getFont());
        SetClip(g);
        //画边框和箭头
        drawControlRect(g);
        //最后打字
        drawControlText(g);
    }

    private void drawControlRect(Graphics g) {
        int thisHeight = getHeight();
        int thisWidth = getWidth();
        g.setColor(backColor);
        fillRect(g, 0, 0, thisWidth, thisHeight);
        g.setColor(borderColor);
        drawRect(g, 0, 0, thisWidth, thisHeight);
        g.setColor(arrowBackColor);
        //两边的框
        fillRect(g, 0, 0, arrowWidth, thisHeight);
        fillRect(g, thisWidth - arrowWidth, 0, arrowWidth, thisHeight);
        g.setColor(borderColor);
        drawRect(g, 0, 0, thisWidth, thisHeight);
        drawRect(g, 0, 0, arrowWidth, thisHeight);
        drawRect(g, thisWidth - arrowWidth, 0, arrowWidth, thisHeight);
        //左箭头
        g.setColor(borderColor);
        fillTriangle(g, arrowWidth / 2 - 2, thisHeight / 2, arrowWidth / 2 + 2, thisHeight / 2 - 4, arrowWidth / 2 + 2, thisHeight / 2 + 4);
        //右箭头
        fillTriangle(g, thisWidth - arrowWidth / 2 - 2, thisHeight / 2 - 4, thisWidth - arrowWidth / 2 - 2, thisHeight / 2 + 4, thisWidth - arrowWidth / 2 + 2, thisHeight / 2);
        //焦点
        if (isFoucsed()) {
            g.setColor(ConstsUI.clFocusShadow);
            drawRect(g, 1, 1, thisWidth - 2, thisHeight - 2);
        }
    }

    private void drawControlText(Graphics g) {
        int thisHeight = getHeight();
        int thisWidth = getWidth();
        SetClip(g, arrowWidth, 1, thisWidth - 2 * arrowWidth, thisHeight - 1);
        g.setColor(foreColor);
        int pos = (thisWidth - getFont().stringWidth(dateTime)) / 2;
        int toppos = (thisHeight - getFont().getHeight()) / 2;
        drawString(g, dateTime, pos, toppos);
        if (canEdit) {
            //显示编辑框
            NumEdit.left = pos + selectStart - 2;
            NumEdit.top = 0;
            NumEdit.width = selectLen + 6;
            NumEdit.height = height;
            NumEdit.text = selectText;
            if (selectType > -1 && NumEdit.visible) {
                NumEdit.paint();
            }
        } else {
            //画选择焦点
            if (selectType > -1) {
                setSelectTypePos(selectType);
                g.setColor(selectedBackColor);
                fillRect(g, pos + selectStart, toppos, selectLen, getFont().getHeight());
                //内容
                g.setColor(selectedFontColor);
                drawString(g, selectText, pos + selectStart, toppos);
            }
        }
    }

    private void doKeyUp(int keyCode) {
        String key = form.getKeyName(keyCode).toUpperCase();
        if (key.equals(ConstsUI.KEY_UP)) {
            //左
            if (dateKind != ConstsUI.dkTime) {
                if (selectType > 0) {
                    selectType--;
                }
            } else {
                if (selectType > 3) {
                    selectType--;
                }
            }
            if (canEdit) {
                NumEdit.visible = true;
            }
        }
        if (key.equals(ConstsUI.KEY_LEFT)) {
            setSelectValue(-1, true);
        }
        if (key.equals(ConstsUI.KEY_DOWN)) {
            if (canEdit) {
                if (!NumEdit.visible) {
                    NumEdit.visible = true;
                    return;
                }
            }
            //右
            if (dateKind != ConstsUI.dkDate) {
                if (selectType < 5) {
                    selectType++;
                }
            } else {
                if (selectType < 2) {
                    selectType++;
                }
            }
        }
        if (key.equals(ConstsUI.KEY_RIGHT)) {
            setSelectValue(1, true);
        }
        repaintControl();
        /*
        if (key.equals(SDAConsts.KEY_SELECT)) {
        this.popVisible = !popVisible;
        }
         */
        setSelectTypePos(selectType);
    }

    private void doPointerPress(int x, int y) {
        int posx = screenXToClient(x);
        int posy = screenYToClient(y);
        int thisHeight = getHeight();
        int thisWidth = getWidth();
        if (InClientRect(posx, posy, 0, 0, thisWidth, thisHeight)) {
            if (InClientRect(posx, posy, 0, 0, arrowWidth, thisHeight)) {
                setSelectValue(-1, true);
            } else if (InClientRect(posx, posy, thisWidth - arrowWidth, 0, arrowWidth, thisHeight)) {
                setSelectValue(1, true);
            }
            getSelectTypeByPos(posx);
        }
    }
    //根据位置获取选择
    private void getSelectTypeByPos(int x) {
        int oldType = selectType;
        boolean find = false;
        if (x > arrowWidth && x < getWidth() - arrowWidth) {
            int pos = (getWidth() - getFont().stringWidth(dateTime)) / 2;
            if (dateKind == ConstsUI.dkDate) {
                for (int i = 0; i < 3; i++) {
                    setSelectTypePos(i);
                    if (pos + selectStart < x && (pos + selectStart + selectLen > x)) {
                        selectType = i;
                        find = true;
                        break;
                    }
                }
            }
            if (dateKind == ConstsUI.dkDateTime) {
                for (int i = 0; i < 6; i++) {
                    setSelectTypePos(i);
                    if (pos + selectStart < x && (pos + selectStart + selectLen > x)) {
                        selectType = i;
                        find = true;
                        break;
                    }
                }
            }
            if (dateKind == ConstsUI.dkTime) {
                for (int i = 3; i < 6; i++) {
                    setSelectTypePos(i);
                    if (pos + selectStart < x && (pos + selectStart + selectLen > x)) {
                        selectType = i;
                        find = true;
                        break;
                    }
                }
            }
        }
        if (!find) {
            selectType = oldType;
        }
    }
    //对选择的内容增加和减少
    private void setSelectValue(int value, boolean incValue) {
        try {
            switch (selectType) {
                case 0:
                    if (incValue) {
                        Year += value;
                    } else {
                        Year = value;
                    }
                    cl.set(Calendar.YEAR, Year);
                    setDateTime();
                    break;
                case 1:
                    if (incValue) {
                        Month += value;
                    } else {
                        Month = value;
                    }
                    cl.set(Calendar.MONTH, Month - 1);
                    setDateTime();
                    break;
                case 2:
                    if (incValue) {
                        Day += value;
                    } else {
                        Day = value;
                    }
                    cl.set(Calendar.DAY_OF_MONTH, Day);
                    setDateTime();
                    break;
                case 3:
                    if (incValue) {
                        Hour += value;
                    } else {
                        Hour = value;
                    }
                    cl.set(Calendar.HOUR_OF_DAY, Hour);
                    setDateTime();
                    break;
                case 4:
                    if (incValue) {
                        Minute += value;
                    } else {
                        Minute = value;
                    }
                    cl.set(Calendar.MINUTE, Minute);
                    setDateTime();
                    break;
                case 5:
                    if (incValue) {
                        Second += value;
                    } else {
                        Second = value;
                    }
                    cl.set(Calendar.SECOND, Second);
                    setDateTime();
                    break;
            }
        } catch (Exception e) {
        }
    }

    protected boolean canDownTabNext() {
        boolean result = false;
        if (dateKind == ConstsUI.dkDate) {
            if (selectType == 2) {
                result = true;
            } else {
                result = false;
            }
        }
        if (dateKind == ConstsUI.dkDateTime || dateKind == ConstsUI.dkTime) {
            if (selectType == 5) {
                result = true;
            } else {
                result = false;
            }
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
        if (dateKind == ConstsUI.dkDate || dateKind == ConstsUI.dkDateTime) {
            if (selectType == 0) {
                result = true;
            } else {
                result = false;
            }
        }
        if (dateKind == ConstsUI.dkTime) {
            if (selectType == 3) {
                result = true;
            } else {
                result = false;
            }
        }
        return result;
    }

    public boolean isCanEdit() {
        return canEdit;
    }

    public void setCanEdit(boolean canEdit) {
        this.canEdit = canEdit;
        if (canEdit) {
            NumEdit.visible=true;
            NumEdit.tabStop=true;
        } else {
            NumEdit.visible=false;
            NumEdit.tabStop=false;
        }
    }
}
