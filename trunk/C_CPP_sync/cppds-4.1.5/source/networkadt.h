#ifndef NETWORKADT_H
#define NETWORKADT_H
#ifndef ARRAYADT_H
  #include "arrayadt.h"
#endif
#ifndef MATRIXADT_H
  #include "matrixadt.h"
#endif
#ifndef QUEUEADT_H
  #include "queueadt.h"
#endif

/****************************************************************************/
template <class BaseData>
struct EdgeStruct
{
  BaseData node1;
  BaseData node2;
};

template <class BaseData, class WeightType>
class NetworkAdt
{
  protected:
    BaseData _FirstValue, _LastValue;
    ArrayAdt<BaseData, bool>* _Nodes;
    MatrixAdt<BaseData, BaseData, WeightType>* _Edges;

    ArrayAdt<BaseData, WeightType> _ShortDistance;
    ArrayAdt<BaseData, BaseData> _ShortPath;

    EdgeStruct<BaseData> FindMinimum(const BaseData& k, const ArrayAdt<BaseData, bool>& Included_);
    EdgeStruct<BaseData> FindMinimum(const BaseData& k, const ArrayAdt<BaseData, bool>& Included_,
                                     const ArrayAdt<BaseData, WeightType>& Distance_);

  public:
    NetworkAdt(const BaseData& firstValue, const BaseData& lastValue);
    NetworkAdt(const NetworkAdt<BaseData, WeightType>& initGraph_);

    ~NetworkAdt();

    NetworkAdt<BaseData, WeightType>& operator = (const NetworkAdt<BaseData, WeightType>& Source_);

    bool Empty() const;
    bool Full() const;
    WeightType EdgeWeight(const BaseData& Value1_, const BaseData& Value2_);

    void AddNode(const BaseData& Value_);
    void RemoveNode(const BaseData& Value_);
    
    void AddEdge(const BaseData& Value1_, const BaseData& Value2_, WeightType Weight_);
    void RemoveEdge(const BaseData& Value1_, const BaseData& Value2_);    
    
    void SearchFrom(BaseData& Start_, void(*ProcessNode)(BaseData& Value_), ArrayAdt<BaseData, bool> Visited_);
    void DepthFirstTraversal(BaseData& Start_, void(*ProcessNode)(BaseData& Value_));
    void BreadthFirstTraversal(BaseData& Start_, void(*ProcessNode)(BaseData& Value_));

    NetworkAdt<BaseData, WeightType>* MinimumSpanningTree();
    void ShortestPath(const BaseData& Start_, const BaseData& Dest_);

    const ArrayAdt<BaseData, WeightType>& ShortestPath_Distance()
        { return _ShortDistance; }
    const ArrayAdt<BaseData, BaseData>& ShortestPath_Path()
        { return _ShortPath; }
};

/****************************************************************************/
template <class BaseData, class WeightType>
NetworkAdt<BaseData, WeightType>::NetworkAdt(const BaseData& firstValue, const BaseData& lastValue):
_FirstValue(firstValue),
_LastValue(lastValue),
_Nodes(new ArrayAdt<BaseData, bool>(firstValue, lastValue)),
_Edges(new MatrixAdt<BaseData, BaseData, WeightType>(firstValue, lastValue, firstValue, lastValue))
{}

/****************************************************************************/
template <class BaseData, class WeightType>
NetworkAdt<BaseData, WeightType>::NetworkAdt(const NetworkAdt<BaseData, WeightType>& Obj_):
_FirstValue(Obj_._FirstValue),
_LastValue(Obj_._LastValue),
_Nodes(new ArrayAdt<BaseData, bool>(Obj_)),
_Edges(new MatrixAdt<BaseData, BaseData, WeightType>(*Obj_._Edges))
{}

/****************************************************************************/
template <class BaseData, class WeightType>
NetworkAdt<BaseData, WeightType>::~NetworkAdt()
{
  delete _Nodes;
  delete _Edges;
}

/****************************************************************************/
template <class BaseData, class WeightType>
NetworkAdt<BaseData, WeightType>&
NetworkAdt<BaseData, WeightType>::operator = (const NetworkAdt<BaseData, WeightType>& Source_)
{
  delete _Nodes;
  delete _Edges;

  _FirstValue = Obj_._FirstValue;
  _LastValue = Obj_._LastValue;
  _Nodes = new ArrayAdt<BaseData, bool>(Obj_);
  _Edges = new MatrixAdt<BaseData, BaseData, WeightType>(*Obj_._Edges);

  return *this;
}

/****************************************************************************/
template <class BaseData, class WeightType>
bool NetworkAdt<BaseData, WeightType>::Empty() const
{
  BaseData i;
  for (i = _FirstValue; i <= _LastValue; i++)
    if (_Nodes[i])
      return false;

  return true;
}

/****************************************************************************/
template <class BaseData, class WeightType>
bool NetworkAdt<BaseData, WeightType>::Full() const
{
  BaseData i;
  for (i = _FirstValue; i <= _LastValue; i++)
    if (!_Nodes[i])
      return false;

  return true;
}

/****************************************************************************/
template <class BaseData, class WeightType>
void NetworkAdt<BaseData, WeightType>::AddNode(const BaseData& Value_)
{
  if (!(_FirstValue <= Value_ && Value_ <= _LastValue))
    Xindex();

  _Nodes[Value_] = true;
}

/****************************************************************************/
template <class BaseData, class WeightType>
void NetworkAdt<BaseData, WeightType>::AddEdge(const BaseData& Value1_, const BaseData& Value2_, WeightType Weight_)
{
  if (!(_FirstValue <= Value1_ && Value1_ <= _LastValue) ||
      !(_FirstValue <= Value2_ && Value2_ <= _LastValue))
    Xindex();

  if (!_Nodes[Value1_] && !_Nodes[Value2_])
    return;

  (*_Edges)[Value1_][Value2_] = Weight_;
  (*_Edges)[Value2_][Value1_] = Weight_;
}

/****************************************************************************/
template <class BaseData, class WeightType>
WeightType NetworkAdt<BaseData, WeightType>::EdgeWeight(const BaseData& Value1_, const BaseData& Value2_)
{
  if (!(_FirstValue <= Value1_ && Value1_ <= _LastValue) ||
      !(_FirstValue <= Value2_ && Value2_ <= _LastValue) ||
      (!_Nodes[Value1_] && !_Nodes[Value2_]))
    return WeightType(0);

  return ((*_Edges)[Value1_][Value2_]);
}

/****************************************************************************/
template <class BaseData, class WeightType>
void NetworkAdt<BaseData, WeightType>::RemoveEdge(const BaseData& Value1_, const BaseData& Value2_)
{
  if (!(_FirstValue <= Value1_ && Value1_ <= _LastValue) ||
      !(_FirstValue <= Value2_ && Value2_ <= _LastValue))
    Xindex();

  if (!_Nodes[Value1_] && !_Nodes[Value2_])
    return;

  if (_Nodes[Value1_] || _Nodes[Value2_])
  {
    (*_Edges)[Value1_][Value2_] = WeightType(0);
    (*_Edges)[Value2_][Value1_] = WeightType(0);
  }  
}

/****************************************************************************/
template <class BaseData, class WeightType>
void NetworkAdt<BaseData, WeightType>::RemoveNode(const BaseData& Value_)
{
  if (!(_FirstValue <= Value_ && Value_ <= _LastValue))
    Xindex();

  _Nodes[Value_] = false;
  BaseData x, y;

  for (x = _FirstValue; x <= _LastValue; x++)
  {  
    if (x == Value_)
    {
      for (y = _FirstValue; y <= _LastValue; y++)
        (*_Edges)[x][y] = WeightType(0);
    }
    else
      (*_Edges)[x][Value_] = WeightType(0);
  }
}

/****************************************************************************/
template <class BaseData, class WeightType>
void NetworkAdt<BaseData, WeightType>::SearchFrom(BaseData& Start_, void(*ProcessNode)(BaseData& Value_),
                                                ArrayAdt<BaseData, bool> Visited_)
{
  BaseData k;

  (*ProcessNode)(Start_);
  Visited_[Start_] = true;

  for (k = _FirstValue; k <= _LastValue; k++)
    if ((!Visited_[k]) && Edge(Start_, k))
      SearchFrom(k, ProcessNode, Visited_);
}

/****************************************************************************/
template <class BaseData, class WeightType>
void NetworkAdt<BaseData, WeightType>::DepthFirstTraversal(BaseData& Start_, void(*ProcessNode)(BaseData& Value_))
{
  ArrayAdt<BaseData, bool> Visited_(_FirstValue, _LastValue);
  BaseData k;

  for (k = _FirstValue; k <= _LastValue; k++)
    Visited_[k] = false;

  SearchFrom(Start_, ProcessNode, Visited_);
}

/****************************************************************************/
template <class BaseData, class WeightType>
void NetworkAdt<BaseData, WeightType>::BreadthFirstTraversal(BaseData& Start_, void(*ProcessNode)(BaseData& Value_))
{
  ArrayAdt<BaseData, bool> NodeInQueue_(_FirstValue, _LastValue);

  BaseData k, CurrentNode_;
  QueueAdt<BaseData> NodesToVisit_;

  for (k = _FirstValue; k <= _LastValue; k++)
    NodeInQueue_[k] = false;

  NodesToVisit_.Enqueue(Start_);
  NodeInQueue_[Start_] = true;

  while (!NodesToVisit_.Empty())
  {
    CurrentNode_ = NodesToVisit_.Front();
    NodesToVisit_.Dequeue();
    (*ProcessNode)(CurrentNode_);

    for (k = _FirstValue; k <= _LastValue; k++)
      if ((!NodeInQueue_[k]) && Edge(CurrentNode_, k))
      {
        NodesToVisit_.Enqueue(k);
        NodeInQueue_[k] = true;
      }
  }  
}

/****************************************************************************/
template <class BaseData, class WeightType>
EdgeStruct<BaseData> NetworkAdt<BaseData, WeightType>::FindMinimum(const BaseData& k, const ArrayAdt<BaseData, bool>& Included_)
{
  BaseData y, Node_;
  WeightType Dist_ = WeightType(0);

  for (y = _FirstValue; y <= _LastValue; y++)
    if (!Included_[y] && y != k)
    {
      WeightType Next_ = EdgeWeight(k, y);
      if (Dist_ > Next_ || !Dist_)
      {
        Dist_ = Next_;
        Node_ = y;
      }
    }

  EdgeStruct<BaseData> Short_;
  Short_.node1 = k;
  Short_.node2 = Node_;

  return Short_;
}

/****************************************************************************/
template <class BaseData, class WeightType>
NetworkAdt<BaseData, WeightType>* NetworkAdt<BaseData, WeightType>::MinimumSpanningTree()
{
  ArrayAdt<BaseData, bool> Included_(_FirstValue, _LastValue);
  BaseData k;
  EdgeStruct<BaseData> e;
  NetworkAdt<BaseData, WeightType>* mst;

  mst = new NetworkAdt<BaseData, WeightType>(_FirstValue, _LastValue);

  mst->AddNode(_FirstValue);
  Included_[_FirstValue] = true;

  for (k = _FirstValue + 1; k <= _LastValue; ++k)
    Included_[k] = false;

  for (k = _FirstValue; k <= _LastValue - 1; k++)
  {
    e = FindMinimum(k, Included_);

    mst->AddNode(e.node1);
    mst->AddNode(e.node2);
    
    Included_[e.node1] = true;
    Included_[e.node2] = true;
    
    msg->AddEdge(e.node1, e.node2, EdgeWeight(e.node1, e.node2));
    msg->AddEdge(e.node2, e.node1, EdgeWeight(e.node2, e.node1));
  }

  return mst;
}

/****************************************************************************/
template <class BaseData, class WeightType>
EdgeStruct<BaseData> NetworkAdt<BaseData, WeightType>::FindMinimum(const BaseData& k,
                                                                 const ArrayAdt<BaseData, bool>& Included_,
                                                                 const ArrayAdt<BaseData, WeightType>& Distance_)
{
  BaseData y, Node_;
  WeightType Dist_ = WeightType(0);

  for (y = _FirstValue; y <= _LastValue; y++)
    if (!Included_[y] && y != k)
    {
      WeightType Next_ = Distance[y];
      if (Dist_ > Next_ || !Dist_)
      {
        Dist_ = Next_;
        Node_ = y;
      }
    }

  EdgeStruct<BaseData> Short_;
  Short_.node1 = k;
  Short_.node2 = Node_;

  return Short_;
}

/****************************************************************************/
template <class BaseData, class WeightType>
void NetworkAdt<BaseData, WeightType>::ShortestPath(const BaseData& Start_, const BaseData& Dest_)
{
  ArrayAdt<BaseData, bool> Included_(_FirstValue, _LastValue);
  _ShortDistance.SetRange(_FirstValue, _LastValue);
  _ShortPath.SetRange(_FirstValue, _LastValue);  
  
  EdgeStruct<BaseData> e;
  BaseData k, r;

  for (k = _FirstValue; k <= _LastValue; ++k)
    Included_[k] = false;
  Included_[Start_] = true;      

  for (k = _FirstValue; k <= _LastValue; ++k)
    _ShortDistance[k] = EdgeWeight(Start_, k);
  _ShortDistance[Start_] = 0;

  for (k = _FirstValue; k <= _LastValue; ++k)
    if (EdgeWeight(Start_, k))
      _ShortPath[k] = Start_;
    else
      _ShortPath[k] = 0;

  do
  {
    e = FindMinimum(Start_, Included_, _ShortDistance);
    Included_[e.node1] = true;
    Included_[e.node2] = true;
    BaseData j = e.node2;    
  
    for (r = _FirstValue; r <= _LastValue - 1; r++)
      if (!Included_[r])
        if (EdgeWeight(j, r))
          if (_ShortDistance[j] + EdgeWeigth(j, r) < _ShortDistance[r])
          {
            _ShortDistance[r] = _ShortDistance[j] + EdgeWeigth(j, r);
            _ShortPath[r] = j;
          }
  }
  while (!Included_[Dest_]);
}

/****************************************************************************/
#endif
