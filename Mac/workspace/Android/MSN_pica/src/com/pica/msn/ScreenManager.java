/**
 * 
 */
package com.pica.msn;

import java.util.Stack;

import android.app.Activity;

public class ScreenManager {
     private static Stack<Activity> activityStack;  
     private static ScreenManager instance; 
     private Activity main;
     private Activity chat;
     public  ScreenManager(){  
     }  
 
     public void popActivity(){  
         Activity activity=activityStack.lastElement();  
         if(activity!=null){  
             activity.finish();  
             activity=null;  
         }  
     }  
     public void popActivity(Activity activity){  
         if(activity!=null){  
             //activity.finish();  
             activityStack.remove(activity);  
             //activity=null;  
         }  
     }  
     public Activity currentActivity(){  
         Activity activity=activityStack.lastElement();  
         return activity;  
     }  
     public void pushActivity(Activity activity){  
         if(activityStack==null){  
             activityStack=new Stack<Activity>();  
         }  
         activityStack.add(activity);  
     }  
       
     public void popAllActivityExceptOne(Class cls){  
         while(true){  
             Activity activity=currentActivity();  
             if(activity==null){  
                 break;  
             }  
             if(activity.getClass().equals(cls) ){  
                 break;  
             }  
             popActivity(activity);  
         }  
     }
     public void setMain(Activity ac)
     {
         main = ac;
     }
     public void setChat(Activity ac)
     {
         chat = ac;
     } 
     
     public void popMain()
     {
         main = null;
     }
     public void popChat()
     {
         chat = null;
     }  
     
     public void KillActivity()
     {
         Activity activity=currentActivity();  
         if(activity==null){  
             return;
         }  
         
         if(main != null)
         {
             if(activity != main)
             {
                 if(chat != null)
                 {
                     if(activity != chat)
                     {
                         activity.finish();
                     }
                 }
                 else
                 {
                     activity.finish();
                 }
             } 
         }
     }
}
