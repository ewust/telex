# $Id: Makefile.in 6952 2009-12-05 00:23:47Z robin $

VPATH = /home/ewust/telex-station/station/bro-1.5.1/aux/broctl

# capstats will be handled separately. 
DIST_IGNORE = config.status aux/capstats

all: pybroccoli pysubnettree capstats config.status

config.status: /home/ewust/telex-station/station/bro-1.5.1/aux/broctl/bin/broctl.in /home/ewust/telex-station/station/bro-1.5.1/aux/broctl/Makefile.in /home/ewust/telex-station/station/bro-1.5.1/aux/broctl/VERSION
	./config.status

install:

install-broctl: 
	MAKE_DESTDIR=$(DESTDIR) ./bin/make-wrapper install --make

install-local: 
	MAKE_DESTDIR=$(DESTDIR) ./bin/make-wrapper install --local

print-config:
	./bin/make-wrapper config

clean:
	rm -f BroControl/*.pyc
	rm -rf .python-build
	( cd aux/capstats && ${MAKE} clean)

distclean:
	rm -f config.status Makefile bin/broctl bin/make-wrapper
	rm -f BroControl/*.pyc
	rm -rf .python-build
	( cd aux/capstats && ${MAKE} distclean)
	rm -rf aux/capstats/capstats-* 
    
distdir:
	@test x"$(distdir)" != x"" || ( echo error: 'make dist' must be called from top-level Makefile && exit 1 )
	rm -f .exclude
	for i in $(DIST_IGNORE); do echo $$i >>.exclude; done;
	find . -regex '.*/\.svn' | sed 's#^\./##g' >>.exclude
	tar cf - -X .exclude * | ( cd $(distdir) && tar xf - )
	rm -f .exclude

	mkdir $(distdir)/aux/capstats; 
	( cd aux/capstats; unset MAKEFLAGS; distdir="" top_distdir="" make distdir; ) 
	( cd aux/capstats/capstats-* && tar cf - . ) | ( cd $(distdir)/aux/capstats && tar xf - ); 

doc:
	python BroControl/options.py >README.options
	python ./bin/broctl --print-doc >README.cmds
	asciidoc --unsafe -a toc -a no-homepage-link -b xhtml11-custom README

pybroccoli:
	@echo broctl: building Python bindings for Broccoli ...        
	( cd /home/ewust/telex-station/station/bro-1.5.1/aux/broccoli/bindings/python \
      && CFLAGS="-I/home/ewust/telex-station/station/bro-1.5.1/aux/broccoli/src" LDFLAGS="-L/home/ewust/telex-station/station/bro-1.5.1/aux/broccoli/src/.libs" python setup.py build -b /home/ewust/telex-station/station/bro-1.5.1/aux/broctl/.python-build )
	@( cd /home/ewust/telex-station/station/bro-1.5.1/aux/broctl/.python-build && test -e lib || ln -s lib.* lib )	

pysubnettree:
	@echo broctl: building pysubnettree Python module ...        
	@( cd /home/ewust/telex-station/station/bro-1.5.1/aux/broctl/aux/pysubnettree \
      && python setup.py build -b /home/ewust/telex-station/station/bro-1.5.1/aux/broctl/.python-build )

capstats:
	@echo broctl: building capstats ...        
	@( cd /home/ewust/telex-station/station/bro-1.5.1/aux/broctl/aux/capstats && ${MAKE} )

## Make distcheck happy.
EMPTY_AUTOMAKE_TARGETS = dvi pdf ps info html tags ctags install-data install-exec uninstall install-dvi install-html install-info install-ps install-pdf installdirs check installcheck mostlyclean maintainer-clean tags ctags
$(EMPTY_AUTOMAKE_TARGETS):
