CC=/Developer/Platforms/iPhoneOS.platform/Developer/usr/bin/arm-apple-darwin10-gcc-4.2.1
CXX=/Developer/Platforms/iPhoneOS.platform/Developer/usr/bin/arm-apple-darwin10-g++-4.2.1
LD=$(CC)

IP=192.168.10.184
PROJECTNAME=timer
SDKVER=4.0

CFLAGS= -I/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS$(SDKVER).sdk/include \
		-I/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS$(SDKVER).sdk/usr/include \
		-I/Developer/Platforms/iPhoneOS.platform/Developer/usr/lib/gcc/arm-apple-darwin10/4.2.1/include \
		-isysroot /Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS$(SDKVER).sdk

LDFLAGS=-framework CoreFoundation \
		-framework Foundation \
		-framework UIKit \
		-lobjc \
		-bind_at_load \
		-isysroot /Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS$(SDKVER).sdk

all:	clean $(PROJECTNAME) install

install:
	ldid_intel -S $(PROJECTNAME)
	scp $(PROJECTNAME) root@$(IP):~/

$(PROJECTNAME):	$(PROJECTNAME).o
	$(LD) $(LDFLAGS) -o $@ $^

%.o:	%.c
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $< -o $@

clean:
	rm -f *.o $(PROJECTNAME)
