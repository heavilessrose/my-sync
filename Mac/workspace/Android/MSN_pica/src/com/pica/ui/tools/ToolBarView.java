package com.pica.ui.tools;

import java.lang.reflect.Array;

import com.pica.msn.MSNApplication;
import com.pica.msn.R;

import android.content.Context;
import android.content.res.Configuration;
import android.content.res.Resources;
import android.graphics.Color;
import android.view.Gravity;
import android.view.LayoutInflater;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.RelativeLayout;
import android.widget.LinearLayout.LayoutParams;

public class ToolBarView extends LinearLayout{
	MSNApplication app;
	public Button btn1;
	public Button btn2;
	public Button btn3;
	public Button btn4;
	public Button btn5;
	public Button bSend;
	
	public ToolBarView(Context context) {
		super(context);
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
                ViewGroup.LayoutParams.FILL_PARENT));

		btn1 = new Button(getContext());
		btn1.setBackgroundResource(R.drawable.btn1);
		btn2 = new Button(getContext());
		btn2.setBackgroundResource(R.drawable.btn2);
		btn3 = new Button(getContext());
		btn3.setBackgroundResource(R.drawable.btn3);
		btn4 = new Button(getContext());
		btn4.setBackgroundResource(R.drawable.btn4);
		btn5 = new Button(getContext());
		btn5.setBackgroundResource(R.drawable.btn5);
        LinearLayout linearH = new LinearLayout(getContext());
        linearH.setOrientation(LinearLayout.HORIZONTAL);
        linearH.setGravity(Gravity.CENTER_VERTICAL);
        linearH.addView(btn1, new LinearLayout.LayoutParams(ViewGroup.LayoutParams.WRAP_CONTENT,ViewGroup.LayoutParams.WRAP_CONTENT));
        linearH.addView(btn2, new LinearLayout.LayoutParams(ViewGroup.LayoutParams.WRAP_CONTENT,ViewGroup.LayoutParams.WRAP_CONTENT));
        linearH.addView(btn3, new LinearLayout.LayoutParams(ViewGroup.LayoutParams.WRAP_CONTENT,ViewGroup.LayoutParams.WRAP_CONTENT));
        linearH.addView(btn4, new LinearLayout.LayoutParams(ViewGroup.LayoutParams.WRAP_CONTENT,ViewGroup.LayoutParams.WRAP_CONTENT));
        linearH.addView(btn5, new LinearLayout.LayoutParams(ViewGroup.LayoutParams.WRAP_CONTENT,ViewGroup.LayoutParams.WRAP_CONTENT));

        linearF.addView(linearH, new LinearLayout.LayoutParams(ViewGroup.LayoutParams.FILL_PARENT,ViewGroup.LayoutParams.FILL_PARENT, 1));

        bSend = new Button(getContext());
        bSend.setText(R.string.text_send);
        LinearLayout linearS = new LinearLayout(getContext());
        linearS.setOrientation(LinearLayout.HORIZONTAL);
        linearS.setGravity(Gravity.CENTER_VERTICAL);
        linearS.addView(bSend, new LinearLayout.LayoutParams(ViewGroup.LayoutParams.WRAP_CONTENT,ViewGroup.LayoutParams.WRAP_CONTENT));
        linearF.addView(linearS, new LinearLayout.LayoutParams(ViewGroup.LayoutParams.WRAP_CONTENT,ViewGroup.LayoutParams.WRAP_CONTENT));
    
        addView(linearF, new LinearLayout.LayoutParams(ViewGroup.LayoutParams.FILL_PARENT,ViewGroup.LayoutParams.WRAP_CONTENT));  
	}
}
