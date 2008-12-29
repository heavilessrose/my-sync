package luke.android.utils.algorithms.graph;

/**
 * ͼ�Ļ�������洢�ṹ ͼG���ɶ��������ϣ��Լ�����֮��Ĺ�ϵ��ɣ�����ļ��ϼ�ΪV������֮��Ĺ�ϵ���ɱߵļ���E G=(V,E).
 * ˵һ���ߴ�v1,���ӵ�v2,��ô��v1Ev2(E��V�ϵ�һ����ϵ����=��<v1,v2>��E.
 * ͼ������ͼ������ͼ֮�֣�����ͼ��һ�����൱������ͼ���������ߣ����������ͼ�Ķ���v1��v2֮����һ����
 * ����ô���д�v1���ӵ�v2�ıߣ�Ҳ�д�v2���ӵ�v1�ıߣ�<v1,v2>��E ����<v2,v1>��E��������ͼ���ϸ����ַ���ġ�
 * 
 * һ��ͼ�Ĵ洢�����ַ�ʽ 1�����ھ�����һ�����������ֱߵ������<v1,v2>��E��Matrix[v1][v2]=Weight.
 * 2���ڽӱ���Ҫ����һ��˳��洢�Ķ�����ÿ�������ϵıߵ����ӱ�
 * �����ʵ�ֲ��õڶ��ַ���������ͼ�ָ���ͼ����ͼ֮�֣�����ͼ����������֮��ͬһ�������ж�����
 * �����ǿ��ǵĶ����޻���ͼ���޻���ͼ��ָ����û���Լ�ָ���Լ��ıߵļ�ͼ������ȡvi����V => <vi,vi>������E����û���ظ��ߡ�
 * 
 * @author WangYinghua
 * 
 */
// ͼ��ADT
public interface Graph {

	// mark
	public static interface Edge {
		public int getWeight();
	}

	int vertexesNum();

	int edgeNum();

	boolean isEdge(Edge edge);

	void setEdge(int from, int to, int weight);

	/** ����ָ���ڵ�ıߵ��������ĵ�һ���� */
	Edge firstEdge(int vertex);

	/** ˳�ű���������һ���� */
	Edge nextEdge(Edge edge);

	/** ���ظñߵ��սᶥ�� */
	int toVertex(Edge edge);

	/** ���ظñߵ���ʼ���� */
	int fromVertex(Edge edge);

	/** ���ظö����Ӧ�ı�� */
	String getVertexLabel(int vertex);

	/** �����ж�����Ϻ� */
	void assignLabels(String[] labels);

	void deepFirstTravel(GraphVisitor visitor);

	void breathFirstTravel(GraphVisitor visitor);

}