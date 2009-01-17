package cn.sda.demo;

import cn.sda.event.DrawGridCellEvent;
import cn.sda.event.MainMenuOnButtonEvent;
import cn.sda.event.MsgBoxCloseEvent;
import cn.sda.event.ToolsBarClickEvent;
import cn.sda.ui.SDABevel;
import cn.sda.ui.SDAChart;
import cn.sda.ui.SDAChartSeries;
import cn.sda.ui.SDACheckBox;
import cn.sda.ui.SDAComboBox;
import cn.sda.ui.SDAConsts;
import cn.sda.ui.SDADataSet;
import cn.sda.ui.SDADateTimePicker;
import cn.sda.ui.SDAEdit;
import cn.sda.ui.SDAForm;
import cn.sda.ui.SDAGrid;
import cn.sda.ui.SDAImage;
import cn.sda.ui.SDALabel;
import cn.sda.ui.SDAListBox;
import cn.sda.ui.SDAMainMenu;
import cn.sda.ui.SDANumbericUpDown;
import cn.sda.ui.SDAPopMenu;
import cn.sda.ui.SDAProgress;
import cn.sda.ui.SDARadioButton;
import cn.sda.ui.SDAScrollBar;
import cn.sda.ui.SDASignPanel;
import cn.sda.ui.SDASpliter;
import cn.sda.ui.SDAStatusBar;
import cn.sda.ui.SDAStatusPanel;
import cn.sda.ui.SDATabControl;
import cn.sda.ui.SDATabSheet;
import cn.sda.ui.SDAToolButton;
import cn.sda.ui.SDAToolsBar;
import cn.sda.ui.SDATrackBar;
import cn.sda.ui.SDATreeNode;
import cn.sda.ui.SDATreeView;
import javax.microedition.lcdui.Image;

/**
 *
 * @author Administrator
 */
public class MainForm extends SDAForm {

    //工具栏
    SDAToolsBar toolbar = new SDAToolsBar();
    //菜单
    SDAMainMenu mainmenu = new SDAMainMenu();
    //页框组件
    SDATabControl tabControl = new SDATabControl();
    //签字板
    SDASignPanel signpanel=new SDASignPanel();
    //图形
    SDAImage img=new SDAImage();

    public MainForm() {
        //工具栏
        toolbar.setDock(SDAConsts.dsTop);
        toolbar.addButton("新建", ((MIDlet1) Application).image1);
        toolbar.addButton("编辑", ((MIDlet1) Application).image2);
        toolbar.addSeparator();
        toolbar.addButton("删除", ((MIDlet1) Application).image1);
        toolbar.addButton("设置", ((MIDlet1) Application).image2);
        toolbar.setShowCaption(true);
        toolbar.setBackImage(((MIDlet1) Application).image3);
        toolbar.setGlyphAlignType(SDAConsts.blGlyphLeft);
        AddControl(toolbar);
        //分割拖动条
        SDASpliter spliter1 = new SDASpliter();
        spliter1.setMinSize(22);
        spliter1.setHeight(4);
        spliter1.setDock(SDAConsts.dsTop);
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

            public void Event(SDAPopMenu memu, String MenuButtonCaption) {
                if (MenuButtonCaption.equals("关闭")) {
                    Application.Exit();
                }
            }
        });
        setMainMemu(mainmenu);
        //页框组件
        //基本组件页1
        SDATabSheet sheet1 = tabControl.addTabSheet();
        sheet1.setCaption("组件1");
        sheet1.setImage(((MIDlet1) Application).image1);
        //基本组件页2
        SDATabSheet sheet2 = tabControl.addTabSheet("组件2", ((MIDlet1) Application).image2);
        //图表页
        SDATabSheet sheet3 = tabControl.addTabSheet("图表", ((MIDlet1) Application).image1);
        //目录树页
        SDATabSheet sheet4 = tabControl.addTabSheet("目录树", ((MIDlet1) Application).image2);
        //表格页
        SDATabSheet sheet5 = tabControl.addTabSheet("表格", ((MIDlet1) Application).image1);
        //签字
        SDATabSheet sheet6 = tabControl.addTabSheet("签字", ((MIDlet1) Application).image2);
        //设置
        tabControl.setButtonPosition(SDAConsts.tabButtonsBottom);
        tabControl.setGlyphAlignType(SDAConsts.blGlyphLeft);
        tabControl.setDock(SDAConsts.dsFill);
        AddControl(tabControl);
        ////////////////////////////
        //基本组件1
        //姓名
        SDALabel label1 = new SDALabel();
        label1.setText("姓名");
        label1.setLeft(5);
        label1.setTop(5);
        label1.setAutoSize(true);
        sheet1.AddControl(label1);
        //编辑框
        SDAEdit edit1 = new SDAEdit();
        edit1.setWidth(130);
        sheet1.AddControl(edit1);
        //对齐
        sheet1.setAlignment(label1, edit1, SDAConsts.amvTop, 0);
        sheet1.setAlignment(label1, edit1, SDAConsts.amhSpaceEqually, 2);
        //性别
        SDALabel label2 = new SDALabel();
        label2.setText("性别");
        label2.setAutoSize(true);
        sheet1.AddControl(label2);
        sheet1.setAlignment(label1, label2, SDAConsts.amhleft, 0);
        sheet1.setAlignment(label1, label2, SDAConsts.amvSpaceEqually, 6);
        //性别选择
        SDAComboBox com1 = new SDAComboBox();
        com1.addItem("男");
        com1.addItem("女");
        com1.setDropDownStyle(SDAConsts.stDropDowmList);
        sheet1.AddControl(com1);
        sheet1.setAlignment(label2, com1, SDAConsts.amvTop, 0);
        sheet1.setAlignment(label2, com1, SDAConsts.amhSpaceEqually, 2);
        //职业
        SDALabel label3 = new SDALabel();
        label3.setText("在职");
        label3.setAutoSize(true);
        sheet1.AddControl(label3);
        sheet1.setAlignment(label2, label3, SDAConsts.amhleft, 0);
        sheet1.setAlignment(label2, label3, SDAConsts.amvSpaceEqually, 6);
        SDACheckBox checkbox1 = new SDACheckBox();
        checkbox1.setChecked(true);
        checkbox1.setWidth(50);
        checkbox1.setText("工作中");
        sheet1.AddControl(checkbox1);
        sheet1.setAlignment(label3, checkbox1, SDAConsts.amvTop, 0);
        sheet1.setAlignment(label3, checkbox1, SDAConsts.amhSpaceEqually, 2);
        //工作日期
        SDALabel label4 = new SDALabel();
        label4.setText("日期");
        label4.setAutoSize(true);
        sheet1.AddControl(label4);
        sheet1.setAlignment(label3, label4, SDAConsts.amhleft, 0);
        sheet1.setAlignment(label3, label4, SDAConsts.amvSpaceEqually, 6);
        SDADateTimePicker date1 = new SDADateTimePicker();
        date1.setWidth(130);
        sheet1.AddControl(date1);
        sheet1.setAlignment(label4, date1, SDAConsts.amvTop, 0);
        sheet1.setAlignment(label4, date1, SDAConsts.amhSpaceEqually, 2);
        //列表
        SDALabel label5 = new SDALabel();
        label5.setText("列表");
        label5.setAutoSize(true);
        sheet1.AddControl(label5);
        sheet1.setAlignment(label4, label5, SDAConsts.amhleft, 0);
        sheet1.setAlignment(label4, label5, SDAConsts.amvSpaceEqually, 6);
        SDAListBox listbox1 = new SDAListBox();
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
        sheet1.setAlignment(label5, listbox1, SDAConsts.amvTop, 0);
        sheet1.setAlignment(label5, listbox1, SDAConsts.amhSpaceEqually, 2);
        //年龄
        SDALabel label6 = new SDALabel();
        label6.setText("年龄");
        label6.setAutoSize(true);
        sheet1.AddControl(label6);
        sheet1.setAlignment(label5, label6, SDAConsts.amhleft, 0);
        sheet1.setAlignment(listbox1, label6, SDAConsts.amvSpaceEqually, 6);
        SDANumbericUpDown num1 = new SDANumbericUpDown();
        num1.setWidth(130);
        num1.setValue(26);
        sheet1.AddControl(num1);
        sheet1.setAlignment(label6, num1, SDAConsts.amvTop, 0);
        sheet1.setAlignment(label6, num1, SDAConsts.amhSpaceEqually, 2);

        //基本组件2//////////////////////
        //形状
        SDALabel label7 = new SDALabel();
        label7.setText("形状");
        label7.setAutoSize(true);
        label7.setLeft(5);
        label7.setTop(5);
        sheet2.AddControl(label7);
        SDABevel bevel1 = new SDABevel();
        bevel1.setBackColor(SDAConsts.clWhite);
        bevel1.setWidth(130);
        bevel1.setHeight(6);
        bevel1.setShape(SDABevel.bsTopLine);
        sheet2.AddControl(bevel1);
        sheet2.setAlignment(label7, bevel1, SDAConsts.amvCenter, 0);
        sheet2.setAlignment(label7, bevel1, SDAConsts.amhSpaceEqually, 2);
        //进度条
        SDALabel label8 = new SDALabel();
        label8.setText("进度");
        label8.setAutoSize(true);
        sheet2.AddControl(label8);
        sheet2.setAlignment(label7, label8, SDAConsts.amhleft, 0);
        sheet2.setAlignment(label7, label8, SDAConsts.amvSpaceEqually, 6);
        SDAProgress progress1 = new SDAProgress();
        progress1.setWidth(130);
        progress1.setHeight(20);
        progress1.setProgress(50);
        progress1.setDirectionType(SDAProgress.dtHORIZONTAL);
        sheet2.AddControl(progress1);
        sheet2.setAlignment(label8, progress1, SDAConsts.amvTop, 0);
        sheet2.setAlignment(label8, progress1, SDAConsts.amhSpaceEqually, 2);
        //状态栏
        SDAStatusBar statusbar = new SDAStatusBar();
        SDAStatusPanel panel1 = statusbar.addStatusPanel("演示");
        panel1.setWidth(40);
        panel1.setStatusBevel(SDAConsts.pbLowered);
        SDAStatusPanel panel2 = statusbar.addStatusPanel("J2MEUI演示系统");
        panel2.setWidth(120);
        panel2.setStatusBevel(SDAConsts.pbLowered);
        sheet2.AddControl(statusbar);
        //滑动条
        SDALabel label9 = new SDALabel();
        label9.setText("滑动");
        label9.setAutoSize(true);
        sheet2.AddControl(label9);
        sheet2.setAlignment(label8, label9, SDAConsts.amhleft, 0);
        sheet2.setAlignment(label8, label9, SDAConsts.amvSpaceEqually, 6);
        SDATrackBar trackbar1 = new SDATrackBar();
        trackbar1.setWidth(130);
        trackbar1.setHeight(15);
        trackbar1.setValue(5);
        trackbar1.setKind(SDAConsts.ktHorizontal);
        sheet2.AddControl(trackbar1);
        sheet2.setAlignment(label9, trackbar1, SDAConsts.amvCenter, 0);
        sheet2.setAlignment(label9, trackbar1, SDAConsts.amhSpaceEqually, 2);
        //滚动条
        SDALabel label10 = new SDALabel();
        label10.setText("滚动");
        label10.setAutoSize(true);
        sheet2.AddControl(label10);
        sheet2.setAlignment(label9, label10, SDAConsts.amhleft, 0);
        sheet2.setAlignment(label9, label10, SDAConsts.amvSpaceEqually, 6);
        SDAScrollBar scrollbar1 = new SDAScrollBar();
        scrollbar1.setWidth(130);
        scrollbar1.setHeight(10);
        scrollbar1.setValue(50);
        scrollbar1.setKind(SDAConsts.ktHorizontal);
        sheet2.AddControl(scrollbar1);
        sheet2.setAlignment(label10, scrollbar1, SDAConsts.amvCenter, 0);
        sheet2.setAlignment(label10, scrollbar1, SDAConsts.amhSpaceEqually, 2);
        //单选
        SDARadioButton rbtn1 = new SDARadioButton();
        rbtn1.setGroupIndex(1);
        rbtn1.setText("男生");
        SDARadioButton rbtn2 = new SDARadioButton();
        rbtn2.setGroupIndex(1);
        rbtn2.setText("女生");
        sheet2.AddControl(rbtn1);
        sheet2.AddControl(rbtn2);
        sheet2.setAlignment(scrollbar1, rbtn1, SDAConsts.amhleft, 0);
        sheet2.setAlignment(scrollbar1, rbtn1, SDAConsts.amvSpaceEqually, 6);
        sheet2.setAlignment(rbtn1, rbtn2, SDAConsts.amvCenter, 0);
        sheet2.setAlignment(rbtn1, rbtn2, SDAConsts.amhSpaceEqually, 6);

        //图表//////////////////////////////////////////////
        SDAChart chart1 = new SDAChart();
        SDAChartSeries ser1 = new SDAChartSeries();
        ser1.setChartType(SDAChartSeries.ctLine);
        chart1.addSeries(ser1);
        ser1.setLineColor(0x00ff0000);
        ser1.setShowPoint(true);
        ser1.setShowMarks(true);
        ser1.setMarkValueType(SDAChartSeries.svPercent);
        ser1.setMarksBackColor(0x00ADFF2F);
        ser1.addData(17, 24);
        ser1.addData(14, -11);
        ser1.addData(13, 21);
        ser1.addData(18, 29.7);
        ser1.addData(12, -13);
        ser1.addData(10.7, 16);
        ser1.addData(16, -25);
        ser1.addData(15, -17);
        chart1.setDock(SDAConsts.dsFill);
        chart1.setAxisText("Y轴方向数字");
        chart1.setFootText("X轴方向数字");
        chart1.setTitleText("年度成绩比较");
        chart1.setShowTitleText(true);
        //chart1.setShowAxisText(true);
        chart1.setShowFootText(true);
        sheet3.AddControl(chart1);
        //目录树
        SDATreeView tree1 = new SDATreeView();
        SDATreeNode n1 = tree1.addChild(null, "中国");
        n1.setExpand(true);
        n1.setImage(((MIDlet1) Application).image1);
        n1.addChild("山东").setImage(((MIDlet1) Application).image1, ((MIDlet1) Application).image2);
        SDATreeNode n2 = n1.addChild("河北");
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
        tree1.setDock(SDAConsts.dsFill);
        sheet4.AddControl(tree1);
        //表格//////////////////////////////////////
        SDADataSet ds = new SDADataSet();
        ds.setAutoID(true);
        SDADataSet.Field f0 = ds.newField("ID");
        SDADataSet.Field f1 = ds.newField("Name");
        SDADataSet.Field f2 = ds.newField("self");
        SDADataSet.Field f3 = ds.newField("num");
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
        f0.setFiledType(SDADataSet.FieldAudoID);
        //增加数据
        ds.addRow(new String[]{"", "北京分公司", "私营", "20"});
        ds.addRow(new String[]{"", "上海分公司", "私营", "25"});
        ds.addRow(new String[]{"", "南京分公司", "私营", "25"});
        ds.addRow(new String[]{"", "石家庄分公司", "私营", "20"});
        ds.addRow(new String[]{"", "广州分公司", "私营", "20"});

        SDAGrid grid1 = new SDAGrid();
        grid1.setScrollBars(SDAConsts.srBoth);
        grid1.setBorderStyle(SDAConsts.bsFixedSingle);
        grid1.setBorderColor(SDAConsts.clBlack);
        grid1.setFixedColor(0x007B9CD6);
        grid1.setBackColor(0x00FAFBEB);
        grid1.setShowFootRow(true);
        grid1.setFootRowColor(0x00FFD700);
        grid1.setDock(SDAConsts.dsFill);
        //增加列
        SDAGrid.Column d1 = grid1.newColumn("Name", "公司名称");
        d1.setColWidth(100);
        d1.setFootType(SDAGrid.Column.ftStaticText);
        d1.setFootValue("总数");
        d1.setFootAlignType(SDAConsts.alignRight);
        d1.setTitleAlignType(SDAConsts.alignCenter);
        SDAGrid.Column d2 = grid1.newColumn("self", "私营");
        d2.setColWidth(50);
        d2.setFootType(SDAGrid.Column.ftCount);
        d2.setFootAlignType(SDAConsts.alignRight);
        d2.setTitleAlignType(SDAConsts.alignCenter);
        SDAGrid.Column d3 = grid1.newColumn("num", "人数");
        d3.setColWidth(50);
        d3.setTitleAlignType(SDAConsts.alignCenter);
        d3.setDataAlignType(SDAConsts.alignRight);
        d3.setFootAlignType(SDAConsts.alignRight);
        d3.setFootType(SDAGrid.Column.ftAvg);
        d3.setFootDisplayFormat("0.0");
        SDAGrid.Column d4 = grid1.newColumn("ID", "序号");
        d4.setColWidth(40);
        d4.setTitleAlignType(SDAConsts.alignLeft);
        d4.setDataAlignType(SDAConsts.alignLeft);
        grid1.addColumn(d4);
        grid1.addColumn(d1);
        grid1.addColumn(d2);
        grid1.addColumn(d3);
        grid1.setDataSet(ds);
        ds.setFiltered();
        grid1.setOnDrawGridCell(new DrawGridCellEvent() {

            public void Event(SDAGrid.Cell cell) {
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
        SDAToolsBar signtool=new SDAToolsBar();
        signtool.addButton("清除签字");
        signtool.addButton("获取图形");
        signtool.setShowCaption(true);
        signtool.setShowButtonRect(true);
        signtool.setDock(SDAConsts.dsTop);
        signtool.setOnClickEvent(new ToolsBarClickEvent() {

            public void Event(SDAToolButton button, int buttonIndex) {
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
        signpanel.setDock(SDAConsts.dsTop);
        sheet6.AddControl(signpanel);
        //分隔条
        SDASpliter spliter2=new SDASpliter();
        spliter2.setDock(SDAConsts.dsTop);
        spliter2.setHeight(4);
        sheet6.AddControl(spliter2);
        //图片
        img.setBackColor(SDAConsts.clGray);
        img.setHeight(40);
        img.setDock(SDAConsts.dsFill);
        img.setCenter(true);
        sheet6.AddControl(img);
    }

    protected void onShow() {
    //打开执行
    }

    protected boolean onCloseQuery() {
        Application.MessageBox("提示:", "是否退出J2meUI演示?", SDAConsts.MB_YESNO, new MsgBoxCloseEvent() {

            public void Event(int modalResult) {
                if (modalResult == SDAConsts.mrYes) {
                    Application.Exit();
                }
            }
        });
        return false;
    }
}
