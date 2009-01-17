package cn.sda.ui;

import cn.sda.event.KeybordEvent;
import cn.sda.event.PointerEvent;
import cn.sda.event.TreeViewSelectChangeEvent;
import cn.sda.event.TreeViewSelectNodeEvent;
import java.util.Enumeration;
import javax.microedition.lcdui.Graphics;
import java.util.Vector;

/**
 * @author not attributable
 * @version 1.0
 */
public class SDATreeView extends SDABaseControl {
    //边框
    private int borderStyle = SDAConsts.bsFixedSingle;
    //隐藏选定内容
    private boolean hideSelection = true;
    //滚动边框(多行)
    private int scrollBars = SDAConsts.srBoth;
    private int barwidth = 10;
    //边框线颜色
    private int borderColor = SDAConsts.clBlack;
    //滚动条颜色
    private int scrollBarColor = SDAConsts.clBtnFace;
    //选中后字体颜色
    private int selectedFontColor = SDAConsts.clWhite;
    //内部变量
    //行数，行最大长度
    private int maxLineLenght = 0;
    private int maxLineNum = 0;
    //最大可见完整行数
    private int maxVisibleLineNum = 0;
    //垂直开始显示行
    private int startLine = 0;
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
    //行的列表
    private Vector TreeNodes;
    //当前选中行
    private int itemIndex = -1;
    private SDATreeNode CurNode = null;
    //绝对行
    private int absIndex = -1;
    //间隔
    private int indent = 10;
    //临时变量
    private int p_index = 0;
    private int p_lindex = 0;
    private int p_len = 0;
    private SDATreeNode p_node = null;
    //事件
    private TreeViewSelectChangeEvent onSelectChange = null;
    private TreeViewSelectNodeEvent onSelectNode = null;

    public SDATreeView() {
        super();
        internalSDATreeView();
    }

    private void internalSDATreeView() {
        TreeNodes = new Vector();
        this.setVisible(true);
        setLeft(0);
        setTop(0);
        setBorderStyle(SDAConsts.bsFixedSingle);
        super.setWidth(160);
        setHeight(100);
        //高度根据字体高度来
        setHeight(getFont().getHeight() + 4);
        super.setBackColor(SDAConsts.clWhite);
        this.onPointerPressed = new PointerEvent() {

            public void Event(SDABaseControl ctrl, int x, int y) {
                doPointerPressed(x, y);
            }
        };
        this.onPointerReleased = new PointerEvent() {

            public void Event(SDABaseControl ctrl, int x, int y) {
                doPointerReleased(x, y);
            }
        };
        this.onKeyUp = new KeybordEvent() {

            public void Event(SDABaseControl ctrl, int keyCode) {
                doKeyUp(keyCode);
            }
        };
    }
    //处理index(absIndex,layerIndex,index)
    protected void setTreeNodesIndex() {
        SDATreeNode node = null;
        p_index = 0;
        p_lindex = 0;
        for (int i = 0; i < TreeNodes.size(); i++) {
            node = (SDATreeNode) TreeNodes.elementAt(i);
            node.setIndex(i);
            node.setLayerIndex(p_lindex);
            setTreeNodeIndex(node);
        }
    }

    private void setTreeNodeIndex(SDATreeNode node) {
        SDATreeNode cnode = null;
        node.setAbsIndex(p_index);
        node.setTree(this);
        p_index++;
        if (node.hasChild()) {
            p_lindex++;
        }
        int index = 0;
        for (int i = 0; i < node.getChildsCount(); i++) {
            cnode = (SDATreeNode) node.getChild(i);
            cnode.setIndex(index);
            cnode.setTree(this);
            cnode.setLayerIndex(p_lindex);
            index++;
            if (cnode.hasChild()) {
                setTreeNodeIndex(cnode);
            }
        }
        if (node.hasChild()) {
            p_lindex--;
        }
    }
    //增加项
    public SDATreeNode addChild(SDATreeNode parent, String nodeString) {
        return internalAddChild(parent, nodeString);
    }

    private SDATreeNode internalAddChild(SDATreeNode parent, String nodeString) {
        SDATreeNode listitem = new SDATreeNode();
        listitem.setNodeString(nodeString);
        if (parent == null) {
            TreeNodes.addElement(listitem);
        } else {
            parent.addChild(nodeString);
        }
        setTreeNodesIndex();
        internalPaint();
        return listitem;
    }

    public void addChild(SDATreeNode parent, SDATreeNode node) {
        if (parent == null) {
            if (!TreeNodes.contains(node)) {
                TreeNodes.addElement(node);
            }
        } else {
            if (!parent.hasChild(node)) {
                parent.addChild(node);
            }
        }
        setTreeNodesIndex();
        internalPaint();
    }
    //删除项
    public void deleteChild(int nodeIndex) {
        internalDeleteChild(nodeIndex);
    }

    private void internalDeleteChild(int nodeIndex) {
        SDATreeNode node = getNodeFromItemIndex(nodeIndex);
        if (node.hasParent()) {
            node.getParent().removeChild(node);
        } else {
            TreeNodes.removeElement(node);
        }
        node.removeAllChild();
        if (TreeNodes.size() == 0) {
            this.itemIndex = -1;
        }
        node = null;
        setTreeNodesIndex();
        internalPaint();
        System.gc();

    }
    //清除所有项目
    public void clear() {
        internalClear();
    }

    private void internalClear() {
        SDATreeNode node = null;
        for (int i = 0; i < TreeNodes.size(); i++) {
            node = (SDATreeNode) TreeNodes.elementAt(i);
            node.removeAllChild();
        }
        TreeNodes.removeAllElements();
        itemIndex = -1;
        System.gc();
        internalPaint();
    }
    //获取项
    public String getNodeString(int NodeIndex) {
        return internalGetNodeString(NodeIndex);
    }

    private String internalGetNodeString(int NodeIndex) {
        String result = null;
        SDATreeNode node = getNodeFromItemIndex(NodeIndex);
        if (node != null) {
            result = node.getNodeString();
        }
        return result;
    }
    //查找判断是否有指定内容的项
    public SDATreeNode getChild(int Index) {
        return getNodeFromItemIndex(Index);
    }

    public SDATreeNode getChild() {
        return getNodeFromItemIndex(itemIndex);
    }

    //获取所有node列表
    public Enumeration getAllChildsNode() {
        Vector nodes = new Vector();
        SDATreeNode node = null;
        for (int i = 0; i < TreeNodes.size(); i++) {
            node = (SDATreeNode) TreeNodes.elementAt(i);
            getChildNode(nodes, node);
        }
        return nodes.elements();
    }

    private void getChildNode(Vector vt, SDATreeNode node) {
        vt.addElement(node);
        SDATreeNode cnode = null;
        if (node.hasChild()) {
            for (int i = 0; i < node.getChildsCount(); i++) {
                cnode = (SDATreeNode) node.getChild(i);
                getChildNode(vt, cnode);
            }
        }
    }
    //获取所有项目数
    public int getChildsCount() {
        p_len = 0;
        SDATreeNode node = null;
        for (int i = 0; i < TreeNodes.size(); i++) {
            node = (SDATreeNode) TreeNodes.elementAt(i);
            getChildNum(node);
        }
        return p_len;
    }

    private void getChildNum(SDATreeNode node) {
        p_len++;
        SDATreeNode cnode = null;
        if (node.hasChild()) {
            for (int i = 0; i < node.getChildsCount(); i++) {
                cnode = (SDATreeNode) node.getChild(i);
                getChildNum(cnode);
            }
        }
    }

    //画
    public void paint() {
        internalPaint();
    }

    private void internalPaint() {
        if (!IsCanPaint()) {
            return;
        }
        Graphics g = form.getGraphics();
        g.setFont(getFont());
        InternalPaint(g);
        PaintChilds();
    }

    protected void InternalPaint(Graphics g) {
        //可视下才画
        if (isVisible()) {
            //初始化引用变量
            int thisWidth = getWidth();
            int thisHeight = getHeight();
            int HBarWidth = getHBarWidth();
            int VBarHeight = getVBarHeight();
            //对齐方式处理后，画组件
            //设置字体
            g.setFont(getFont());
            //外框
            SetClip(g);
            //透明
            if (!isTransparent()) {
                g.setColor(backColor);
                fillRect(g, 0, 0, thisWidth, thisHeight);
            }
            if (borderStyle == SDAConsts.bsFixed3D) {
                g.setColor(SDAConsts.clGray);
                drawLine(g, 0, 0, thisWidth, 0);
                drawLine(g, 0, 0, 0, thisHeight);
                g.setColor(SDAConsts.clBlack);
                drawLine(g, 1, 1, thisWidth - 1, 1);
                drawLine(g, 1, 1, 1, thisHeight - 1);
                g.setColor(SDAConsts.clBtnFace);
                drawLine(g, 2, thisHeight - 1, thisWidth - 2, thisHeight - 1);
                drawLine(g, thisWidth - 1, 2, thisWidth - 1, thisHeight - 1);
                g.setColor(SDAConsts.clWhite);
                drawLine(g, 1, thisHeight, thisWidth, thisHeight);
                drawLine(g, thisWidth, 1, thisWidth, thisHeight);
                if (isFoucsed()) {
                    g.setColor(SDAConsts.clFocusShadow);
                    drawRect(g, 1, 1, thisWidth - 2, thisHeight - 2);
                }
            }
            if (borderStyle == SDAConsts.bsFixedSingle) {
                //边框
                g.setColor(this.getBorderColor());
                drawRect(g, 0, 0, thisWidth, thisHeight);
                if (isFoucsed()) {
                    g.setColor(SDAConsts.clFocusShadow);
                    drawRect(g, 1, 1, thisWidth - 2, thisHeight - 2);
                }
            }
            //打字     
            g.setColor(getForeColor());
            if (scrollBars == SDAConsts.srHorizontal) {
                SetClip(g, 2, 2, thisWidth - 4, thisHeight - barwidth - 4);
            }
            if (scrollBars == SDAConsts.srVertical) {
                SetClip(g, 2, 2, thisWidth - barwidth - 4, thisHeight - 4);
            }
            if (scrollBars == SDAConsts.srBoth) {
                SetClip(g, 2, 2, thisWidth - barwidth - 4, thisHeight - barwidth - 4);
            }
            SDATreeNode node = null;
            maxLineLenght = getHBarWidth();
            absIndex = 0;
            for (int i = 0; i < TreeNodes.size(); i++) {
                node = (SDATreeNode) TreeNodes.elementAt(i);
                paintNodes(g, node);
            }
            //滚动条
            if (borderStyle == SDAConsts.bsFixed3D) {
                SetClip(g, 2, 2, thisWidth - 2, thisHeight - 2);
            } else {
                SetClip(g, 1, 1, thisWidth - 1, thisHeight - 1);
            }
            if ((scrollBars == SDAConsts.srHorizontal) || (scrollBars == SDAConsts.srBoth)) {
                //水平
                g.setColor(scrollBarColor);
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

                if (maxLineLenght > HBarWidth) {
                    pwidth = ((HBarWidth) * (HBarWidth - 2 * barwidth)) / maxLineLenght;
                }

                //计算块位置
                int ppos = (startLeft * (HBarWidth - 2 * barwidth)) / maxLineLenght;
                //修正位置
                if (ppos + pwidth > HBarWidth - 2 * barwidth) {
                    ppos = HBarWidth - 2 * barwidth - pwidth;
                }
                //画块
                g.setColor(scrollBarColor);
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
            if ((scrollBars == SDAConsts.srVertical) || (scrollBars == SDAConsts.srBoth)) {
                //垂直
                g.setColor(scrollBarColor);
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
                if (maxLineNum * getFont().getHeight() > VBarHeight) {
                    pheight = ((VBarHeight) * (VBarHeight - 2 * barwidth) / (maxLineNum * getFont().getHeight()));
                }
                //计算块位置
                int ppos = barwidth;
                if (maxLineNum > 0) {
                    ppos = (startLine * getFont().getHeight() * (VBarHeight - 2 * barwidth)) / (maxLineNum * getFont().getHeight());
                }
                //修正位置
                if (ppos + pheight > VBarHeight - 2 * barwidth) {
                    ppos = VBarHeight - 2 * barwidth - pheight;
                }
                //画块
                g.setColor(scrollBarColor);
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
        }
    }
    //画节点
    private void paintNodes(Graphics g, SDATreeNode node) {
        getMaxLineInfo();
        //根据传递的SDATreeNode来画
        //int HBarWidth = getHBarWidth();
        int fontHeight = getFont().getHeight();
        int imageWidth = node.getImage() != null ? fontHeight : 0;
        //画自己
        if (node.equals(CurNode)) {
            //画背景
            g.setColor(SDAConsts.clActiveCaption);
            fillRect(g, indent + 4 - startLeft + indent * (node.getLayerIndex() + 1) + imageWidth, 2 + fontHeight * (absIndex - startLine),
                    getFont().stringWidth(node.getNodeString()), fontHeight);
            //线
            g.setColor(getForeColor());
            drawDotLine(g, 2 - startLeft + indent * (node.getLayerIndex() + 1), 2 + fontHeight * (absIndex - startLine) + fontHeight / 2,
                    indent, 0);
            p_len = 0;
            getVLineLength(node);
            if (p_len > 0) {
                drawDotLine(g, indent + 2 - startLeft + indent * (node.getLayerIndex() + 1),
                        2 + fontHeight * (absIndex - startLine) + fontHeight / 2, 0, p_len * fontHeight);
            }
            //画图标
            if (node.getImage() != null) {
                if (node.getSelectedImage() != null) {
                    drawImage(g, node.getSelectedImage(), indent + 4 - startLeft + indent * (node.getLayerIndex() + 1),
                            1 + fontHeight * (absIndex - startLine) + (fontHeight - node.getSelectedImage().getHeight()) / 2, 0);
                } else {
                    drawImage(g, node.getImage(), indent + 4 - startLeft + indent * (node.getLayerIndex() + 1),
                            1 + fontHeight * (absIndex - startLine) + (fontHeight - node.getImage().getHeight()) / 2, 0);
                }
                //打字
                g.setColor(selectedFontColor);
                drawString(g, node.getNodeString(), indent + 4 - startLeft + indent * (node.getLayerIndex() + 1) + imageWidth,
                        2 + fontHeight * (absIndex - startLine));
            } else {
                //打字
                g.setColor(selectedFontColor);
                drawString(g, node.getNodeString(), indent + 4 - startLeft + indent * (node.getLayerIndex() + 1),
                        2 + fontHeight * (absIndex - startLine));
            }
        } else {
            //打字
            g.setColor(getForeColor());
            drawDotLine(g, 2 - startLeft + indent * (node.getLayerIndex() + 1), 2 + fontHeight * (absIndex - startLine) + fontHeight / 2,
                    indent, 0);
            p_len = 0;
            getVLineLength(node);
            if (p_len > 0) {
                drawDotLine(g, indent + 2 - startLeft + indent * (node.getLayerIndex() + 1),
                        2 + fontHeight * (absIndex - startLine) + fontHeight / 2, 0, p_len * fontHeight);
            }
            //画图标
            if (node.getImage() != null) {
                drawImage(g, node.getImage(), indent + 4 - startLeft + indent * (node.getLayerIndex() + 1),
                        1 + fontHeight * (absIndex - startLine) + (fontHeight - node.getImage().getHeight()) / 2, 0);
                //打字
                drawString(g, node.getNodeString(), indent + 4 - startLeft + indent * (node.getLayerIndex() + 1) + imageWidth,
                        2 + fontHeight * (absIndex - startLine));
            } else {
                //打字
                drawString(g, node.getNodeString(), indent + 4 - startLeft + indent * (node.getLayerIndex() + 1),
                        2 + fontHeight * (absIndex - startLine));
            }
        }
        absIndex++;
        //遍历下一级
        SDATreeNode cnode = null;
        //int 
        if (node.isExpand()) {
            if (node.hasChild()) {
                g.setColor(backColor);
                fillRect(g, 2 - startLeft + indent * (node.getLayerIndex() + 1) - 4,
                        2 + fontHeight * (absIndex - startLine) - fontHeight / 2 - 4,
                        8, 8);
                g.setColor(foreColor);
                drawRect(g, 2 - startLeft + indent * (node.getLayerIndex() + 1) - 4,
                        2 + fontHeight * (absIndex - startLine) - fontHeight / 2 - 4,
                        8, 8);
                drawLine(g, 2 - startLeft + indent * (node.getLayerIndex() + 1) - 4,
                        2 + fontHeight * (absIndex - startLine) - fontHeight / 2,
                        2 - startLeft + indent * (node.getLayerIndex() + 1) + 4,
                        2 + fontHeight * (absIndex - startLine) - fontHeight / 2);
                for (int i = 0; i < node.getChildsCount(); i++) {
                    cnode = node.getChild(i);
                    paintNodes(g, cnode);
                }
            }
        } else {
            if (node.hasChild()) {
                g.setColor(backColor);
                fillRect(g, 2 - startLeft + indent * (node.getLayerIndex() + 1) - 4,
                        2 + fontHeight * (absIndex - startLine) - fontHeight / 2 - 4,
                        8, 8);
                g.setColor(foreColor);
                drawRect(g, 2 - startLeft + indent * (node.getLayerIndex() + 1) - 4,
                        2 + fontHeight * (absIndex - startLine) - fontHeight / 2 - 4,
                        8, 8);
                drawLine(g, 2 - startLeft + indent * (node.getLayerIndex() + 1) - 4,
                        2 + fontHeight * (absIndex - startLine) - fontHeight / 2,
                        2 - startLeft + indent * (node.getLayerIndex() + 1) + 4,
                        2 + fontHeight * (absIndex - startLine) - fontHeight / 2);
                drawLine(g, 2 - startLeft + indent * (node.getLayerIndex() + 1),
                        2 + fontHeight * (absIndex - startLine) - fontHeight / 2 - 4,
                        2 - startLeft + indent * (node.getLayerIndex() + 1),
                        2 + fontHeight * (absIndex - startLine) - fontHeight / 2 + 4);
            }
        }
    }

    //获取向下画的线长度
    private void getVLineLength(SDATreeNode node) {
        if (node.hasChild() && node.isExpand()) {
            SDATreeNode cnode = null;
            for (int i = 0; i < node.getChildsCount(); i++) {
                cnode = node.getChild(i);
                p_len++;
                if (cnode.hasChild() && cnode.isExpand()) {
                    getVLineLength(cnode);
                }
            }
        }
    }
    //根据位置获取Node
    private SDATreeNode getNodeFromItemIndex(int index) {
        SDATreeNode node = null;
        int clickLine = index;
        p_len = -1;
        p_node = null;
        for (int i = 0; i < TreeNodes.size(); i++) {
            node = (SDATreeNode) TreeNodes.elementAt(i);
            calClickLine(node, clickLine);
        }
        return p_node;
    }

    private void calClickLine(SDATreeNode node, int lineID) {
        p_len++;
        if (p_len == lineID) {
            p_node = node;
            return;
        }
        SDATreeNode cnode = null;
        if (node.isExpand() && node.hasChild()) {
            for (int i = 0; i < node.getChildsCount(); i++) {
                cnode = node.getChild(i);
                calClickLine(cnode, lineID);
            }
        }
    }

    public int getSelectedFontColor() {
        return selectedFontColor;
    }

    public void setSelectedFontColor(int selectedFontColor) {
        this.selectedFontColor = selectedFontColor;
        internalPaint();
    }

    public int getBorderStyle() {
        return borderStyle;
    }

    public void setBorderStyle(int borderStyle) {
        this.borderStyle = borderStyle;
        internalPaint();
    }

    public boolean isHideSelection() {
        return hideSelection;
    }

    public void setHideSelection(boolean hideSelection) {
        this.hideSelection = hideSelection;
        internalPaint();
    }

    public int getScrollBars() {
        return scrollBars;
    }

    public void setScrollBars(int scrollBars) {
        this.scrollBars = scrollBars;
        internalPaint();
    }

    public int getScrollBarWidth() {
        return barwidth;
    }

    public void setScrollBarWidth(int barwidth) {
        this.barwidth = barwidth;
        internalPaint();
    }

    private void getMaxLineInfo() {
        //获取总行数
        p_len = 0;
        maxLineLenght = getHBarWidth();
        SDATreeNode node = null;
        for (int i = 0; i < TreeNodes.size(); i++) {
            node = (SDATreeNode) TreeNodes.elementAt(i);
            getNodeNum(node);
        }
        maxLineNum = p_len;
        if (maxLineNum <= getmaxVisibleLineNum()) {
            startLine = 0;
        }
    }

    private void getNodeNum(SDATreeNode node) {
        p_len++;
        int cwidth = getFont().stringWidth(node.getNodeString()) +
                indent * (node.getLayerIndex() + 2) + (node.getImage() != null ? getFont().getHeight() : 0);
        if (maxLineLenght < cwidth) {
            maxLineLenght = cwidth;
        }
        SDATreeNode cnode = null;
        if (node.hasChild() && node.isExpand()) {
            for (int i = 0; i < node.getChildsCount(); i++) {
                cnode = node.getChild(i);
                getNodeNum(cnode);
            }
        }
    }

    private int getmaxVisibleLineNum() {
        maxVisibleLineNum = getVBarHeight() / getFont().getHeight();
        if (maxVisibleLineNum > maxLineNum) {
            maxVisibleLineNum = maxLineNum;
        }
        return maxVisibleLineNum;
    }

    public int getBorderColor() {
        return borderColor;
    }

    public void setBorderColor(int borderColor) {
        this.borderColor = borderColor;
        internalPaint();
    }

    public int getScrollBarColor() {
        return scrollBarColor;
    }

    public void setScrollBarColor(int scrollBarColor) {
        this.scrollBarColor = scrollBarColor;
        internalPaint();
    }

    public int getIndent() {
        return indent;
    }

    public void setIndent(int indent) {
        this.indent = indent;
    }

    private int getHBarWidth() {
        int swidth = 0;
        if ((scrollBars == SDAConsts.srHorizontal) || (scrollBars == SDAConsts.srNone)) {
            swidth = getWidth();
        }
        if ((scrollBars == SDAConsts.srBoth) || (scrollBars == SDAConsts.srVertical)) {
            swidth = getWidth() - barwidth;
        }
        return swidth;
    }

    private int getVBarHeight() {
        int sheight = 0;
        if ((scrollBars == SDAConsts.srVertical) || (scrollBars == SDAConsts.srNone)) {
            sheight = getHeight();
        }
        if ((scrollBars == SDAConsts.srBoth) || (scrollBars == SDAConsts.srHorizontal)) {
            sheight = getHeight() - barwidth;
        }
        return sheight;
    }

    public int getItemIndex() {
        return itemIndex;
    }

    public void setItemIndex(int itemIndex) {
        this.itemIndex = itemIndex;
        internalPaint();
    }
    //处理事件的执行
    //点箭头滚动内容
    protected void doPointerPressed(int x, int y) {
        int posx = screenXToClient(x);
        int posy = screenYToClient(y);
        int thisWidth = getWidth();
        int thisHeight = getHeight();
        int VBarHeight = getVBarHeight();
        int HBarWidth = getHBarWidth();
        int fontHeight = getFont().getHeight();
        //确定点击了滚动条区域
        if ((scrollBars == SDAConsts.srHorizontal) || (scrollBars == SDAConsts.srBoth)) {
            //判断是否点击了左箭头
            if (InClientRect(posx, posy, 0, thisHeight - barwidth, barwidth, barwidth)) {
                //向右滚动
                if (startLeft > 0) {
                    int step = getFont().charWidth('x');
                    startLeft -= step;
                    if (startLeft < 0) {
                        startLeft = 0;
                    }
                }
            } else //右箭头
            if (InClientRect(posx, posy, HBarWidth - barwidth, thisHeight - barwidth, barwidth, barwidth)) {
                //向左滚动
                if (maxLineLenght - startLeft > HBarWidth) {
                    int step = getFont().charWidth('x');
                    startLeft += step;
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
                    //点了空白的，滚动到点击的位置
                    //计算滚动块要到位置
                    int tpos = posx > HSLeft ? (posx - HSWidth) : (posx);
                    //计算StartLeft
                    startLeft = ((tpos - barwidth) * (maxLineLenght)) / (HBarWidth - 2 * barwidth);
                }
            }
        }

        if ((scrollBars == SDAConsts.srVertical) || (scrollBars == SDAConsts.srBoth)) {

            //只有垂直滚动条
            //判断是否点击了上箭头
            if (InClientRect(posx, posy, thisWidth - barwidth, 0, barwidth, barwidth)) {
                //向下滚动
                if (startLine > 0) {
                    startLine--;
                }
            } else //下箭头
            if (InClientRect(posx, posy, thisWidth - barwidth, VBarHeight - barwidth, barwidth, barwidth)) {
                //向上滚动
                startLine = ((maxLineNum - startLine) * fontHeight > VBarHeight) ? startLine + 1 : startLine;
            } else //滚动条
            if (InClientRect(posx, posy, VSLeft, VSTop, VSWidth, VSHeight)) {
                //记录位置
                oldScrollPointx = posx;
                oldScrollPointy = posy;
                isscrollbarpointdown = true;
                scrollbardownHV = 1;
                oldStartLine = startLine;
            } else {
                if (InClientRect(posx, posy, thisWidth - barwidth, 0, barwidth, VBarHeight)) {
                    //空白的
                    //计算滚动块要到位置
                    int tpos = posy > VSTop ? (posy - VSHeight) : (posy);
                    //计算StartLine
                    int oldline = startLine;
                    startLine = ((tpos - barwidth) * (maxLineNum * fontHeight)) / (VBarHeight - 2 * barwidth) / fontHeight;
                    if (oldline == startLine) {
                        startLine = posy > VSTop ? startLine + 1 : startLine - 1;
                    }
                }

            }
        }
        //点到展开按钮
        if (InClientRect(posx, posy, 0, 0, HBarWidth, VBarHeight)) {
            itemIndex = posy / fontHeight + startLine;
            SDATreeNode node = getNodeFromItemIndex(itemIndex);
            if (node != null) {
                CurNode = node;
                doSelectChange();
                doSelectNode();
                if (node.hasChild()) {
                    if (InClientRect(posx, posy, indent * (node.getLayerIndex() + 1) - 2 - startLeft, (itemIndex - startLine + 1) * fontHeight - fontHeight / 2 - 2, 8, 8)) {
                        node.setExpand(!node.isExpand());
                    }
                }
            }
        }
    }
    //拖动事件处理
    protected void doPointerReleased(int x, int y) {
        int posx = screenXToClient(x);
        int posy = screenYToClient(y);
        int VBarHeight = getVBarHeight();
        int HBarWidth = getHBarWidth();
        //根据点击的位置,判断滚动的多少
        if ((scrollBars == SDAConsts.srHorizontal) || ((scrollBars == SDAConsts.srBoth) && (scrollbardownHV == 0))) {
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
                } else {
                    oldStartLeft = startLeft;
                    if (stepx > 0) {
                        startLeft = maxLineLenght - startLeft < getHBarWidth() ? startLeft + 1 : startLeft;
                    }
                    if (stepx < 0) {
                        startLeft = startLeft == 0 ? 0 : startLeft - 1;
                    }
                }
            }
        }
        if ((scrollBars == SDAConsts.srVertical) || ((scrollBars == SDAConsts.srBoth) && (scrollbardownHV == 1))) {
            if (isscrollbarpointdown) {
                int stepy = posy - oldScrollPointy;
                //根据滚动多少来重新定位
                //计算滚动块要到位置
                int tpos = VSTop + stepy;
                VSTop = tpos < barwidth ? barwidth : tpos;
                VSTop = VSTop + VSHeight > VBarHeight - barwidth ? VBarHeight - barwidth - VSHeight : VSTop;
                //计算StartLine
                startLine = ((VSTop - barwidth) * (maxLineNum * getFont().getHeight())) / (VBarHeight - 2 * barwidth) / getFont().getHeight();
                if (oldStartLine == startLine) {
                    if (stepy > 0) {
                        startLine = ((maxLineNum - startLine) * getFont().getHeight() > getVBarHeight()) ? startLine + 1 : startLine;
                    }
                    if (stepy < 0) {
                        startLine = startLine > 0 ? startLine - 1 : startLine;
                    }
                    oldStartLine = startLine;
                } else {
                    if ((stepy > 0) && (startLine < oldStartLine)) {
                        startLine = oldStartLine;
                    }
                    oldStartLine = startLine;
                }
            }
        }
        isscrollbarpointdown = false;
    }
    //键盘事件处理
    protected void doKeyUp(int keyCode) {
        maxVisibleLineNum = getmaxVisibleLineNum();
        String key = form.getKeyName(keyCode).toUpperCase();
        try {
            if (key.equals(SDAConsts.KEY_UP)) {
                if (itemIndex > startLine) {
                    itemIndex--;
                    return;
                }
                //上
                if (startLine > 0) {
                    startLine--;
                    itemIndex = startLine;
                }
            }
            if (key.equals(SDAConsts.KEY_DOWN)) {
                if (itemIndex < startLine + maxVisibleLineNum - 1) {
                    itemIndex++;
                    return;
                }
                //下
                startLine = ((maxLineNum - startLine) * getFont().getHeight() > getVBarHeight()) ? startLine + 1 : startLine;
                itemIndex = startLine + maxVisibleLineNum - 1;
            }
            if (itemIndex < startLine) {
                itemIndex = startLine;
            }
            if (itemIndex > startLine + maxVisibleLineNum) {
                itemIndex = startLine + maxVisibleLineNum;
            }
        } finally {
            getCurNode();
            doSelectChange();
            if (key.equals(SDAConsts.KEY_LEFT)) {
                setExp(false);
            }
            if (key.equals(SDAConsts.KEY_RIGHT)) {
                //右
                setExp(true);
            }
            repaintControl();
        }
    }

    private void setExp(boolean expand) {
        SDATreeNode node = getNodeFromItemIndex(itemIndex);
        if (node != null) {
            CurNode = node;
            if (node.hasChild()) {
                node.setExpand(expand);
            }
        }
    }

    public SDATreeNode getCurNode() {
        SDATreeNode node = getNodeFromItemIndex(itemIndex);
        if (node != null) {
            CurNode = node;
        }
        return CurNode;
    }

    public void setOnSelectChange(TreeViewSelectChangeEvent onSelectChange) {
        this.onSelectChange = onSelectChange;
    }

    public void setOnSelectNode(TreeViewSelectNodeEvent onSelectNode) {
        this.onSelectNode = onSelectNode;
    }

    private void doSelectChange() {
        if (this.onSelectChange != null) {
            if (CurNode != null) {
                onSelectChange.Event(CurNode);
            }
        }
    }

    private void doSelectNode() {
        if (this.onSelectNode != null) {
            if (CurNode != null) {
                onSelectNode.Event(CurNode);
            }
        }
    }

    protected boolean canDownTabNext() {
        boolean result=false;
        if(absIndex==getChildsCount()-1){
            result=true;
        }
        return result;
    }

    protected boolean canRightTabNext() {
        boolean result=false;
        if(absIndex==-1||absIndex==0){
            result=true;
        }
        return result;
    }

    protected boolean canUpTabPrior() {
        return false;
    }

    protected boolean canLeftTabPrior() {
        return false;
    }
    
}

