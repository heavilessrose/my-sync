package com.pica.msn;
import java.util.*;
public class Group {
	
	/**��ϵ��|Ƶ��|������ ��jid */
	public String SUBITEM_JID;//jid ��ϵ�� Ƶ�� ������ ��������
	
	/**��ϵ�˷������ID*/
    public String GROUP_id;//  
    /**��gb��*/
    public String GROUP_gbcode;// 
    /**����*/
    public String GROUP_name;//  
    /**������ �û��Զ������ �����ҷ��� Ƶ������ ���*/
    public byte GROUP_flag;//  
    /**���չ��״̬*/
    public boolean  GROUP_isexpand;
    /**���ڳ�Ա������*/
    public short GROUP_itemnum;// 
    /**��������״̬������*/
    public String GROUP_maxweight_itemnum;
    
    public Vector<Contact> contactVector;
    
    public Group()
    {
    	contactVector=new Vector<Contact>();
    }
}
