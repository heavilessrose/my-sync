package com.pica.ui.tools;

import android.content.Context;
import android.graphics.Color;
import android.view.Gravity;
import android.view.ViewGroup;
import android.widget.AbsListView;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.TextView;

import com.pica.msn.R;

public class GroupView extends LinearLayout
{
	public int groupIndex;
	public GroupView(Context context, boolean isExpanded, String str, int index)
	{
		super(context);
    	setBackgroundColor(Color.WHITE);
    	setOrientation(LinearLayout.VERTICAL);
    	
    	groupIndex = index;
    	
		AbsListView.LayoutParams lp = new AbsListView.LayoutParams(
                ViewGroup.LayoutParams.FILL_PARENT, ViewGroup.LayoutParams.WRAP_CONTENT);
		setLayoutParams(lp);
		LinearLayout layout = new LinearLayout(getContext());
		setOrientation(LinearLayout.HORIZONTAL);
//		setGravity(Gravity.CENTER_VERTICAL);		
		ImageView expandImage = new ImageView(context);
		if(isExpanded)
		{
			expandImage.setImageResource(R.drawable.group_expanded);
		}
		else
		{
			expandImage.setImageResource(R.drawable.group_unexpanded);
		}        		
		layout.addView(expandImage, new LinearLayout.LayoutParams(ViewGroup.LayoutParams.WRAP_CONTENT,ViewGroup.LayoutParams.WRAP_CONTENT));
		
    	TextView text = new TextView(context);
    	text.setTextSize(18);
    	text.setText(str);
    	layout.addView(text, new LinearLayout.LayoutParams(ViewGroup.LayoutParams.WRAP_CONTENT,ViewGroup.LayoutParams.WRAP_CONTENT));
    	addView(layout, new LinearLayout.LayoutParams(ViewGroup.LayoutParams.FILL_PARENT,ViewGroup.LayoutParams.WRAP_CONTENT));
    	
//    	LinearLayout lineLayout = new LinearLayout(getContext());
//    	lineLayout.setOrientation(LinearLayout.VERTICAL);
//    	ImageView lineView = new ImageView(getContext());
//    	lineView.setImageResource(R.drawable.list_line);
//        lineLayout.addView(lineView, new LinearLayout.LayoutParams(ViewGroup.LayoutParams.FILL_PARENT,ViewGroup.LayoutParams.WRAP_CONTENT));
//        addView(lineLayout, new LinearLayout.LayoutParams(ViewGroup.LayoutParams.FILL_PARENT,ViewGroup.LayoutParams.WRAP_CONTENT));
	}

}
