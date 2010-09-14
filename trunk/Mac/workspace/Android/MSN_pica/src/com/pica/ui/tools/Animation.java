package com.pica.ui.tools;
import java.util.*;

import com.pica.ui.tools.AnimationListener;
public class Animation extends TimerTask
{
	private AnimationListener listen;
	private Object tag;
	public Animation(AnimationListener ev,Object tag)
	{
		this.listen = ev;
		this.tag = tag;
	}
	
	public void run()
	{
		listen.TimerNotify(tag);
	}
}
