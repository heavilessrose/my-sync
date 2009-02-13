# From Thinking in Patterns (with Java) by Bruce Eckel
# At http://www.BruceEckel.com
# (c)2003 Bruce Eckel 
# Copyright notice in Copyright.txt
# Automatically-generated MAKEFILE 
# For examples in directory .\refactor\trash
# using the JDK 1.4 compiler
# Invoke with: make

HOME := ../../

ifndef MAKECMDGOALS
MAKECMDGOALS := javac
endif

# Command.com is too weak to build this under Windows NT/2000:
ifeq ($(OS),Windows_NT)
COMSPEC=$(SYSTEMROOT)\system32\cmd.exe
endif

ifneq ($(MAKECMDGOALS),clean)
include $(HOME)/$(MAKECMDGOALS).mac
endif

.SUFFIXES : .class .java
.java.class :
	$(JVC) $(JVCFLAGS) $<

javac: \
	Trash.class \
	Aluminum.class \
	Paper.class \
	Glass.class \
	Cardboard.class \
	ParseTrash.class \
	Fillable.class \
	FillableCollection.class

jikes: \
	Trash.class \
	Aluminum.class \
	Paper.class \
	Glass.class \
	Cardboard.class \
	ParseTrash.class \
	Fillable.class \
	FillableCollection.class

clean:
ifeq ($(notdir $(SHELL)),COMMAND.COM)
	del *.class
else
	rm -f *.class
endif


Trash.class: Trash.java 

Aluminum.class: Aluminum.java 

Paper.class: Paper.java 

Glass.class: Glass.java 

Cardboard.class: Cardboard.java 

ParseTrash.class: ParseTrash.java 

Fillable.class: Fillable.java 

FillableCollection.class: FillableCollection.java 

