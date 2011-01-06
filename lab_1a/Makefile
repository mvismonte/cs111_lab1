# CS 111 Lab 1 Makefile

CC = gcc
CFLAGS = -g -W -Wall -Werror -Wno-unused
LAB = 1
DISTDIR = lab1a-$(USER)
V = @

all: cmdline

ospsh: cmdline.o ospsh.o main-b.o
	@echo + link $@
	$(V)$(CC) $(CFLAGS) -lpthread -o $@ cmdline.o ospsh.o main-b.o

ospsh.o: ospsh.c cmdline.h ospsh.h
	@echo + cc ospsh.c
	$(V)$(CC) $(CFLAGS) -c ospsh.c

cmdline.o: cmdline.c cmdline.h
	@echo + cc cmdline.c
	$(V)$(CC) $(CFLAGS) -c cmdline.c

main-a.o: main-a.c cmdline.h
	@echo + cc main-a.c
	$(V)$(CC) $(CFLAGS) -c main-a.c

main-b.o: main-b.c cmdline.h ospsh.h
	@echo + cc main-b.c
	$(V)$(CC) $(CFLAGS) -c main-b.c

cmdline: cmdline.o main-a.o
	@echo + link cmdline
	$(V)$(CC) $(CFLAGS) -o $@ cmdline.o main-a.o

tarball: realclean
	@echo + mk $(DISTDIR).tar.gz
	$(V)mkdir $(DISTDIR)
	$(V)tar cf - `ls | grep -v '^$(DISTDIR)'` | (cd $(DISTDIR) && tar xf -)
	$(V)/bin/rm -rf `find $(DISTDIR) -name CVS -print`
	$(V)date > $(DISTDIR)/tarballstamp
	$(V)/bin/bash ./check-lab.sh $(DISTDIR) || (rm -rf $(DISTDIR); false)
	$(V)tar cf $(DISTDIR).tar $(DISTDIR)
	$(V)gzip $(DISTDIR).tar
	$(V)/bin/rm -rf $(DISTDIR)

clean:
	@echo + clean
	$(V)rm -rf *.o *~ *.bak core *.core ospsh cmdline freecheck

realclean: clean
	@echo + realclean
	$(V)rm -rf $(DISTDIR) $(DISTDIR).tar.gz

.PHONY: tarball clean realclean
