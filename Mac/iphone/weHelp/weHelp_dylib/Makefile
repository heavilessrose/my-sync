Compiler=/Developer/Platforms/iPhoneOS.platform/Developer/usr/bin/arm-apple-darwin9-gcc-4.0.1
CXX	:= /Developer/Platforms/iPhoneOS.platform/Developer/usr/bin/arm-apple-darwin9-g++-4.0.1
#IP=root@192.168.10.135
IP=root@192.168.10.184
SDKVER=2.2.1
Sysroot=/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS$(SDKVER).sdk

LDFLAGS=	-lobjc \
				-framework Foundation \
				-framework UIKit \
				-framework CoreFoundation \
				-framework QuartzCore \
				-framework OpenGLES \
				-framework CoreGraphics \
				-framework CoreTelephony \
				-multiply_defined suppress \
				-L$(Sysroot)/usr/lib \
				-F$(Sysroot)/System/Library/Frameworks \
				-F$(Sysroot)/System/Library/PrivateFrameworks \
				-dynamiclib \
				-init _WinksCallInitializeHook \
				-Wall \
				-Werror \
				-lsubstrate \
				-lobjc \
				-ObjC++ \
				-fobjc-exceptions \
				-fobjc-call-cxx-cdtors
				#-march=armv6
				#-mcpu=arm1176jzf-s

CFLAGS= -dynamiclib \
		-I../glgif \
		-I../engine \
		-I./ \
		-std=c99 \
		-DSHOW_WINKS_LOG \
		-I/Developer/SDKs/iPhoneOS.sdk/Versions/iPhoneOS2.0.wu.sdk/include \
		-I/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS$(SDKVER).sdk/usr/include \
		-isysroot /Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS$(SDKVER).sdk
		
Objects= WinksCallLibrary.o PlayerView.o MyLog.o GreetView.o glgif/GifVideo.o glgif/Video.o glgif/VideoSource.o \
		glgif/VideoTexture.o glgif/dgif_lib.o glgif/egif_lib.o glgif/gif_err.o glgif/gifalloc.o \
		engine/wk_engin.o engine/wk_socketengin.o 
		
Sources= WinksCallLibrary.mm PlayerView.mm MyLog.m GreetView.m glgif/GifVideo.m glgif/Video.m glgif/VideoSource.m \
		glgif/VideoTexture.m glgif/dgif_lib.c glgif/egif_lib.c glgif/gif_err.c glgif/gifalloc.c \
		engine/wk_engin.m engine/wk_socketengin.m 


Target=WeHelpHook.dylib


compile:	$(Target) 

all: $(Target) install

install:
		scp $(Target) $(IP):/var/root
		ssh $(IP) chmod 755 $(Target) 
		ssh $(IP) ldid -S $(Target)
		ssh $(IP) rm -f /Library/MobileSubstrate/DynamicLibraries/*
		#ssh $(IP) rm -f /var/mobile/callLog
		ssh $(IP) mv $(Target) /Library/MobileSubstrate/DynamicLibraries
		ssh $(IP) killall SpringBoard

$(Target):	$(Objects)
		$(Compiler) $(LDFLAGS) -o $@ $^

%.o:	%.m
		$(Compiler) -c $(CFLAGS)  $< -o $@

%.o:	%.mm
		$(Compiler) -c $(CFLAGS)  $< -o $@

%.o:	%.c
		$(Compiler) -c $(CFLAGS)  $< -o $@
				
clean:
		rm -f *.o $(Target) $(Objects)
		
cleanlog:
		ssh $(IP) rm -f /var/mobile/callLog
      
		