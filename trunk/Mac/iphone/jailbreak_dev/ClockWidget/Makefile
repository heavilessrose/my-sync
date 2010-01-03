# Makefile for gcc compiler for iPhone (SDK Headers)

PROJECTNAME:=Clock
PRINCIPALCLASS:=$(PROJECTNAME)Widget
WIDGETFOLDER:=$(PROJECTNAME).wdgt
INSTALLFOLDER:=/var/mobile/Library/SmartScreen

CC:=arm-apple-darwin9-gcc
CPP:=arm-apple-darwin9-g++
LD:=$(CC)
SDK:=/var/toolchain/sys30

LDFLAGS = -arch arm -lobjc 
LDFLAGS += -framework CoreFoundation 
LDFLAGS += -framework Foundation 
LDFLAGS += -framework UIKit 
LDFLAGS += -framework CoreGraphics
//LDFLAGS += -framework AddressBookUI
//LDFLAGS += -framework AddressBook
//LDFLAGS += -framework QuartzCore
//LDFLAGS += -framework GraphicsServices
//LDFLAGS += -framework CoreSurface
//LDFLAGS += -framework CoreAudio
//LDFLAGS += -framework Celestial
//LDFLAGS += -framework AudioToolbox
//LDFLAGS += -framework WebCore
//LDFLAGS += -framework WebKit
//LDFLAGS += -framework Security
//LDFLAGS += -framework SystemConfiguration
//LDFLAGS += -framework CFNetwork
//LDFLAGS += -framework MediaPlayer
//LDFLAGS += -framework OpenGLES
//LDFLAGS += -framework OpenAL
LDFLAGS += -L"$(SDK)/usr/lib"
LDFLAGS += -F"$(SDK)/System/Library/Frameworks"
LDFLAGS += -F"$(SDK)/System/Library/PrivateFrameworks"
LDFLAGS += -bind_at_load
LDFLAGS += -multiply_defined suppress
LDFLAGS += -march=armv6
LDFLAGS += -mcpu=arm1176jzf-s 
LDFLAGS += -mmacosx-version-min=10.5
LDFLAGS += -dynamiclib

CFLAGS += -I"$(SDK)/usr/include" 
CFLAGS += -DDEBUG -std=gnu99 -O0
CFLAGS += -Diphoneos_version_min=3.0
CFLAGS += -Wno-attributes -Wno-trigraphs -Wreturn-type -Wunused-variable

BUILDDIR=./build/3.0
SRCDIR=./
RESDIR=./Resources
OBJS+=$(patsubst %.m,%.o,$(wildcard $(SRCDIR)/*.m))
OBJS+=$(patsubst %.c,%.o,$(wildcard $(SRCDIR)/*.c))
OBJS+=$(patsubst %.mm,%.o,$(wildcard $(SRCDIR)/*.mm))
OBJS+=$(patsubst %.cpp,%.o,$(wildcard $(SRCDIR)/*.cpp))
OBJS+=$(patsubst %.m,%.o,$(wildcard ./*.m))
PCH=$(wildcard *.pch)
RESOURCES=$(wildcard $(RESDIR)/*)

CFLAGS += $(addprefix -I,$(SRCDIR))

CPPFLAGS=$CFLAGS

all:	$(PROJECTNAME)

$(PROJECTNAME):	$(OBJS)
	$(LD) $(LDFLAGS) -o $@ $^

%.o:	%.m
	$(CC) --include $(PCH) -c $(CFLAGS) $< -o $@

%.o:	%.c
	$(CC) --include $(PCH) -c $(CFLAGS) $< -o $@

%.o:	%.mm
	$(CPP) --include $(PCH) -c $(CPPFLAGS) $< -o $@

%.o:	%.cpp
	$(CPP) --include $(PCH) -c $(CPPFLAGS) $< -o $@

dist:	$(PROJECTNAME)
	@rm -rf $(BUILDDIR)
	@mkdir -p $(BUILDDIR)/$(WIDGETFOLDER)
ifneq ($(RESOURCES),)
	cp -r $(RESOURCES) $(BUILDDIR)/$(WIDGETFOLDER)
	@rm -fr $(BUILDDIR)/$(WIDGETFOLDER)/.svn
	@rm -fr $(BUILDDIR)/$(WIDGETFOLDER)/*/.svn
endif
	@cp Info.plist $(BUILDDIR)/$(WIDGETFOLDER)/Info.plist
	@./plutil -key CFBundleExecutable -value $(PROJECTNAME) $(BUILDDIR)/$(WIDGETFOLDER)/Info.plist 
	@./plutil -key CFBundleName -value $(PROJECTNAME) $(BUILDDIR)/$(WIDGETFOLDER)/Info.plist 
	@./plutil -key NSPrincipalClass -value $(PRINCIPALCLASS) $(BUILDDIR)/$(WIDGETFOLDER)/Info.plist 
	ldid -S $(PROJECTNAME)
	@mv $(PROJECTNAME) $(BUILDDIR)/$(WIDGETFOLDER)

install: dist
	@rm -fr $(INSTALLFOLDER)/$(WIDGETFOLDER)
	cp -r $(BUILDDIR)/$(WIDGETFOLDER) $(INSTALLFOLDER)/
	@chown -R mobile:mobile $(INSTALLFOLDER)/$(WIDGETFOLDER)
	@echo "Widget $(WIDGETFOLDER) installed, please respring iPhone"
	@./respring

uninstall:
	@rm -fr $(INSTALLFOLDER)/$(WIDGETFOLDER)
	@./respring
	@echo "Widget $(WIDGETFOLDER) uninstalled, please respring iPhone"

install_respring:
	cp ./respring /usr/bin/respring

clean:
	@rm -f $(OBJS)
	@rm -rf $(BUILDDIR)
	@rm -f $(PROJECTNAME)

