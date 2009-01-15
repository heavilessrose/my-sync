#include <iostream.h>
#include <stdlib.h>

enum { SECTION1, SECTION2, RETURN_DATA, SWAP };

struct Sdata
{
  size_t _Source;
  size_t _Target;
  size_t _Reset;
  int _FncNum;
  Sdata* _Next;
};

char* _Data = NULL;
size_t* _Indices = NULL;
size_t _Size = 0;
size_t _Done = 0;
size_t _ZeroTerm;
Sdata* _Stack = NULL;

void Push(size_t s, size_t x, size_t r, int f);
Sdata* Pop();

void Swap();
void ReverseData(char* Data_, size_t Size_);
void ShiftToTop(char* Ptr_, size_t Dist_, char Item_);
void ShiftBack(char* Ptr_, size_t Dist_, char Item_);
void Reset(size_t s);

void Section1(size_t s, size_t x, size_t r);
void Section2(size_t s);

void MoveToTop(size_t s, size_t x, size_t r);

int HasMore();
void Reload();

void Permute(char* Set_, size_t Size_);
char* NextPermutation();

//void RetrievePrSel();
//void Show();

void Push(size_t s, size_t x, size_t r, int f)
{
  Sdata* Node_ = new Sdata;
  Node_->_Source = s;
  Node_->_Target = x;
  Node_->_Reset = r;
  Node_->_FncNum = f;
  Node_->_Next = _Stack;
  _Stack = Node_;
}

Sdata* Pop()
{
  if (_Stack)
  {
    Sdata* Node_ = _Stack;
    _Stack = _Stack->_Next;
    Node_->_Next = NULL;
    return Node_;
  }

  return NULL;
}

void Swap()
{
  char Temp_ = _Data[0];
  _Data[0] = _Data[1];
  _Data[1] = Temp_;
}

void ShiftToTop(char* Ptr_, size_t Dist_, char Item_)
{
  char* Start_ = Ptr_;
  Ptr_ -= Dist_;
  
  while (Ptr_ != Start_)
  {
    ++Ptr_;
    Ptr_[-1] = Ptr_[0];
  }

  *Ptr_ = Item_;
}

void ShiftBack(char* Ptr_, size_t Dist_, char Item_)
{
  char* End_ = Ptr_ - Dist_ + 1;
  ++Ptr_;

  do
  {
    Ptr_[0] = Ptr_[-1];
    --Ptr_;    
  }
  while (Ptr_ != End_);

  *Ptr_ = Item_;
}

void Reset(size_t s)
{
  // resetting
  ShiftBack(&_Data[s-1], s - _Indices[s], _Data[s]);
}

void MoveToTop(size_t s, size_t x, size_t r)
{
  if (s)
  {
    Push(s, x, r, SECTION1);
    
    if (s > 1)
      MoveToTop(s - 1, s - 2, 0);
    else
      Push(0, 0, 0, RETURN_DATA);
        
    return;
  }

  Section1(s, x, r);
}

void Section1(size_t s, size_t x, size_t r)
{
  if (r)
    Reset(s);
  
  _Indices[s] = x;

  // moving to top
  ShiftToTop(&_Data[s], s - x, _Data[_Indices[s]]);
  
  if (_Indices[s] == 0)
  {
    if (s == 1)
    {
      Push(s, x, r, SECTION2);
      Push(0, 0, 0, RETURN_DATA);
      return;
    }
    else
    {
      Push(s, x, r, SECTION2);
      MoveToTop(s - 1, s - 2, 0);
      return;
    }
      
    Section2(s);
  }
  else
    MoveToTop(s, x - 1, 1);
}

void Section2(size_t s)
{
  Reset(s);
  if (s >= _Size - 1)
    _Done = 1;
}

int HasMore()
{
  return (!_Done || _Stack);
}

void ReverseData(char* Data_, size_t Size_)
{
  size_t x = 0;
  size_t y = Size_ - 1;

  while (x < y)
  {
    char Temp_ = Data_[x];
    Data_[x] = Data_[y];
    Data_[y] = Temp_;

    ++x;
    --y;
  }
}

void Reload()
{
  _Done = 0;  
}

void Permute(char* Set_, size_t Size_)
{
  _Data = Set_;
  _Size = Size_;
  _Done = 0;

  delete[] _Indices;
  _Indices = new size_t[_Size];

  size_t x;
  for (x = 0; x < _Size; x++)
    _Indices[x] = x;

  if (_Size == 1)
  {
    Push(0, 0, 0, RETURN_DATA);
    _Done = 1;
    return;
  }
  else if (Size_ == 2)
  {
    Push(0, 0, 0, RETURN_DATA);
    Push(0, 0, 0, SWAP);
    Push(0, 0, 0, RETURN_DATA);
    _Done = 1;
    return;    
  }
  else
    MoveToTop(_Size - 1, _Size - 2, 0);
}

char* NextPermutation()
{
  int Found_ = 0;
  Sdata* Node_;

  while (HasMore() && !Found_)
    if (_Stack)
    {
      Node_ = Pop();
        
      if (Node_->_FncNum == SECTION1)
        Section1(Node_->_Source, Node_->_Target, Node_->_Reset);
      else if (Node_->_FncNum == RETURN_DATA)
        Found_ = 1;              
      else if (Node_->_FncNum == SECTION2)
        Section2(Node_->_Source);
      else if (Node_->_FncNum == SWAP)
        Swap();

      delete Node_;
    }

  if (Found_)
    return _Data;
  
  return NULL;
}

int main()
{
  _Size = 5;
  char* RetData_;
  _Data = new char[_Size + 1];

  size_t x;  
  for (x = 0; x < _Size; x++)
    _Data[x] = 'A' + x;
  _Data[_Size] = 0;    

  system("clear");
  Permute(_Data, _Size);
  cout <<"Has More: " <<HasMore() <<endl;  
  
  for (;;)
  {
    RetData_ = NextPermutation();
    if (RetData_)
      cout <<RetData_ <<endl;
    else
      break;
  }

  cout <<"Has More: " <<HasMore() <<endl;
  cout <<"Second Run:" <<endl;
  Permute(_Data, _Size);
  cout <<"Has More: " <<HasMore() <<endl;  
  
  for (;;)
  {
    RetData_ = NextPermutation();
    if (RetData_)
      cout <<RetData_ <<endl;
    else
      break;
  }

  cout <<"Has More: " <<HasMore() <<endl;  

  Sdata* Node_;
  while (_Stack)
  {
    Node_ = Pop();
    delete Node_;
  }

  delete[] _Indices;
  delete[] _Data;

  cout <<"SUCCESS Testing permalgo" <<endl;
  return 0;
}


