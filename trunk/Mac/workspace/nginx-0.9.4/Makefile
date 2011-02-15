
default:	build

clean:
	rm -rf objs

renew:
	rm -rf Makefile objs
	
build:
	$(MAKE) -f objs/Makefile
	$(MAKE) -f objs/Makefile manpage

install:
	$(MAKE) -f objs/Makefile install

upgrade:
	./workingDir/sbin/nginx -t

	kill -USR2 `cat ./workingDir/logs/nginx.pid`
	sleep 1
	test -f ./workingDir/logs/nginx.pid.oldbin

	kill -QUIT `cat ./workingDir/logs/nginx.pid.oldbin`
