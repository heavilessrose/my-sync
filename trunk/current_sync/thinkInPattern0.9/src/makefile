# Master makefile for Thinking in Patterns by Bruce Eckel
# at http://www.BruceEckel.com
# (c)2003 Bruce Eckel 
# Copyright notice in Copyright.txt
# Compiles all the code in the book
# Using the JDK 1.4 compiler

help: 
	@echo To compile all programs from 
	@echo Thinking in Patterns \(with Java\), type
	@echo one of the following commands,
	@echo according to your platform:
	@echo make javac
	@echo make jikes
	@echo make clean
	@echo Make sure that this directory and '.' are
	@echo in your CLASSPATH before compiling

$(MAKECMDGOALS): 
	$(MAKE) $(MAKECMDGOALS) -C com/bruceeckel/tools
	$(MAKE) $(MAKECMDGOALS) -C simplifying
	$(MAKE) $(MAKECMDGOALS) -C builder
	$(MAKE) $(MAKECMDGOALS) -C templatemethod
	$(MAKE) $(MAKECMDGOALS) -C com/bruceeckel/util
	$(MAKE) $(MAKECMDGOALS) -C singleton
	$(MAKE) $(MAKECMDGOALS) -C proxy
	$(MAKE) $(MAKECMDGOALS) -C state
	$(MAKE) $(MAKECMDGOALS) -C statemachine
	$(MAKE) $(MAKECMDGOALS) -C statemachine/mouse
	$(MAKE) $(MAKECMDGOALS) -C statemachine/mousetrap1
	$(MAKE) $(MAKECMDGOALS) -C statemachine/mousetrap2
	$(MAKE) $(MAKECMDGOALS) -C statemachine2
	$(MAKE) $(MAKECMDGOALS) -C statemachine/vendingmachine
	$(MAKE) $(MAKECMDGOALS) -C decorator/nodecorators
	$(MAKE) $(MAKECMDGOALS) -C decorator/alldecorators
	$(MAKE) $(MAKECMDGOALS) -C decorator/compromise
	$(MAKE) $(MAKECMDGOALS) -C factory
	$(MAKE) $(MAKECMDGOALS) -C factory/shapefact1
	$(MAKE) $(MAKECMDGOALS) -C factory/shapefact2
	$(MAKE) $(MAKECMDGOALS) -C command
	$(MAKE) $(MAKECMDGOALS) -C composite
	$(MAKE) $(MAKECMDGOALS) -C strategy
	$(MAKE) $(MAKECMDGOALS) -C chainofresponsibility
	$(MAKE) $(MAKECMDGOALS) -C adapter
	$(MAKE) $(MAKECMDGOALS) -C facade
	$(MAKE) $(MAKECMDGOALS) -C bridge
	$(MAKE) $(MAKECMDGOALS) -C observer
	$(MAKE) $(MAKECMDGOALS) -C multipledispatch
	$(MAKE) $(MAKECMDGOALS) -C visitor
	$(MAKE) $(MAKECMDGOALS) -C refactor/trash
	$(MAKE) $(MAKECMDGOALS) -C refactor/recyclea
	$(MAKE) $(MAKECMDGOALS) -C refactor/recycleap
	$(MAKE) $(MAKECMDGOALS) -C refactor/recycleb
	$(MAKE) $(MAKECMDGOALS) -C refactor/doubledispatch
	$(MAKE) $(MAKECMDGOALS) -C refactor/trashvisitor
	$(MAKE) $(MAKECMDGOALS) -C refactor/dynatrash
	$(MAKE) $(MAKECMDGOALS) -C projects
	$(MAKE) $(MAKECMDGOALS) -C flyweight
