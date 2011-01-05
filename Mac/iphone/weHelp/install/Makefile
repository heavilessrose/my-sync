IP=root@192.168.10.184
Target=wehelp_v1.1.deb
SDKVER=4.1
Programset=../weHelp_UI/build/$(SDKVER)/weHelp_UI.app
Programdaemon=../weHelp_daemon/weHelper_daemon

all: install

install:
		cd ../weHelp_UI && $(MAKE)		
		cd ../weHelp_daemon && $(MAKE)
		cp -rf $(Programset) wehelp/Applications/
		cp -f $(Programdaemon) wehelp/var/mobile/
		cp -f ../weHelp_daemon/com.wehelp.luke.plist wehelp/Library/LaunchDaemons/
		dpkg-deb -b ./wehelp $(Target)
		mv -f $(Target) pcksources
		cd pcksources && $(MAKE)
clean:
		rm -rf wehelp/Applications/weHelp_UI.app
		rm -f wehelp/var/mobile/weHelper_daemon
		rm -f wehelp/Library/LaunchDaemons/com.wehelp.luke.plist
		make  clean -C pcksources
		make  clean -C ../weHelp_UI
		make  clean -C ../weHelp_daemon
		