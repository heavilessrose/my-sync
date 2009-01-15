#ifndef GRAPHADT_H
#define GRAPHADT_H
#ifndef ARRAYADT_H
  #include "arrayadt.h"
#endif
#ifndef LISTADT_H
  #include "listadt.h"
#endif
#ifndef QUEUEADT_H
  #include "queueadt.h"
#endif

/****************************************************************************/
template <class BaseData>
class GraphAdt
{
  protected:
    BaseData _FirstValue, _LastValue;
    ArrayAdt<BaseData, bool>* _Nodes;
    ArrayAdt<BaseData, ListAdt<BaseData>*>* _Edges;

    inline void Xindex() const
        { THROW (FallibleBase::ArrayIndexRangeErr()); }    

  public:
    GraphAdt(const BaseData& firstValue, const BaseData& lastValue);
    GraphAdt(const GraphAdt<BaseData>& initGraph_);

    ~GraphAdt();

    void operator = (const GraphAdt<BaseData>& Source_);

    bool Empty() const;
    bool Full() const;
    bool Edge(const BaseData& Value1_, const BaseData& Value2_);    

    void AddNode(const BaseData& Value_);
    void RemoveNode(const BaseData& Value_);
    
    void AddEdge(const BaseData& Value1_, const BaseData& Value2_);
    void RemoveEdge(const BaseData& Value1_, const BaseData& Value2_);

    void SearchFrom(BaseData& Start_, void(*ProcessNode)(BaseData& Value_), ArrayAdt<BaseData, bool> Visited_);
    void DepthFirstTraversal(BaseData& Start_, void(*ProcessNode)(BaseData& Value_));
    void BreadthFirstTraversal(BaseData& Start_, void(*ProcessNode)(BaseData& Value_));

    GraphAdt<BaseData>* FindTransitiveClosure();
};

/****************************************************************************/
template <class BaseData>
GraphAdt<BaseData>::GraphAdt(const BaseData& firstValue, const BaseData& lastValue):
_FirstValue(firstValue),
_LastValue(lastValue),
_Nodes(new ArrayAdt<BaseData, bool>(firstValue, lastValue)),
_Edges(new ArrayAdt<BaseData, ListAdt<BaseData>*>(firstValue, lastValue))
{}

/****************************************************************************/
template <class BaseData>
GraphAdt<BaseData>::GraphAdt(const GraphAdt<BaseData>& Obj_):
_FirstValue(Obj_._FirstValue),
_LastValue(Obj_._LastValue),
_Nodes(new ArrayAdt<BaseData, bool>(Obj_)),
_Edges(new ArrayAdt<BaseData, ListAdt<BaseData>*>(Obj_._FirstValue, Obj_._LastValue))
{
  BaseData i;
  for (i = Obj_._FirstValue; i <= Obj_._LastValue; i++)
    if (Obj_._Edges[i])
      _Edges[i] = new ListAdt<BaseData>(*Obj_._Edges[i]);
}

/****************************************************************************/
template <class BaseData>
GraphAdt<BaseData>::~GraphAdt()
{
  delete _Nodes;
  delete _Edges;
}

/****************************************************************************/
template <class BaseData>
void GraphAdt<BaseData>::operator = (const GraphAdt<BaseData>& Source_)
{
  delete _Nodes;
  delete _Edges;

  _FirstValue = Obj_._FirstValue;
  _LastValue = Obj_._LastValue;
  _Nodes = new ArrayAdt<BaseData, bool>(Obj_);
  _Edges = new ArrayAdt<BaseData, ListAdt<BaseData>*>(Obj_._FirstValue, Obj_._LastValue);

  BaseData i;
  for (i = Obj_._FirstValue; i <= Obj_._LastValue; i++)
    if (Obj_._Edges[i])
      _Edges[i] = new ListAdt<BaseData>(*Obj_._Edges[i]);

  return *this;
}

/****************************************************************************/
template <class BaseData>
bool GraphAdt<BaseData>::Empty() const
{
  BaseData i;
  for (i = _FirstValue; i <= _LastValue; i++)
    if (_Nodes[i])
      return false;

  return true;
}

/****************************************************************************/
template <class BaseData>
bool GraphAdt<BaseData>::Full() const
{
  BaseData i;
  for (i = _FirstValue; i <= _LastValue; i++)
    if (!_Nodes[i])
      return false;

  return true;
}

/****************************************************************************/
template <class BaseData>
bool GraphAdt<BaseData>::Edge(const BaseData& Value1_, const BaseData& Value2_)
{
  if (!(_FirstValue <= Value1_ && Value1_ <= _LastValue) ||
      !(_FirstValue <= Value2_ && Value2_ <= _LastValue) ||
      (!_Nodes[Value1_] && !_Nodes[Value2_]))
    return false;

  size_t i;

  if (_Nodes[Value1_])
  {
    i = _Edges[Value1_]->Count();
    _Edges[Value1_]->First();
  
    while (i)
    {
      if (_Edges[Value1_]->Examine() == Value2_)
        return true;

      _Edges[Value1_]->Next();
      --i;
    }
  }

  if (_Nodes[Value1_])
  {
    i = _Edges[Value2_]->Count();
    _Edges[Value2_]->First();
  
    while (i)
    {
      if (_Edges[Value2_]->Examine() == Value1_)
        return true;

      _Edges[Value2_]->Next();
      --i;      
    }
  }

  return false;
}

/****************************************************************************/
template <class BaseData>
void GraphAdt<BaseData>::AddNode(const BaseData& Value_)
{
  if (!(_FirstValue <= Value_ && Value_ <= _LastValue))
    Xindex();

  _Nodes[Value_] = true;
}

/****************************************************************************/
template <class BaseData>
void GraphAdt<BaseData>::RemoveNode(const BaseData& Value_)
{
  if (!(_FirstValue <= Value_ && Value_ <= _LastValue))
    Xindex();

  _Nodes[Value_] = false;

  size_t Max_;
  BaseData x;

  for (x = _FirstValue; x <= _LastValue; x++)
  {
    Max_ = _Edges[x]->Count();
    _Edges[x]->First();

    if (x == Value_)    
    {
      while (Max_)
      {
        _Edges[x]->Remove();
        --Max_;      
      }
    }
    else
    {
      while (Max_)
      {
        if (_Edges[x]->Examine() == Value_)
        {
          size_t Pos_ = _Edges[x]->Current();
          _Edges[x]->Remove();
          _Edges[x]->MakeCurrent(Pos_);
        }
        else
          _Edges[x]->Next();
        --Max_;
      }
    }
  }
}

/****************************************************************************/
template <class BaseData>
void GraphAdt<BaseData>::AddEdge(const BaseData& Value1_, const BaseData& Value2_)
{
  if (!(_FirstValue <= Value1_ && Value1_ <= _LastValue) ||
      !(_FirstValue <= Value2_ && Value2_ <= _LastValue))
    Xindex();

  if (!_Nodes[Value1_] && !_Nodes[Value2_])
    return;

  if (_Nodes[Value1_] && _Nodes[Value2_])
  {
    _Edges[Value1_]->Last();
    _Edges[Value1_]->InsertAfter(Value2_);
    _Edges[Value2_]->Last();
    _Edges[Value2_]->InsertAfter(Value1_);    
  }
}

/****************************************************************************/
template <class BaseData>
void GraphAdt<BaseData>::RemoveEdge(const BaseData& Value1_, const BaseData& Value2_)
{
  if (!(_FirstValue <= Value1_ && Value1_ <= _LastValue) ||
      !(_FirstValue <= Value2_ && Value2_ <= _LastValue))
    Xindex();

  if (!_Nodes[Value1_] && !_Nodes[Value2_])
    return;    

  if (_Nodes[Value1_] || _Nodes[Value2_])
  {
    size_t Max_;
  
    Max_ = _Edges[Value1_]->Count();
    _Edges[Value1_]->First();

    while (Max_)
    {
      if (_Edges[Value1_]->Examine() == Value2_)
      {
        size_t Pos_ = _Edges[Value1_]->Current();
        _Edges[Value1_]->Remove();
        _Edges[Value1_]->MakeCurrent(Pos_);
      }
      else
        _Edges[Value1_]->Next();
      --Max_;      
    }
    
    Max_ = _Edges[Value2_]->Count();
    _Edges[Value2_]->First();

    while (Max_)
    {
      if (_Edges[Value2_]->Examine() == Value1_)
      {
        size_t Pos_ = _Edges[Value2_]->Current();
        _Edges[Value2_]->Remove();
        _Edges[Value2_]->MakeCurrent(Pos_);
      }
      else
        _Edges[Value2_]->Next();
      --Max_;      
    }    
  }  
}

/****************************************************************************/
template <class BaseData>
void GraphAdt<BaseData>::SearchFrom(BaseData& Start_, void(*ProcessNode)(BaseData& Value_),
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
template <class BaseData>
void GraphAdt<BaseData>::DepthFirstTraversal(BaseData& Start_, void(*ProcessNode)(BaseData& Value_))
{
  ArrayAdt<BaseData, bool> Visited_(_FirstValue, _LastValue);
  BaseData k;

  for (k = _FirstValue; k <= _LastValue; k++)
    Visited_[k] = false;

  SearchFrom(Start_, ProcessNode, Visited_);
}

/****************************************************************************/
template <class BaseData>
void GraphAdt<BaseData>::BreadthFirstTraversal(BaseData& Start_, void(*ProcessNode)(BaseData& Value_))
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
template <class BaseData>
GraphAdt<BaseData>* GraphAdt<BaseData>::FindTransitiveClosure()
{
  BaseData i, j, k;
  GraphAdt<BaseData>* tc;

  tc = new GraphAdt<BaseData>(*this);

  for (k = _FirstValue; k <= _LastValue; k++)
    for (i = _FirstValue; i <= _LastValue; i++)
      for (j = _FirstValue; j <= _LastValue; j++)
        if ( (*(tc->_Edges)[i][k]) && (*(tc->_Edges)[k][j]) )
          *(tc->_Edges)[i][j] = true;
        
  return (tc);
}

/****************************************************************************/
#endif


