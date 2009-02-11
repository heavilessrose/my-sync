@echo off
if "%JAVA_HOME%"=="" goto no_java_home

set IXSTRIM_HOME=%~dp0..

set JAVA_CMD=%JAVA_HOME%\bin\java 

set JVM_PARAMS=-Xms64M -Xmx150M

set CP=%IXSTRIM_HOME%/classes
set CP=%IXSTRIM_HOME%/lib/servlet.jar;%CP%

%JAVA_CMD% %JVM_PARAMS% -classpath %CP% ixstrim.examples.servlet.HttpServer %1

goto end

:no_java_home
@echo The JAVA_HOME environment variable is not defined
@echo This environment variable is needed to run this program

:end