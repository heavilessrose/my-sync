package cn.sda.ui;

import java.util.*;

import javax.microedition.lcdui.*;

import cn.sda.event.*;

public class FormUI extends BaseControl {

	protected String caption = "Form";
	private SDAFormCaption formCatpion = null;
	private SDAFormContainer formContainer = null;
	protected final MainApp Application = MainApp.GetInstance();
	protected Canvas canvas = null;
	protected Graphics graphics = null;
	protected BaseControl focusControl = null;
	private BaseControl operateControl = null;
	protected BaseControl popControl = null;
	private boolean inputAbort = false;
	private InputPanel inputPanel = null;
	private Hashtable hotKeyList = new Hashtable();
	// 标题
	private Image captionBackImage = null;
	private int captionAlignType = ConstsUI.alignLeft;
	private Image captionIcon = null;
	private boolean showCloseButton = true;
	// 主菜单
	private MainMenuUI mainMemu = null;

	private void InitForm(String caption) {
		this.caption = caption;
		this.form = this;
		this.width = 100;
		this.height = 100;
		this.setBackColor(ConstsUI.clWhite);
		this.graphics = Application.GetGraphics();
		this.canvas = Application.GetCanvas();
		this.formContainer = new SDAFormContainer();
		this.formContainer.form = this;

		this.formContainer.setWidth(this.canvas.getWidth());
		this.formContainer.setHeight(this.canvas.getHeight());
		this.formCatpion = new SDAFormCaption();
		this.formCatpion.form = this;
		this.formCatpion.setDock(ConstsUI.dsTop);
		this.formContainer.AddControl(this.formCatpion);
		this.setDock(ConstsUI.dsFill);
		this.formContainer.AddControl(this);
	}

	public FormUI() {
		InitForm("Form");
	}

	public FormUI(String caption) {
		InitForm(caption);
	}

	public void setLeft(int left) {
		this.formContainer.left = left;
	}

	public void setTop(int top) {
		this.formContainer.top = top;
	}

	public void setHeight(int height) {
		this.formContainer.height = height;
	}

	public void setWidth(int width) {
		this.formContainer.width = width;
	}

	// 初始化方法
	public Canvas GetCanvas() {
		return this.canvas;
	}

	protected void closePopCtrl() {
		this.inputAbort = true;
		this.popControl = null;
	}

	protected String getKeyName(int keyCode) {
		return String.valueOf(keyCode);
	}

	private void NextTabOrder() {
		// System.out.println("NextTabOrder");
		BaseControl nextTabCtrl = null;
		if (focusControl != null) {
			nextTabCtrl = focusControl.FindNextFocusedCtrl(null, false);
			if (nextTabCtrl == null && focusControl.parent != null) {
				nextTabCtrl = focusControl.parent.FindNextFocusedCtrl(
						focusControl, false);
			}
		}
		if (nextTabCtrl == null) {
			nextTabCtrl = this.FindNextFocusedCtrl(null, false);
		}
		if (nextTabCtrl != null) {
			nextTabCtrl.setFoucsed();
		}
	}

	private void PriorTabOrder() {
		// System.out.println("PriorTabOrder");
		BaseControl nextTabCtrl = null;
		if (focusControl != null && focusControl.parent != null) {
			nextTabCtrl = focusControl.parent.FindNextFocusedCtrl(focusControl,
					true);
		}
		if (nextTabCtrl == null) {
			nextTabCtrl = this.FindNextFocusedCtrl(null, true);
		}
		if (nextTabCtrl != null) {
			nextTabCtrl.setFoucsed();
		}
	}

	public void Repaint() {
		this.paint();
		serviceRepaints();
	}

	private final void DrawForm() {
		// ConsoleInfo("DrawFormBegin,width="+this.width+",height="+this.height);
		SetClip(graphics, 0, 0, this.width, this.height);
		if (Application.getSkins() == null) {
			graphics.setColor(this.getBackColor());
			fillRect(graphics, 1, 0, this.width - 2, this.height - 1);
		} else {
			Image skin = Application.getSkins().getImage("form_bg.png");
			skin = ImageUtils.processImage(skin, this.width - 2,
					this.height - 1, ImageUtils.MODE_STRETCH);
			drawImage(graphics, skin, 1, 0, 0);
		}
	}

	public void paint() {
		this.ctl3d = Application.isCtrl3d();
		this.formContainer.paint();
		this.formCatpion.paint();

		if (!IsCanPaint()) {
			return;
		}
		DrawForm();
		PaintChilds();
		if (this.equals(Application.getFoucsedForm())) {
			if (focusControl != null && !focusControl.equals(this)) {
				focusControl.paint();
			}
			if (popControl != null && !popControl.equals(focusControl)) {
				popControl.paint();
			}
			if (inputPanel != null) {
				inputPanel.paint();
			}
		}
	}

	protected final void pointerPressed(int x, int y) {
		if (this.formCatpion.visible && this.formCatpion.InScreenRect(x, y)) {
			if (this.formCatpion.onPointerPressed != null) {
				this.formCatpion.onPointerPressed.Event(null, x, y);
			}
			return;
		}
		inputAbort = false;
		if (popControl != null) {
			if (popControl.onPointerPressed != null) {
				popControl.onPointerPressed.Event(popControl, x, y);
			}
		} else {
			BaseControl ctrl = FindControl(x, y);
			operateControl = ctrl;
			if (operateControl != null) {
				operateControl.setFoucsed();
				if (!operateControl.isDown()) {
					operateControl.setDown(true);
				}
				if (operateControl.onPointerPressed != null) {
					operateControl.onPointerPressed.Event(ctrl, x, y);
				}

			}
		}
		if (this.equals(Application.getFoucsedForm())) {
			this.serviceRepaints();
		}
	}

	protected final void pointerReleased(int x, int y) {
		if (inputAbort) {
			return;
		}
		if (this.formCatpion.visible && this.formCatpion.InScreenRect(x, y)) {
			if (this.formCatpion.onPointerReleased != null) {
				this.formCatpion.onPointerReleased.Event(null, x, y);
			}
			return;
		}
		if (popControl != null) {
			if (popControl.onPointerReleased != null) {
				popControl.onPointerReleased.Event(popControl, x, y);
			}
			if (popControl != null && !popControl.InScreenRect(x, y)) {
				popControl = null;
				this.paint();
			}
		} else {
			if (operateControl != null) {

				if (operateControl.isDown()) {
					operateControl.setDown(false);
				}
				if (operateControl.InScreenRect(x, y)) {
					if (operateControl.onPointerReleased != null) {
						operateControl.onPointerReleased.Event(operateControl,
								x, y);
					}
					operateControl.Click();
				}
			}
		}
		operateControl = null;
		if (this.equals(Application.getFoucsedForm())) {
			this.serviceRepaints();
		}

	}

	protected final void pointerDragged(int x, int y) {
		if (inputAbort) {
			return;
		}
		if (popControl != null) {
			if (popControl.onPointerDragged != null) {
				popControl.onPointerDragged.Event(popControl, x, y);
				if (popControl != null) {
					popControl.paint();
				}
			}
		} else {
			if (operateControl != null) {
				if (operateControl.InScreenRect(x, y)) {
					operateControl.setDown(true);
				} else {
					operateControl.setDown(false);
				}
				operateControl.paint();
			}
			BaseControl ctrl = FindControl(x, y);
			if (ctrl != null && ctrl.onPointerDragged != null) {
				ctrl.onPointerDragged.Event(ctrl, x, y);
				ctrl.paint();
			}
		}
		if (this.equals(Application.getFoucsedForm())) {
			this.serviceRepaints();
		}
	}

	protected final void keyPressed(int keyCode) {
		inputAbort = false;
		if (popControl != null) {
			if (popControl.onKeyDown != null) {
				popControl.onKeyDown.Event(popControl, keyCode);
				if (popControl != null) {
					popControl.paint();
				}
			}
		} else {
			// keydown
			this.operateControl = focusControl;
			if (operateControl != null && operateControl.onKeyDown != null) {
				operateControl.onKeyDown.Event(operateControl, keyCode);
				operateControl.paint();
			}
		}
		if (this.equals(Application.getFoucsedForm())) {
			this.serviceRepaints();
		}
	}

	protected final void keyReleased(int keyCode) {
		String keyName = getKeyName(keyCode).toUpperCase();
		Object hotKeyEvent = hotKeyList.get(keyName);
		if (hotKeyEvent != null) {
			((HotKeyEvent) hotKeyEvent).Event(keyName);
			inputAbort = true;
		}

		if (inputAbort) {
			return;
		}
		if (popControl != null) {
			if (popControl.onKeyUp != null) {
				popControl.onKeyUp.Event(popControl, keyCode);
				if (popControl != null) {
					popControl.paint();
				}
			}
		} else {
			if (focusControl == null
					|| (keyName.equals(ConstsUI.KEY_UP) && focusControl
							.canUpTabPrior())
					|| (keyName.equals(ConstsUI.KEY_LEFT) && focusControl
							.canLeftTabPrior())) {
				this.PriorTabOrder();
				operateControl = null;
			}
			if (focusControl == null
					|| (keyName.equals(ConstsUI.KEY_DOWN) && focusControl
							.canDownTabNext())
					|| (keyName.equals(ConstsUI.KEY_RIGHT) && focusControl
							.canRightTabNext())) {
				this.NextTabOrder();
				operateControl = null;
			}
			if (operateControl != null) {
				if (operateControl.onKeyUp != null) {
					operateControl.onKeyUp.Event(operateControl, keyCode);
				}
				if (keyName.equals(ConstsUI.KEY_SELECT)
						|| keyName.equals(ConstsUI.KEY_ENTER)) {
					operateControl.Click();
				}
			}
		}

		if (this.equals(Application.getFoucsedForm())) {
			this.serviceRepaints();
		}
	}

	protected void InternalShow() {
		this.onShow();
		inputAbort = true;
		this.visible = true;
		this.operateControl = null;
		Application.ShowForm(this);
	}

	public void Show() {
		setInputPanel(Application.inputPanel);
		InternalShow();
	}

	public void Close() {
		if (!this.onCloseQuery()) {
			return;
		}
		this.onClose();
		inputAbort = true;
		this.visible = false;
		this.focusControl = null;
		Application.CloseForm(this);
	}

	public String getCaption() {
		return caption;
	}

	public void setCaption(String caption) {
		this.caption = caption;
	}

	protected void setInputAbort(boolean inputAbort) {
		this.inputAbort = inputAbort;
	}

	public void setInputPanel(InputPanel inputPanel) {
		if (this.inputPanel == null) {
			this.inputPanel = inputPanel;
			this.AddControl(this.inputPanel);
			AddControl(inputPanel.charsPanel);
		} else {
			this.inputPanel.parent = this;
			this.inputPanel.form = this;
			this.inputPanel.charsPanel.parent = this;
			this.inputPanel.charsPanel.form = this;
		}
	}

	public boolean isShowCaption() {
		return this.formCatpion.visible;
	}

	public void setShowCaption(boolean showCaption) {
		this.formCatpion.visible = showCaption;
	}

	public Graphics getGraphics() {
		return graphics;
	}

	public BaseControl getOperateControl() {
		return operateControl;
	}

	public void registerHotKey(String keyName, HotKeyEvent enent) {
		hotKeyList.put(keyName, enent);
	}

	private class SDAFormCaption extends BaseControl {

		public SDAFormCaption() {
			this.height = getFont().getHeight() + 2;
			this.width = 100;
			this.tabStop = false;
			backColor = ConstsUI.clActiveCaption;
			foreColor = ConstsUI.clWhite;
			setOnPointerPressed(new PointerEvent() {

				public void Event(BaseControl ctrl, int x, int y) {
					doPointerPressed(x, y);
				}
			});
		}

		private final void DrawCaption() {
			graphics.setColor(backColor);
			fillRect(graphics, 0, 0, this.width, this.height);
			// 背景
			if (captionBackImage != null) {
				Image img = ImageUtils.processImage(captionBackImage, width,
						height, ImageUtils.MODE_STRETCH);
				drawImage(graphics, img, 0, 0, 0);
			}
			// 图标
			if (captionIcon != null) {
				drawImage(graphics, captionIcon, 2, (height - captionIcon
						.getHeight()) / 2, 0);
			}
			// 字体
			graphics.setColor(foreColor);
			graphics.setFont(getFont());
			int leftPos = 0;
			if (captionAlignType == ConstsUI.alignLeft) {
				leftPos = 2 + (captionIcon != null ? getFont().getHeight() : 0);
			}
			if (captionAlignType == ConstsUI.alignCenter) {
				leftPos = (width - getFont().stringWidth(caption)) / 2;
			}
			if (captionAlignType == ConstsUI.alignRight) {
				leftPos = width - getFont().stringWidth(caption) - height - 1;
			}
			drawString(graphics, this.form.caption, leftPos, 1);
			// 关闭按钮
			graphics.setColor(foreColor);
			drawRect(graphics, width - 4 * height / 5 - 1, height / 5 - 1,
					3 * height / 5, 3 * height / 5);
			// 画叉
			drawLine(graphics, width - 4 * height / 5 - 1, height / 5 - 1,
					width - height / 5 - 1, 4 * height / 5 - 1);
			drawLine(graphics, width - 4 * height / 5 - 1, 4 * height / 5 - 1,
					width - height / 5 - 1, height / 5 - 1);
		}

		public void paint() {
			if (IsCanPaint()) {
				SetClip(graphics, 0, 0, this.width, this.height);
				DrawCaption();
			}
		}

		private void doPointerPressed(int x, int y) {
			int posx = screenXToClient(x);
			int posy = screenYToClient(y);
			if (InClientRect(posx, posy, width - height, 0, height, height)) {
				// 关闭
				form.Close();
			}
		}
	}

	private class SDAFormContainer extends BaseControl {

		public SDAFormContainer() {
			this.setLeft(0);
			this.setTop(0);
		}

		public void paint() {
			if (IsCanPaint()) {
				SetClip(this.form.graphics, 0, 0, this.width, this.height);
				this.form.graphics.setColor(ConstsUI.clBtnShadow);
				drawRect(this.form.graphics, 0, 0, this.width - 1,
						this.height - 1);
				setChildsLayout();
				// 画主菜单

			}
		}
	}

	public int getCaptionAlignType() {
		return captionAlignType;
	}

	public void setCaptionAlignType(int captionAlignType) {
		this.captionAlignType = captionAlignType;
	}

	public Image getCaptionBackImage() {
		return captionBackImage;
	}

	public void setCaptionBackImage(Image captionBackImage) {
		this.captionBackImage = captionBackImage;
	}

	public Image getCaptionIcon() {
		return captionIcon;
	}

	public void setCaptionIcon(Image captionIcon) {
		this.captionIcon = captionIcon;
	}

	public boolean isShowCloseButton() {
		return showCloseButton;
	}

	public void setShowCloseButton(boolean showCloseButton) {
		this.showCloseButton = showCloseButton;
	}

	protected void onShow() {

	}

	protected boolean onCloseQuery() {
		return true;
	}

	protected void onClose() {

	}

	public MainMenuUI getMainMemu() {
		return mainMemu;
	}

	public void setMainMemu(MainMenuUI mainMemu) {
		if (this.mainMemu != null) {
			RemoveControl(this.mainMemu);
			this.mainMemu = null;
		}
		if (mainMemu != null) {
			this.mainMemu = mainMemu;
			if (!ctrlList.contains(mainMemu)) {
				ctrlList.addElement(mainMemu);
				mainMemu.parent = this;
				mainMemu.SetForm(this);
			}
		}
	}
}
