#include <iostream.h>
#include <stdlib.h>

struct Sdata
{
  size_t _Source;
  size_t _Target;
  size_t _Reset;
  int _FncNum;
  Sdata* _Next;
};

char* _Data = NULL;
char* _SelData = NULL;
size_t* _Indices = NULL;
size_t _Size = 0;
size_t _Sel = 0;
size_t _Done = 0;
size_t _ZeroTerm = 0;
Sdata* _Stack = NULL;

void Push(size_t s, size_t x, size_t r, int f);
Sdata* Pop();
void ResetCr(size_t x);
void ShiftUp(size_t Index);
void Show();
void CrMoveToTop(size_t Index);
void Combine();
int HasMore();
void MoveUpFromZero(size_t last);
void ReverseData(char* Data_, size_t Size_);

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

void RetrieveCrSel()
{
  size_t cnt = 0;
  size_t x = _Size;

  do
  {
    --x;
    if (_Indices[x])
      _SelData[cnt++] = _Data[x];
  }
  while (x);

  ReverseData(_SelData, _Sel);
}

void ShiftUp(size_t Index_)
{
  _Indices[Index_ + 1] = 1;
  _Indices[Index_] = 0;
}

void ResetCr(size_t x)
{
  if (x && _Indices[x - 1])
    ResetCr(x - 1);

  if (x)
  {
    size_t pos;
    for (pos = x - 1; !_Indices[pos] && pos; pos--);
    if (_Indices[pos])
      ++pos;

    if (pos < x)
    {
      _Indices[x] = 0;
      _Indices[pos] = 1;
    }
  }
}

void CrMoveToTop(size_t Index_)
{
  if (Index_ < _Size - 1)
  {
    if (_Indices[Index_ + 1] == 0)
    {
      ShiftUp(Index_);

      if (Index_ > 0 && _Indices[Index_ - 1])
      {
        ResetCr(Index_ - 1);
        Push(0, 0, 0, 8);
        return;
      }
      else
      {
        Push(Index_ + 1, 0, 0, 6);
        Push(0, 0, 0, 8);
        return;        
      }
    }    
    else
    {
      size_t last = !Index_ || !_Indices[Index_ - 1];
      Push(last, 0, 0, 5);
      CrMoveToTop(Index_ + 1);
      return;      
    }
  }
  else
    _Done = 1;
}

void MoveUpFromZero(size_t Last_)
{
  if (Last_ && !_Done)
    CrMoveToTop(0);
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

void Combine(char* Set_, size_t Size_, size_t Sel_)
{
  _Data = Set_;
  _Size = Size_;
  _Sel = Sel_;
  _Done = 0;    

  if (_Indices)
    delete[] _Indices;
    
  if (_SelData)
    delete[] _SelData;

  _Indices = new size_t[_Size];    
  
  if (_ZeroTerm)
  {
    _SelData = new char[_Size + 1];
    _SelData[_Size] = char(0);
  }
  else
    _SelData = new char[_Size];

  int x;
  for (x = 0; x < _Sel; x++)
    _Indices[x] = 1;

  for (;x < _Size; x++)
    _Indices[x] = 0;

  if (_Sel == _Size)
    Push(0, 0, 0, 8);
  else
  {
    Push(0, 0, 0, 7);
    Push(0, 0, 0, 8);
  }
}

char* NextCombination()
{
  int Found_ = 0;
  Sdata* Node_;

  while (HasMore() && !Found_)
    if (_Stack)
    {
      Node_ = Pop();
        
      if (Node_->_FncNum == 5)
        MoveUpFromZero(Node_->_Source);
      else if (Node_->_FncNum == 6)
        CrMoveToTop(Node_->_Source);
      else if (Node_->_FncNum == 7)
        CrMoveToTop(0);      
      else if (Node_->_FncNum == 8)
      {
        RetrieveCrSel();      
        Found_ = 1;        
      }
      
      delete Node_;
    }

  if (Found_)
    return _SelData;
  
  return NULL;
}

int main()
{
  _Size = 7;
  _Sel = 3;
  _ZeroTerm = 1;
  char* RetData_;
  char* _Data = new char[_Size + 1];

  size_t x;  
  for (x = 0; x < _Size; x++)
    _Data[x] = 'A' + x;
  _Data[_Size] = 0;

  system("clear");
  Combine(_Data, _Size, _Sel);
  cout <<"Has More: " <<HasMore() <<endl;
  
  for (;;)
  {
    RetData_ = NextCombination();
    if (RetData_)
      cout <<RetData_ <<endl;
    else
      break;
  }

  cout <<"Has More: " <<HasMore() <<endl;
  cout <<"Second Run:" <<endl;
  Combine(_Data, _Size, _Sel);  
  cout <<"Has More: " <<HasMore() <<endl;
  
  for (;;)
  {
    RetData_ = NextCombination();
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
  delete[] _SelData;  

  cout <<"SUCCESS Testing combalgo" <<endl;
  return 0;
}



