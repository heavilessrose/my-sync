package com.pica.ui.tools;

import android.content.Context;
import android.util.AttributeSet;
import android.view.Gravity;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.LinearLayout;

import com.pica.msn.MSNApplication;
import com.pica.msn.R;

public class ZwpToolBarView extends LinearLayout{
	MSNApplication app;
	public Button btn1;
	public Button btn2;
	public Button bBack;
	
	public ZwpToolBarView(Context context, AttributeSet attrs) {
		super(context, attrs);
		// TODO Auto-generated constructor stub
		app = MSNApplication.getApplication();
    	if (app == null) {
    		return;
    	}
		setBackgroundResource(R.drawable.middle);
		init();
	}
	private void init()
	{
		setOrientation(LinearLayout.HORIZONTAL);		
		
        LinearLayout linearF = new LinearLayout(getContext());
        linearF.setOrientation(LinearLayout.HORIZONTAL);
        linearF.setGravity(Gravity.CENTER_VERTICAL);
        linearF.setLayoutParams(new ViewGroup.LayoutParams(
                ViewGroup.LayoutParams.FILL_PARENT,
                ViewGroup.LayoutParams.WRAP_CONTENT));

		btn1 = new Button(getContext());
		btn1.setBackgroundResource(R.drawable.home);
		btn2 = new Button(getContext());
		btn2.setBackgroundResource(R.drawable.reload);
        LinearLayout linearH = new LinearLayout(getContext());
        linearH.setOrientation(LinearLayout.HORIZONTAL);
        linearH.setGravity(Gravity.CENTER_VERTICAL);
        linearH.setLayoutParams(new ViewGroup.LayoutParams(
                ViewGroup.LayoutParams.FILL_PARENT,
                ViewGroup.LayoutParams.WRAP_CONTENT));
        linearH.addView(btn1, new LinearLayout.LayoutParams(ViewGroup.LayoutParams.WRAP_CONTENT,ViewGroup.LayoutParams.WRAP_CONTENT));
        linearH.addView(btn2, new LinearLayout.LayoutParams(ViewGroup.LayoutParams.WRAP_CONTENT,ViewGroup.LayoutParams.WRAP_CONTENT));

        linearF.addView(linearH, new LinearLayout.LayoutParams(ViewGroup.LayoutParams.FILL_PARENT,ViewGroup.LayoutParams.WRAP_CONTENT, 1));
    
        addView(linearF, new LinearLayout.LayoutParams(ViewGroup.LayoutParams.FILL_PARENT,ViewGroup.LayoutParams.WRAP_CONTENT));  
	}
}
