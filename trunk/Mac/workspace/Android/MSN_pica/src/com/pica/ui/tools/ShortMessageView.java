package com.pica.ui.tools;

import com.pica.msn.MainActivity;
import com.pica.msn.R;

import android.content.Context;
import android.graphics.Color;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.ListView;
import android.widget.AdapterView.OnItemClickListener;

public class ShortMessageView extends ListView {
	
	public String[] text_template=new String[] {
    	(String)getContext().getText(R.string.text_template1), 
    	(String)getContext().getText(R.string.text_template2), 
    	(String)getContext().getText(R.string.text_template3), 
    	(String)getContext().getText(R.string.text_template4), 
    	(String)getContext().getText(R.string.text_template5), 
    	(String)getContext().getText(R.string.text_template6), 
    	(String)getContext().getText(R.string.text_template7), 
    	(String)getContext().getText(R.string.text_template8), 
    	(String)getContext().getText(R.string.text_template9), 
    	(String)getContext().getText(R.string.text_template10), 
    	};
	
	public ShortMessageView(Context context) 
	{
		super(context);
		// TODO Auto-generated constructor stub
        setAdapter(new ArrayAdapter<String>(getContext(),
                android.R.layout.simple_list_item_1, text_template
                ));
        setBackgroundColor(Color.WHITE);
	}
}