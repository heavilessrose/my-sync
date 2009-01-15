#ifndef BUILTINSERIESCMP_H
#define BUILTINSERIESCMP_H
#ifndef SERIESCOMPARE_CPP
  #include "seriescmp.cpp"
#endif
#ifndef MEMPOOL_H
  #include "mempool.h"
#endif

/****************************************************************************/
template <class T>
class BuiltInSeriesCompare : public SeriesCompare<T>
{
  protected:
    T*(*_AllocFn)(T);
    size_t _Size;
    T** _DataList;

    BuiltInSeriesCompare(T Unit_, T* List_, size_t Sz_,
			 int Scheme_, T Zval_, T Disp_,
			 T*(*AllocFn_)(T), Boolean DelParam_);
    virtual void Clear();

    BuiltInSeriesCompare<T>& SetMinValue(T MinVal_);
    BuiltInSeriesCompare<T>& SetMaxValue(T MaxVal_);

  public:
    virtual ~BuiltInSeriesCompare();
    BuiltInSeriesCompare(const BuiltInSeriesCompare<T>& Obj_);
    BuiltInSeriesCompare<T>& operator = (const BuiltInSeriesCompare<T>& Obj_);
    virtual BuiltInSeriesCompare<T>& FindQuotient();
};

/****************************************************************************/
#endif





