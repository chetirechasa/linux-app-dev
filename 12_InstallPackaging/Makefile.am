SUBDIRS = lib manual po src tests
ACLOCAL_AMFLAGS = -I m4

@DX_RULES@

all-local:	doxygen-doc

install-data-local:
	mkdir -p $(DESTDIR)/$(docdir)
	cp -a doxygen-doc/html $(DESTDIR)/$(docdir)

http:	doxygen-doc
	python3 -m http.server --directory $</html

gitclean:
	git clean -fdx