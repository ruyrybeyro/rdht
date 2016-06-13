bin_PROGRAMS = rdht
rdht_SOURCES = dht.c
AUTOMAKE_OPTIONS = foreign

noinst_HEADERS = 

distclean-local:
	rm -rf autom4te.cache .deps Makefile.in configure

splint:
	splint dht.c -warnposix -I/usr/local/include  -I/usr/include/arm-linux-gnueabihf -shiftimplementation
