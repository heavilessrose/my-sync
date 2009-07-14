CC=/Developer/Platforms/iPhoneOS.platform/Developer/usr/bin/arm-apple-darwin9-gcc-4.0.1
CXX=/Developer/Platforms/iPhoneOS.platform/Developer/usr/bin/arm-apple-darwin9-g++-4.0.1
LD=$(CC)

CFLAGS= -I/Developer/SDKs/iPhoneOS.sdk/Versions/iPhoneOS2.0.sdk/include \
		-I/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS2.0.sdk/usr/include \
		-I/Developer/Platforms/iPhoneOS.platform/Developer/usr/lib/gcc/arm-apple-darwin9/4.0.1/include \
		-isysroot /Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS2.0.sdk

LDFLAGS=-framework CoreFoundation -framework Foundation -framework UIKit \
	-lobjc -bind_at_load -isysroot /Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS2.0.sdk

all:	timer

timer:	timer.o
	$(LD) $(LDFLAGS) -o $@ $^

%.o:	%.c
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $< -o $@

clean:
	rm *.o timer
