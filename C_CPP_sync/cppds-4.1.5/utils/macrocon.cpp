#ifndef MACROCON_H
  #include "macrocon.h"
#endif

/****************************************************************************/
void MacroConverter::Convert(const char* MacroName, const char* Host)
{
  if (!strcmp(MacroName, "OBJACCEPTOR_CONSTRUCTOR_METHODDEFN"))
  {
    cout <<"OBJACCEPTOR_CONSTRUCTOR_TOPDEFN("
         <<Host <<"Acceptor::" <<Host <<"Acceptor, "
         <<Host <<")" <<endl;
  }
  else if (!strcmp(MacroName, "OBJACCEPTOR_ACCEPTDATA_METHODDEFN"))
  {
    cout <<"OBJACCEPTOR_ACCEPTDATA_TOPDEFN("
         <<Host <<"Acceptor::AcceptDataPtr, "
         <<Host <<")" <<endl;
  }
  else if (!strcmp(MacroName, "OBJACCEPTOR_GIVEDATA_METHODDEFN"))
  {
    cout <<"OBJACCEPTOR_GIVEDATA_TOPDEFN("
         <<Host <<"Acceptor::GiveDataPtr)" <<endl;
  }
  else if (!strcmp(MacroName, "OBJACCEPTORTEMP_CONSTRUCTOR_METHODDEFN"))
  {
    cout <<"template <class T>" <<endl
         <<"OBJACCEPTOR_CONSTRUCTOR_TOPDEFN("
         <<Host <<"Acceptor<T>::" <<Host <<"Acceptor, "
         <<Host <<"<T>)" <<endl;
  }
  else if (!strcmp(MacroName, "OBJACCEPTORTEMP_ACCEPTDATA_METHODDEFN"))
  {
    cout <<"template <class T>" <<endl
         <<"OBJACCEPTOR_ACCEPTDATA_TOPDEFN("
         <<Host <<"Acceptor<T>::AcceptDataPtr, "
         <<Host <<"<T>)" <<endl;
  }
  else if (!strcmp(MacroName, "OBJACCEPTORTEMP_GIVEDATA_METHODDEFN"))
  {
    cout <<"template <class T>" <<endl
         <<"OBJACCEPTOR_GIVEDATA_TOPDEFN("
         <<Host <<"Acceptor<T>::GiveDataPtr)" <<endl;
  }
  else if (!strcmp(MacroName, "OBJACCEPTORTEMP_HOSTCREATEFNC_METHODDEFN"))
  {
    cout <<"template <class T>" <<endl
         <<"OBJACCEPTOR_CREATEFNC_TOPDEFN("
         <<Host <<"Acceptor<T>::CreateFrom"
         <<Host <<", " <<Host <<"<T>, " <<Host <<"<T>)" <<endl;    
  }
  else if (!strcmp(MacroName, "OBJACCEPTORTEMP_HOSTASSIGNFNC_METHODDEFN"))
  {
    cout <<"template <class T>" <<endl
         <<"OBJACCEPTOR_ASSIGNFNC_TOPDEFN("
         <<Host <<"Acceptor<T>::AssignFrom"
         <<Host <<", " <<Host <<"<T>)" <<endl;  
  }
  else if (!strcmp(MacroName, "OBJECTIMP_GIVEOBJECTACCEPTOR_METHODDEFN"))
  {
    cout <<"OBJECTIMP_GIVEOBJECTACCEPTOR_TOPDEFN("
         <<Host <<"::GiveObjectAcceptor, " <<Host <<"Acceptor)" <<endl;
  }
  else if (!strcmp(MacroName, "OBJECTIMP_ASSIGN_METHODDEFN"))
  {
    cout <<"OBJECTIMP_ASSIGN_TOPDEFN("
         <<Host <<"::Assign, " <<Host <<"Acceptor, ObjComp_->AssignFrom"
         <<Host <<")" <<endl;    
  }
  else if (!strcmp(MacroName, "OBJECTIMP_CLONE_METHODDEFN"))
  {
    cout <<"OBJECTIMP_CLONE_TOPDEFN("
         <<Host <<"::Clone, " <<Host <<", " <<Host
         <<"Acceptor, ObjComp_->CreateFrom" <<Host <<")" <<endl;
  }
  else if (!strcmp(MacroName, "OBJECTIMP_GIVENULLOBJECT_METHODDEFN"))
  {
    cout <<"OBJECTIMP_GIVENULLOBJECT_TOPDEFN("
         <<Host <<"::GiveNullObject, " <<Host <<")" <<endl;
  }
  else if (!strcmp(MacroName, "OBJECTIMP_NULLOBJECT_METHODDEFN"))
  {
    cout <<"OBJECTIMP_NULLOBJECT_TOPDEFN("
         <<Host <<"::NULLOBJECT, " <<Host <<"::GiveNullObject)" <<endl;
  }
  else if (!strcmp(MacroName, "OBJECTIMPTEMP_GIVEOBJECTACCEPTOR_METHODDEFN"))
  {
    cout <<"template <class T>" <<endl
         <<"OBJECTIMP_GIVEOBJECTACCEPTOR_TOPDEFN("
         <<Host <<"<T>::GiveObjectAcceptor, "
         <<Host <<"Acceptor<T>)" <<endl;
  }
  else if (!strcmp(MacroName, "OBJECTIMPTEMP_ASSIGN_METHODDEFN"))
  {
    cout <<"template <class T>" <<endl
         <<"OBJECTIMP_ASSIGN_TOPDEFN("
         <<Host <<"<T>::Assign, "
         <<Host <<"Acceptor<T>, ObjComp_->AssignFrom"
         <<Host <<")" <<endl;
  }
  else if (!strcmp(MacroName, "OBJECTIMPTEMP_CLONE_METHODDEFN"))
  {
    cout <<"template <class T>" <<endl
         <<"OBJECTIMP_CLONE_TOPDEFN(" <<Host <<"<T>::Clone, "
         <<Host <<"<T>, " <<Host <<"Acceptor<T>, ObjComp_->CreateFrom"
         <<Host <<")" <<endl;
  }
  else if (!strcmp(MacroName, "OBJECTIMPTEMP_GIVENULLOBJECT_METHODDEFN"))
  {
    cout <<"template <class T>" <<endl
         <<"OBJECTIMP_GIVENULLOBJECT_TOPDEFN(" <<Host
         <<"<T>::GiveNullObject, " <<Host <<"<T>)" <<endl;
  }
  else if (!strcmp(MacroName, "OBJECTIMPTEMP_NULLOBJECT_METHODDEFN"))
  {
    cout <<"template <class T>" <<endl
         <<"OBJECTIMP_NULLOBJECT_TOPDEFN(" <<Host <<"<T>::NULLOBJECT, "
         <<Host <<"<T>::GiveNullObject)" <<endl;
  }
  else if (!strcmp(MacroName, "OBJACCEPTOR_HOSTCLASS_DEFN"))
  {
    Convert("OBJACCEPTOR_CONSTRUCTOR_METHODDEFN", Host);
    Convert("OBJACCEPTOR_ACCEPTDATA_METHODDEFN", Host);
    Convert("OBJACCEPTOR_GIVEDATA_METHODDEFN", Host);
    Convert("OBJACCEPTOR_CREATEFNC_METHODDEFN", NULL, Host, Host);
    Convert("OBJACCEPTOR_ASSIGNFNC_METHODDEFN", NULL, Host, Host);
    Convert("OBJACCEPTOR_HOSTCOMPFNC_METHODDEFN", "IsLesserTo", Host, NULL);
    Convert("OBJACCEPTOR_HOSTCOMPFNC_METHODDEFN", "IsEqualTo", Host, NULL);
    Convert("OBJACCEPTOR_HOSTCOMPFNC_METHODDEFN", "IsGreaterTo", Host, NULL);
    Convert("MEMORYOPS_DEFN", NULL, Host, "Acceptor");
  }
  else if (!strcmp(MacroName, "OBJACCEPTOR_TEMPLATE_HOSTCLASS_DEFN"))
  {
    Convert("OBJACCEPTORTEMP_CONSTRUCTOR_METHODDEFN", Host);
    Convert("OBJACCEPTORTEMP_ACCEPTDATA_METHODDEFN", Host);
    Convert("OBJACCEPTORTEMP_GIVEDATA_METHODDEFN", Host);
    Convert("OBJACCEPTORTEMP_HOSTCREATEFNC_METHODDEFN", Host);
    Convert("OBJACCEPTORTEMP_HOSTASSIGNFNC_METHODDEFN", Host);    
    Convert("OBJACCEPTORTEMP_HOSTCOMPFNC_METHODDEFN", "IsLesserTo", Host, NULL);
    Convert("OBJACCEPTORTEMP_HOSTCOMPFNC_METHODDEFN", "IsEqualTo", Host, NULL);
    Convert("OBJACCEPTORTEMP_HOSTCOMPFNC_METHODDEFN", "IsGreaterTo", Host, NULL);
    Convert("MEMORYOPS_TEMPLATE_DEFN", NULL, Host, "Acceptor");
  }  
  else if (!strcmp(MacroName, "OBJECTIMP_COMMONFUNCS_DEFN"))
  {
    Convert("OBJECTIMP_GIVEOBJECTACCEPTOR_METHODDEFN", Host);
    Convert("OBJECTIMP_ASSIGN_METHODDEFN", Host);
    Convert("OBJECTIMP_CLONE_METHODDEFN", Host);
    Convert("OBJECTIMP_GIVENULLOBJECT_METHODDEFN", Host);
    Convert("OBJECTIMP_NULLOBJECT_METHODDEFN", Host);
    Convert("OBJECTIMP_COMPFNC_METHODDEFN", "IsEqual", Host, NULL);
    Convert("OBJECTIMP_COMPFNC_METHODDEFN", "IsLesser", Host, NULL);
    Convert("OBJECTIMP_COMPFNC_METHODDEFN", "IsGreater", Host, NULL);
  }
  else if (!strcmp(MacroName, "OBJECTIMPTEMP_COMMONFUNCS_DEFN"))
  {
    Convert("OBJECTIMPTEMP_GIVEOBJECTACCEPTOR_METHODDEFN", Host);
    Convert("OBJECTIMPTEMP_ASSIGN_METHODDEFN", Host);
    Convert("OBJECTIMPTEMP_CLONE_METHODDEFN", Host);
    Convert("OBJECTIMPTEMP_GIVENULLOBJECT_METHODDEFN", Host);
    Convert("OBJECTIMPTEMP_NULLOBJECT_METHODDEFN", Host);
    Convert("OBJECTIMPTEMP_COMPFNC_METHODDEFN", "IsEqual", Host, NULL);
    Convert("OBJECTIMPTEMP_COMPFNC_METHODDEFN", "IsLesser", Host, NULL);
    Convert("OBJECTIMPTEMP_COMPFNC_METHODDEFN", "IsGreater", Host, NULL);
  }  
}

/****************************************************************************/
void MacroConverter::Convert(const char* MacroName, const char* Prefix,
                             const char* Host, const char* Target)
{
  if (!strcmp(MacroName, "MEMORYOPS_DEFN"))
  {
    cout <<"MEMORYOPS_DEFN("
         <<(Prefix ? Prefix:"") <<Host
         <<(Target ? Target:"") <<")" <<endl;
  }
  else if (!strcmp(MacroName, "MEMORYOPS_TEMPLATE_DEFN"))
  {
    cout <<"MEMORYOPS_TEMPLATE_DEFN("
         <<(Prefix ? Prefix:"") <<Host
         <<(Target ? Target:"") <<")" <<endl;
  }  
  else if (!strcmp(MacroName, "OBJACCEPTOR_HOSTCOMPFNC_METHODDEFN"))
  {
    cout <<"OBJACCEPTOR_HOSTCOMPFNC_TOPDEFN("
         <<Host <<"Acceptor::"
         <<Prefix <<Host <<", _ObjPtr->" <<Prefix <<Host <<", " <<Host
         <<")" <<endl;
  }
  else if (!strcmp(MacroName, "OBJACCEPTOR_CREATEFNC_METHODDEFN"))  
  {
    cout <<"OBJACCEPTOR_CREATEFNC_TOPDEFN("
         <<Host <<"Acceptor::CreateFrom" <<Target <<", " <<Host <<", "
         <<Target <<")" <<endl;
  }
  else if (!strcmp(MacroName, "OBJACCEPTOR_ASSIGNFNC_METHODDEFN"))
  {
    cout <<"OBJACCEPTOR_ASSIGNFNC_TOPDEFN("
         <<Host <<"Acceptor::AssignFrom" <<Target <<", "
         <<Target <<")" <<endl;  
  }
  else if (!strcmp(MacroName, "OBJACCEPTOR_COMPFNC_METHODDEFN"))
  {
    cout <<"OBJACCEPTOR_COMPFNC_TOPDEFN("
         <<Host <<"Acceptor::" <<Prefix <<Target <<", "
         <<"_ObjPtr->" <<Prefix <<Host <<", " <<Host <<", "
         <<Target <<")" <<endl;
  }
  else if (!strcmp(MacroName, "OBJACCEPTOR_CREATEFROMTYPEWRAPPER_METHODDEFN"))
  {
    cout <<"OBJACCEPTOR_CREATEFROMTYPEWRAPPER_TOPDEFN("
         <<Host <<"Acceptor::CreateFrom" <<Target <<", "
         <<Host <<")" <<endl;    
  }
  else if (!strcmp(MacroName, "OBJACCEPTOR_ASSIGNFROMTYPEWRAPPER_METHODDEFN"))
  {
    cout <<"OBJACCEPTOR_ASSIGNFROMTYPEWRAPPER_TOPDEFN("
         <<Host <<"Acceptor::AssignFrom" <<Target <<")" <<endl;    
  }
  else if (!strcmp(MacroName, "OBJACCEPTOR_COMPFROMTYPEWRAPPER_METHODDEFN"))
  {
    cout <<"OBJACCEPTOR_COMPFROMTYPEWRAPPER_TOPDEFN("
         <<Host <<"Acceptor::" <<Prefix <<Target <<", _ObjPtr->"
         <<Prefix <<Host <<", " <<Host <<")" <<endl;
  }
  else if (!strcmp(MacroName, "OBJACCEPTOR_CREATEFROMRATIONAL_METHODDEFN"))
  {
    cout <<"OBJACCEPTOR_CREATEFROMRATIONAL_TOPDEFN("
         <<Host <<"Acceptor::CreateFrom" <<Target <<", "
         <<Host <<")" <<endl;
  }
  else if (!strcmp(MacroName, "OBJACCEPTOR_ASSIGNFROMRATIONAL_METHODDEFN"))
  {
    cout <<"OBJACCEPTOR_ASSIGNFROMRATIONAL_TOPDEFN("
         <<Host <<"Acceptor::AssignFrom" <<Target <<")" <<endl;
  }
  else if (!strcmp(MacroName, "OBJACCEPTOR_COMPFROMRATIONAL_METHODDEFN"))
  {
    cout <<"OBJACCEPTOR_COMPFROMRATIONAL_TOPDEFN("
         <<Host <<"Acceptor::" <<Prefix <<Target <<", _ObjPtr->"
         <<Prefix <<Host <<", " <<Host <<")" <<endl;
  }
  else if (!strcmp(MacroName, "OBJECTIMP_COMPFNC_METHODDEFN"))
  {
    cout <<"OBJACCEPTOR_COMPFNC_TOPDEFN("
         <<Host <<"::" <<Prefix <<", " <<Host <<"Acceptor, Obj_."
         <<Prefix <<", ObjComp_->" <<Prefix <<"To" <<Host <<")" <<endl;
  }  
  else if (!strcmp(MacroName, "OBJACCEPTORTEMP_HOSTCOMPFNC_METHODDEFN"))
  {
    cout <<"template <class T>" <<endl
         <<"OBJACCEPTOR_HOSTCOMPFNC_TOPDEFN("
         <<Host <<"Acceptor<T>::" <<Prefix <<Host <<", _ObjPtr->"
         <<Prefix <<Host <<", " <<Host <<"<T>)" <<endl;    
  }
  else if (!strcmp(MacroName, "OBJACCEPTORTEMP_CREATEFNC_METHODDEFN"))
  {
    cout <<"template <class T>" <<endl
         <<"OBJACCEPTOR_CREATEFNC_TOPDEFN("
         <<Host <<"Acceptor<T>::CreateFrom"
         <<Target <<", " <<Host <<"<T>, " <<Target <<")" <<endl;    
  }
  else if (!strcmp(MacroName, "OBJACCEPTORTEMP_ASSIGNFNC_METHODDEFN"))
  {
    cout <<"template <class T>" <<endl
         <<"OBJACCEPTOR_ASSIGNFNC_TOPDEFN("
         <<Host <<"Acceptor<T>::AssignFrom"
         <<Target <<", " <<Target <<")" <<endl;    
  }
  else if (!strcmp(MacroName, "OBJACCEPTORTEMP_COMPFNC_METHODDEFN"))
  {
    cout <<"template <class T>" <<endl
         <<"OBJACCEPTOR_COMPFNC_TOPDEFN("
         <<Host <<"Acceptor<T>::" <<Prefix <<Target <<", _ObjPtr->"
         <<Prefix <<Host <<", " <<Host <<"<T>, " <<Target <<")" <<endl;
  }
  else if (!strcmp(MacroName, "OBJACCEPTORTEMP_CREATEFROMTYPEWRAPPER_METHODDEFN"))
  {
    cout <<"template <class T>" <<endl
         <<"OBJACCEPTOR_CREATEFROMTYPEWRAPPER_TOPDEFN("
         <<Host <<"Acceptor<T>::CreateFrom" <<Target <<", "
         <<Host <<"<T>)" <<endl;  
  }
  else if (!strcmp(MacroName, "OBJACCEPTORTEMP_ASSIGNFROMTYPEWRAPPER_METHODDEFN"))
  {
    cout <<"template <class T>" <<endl
         <<"OBJACCEPTOR_ASSIGNFROMTYPEWRAPPER_TOPDEFN("
         <<Host <<"Acceptor<T>::AssignFrom" <<Target <<")" <<endl;    
  }
  else if (!strcmp(MacroName, "OBJACCEPTORTEMP_COMPFROMTYPEWRAPPER_METHODDEFN"))
  {
    cout <<"template <class T>" <<endl
         <<"OBJACCEPTOR_COMPFROMTYPEWRAPPER_TOPDEFN("
         <<Host <<"Acceptor<T>::" <<Prefix <<Target <<", _ObjPtr->"
         <<Prefix <<Host <<", " <<Host <<"<T>)" <<endl;  
  }
  else if (!strcmp(MacroName, "OBJACCEPTORTEMP_CREATEFROMRATIONAL_METHODDEFN"))
  {
    cout <<"template <class T>" <<endl
         <<"OBJACCEPTOR_CREATEFROMRATIONAL_TOPDEFN("
         <<Host <<"Acceptor<T>::CreateFrom" <<Target <<", "
         <<Host <<"<T>)" <<endl;    
  }
  else if (!strcmp(MacroName, "OBJACCEPTORTEMP_ASSIGNFROMRATIONAL_METHODDEFN"))
  {
    cout <<"template <class T>" <<endl
         <<"OBJACCEPTOR_ASSIGNFROMRATIONAL_TOPDEFN("
         <<Host <<"Acceptor<T>::AssignFrom" <<Target <<")" <<endl;    
  }
  else if (!strcmp(MacroName, "OBJACCEPTORTEMP_COMPFROMRATIONAL_METHODDEFN"))
  {
    cout <<"template <class T>" <<endl
         <<"OBJACCEPTOR_COMPFROMRATIONAL_TOPDEFN("
         <<Host <<"Acceptor<T>::" <<Prefix <<Target
         <<", _ObjPtr->" <<Prefix <<Host <<", " <<Host <<"<T>)" <<endl;    
  }
  else if (!strcmp(MacroName, "OBJECTIMPTEMP_COMPFNC_METHODDEFN"))
  {
    cout <<"template <class T>" <<endl
         <<"OBJACCEPTOR_COMPFNC_TOPDEFN(" <<Host <<"<T>::" <<Prefix
         <<", " <<Host <<"Acceptor<T>, Obj_." <<Prefix <<", ObjComp_->"
         <<Prefix <<"To" <<Host <<")" <<endl;  
  }
  else if (!strcmp(MacroName, "OBJACCEPTOR_DISPATCHFUNCS_DEFN"))
  {
    Convert("OBJACCEPTOR_CREATEFNC_METHODDEFN", NULL, Host, Target);
    Convert("OBJACCEPTOR_ASSIGNFNC_METHODDEFN", NULL, Host, Target);
    Convert("OBJACCEPTOR_COMPFNC_METHODDEFN", "IsLesserTo", Host, Target);
    Convert("OBJACCEPTOR_COMPFNC_METHODDEFN", "IsEqualTo", Host, Target);
    Convert("OBJACCEPTOR_COMPFNC_METHODDEFN", "IsGreaterTo", Host, Target);    
  }
  else if (!strcmp(MacroName, "OBJACCEPTOR_TYPEWRAPPER_DISPATCHFUNCS_DEFN"))
  {
    Convert("OBJACCEPTOR_CREATEFROMTYPEWRAPPER_METHODDEFN", NULL, Host, Target);
    Convert("OBJACCEPTOR_ASSIGNFROMTYPEWRAPPER_METHODDEFN", NULL, Host, Target);
    Convert("OBJACCEPTOR_COMPFROMTYPEWRAPPER_METHODDEFN", "IsLesserTo", Host, Target);
    Convert("OBJACCEPTOR_COMPFROMTYPEWRAPPER_METHODDEFN", "IsEqualTo", Host, Target);
    Convert("OBJACCEPTOR_COMPFROMTYPEWRAPPER_METHODDEFN", "IsGreaterTo", Host, Target);
  }
  else if (!strcmp(MacroName, "OBJACCEPTOR_RATIONAL_DISPATCHFUNCS_DEFN"))
  {
    Convert("OBJACCEPTOR_CREATEFROMRATIONAL_METHODDEFN", NULL, Host, Target);
    Convert("OBJACCEPTOR_ASSIGNFROMRATIONAL_METHODDEFN", NULL, Host, Target);
    Convert("OBJACCEPTOR_COMPFROMRATIONAL_METHODDEFN", "IsLesserTo", Host, Target);
    Convert("OBJACCEPTOR_COMPFROMRATIONAL_METHODDEFN", "IsEqualTo", Host, Target);
    Convert("OBJACCEPTOR_COMPFROMRATIONAL_METHODDEFN", "IsGreaterTo", Host, Target);  
  }
  else if (!strcmp(MacroName, "OBJACCEPTOR_TEMPLATE_DISPATCHFUNCS_DEFN"))
  {
    Convert("OBJACCEPTORTEMP_CREATEFNC_METHODDEFN", NULL, Host, Target);
    Convert("OBJACCEPTORTEMP_ASSIGNFNC_METHODDEFN", NULL, Host, Target);
    Convert("OBJACCEPTORTEMP_COMPFNC_METHODDEFN", "IsLesserTo", Host, Target);
    Convert("OBJACCEPTORTEMP_COMPFNC_METHODDEFN", "IsEqualTo", Host, Target);
    Convert("OBJACCEPTORTEMP_COMPFNC_METHODDEFN", "IsGreaterTo", Host, Target);
  }
  else if (!strcmp(MacroName, "OBJACCEPTOR_TEMPLATE_TYPEWRAPPER_DISPATCHFUNCS_DEFN"))
  {
    Convert("OBJACCEPTORTEMP_CREATEFROMTYPEWRAPPER_METHODDEFN", NULL, Host, Target);
    Convert("OBJACCEPTORTEMP_ASSIGNFROMTYPEWRAPPER_METHODDEFN", NULL, Host, Target);
    Convert("OBJACCEPTORTEMP_COMPFROMTYPEWRAPPER_METHODDEFN", "IsLesserTo", Host, Target);
    Convert("OBJACCEPTORTEMP_COMPFROMTYPEWRAPPER_METHODDEFN", "IsEqualTo", Host, Target);
    Convert("OBJACCEPTORTEMP_COMPFROMTYPEWRAPPER_METHODDEFN", "IsGreaterTo", Host, Target);
  }
  else if (!strcmp(MacroName, "OBJACCEPTOR_TEMPLATE_RATIONAL_DISPATCHFUNCS_DEFN"))
  {
    Convert("OBJACCEPTORTEMP_CREATEFROMRATIONAL_METHODDEFN", NULL, Host, Target);
    Convert("OBJACCEPTORTEMP_ASSIGNFROMRATIONAL_METHODDEFN", NULL, Host, Target);
    Convert("OBJACCEPTORTEMP_COMPFROMRATIONAL_METHODDEFN", "IsLesserTo", Host, Target);
    Convert("OBJACCEPTORTEMP_COMPFROMRATIONAL_METHODDEFN", "IsEqualTo", Host, Target);
    Convert("OBJACCEPTORTEMP_COMPFROMRATIONAL_METHODDEFN", "IsGreaterTo", Host, Target);
  }
  else if (!strcmp(MacroName, "OBJACCEPTOR_HOSTCLASS_DEFN"))
  {
    Convert("OBJACCEPTOR_CONSTRUCTOR_METHODDEFN", Host);
    Convert("OBJACCEPTOR_ACCEPTDATA_METHODDEFN", Host);
    Convert("OBJACCEPTOR_GIVEDATA_METHODDEFN", Host);
    Convert("OBJACCEPTOR_CREATEFNC_METHODDEFN", NULL, Host, Host);
    Convert("OBJACCEPTOR_ASSIGNFNC_METHODDEFN", NULL, Host, Host);
    Convert("OBJACCEPTOR_HOSTCOMPFNC_METHODDEFN", "IsLesserTo", Host, NULL);
    Convert("OBJACCEPTOR_HOSTCOMPFNC_METHODDEFN", "IsEqualTo", Host, NULL);
    Convert("OBJACCEPTOR_HOSTCOMPFNC_METHODDEFN", "IsGreaterTo", Host, NULL);
    Convert("MEMORYOPS_DEFN", NULL, Host, "Acceptor");
  }
  else if (!strcmp(MacroName, "OBJACCEPTOR_TEMPLATE_HOSTCLASS_DEFN"))
  {
    Convert("OBJACCEPTORTEMP_CONSTRUCTOR_METHODDEFN", Host);
    Convert("OBJACCEPTORTEMP_ACCEPTDATA_METHODDEFN", Host);
    Convert("OBJACCEPTORTEMP_GIVEDATA_METHODDEFN", Host);
    Convert("OBJACCEPTORTEMP_HOSTCREATEFNC_METHODDEFN", Host);
    Convert("OBJACCEPTORTEMP_HOSTASSIGNFNC_METHODDEFN", Host);    
    Convert("OBJACCEPTORTEMP_HOSTCOMPFNC_METHODDEFN", "IsLesserTo", Host, NULL);
    Convert("OBJACCEPTORTEMP_HOSTCOMPFNC_METHODDEFN", "IsEqualTo", Host, NULL);
    Convert("OBJACCEPTORTEMP_HOSTCOMPFNC_METHODDEFN", "IsGreaterTo", Host, NULL);
    Convert("MEMORYOPS_TEMPLATE_DEFN", NULL, Host, "Acceptor");
  }  
  else if (!strcmp(MacroName, "OBJECTIMP_COMMONFUNCS_DEFN"))
  {
    Convert("OBJECTIMP_GIVEOBJECTACCEPTOR_METHODDEFN", Host);
    Convert("OBJECTIMP_ASSIGN_METHODDEFN", Host);
    Convert("OBJECTIMP_CLONE_METHODDEFN", Host);
    Convert("OBJECTIMP_GIVENULLOBJECT_METHODDEFN", Host);
    Convert("OBJECTIMP_NULLOBJECT_METHODDEFN", Host);
    Convert("OBJECTIMP_COMPFNC_METHODDEFN", "IsEqual", Host, NULL);
    Convert("OBJECTIMP_COMPFNC_METHODDEFN", "IsLesser", Host, NULL);
    Convert("OBJECTIMP_COMPFNC_METHODDEFN", "IsGreater", Host, NULL);
  }
  else if (!strcmp(MacroName, "OBJECTIMPTEMP_COMMONFUNCS_DEFN"))
  {
    Convert("OBJECTIMPTEMP_GIVEOBJECTACCEPTOR_METHODDEFN", Host);
    Convert("OBJECTIMPTEMP_ASSIGN_METHODDEFN", Host);
    Convert("OBJECTIMPTEMP_CLONE_METHODDEFN", Host);
    Convert("OBJECTIMPTEMP_GIVENULLOBJECT_METHODDEFN", Host);
    Convert("OBJECTIMPTEMP_NULLOBJECT_METHODDEFN", Host);
    Convert("OBJECTIMPTEMP_COMPFNC_METHODDEFN", "IsEqual", Host, NULL);
    Convert("OBJECTIMPTEMP_COMPFNC_METHODDEFN", "IsLesser", Host, NULL);
    Convert("OBJECTIMPTEMP_COMPFNC_METHODDEFN", "IsGreater", Host, NULL);
  }    
}

/****************************************************************************/
int main()
{
  // for Rational class
#if CONVERT_RATIONAL
  MacroConverter::Convert("OBJACCEPTOR_TEMPLATE_HOSTCLASS_DEFN", "Rational");
  MacroConverter::Convert("OBJACCEPTOR_TEMPLATE_DISPATCHFUNCS_DEFN", NULL, "Rational", "ChrString");
  MacroConverter::Convert("OBJACCEPTOR_TEMPLATE_TYPEWRAPPER_DISPATCHFUNCS_DEFN", NULL, "Rational", "LongNumber");
  MacroConverter::Convert("OBJACCEPTOR_TEMPLATE_TYPEWRAPPER_DISPATCHFUNCS_DEFN", NULL, "Rational", "ULongInt");
  MacroConverter::Convert("OBJACCEPTOR_TEMPLATE_TYPEWRAPPER_DISPATCHFUNCS_DEFN", NULL, "Rational", "UInteger");
  MacroConverter::Convert("OBJACCEPTOR_TEMPLATE_TYPEWRAPPER_DISPATCHFUNCS_DEFN", NULL, "Rational", "UShortInt");
  MacroConverter::Convert("OBJACCEPTOR_TEMPLATE_TYPEWRAPPER_DISPATCHFUNCS_DEFN", NULL, "Rational", "LongInt");
  MacroConverter::Convert("OBJACCEPTOR_TEMPLATE_TYPEWRAPPER_DISPATCHFUNCS_DEFN", NULL, "Rational", "Integer");
  MacroConverter::Convert("OBJACCEPTOR_TEMPLATE_TYPEWRAPPER_DISPATCHFUNCS_DEFN", NULL, "Rational", "ShortInt");
  MacroConverter::Convert("OBJACCEPTOR_TEMPLATE_TYPEWRAPPER_DISPATCHFUNCS_DEFN", NULL, "Rational", "Double");
  MacroConverter::Convert("OBJACCEPTOR_TEMPLATE_TYPEWRAPPER_DISPATCHFUNCS_DEFN", NULL, "Rational", "LongDouble");
  MacroConverter::Convert("OBJACCEPTOR_TEMPLATE_TYPEWRAPPER_DISPATCHFUNCS_DEFN", NULL, "Rational", "Float");
#endif

  // for Date class
#if CONVERT_DATE
  MacroConverter::Convert("OBJACCEPTOR_HOSTCLASS_DEFN", "Date");
  MacroConverter::Convert("OBJACCEPTOR_DISPATCHFUNCS_DEFN", NULL, "Date", "ChrString");
  MacroConverter::Convert("OBJACCEPTOR_DISPATCHFUNCS_DEFN", NULL, "Date", "Year");
  MacroConverter::Convert("OBJACCEPTOR_DISPATCHFUNCS_DEFN", NULL, "Date", "YMonth");  
#endif

#if CONVERT_DOUBLE
  MacroConverter::Convert("OBJACCEPTOR_HOSTCLASS_DEFN", "Double");
  MacroConverter::Convert("OBJACCEPTOR_DISPATCHFUNCS_DEFN", NULL, "Double", "ChrString");
  MacroConverter::Convert("OBJACCEPTOR_TYPEWRAPPER_DISPATCHFUNCS_DEFN", NULL, "Double", "LongNumber");
  MacroConverter::Convert("OBJACCEPTOR_TYPEWRAPPER_DISPATCHFUNCS_DEFN", NULL, "Double", "ULongInt");
  MacroConverter::Convert("OBJACCEPTOR_TYPEWRAPPER_DISPATCHFUNCS_DEFN", NULL, "Double", "UInteger");
  MacroConverter::Convert("OBJACCEPTOR_TYPEWRAPPER_DISPATCHFUNCS_DEFN", NULL, "Double", "UShortInt");
  MacroConverter::Convert("OBJACCEPTOR_TYPEWRAPPER_DISPATCHFUNCS_DEFN", NULL, "Double", "LongInt");
  MacroConverter::Convert("OBJACCEPTOR_TYPEWRAPPER_DISPATCHFUNCS_DEFN", NULL, "Double", "Integer");
  MacroConverter::Convert("OBJACCEPTOR_TYPEWRAPPER_DISPATCHFUNCS_DEFN", NULL, "Double", "ShortInt");
  MacroConverter::Convert("OBJACCEPTOR_TYPEWRAPPER_DISPATCHFUNCS_DEFN", NULL, "Double", "Float");
  MacroConverter::Convert("OBJACCEPTOR_TYPEWRAPPER_DISPATCHFUNCS_DEFN", NULL, "Double", "LongDouble");
  MacroConverter::Convert("OBJACCEPTOR_RATIONAL_DISPATCHFUNCS_DEFN", NULL, "Double", "Rational");
#endif

#if CONVERT_FLOAT
  MacroConverter::Convert("OBJACCEPTOR_HOSTCLASS_DEFN", "Float");
  MacroConverter::Convert("OBJACCEPTOR_DISPATCHFUNCS_DEFN", NULL, "Float", "ChrString");
  MacroConverter::Convert("OBJACCEPTOR_TYPEWRAPPER_DISPATCHFUNCS_DEFN", NULL, "Float", "LongNumber");
  MacroConverter::Convert("OBJACCEPTOR_TYPEWRAPPER_DISPATCHFUNCS_DEFN", NULL, "Float", "ULongInt");
  MacroConverter::Convert("OBJACCEPTOR_TYPEWRAPPER_DISPATCHFUNCS_DEFN", NULL, "Float", "UInteger");
  MacroConverter::Convert("OBJACCEPTOR_TYPEWRAPPER_DISPATCHFUNCS_DEFN", NULL, "Float", "UShortInt");
  MacroConverter::Convert("OBJACCEPTOR_TYPEWRAPPER_DISPATCHFUNCS_DEFN", NULL, "Float", "LongInt");
  MacroConverter::Convert("OBJACCEPTOR_TYPEWRAPPER_DISPATCHFUNCS_DEFN", NULL, "Float", "Integer");
  MacroConverter::Convert("OBJACCEPTOR_TYPEWRAPPER_DISPATCHFUNCS_DEFN", NULL, "Float", "ShortInt");
  MacroConverter::Convert("OBJACCEPTOR_TYPEWRAPPER_DISPATCHFUNCS_DEFN", NULL, "Float", "Double");
  MacroConverter::Convert("OBJACCEPTOR_TYPEWRAPPER_DISPATCHFUNCS_DEFN", NULL, "Float", "LongDouble");
  MacroConverter::Convert("OBJACCEPTOR_RATIONAL_DISPATCHFUNCS_DEFN", NULL, "Float", "Rational");
#endif

#if CONVERT_INTEGER
  MacroConverter::Convert("OBJACCEPTOR_HOSTCLASS_DEFN", "Integer");
  MacroConverter::Convert("OBJACCEPTOR_DISPATCHFUNCS_DEFN", NULL, "Integer", "ShortInt");
  MacroConverter::Convert("OBJACCEPTOR_DISPATCHFUNCS_DEFN", NULL, "Integer", "UShortInt");
  MacroConverter::Convert("OBJACCEPTOR_DISPATCHFUNCS_DEFN", NULL, "Integer", "ChrString");
  MacroConverter::Convert("OBJACCEPTOR_TYPEWRAPPER_DISPATCHFUNCS_DEFN", NULL, "Integer", "LongNumber");
  MacroConverter::Convert("OBJACCEPTOR_TYPEWRAPPER_DISPATCHFUNCS_DEFN", NULL, "Integer", "ULongInt");
  MacroConverter::Convert("OBJACCEPTOR_TYPEWRAPPER_DISPATCHFUNCS_DEFN", NULL, "Integer", "LongInt");
  MacroConverter::Convert("OBJACCEPTOR_TYPEWRAPPER_DISPATCHFUNCS_DEFN", NULL, "Integer", "Float");
  MacroConverter::Convert("OBJACCEPTOR_TYPEWRAPPER_DISPATCHFUNCS_DEFN", NULL, "Integer", "Double");
  MacroConverter::Convert("OBJACCEPTOR_TYPEWRAPPER_DISPATCHFUNCS_DEFN", NULL, "Integer", "LongDouble");
#endif

#if CONVERT_LDOUBLE
  MacroConverter::Convert("OBJACCEPTOR_HOSTCLASS_DEFN", "LongDouble");
  MacroConverter::Convert("OBJACCEPTOR_DISPATCHFUNCS_DEFN", NULL, "LongDouble", "ChrString");
  MacroConverter::Convert("OBJACCEPTOR_TYPEWRAPPER_DISPATCHFUNCS_DEFN", NULL, "LongDouble", "LongNumber");
  MacroConverter::Convert("OBJACCEPTOR_TYPEWRAPPER_DISPATCHFUNCS_DEFN", NULL, "LongDouble", "ULongInt");
  MacroConverter::Convert("OBJACCEPTOR_TYPEWRAPPER_DISPATCHFUNCS_DEFN", NULL, "LongDouble", "UInteger");
  MacroConverter::Convert("OBJACCEPTOR_TYPEWRAPPER_DISPATCHFUNCS_DEFN", NULL, "LongDouble", "UShortInt");
  MacroConverter::Convert("OBJACCEPTOR_TYPEWRAPPER_DISPATCHFUNCS_DEFN", NULL, "LongDouble", "LongInt");
  MacroConverter::Convert("OBJACCEPTOR_TYPEWRAPPER_DISPATCHFUNCS_DEFN", NULL, "LongDouble", "Integer");
  MacroConverter::Convert("OBJACCEPTOR_TYPEWRAPPER_DISPATCHFUNCS_DEFN", NULL, "LongDouble", "ShortInt");
  MacroConverter::Convert("OBJACCEPTOR_TYPEWRAPPER_DISPATCHFUNCS_DEFN", NULL, "LongDouble", "Float");
  MacroConverter::Convert("OBJACCEPTOR_TYPEWRAPPER_DISPATCHFUNCS_DEFN", NULL, "LongDouble", "Double");
  MacroConverter::Convert("OBJACCEPTOR_RATIONAL_DISPATCHFUNCS_DEFN", NULL, "LongDouble", "Rational");  
#endif

#if CONVERT_LONGINT
  MacroConverter::Convert("OBJACCEPTOR_HOSTCLASS_DEFN", "LongInt");
  MacroConverter::Convert("OBJACCEPTOR_DISPATCHFUNCS_DEFN", NULL, "LongInt", "ShortInt");
  MacroConverter::Convert("OBJACCEPTOR_DISPATCHFUNCS_DEFN", NULL, "LongInt", "Integer");
  MacroConverter::Convert("OBJACCEPTOR_DISPATCHFUNCS_DEFN", NULL, "LongInt", "UShortInt");
  MacroConverter::Convert("OBJACCEPTOR_DISPATCHFUNCS_DEFN", NULL, "LongInt", "UInteger");
  MacroConverter::Convert("OBJACCEPTOR_DISPATCHFUNCS_DEFN", NULL, "LongInt", "ChrString");
  MacroConverter::Convert("OBJACCEPTOR_TYPEWRAPPER_DISPATCHFUNCS_DEFN", NULL, "LongInt", "LongNumber");
  MacroConverter::Convert("OBJACCEPTOR_TYPEWRAPPER_DISPATCHFUNCS_DEFN", NULL, "LongInt", "Float");
  MacroConverter::Convert("OBJACCEPTOR_TYPEWRAPPER_DISPATCHFUNCS_DEFN", NULL, "LongInt", "Double");
  MacroConverter::Convert("OBJACCEPTOR_TYPEWRAPPER_DISPATCHFUNCS_DEFN", NULL, "LongInt", "LongDouble");
#endif

#if CONVERT_LONGNUM
  MacroConverter::Convert("OBJACCEPTOR_HOSTCLASS_DEFN", "LongNumber");
  MacroConverter::Convert("OBJACCEPTOR_DISPATCHFUNCS_DEFN", NULL, "LongNumber", "LongInt");
  MacroConverter::Convert("OBJACCEPTOR_DISPATCHFUNCS_DEFN", NULL, "LongNumber", "ShortInt");
  MacroConverter::Convert("OBJACCEPTOR_DISPATCHFUNCS_DEFN", NULL, "LongNumber", "Integer");
  MacroConverter::Convert("OBJACCEPTOR_DISPATCHFUNCS_DEFN", NULL, "LongNumber", "ULongInt");
  MacroConverter::Convert("OBJACCEPTOR_DISPATCHFUNCS_DEFN", NULL, "LongNumber", "UShortInt");
  MacroConverter::Convert("OBJACCEPTOR_DISPATCHFUNCS_DEFN", NULL, "LongNumber", "UInteger");
  MacroConverter::Convert("OBJACCEPTOR_DISPATCHFUNCS_DEFN", NULL, "LongNumber", "ChrString");
  MacroConverter::Convert("OBJACCEPTOR_DISPATCHFUNCS_DEFN", NULL, "LongNumber", "Float");
  MacroConverter::Convert("OBJACCEPTOR_DISPATCHFUNCS_DEFN", NULL, "LongNumber", "Double");
  MacroConverter::Convert("OBJACCEPTOR_DISPATCHFUNCS_DEFN", NULL, "LongNumber", "LongDouble");
#endif

#if CONVERT_SHORTINT
  MacroConverter::Convert("OBJACCEPTOR_HOSTCLASS_DEFN", "ShortInt");
  MacroConverter::Convert("OBJACCEPTOR_DISPATCHFUNCS_DEFN", NULL, "ShortInt", "ChrString");
  MacroConverter::Convert("OBJACCEPTOR_TYPEWRAPPER_DISPATCHFUNCS_DEFN", NULL, "ShortInt", "LongNumber");
  MacroConverter::Convert("OBJACCEPTOR_TYPEWRAPPER_DISPATCHFUNCS_DEFN", NULL, "ShortInt", "ULongInt");
  MacroConverter::Convert("OBJACCEPTOR_TYPEWRAPPER_DISPATCHFUNCS_DEFN", NULL, "ShortInt", "UInteger");
  MacroConverter::Convert("OBJACCEPTOR_TYPEWRAPPER_DISPATCHFUNCS_DEFN", NULL, "ShortInt", "LongInt");
  MacroConverter::Convert("OBJACCEPTOR_TYPEWRAPPER_DISPATCHFUNCS_DEFN", NULL, "ShortInt", "Integer");
  MacroConverter::Convert("OBJACCEPTOR_TYPEWRAPPER_DISPATCHFUNCS_DEFN", NULL, "ShortInt", "Float");
  MacroConverter::Convert("OBJACCEPTOR_TYPEWRAPPER_DISPATCHFUNCS_DEFN", NULL, "ShortInt", "Double");
  MacroConverter::Convert("OBJACCEPTOR_TYPEWRAPPER_DISPATCHFUNCS_DEFN", NULL, "ShortInt", "LongDouble");
#endif

#if CONVERT_TIMEOFDAY
  MacroConverter::Convert("OBJACCEPTOR_DISPATCHFUNCS_DEFN", NULL, "TimeOfDay", "ChrString");
#endif

#if CONVERT_ULONGINT
  MacroConverter::Convert("OBJACCEPTOR_CREATEFNC_METHODDEFN", NULL, "ULongInt", "ChrString");
  MacroConverter::Convert("OBJACCEPTOR_ASSIGNFNC_METHODDEFN", NULL, "ULongInt", "ChrString");
  MacroConverter::Convert("OBJACCEPTOR_CREATEFNC_METHODDEFN", NULL, "ULongInt", "LongInt");
  MacroConverter::Convert("OBJACCEPTOR_ASSIGNFNC_METHODDEFN", NULL, "ULongInt", "LongInt");
  MacroConverter::Convert("OBJACCEPTOR_CREATEFNC_METHODDEFN", NULL, "ULongInt", "ShortInt");
  MacroConverter::Convert("OBJACCEPTOR_ASSIGNFNC_METHODDEFN", NULL, "ULongInt", "ShortInt");
  MacroConverter::Convert("OBJACCEPTOR_CREATEFNC_METHODDEFN", NULL, "ULongInt", "Integer");
  MacroConverter::Convert("OBJACCEPTOR_ASSIGNFNC_METHODDEFN", NULL, "ULongInt", "Integer");
  MacroConverter::Convert("OBJACCEPTOR_CREATEFNC_METHODDEFN", NULL, "ULongInt", "ULongInt");
  MacroConverter::Convert("OBJACCEPTOR_ASSIGNFNC_METHODDEFN", NULL, "ULongInt", "ULongInt");
  MacroConverter::Convert("OBJACCEPTOR_CREATEFNC_METHODDEFN", NULL, "ULongInt", "UShortInt");
  MacroConverter::Convert("OBJACCEPTOR_ASSIGNFNC_METHODDEFN", NULL, "ULongInt", "UShortInt");
  MacroConverter::Convert("OBJACCEPTOR_CREATEFNC_METHODDEFN", NULL, "ULongInt", "UInteger");
  MacroConverter::Convert("OBJACCEPTOR_CREATEFROMTYPEWRAPPER_METHODDEFN", NULL, "ULongInt", "LongNumber");
  MacroConverter::Convert("OBJACCEPTOR_ASSIGNFROMTYPEWRAPPER_METHODDEFN", NULL, "ULongInt", "LongNumber");
  MacroConverter::Convert("OBJACCEPTOR_CREATEFROMTYPEWRAPPER_METHODDEFN", NULL, "ULongInt", "Float");
  MacroConverter::Convert("OBJACCEPTOR_ASSIGNFROMTYPEWRAPPER_METHODDEFN", NULL, "ULongInt", "Float");
  MacroConverter::Convert("OBJACCEPTOR_CREATEFROMTYPEWRAPPER_METHODDEFN", NULL, "ULongInt", "Double");
  MacroConverter::Convert("OBJACCEPTOR_ASSIGNFROMTYPEWRAPPER_METHODDEFN", NULL, "ULongInt", "Double");
  MacroConverter::Convert("OBJACCEPTOR_CREATEFROMTYPEWRAPPER_METHODDEFN", NULL, "ULongInt", "LongDouble");
  MacroConverter::Convert("OBJACCEPTOR_ASSIGNFROMTYPEWRAPPER_METHODDEFN", NULL, "ULongInt", "LongDouble");
  MacroConverter::Convert("MEMORYOPS_DEFN", NULL, "ULongIntAcceptor", NULL);
#endif

#if CONVERT_UNSIGNED
  MacroConverter::Convert("OBJACCEPTOR_CREATEFNC_METHODDEFN", NULL, "UInteger", "ChrString");
  MacroConverter::Convert("OBJACCEPTOR_ASSIGNFNC_METHODDEFN", NULL, "UInteger", "ChrString");
  MacroConverter::Convert("OBJACCEPTOR_CREATEFNC_METHODDEFN", NULL, "UInteger", "ShortInt");
  MacroConverter::Convert("OBJACCEPTOR_ASSIGNFNC_METHODDEFN", NULL, "UInteger", "ShortInt");
  MacroConverter::Convert("OBJACCEPTOR_CREATEFNC_METHODDEFN", NULL, "UInteger", "Integer");
  MacroConverter::Convert("OBJACCEPTOR_ASSIGNFNC_METHODDEFN", NULL, "UInteger", "Integer");
  MacroConverter::Convert("OBJACCEPTOR_CREATEFNC_METHODDEFN", NULL, "UInteger", "UShortInt");
  MacroConverter::Convert("OBJACCEPTOR_CREATEFNC_METHODDEFN", NULL, "UInteger", "UInteger");
  MacroConverter::Convert("OBJACCEPTOR_ASSIGNFNC_METHODDEFN", NULL, "UInteger", "UInteger");  
  MacroConverter::Convert("OBJACCEPTOR_CREATEFROMTYPEWRAPPER_METHODDEFN", NULL, "UInteger", "LongNumber");
  MacroConverter::Convert("OBJACCEPTOR_ASSIGNFROMTYPEWRAPPER_METHODDEFN", NULL, "UInteger", "LongNumber");
  MacroConverter::Convert("OBJACCEPTOR_CREATEFROMTYPEWRAPPER_METHODDEFN", NULL, "UInteger", "LongInt");
  MacroConverter::Convert("OBJACCEPTOR_ASSIGNFROMTYPEWRAPPER_METHODDEFN", NULL, "UInteger", "LongInt");
  MacroConverter::Convert("OBJACCEPTOR_CREATEFROMTYPEWRAPPER_METHODDEFN", NULL, "UInteger", "ULongInt");
  MacroConverter::Convert("OBJACCEPTOR_ASSIGNFROMTYPEWRAPPER_METHODDEFN", NULL, "UInteger", "ULongInt");
  MacroConverter::Convert("OBJACCEPTOR_CREATEFROMTYPEWRAPPER_METHODDEFN", NULL, "UInteger", "Float");
  MacroConverter::Convert("OBJACCEPTOR_ASSIGNFROMTYPEWRAPPER_METHODDEFN", NULL, "UInteger", "Float");
  MacroConverter::Convert("OBJACCEPTOR_CREATEFROMTYPEWRAPPER_METHODDEFN", NULL, "UInteger", "Double");
  MacroConverter::Convert("OBJACCEPTOR_ASSIGNFROMTYPEWRAPPER_METHODDEFN", NULL, "UInteger", "Double");
  MacroConverter::Convert("OBJACCEPTOR_CREATEFROMTYPEWRAPPER_METHODDEFN", NULL, "UInteger", "LongDouble");
  MacroConverter::Convert("OBJACCEPTOR_ASSIGNFROMTYPEWRAPPER_METHODDEFN", NULL, "UInteger", "LongDouble");
  MacroConverter::Convert("MEMORYOPS_DEFN", NULL, "UIntegerAcceptor", NULL);  
#endif

#if CONVERT_USHRTINT
  MacroConverter::Convert("OBJACCEPTOR_CREATEFNC_METHODDEFN", NULL, "UShortInt", "ChrString");
  MacroConverter::Convert("OBJACCEPTOR_ASSIGNFNC_METHODDEFN", NULL, "UShortInt", "ChrString");
  MacroConverter::Convert("OBJACCEPTOR_CREATEFNC_METHODDEFN", NULL, "UShortInt", "ShortInt");
  MacroConverter::Convert("OBJACCEPTOR_ASSIGNFNC_METHODDEFN", NULL, "UShortInt", "ShortInt");
  MacroConverter::Convert("OBJACCEPTOR_CREATEFNC_METHODDEFN", NULL, "UShortInt", "UShortInt");
  MacroConverter::Convert("OBJACCEPTOR_ASSIGNFNC_METHODDEFN", NULL, "UShortInt", "UShortInt");
  MacroConverter::Convert("OBJACCEPTOR_CREATEFROMTYPEWRAPPER_METHODDEFN", NULL, "UShortInt", "LongNumber");
  MacroConverter::Convert("OBJACCEPTOR_ASSIGNFROMTYPEWRAPPER_METHODDEFN", NULL, "UShortInt", "LongNumber");
  MacroConverter::Convert("OBJACCEPTOR_CREATEFROMTYPEWRAPPER_METHODDEFN", NULL, "UShortInt", "LongInt");
  MacroConverter::Convert("OBJACCEPTOR_ASSIGNFROMTYPEWRAPPER_METHODDEFN", NULL, "UShortInt", "LongInt");
  MacroConverter::Convert("OBJACCEPTOR_CREATEFROMTYPEWRAPPER_METHODDEFN", NULL, "UShortInt", "Integer");
  MacroConverter::Convert("OBJACCEPTOR_ASSIGNFROMTYPEWRAPPER_METHODDEFN", NULL, "UShortInt", "Integer");
  MacroConverter::Convert("OBJACCEPTOR_CREATEFROMTYPEWRAPPER_METHODDEFN", NULL, "UShortInt", "ULongInt");
  MacroConverter::Convert("OBJACCEPTOR_ASSIGNFROMTYPEWRAPPER_METHODDEFN", NULL, "UShortInt", "ULongInt");
  MacroConverter::Convert("OBJACCEPTOR_CREATEFROMTYPEWRAPPER_METHODDEFN", NULL, "UShortInt", "UInteger");
  MacroConverter::Convert("OBJACCEPTOR_ASSIGNFROMTYPEWRAPPER_METHODDEFN", NULL, "UShortInt", "UInteger");
  MacroConverter::Convert("OBJACCEPTOR_CREATEFROMTYPEWRAPPER_METHODDEFN", NULL, "UShortInt", "Float");
  MacroConverter::Convert("OBJACCEPTOR_ASSIGNFROMTYPEWRAPPER_METHODDEFN", NULL, "UShortInt", "Float");
  MacroConverter::Convert("OBJACCEPTOR_CREATEFROMTYPEWRAPPER_METHODDEFN", NULL, "UShortInt", "Double");
  MacroConverter::Convert("OBJACCEPTOR_ASSIGNFROMTYPEWRAPPER_METHODDEFN", NULL, "UShortInt", "Double");
  MacroConverter::Convert("OBJACCEPTOR_CREATEFROMTYPEWRAPPER_METHODDEFN", NULL, "UShortInt", "LongDouble");
  MacroConverter::Convert("OBJACCEPTOR_ASSIGNFROMTYPEWRAPPER_METHODDEFN", NULL, "UShortInt", "LongDouble");
  MacroConverter::Convert("MEMORYOPS_DEFN", NULL, "UShortIntAcceptor", NULL);
#endif

#if CONVERT_YEAR
  MacroConverter::Convert("OBJACCEPTOR_HOSTCLASS_DEFN", "Year");
  MacroConverter::Convert("OBJACCEPTOR_DISPATCHFUNCS_DEFN", NULL, "Year", "ChrString");
#endif

#if CONVERT_YMONTH
  MacroConverter::Convert("OBJACCEPTOR_HOSTCLASS_DEFN", "YMonth");
  MacroConverter::Convert("OBJACCEPTOR_DISPATCHFUNCS_DEFN", NULL, "YMonth", "ChrString");
#endif

  return 0;
}

/****************************************************************************/

