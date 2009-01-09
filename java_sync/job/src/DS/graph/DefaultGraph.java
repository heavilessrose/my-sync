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
	 * 广度优先遍历
	 * 广度优先遍历从每个指定顶点开始，自顶向下一层一层的访问。上一层所有顶点访问完了才继续下一层的访问。可以把二叉树的广度优先遍历看成图的广度优先遍历的特例
	 * 。 （二叉树是连通的无回路的有向图，也是一棵根树） 同样，广度优先也要借助与一个队列来存储待访问顶点
	 * 
	 * 下面是breathFirstTravel的实现，为了减小Java库的影响，我自己实现一个很简单的队列。（你也可以使用
	 * ArrayList,但是记住队列的定义，只能在头删除，在尾插入）
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

	// ///////////////////////////// 深度优先遍历
	/**
	 * 深度优先遍历
	 * 即从从某一点开始能继续往前就往前不能则回退到某一个还有边没访问的顶点，沿这条边看该边指向的点是否已访问，如果没有访问，那么从该指向的点继续操作。
	 * 
	 * 那么什么时候结束呢，这里我们在图的ADT实现里加上一个标志数组。该数组记录某一顶点是否已访问，如果找不到不到能继续往前访问的未访问点，则结束。
	 * 
	 * 你可能会问，如果指定图不是连通图（既有2个以上的连通分量）呢? OK,解决这个问题，我们可以让每一个顶点都有机会从它开始遍历。
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
		 * For example, we have a graph: 0→1→2 ↓ ↑↓ 3 4 5 ↓ ↑↓ 6→7←8
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

	// /////////////////////////////////////拓扑排序/////////////////////////////////////////
	/*
	 * 拓扑排序 考虑一个任务安排的例子，比如有很多任务T1,T2,....
	 * 这些任务又是相互关联的，比如Tj完成前必须要求Ti已完成，这样T1,T2...
	 * .序列关于这样的先决条件构成一个图，其中如果Ti必须要先于Tj完成，那么<Ti,Tj>就是该图中的一条路径，路径长度为1的就是一条边。
	 * 拓扑排序就是把这些任务按照完成的先后顺序排列出来。显然，这样的顺序可能不是唯一的，比如Tk，Tl如果没有在一条路径上，那么他们之间的顺序是任意的。
	 * 拓扑排序至少有两种解法
	 * 
	 * 1）首先找出入度（连接到改点的边的数目）为零的顶点放入队列，然后依次遍历这些顶点，每次访问到其中的一个顶点时，把该定点关联到的其它顶点的边移去，
	 * 也就是使得关联顶点的入度减1.如果减1后该定点入度也变为0了，那么把该定点加入队列下次从他开始处理，直到没有入度为0的定点了。
	 * 这里要注意，如果给定的图有回路那么，可能不会处理完所有顶点就退出了。这里一开始计算了各个顶点的入度，计算的时候，每条边需要访问一次。
	 * 如果是相邻矩阵的存储方式，计算入度只需要计算每列的非零个数。 一般也可以静态的给出。
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
	 * 借助于图的深度优先周游，每次在回退到改顶点的时候把该顶点送入结果数组。 这样得到的数组恰好就是拓扑排序的逆序，因为最底层的节点是最先回退到的。
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
	 * C1 → C3 ← C2 ↓ ↓ ↓ C8 C4 C5 ↓ ↓ ↓ C9 → C7 ← C6
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