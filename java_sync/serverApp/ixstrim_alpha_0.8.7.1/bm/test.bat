@echo off

set host=localhost
set port=80

set maxtime=1
set url=http://%host%:%port%/%1
call _prepare.bat 

cls

set maxtime=10
set url=http://%host%:%port%/%1
call _test.bat %1

