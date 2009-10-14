
PROJECTNAME=addr_sql_test
SDKVER=2.0
SDK=/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS$(SDKVER).sdk
IPHONE_IP=192.168.10.135
BUILDDIR=./build/$(SDKVER)
DESTDIR=/var/root/

CC=/Developer/Platforms/iPhoneOS.platform/Developer/usr/bin/arm-apple-darwin9-gcc-4.0.1
CXX=/Developer/Platforms/iPhoneOS.platform/Developer/usr/bin/arm-apple-darwin9-g++-4.0.1
LD=$(CC)

CFLAGS= -I/Developer/SDKs/iPhoneOS.sdk/Versions/iPhoneOS$(SDKVER).sdk/include \
		-I${SDK}/usr/include \
		-I/Developer/Platforms/iPhoneOS.platform/Developer/usr/lib/gcc/arm-apple-darwin9/4.0.1/include \
		-isysroot ${SDK}

LDFLAGS=-framework CoreFoundation -framework Foundation -framework UIKit \
		-isysroot ${SDK} \
		-L/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneSimulator$(SDKVER).sdk/usr/lib \
		-lsqlite3
		-march=armv6 \
		-mcpu=arm1176jzf-s \

all:	install

${PROJECTNAME}:	${PROJECTNAME}.o
	$(LD) $(LDFLAGS) -o $@ $^

%.o:	%.c
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $< -o $@

clean:
	rm *.o
	rm ${PROJECTNAME}
	
install:	${PROJECTNAME}
	ssh root@$(IPHONE_IP) 'rm -fr ${PROJECTNAME}'
	scp -r ./$(PROJECTNAME) root@$(IPHONE_IP):${DESTDIR}
	ssh root@$(IPHONE_IP) 'ldid -S ${PROJECTNAME}'
