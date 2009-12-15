CC=/Developer/Platforms/iPhoneOS.platform/Developer/usr/bin/arm-apple-darwin9-gcc-4.0.1
CXX=/Developer/Platforms/iPhoneOS.platform/Developer/usr/bin/arm-apple-darwin9-g++-4.0.1
LD=$(CC)

Sysroot=/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS$(SDKVER).sdk

IP=192.168.10.184
PROJECTNAME=weHelper_daemon
SDKVER=2.2.1
INCDIR=./inc
SRCDIR=./src
LAUNCHPLIST=com.wehelp.luke.plist

CFLAGS=-I"$(Sysroot)/usr/include" \
		-F"$(Sysroot)/System/Library/Frameworks" \
		-F"$(Sysroot)/System/Library/PrivateFrameworks" \
		-isysroot $(Sysroot) \
		-I"$(INCDIR)" \
		-DDEBUG_SMS_ENABLED \
		-march=armv6 \
		-mcpu=arm1176jzf-s \
		-DSHOW_LOG
		#-fobjc-call-cxx-cdtors

LDFLAGS=-framework CoreFoundation \
		-framework Foundation \
		-framework CoreTelephony \
		-lobjc \
		-bind_at_load \
		-isysroot $(Sysroot) \
		-F"$(Sysroot)/System/Library/Frameworks" \
		-F"$(Sysroot)/System/Library/PrivateFrameworks" \
		-march=armv6 \
		-mcpu=arm1176jzf-s
		#-fobjc-call-cxx-cdtors
		#-framework UIKit
		
OBJS=$(patsubst %.m,%.o,$(wildcard $(SRCDIR)/*.m)) \
	$(patsubst %.c,%.o,$(wildcard $(SRCDIR)/*.c))
#OBJS=$(SRCDIR)/main.o	
	
all:	$(PROJECTNAME) sign

sign:
	ldid_intel -S $(PROJECTNAME)

install:
	#ldid_intel -S $(PROJECTNAME)
	ssh root@$(IP) rm -f /var/mobile/$(PROJECTNAME)
	scp $(PROJECTNAME) root@$(IP):/var/mobile/
	ssh root@$(IP) ldid -S /var/mobile/$(PROJECTNAME)
	scp $(LAUNCHPLIST) root@$(IP):/Library/LaunchDaemons
	ssh root@$(IP) /bin/launchctl unload /Library/LaunchDaemons/com.wehelp.luke.plist
	ssh root@$(IP) /bin/launchctl load /Library/LaunchDaemons/com.wehelp.luke.plist

$(PROJECTNAME):	$(OBJS)
	$(LD) $(LDFLAGS) -o $@ $^

%.o:	%.m
	$(CC) -c $(CFLAGS) $< -o $@

%.o:	%.c
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $< -o $@

clean:
	rm $(SRCDIR)/*.o $(PROJECTNAME)
