#ifndef LISTADT_H
#define LISTADT_H
#ifndef SIMPLELIST_CPP
  #include "simplelist.cpp"
#endif

#define MAX_LIST_SIZE           512

/****************************************************************************/
template <class BaseData>
class ListAdt
{
  protected:
    SimpleList<BaseData> _List;
    SimpleListIterator<BaseData>* _Iter;

  public:
    ListAdt();
    ListAdt(const ListAdt<BaseData>& initList_);
    ~ListAdt();

    void operator = (const ListAdt<BaseData>& Source_);

    void First();
    void Last();

    void MakeCurrent(size_t Position_);

    void Prev();
    void Next();

    BaseData Examine();

    size_t Current();
    size_t Count();

    void InsertBefore(const BaseData& Item_);
    void InsertAfter(const BaseData& Item_);

    void Remove();
    void Replace(const BaseData& Item_);
};

/****************************************************************************/
template <class BaseData>
ListAdt<BaseData>::ListAdt():
_List(),
_Iter(new SimpleListIterator<BaseData>(&_List))
{}

/****************************************************************************/
template <class BaseData>
ListAdt<BaseData>::ListAdt(const ListAdt<BaseData>& initList_):
_List(initList_),
_Iter(new SimpleListIterator<BaseData>(&_List))
{}

/****************************************************************************/
template <class BaseData>
ListAdt<BaseData>::~ListAdt()
{
  delete _Iter;
}

/****************************************************************************/
template <class BaseData>
void ListAdt<BaseData>::operator = (const ListAdt<BaseData>& Source_)
{
  _List = Source_;
  delete _Iter;
  _Iter = (new SimpleListIterator<BaseData>(&_List));
}

/****************************************************************************/
template <class BaseData>
void ListAdt<BaseData>::First()
{
  _Iter->Head();
}

/****************************************************************************/
template <class BaseData>
void ListAdt<BaseData>::Last()
{
  _Iter->Tail();
}

/****************************************************************************/
template <class BaseData>
void ListAdt<BaseData>::MakeCurrent(size_t Position_)
{
  if (1 <= Position_ && Position_ <= Count())
    _Iter->IndexNode(Position_ - 1);
}

/****************************************************************************/
template <class BaseData>
void ListAdt<BaseData>::Prev()
{
  _Iter->Prev();
}

/****************************************************************************/
template <class BaseData>
void ListAdt<BaseData>::Next()
{
  _Iter->Next();
}

/****************************************************************************/
template <class BaseData>
BaseData ListAdt<BaseData>::Examine()
{
  SimpleNode<BaseData>* Item_ = _Iter->Here();

  if (Item_ && Item_->Value())
    return *(Item_->Value());
}

/****************************************************************************/
template <class BaseData>
size_t ListAdt<BaseData>::Current()
{
  return (_Iter->Position() + 1);
}

/****************************************************************************/
template <class BaseData>
size_t ListAdt<BaseData>::Count()
{
  return _Iter->Count();
}

/****************************************************************************/
template <class BaseData>
void ListAdt<BaseData>::InsertBefore(const BaseData& Item_)
{
  _Iter->InsertBefore(new BaseData(Item_));
}

/****************************************************************************/
template <class BaseData>
void ListAdt<BaseData>::InsertAfter(const BaseData& Item_)
{
  _Iter->InsertAfter(new BaseData(Item_));
}

/****************************************************************************/
template <class BaseData>
void ListAdt<BaseData>::Remove()
{
  _Iter->Remove();
}

/****************************************************************************/
template <class BaseData>
void ListAdt<BaseData>::Replace(const BaseData& Item_)
{
  _Iter->Update(_Iter->Position(), new BaseData(Item_));
}

/****************************************************************************/
#endif
