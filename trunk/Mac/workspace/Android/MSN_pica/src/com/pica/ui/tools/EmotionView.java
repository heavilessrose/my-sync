package com.pica.ui.tools;
import com.pica.msn.MSNApplication;

import android.content.Context;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.GridView;
import android.widget.ImageView;

public class EmotionView extends GridView{
    int frame = 45;
    static MSNApplication app;
	public EmotionView(Context context) {
		super(context);
		app = MSNApplication.getApplication();
		frame = (int)(45*app.screenScale);
		setAdapter(new ImageAdapter(getContext()));
		setNumColumns(8);
	}
	
    public class ImageAdapter extends BaseAdapter {
        public ImageAdapter(Context c) {
            mContext = c;
        }

        public int getCount() {
            return EmotionTextView.mThumbIds.length;
        }

        public Object getItem(int position) {
            return position;
        }

        public long getItemId(int position) {
            return position;
        }

        public View getView(int position, View convertView, ViewGroup parent) {
            ImageView imageView;
            if (convertView == null) {
                imageView = new ImageView(mContext);
                imageView.setLayoutParams(new GridView.LayoutParams(frame, frame));
                imageView.setAdjustViewBounds(false);
                imageView.setScaleType(ImageView.ScaleType.CENTER_CROP);
                imageView.setPadding(8, 8, 8, 8);
            } else {
                imageView = (ImageView) convertView;
            }

            imageView.setImageResource(EmotionTextView.mThumbIds[position]);

            return imageView;
        }

        private Context mContext;
    }

}
