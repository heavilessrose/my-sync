# Makefile for iPhone Application for Xcode gcc compiler (SDK Headers)

PROJECTNAME=NetStatus
APPFOLDER=$(PROJECTNAME).app
INSTALLFOLDER=$(PROJECTNAME).app

IPHONE_IP=192.168.10.136
#IPHONE_IP=192.168.10.135

SDKVER=2.2.1
SDK=/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS$(SDKVER).sdk

CC=/Developer/Platforms/iPhoneOS.platform/Developer/usr/bin/arm-apple-darwin9-gcc-4.0.1
CPP=/Developer/Platforms/iPhoneOS.platform/Developer/usr/bin/arm-apple-darwin9-g++-4.0.1
LD=$(CC)

LDFLAGS += -lobjc \
			-ObjC++ \
			-fobjc-exceptions \
			-multiply_defined suppress \
			-framework CoreFoundation \
			-framework Foundation \
			-framework UIKit \
			-framework CoreGraphics \
			-framework SystemConfiguration \
			-L"$(SDK)/usr/lib" \
			-F"$(SDK)/System/Library/Frameworks" \
			-F"$(SDK)/System/Library/PrivateFrameworks"
			#-framework AddressBookUI
			#-framework AddressBook
			#-framework QuartzCore
			#-framework OpenGLES
			#-m64
			#-march=armv6
			#-mcpu=arm1176jzf-s
			#-fobjc-call-cxx-cdtors

//LDFLAGS += -framework GraphicsServices
//LDFLAGS += -framework CoreSurface
//LDFLAGS += -framework CoreAudio
//LDFLAGS += -framework Celestial
//LDFLAGS += -framework AudioToolbox
//LDFLAGS += -framework WebCore
//LDFLAGS += -framework WebKit
//LDFLAGS += -framework SystemConfiguration
//LDFLAGS += -framework CFNetwork
//LDFLAGS += -framework MediaPlayer
//LDFLAGS += -framework OpenAL

CFLAGS += -I"/Developer/Platforms/iPhoneOS.platform/Developer/usr/lib/gcc/arm-apple-darwin9/4.0.1/include/" \
			-I"$(SDK)/usr/include" \
			-I"/Developer/Platforms/iPhoneOS.platform/Developer/usr/include/" \
			-I"/Developer/Platforms/iPhoneSimulator.platform/Developer/SDKs/iPhoneSimulator$(SDKVER).sdk/usr/include" \
			-F"$(SDK)/System/Library/Frameworks" \
			-F"$(SDK)/System/Library/PrivateFrameworks" \
			-I"./" \
			-DDEBUG -std=c99 \
			-Diphoneos_version_min=2.0
			#-DIPHONE_GIF_PREVIEW_TEST
			#-DTEST_STATIC_IMG_PREVIEW

CPPFLAGS=$CFLAGS

BUILDDIR=./build/$(SDKVER)
SRCDIR=./Classes
RESDIR=./Resources

OBJS=$(patsubst %.m,%.o,$(wildcard $(SRCDIR)/*.m)) \
	$(patsubst %.c,%.o,$(wildcard $(SRCDIR)/*.c)) \
	$(patsubst %.cpp,%.o,$(wildcard $(SRCDIR)/*.cpp)) \
	$(patsubst %.mm,%.o,$(wildcard $(SRCDIR)/*.mm)) \
	$(patsubst %.m,%.o,$(wildcard *.m))

PCH=$(wildcard *.pch)
RESOURCES=$(wildcard $(RESDIR)/*)
NIBS=$(patsubst %.xib,%.nib,$(wildcard *.xib))

all:	$(PROJECTNAME)

$(PROJECTNAME):	$(OBJS)
	$(LD) $(LDFLAGS) -o $@ $^ 

%.o:	%.m
	$(CC) -c $(CFLAGS) $< -o $@

%.o:	%.c
	$(CC) -c $(CFLAGS) $< -o $@

%.o:	%.cpp
	$(CPP) -c $(CPPFLAGS) $< -o $@

%.o:	%.mm
	$(CC) -c $(CFLAGS) $< -o $@

%.nib:	%.xib
	ibtool $< --compile $@

dist:	$(PROJECTNAME) $(NIBS)
	rm -rf $(BUILDDIR)
	mkdir -p $(BUILDDIR)/$(APPFOLDER)
	#cp -r $(RESOURCES) $(BUILDDIR)/$(APPFOLDER)
	cp NetStatus-Info.plist $(BUILDDIR)/$(APPFOLDER)/Info.plist
	@echo "APPL????" > $(BUILDDIR)/$(APPFOLDER)/PkgInfo
	#rm -rf $(BUILDDIR)/$(APPFOLDER)/English.lproj/.svn
	mv $(NIBS) $(BUILDDIR)/$(APPFOLDER)
	export CODESIGN_ALLOCATE=/Developer/Platforms/iPhoneOS.platform/Developer/usr/bin/codesign_allocate; ldid_intel -S $(PROJECTNAME)
	mv $(PROJECTNAME) $(BUILDDIR)/$(APPFOLDER)

install: uninstall dist

	#只上传程序不上传资源, 节省安装时间
	#scp -r $(BUILDDIR)/$(APPFOLDER)/$(PROJECTNAME) root@$(IPHONE_IP):/Applications/$(INSTALLFOLDER)/$(PROJECTNAME)
	
	#上传程序及资源
	scp -r $(BUILDDIR)/$(APPFOLDER) root@$(IPHONE_IP):/Applications/$(INSTALLFOLDER)
	
	@echo "Application $(INSTALLFOLDER) installed, please respring iPhone"
	ssh root@$(IPHONE_IP) 'killall SpringBoard'

uninstall:	
	ssh root@$(IPHONE_IP) 'rm -f /private/var/mobile/Library/Caches/com.apple.mobile.installation.plist'
	
	#只删除程序
	#ssh root@$(IPHONE_IP) 'rm -fr /Applications/$(INSTALLFOLDER)/$(PROJECTNAME)'
	
	#删除程序及资源
	ssh root@$(IPHONE_IP) 'rm -fr /Applications/$(INSTALLFOLDER)'
	
	@echo "Application $(INSTALLFOLDER) uninstalled"

install_respring:
	scp respring_arm root@$(IPHONE_IP):/usr/bin/respring

clean:
	@rm -f $(SRCDIR)/*.o *.o
	@rm -f $(SRCDIR2)/*.o *.o
	@rm -f $(SRCDIR3)/*.o *.o
	@rm -rf $(BUILDDIR)
	@rm -f $(PROJECTNAME)
	
log:
	scp root@$(IPHONE_IP):/var/mobile/appLog ~/Desktop/appLog
	ssh root@$(IPHONE_IP) 'rm -fr /var/mobile/appLog'
	mate ~/Desktop/appLog

cleanlog:
	ssh root@$(IPHONE_IP) 'rm -fr /var/mobile/appLog'

