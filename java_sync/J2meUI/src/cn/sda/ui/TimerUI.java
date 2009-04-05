package cn.sda.ui;

import cn.sda.event.TimerTickEvent;
import java.util.Timer;
import java.util.TimerTask;

/**
 *
 * @author Administrator
 */
public class TimerUI {

    private int interval = 1000;
    private boolean enabled = false;
    private TimerTickEvent onTick = null;
    private Timer timer = null;
    private TimerTask timertask = null;

    public TimerUI() {

    }

    public boolean isEnabled() {
        return enabled;
    }

    public void setEnabled(boolean enabled) {
        if (onTick != null) {
            this.enabled = enabled;
            if (enabled) {
                timer = new Timer();
                timertask = new TimerTask() {

                    public void run() {
                        //时钟执行
                        doOnTick();
                    }
                };
                timer.schedule(timertask, interval, interval);
            } else {
                timertask.cancel();
                timer.cancel();
                timertask = null;
                timer = null;
            }
        }
    }

    public int getInterval() {
        return interval;
    }

    public void setInterval(int interval) {
        this.interval = interval;
    }

    public void setOnTick(TimerTickEvent onTick) {
        this.onTick = onTick;
    }

    private void doOnTick() {
        if (onTick != null) {
            onTick.Event();
        }
    }
}
