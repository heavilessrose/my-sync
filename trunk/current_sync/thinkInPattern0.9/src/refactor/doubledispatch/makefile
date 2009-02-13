# From Thinking in Patterns (with Java) by Bruce Eckel
# At http://www.BruceEckel.com
# (c)2003 Bruce Eckel 
# Copyright notice in Copyright.txt
# Automatically-generated MAKEFILE 
# For examples in directory .\refactor\doubledispatch
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
	TypedBinMember.class \
	DDAluminum.class \
	DDPaper.class \
	DDGlass.class \
	DDCardboard.class \
	TypedBin.class \
	DoubleDispatch.class

jikes: \
	TypedBinMember.class \
	DDAluminum.class \
	DDPaper.class \
	DDGlass.class \
	DDCardboard.class \
	TypedBin.class \
	DoubleDispatch.class

clean:
ifeq ($(notdir $(SHELL)),COMMAND.COM)
	del *.class
else
	rm -f *.class
endif


TypedBinMember.class: TypedBinMember.java 

DDAluminum.class: DDAluminum.java 

DDPaper.class: DDPaper.java 

DDGlass.class: DDGlass.java 

DDCardboard.class: DDCardboard.java 

TypedBin.class: TypedBin.java 

DoubleDispatch.class: DoubleDispatch.java 
	$(JVC) $(JVCFLAGS) $<
	java refactor.doubledispatch.DoubleDispatch
