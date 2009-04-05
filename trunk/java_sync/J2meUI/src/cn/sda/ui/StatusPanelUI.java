package cn.sda.ui;

/**
 *
 * @author Administrator
 */
public class StatusPanelUI {

    private int highLightColor=ConstsUI.clWhite;
    private int shadowColor=ConstsUI.clBtnShadow;
    private byte statusBevel=ConstsUI.pbNone;
    private String text="";
    private int alignType=ConstsUI.alignLeft;
    private int width=50;
    
    private StatusBarUI parentBar=null;

    public int getAlignType() {
        return alignType;
    }

    public void setAlignType(int alignType) {
        this.alignType = alignType;
    }

    public byte getStatusBevel() {
        return statusBevel;
    }

    public void setStatusBevel(byte statusBevel) {
        this.statusBevel = statusBevel;
        doRepaint();
    }

    public String getText() {
        return text;
    }

    public void setText(String text) {
        this.text = text;
        doRepaint();
    }

    public int getWidth() {
        return width;
    }

    public void setWidth(int width) {
        this.width = width;
        doRepaint();
    }

    public int getHighLightColor() {
        return highLightColor;
    }

    public void setHighLightColor(int highLightColor) {
        this.highLightColor = highLightColor;
        doRepaint();
    }

    public int getShadowColor() {
        return shadowColor;
    }

    public void setShadowColor(int shadowColor) {
        this.shadowColor = shadowColor;
        doRepaint();
    }

    protected void setParentBar(StatusBarUI parentBar) {
        this.parentBar = parentBar;
    }
    private void doRepaint(){
        if(this.parentBar!=null){
            parentBar.repaintControl();
        }
    }
}
