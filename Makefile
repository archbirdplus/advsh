#
#  John Coker
#  University of California, Berkeley
#
#  Makefile for ASH, the wonderful Adventure SHell
#  John Coker
#

DEST	      = /usr/local

PROGRAM	      = advsh

MANDIR	      = /usr/man/man1

MANUAL	      = advsh.1

EXTHDRS	      = /usr/include/ctype.h \
		/usr/include/errno.h \
		/usr/include/pwd.h \
		/usr/include/setjmp.h \
		/usr/include/signal.h \
		/usr/include/stdio.h \
		/usr/include/sys/dir.h \
		/usr/include/sys/file.h \
		/usr/include/sys/ioctl.h \
		/usr/include/sys/stat.h \
		/usr/include/sys/ttychars.h \
		/usr/include/sys/ttychars.h \
		/usr/include/sys/ttydev.h \
		/usr/include/sys/ttydev.h \
		/usr/include/sys/types.h \
		hgets/hist.h \
		jc/jobs.h

HDRS	      = cmdlist.h \
		defs.h \
		functs.h \
		names.h \
		types.h

CFLAGS	      = -O

IFLAGS	      = -o john

LDFLAGS	      = $(CFLAGS)

MYLIBS	      = libjc.a libwcb.a

LIBS	      = $(MYLIBS)

LINKER	      = cc

MAKEFILE      = Makefile

OBJS	      = alias.o \
		command.o \
		debug.o \
		drop.o \
		file.o \
		help.o \
		hist.o \
		hit.o \
		inventory.o \
		items.o \
		look.o \
		magic.o \
		main.o \
		move.o \
		names.o \
		parse.o \
		pickup.o \
		say.o \
		scores.o \
		setup.o \
		shell.o \
		sigs.o \
		source.o \
		use.o \
		util.o \
		vars.o \
		wield.o

PRINT	      = tgrind -Pgp

SRCS	      = alias.c \
		command.c \
		debug.c \
		drop.c \
		file.c \
		help.c \
		hist.c \
		hit.c \
		inventory.c \
		items.c \
		look.c \
		magic.c \
		main.c \
		move.c \
		names.c \
		parse.c \
		pickup.c \
		say.c \
		scores.c \
		setup.c \
		shell.c \
		sigs.c \
		source.c \
		use.c \
		util.c \
		vars.c \
		wield.c

DISTRIB	      = README $(MAKEFILE) $(SRCS) $(HDRS) $(MANUAL) $(MYLIBS)

TAR	      = $(PROGRAM).tar

$(PROGRAM):     $(LIBS) $(OBJS)
		@rm -f $(PROGRAM) core
		@echo "Linking \`$(PROGRAM)' ..."
		@$(LINKER) $(LDFLAGS) $(OBJS) $(LIBS) -o $(PROGRAM)
		@size $(PROGRAM)

install:	$(DEST)/$(PROGRAM) $(MANDIR)/$(MANUAL)
		@echo "Installation completed."

libjc.a:;	@cd jc ; make install

libwcb.a:	hgets/libwcb.a
		@cd hgets ; make install

clean:;		@rm -f $(OBJS) $(PROGRAM) core *.out .#* $(TAR)

depend:;	@rm -f .#*
		@mkmf -f $(MAKEFILE) PROGRAM=$(PROGRAM) DEST=$(DEST)

tar::		$(TAR)

$(TAR):		$(DISTRIB)
		@rm -f $(TAR)
		tar cvf $(TAR) $(DISTRIB)
		@ls -lgs $(TAR)

index:;		@ctags -wx $(HDRS) $(SRCS)

print:;		@$(PRINT) $(HDRS) $(SRCS)

tags:;          @ctags $(HDRS) $(SRCS)

update:		$(DEST)/$(PROGRAM)

manual:		$(MANDIR)/$(MANUAL)

$(DEST)/$(PROGRAM): $(PROGRAM)
		install $(IFLAGS) -c -s -m 0755 $(PROGRAM) $(DEST)
		@ls -lgs $(DEST)/$(PROGRAM)

$(MANDIR)/$(MANUAL): $(MANUAL)
		install $(IFLAGS) -c -m 0644 $(MANUAL) $(MANDIR)
		@ls -lgs $(MANDIR)/$(MANUAL)

.DEFAULT:;	co $@

.SUFFIXES:;
.SUFFIXES:	.o .c
###
alias.o: /usr/include/stdio.h defs.h types.h /usr/include/pwd.h
command.o: /usr/include/stdio.h /usr/include/ctype.h \
	/usr/include/sys/file.h defs.h types.h /usr/include/pwd.h functs.h \
	cmdlist.h
debug.o: /usr/include/stdio.h /usr/include/signal.h \
	/usr/include/sys/types.h /usr/include/sys/ioctl.h \
	/usr/include/sys/ttychars.h /usr/include/sys/ttydev.h \
	/usr/include/sys/ttychars.h /usr/include/sys/ttydev.h defs.h types.h \
	/usr/include/pwd.h jc/jobs.h
drop.o: /usr/include/stdio.h /usr/include/signal.h /usr/include/sys/types.h \
	/usr/include/sys/stat.h /usr/include/sys/file.h defs.h types.h \
	/usr/include/pwd.h
file.o: /usr/include/stdio.h /usr/include/signal.h /usr/include/sys/types.h \
	/usr/include/sys/file.h /usr/include/sys/stat.h defs.h types.h \
	/usr/include/pwd.h
help.o: /usr/include/stdio.h defs.h functs.h
hist.o: /usr/include/stdio.h /usr/include/ctype.h defs.h hgets/hist.h
hit.o: /usr/include/stdio.h /usr/include/sys/types.h \
	/usr/include/sys/file.h /usr/include/sys/stat.h defs.h types.h \
	/usr/include/pwd.h jc/jobs.h
inventory.o: /usr/include/stdio.h /usr/include/sys/types.h \
	/usr/include/sys/file.h defs.h types.h /usr/include/pwd.h
items.o: /usr/include/stdio.h /usr/include/sys/types.h \
	/usr/include/sys/file.h /usr/include/sys/stat.h defs.h types.h \
	/usr/include/pwd.h
look.o: /usr/include/stdio.h /usr/include/sys/types.h \
	/usr/include/sys/file.h /usr/include/sys/dir.h \
	/usr/include/sys/stat.h defs.h types.h /usr/include/pwd.h
magic.o: /usr/include/stdio.h /usr/include/sys/types.h \
	/usr/include/sys/file.h /usr/include/sys/dir.h defs.h types.h \
	/usr/include/pwd.h jc/jobs.h
main.o: /usr/include/stdio.h /usr/include/signal.h /usr/include/errno.h \
	/usr/include/sys/types.h /usr/include/sys/dir.h \
	/usr/include/sys/file.h defs.h types.h /usr/include/pwd.h jc/jobs.h
move.o: /usr/include/stdio.h /usr/include/sys/types.h \
	/usr/include/sys/stat.h /usr/include/sys/file.h defs.h types.h \
	/usr/include/pwd.h
names.o: /usr/include/stdio.h /usr/include/sys/types.h \
	/usr/include/sys/file.h defs.h types.h /usr/include/pwd.h names.h
parse.o: /usr/include/stdio.h /usr/include/ctype.h defs.h types.h \
	/usr/include/pwd.h
pickup.o: /usr/include/stdio.h /usr/include/sys/types.h \
	/usr/include/sys/stat.h /usr/include/sys/file.h defs.h types.h \
	/usr/include/pwd.h
say.o: /usr/include/stdio.h
scores.o: /usr/include/stdio.h defs.h types.h /usr/include/pwd.h
setup.o: /usr/include/stdio.h /usr/include/signal.h \
	/usr/include/sys/types.h /usr/include/sys/ioctl.h \
	/usr/include/sys/ttychars.h /usr/include/sys/ttydev.h \
	/usr/include/sys/ttychars.h /usr/include/sys/ttydev.h \
	/usr/include/sys/file.h defs.h types.h /usr/include/pwd.h \
	hgets/hist.h
shell.o: /usr/include/stdio.h /usr/include/setjmp.h \
	/usr/include/sys/types.h /usr/include/sys/stat.h types.h \
	/usr/include/pwd.h defs.h hgets/hist.h
sigs.o: /usr/include/stdio.h /usr/include/signal.h
source.o: /usr/include/stdio.h /usr/include/pwd.h /usr/include/sys/file.h
use.o: /usr/include/stdio.h /usr/include/sys/types.h \
	/usr/include/sys/file.h /usr/include/sys/stat.h defs.h types.h \
	/usr/include/pwd.h jc/jobs.h
util.o: /usr/include/stdio.h /usr/include/ctype.h /usr/include/sys/types.h \
	/usr/include/sys/dir.h defs.h types.h /usr/include/pwd.h
vars.o: /usr/include/stdio.h defs.h types.h /usr/include/pwd.h
wield.o: /usr/include/stdio.h /usr/include/sys/types.h \
	/usr/include/sys/file.h /usr/include/sys/stat.h defs.h types.h \
	/usr/include/pwd.h
