package com.pica.ui.tools;

//import java.util.HashMap;
//import java.util.Map;
import android.content.Context;
import android.content.res.Configuration;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Color;
import android.graphics.drawable.BitmapDrawable;
import android.view.Gravity;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AbsListView;
import android.widget.BaseExpandableListAdapter;
import android.widget.ExpandableListView;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.TextView;

import com.pica.msn.Contact;
import com.pica.msn.Group;
import com.pica.msn.MSNApplication;
import com.pica.msn.MsnUtil;
import com.pica.msn.R;

public class RosterList extends ExpandableListView{
	MSNApplication app;
	//public ImageView adImage;
	private int iDes = 0;
	public MyExpandableListAdapter mAdapter;
	Context mContext;
	byte[] AD_ImageData;
	public AdImageView adImage;
	public String selectRoster_imid;
	public int selectRoster_groupIndex;
	public int lastSelectGroupIndex = -1;
	public RosterList(Context context) {
		super(context);
		mContext = context;
		app = MSNApplication.getApplication();
    	if (app == null) {
    		return;
    	}
    	adImage = new AdImageView(mContext);
    	//adImage = new ImageView(context);
    	this.setIndicatorBounds(-1, -1);

        mAdapter = new MyExpandableListAdapter();
        setAdapter(mAdapter);
        //this.setIndicatorBounds(-1, -1);
	}
	
	public void UpdateView()
	{
		mAdapter.notifyDataSetChanged();
	}

	public void getPortaitHash(Contact contact)
	{
        if(contact != null)
        {
            if (!contact.CONTACT_hasSendGetImage)// 该联系人的头像哈系不为空，并且没有发过包
            {
                app.jabber.getContactPortrait(contact.SUBITEM_JID);
                contact.CONTACT_hasSendGetImage = true;
            }
        }
	}
	
	public void updateADImage()
	{
		AD_ImageData = MsnUtil.getDataFromFile(MSNApplication.RMS_AD_DATA, "roster_ad.png");
    	UpdateView();
	}
	
    public class MyExpandableListAdapter extends BaseExpandableListAdapter {
        // Sample data set.  children[i] contains the children (String[]) for groups[i].
    	Contact contact;
    	RosterItem itemR;
    	RosterItem item;
    	RosterMiniItem MiniItem;
    	
        public Object getChild(int groupPosition, int childPosition) {
        //	return app.childData.get(groupPosition).get(childPosition);
        	//return app.getCurrentChildData().get(groupPosition).get(childPosition);        	
        	try
			{
				return app.getCurrentGroupData().elementAt(groupPosition).contactVector.elementAt(childPosition);
			}
        	catch (Exception e)
			{
				// TODO Auto-generated catch block
				e.printStackTrace();
				return null;
			}
        }

        public long getChildId(int groupPosition, int childPosition) {
            return childPosition;
        }

        public int getChildrenCount(int groupPosition) {
           // return app.childData.get(groupPosition).size();
        	//return app.getCurrentChildData().get(groupPosition).size();
        	try
        	{
        		return app.getCurrentGroupData().elementAt(groupPosition).contactVector.size();
        	}catch(Exception e)
        	{
        		return 0;
        	}
        }


        public RosterItem getRosterView(Contact contact, boolean bExpand) {
//        	  AbsListView.LayoutParams lp = new AbsListView.LayoutParams(ViewGroup.LayoutParams.FILL_PARENT, 100);
        	  Contact con=null;
      		Object ob2=getSelectedItem();
      		//if(ob2 instanceof HashMap)
        	if(ob2 instanceof Contact)  
      		{
      			 //Map<String, Contact> curChild=(Map<String, Contact> )ob2;
  				 //con=(Contact)curChild.get("CONTACT"); 
        		con=(Contact)ob2;
  				// Log.v("contact=", "CONTACT");
      		}

              //item = new RosterItem(getContext(),contact,false, bExpand);
              
      		if(RosterItem.currentRosterItem!=null)
      		{
      			Contact thisContact=RosterItem.currentContact;
      			if(con!=null&&con==contact)
      			{
          			if(contact==thisContact)
          			{
          				item.showVcard(contact, 3000,item);
          			}
          			else
          			{
          				RosterItem.hideVcard();
          				item.showVcard(contact, 3000,item);
          			}	
      			}
  	
      		}    
              return item;
        }
        
        
        public View getChildView(int groupPosition, int childPosition, boolean isLastChild,
                View convertView, ViewGroup parent)
        {
            contact=(Contact)getChild(groupPosition, childPosition);
            if (contact != null)
            {
            	if( (contact.CONTACT_imid.equals(selectRoster_imid))
                		&& (groupPosition == selectRoster_groupIndex) )
                {
                	//已选中的item要展开显示
                	itemR = new RosterItem(getContext(),contact, groupPosition, false, true);
                	if(RosterItem.currentRosterItem != null)
    				{
    					RosterItem.hideVcard();				
    				}
                	itemR.showVcard(contact, 3000, itemR);
                }
                else
                {
                	itemR = new RosterItem(getContext(),contact, groupPosition, false, false);
                }
            }
            //System.gc();
            return itemR;
          	

////          	long tt = Runtime.getRuntime().freeMemory();
////          	if(tt < 500000)
////          	{
////          		if(app.bRefoot)
////          		{
////          			iDes++;
////          			if(iDes >= 7)
////          			{
////          				app.bRefoot = false;
////          			}
////          		}
////          		else
////          		{
////          			Log.v("gc", "gc");
////	          		System.gc();
////
////          		}
////          	}     	

        }

        public Object getGroup(int groupPosition) {
        	//return app.childData.get(groupPosition);
        	if(groupPosition < app.getCurrentGroupData().size())
        	{
        		return app.getCurrentGroupData().get(groupPosition);
        	}
        	else
        	{
        	    adImage = new AdImageView(mContext);
        		return adImage;
        	}
        	
        }

        public int getGroupCount() {
           // return app.childData.size();
        	if( (AD_ImageData != null) && (AD_ImageData.length != 0) )
        	{
        		return app.getCurrentGroupData().size() + 1;
        	}
        	else
        	{
        		return app.getCurrentGroupData().size();
        	}
        	
        }

        public long getGroupId(int groupPosition) {
            return groupPosition;
        }

        public View getGroupView(int groupPosition, boolean isExpanded, View convertView,
                ViewGroup parent) 
        {
        	if(groupPosition < app.getCurrentGroupData().size())
        	{
        		Group group=app.getCurrentGroupData().elementAt(groupPosition);
                String showText;
                if(app.getIsOnlyShowOnlineContact())
                {
                	showText=group.GROUP_name+"("+group.GROUP_maxweight_itemnum+")";
                }
                else
                {
            	//////////////////增加按状态排序功能
            	if(MSNApplication.ROSTER_NORMAL_HAS_OFFLINE == app.getGroupOrder())
            	{
                	showText=group.GROUP_name+"("+group.GROUP_maxweight_itemnum+"/"+group.GROUP_itemnum+")";
                }
            	else
            	{
            		showText=group.GROUP_name + "(" + group.GROUP_itemnum + ")";
            	}
            	//////////////////////////
            }
                GroupView groupView = new GroupView(mContext, isExpanded, showText, groupPosition);
                if(groupPosition == lastSelectGroupIndex)
                {
                	groupView.setBackgroundColor(Color.rgb(185, 235, 246));
                	//lastSelectGroup = groupView;
                }
        	
	            return groupView;
        	}
        	else
        	{
        		adImage = new AdImageView(mContext);
        		return adImage;
        	}
	        	
        }
        
        public TextView getGroupTextView()
        {
            AbsListView.LayoutParams lp = new AbsListView.LayoutParams(
                    ViewGroup.LayoutParams.FILL_PARENT, ViewGroup.LayoutParams.WRAP_CONTENT);        	
        	TextView text = new TextView(mContext);
        	//text.setTextSize(18);
        	text.setGravity(Gravity.CENTER_VERTICAL);
        	text.setPadding(40, 0, 0, 0);
        	text.setLayoutParams(lp);
        	return text;
        }

        public boolean isChildSelectable(int groupPosition, int childPosition) {
            return true;
        }

        public boolean hasStableIds() {
            return true;
        }

        public boolean collapseGroup(int groupPos)
        {
			return true;        	
        }
    }
}
