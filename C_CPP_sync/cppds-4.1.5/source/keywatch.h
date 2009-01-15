#ifndef KEYWATCH_H
#define KEYWATCH_H

struct ControllerCode
{
  int MAXLENGTH;
  volatile char* _KeyArray;
  volatile char* _ExtKeyArray;
};

#ifdef __cplusplus
extern "C"
{
#endif
  const struct ControllerCode ReadControllerCode(void);
  void ExitControllerCode(void);
  void InitControllerCode(void);
#ifdef __cplusplus
}
#endif

#endif