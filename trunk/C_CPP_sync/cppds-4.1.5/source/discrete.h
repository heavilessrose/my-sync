// FILE : DISCRETE.H
//
// Discrete math library header file.
//
#ifndef DISCRETE_H
#define DISCRETE_H
#ifndef INCL_MATH_H
  #include <math.h>
  #define INCL_MATH_H
#endif
#ifndef FORLOOP_CPP
  #include "forloop.cpp"
#endif

#define DISCRETE_DEBUG          0

#if DISCRETE_DEBUG
  #include <assert.h>
#endif

/****************************************************************************/
template <class TYPE>
class Discrete
{
  protected:
    static TYPE Factorial2(const TYPE& Low_, const TYPE& High_);
  
  public:
    enum
    {
      OBJECTS_DISTINCT          = 1,
      CONTAINERS_DISTINCT       = 2,
      EMPTY_CONTAINERS_ALLOWED  = 4
    };
  
    static TYPE Factorial(const TYPE& Val_);
    static TYPE Permutation(const TYPE& NumObjs_, const TYPE& PermSize_);
    static TYPE Permutation2(const TYPE& SetSize_, const TYPE* IndishObjects_, size_t Len_);
    static TYPE Arrangement(const TYPE& NumObjs_, const TYPE& PermSize_);
    static TYPE Combination(const TYPE& NumObjs_, const TYPE& CombSize_);
    static TYPE PermutationWithRep(const TYPE& NumObjs_, const TYPE& PermSize_);
    static TYPE CombinationWithRep(const TYPE& NumObjs_, const TYPE& CombSize_);
    static TYPE GCD(const TYPE& Num1_, const TYPE& Num2_);
    static TYPE LCM(const TYPE& Num1_, const TYPE& Num2_);
    static TYPE NumberOfRelations(const TYPE& SizeSetA_, const TYPE& SizeSetB_);
    static TYPE NumberOfFunctions(const TYPE& SizeSetA_, const TYPE& SizeSetB_);
    static TYPE NumberOf1To1Functions(const TYPE& SizeSetA_, const TYPE& SizeSetB_);
    static TYPE NumberOfOntoFunctions(const TYPE& SizeSetA_, const TYPE& SizeSetB_);
    static TYPE StirlingNumber2(const TYPE& DistinctObjs_, const TYPE& IdentConts_);
    static TYPE NumberOfDistributions(const TYPE& NumObjects_, const TYPE& NumConts_, int Properties_);
};

/****************************************************************************/
template <class TYPE>
class DiscreteSet;

template <class TYPE>
void Transferring(ForLoop<TYPE>* LoopPtr_, void* ExecBlock_)
{
  ((DiscreteSet<TYPE>*)ExecBlock_)->Accept(LoopPtr_);
}

template <class TYPE>
class DiscreteSet
{
  protected:
    enum
    {
      SECTION1,
      SECTION2,
      RETURN_DATA,
      SWAP,
      MOVE_UP_FROM_ZERO,
      COMB_MOVE_TO_TOP,
      RETRIEVE_SELECTION,
    };

    struct Sdata
    {
      size_t _Source;
      size_t _Target;
      size_t _Reset;
      int _FncNum;
      Sdata* _Next;
    };

    TYPE* _Data;
    TYPE* _SelData;
    size_t* _Indices;
    size_t _Size;
    size_t _Sel;
    size_t _Done;
    size_t _ZeroTerm;
    Sdata* _Stack;

    ForLoop<size_t>** _Loops;
    DiscreteSet<TYPE>* _CascDisc;
    TYPE* _CascData;

    void Push(size_t s, size_t x, size_t r, int f);
    TYPENAME DiscreteSet<TYPE>::Sdata* Pop();
    void ClearLoops();

    void Swap();
    void ReverseData(TYPE* Data_, size_t Size_);
    void ShiftUp(size_t Index_);
    void ShiftToTop(TYPE* Ptr_, size_t Dist_, TYPE Item_);
    void ShiftBack(TYPE* Ptr_, size_t Dist_, TYPE Item_);
    void ResetCr(size_t s);    
    void Reset(size_t s);

    void RetrieveCrSel();
    void RetrievePrWithRepSel();
    void Section1(size_t s, size_t x, size_t r);
    void Section2(size_t s);

    void MoveToTop(size_t s, size_t x, size_t r);
    void MoveUpFromZeroWithRep(size_t last);
    void MoveUpFromZero(size_t last);
    void CrMoveToTop(size_t Index_);
    void CrWithRepMoveToTop(size_t Index_);
    void Permute(TYPE* Set_, size_t Size_);    

  public:
    DiscreteSet(int ZeroTerminated_=0);
    ~DiscreteSet();

    void Accept(ForLoop<size_t>* LoopPtr_);
    int HasMore() const;

    void Permute(TYPE* Set_, size_t Size_, size_t Sel_);
    TYPE* NextPermutation();

    void Combine(TYPE* Set_, size_t Size_, size_t Sel_);
    TYPE* NextCombination();

    void PermuteWithRep(TYPE* Set_, size_t Size_, size_t Sel_);
    TYPE* NextPermutationWithRep();

    void CombineWithRep(TYPE* Set_, size_t Size_, size_t Sel_);
    TYPE* DiscreteSet<TYPE>::NextCombinationWithRep();
};

/****************************************************************************/
#endif




