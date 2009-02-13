echo off
rem make_palm.bat
rem this batch file assists in the development of the J2ME/MIDP compatible
rem components of the library.
rem To make this work, edit the lines for MIDP_ROOT and DEVEL_ROOT
rem This file will only work within the J2ME distribution

set MIDP_ROOT=c:\var\j2mewtk
set MIDP_CLASSES=%MIDP_ROOT%\lib\midpapi.zip

set DEVEL_ROOT=c:\usr\devel\jon\bc\src\crypto
set SRC_ROOT=%DEVEL_ROOT%\src
set OUTPUT_CLASSES=%DEVEL_ROOT%\classes
set PV_CLASSES=%DEVEL_ROOT%\verify
set J2ME_ROOT=%DEVEL_ROOT%\j2me

set APP_DIR= .\j2me\org\bouncycastle\crypto\examples\

rem Just to make it a bit easier to deal with things
rem and command lines and such
set PC=%MIDP_CLASSES%
set OC=%OUTPUT_CLASSES%
set SR=%SRC_ROOT%
set JM=%J2ME_ROOT%

rem Now compile the relevant classes

:COMPAT
echo compat
rem 1. The compatibility classes for the J2ME 
rem 2. The base classes
rem 3. The digests
cd %JM%
javac -g:none -d %OC% -classpath %OC%;%PC% -bootclasspath %PC% java\lang\*.java java\math\*.java java\security\*.java java\io\*.java %SR%\org\bouncycastle\crypto\*.java %SR%\org\bouncycastle\crypto\digests\*java 
cd %SR%\org\bouncycastle\util\encoders
javac -g:none -d %OC% -classpath %OC%;%PC% -bootclasspath %PC% *.java

:ECMATH
cd %SR%\org\bouncycastle\math\ec
javac -g:none -d %OC% -classpath %OC%;%PC% -bootclasspath %PC% *.java

:PARAMETERS
echo param
rem 4. The parameters
cd %SR%\org\bouncycastle\crypto\params
javac -g:none -d %OC% -classpath %OC%;%PC% -bootclasspath %PC% *.java

:ENCODINGS
echo encoding
rem 5. The encodings
cd %SR%\org\bouncycastle\crypto\encodings
javac -g:none -d %OC% -classpath %OC%;%PC% -bootclasspath %PC% *.java

: PADDINGS
echo padding
cd %SR%\org\bouncycastle\crypto\paddings
javac -g:none -d %OC% -classpath %OC%;%PC% -bootclasspath %PC% *.java

:ENGINES
echo engines
rem 6. The engines
cd %SR%\org\bouncycastle\crypto\engines
javac -g:none -d %OC% -classpath %OC%;%PC% -bootclasspath %PC% *.java

:AGREEMENT
echo agreement
rem 6.5 The DH agreement classes
cd %SR%\org\bouncycastle\crypto\agreement
javac -g:none -d %OC% -classpath %OC%;%PC% -bootclasspath %PC% *.java

:MODES
echo modes
rem 7. The Modes
cd %SR%\org\bouncycastle\crypto\modes
javac -g:none -d %OC% -classpath %OC%;%PC% -bootclasspath %PC% *.java

:MACS
echo macs
rem 8. The MACS
cd %SR%\org\bouncycastle\crypto\macs
javac -g:none -d %OC% -classpath %OC%;%PC% -bootclasspath %PC% *.java

:GENERATORS
echo generators
rem 9. The Generators
cd %SR%\org\bouncycastle\crypto\generators
javac -g:none -d %OC% -classpath %OC%;%PC% -bootclasspath %PC% *.java

:IO
echo io
rem 10. The IO classes are compiled
cd %SR%\org\bouncycastle\crypto\io
javac -g:none -d %OC% -classpath %OC%;%PC% -bootclasspath %PC% *.java


:TESTUTIL
echo test util
rem 11a. The J2ME test util
cd %SR%\org\bouncycastle\util\test
javac -g:none -d %OC% -classpath %OC%;%PC% -bootclasspath %PC% *.java

:ASN1
echo ASN1
cd %SR%\org\bouncycastle\asn1\
javac -g:none -d %OC% -classpath %OC%;%PC% -bootclasspath %PC% *.java pkcs\*.java x509\*.java x9\*.java util\DERDump.java util\ASN1Dump.java

rem test\*.java util\*.java 

:SIGNERS
echo Signers
cd %SR%\org\bouncycastle\crypto\signers
javac -g:none -d %OC% -classpath %OC%;%PC% -bootclasspath %PC% *.java 

:TEST
echo javatest
cd %SR%\org\bouncycastle\crypto\test
javac -g:none -d %OC% -classpath %OC%;%PC% -bootclasspath %PC% *.java

echo j2me test
cd %JM%\org\bouncycastle\crypto\test
javac -g:none -d %OC% -classpath %OC%;%PC% -bootclasspath %PC% *.java


:EXAMPLES
echo examples
cd %JM%\org\bouncycastle\crypto\examples
javac -deprecation -g:none -d %OC% -classpath %OC%;%PC% -bootclasspath %PC% MIDPTest.java

:VERIFY
echo verify
%MIDP_ROOT%\bin\preverify -d %PV_CLASSES% -classpath %PC% %OC%

:JAR
rem create the jar file
cd %SR%
cd ..
jar -cfm midp_test.jar %APP_DIR%\midp_test.mf -C %PV_CLASSES% .

:RUN
echo running created example
rem Test using the MIDP provided with J2ME, this is much easier than
rem installing the PRC created above into a Palm.
copy %APP_DIR%\midp_test.jad .

%MIDP_ROOT%\bin\emulator -Xdescriptor:midp_test.jad

:END
cd %SR%
cd ..
