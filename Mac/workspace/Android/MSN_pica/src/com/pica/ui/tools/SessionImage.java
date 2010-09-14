/**
 * 
 */
package com.pica.ui.tools;

import com.pica.msn.MSNApplication;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Paint;
import android.widget.ImageView;

public class SessionImage extends ImageView{

    /**
     * @param context
     */
    private MSNApplication app;
    private int num = 0;
    private int type = 0;
    private Paint paint;
    int startX=31;
    int startY=18;
    public SessionImage(Context context, int type) {
        super(context);
        app = MSNApplication.getApplication();
        this.type = type;
        paint = new Paint();
        int size = (int)(14*app.screenScale);
        paint.setTextSize(size);
        paint.setAntiAlias(true);
        paint.setColor(0xffffffff);
        startY = (int)(18*app.screenScale);
        // TODO Auto-generated constructor stub
    }
    
    public void setNum(int num)
    {
        this.num = num;
    }
    
    protected void onDraw (Canvas canvas)
    {
        super.onDraw(canvas);
        
        if(type == 1)
        {
            if(num == 0)
            {
                
            }
            else if(num > 0 && num <= 9)
            {
                startX = (int)(31*app.screenScale);
                canvas.drawText(String.valueOf(num), startX, startY, paint);
            }
            else if(num >= 10 && num <= 99)
            {
                startX = (int)(23*app.screenScale);
                canvas.drawText(String.valueOf(num), startX, startY, paint);
            }
            else
            {
                startX = (int)(19*app.screenScale);
                canvas.drawText(String.valueOf(num), startX, startY, paint);
            }
        }
    }
}
