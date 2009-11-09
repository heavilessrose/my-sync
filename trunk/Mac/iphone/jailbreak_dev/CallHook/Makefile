Compiler=/Developer/Platforms/iPhoneOS.platform/Developer/usr/bin/arm-apple-darwin9-gcc-4.0.1
CXX	:= /Developer/Platforms/iPhoneOS.platform/Developer/usr/bin/arm-apple-darwin9-g++-4.0.1
//IP=root@192.168.10.157
//IP=root@169.254.150.114
IP=root@192.168.10.111
//IP=root@192.168.10.184
Sysroot=/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS2.0.sdk

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
				-init _ExampleCallInitializeMyHook \
				-Wall \
				-Werror \
				-lsubstrate \
				-lobjc \
				-ObjC++ \
				-fobjc-exceptions \
				-march=armv6 \
				-mcpu=arm1176jzf-s \
				-fobjc-call-cxx-cdtors

CFLAGS= -dynamiclib \
		-I/Developer/SDKs/iPhoneOS.sdk/Versions/iPhoneOS2.0.wu.sdk/include \
		-I/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS2.0.sdk/usr/include \
		-isysroot /Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS2.0.sdk
		
Objects= ExampleCallLibrary.o PlayerView.o GifVideo.o Video.o VideoSource.o \
		VideoTexture.o dgif_lib.o egif_lib.o gif_err.o gifalloc.o MyView.o MyLog.o
		
Sources= ExampleCallLibrary.mm PlayerView.mm GifVideo.m Video.m VideoSource.m \
		VideoTexture.m dgif_lib.c egif_lib.c gif_err.c gifalloc.c MyView.mm MyLog.m


Target=CallHook.dylib


compile:	$(Target) 

all: $(Target) install

install:
		scp $(Target) $(IP):/var/root
		ssh $(IP) chmod 755 $(Target) 
		ssh $(IP) ldid -S $(Target)
		ssh $(IP) rm -f /Library/MobileSubstrate/DynamicLibraries/*
		ssh $(IP) rm -f /var/mobile/callLog
		ssh $(IP) mv $(Target) /Library/MobileSubstrate/DynamicLibraries
		ssh $(IP) killall SpringBoard

installbak:
		scp $(Target) $(IP):/var/root
		ssh $(IP) chmod 755 $(Target) 
		ssh $(IP) ldid -S $(Target)
		ssh $(IP) rm -f /Library/MobileSubstrate/DynamicLibraries/*
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
		rm -f *.o $(Target)