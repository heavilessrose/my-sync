
default:	build

clean:
	find ./objs -name *.o -type f -exec rm -fr {} \;
	rm -rf ./objs/nginx ./objs/nginx.8

build:
	$(MAKE) -f objs/Makefile
	$(MAKE) -f objs/Makefile manpage

install:
	$(MAKE) -f objs/Makefile install

upgrade:
	/Users/luke/Mac/Mac/workspace/nginx-0.9.4/workingDir/sbin/nginx -t

	kill -USR2 `cat /Users/luke/Mac/Mac/workspace/nginx-0.9.4/workingDir/logs/nginx.pid`
	sleep 1
	test -f /Users/luke/Mac/Mac/workspace/nginx-0.9.4/workingDir/logs/nginx.pid.oldbin

	kill -QUIT `cat /Users/luke/Mac/Mac/workspace/nginx-0.9.4/workingDir/logs/nginx.pid.oldbin`
