package luke.android.utils.algorithms.graph;

/**
 * 图的基本概念及存储结构 图G是由顶点的有穷集合，以及顶点之间的关系组成，顶点的集合记为V，顶点之间的关系构成边的集合E G=(V,E).
 * 说一条边从v1,连接到v2,那么有v1Ev2(E是V上的一个关系）《=》<v1,v2>∈E.
 * 图有有向图，无向图之分，无向图的一条边相当于有向图的中两条边，即如果无向图的顶点v1和v2之间有一条边
 * ，那么既有从v1连接到v2的边，也有从v2连接到v1的边，<v1,v2>∈E 并且<v2,v1>∈E，而有向图是严格区分方向的。
 * 
 * 一般图的存储有两种方式 1）相邻矩阵，用一个矩阵来保持边的情况，<v1,v2>∈E则Matrix[v1][v2]=Weight.
 * 2）邻接表，需要保存一个顺序存储的顶点表和每个顶点上的边的链接表。
 * 这里的实现采用第二种方案，另外图又复杂图，简单图之分，复杂图可能在两点之间同一个方向有多条边
 * ，我们考虑的都是无环简单图，无环简单图是指顶点没有自己指向自己的边的简单图，即任取vi属于V => <vi,vi>不属于E并且没有重复边。
 * 
 * @author WangYinghua
 * 
 */
// 图的ADT
public interface Graph {

	// mark
	public static interface Edge {
		public int getWeight();
	}

	int vertexesNum();

	int edgeNum();

	boolean isEdge(Edge edge);

	void setEdge(int from, int to, int weight);

	/** 返回指定节点的边的链表里存的第一条边 */
	Edge firstEdge(int vertex);

	/** 顺着边链表返回下一条边 */
	Edge nextEdge(Edge edge);

	/** 返回该边的终结顶点 */
	int toVertex(Edge edge);

	/** 返回该边的起始顶点 */
	int fromVertex(Edge edge);

	/** 返回该定点对应的标号 */
	String getVertexLabel(int vertex);

	/** 给所有顶点标上号 */
	void assignLabels(String[] labels);

	void deepFirstTravel(GraphVisitor visitor);

	void breathFirstTravel(GraphVisitor visitor);

}