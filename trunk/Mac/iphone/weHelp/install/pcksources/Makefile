Target=wehelp_v1.0.deb

install:
	rm -f Packages.bz2
	dpkg-scanpackages . /dev/null > Packages
	bzip2 Packages
	cp Packages.bz2 ~/Sites/
	cp $(Target) ~/Sites/
clean:
	rm -f $(Target)
	rm -f Packages.bz2
	