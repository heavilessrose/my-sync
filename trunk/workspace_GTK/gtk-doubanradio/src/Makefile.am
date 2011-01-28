## Process this file with automake to produce Makefile.in

## Created by Anjuta

uidir = $(datadir)/gtk_doubanradio/ui
ui_DATA = gtk_doubanradio.ui

AM_CPPFLAGS = \
	-DPACKAGE_LOCALE_DIR=\""$(prefix)/$(DATADIRNAME)/locale"\" \
	-DPACKAGE_SRC_DIR=\""$(srcdir)"\" \
	-DPACKAGE_DATA_DIR=\""$(datadir)"\" \
	$(GTK_DOUBANRADIO_CFLAGS)

AM_CFLAGS =\
	 -Wall\
	 -g

bin_PROGRAMS = gtk_doubanradio

gtk_doubanradio_SOURCES = \
	callbacks.c \
	callbacks.h \
	main.c

gtk_doubanradio_LDFLAGS = \
	-Wl,--export-dynamic

gtk_doubanradio_LDADD = $(GTK_DOUBANRADIO_LIBS)

EXTRA_DIST = $(ui_DATA)

# Remove ui directory on uninstall
uninstall-local:
	-rm -r $(uidir)
	-rm -r $(datadir)/gtk_doubanradio
