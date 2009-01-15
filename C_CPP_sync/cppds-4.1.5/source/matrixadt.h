#ifndef MATRIXADT_H
#define MATRIXADT_H
#ifndef ARRAYADT_H
  #include "arrayadt.h"
#endif

#define MAX_LIST_SIZE           512

/****************************************************************************/
template <class ColIndexType, class RowIndexType, class BaseData>
class MatrixTypeDefn
{
  public:
    typedef ArrayAdt<ColIndexType, BaseData> ColType;
    typedef ArrayAdt<RowIndexType, ColType> RowColType;
};

template <class ColIndexType, class RowIndexType, class BaseData>
class MatrixAdt
{
  protected:
    MatrixTypeDefn<ColIndexType, RowIndexType, BaseData>::RowColType _Matrix;

  public:
    MatrixAdt() {}
    MatrixAdt(ColIndexType xlo, ColIndexType xhi, RowIndexType ylo, RowIndexType yhi);
    MatrixAdt(const MatrixAdt<ColIndexType, RowIndexType, BaseData>& Obj_);
    ~MatrixAdt() {}

    void SetColRange(ColIndexType xlo, ColIndexType xhi);
    void SetRowRange(RowIndexType ylo, RowIndexType yhi);

    MatrixAdt<ColIndexType, RowIndexType, BaseData>&
    operator = (const MatrixAdt<ColIndexType, RowIndexType, BaseData>& Obj_);

    BaseData Retrieve(ColIndexType x, RowIndexType y);
    void Assign(ColIndexType x, RowIndexType y, const BaseData& Value_);

    ColIndexType ColLowIndex() const
        { return _Matrix[0].LowIndex(); }
    ColIndexType ColHighIndex() const
        { return _Matrix[0].HighIndex(); }

    RowIndexType RowLowIndex() const
        { return _Matrix.LowIndex(); }
    RowIndexType RowHighIndex() const
        { return _Matrix.HighIndex(); }
};

/****************************************************************************/
template <class ColIndexType, class RowIndexType, class BaseData>
MatrixAdt<ColIndexType, RowIndexType, BaseData>::MatrixAdt(ColIndexType xlo, ColIndexType xhi, RowIndexType ylo, RowIndexType yhi)
{
  RowIndexType y;
  _Matrix.SetRange(ylo, yhi);

  for (y = ylo; y <= yhi; y++)
    _Matrix[y].SetRange(xlo, xhi);
}

/****************************************************************************/
template <class ColIndexType, class RowIndexType, class BaseData>
MatrixAdt<ColIndexType, RowIndexType, BaseData>::MatrixAdt(const MatrixAdt<ColIndexType, RowIndexType, BaseData>& Obj_):
_Matrix(Obj_._Matrix)
{}

/****************************************************************************/
template <class ColIndexType, class RowIndexType, class BaseData>
MatrixAdt<ColIndexType, RowIndexType, BaseData>&
MatrixAdt<ColIndexType, RowIndexType, BaseData>::operator = (const MatrixAdt<ColIndexType, RowIndexType, BaseData>& Obj_)
{
  _Matrix = Obj_._Matrix;
  return *this;
}

/****************************************************************************/
template <class ColIndexType, class RowIndexType, class BaseData>
BaseData MatrixAdt<ColIndexType, RowIndexType, BaseData>::Retrieve(ColIndexType x, RowIndexType y)
{
  return _Matrix[y][x];
}

/****************************************************************************/
template <class ColIndexType, class RowIndexType, class BaseData>
void MatrixAdt<ColIndexType, RowIndexType, BaseData>::Assign(ColIndexType x, RowIndexType y, const BaseData& Value_)
{
  _Matrix[y][x] = Value_;
}

/****************************************************************************/
template <class ColIndexType, class RowIndexType, class BaseData>
void MatrixAdt<ColIndexType, RowIndexType, BaseData>::SetColRange(ColIndexType xlo, ColIndexType xhi)
{
  RowIndexType y;
  RowIndexType ylo = _Matrix.LowIndex();
  RowIndexType yhi = _Matrix.HighIndex();
  
  for (y = ylo; y <= yhi; y++)
    _Matrix[y].SetRange(xlo, xhi);
}

/****************************************************************************/
template <class ColIndexType, class RowIndexType, class BaseData>
void MatrixAdt<ColIndexType, RowIndexType, BaseData>::SetRowRange(RowIndexType ylo, RowIndexType yhi)
{
  _Matrix.SetRange(ylo, yhi);
}

/****************************************************************************/
#endif
