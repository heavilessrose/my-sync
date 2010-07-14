

#configure customs
./configure --prefix=/Users/luke/Documents/my-sync/trunk/Mac/workspace/C/pidgin/dist \
--with-static-prpls=jabber,msn,irc \
--disable-gtkspell --disable-vv --disable-avahi --disable-nm --disable-gstreamer \
--enable-gnutls=yes --with-gnutls-includes=/opt/local/include/ --with-gnutls-libs=/opt/local/lib \
--enable-nss=yes \
--disable-gestures --disable-perl --disable-tcl --disable-tk \
--enable-doxygen --enable-dot


--enable-debug
--disable-plugins 
otr 加密



make distclean


// find nss default, while, use below symbols not work
--enable-nss --with-nss-includes=/opt/local/include/nss --with-nspr-includes=/opt/local/include/nspr --with-nss-libs=/opt/local/lib/nss --with-nspr-libs=/opt/local/lib

