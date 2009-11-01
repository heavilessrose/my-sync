CC=/Developer/Platforms/iPhoneOS.platform/Developer/usr/bin/arm-apple-darwin9-gcc-4.0.1
CXX=/Developer/Platforms/iPhoneOS.platform/Developer/usr/bin/arm-apple-darwin9-g++-4.0.1
LD=$(CC)

IP=192.168.1.109
PROJECTNAME=timer
SDKVER=2.2.1

CFLAGS= -I/Developer/SDKs/iPhoneOS.sdk/Versions/iPhoneOS$(SDKVER).sdk/include \
		-I/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS$(SDKVER).sdk/usr/include \
		-I/Developer/Platforms/iPhoneOS.platform/Developer/usr/lib/gcc/arm-apple-darwin9/4.0.1/include \
		-isysroot /Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS$(SDKVER).sdk

LDFLAGS=-framework CoreFoundation \
		-framework Foundation \
		-framework UIKit \
		-lobjc \
		-bind_at_load \
		-isysroot /Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS$(SDKVER).sdk

all:	install

install:
	ldid_intel -S $(PROJECTNAME)
	scp $(PROJECTNAME) root@$(IP):~/tests/

$(PROJECTNAME):	$(PROJECTNAME).o
	$(LD) $(LDFLAGS) -o $@ $^

%.o:	%.c
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $< -o $@

clean:
	rm *.o $(PROJECTNAME)
