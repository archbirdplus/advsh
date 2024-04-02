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

USR_INCLUDE=/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include

EXTHDRS	      = $(USR_INCLUDE)/ctype.h \
		$(USR_INCLUDE)/errno.h \
		$(USR_INCLUDE)/pwd.h \
		$(USR_INCLUDE)/setjmp.h \
		$(USR_INCLUDE)/signal.h \
		$(USR_INCLUDE)/stdio.h \
		$(USR_INCLUDE)/sys/dir.h \
		$(USR_INCLUDE)/sys/file.h \
		$(USR_INCLUDE)/sys/ioctl.h \
		$(USR_INCLUDE)/sys/stat.h \
		$(USR_INCLUDE)/sys/ttychars.h \
		$(USR_INCLUDE)/sys/ttychars.h \
		$(USR_INCLUDE)/sys/ttydev.h \
		$(USR_INCLUDE)/sys/ttydev.h \
		$(USR_INCLUDE)/sys/types.h \

		#hgets/hist.h \
		#jc/jobs.h

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

# .DEFAULT:;	co $@

.SUFFIXES:;
.SUFFIXES:	.o .c
###
alias.o: $(USR_INCLUDE)/stdio.h defs.h types.h $(USR_INCLUDE)/pwd.h
command.o: $(USR_INCLUDE)/stdio.h $(USR_INCLUDE)/ctype.h \
	$(USR_INCLUDE)/sys/file.h defs.h types.h $(USR_INCLUDE)/pwd.h functs.h \
	cmdlist.h
debug.o: $(USR_INCLUDE)/stdio.h $(USR_INCLUDE)/signal.h \
	$(USR_INCLUDE)/sys/types.h $(USR_INCLUDE)/sys/ioctl.h \
	$(USR_INCLUDE)/sys/ttychars.h $(USR_INCLUDE)/sys/ttydev.h \
	$(USR_INCLUDE)/sys/ttychars.h $(USR_INCLUDE)/sys/ttydev.h defs.h types.h \
	$(USR_INCLUDE)/pwd.h jc/jobs.h
drop.o: $(USR_INCLUDE)/stdio.h $(USR_INCLUDE)/signal.h $(USR_INCLUDE)/sys/types.h \
	$(USR_INCLUDE)/sys/stat.h $(USR_INCLUDE)/sys/file.h defs.h types.h \
	$(USR_INCLUDE)/pwd.h
file.o: $(USR_INCLUDE)/stdio.h $(USR_INCLUDE)/signal.h $(USR_INCLUDE)/sys/types.h \
	$(USR_INCLUDE)/sys/file.h $(USR_INCLUDE)/sys/stat.h defs.h types.h \
	$(USR_INCLUDE)/pwd.h
help.o: $(USR_INCLUDE)/stdio.h defs.h functs.h
hist.o: $(USR_INCLUDE)/stdio.h $(USR_INCLUDE)/ctype.h defs.h hgets/hist.h
hit.o: $(USR_INCLUDE)/stdio.h $(USR_INCLUDE)/sys/types.h \
	$(USR_INCLUDE)/sys/file.h $(USR_INCLUDE)/sys/stat.h defs.h types.h \
	$(USR_INCLUDE)/pwd.h jc/jobs.h
inventory.o: $(USR_INCLUDE)/stdio.h $(USR_INCLUDE)/sys/types.h \
	$(USR_INCLUDE)/sys/file.h defs.h types.h $(USR_INCLUDE)/pwd.h
items.o: $(USR_INCLUDE)/stdio.h $(USR_INCLUDE)/sys/types.h \
	$(USR_INCLUDE)/sys/file.h $(USR_INCLUDE)/sys/stat.h defs.h types.h \
	$(USR_INCLUDE)/pwd.h
look.o: $(USR_INCLUDE)/stdio.h $(USR_INCLUDE)/sys/types.h \
	$(USR_INCLUDE)/sys/file.h $(USR_INCLUDE)/sys/dir.h \
	$(USR_INCLUDE)/sys/stat.h defs.h types.h $(USR_INCLUDE)/pwd.h
magic.o: $(USR_INCLUDE)/stdio.h $(USR_INCLUDE)/sys/types.h \
	$(USR_INCLUDE)/sys/file.h $(USR_INCLUDE)/sys/dir.h defs.h types.h \
	$(USR_INCLUDE)/pwd.h jc/jobs.h
main.o: $(USR_INCLUDE)/stdio.h $(USR_INCLUDE)/signal.h $(USR_INCLUDE)/errno.h \
	$(USR_INCLUDE)/sys/types.h $(USR_INCLUDE)/sys/dir.h \
	$(USR_INCLUDE)/sys/file.h defs.h types.h $(USR_INCLUDE)/pwd.h jc/jobs.h
move.o: $(USR_INCLUDE)/stdio.h $(USR_INCLUDE)/sys/types.h \
	$(USR_INCLUDE)/sys/stat.h $(USR_INCLUDE)/sys/file.h defs.h types.h \
	$(USR_INCLUDE)/pwd.h
names.o: $(USR_INCLUDE)/stdio.h $(USR_INCLUDE)/sys/types.h \
	$(USR_INCLUDE)/sys/file.h defs.h types.h $(USR_INCLUDE)/pwd.h names.h
parse.o: $(USR_INCLUDE)/stdio.h $(USR_INCLUDE)/ctype.h defs.h types.h \
	$(USR_INCLUDE)/pwd.h
pickup.o: $(USR_INCLUDE)/stdio.h $(USR_INCLUDE)/sys/types.h \
	$(USR_INCLUDE)/sys/stat.h $(USR_INCLUDE)/sys/file.h defs.h types.h \
	$(USR_INCLUDE)/pwd.h
say.o: $(USR_INCLUDE)/stdio.h
scores.o: $(USR_INCLUDE)/stdio.h defs.h types.h $(USR_INCLUDE)/pwd.h
setup.o: $(USR_INCLUDE)/stdio.h $(USR_INCLUDE)/signal.h \
	$(USR_INCLUDE)/sys/types.h $(USR_INCLUDE)/sys/ioctl.h \
	$(USR_INCLUDE)/sys/ttychars.h $(USR_INCLUDE)/sys/ttydev.h \
	$(USR_INCLUDE)/sys/ttychars.h $(USR_INCLUDE)/sys/ttydev.h \
	$(USR_INCLUDE)/sys/file.h defs.h types.h $(USR_INCLUDE)/pwd.h \
	hgets/hist.h
shell.o: $(USR_INCLUDE)/stdio.h $(USR_INCLUDE)/setjmp.h \
	$(USR_INCLUDE)/sys/types.h $(USR_INCLUDE)/sys/stat.h types.h \
	$(USR_INCLUDE)/pwd.h defs.h hgets/hist.h
sigs.o: $(USR_INCLUDE)/stdio.h $(USR_INCLUDE)/signal.h
source.o: $(USR_INCLUDE)/stdio.h $(USR_INCLUDE)/pwd.h $(USR_INCLUDE)/sys/file.h
use.o: $(USR_INCLUDE)/stdio.h $(USR_INCLUDE)/sys/types.h \
	$(USR_INCLUDE)/sys/file.h $(USR_INCLUDE)/sys/stat.h defs.h types.h \
	$(USR_INCLUDE)/pwd.h jc/jobs.h
util.o: $(USR_INCLUDE)/stdio.h $(USR_INCLUDE)/ctype.h $(USR_INCLUDE)/sys/types.h \
	$(USR_INCLUDE)/sys/dir.h defs.h types.h $(USR_INCLUDE)/pwd.h
vars.o: $(USR_INCLUDE)/stdio.h defs.h types.h $(USR_INCLUDE)/pwd.h
wield.o: $(USR_INCLUDE)/stdio.h $(USR_INCLUDE)/sys/types.h \
	$(USR_INCLUDE)/sys/file.h $(USR_INCLUDE)/sys/stat.h defs.h types.h \
	$(USR_INCLUDE)/pwd.h
