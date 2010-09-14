package com.pica.msn;

import java.util.Vector;

public interface ParserNotifier
{   
    /**����cache״̬ ��ʱû��*/
    public void setCacheStatus(boolean isCacheEmpty);
    
    /**�յ��ôοͻ��˵�¼��session id http������Ҫ�� socket��������Ҫ*/
    public void receiveSessionId(String sid);
    
    /**��� errorCodeΪ�� �յ�jerror ��Ҫ���ص���¼ҳ��<br>
     * 	1401 ��¼passportʧ��<br>
		1402 �ͻ���û��Ϊ����Ĳ�������ͬ��׼��<br>
		1403 ����ͬ������ʧ�ܡ��ͻ���Ӧ��ִ��һ����ȫͬ��<br>
		1600 �������ͻ����߳�<br>
		1601 Pica Server��ʱ��û���յ��ͻ��˵�������
     * */
    public void receiveLoginJerror(String errorMsg,String errorCode);
    
	//#ifdef support_save_rms
    /**�յ�addition==0  ͬ������ ��Ҫ��ʾ�û�Ȼ��ע���˳�*/
    public void receiveSynchronizeError();
    //#endif
    
    /** �յ�iq error�� ���ؽ�������ȴ�ǰ��ҳ�� ���jidΪȺ��jid ���ʾ����Ⱥ�����б���*/
    public void receiveOtherJerror(String jid,String errorMsg);

	
	/**�ͻ����յ��˰��� ��������״̬  <presence type=��available'/>*/
	public void receivePicaIqRoster();
	
	/**�յ���������*/
	public void receiveVcardTemp(String nickname,String impresa,String portrait_data,String resourceId,String url,Vector nodeVec);
		
	
	/**�Թ������н�������*/
	public void receiveAD(String type,String id,String action,String target,String param0,String dbid,String sale,String text,String src);
	
	/**����յ���Ҫ����RMS ������id������*/
	public void receiveADComplete();

	
	/**�յ������� ��ˢ��ҳ��
	 * @param type �����ҵ����� type=��fixed�̶�|custom���� zwp��wapƵ��
	 * */
	public void receiveRoom(String jid,String name,String type);
	

	/***
	 * �յ���֤���� ������֤����ĺ��ѵĶ��� ���䱣�浽verifyFriendVector�������� Ȼ����ʾ��֤����ҳ��
	 * @param imid ��֤��ϵ�˵�imid
	 * @param action =subscriptionanswer
	 * @param nickname ��֤��ϵ�˵��ǳ�
	 * @param realname ��֤��ϵ�˵���ʵ����
	 * @param sex ��֤��ϵ�˵��Ա�
	 * @param age ��֤��ϵ�˵�����
	 * @param province ��֤��ϵ�˵�ʡ��
	 * @param desc ��֤��ϵ�˵�����
	 */
	public void receiveVerify(String imid,String action,String nickname,String realname,String sex,String age,String province,String desc);
	
	
	/**�յ��˳�ע��ĳ�����ҵİ� �˳��ȴ�ҳ�� ɾ��һ�������� Ȼ��ˢ��ҳ��*/
	public void receiveExitCluster(String jid);
	
	/**�ͻ����յ��µ��������б���˳��ȴ�ҳ�� ����һ�������� �����б���ʾ��
	 * �����ǰ����ͼΪ��WAP��ͼ��������ʾ��Ϣ���ȵ㻰���б��Ѹ��£��Ƿ����ڲ鿴������
		����û������ȷ���������л���ͼ����ϵ����ͼ��ͬʱչ�����ȵ㻰�⡱�顣*/
	public void receiveJoinCluster(String jid,String name,String type);
	
	/**�յ����߳������һ��߱�����������İ� ���û���ʾ*/
	public void receiveKickoffOrBlacklistChatroom(String role,String reason);
	
    /***
     * �յ�������Ϣ
     * @param jid ����Ϣ��ϵ�˵�jid
     * @param imid ����Ϣ��ϵ��(İ����)��imid
     * @param sid �Ựsid
     * @param body ��Ϣ����
     * @param stamp ��Ϣʱ��
     * @param nudge ��Ϣ�Ƿ���
     * @param status ="fail"ʧ��
     * @param reason ��Ϣʧ��ԭ��
     * @param transfer_ftID �ļ�����ʧ�� �������ֵ�������ļ�������Ϣ
     */
    public void receiveMessage(String jid,String imid,String sid, String body, String stamp,boolean nudge,String status,String reason,String transfer_ftID);

	//#ifdef support_groupmessage
    /**�յ���ʱȺ����Ϣ �п�����İ���˷��͵� ��imid����*/
    public void receiveGroupMessage(String from,String jid,String imid,String sid,  String body,String stamp,boolean nudge,String status,String reason);
	//#endif
    
    /**�յ�Ⱥ���������Ϣ*/
    public void receiveClusterMessage(String from, String to,String name, String body, String stamp);
    

	
	/**�յ� ��ʾ�û���֤ͨ�� ���ŷ��ͻ�ȡPICA�����б�*/
	public void receiveIqAuth();
	
	/***
	 * �յ�dispacth�ذ�
	 * @param result result=0ϵͳ�������¼ result=1�ͻ�����Ҫǿ������  result=2�ɹ�
	 * @param noticeVec װ��ϵͳ��ʾ��Ϣ������
	 * @param force force=1ǿ������ force=0��������
	 * @param url ���������ͻ��˵�URL
	 * @param version  �¿ͻ��˰汾��
	 * @param id  Dispatch�������ʱpica�ʺ�  ��67890001@pica
	 * @param password ��ʱpica�˺ŵ�����
	 * @param ip ��½pica�������ĵ�ַ�Ͷ˿� jabber������
	 * @param ua �ֻ�UA
	 * @param carrier ��Ӫ����Ϣ
	 * @param adurl ����������URL
	 * @param ad_interval ȡ����ʱ����
	 * @param zwp ZWP���ص�IP�Ͷ˿� �������û�У�����ֵΪ�գ���֧����WAP
	 * @param fpage ��WAP����ҳ��ַ
	 * @param add_friend msn.cn�û���Ӻ��ѵĵ�ַ
	 * @param room_name  MSNȺ��������� �������û�л���Ϊ�գ���֧��MSNȺ����
	 * @param room_search ����MSNȺ�������ң��ĵ�ַ
	 * @param hotmail Hotmail������� �ĵ�ַ �������û�л���Ϊ�գ���֧������
	 * @param spaces MSN spaces��ַ �������û�л���Ϊ�գ���֧�ֿռ�
	 * @param search  MSN ������ַ �������û�л���Ϊ�գ���֧������
	 * @param roomCreateAddress ����/�����Խ�Ⱥ�ĵ�ַ
	 * @param roomInviteAddress ��������Խ�Ⱥ�ĵ�ַ
	 * @param transparent �Կͻ��������壬�ͻ��˵�¼MSNʱ���˽ڵ�͸��������
	 * @param transfer_limit
	 * @param data_url
	 * @param data_method
	 * @param capability ����
	 */
	public void receiveDispatch(String result,Vector noticeVec,String force,String url,String version,String id,String password,String ip,String ua,String carrier,String adurl,String ad_interval,String zwp,String fpage,
			String add_friend,String room_name,String room_search,String hotmail,String spaces,String search,String roomCreateAddress,String roomInviteAddress,String transparent,String transfer_limit,String data_url,
			String data_method,String capability);
	
	/**
	 * �յ������б�
	 * @param last last=��0���к�����|1û�к�����
	 * @param groupName ������ 
	 * @param groupID ����ID
	 * @param gbcode ������ǰ4���ַ���gb2312��
	 * @param sync  new�·���|updated�޸ĵķ���|deletedɾ���ķ���
	 * @param version ����İ汾��
	 */
    public void receiveGroup(String last, String groupName, String groupID, String gbcode,String sync, String version);
    
    /**�յ������б� ��û�к�������
     * */
    public void receiveGroupComplete(String last,String version);
    
    /**
     * �յ���ϵ���б�(��¼ʱ�յ����������/ɾ����ϵ�˵Ȳ����ɹ�)
     * @param action=add���|deleteֻɾ��|rmblkɾ������ֹ|block��ֹ|unblock�����ֹ
     * @param last  last=��0���к�����|1û�к����� 
     * @param jid  ��ϵ�˵�jid
     * @param imid ��ϵ�˵�imid
     * @param nickname ��ϵ�˵��ǳ�
     * @param gbcode ��ϵ�˵�gbcode��
     * @param block  ��ϵ�˵���ֹ��� 0����|1��ֹ
     * @param sync  new����ϵ��|updated�޸ĵ���ϵ��|deletedɾ������ϵ��
     * @param groupId ��ϵ�˵ķ���id
     * @param impresa ��ϵ�˵��������
     * @param portrait ��ϵ�˵�ͷ���ʶ
     * @param phone ��ϵ�˵ĵ绰
     * @param gleam ��ϵ�˵Ŀؼ��������
     * @param url ��ϵ�˵Ŀռ��ַ
     * @param version ��ϵ�˵�
     * @param nodeVec ���������а����Ľڵ�Ԫ��
     * @param deviceId �豸id
     * @param addition
     */
    public void receiveRoster(String action,String last,String jid,String imid,String nickname,String  gbcode,String  block, 
    		String sync, String groupId,String impresa,String portrait,String phone, String gleam,
    		String url,String version,Vector nodeVec,String deviceId,String addition);
    
    /**�յ���ϵ���б� û�к�������<br>
     * ���action��Ϊ�� ���ʾ�����ɾ������ϵ�˲��� ��ʱ������Ҫ���������б����ݿ�����
     * ���汾�ű��浽RMS
     * */
    public void receiveRosterComplete(String  last,String  version,String action);
    
	//#ifdef support_head_image
    /***
     * �յ���ϵ�˵�ͷ�� ת��� ���浽rms����ȥ url��imageData���ܹ���
     */
    public void receiveContactPortrait(String jid,String portraithash,String imageData,String url);
    //#endif
    
	//#ifdef support_groupmessage
    /**�û�������뿪���˻Ự*/
    public void receiveParticipant(String sid, String jid,String[][]node);
    //#endif
    
    /**�û��յ�Ƶ���б�  Ҫˢ���б�*/
    public void receiveChannel(String jid,String name,String groupname,String[][] options,boolean iswap,boolean auth);
    
    /**ˢ��ҳ��
     * */
    public void receiveUpdateUI();
    /**�ڴ治��*/
    public void receiveOutOfMemory();
    
    /**�û�����ǳ��ɹ� �յ�presece type=unavailable�� �û��յ�msnע���ɹ���*/
    public void receiveLogoutMsn();
    
    /**�յ��û��ı��Լ���״̬*/
    public void receiveModifyMyState(String show);
    
    /**	��ʾ�ͻ��˷�������״̬  ���������سɹ� ���������ǿͻ��˷����¼MSN*/
    public void receiveLoginPicaSuccess();
    
    /**��ϵ��״̬�ȵĸı��
	 * @param type    available����������|unavailable�ѻ�
	 * @param from    ��ϵ�˵�jid
	 * @param version  ��ϵ��ͬ���汾
	 * @param show   ��ϵ��״ֵ̬
	 * @param nickname  ��ϵ���ǳ�
	 * @param gbcode  ��ϵ���ǳ�ǰ4���ַ���gb2312��
	 * @param impresa  ��ϵ�˸�����Ϣ��������
	 * @param portrait  ��ϵ��ͷ���ʶ
	 * @param mobile  ��ϵ�� 0���ƶ��û�|1�ƶ��û�
	 * @param gleam ����ϵ�˵Ŀռ������� 0=û�и���|1=�и���  
	 * @param url ����ϵ�˵Ŀռ��ַ
	 * @param phone ����ϵ�˵ĵ绰
	 * @param nodeVec ���������а����Ľڵ�Ԫ��
     * */
    public void receiveContactChangeState(String type,String from,String version,String  show,String  nickname,String gbcode,String  impresa,String  portraithash,String gleam,String url,String phone,Vector nodeVec);

    /**�յ�additon=0�İ� �ͻ�����Ҫɾ�����ر������ϵ�˷����б���ϵ���б��Ͱ汾��*/
    public void receiveAdditonEqualsZero();
    
    
    ///////////////////////////////////
    //�ļ����������ܵĴ���
    //////////////////////////////////
    
    /**
     *  ���շ��յ��Ĵ����ļ�����
     * @param from ��msn.pica��
     * @param jid
     * @param imid
     * @param type type=��ft��
     * @param transferID ������������ʶ������ļ�
     * @param sid �Ựsid
     * @param ftID
     * @param action
     * @param transfer_name �ļ�����
     * @param transfer_size �ļ���С���ֽ�����
     */
    public void receiveTransferInviteRequest(String from,String jid,String imid,String type,String transferID,String sid,String ftID,
    		String action,String transfer_name,String transfer_size);
    
    /**
     *�����ļ�����Ĵ��� ���ͷ��յ��Ļ�Ӧ ������շ��ܾ������󣬸ô��͹�����ֹ
     * @param from
     * @param type
     * @param transferID
     * @param sid
     * @param ftID
     * @param action  accept ���� | reject �ܾ�
     */
    public void receiveTransferInviteResponse(String from,String type,String transferID,String sid,String transfer_ftID,String transfer_action);
    
    /**
     * �����ļ� ���ڷ�ʽ ���ͷ��յ�����Ӧ ������ͳɹ������������ʣ�������ֱ���ļ�ȫ��������ɣ������͹�����ֹ
     * @param from
     * @param type
     * @param transferID
     * @param sid
     * @param transfer_ftID
     * @param transfer_seqid
     * @param transfer_action  ack
     * @param transfer_status status=��ok�ɹ�|failʧ��|canceldȡ����
     */
    public void receiveTransferInnerResponse(String from,String type,String transferID,String sid,String transfer_ftID,String transfer_seqid,
    		String transfer_action,String transfer_status);
    
    /**
     * transfer���շ����յ�������
     * @param from
     * @param jid
     * @param imid
     * @param type
     * @param transferID
     * @param sid
     * @param transfer_ftID
     * @param transfer_seqid
     * @param transfer_action
     * @param transfer_bytes
     * @param transfer_start ������ʼλ��
     * @param transfer_end ���ݽ���λ��
     * @param transfer_base64Length Base64���볤��
     * @param transfer_base64Data Base64���ݿ�
     */
    public void receiveTransferData(String from,String jid,String imid,String type,String transferID,String sid,
    		String transfer_ftID,String transfer_seqid,String transfer_action,String transfer_bytes,
    		String transfer_start,String transfer_end,String transfer_base64Length,String transfer_base64Data);
    
    
    /**
     * transfer ���ͷ������ļ��������ͷ��յ�����Ӧ ��Ϊ������ȫ�ɹ���
     * @param from
     * @param type
     * @param transferID
     * @param sid
     * @param transfer_ftID
     * @param transfer_action
     */
    public void receiveTransferCompleteAck(String from,String type,String transferID,String sid,String transfer_ftID,String transfer_action);
    
    
    /**���շ��յ�complete ��Ϊ������� ��Ҫ�ٷ�һ��complete_ack��������*/
    public void receiveTransferComplete(String from,String type,String transferID,String sid,String transfer_ftID,String transfer_action);

    /**
     * �����ļ������ⷽʽ��  transfer���շ����յ�������
     * @param from
     * @param jid
     * @param imid
     * @param type
     * @param transferID
     * @param sid
     * @param transfer_ftID
     * @param transfer_action
     * @param transfer_url ��ȡ���ļ��ĵ�ַ
     */
    public void receiveTransferOuterReceive(String from,String jid,String imid,String type,String transferID,String sid,String transfer_ftID,
    		String transfer_action,String transfer_url);
    
    /**
     * ���ļ����͵Ĺ����У�ȡ���ļ��Ĵ���
     * @param from
     * @param type
     * @param transferID
     * @param sid
     * @param transfer_ftID
     * @param transfer_action cancel ȡ��
     */
    public void receiveTransferCancelResponse(String from,String type,String transferID,String sid,String transfer_ftID,String transfer_action);
    
    /**
     * ������������
     * @param from
     * @param jid
     * @param imid
     * @param type
     * @param sid
     * @param transfer_data_mime
     * @param transfer_bytes �ļ��ܳ���
     * @param transfer_base64Length Base64���볤��
     * @param transfer_base64Data Base64����ͷ���ļ�����
     * @param transfer_url_mime
     * @param transfer_url �����ļ���url
     */
    public void receiveTransferVoiceclip(String from,String jid,String imid,String type,String sid,String transfer_data_mime,String transfer_bytes,String transfer_base64Length,String transfer_base64Data,String transfer_url_mime,String transfer_url);
}

