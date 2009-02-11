@echo off
if "%JAVA_HOME%"=="" goto no_java_home

set IXSTRIM_HOME=..

set JAVA_CMD=%JAVA_HOME%\bin\java 

set CP=%IXSTRIM_HOME%/classes

set JVM_PARAMS=-Xms50M

%JAVA_CMD% %JVM_PARAMS% -classpath %CP% ixstrim.util.benchmark.Benchmark %1 %2 %3

goto end

:no_java_home
@echo Please specify JAVA_HOME 

:end