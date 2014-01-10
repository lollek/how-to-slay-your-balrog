# BINDIR is the directory where the moria binary while be put
# LIBDIR is where the other files (score, news, hours) will be put
# LIBDIR must be the same directory defined in config.h
# OWNER is who you want the game to be chown to.
# GROUP is who you want the game to be chgrp to.
#
#BINDIR = /opt/bin
#LIBDIR = /opt/share/moria
#OWNER = games
#GROUP = games

# C compile
CC = gcc
CFLAGS = -Wall -Wextra -pedantic -g

# CXX compile
CXX = g++
CXXFLAGS = -Wall -Wextra -pedantic -Weffc++ -Werror --std=c++11 -g

# Shared compile
LFLAGS = -lcurses

ifeq ($(mode),strict)
  CFLAGS += -Werror
endif

# C only files:
CSRCS = main.c
COBJS = $(addsuffix .o, $(basename $(CSRCS)))

# C++ only files:
CXXSRCS = Graphics.cc Game.cc Main.cc Player.cc
CXXOBJS = $(addsuffix .o, $(basename $(CXXSRCS)))

# Shared files:
SRCS = misc1.c misc2.c misc3.c misc4.c store1.c files.c io.c \
	create.c desc.c generate.c sets.c dungeon.c creature.c death.c \
	eat.c help.c magic.c potions.c prayer.c save.c staffs.c wands.c \
	scrolls.c spells.c wizard.c store2.c moria1.c moria2.c \
	moria3.c moria4.c monsters.c treasure.c variable.c rnd.c recall.c \
	unix.c player.c tables.c
OBJS = $(addsuffix .o, $(basename $(SRCS)))

#LIBFILES = hours news origcmds.hlp owizcmds.hlp roglcmds.hlp rwizcmds.hlp \
#	version.hlp welcome.hlp

moria : $(OBJS) $(COBJS)
	$(CC) $^ $(LFLAGS) -o moria

cxxhack: $(OBJS) $(CXXOBJS)
	$(CXX) $^ $(LFLAGS) -o moria

# you must define BINDIR and LIBDIR before installing
# assumes that BINDIR and LIBDIR exist
# install:
# 	chmod 755 $(BINDIR)
# 	cp moria $(BINDIR)
# 	chmod 4711 $(BINDIR)/moria
# 	chmod 711 $(LIBDIR)
# 	(cd files; cp $(LIBFILES) $(LIBDIR))
# 	(cd $(LIBDIR); chmod 444 $(LIBFILES))
# 	(cd $(LIBDIR); touch scores; chmod 644 scores)
# 	chown $(OWNER) $(BINDIR)/moria
# 	chgrp $(GROUP) $(BINDIR)/moria
# 	(cd $(LIBDIR); chown $(OWNER) $(LIBFILES) scores)
# 	(cd $(LIBDIR); chgrp $(GROUP) $(LIBFILES) scores)
# If you are short on disk space, or aren't interested in debugging moria.
#	strip $(BINDIR)/moria

clean:
	$(RM) *.o
	$(RM) moria

# C only
main.o: constant.h types.h externs.h config.h

#C++ only
Graphics.o: Graphics.hh
Player.o: Player.hh
Game.o: Game.hh Graphics.hh Player.hh Tables.hh
Main.o: Game.hh

# Shared
create.o: constant.h types.h externs.h config.h
creature.o: constant.h types.h externs.h config.h
death.o: constant.h types.h externs.h config.h
desc.o: constant.h types.h externs.h config.h
dungeon.o: constant.h types.h externs.h config.h
eat.o: constant.h types.h externs.h config.h
files.o: constant.h types.h externs.h config.h
generate.o: constant.h types.h externs.h config.h
help.o: constant.h types.h externs.h config.h
io.o: constant.h types.h externs.h config.h
magic.o: constant.h types.h externs.h config.h
misc1.o: constant.h types.h externs.h config.h
misc2.o: constant.h types.h externs.h config.h
misc3.o: constant.h types.h externs.h config.h
misc4.o: constant.h types.h externs.h config.h
monsters.o: constant.h types.h config.h
moria1.o: constant.h types.h externs.h config.h
moria2.o: constant.h types.h externs.h config.h
moria3.o: constant.h types.h externs.h config.h
moria4.o: constant.h types.h externs.h config.h
player.o: constant.h types.h config.h
potions.o: constant.h types.h externs.h config.h
prayer.o: constant.h types.h externs.h config.h
recall.o: constant.h config.h types.h externs.h
rnd.o: constant.h types.h
save.o: constant.h types.h externs.h config.h
scrolls.o: constant.h types.h externs.h config.h
sets.o: constant.h config.h
spells.o: constant.h types.h externs.h config.h
staffs.o: constant.h types.h externs.h config.h
store1.o: constant.h types.h externs.h config.h
store2.o: constant.h types.h externs.h config.h
tables.o: constant.h types.h config.h
treasure.o: constant.h types.h config.h
unix.o: constant.h config.h types.h externs.h
variable.o: constant.h types.h config.h
wands.o: constant.h types.h externs.h config.h
wizard.o: constant.h types.h externs.h config.h
