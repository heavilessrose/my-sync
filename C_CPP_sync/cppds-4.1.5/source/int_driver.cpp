#ifndef QUICKINT_H
  #include "quickint.h"
#endif

#define TEST_QUICKPARSER                1
#define TEST_QUICKPARSER_1              0
#define TEST_QUICKPARSER_2              0
#define TEST_QUICKPARSER_3              0
#define TEST_QUICKPARSER_4              1
#define TEST_QUICKPARSER_4a             0
#define TEST_QUICKPARSER_4b             1

void ShowResultCodeString(Parser* p, char* Text_, ParseResultInfo* Results_, QuickParseStatus* Status_)
{
  int i;
  char CodeStr_[4096];
  
  CodeStr_[0] = 0;
  cout <<"################################################################################################################" <<endl;
  cout <<"Parsing Line: " <<Text_ <<endl;
  cout <<"Remaining Line: " <<Status_->NewInputStr <<endl;
  cout <<"Deepest Level: " <<Status_->DeepestLevel <<endl;
  
  for (i = 0; i <= Status_->DeepestLevel; i++)
  {
    cout <<p->GetResultCodeString(Results_, CodeStr_, 0, i) <<endl;
    CodeStr_[0] = 0;
  }
}

int main(int argc, char* argv[])
{
  Parser* pptr = Parser::MakeParserPtr();

#if TEST_QUICKPARSER
  char Buffer_[512];
  char Syntax_[1024];
  ParseResultInfo* Results_ = new ParseResultInfo();
  ParseResultInfo* Results2_ = new ParseResultInfo();
  QuickParseStatus* Status_ = new QuickParseStatus();
  QuickParseStatus* Status2_ = new QuickParseStatus();

  pptr->UseBrackets(TRUE);
  pptr->ReadDefinitionFile("mysqldefn.txt");

//<plain text> ::= "TEXT"[<whitespace>"FORMAT"<assignment><quoted string>]
//                       [<whitespace>"ANCHOR"[(<assignment><quoted string> |
//                                              <whitespace><unquoted string>"("<quoted string>")" |
//                                              <whitespace><unquoted string>"(*)")]]

#if TEST_QUICKPARSER_1
  strcpy(Syntax_, "\"{TEXT\"[((<whitespace>\"FORMAT\"<assignment><quoted string>");
  strcat(Syntax_, "[<whitespace>\"ANCHOR\"[(<assignment><quoted string> |");
  strcat(Syntax_, " <whitespace><unquoted string>\"(\"<quoted string>\")\" |");
  strcat(Syntax_, " <whitespace><unquoted string>\"(*)\")]]) | ");
  strcat(Syntax_, "(<whitespace>\"ANCHOR\"[(<assignment><quoted string> |");
  strcat(Syntax_, " <whitespace><unquoted string>\"(\"<quoted string>\")\" |");
  strcat(Syntax_, " <whitespace><unquoted string>\"(*)\")]");
  strcat(Syntax_, "[<whitespace>\"FORMAT\"<assignment><quoted string>]))]");
  strcat(Syntax_, "\"}\"(<terminator> | <semi-terminator>)");  
  
  strcpy(Buffer_, "{TEXT FORMAT=[P,B] ANCHOR=[the_anchor]}: visitnum INTEGER(10) UNSIGNED NOT NULL DEFAULT 0");
  pptr->QuickParse_Main(Results_, Status_, "<plain text>", Buffer_, Syntax_);
  ShowResultCodeString(pptr, Buffer_, Results_, Status_);
  pptr->ShowResults(Results_, 0);  
  cout <<"/**************************************************************************************************************/"
       <<endl;
  strcpy(Buffer_, "{TEXT ANCHOR FORMAT=[P,B]}; URL hyperlinks([stufflink]): small_value SMALLINT NOT NULL DEFAULT 0");
  Status_->Reset();
  Results_->Reset();
  pptr->QuickParse_Main(Results_, Status_, "<plain text>", Buffer_, Syntax_);
  ShowResultCodeString(pptr, Buffer_, Results_, Status_);
  pptr->ShowResults(Results_, 0);    
  cout <<"/**************************************************************************************************************/"
       <<endl;
  strcpy(Buffer_, "{TEXT};");
  Status_->Reset();
  Results_->Reset();  
  pptr->QuickParse_Main(Results_, Status_, "<plain text>", Buffer_, Syntax_);
  ShowResultCodeString(pptr, Buffer_, Results_, Status_);
  pptr->ShowResults(Results_, 0);      
  cout <<"/**************************************************************************************************************/"
       <<endl;
  strcpy(Buffer_, "{TEXT ANCHOR anchorlinks(*)}; URL=[http://www.mystuff.com/otherstuff.txt]: tstamp_value TIMESTAMP");
  Status_->Reset();
  Results_->Reset();    
  pptr->QuickParse_Main(Results_, Status_, "<plain text>", Buffer_, Syntax_);
  ShowResultCodeString(pptr, Buffer_, Results_, Status_);
  pptr->ShowResults(Results_, 0);      
  cout <<"/**************************************************************************************************************/"
       <<endl;              
  strcpy(Buffer_, "{TEXT ANCHOR anchorlinks([my_anchor])}; URL hyperlinks([stufflink2]): dt_value DATETIME");
  Status_->Reset();
  Results_->Reset();
  pptr->QuickParse_Main(Results_, Status_, "<plain text>", Buffer_, Syntax_);
  ShowResultCodeString(pptr, Buffer_, Results_, Status_);
  pptr->ShowResults(Results_, 0);
  cout <<"/**************************************************************************************************************/"
       <<endl;
  strcpy(Buffer_, "{INPUT IMAGE SRC pics(*)};");
  Status_->Reset();
  Results_->Reset();    
  pptr->QuickParse_Main(Results_, Status_, "<plain text>", Buffer_, Syntax_);
  ShowResultCodeString(pptr, Buffer_, Results_, Status_);
  pptr->ShowResults(Results_, 0);  
  cout <<"/**************************************************************************************************************/" <<endl;
  cout <<"/**************************************************************************************************************/" <<endl;

  delete Results_;
  delete Status_;
#endif

//<image> ::= "IMAGE"[<whitespace>"SRC"(<assignment><quoted string> |
//                                      <whitespace><unquoted string>"("<quoted string>")" |
//                                      <whitespace><unquoted string>"(*)")]
//                   [<whitespace>"ALT"(<assignment><quoted string> |
//                                      <whitespace><unquoted string>"("<quoted string>")" |
//                                      <whitespace><unquoted string>"(*)")]

#if TEST_QUICKPARSER_2
  strcpy(Syntax_, "\"{INPUT IMAGE\"[((<whitespace>\"SRC\"(<assignment><quoted string> | ");
  strcat(Syntax_, "<whitespace><unquoted string>\"(\"<quoted string>\")\" | ");
  strcat(Syntax_, "<whitespace><unquoted string>\"(*)\")");
  strcat(Syntax_, "[<whitespace>\"ALT\"(<assignment><quoted string> | ");
  strcat(Syntax_, "<whitespace><unquoted string>\"(\"<quoted string>\")\" | ");
  strcat(Syntax_, "<whitespace><unquoted string>\"(*)\")]) | ");
  strcat(Syntax_, "(<whitespace>\"ALT\"(<assignment><quoted string> | ");
  strcat(Syntax_, "<whitespace><unquoted string>\"(\"<quoted string>\")\" | ");
  strcat(Syntax_, "<whitespace><unquoted string>\"(*)\")");
  strcat(Syntax_, "[<whitespace>\"SRC\"(<assignment><quoted string> | ");
  strcat(Syntax_, "<whitespace><unquoted string>\"(\"<quoted string>\")\" | ");
  strcat(Syntax_, "<whitespace><unquoted string>\"(*)\")]))]");
  strcat(Syntax_, "\"}\"(<terminator> | <semi-terminator>)");

  strcpy(Buffer_, "{INPUT IMAGE SRC=[image.gif] ALT=[an image]}; URL hyperlinks([stufflink3]):");
  Status2_->Reset();
  Results2_->Reset();    
  pptr->QuickParse_Main(Results2_, "<image>", Buffer_, Syntax_);
  ShowResultCodeString(pptr, Buffer_, Results2_, Status2_);
  pptr->ShowResults(Results2_, 0);    
  cout <<"/**************************************************************************************************************/"
       <<endl;
  strcpy(Buffer_, "{INPUT IMAGE}; URL hyperlinks([stufflink4]):");
  Status2_->Reset();
  Results2_->Reset();    
  pptr->QuickParse_Main(Results2_, "<image>", Buffer_, Syntax_);
  ShowResultCodeString(pptr, Buffer_, Results2_, Status2_);
  pptr->ShowResults(Results2_, 0);      
  cout <<"/**************************************************************************************************************/"
       <<endl;
  strcpy(Buffer_, "{INPUT IMAGE SRC xpics([graphics_image2]) ALT=[another image]}; URL:");
  Status2_->Reset();
  Results2_->Reset();    
  pptr->QuickParse_Main(Results2_, "<image>", Buffer_, Syntax_);
  ShowResultCodeString(pptr, Buffer_, Results2_, Status2_);
  pptr->ShowResults(Results2_, 0);      
  cout <<"/**************************************************************************************************************/"
       <<endl;
  strcpy(Buffer_, "{INPUT IMAGE SRC pics([graphics_image]) ALT descs([image_desc])}; URL hyperlinks([stufflink9]):");
  Status2_->Reset();
  Results2_->Reset();    
  pptr->QuickParse_Main(Results2_, "<image>", Buffer_, Syntax_);
  ShowResultCodeString(pptr, Buffer_, Results2_, Status2_);
  pptr->ShowResults(Results2_, 0);      
  cout <<"/**************************************************************************************************************/"
       <<endl;
  strcpy(Buffer_, "{INPUT IMAGE SRC pics(*)};");
  Status2_->Reset();
  Results2_->Reset();    
  pptr->QuickParse_Main(Results2_, "<image>", Buffer_, Syntax_);
  ShowResultCodeString(pptr, Buffer_, Results2_, Status2_);
  pptr->ShowResults(Results2_, 0);      
  cout <<"/**************************************************************************************************************/"
       <<endl;
  strcpy(Buffer_, "{TEXT ANCHOR anchorlinks([my_anchor])};");
  Status2_->Reset();
  Results2_->Reset();    
  pptr->QuickParse_Main(Results2_, "<image>", Buffer_, Syntax_);
  ShowResultCodeString(pptr, Buffer_, Results2_, Status2_);
  pptr->ShowResults(Results2_, 0);      
  cout <<"/**************************************************************************************************************/" <<endl;
  cout <<"/**************************************************************************************************************/" <<endl;

  delete Results2_;
  delete Status2_;
#endif

//<button> ::= ("{INPUT BUTTON"([<whitespace>"TEXT"[<whitespace>"FORMAT"<assignment><quoted string>]] |
//                              [<whitespace>"IMAGE"[<whitespace>"SRC"(<assignment><quoted string> |
//                                                                     <whitespace><unquoted string>"("<quoted string>")" |
//                                                                     <whitespace><unquoted string>"(*)")]])
//               "}"(<terminator> | <semi-terminator>))

#if TEST_QUICKPARSER_3
  strcpy(Syntax_, "\"{INPUT BUTTON\"([<whitespace>\"TEXT\"[<whitespace>\"FORMAT\"<assignment><quoted string>]] | ");
  strcat(Syntax_, "[<whitespace>\"IMAGE\"[<whitespace>\"SRC\"(<assignment><quoted string> | ");  
  strcat(Syntax_, "<whitespace><unquoted string>\"(\"<quoted string>\")\" | ");  
  strcat(Syntax_, "<whitespace><unquoted string>\"(*)\")]])");
  strcat(Syntax_, "\"}\"(<terminator> | <semi-terminator>)");
  
  strcpy(Buffer_, "{INPUT BUTTON TEXT}: visitnum INTEGER(10) UNSIGNED NOT NULL DEFAULT 0");
  Status_->Reset();
  Results_->Reset();  
  pptr->QuickParse_Main(Results_, Status_, "<button>", Buffer_, Syntax_);
  ShowResultCodeString(pptr, Buffer_, Results_, Status_);
  pptr->ShowResults(Results_, 0);  
  cout <<"/**************************************************************************************************************/"
       <<endl;
  strcpy(Buffer_, "{INPUT BUTTON TEXT FORMAT=[P,B]}; URL hyperlinks([stufflink]): small_value SMALLINT NOT NULL DEFAULT 0");
  Status_->Reset();
  Results_->Reset();
  pptr->QuickParse_Main(Results_, Status_, "<button>", Buffer_, Syntax_);
  ShowResultCodeString(pptr, Buffer_, Results_, Status_);
  pptr->ShowResults(Results_, 0);    
  cout <<"/**************************************************************************************************************/"
       <<endl;
  strcpy(Buffer_, "{INPUT BUTTON};");
  Status_->Reset();
  Results_->Reset();  
  pptr->QuickParse_Main(Results_, Status_, "<button>", Buffer_, Syntax_);
  ShowResultCodeString(pptr, Buffer_, Results_, Status_);
  pptr->ShowResults(Results_, 0);      
  cout <<"/**************************************************************************************************************/"
       <<endl;
  strcpy(Buffer_, "{INPUT BUTTON IMAGE}; URL=[http://www.mystuff.com/otherstuff.txt]: tstamp_value TIMESTAMP");
  Status_->Reset();
  Results_->Reset();    
  pptr->QuickParse_Main(Results_, Status_, "<button>", Buffer_, Syntax_);
  ShowResultCodeString(pptr, Buffer_, Results_, Status_);
  pptr->ShowResults(Results_, 0);      
  cout <<"/**************************************************************************************************************/"
       <<endl;              
  strcpy(Buffer_, "{INPUT BUTTON IMAGE SRC=[image.gif]}; URL hyperlinks([stufflink2]): dt_value DATETIME");
  Status_->Reset();
  Results_->Reset();
  pptr->QuickParse_Main(Results_, Status_, "<button>", Buffer_, Syntax_);
  ShowResultCodeString(pptr, Buffer_, Results_, Status_);
  pptr->ShowResults(Results_, 0);
  cout <<"/**************************************************************************************************************/"
       <<endl;
  strcpy(Buffer_, "{INPUT BUTTON IMAGE SRC xpics([graphics_image2])};");
  Status_->Reset();
  Results_->Reset();    
  pptr->QuickParse_Main(Results_, Status_, "<button>", Buffer_, Syntax_);
  ShowResultCodeString(pptr, Buffer_, Results_, Status_);
  pptr->ShowResults(Results_, 0);  
  cout <<"/**************************************************************************************************************/"
       <<endl;
  strcpy(Buffer_, "{INPUT BUTTON IMAGE SRC pics(*)};");
  Status_->Reset();
  Results_->Reset();    
  pptr->QuickParse_Main(Results_, Status_, "<button>", Buffer_, Syntax_);
  ShowResultCodeString(pptr, Buffer_, Results_, Status_);
  pptr->ShowResults(Results_, 0);  
  cout <<"/**************************************************************************************************************/" <<endl;  
  cout <<"/**************************************************************************************************************/" <<endl;

  delete Results_;
  delete Status_;  
#endif

// ***** mysql column type definitions *****
// <sql string> ::= (<double quoted string> | <single quoted string>)
// <float> ::= (<integer> | <integer>"."<integer> | "."<integer>)
// <default value float> ::= "DEFAULT"<whitespace><float>
// <default value integer> ::= "DEFAULT"<whitespace><integer>
// <defalut value string> ::= "DEFAULT"<whitespace><sql string>
// <null status> ::= ("NOT NULL" | "NULL")
// <int length> ::= [<whitespace>]"("[<whitespace>]<integer>[<whitespace>]")"
//                  ["UNSIGNED"]["ZEROFILL"]
// <float length> ::= [<whitespace>]"("[<whitespace>]<integer>[<whitespace>]","[<whitespace>]<integer>[<whitespace>]")"
//                    ["UNSIGNED"]["ZEROFILL"]
// <string length> ::= [<whitespace>]"("[<whitespace>]<integer>[<whitespace>]")"["BINARY"]
// <mysql field definition> ::= (<integer field type> | <float field type> | <string field type> |
//                               <date field type> | <time field type> | <blob field type> | <text field type>)
//                              ["AUTO_INCREMENT"]["PRIMARY KEY"]
// <integer field type> ::= ("TINYINT"[<int length>][<whitespace><null status>][<whitespace><default value integer>] |
//                           "SMALLINT"[<int length>][<whitespace><null status>][<whitespace><default value integer>] |
//                           "MEDIUMINT"[<int length>][<whitespace><null status>][<whitespace><default value integer>] |
//                           "INT"[<int length>][<whitespace><null status>][<whitespace><default value integer>] |
//                           "INTEGER"[<int length>][<whitespace><null status>][<whitespace><default value integer>] |
//                           "BIGINT"[<int length>][<whitespace><null status>][<whitespace><default value integer>])
// <float field type> ::= ("REAL"[<float length>][<whitespace><null status>][<whitespace><default value float>] |
//                         "DOUBLE"[<float length>][<whitespace><null status>][<whitespace><default value float>] |
//                         "FLOAT"[<float length>][<whitespace><null status>][<whitespace><default value float>])
// <string field type> ::= ("VARCHAR"<string length>[<whitespace><null status>][<whitespace><default value string>] |
//                          "CHAR"<string length>[<whitespace><null status>][<whitespace><default value string>])
// <date field type> ::= ("DATE"[<whitespace><null status>][<whitespace><default value string>] |
//                        "DATETIME"[<whitespace><null status>][<whitespace><default value string>])
// <time field type> ::= ("TIME"[<whitespace><null status>][<whitespace><default value string>] |
//                        "TIMESTAMP"[<whitespace><null status>][<whitespace><default value string>])
// <blob field type> ::= ("TINYBLOB"[<whitespace><null status>][<whitespace><default value string>] |
//                        "BLOB"[<whitespace><null status>][<whitespace><default value string>] |
//                        "MEDIUMBLOB"[<whitespace><null status>][<whitespace><default value string>] |
//                        "LONGBLOB"[<whitespace><null status>][<whitespace><default value string>])
// <text field type> ::= ("TINYTEXT"[<whitespace><null status>][<whitespace><default value string>] |
//                        "TEXT"[<whitespace><null status>][<whitespace><default value string>] |
//                        "MEDIUMTEXT"[<whitespace><null status>][<whitespace><default value string>] |
//                        "LONGTEXT"[<whitespace><null status>][<whitespace><default value string>])
//
// ***** table indexes definitions *****
// <index column names> ::= [<whitespace>]<unquoted string>[<whitespace>][\",\"<index column names>]
// <primary key definition> ::= [<whitespace>]\"PRIMARY\"<whitespace>\"KEY\"[<whitespace>]\"(\"<index column names>\")\"
// <index key definition> ::= [<whitespace>](\"INDEX\" | \"KEY\")[<whitespace><unquoted string>][<whitespace>]\"(\"<index column names>\")\"
// <unique key definition> ::= [<whitespace>]\"UNIQUE\"[<whitespace>\"INDEX\"][<whitespace><unquoted string>][<whitespace>]\"(\"<index column names>\")\"
//

#if TEST_QUICKPARSER_4
#if TEST_QUICKPARSER_4a
//  system("clear");
  Parser* pptr1 = Parser::MakeParserPtr();
  Parser* pptr2 = Parser::MakeParserPtr();
  Parser* pptr3 = Parser::MakeParserPtr();
  Parser* pptr4 = Parser::MakeParserPtr();
  Parser* pptr5 = Parser::MakeParserPtr();
  Parser* pptr6 = Parser::MakeParserPtr();
  Parser* pptr7 = Parser::MakeParserPtr();

  pptr1->CopyParseSymbols(*pptr);
  pptr2->CopyParseSymbols(*pptr);
  pptr3->CopyParseSymbols(*pptr);
  pptr4->CopyParseSymbols(*pptr);
  pptr5->CopyParseSymbols(*pptr);
  pptr6->CopyParseSymbols(*pptr);
  pptr7->CopyParseSymbols(*pptr);

  strcpy(Buffer_, " visitnum INTEGER(10) UNSIGNED ZEROFILL NOT NULL DEFAULT 0 AUTO_INCREMENT PRIMARY KEY:");
  Status_->Reset();
  Results_->Reset();  
  pptr1->QuickParse_Main(Results_, Status_, "<mysql column definition>", Buffer_, NULL);
  ShowResultCodeString(pptr, Buffer_, Results_, Status_);
  pptr1->ShowResults(Results_, 0);
  Parser::DestroyParser(0, pptr1);  
  cout <<"/**************************************************************************************************************/"
       <<endl;
       
  strcpy(Buffer_, "small_value SMALLINT NOT NULL DEFAULT 0:");
  Status_->Reset();
  Results_->Reset();
  pptr2->QuickParse_Main(Results_, Status_, "<mysql column definition>", Buffer_, NULL);
  ShowResultCodeString(pptr, Buffer_, Results_, Status_);
  pptr2->ShowResults(Results_, 0);
  Parser::DestroyParser(0, pptr2);
  cout <<"/**************************************************************************************************************/"
       <<endl;
       
  strcpy(Buffer_, "float_value DOUBLE(10, 2) ZEROFILL NULL DEFAULT 0 PRIMARY KEY:");
  Status_->Reset();
  Results_->Reset();  
  pptr3->QuickParse_Main(Results_, Status_, "<mysql column definition>", Buffer_, NULL);
  ShowResultCodeString(pptr, Buffer_, Results_, Status_);
  pptr3->ShowResults(Results_, 0);
  cout <<"/**************************************************************************************************************/"
       <<endl;
       
  strcpy(Buffer_, " tstamp_value TIMESTAMP:");
  Status_->Reset();
  Results_->Reset();    
  pptr4->QuickParse_Main(Results_, Status_, "<mysql column definition>", Buffer_, NULL);
  ShowResultCodeString(pptr, Buffer_, Results_, Status_);
  pptr4->ShowResults(Results_, 0);
  cout <<"/**************************************************************************************************************/"
       <<endl;
       
  strcpy(Buffer_, " dt_value DATETIME DEFAULT \"1999-01-01 20:20:20\":");
  Status_->Reset();
  Results_->Reset();
  pptr5->QuickParse_Main(Results_, Status_, "<mysql column definition>", Buffer_, NULL);
  ShowResultCodeString(pptr, Buffer_, Results_, Status_);
  pptr5->ShowResults(Results_, 0);
  Parser::DestroyParser(0, pptr5);
  cout <<"/**************************************************************************************************************/"
       <<endl;
       
  strcpy(Buffer_, " vchar1 VARCHAR(10) NOT NULL DEFAULT \"sample\":");
  Status_->Reset();
  Results_->Reset();    
  pptr6->QuickParse_Main(Results_, Status_, "<mysql column definition>", Buffer_, NULL);
  ShowResultCodeString(pptr, Buffer_, Results_, Status_);
  pptr6->ShowResults(Results_, 0);
  cout <<"/**************************************************************************************************************/"
       <<endl;
       
  strcpy(Buffer_, " charval CHAR(10) BINARY DEFAULT \"\":");
  Status_->Reset();
  Results_->Reset();    
  pptr7->QuickParse_Main(Results_, Status_, "<mysql column definition>", Buffer_, NULL);
  ShowResultCodeString(pptr, Buffer_, Results_, Status_);
  pptr7->ShowResults(Results_, 0);
  cout <<"/**************************************************************************************************************/" <<endl;  
  cout <<"/**************************************************************************************************************/" <<endl;

  delete Results_;
  delete Status_;
#endif

#if TEST_QUICKPARSER_4b
//  system("clear");  
  Parser* pptr4b = Parser::MakeParserPtr();
  pptr4b->CopyParseSymbols(*pptr);

  strcpy(Buffer_, "PRIMARY KEY(visitnum):");
  Status_->Reset();
  Results_->Reset();
  pptr4b->QuickParse_Main(Results_, Status_, "<primary key definition>", Buffer_, NULL);
  ShowResultCodeString(pptr4b, Buffer_, Results_, Status_);
  pptr4b->ShowResults(Results_, 0);
  cout <<"/**************************************************************************************************************/"
       <<endl;

  strcpy(Buffer_, "PRIMARY KEY(reftable, reffield, fieldname):");
  Status_->Reset();
  Results_->Reset();
  pptr4b->QuickParse_Main(Results_, Status_, "<primary key definition>", Buffer_, NULL);
  ShowResultCodeString(pptr4b, Buffer_, Results_, Status_);
  pptr4b->ShowResults(Results_, 0);
  cout <<"/**************************************************************************************************************/"
       <<endl;

  strcpy(Buffer_, "INDEX (visitnum):");
  Status_->Reset();
  Results_->Reset();
  pptr4b->QuickParse_Main(Results_, Status_, "<index key definition>", Buffer_, NULL);
  ShowResultCodeString(pptr4b, Buffer_, Results_, Status_);
  pptr4b->ShowResults(Results_, 0);
  cout <<"/**************************************************************************************************************/"
       <<endl;

  strcpy(Buffer_, "KEY keyindex (reftable, reffield, fieldname):");
  Status_->Reset();
  Results_->Reset();
  pptr4b->QuickParse_Main(Results_, Status_, "<index key definition>", Buffer_, NULL);
  ShowResultCodeString(pptr4b, Buffer_, Results_, Status_);
  pptr4b->ShowResults(Results_, 0);
  cout <<"/**************************************************************************************************************/"
       <<endl;

  strcpy(Buffer_, "UNIQUE (visitnum, ordernum):");
  Status_->Reset();
  Results_->Reset();
  pptr4b->QuickParse_Main(Results_, Status_, "<unique key definition>", Buffer_, NULL);
  ShowResultCodeString(pptr4b, Buffer_, Results_, Status_);
  pptr4b->ShowResults(Results_, 0);
  cout <<"/**************************************************************************************************************/"
       <<endl;

  strcpy(Buffer_, "UNIQUE INDEX (ordernum):");
  Status_->Reset();
  Results_->Reset();
  pptr4b->QuickParse_Main(Results_, Status_, "<unique key definition>", Buffer_, NULL);
  ShowResultCodeString(pptr4b, Buffer_, Results_, Status_);
  pptr4b->ShowResults(Results_, 0);
  cout <<"/**************************************************************************************************************/"
       <<endl;

  strcpy(Buffer_, "UNIQUE INDEX fldindex(reftable, reffield, fieldname):");
  Status_->Reset();
  Results_->Reset();
  pptr4b->QuickParse_Main(Results_, Status_, "<unique key definition>", Buffer_, NULL);
  ShowResultCodeString(pptr4b, Buffer_, Results_, Status_);
  pptr4b->ShowResults(Results_, 0);                              
  cout <<"/**************************************************************************************************************/" <<endl;
  cout <<"/**************************************************************************************************************/" <<endl;       

  delete Results_;
  delete Status_;
#endif
#endif

#endif

  Parser::DestroyParser(0, pptr);
  return 0;
}





