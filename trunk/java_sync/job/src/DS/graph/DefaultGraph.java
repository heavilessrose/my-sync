package DS.graph;

import java.util.Arrays;

/**
 * 
 * @author WangYinghua
 * 
 */
public class DefaultGraph implements Graph {

	private static class _Edge implements Edge {

		private static final _Edge NullEdge = new _Edge();

		int from;
		int to;
		int weight;

		_Edge nextEdge;

		private _Edge() {
			weight = Integer.MAX_VALUE;
		}

		_Edge(int from, int to, int weight) {

			this.from = from;
			this.to = to;
			this.weight = weight;
		}

		public int getWeight() {
			return weight;
		}

	}

	private static class _EdgeStaticQueue {
		_Edge first;
		_Edge last;
	}

	private int numVertexes;
	private String[] labels;
	private int numEdges;

	private _EdgeStaticQueue[] edgeQueues;

	// tag the specified vertex be visited or not
	private boolean[] visitTags;

	/**
     * 
     */
	public DefaultGraph(int numVertexes) {
		if (numVertexes < 1) {
			throw new IllegalArgumentException();
		}
		this.numVertexes = numVertexes;
		this.visitTags = new boolean[numVertexes];
		this.labels = new String[numVertexes];
		for (int i = 0; i < numVertexes; i++) {
			labels[i] = i + "";

		}
		this.edgeQueues = new _EdgeStaticQueue[numVertexes];
		for (int i = 0; i < numVertexes; i++) {
			edgeQueues[i] = new _EdgeStaticQueue();
			edgeQueues[i].first = edgeQueues[i].last = _Edge.NullEdge;

		}
		this.numEdges = 0;
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see algorithms.graph.Graph#edgeNum()
	 */
	// @Override
	public int edgeNum() {

		return numEdges;
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see algorithms.graph.Graph#firstEdge(int)
	 */
	// @Override
	public Edge firstEdge(int vertex) {
		if (vertex >= numVertexes)
			throw new IllegalArgumentException();
		return edgeQueues[vertex].first;

	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see algorithms.graph.Graph#isEdge(algorithms.graph.Graph.Edge)
	 */
	// @Override
	public boolean isEdge(Edge edge) {

		return (edge != _Edge.NullEdge);
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see algorithms.graph.Graph#nextEdge(algorithms.graph.Graph.Edge)
	 */
	// @Override
	public Edge nextEdge(Edge edge) {

		return ((_Edge) edge).nextEdge;
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see algorithms.graph.Graph#vertexesNum()
	 */
	// @Override
	public int vertexesNum() {

		return numVertexes;
	}

	// @Override
	public int fromVertex(Edge edge) {

		return ((_Edge) edge).from;
	}

	// @Override
	public void setEdge(int from, int to, int weight) {
		// we don't allow ring exist
		if (from < 0 || from >= numVertexes || to < 0 || to >= numVertexes
				|| weight < 0 || from == to)
			throw new IllegalArgumentException();
		_Edge edge = new _Edge(from, to, weight);
		edge.nextEdge = _Edge.NullEdge;
		if (edgeQueues[from].first == _Edge.NullEdge)
			edgeQueues[from].first = edge;
		else
			edgeQueues[from].last.nextEdge = edge;
		edgeQueues[from].last = edge;

	}

	// @Override
	public int toVertex(Edge edge) {

		return ((_Edge) edge).to;
	}

	// @Override
	public String getVertexLabel(int vertex) {
		return labels[vertex];
	}

	// @Override
	public void assignLabels(String[] labels) {
		System.arraycopy(labels, 0, this.labels, 0, labels.length);

	}

	// ///////////////////////////////////////
	/*
	 * ������ȱ���
	 * ������ȱ�����ÿ��ָ�����㿪ʼ���Զ�����һ��һ��ķ��ʡ���һ�����ж���������˲ż�����һ��ķ��ʡ����԰Ѷ������Ĺ�����ȱ�������ͼ�Ĺ�����ȱ���������
	 * �� ������������ͨ���޻�·������ͼ��Ҳ��һ�ø����� ͬ�����������ҲҪ������һ���������洢�����ʶ���
	 * 
	 * ������breathFirstTravel��ʵ�֣�Ϊ�˼�СJava���Ӱ�죬���Լ�ʵ��һ���ܼ򵥵Ķ��С�����Ҳ����ʹ��
	 * ArrayList,���Ǽ�ס���еĶ��壬ֻ����ͷɾ������β���룩
	 */
	private static class _IntQueue {
		private static class _IntQueueNode {
			_IntQueueNode next;
			int value;
		}

		_IntQueueNode first;
		_IntQueueNode last;

		// queue can only insert to the tail
		void add(int i) {
			_IntQueueNode node = new _IntQueueNode();
			node.value = i;
			node.next = null;
			if (first == null)
				first = node;
			else
				last.next = node;
			last = node;
		}

		boolean isEmpty() {
			return first == null;
		}

		// queue can only remove from the head
		int remove() {
			int val = first.value;
			if (first == last)
				first = last = null;
			else
				first = first.next;
			return val;
		}

	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see
	 * algorithms.graph.Graph#breathFirstTravel(algorithms.graph.GraphVisitor)
	 */
	// @Override
	public void breathFirstTravel(GraphVisitor visitor) {
		Arrays.fill(visitTags, false);// reset all visit tags

		for (int i = 0; i < numVertexes; i++) {
			if (!visitTags[i]) {

				do_BFS(i, visitor);
			}
		}

	}

	private void do_BFS(int v, GraphVisitor visitor) {
		// and BFS will use an queue to keep the unvisited vertexes
		// we can also just use java.util.ArrayList
		_IntQueue queue = new _IntQueue();
		queue.add(v);
		while (!queue.isEmpty()) {
			int fromV = queue.remove();
			visitor.visit(this, fromV);
			visitTags[fromV] = true;
			for (Edge e = firstEdge(fromV); isEdge(e); e = nextEdge(e)) {
				if (!visitTags[toVertex(e)]) {
					queue.add(toVertex(e));
				}
			}
		}
	}

	// ///////////////////////////////////////

	// ///////////////////////////// ������ȱ���
	/**
	 * ������ȱ���
	 * ���Ӵ�ĳһ�㿪ʼ�ܼ�����ǰ����ǰ��������˵�ĳһ�����б�û���ʵĶ��㣬�������߿��ñ�ָ��ĵ��Ƿ��ѷ��ʣ����û�з��ʣ���ô�Ӹ�ָ��ĵ����������
	 * 
	 * ��ôʲôʱ������أ�����������ͼ��ADTʵ�������һ����־���顣�������¼ĳһ�����Ƿ��ѷ��ʣ�����Ҳ��������ܼ�����ǰ���ʵ�δ���ʵ㣬�������
	 * 
	 * ����ܻ��ʣ����ָ��ͼ������ͨͼ������2�����ϵ���ͨ��������? OK,���������⣬���ǿ�����ÿһ�����㶼�л��������ʼ������
	 * 
	 * @see algorithms.graph.Graph#deepFirstTravel(algorithms.graph.GraphVisitor)
	 */
	// @Override
	public void deepFirstTravel(GraphVisitor visitor) {
		Arrays.fill(visitTags, false);// reset all visit tags
		for (int i = 0; i < numVertexes; i++) {
			if (!visitTags[i])
				do_DFS(i, visitor);
		}

	}

	private final void do_DFS(int v, GraphVisitor visitor) {
		// first visit this vertex
		visitor.visit(this, v);
		visitTags[v] = true;

		// for each edge from this vertex, we do one time
		// and this for loop is very classical in all graph algorithms
		for (Edge e = firstEdge(v); isEdge(e); e = nextEdge(e)) {
			if (!visitTags[toVertex(e)]) {
				do_DFS(toVertex(e), visitor);
			}
		}

	}

	// /////////////////////////////

	public static void main(String[] args) {
		/**
		 * For example, we have a graph: 0��1��2 �� ���� 3 4 5 �� ���� 6��7��8
		 * 
		 * here ,all weight is 0, its a DAG(Directed Acyclic Graph)
		 */

		DefaultGraph g = new DefaultGraph(9);
		g.setEdge(0, 1, 0);
		g.setEdge(0, 3, 0);
		g.setEdge(1, 2, 0);
		g.setEdge(4, 1, 0);
		g.setEdge(2, 5, 0);

		g.setEdge(3, 6, 0);
		g.setEdge(7, 4, 0);
		g.setEdge(5, 8, 0);
		g.setEdge(6, 7, 0);
		g.setEdge(8, 7, 0);

		// now we visit it
		GraphVisitor visitor = new GraphVisitor() {
			// @Override
			public void visit(Graph g, int vertex) {
				System.out.print(g.getVertexLabel(vertex) + " ");

			}

		};
		System.out.println("DFS==============:");
		g.deepFirstTravel(visitor);
		System.out.println();
		System.out.println("BFS==============:");
		g.breathFirstTravel(visitor);
		System.out.println();
	}

	// /////////////////////////////////////��������/////////////////////////////////////////
	/*
	 * �������� ����һ�������ŵ����ӣ������кܶ�����T1,T2,....
	 * ��Щ���������໥�����ģ�����Tj���ǰ����Ҫ��Ti����ɣ�����T1,T2...
	 * .���й����������Ⱦ���������һ��ͼ���������Ti����Ҫ����Tj��ɣ���ô<Ti,Tj>���Ǹ�ͼ�е�һ��·����·������Ϊ1�ľ���һ���ߡ�
	 * ����������ǰ���Щ��������ɵ��Ⱥ�˳�����г�������Ȼ��������˳����ܲ���Ψһ�ģ�����Tk��Tl���û����һ��·���ϣ���ô����֮���˳��������ġ�
	 * �����������������ֽⷨ
	 * 
	 * 1�������ҳ���ȣ����ӵ��ĵ�ıߵ���Ŀ��Ϊ��Ķ��������У�Ȼ�����α�����Щ���㣬ÿ�η��ʵ����е�һ������ʱ���Ѹö������������������ı���ȥ��
	 * Ҳ����ʹ�ù����������ȼ�1.�����1��ö������Ҳ��Ϊ0�ˣ���ô�Ѹö����������´δ�����ʼ����ֱ��û�����Ϊ0�Ķ����ˡ�
	 * ����Ҫע�⣬���������ͼ�л�·��ô�����ܲ��ᴦ�������ж�����˳��ˡ�����һ��ʼ�����˸����������ȣ������ʱ��ÿ������Ҫ����һ�Ρ�
	 * ��������ھ���Ĵ洢��ʽ���������ֻ��Ҫ����ÿ�еķ�������� һ��Ҳ���Ծ�̬�ĸ�����
	 */
	private final void calculateInDegrees(int[] inDegrees) {
		Arrays.fill(inDegrees, 0);
		for (int v = 0; v < numVertexes; v++) {
			for (Edge e = firstEdge(v); isEdge(e); e = nextEdge(e)) {
				inDegrees[toVertex(e)]++;
			}
		}
	}

	/**
	 * 
	 * @param sortedVertexes
	 */
	public void topologySort(int[] sortedVertexes) {
		// first calculate the inDegrees
		int[] inDegrees = new int[numVertexes];
		calculateInDegrees(inDegrees);

		Arrays.fill(visitTags, false);

		_IntQueue queue = new _IntQueue();

		for (int v = 0; v < numVertexes - 1; v++) {
			if (inDegrees[v] == 0)
				queue.add(v);
		}

		int index = 0;
		while (!queue.isEmpty()) {

			int from = queue.remove();
			System.out.println("visit:" + from);
			sortedVertexes[index++] = from;
			for (Edge e = firstEdge(from); isEdge(e); e = nextEdge(e)) {

				if (--inDegrees[toVertex(e)] == 0) {
					queue.add(toVertex(e));
				}
			}
		}
		if (index < numVertexes) {
			throw new IllegalArgumentException("There is a loop");
		}

	}

	/*
	 * ������ͼ������������Σ�ÿ���ڻ��˵��Ķ����ʱ��Ѹö������������顣 �����õ�������ǡ�þ������������������Ϊ��ײ�Ľڵ������Ȼ��˵��ġ�
	 */
	/**
	 * 
	 * @param sortedVertexes
	 */
	public void topologySort_byDFS(int[] sortedVertexes) {
		Arrays.fill(visitTags, false);
		int num = 0;
		for (int i = 0; i < numVertexes; i++) {
			if (!visitTags[i])
				num = do_topsort(i, sortedVertexes, num);
		}

	}

	private final int do_topsort(int v, int[] sortedVertexes, int num) {
		visitTags[v] = true;

		for (Edge e = firstEdge(v); isEdge(e); e = nextEdge(e)) {
			if (!visitTags[toVertex(e)])
				num = do_topsort(toVertex(e), sortedVertexes, num);
		}
		num++;
		sortedVertexes[numVertexes - num] = v;

		return num;
	}

	/**
	 * Given graph :
	 * 
	 * C1 �� C3 �� C2 �� �� �� C8 C4 C5 �� �� �� C9 �� C7 �� C6
	 */
	public static void testTopologySort() {
		DefaultGraph g = new DefaultGraph(9);
		g.setEdge(0, 1, 0);
		g.setEdge(2, 1, 0);
		g.setEdge(0, 3, 0);
		g.setEdge(1, 4, 0);
		g.setEdge(2, 5, 0);
		g.setEdge(3, 6, 0);
		g.setEdge(4, 7, 0);
		g.setEdge(5, 8, 0);
		g.setEdge(6, 7, 0);
		g.setEdge(8, 7, 0);

		g.assignLabels(new String[] { "C1", "C3", "C2", "C8", "C4", "C5", "C9",
				"C7", "C6" });

		int[] sorted = new int[g.vertexesNum()];
		// g.topologySort(sorted);
		g.topologySort_byDFS(sorted);
		System.out.println("Topology Sort Result==============:");
		for (int i = 0; i < sorted.length; i++)
			System.out.print(g.getVertexLabel(sorted[i]) + ",");
		System.out.println();
	}
}