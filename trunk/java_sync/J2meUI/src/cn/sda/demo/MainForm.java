package cn.sda.demo;

import cn.sda.event.DrawGridCellEvent;
import cn.sda.event.MainMenuOnButtonEvent;
import cn.sda.event.MsgBoxCloseEvent;
import cn.sda.event.ToolsBarClickEvent;
import cn.sda.ui.Bevel;
import cn.sda.ui.ChartUI;
import cn.sda.ui.ChartSeriesUI;
import cn.sda.ui.CheckBoxUI;
import cn.sda.ui.ComboBoxUI;
import cn.sda.ui.ConstsUI;
import cn.sda.ui.DataSetUI;
import cn.sda.ui.DateTimePickerUI;
import cn.sda.ui.EditUI;
import cn.sda.ui.FormUI;
import cn.sda.ui.GridUI;
import cn.sda.ui.ImageUI;
import cn.sda.ui.LabelUI;
import cn.sda.ui.ListBoxUI;
import cn.sda.ui.MainMenuUI;
import cn.sda.ui.NumbericUpDown;
import cn.sda.ui.PopMenuUI;
import cn.sda.ui.ProgressUI;
import cn.sda.ui.RadioButtonUI;
import cn.sda.ui.ScrollBarUI;
import cn.sda.ui.SignPanelUI;
import cn.sda.ui.SpliterUI;
import cn.sda.ui.StatusBarUI;
import cn.sda.ui.StatusPanelUI;
import cn.sda.ui.TabControlUI;
import cn.sda.ui.TabSheetUI;
import cn.sda.ui.ToolButtonUI;
import cn.sda.ui.ToolsBarUI;
import cn.sda.ui.TrackBarUI;
import cn.sda.ui.TreeNodeUI;
import cn.sda.ui.TreeViewUI;
import javax.microedition.lcdui.Image;

/**
 *
 * @author Administrator
 */
public class MainForm extends FormUI {

    //工具栏
    ToolsBarUI toolbar = new ToolsBarUI();
    //菜单
    MainMenuUI mainmenu = new MainMenuUI();
    //页框组件
    TabControlUI tabControl = new TabControlUI();
    //签字板
    SignPanelUI signpanel=new SignPanelUI();
    //图形
    ImageUI img=new ImageUI();

    public MainForm() {
        //工具栏
        toolbar.setDock(ConstsUI.dsTop);
        toolbar.addButton("新建", ((MIDlet1) Application).image1);
        toolbar.addButton("编辑", ((MIDlet1) Application).image2);
        toolbar.addSeparator();
        toolbar.addButton("删除", ((MIDlet1) Application).image1);
        toolbar.addButton("设置", ((MIDlet1) Application).image2);
        toolbar.setShowCaption(true);
        toolbar.setBackImage(((MIDlet1) Application).image3);
        toolbar.setGlyphAlignType(ConstsUI.blGlyphLeft);
        AddControl(toolbar);
        //分割拖动条
        SpliterUI spliter1 = new SpliterUI();
        spliter1.setMinSize(22);
        spliter1.setHeight(4);
        spliter1.setDock(ConstsUI.dsTop);
        AddControl(spliter1);
        //主菜单
        this.mainmenu.setBackImage(((MIDlet1) Application).image3);
        if (!Application.hasPointer()) {
            this.mainmenu.setLeftMenuName("输入法");
            this.mainmenu.setRightMenuName("关闭");
            this.mainmenu.setLeftMemu(this.mainmenu.getInputMemu());
        } else {
            this.mainmenu.setLeftMenuName("关闭");
        }
        this.mainmenu.setOnMainMenuButtonEvent(new MainMenuOnButtonEvent() {

            public void Event(PopMenuUI memu, String MenuButtonCaption) {
                if (MenuButtonCaption.equals("关闭")) {
                    Application.Exit();
                }
            }
        });
        setMainMemu(mainmenu);
        //页框组件
        //基本组件页1
        TabSheetUI sheet1 = tabControl.addTabSheet();
        sheet1.setCaption("组件1");
        sheet1.setImage(((MIDlet1) Application).image1);
        //基本组件页2
        TabSheetUI sheet2 = tabControl.addTabSheet("组件2", ((MIDlet1) Application).image2);
        //图表页
        TabSheetUI sheet3 = tabControl.addTabSheet("图表", ((MIDlet1) Application).image1);
        //目录树页
        TabSheetUI sheet4 = tabControl.addTabSheet("目录树", ((MIDlet1) Application).image2);
        //表格页
        TabSheetUI sheet5 = tabControl.addTabSheet("表格", ((MIDlet1) Application).image1);
        //签字
        TabSheetUI sheet6 = tabControl.addTabSheet("签字", ((MIDlet1) Application).image2);
        //设置
        tabControl.setButtonPosition(ConstsUI.tabButtonsBottom);
        tabControl.setGlyphAlignType(ConstsUI.blGlyphLeft);
        tabControl.setDock(ConstsUI.dsFill);
        AddControl(tabControl);
        ////////////////////////////
        //基本组件1
        //姓名
        LabelUI label1 = new LabelUI();
        label1.setText("姓名");
        label1.setLeft(5);
        label1.setTop(5);
        label1.setAutoSize(true);
        sheet1.AddControl(label1);
        //编辑框
        EditUI edit1 = new EditUI();
        edit1.setWidth(130);
        sheet1.AddControl(edit1);
        //对齐
        sheet1.setAlignment(label1, edit1, ConstsUI.amvTop, 0);
        sheet1.setAlignment(label1, edit1, ConstsUI.amhSpaceEqually, 2);
        //性别
        LabelUI label2 = new LabelUI();
        label2.setText("性别");
        label2.setAutoSize(true);
        sheet1.AddControl(label2);
        sheet1.setAlignment(label1, label2, ConstsUI.amhleft, 0);
        sheet1.setAlignment(label1, label2, ConstsUI.amvSpaceEqually, 6);
        //性别选择
        ComboBoxUI com1 = new ComboBoxUI();
        com1.addItem("男");
        com1.addItem("女");
        com1.setDropDownStyle(ConstsUI.stDropDowmList);
        sheet1.AddControl(com1);
        sheet1.setAlignment(label2, com1, ConstsUI.amvTop, 0);
        sheet1.setAlignment(label2, com1, ConstsUI.amhSpaceEqually, 2);
        //职业
        LabelUI label3 = new LabelUI();
        label3.setText("在职");
        label3.setAutoSize(true);
        sheet1.AddControl(label3);
        sheet1.setAlignment(label2, label3, ConstsUI.amhleft, 0);
        sheet1.setAlignment(label2, label3, ConstsUI.amvSpaceEqually, 6);
        CheckBoxUI checkbox1 = new CheckBoxUI();
        checkbox1.setChecked(true);
        checkbox1.setWidth(50);
        checkbox1.setText("工作中");
        sheet1.AddControl(checkbox1);
        sheet1.setAlignment(label3, checkbox1, ConstsUI.amvTop, 0);
        sheet1.setAlignment(label3, checkbox1, ConstsUI.amhSpaceEqually, 2);
        //工作日期
        LabelUI label4 = new LabelUI();
        label4.setText("日期");
        label4.setAutoSize(true);
        sheet1.AddControl(label4);
        sheet1.setAlignment(label3, label4, ConstsUI.amhleft, 0);
        sheet1.setAlignment(label3, label4, ConstsUI.amvSpaceEqually, 6);
        DateTimePickerUI date1 = new DateTimePickerUI();
        date1.setWidth(130);
        sheet1.AddControl(date1);
        sheet1.setAlignment(label4, date1, ConstsUI.amvTop, 0);
        sheet1.setAlignment(label4, date1, ConstsUI.amhSpaceEqually, 2);
        //列表
        LabelUI label5 = new LabelUI();
        label5.setText("列表");
        label5.setAutoSize(true);
        sheet1.AddControl(label5);
        sheet1.setAlignment(label4, label5, ConstsUI.amhleft, 0);
        sheet1.setAlignment(label4, label5, ConstsUI.amvSpaceEqually, 6);
        ListBoxUI listbox1 = new ListBoxUI();
        listbox1.setShowCheckBox(true);
        listbox1.setShowItemImage(true);
        listbox1.addItem("博士生", ((MIDlet1) Application).image1, false);
        listbox1.addItem("硕士生", ((MIDlet1) Application).image2, false);
        listbox1.addItem("本科生", ((MIDlet1) Application).image1, true);
        listbox1.addItem("大专生", ((MIDlet1) Application).image2, false);
        listbox1.addItem("高中生", ((MIDlet1) Application).image1, false);
        listbox1.addItem("小学生", ((MIDlet1) Application).image2, false);
        listbox1.setWidth(130);
        listbox1.setHeight(80);
        sheet1.AddControl(listbox1);
        sheet1.setAlignment(label5, listbox1, ConstsUI.amvTop, 0);
        sheet1.setAlignment(label5, listbox1, ConstsUI.amhSpaceEqually, 2);
        //年龄
        LabelUI label6 = new LabelUI();
        label6.setText("年龄");
        label6.setAutoSize(true);
        sheet1.AddControl(label6);
        sheet1.setAlignment(label5, label6, ConstsUI.amhleft, 0);
        sheet1.setAlignment(listbox1, label6, ConstsUI.amvSpaceEqually, 6);
        NumbericUpDown num1 = new NumbericUpDown();
        num1.setWidth(130);
        num1.setValue(26);
        sheet1.AddControl(num1);
        sheet1.setAlignment(label6, num1, ConstsUI.amvTop, 0);
        sheet1.setAlignment(label6, num1, ConstsUI.amhSpaceEqually, 2);

        //基本组件2//////////////////////
        //形状
        LabelUI label7 = new LabelUI();
        label7.setText("形状");
        label7.setAutoSize(true);
        label7.setLeft(5);
        label7.setTop(5);
        sheet2.AddControl(label7);
        Bevel bevel1 = new Bevel();
        bevel1.setBackColor(ConstsUI.clWhite);
        bevel1.setWidth(130);
        bevel1.setHeight(6);
        bevel1.setShape(Bevel.bsTopLine);
        sheet2.AddControl(bevel1);
        sheet2.setAlignment(label7, bevel1, ConstsUI.amvCenter, 0);
        sheet2.setAlignment(label7, bevel1, ConstsUI.amhSpaceEqually, 2);
        //进度条
        LabelUI label8 = new LabelUI();
        label8.setText("进度");
        label8.setAutoSize(true);
        sheet2.AddControl(label8);
        sheet2.setAlignment(label7, label8, ConstsUI.amhleft, 0);
        sheet2.setAlignment(label7, label8, ConstsUI.amvSpaceEqually, 6);
        ProgressUI progress1 = new ProgressUI();
        progress1.setWidth(130);
        progress1.setHeight(20);
        progress1.setProgress(50);
        progress1.setDirectionType(ProgressUI.dtHORIZONTAL);
        sheet2.AddControl(progress1);
        sheet2.setAlignment(label8, progress1, ConstsUI.amvTop, 0);
        sheet2.setAlignment(label8, progress1, ConstsUI.amhSpaceEqually, 2);
        //状态栏
        StatusBarUI statusbar = new StatusBarUI();
        StatusPanelUI panel1 = statusbar.addStatusPanel("演示");
        panel1.setWidth(40);
        panel1.setStatusBevel(ConstsUI.pbLowered);
        StatusPanelUI panel2 = statusbar.addStatusPanel("J2MEUI演示系统");
        panel2.setWidth(120);
        panel2.setStatusBevel(ConstsUI.pbLowered);
        sheet2.AddControl(statusbar);
        //滑动条
        LabelUI label9 = new LabelUI();
        label9.setText("滑动");
        label9.setAutoSize(true);
        sheet2.AddControl(label9);
        sheet2.setAlignment(label8, label9, ConstsUI.amhleft, 0);
        sheet2.setAlignment(label8, label9, ConstsUI.amvSpaceEqually, 6);
        TrackBarUI trackbar1 = new TrackBarUI();
        trackbar1.setWidth(130);
        trackbar1.setHeight(15);
        trackbar1.setValue(5);
        trackbar1.setKind(ConstsUI.ktHorizontal);
        sheet2.AddControl(trackbar1);
        sheet2.setAlignment(label9, trackbar1, ConstsUI.amvCenter, 0);
        sheet2.setAlignment(label9, trackbar1, ConstsUI.amhSpaceEqually, 2);
        //滚动条
        LabelUI label10 = new LabelUI();
        label10.setText("滚动");
        label10.setAutoSize(true);
        sheet2.AddControl(label10);
        sheet2.setAlignment(label9, label10, ConstsUI.amhleft, 0);
        sheet2.setAlignment(label9, label10, ConstsUI.amvSpaceEqually, 6);
        ScrollBarUI scrollbar1 = new ScrollBarUI();
        scrollbar1.setWidth(130);
        scrollbar1.setHeight(10);
        scrollbar1.setValue(50);
        scrollbar1.setKind(ConstsUI.ktHorizontal);
        sheet2.AddControl(scrollbar1);
        sheet2.setAlignment(label10, scrollbar1, ConstsUI.amvCenter, 0);
        sheet2.setAlignment(label10, scrollbar1, ConstsUI.amhSpaceEqually, 2);
        //单选
        RadioButtonUI rbtn1 = new RadioButtonUI();
        rbtn1.setGroupIndex(1);
        rbtn1.setText("男生");
        RadioButtonUI rbtn2 = new RadioButtonUI();
        rbtn2.setGroupIndex(1);
        rbtn2.setText("女生");
        sheet2.AddControl(rbtn1);
        sheet2.AddControl(rbtn2);
        sheet2.setAlignment(scrollbar1, rbtn1, ConstsUI.amhleft, 0);
        sheet2.setAlignment(scrollbar1, rbtn1, ConstsUI.amvSpaceEqually, 6);
        sheet2.setAlignment(rbtn1, rbtn2, ConstsUI.amvCenter, 0);
        sheet2.setAlignment(rbtn1, rbtn2, ConstsUI.amhSpaceEqually, 6);

        //图表//////////////////////////////////////////////
        ChartUI chart1 = new ChartUI();
        ChartSeriesUI ser1 = new ChartSeriesUI();
        ser1.setChartType(ChartSeriesUI.ctLine);
        chart1.addSeries(ser1);
        ser1.setLineColor(0x00ff0000);
        ser1.setShowPoint(true);
        ser1.setShowMarks(true);
        ser1.setMarkValueType(ChartSeriesUI.svPercent);
        ser1.setMarksBackColor(0x00ADFF2F);
        ser1.addData(17, 24);
        ser1.addData(14, -11);
        ser1.addData(13, 21);
        ser1.addData(18, 29.7);
        ser1.addData(12, -13);
        ser1.addData(10.7, 16);
        ser1.addData(16, -25);
        ser1.addData(15, -17);
        chart1.setDock(ConstsUI.dsFill);
        chart1.setAxisText("Y轴方向数字");
        chart1.setFootText("X轴方向数字");
        chart1.setTitleText("年度成绩比较");
        chart1.setShowTitleText(true);
        //chart1.setShowAxisText(true);
        chart1.setShowFootText(true);
        sheet3.AddControl(chart1);
        //目录树
        TreeViewUI tree1 = new TreeViewUI();
        TreeNodeUI n1 = tree1.addChild(null, "中国");
        n1.setExpand(true);
        n1.setImage(((MIDlet1) Application).image1);
        n1.addChild("山东").setImage(((MIDlet1) Application).image1, ((MIDlet1) Application).image2);
        TreeNodeUI n2 = n1.addChild("河北");
        n2.setImage(((MIDlet1) Application).image1, ((MIDlet1) Application).image2);
        n2.setExpand(false);
        n2.addChild("唐山").setImage(((MIDlet1) Application).image1, ((MIDlet1) Application).image2);
        n2.addChild("邯郸").setImage(((MIDlet1) Application).image1, ((MIDlet1) Application).image2);
        n2.addChild("沧州").setImage(((MIDlet1) Application).image1, ((MIDlet1) Application).image2);
        n2.addChild("保定").setImage(((MIDlet1) Application).image1, ((MIDlet1) Application).image2);
        n2.addChild("唐山").setImage(((MIDlet1) Application).image1, ((MIDlet1) Application).image2);
        n1.addChild("河南").setImage(((MIDlet1) Application).image1, ((MIDlet1) Application).image2);
        n1.addChild("江苏").setImage(((MIDlet1) Application).image1, ((MIDlet1) Application).image2);
        n1.addChild("浙江").setImage(((MIDlet1) Application).image1, ((MIDlet1) Application).image2);
        tree1.setDock(ConstsUI.dsFill);
        sheet4.AddControl(tree1);
        //表格//////////////////////////////////////
        DataSetUI ds = new DataSetUI();
        ds.setAutoID(true);
        DataSetUI.Field f0 = ds.newField("ID");
        DataSetUI.Field f1 = ds.newField("Name");
        DataSetUI.Field f2 = ds.newField("self");
        DataSetUI.Field f3 = ds.newField("num");
        ds.addFiled(f0);
        ds.addFiled(f1);
        ds.addFiled(f2);
        ds.addFiled(f3);
        /*
        ds.setOnDataFilter(new DataFilterEvent() {
        public void Event(SDADataSet.Row row) {           
        if(Integer.parseInt(row.getValueByFieldName("Age"))==25)
        {
        row.setVisible(true);
        }else
        {
        row.setVisible(false);
        }
        }
        });
         */
        f0.setFiledType(DataSetUI.FieldAudoID);
        //增加数据
        ds.addRow(new String[]{"", "北京分公司", "私营", "20"});
        ds.addRow(new String[]{"", "上海分公司", "私营", "25"});
        ds.addRow(new String[]{"", "南京分公司", "私营", "25"});
        ds.addRow(new String[]{"", "石家庄分公司", "私营", "20"});
        ds.addRow(new String[]{"", "广州分公司", "私营", "20"});

        GridUI grid1 = new GridUI();
        grid1.setScrollBars(ConstsUI.srBoth);
        grid1.setBorderStyle(ConstsUI.bsFixedSingle);
        grid1.setBorderColor(ConstsUI.clBlack);
        grid1.setFixedColor(0x007B9CD6);
        grid1.setBackColor(0x00FAFBEB);
        grid1.setShowFootRow(true);
        grid1.setFootRowColor(0x00FFD700);
        grid1.setDock(ConstsUI.dsFill);
        //增加列
        GridUI.Column d1 = grid1.newColumn("Name", "公司名称");
        d1.setColWidth(100);
        d1.setFootType(GridUI.Column.ftStaticText);
        d1.setFootValue("总数");
        d1.setFootAlignType(ConstsUI.alignRight);
        d1.setTitleAlignType(ConstsUI.alignCenter);
        GridUI.Column d2 = grid1.newColumn("self", "私营");
        d2.setColWidth(50);
        d2.setFootType(GridUI.Column.ftCount);
        d2.setFootAlignType(ConstsUI.alignRight);
        d2.setTitleAlignType(ConstsUI.alignCenter);
        GridUI.Column d3 = grid1.newColumn("num", "人数");
        d3.setColWidth(50);
        d3.setTitleAlignType(ConstsUI.alignCenter);
        d3.setDataAlignType(ConstsUI.alignRight);
        d3.setFootAlignType(ConstsUI.alignRight);
        d3.setFootType(GridUI.Column.ftAvg);
        d3.setFootDisplayFormat("0.0");
        GridUI.Column d4 = grid1.newColumn("ID", "序号");
        d4.setColWidth(40);
        d4.setTitleAlignType(ConstsUI.alignLeft);
        d4.setDataAlignType(ConstsUI.alignLeft);
        grid1.addColumn(d4);
        grid1.addColumn(d1);
        grid1.addColumn(d2);
        grid1.addColumn(d3);
        grid1.setDataSet(ds);
        ds.setFiltered();
        grid1.setOnDrawGridCell(new DrawGridCellEvent() {

            public void Event(GridUI.Cell cell) {
                if (cell.getRowIndex() % 2 > 0) {
                    cell.setBackColor(0x00CEDEF7);
                }
                if (cell.getFieldName().equals("Age")) {
                    if (Integer.parseInt(cell.getFieldValue()) > 20) {
                        cell.setBackColor(0x004AA2E7);
                    }
                }
            }
        });
        sheet5.AddControl(grid1);
        //签字
        //工具按钮
        ToolsBarUI signtool=new ToolsBarUI();
        signtool.addButton("清除签字");
        signtool.addButton("获取图形");
        signtool.setShowCaption(true);
        signtool.setShowButtonRect(true);
        signtool.setDock(ConstsUI.dsTop);
        signtool.setOnClickEvent(new ToolsBarClickEvent() {

            public void Event(ToolButtonUI button, int buttonIndex) {
                if(buttonIndex==0){
                    signpanel.clear();
                }
                if(buttonIndex==1){
                    Image image=signpanel.getSignImage();
                    img.LoadImage(image);
                }
            }
        });
        sheet6.AddControl(signtool);
        //签字板
        //signpanel.setBackColor(SDAConsts.clBtnFace);
        signpanel.setHeight(60);
        signpanel.setDock(ConstsUI.dsTop);
        sheet6.AddControl(signpanel);
        //分隔条
        SpliterUI spliter2=new SpliterUI();
        spliter2.setDock(ConstsUI.dsTop);
        spliter2.setHeight(4);
        sheet6.AddControl(spliter2);
        //图片
        img.setBackColor(ConstsUI.clGray);
        img.setHeight(40);
        img.setDock(ConstsUI.dsFill);
        img.setCenter(true);
        sheet6.AddControl(img);
    }

    protected void onShow() {
    //打开执行
    }

    protected boolean onCloseQuery() {
        Application.MessageBox("提示:", "是否退出J2meUI演示?", ConstsUI.MB_YESNO, new MsgBoxCloseEvent() {

            public void Event(int modalResult) {
                if (modalResult == ConstsUI.mrYes) {
                    Application.Exit();
                }
            }
        });
        return false;
    }
}
