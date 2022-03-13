BASE_FLAGS = -g -Wall -Wextra

MINGW_GCC = i686-w64-mingw32-gcc
MINGW_ROOT = /usr/i686-w64-mingw32/sys-root/mingw/
MINGW_PKG_CONFIG = /usr/bin/i686-w64-mingw32-pkg-config
MINGW_FT_LIBS = `${MINGW_PKG_CONFIG} libpng bzip2 --libs`
MINGW_DYN_LIBS = -mwindows


all: \
	dungeon_lib.c dungeon_generator.c dungeon_lib_win.o dungeon_lib_lin.o \
	character_creator.c character_lib.h
	make \
		dungeon_lib_win.o \
		dungeon_lib_lin.o \
		dungeon_generator.exe \
		dungeon_generator \
		character_creator.exe \
		character_creator \
		game_module.exe \
		game_module

all-linux: \
	dungeon_lib.c dungeon_generator.c dungeon_lib_lin.o \
	character_creator.c character_lib.h
	make \
		dungeon_lib_lin.o \
		dungeon_generator \
		character_creator \
		game_module

all-windows: \
	dungeon_lib.c dungeon_generator.c dungeon_lib_win.o \
	character_creator.c character_lib.h
	make \
		dungeon_lib_win.o \
		dungeon_generator.exe \
		character_creator.exe \
		game_module.exe \

dungeon_lib_win.o : dungeon_lib.c dungeon_lib.h
	$(MINGW_GCC) \
		$(BASE_FLAGS) \
		-c dungeon_lib.c \
		-static-libgcc \
		$(MINGW_DYN_LIBS) \
		`$(MINGW_ROOT)/bin/sdl2-config --libs` \
		`$(MINGW_ROOT)/bin/freetype-config --libs` \
		`$(MINGW_ROOT)/bin/sdl2-config --cflags` \
		`$(MINGW_ROOT)/bin/freetype-config --cflags` \
		-o dungeon_lib_win.o

dungeon_lib_lin.o : dungeon_lib.c dungeon_lib.h
	gcc \
		$(BASE_FLAGS) \
		-c dungeon_lib.c \
		`sdl2-config --libs` \
		`freetype-config --libs` \
		`sdl2-config --cflags` \
		`freetype-config --cflags` \
		-o dungeon_lib_lin.o

dungeon_audio_lin.o : dungeon_audio.c dungeon_audio.h
	gcc \
		$(BASE_FLAGS) \
		-c \
		dungeon_audio.c \
		`sdl2-config --libs` \
		`sdl2-config --cflags` \
		-o dungeon_audio_lin.o

dungeon_audio_win.o : dungeon_audio.c dungeon_audio.h
	$(MINGW_GCC) \
		$(BASE_FLAGS) \
		-c \
		dungeon_audio.c \
		$(MINGW_DYN_LIBS) \
		`$(MINGW_ROOT)/bin/sdl2-config --libs` \
		`$(MINGW_ROOT)/bin/sdl2-config --cflags` \
		-o dungeon_audio_win.o

monster_lib_lin.o: monster_lib.h monster_lib.c
	gcc \
		$(BASE_FLAGS) \
		-c \
		monster_lib.c \
		`sdl2-config --cflags` \
		`sdl2-config --libs` \
		-o monster_lib_lin.o

monster_lib_win.o: monster_lib.h monster_lib.c
	$(MINGW_GCC) \
		$(BASE_FLAGS) \
		-c \
		monster_lib.c \
		$(MINGW_DYN_LIBS) \
		`$(MINGW_ROOT)/bin/sdl2-config --cflags` \
		`$(MINGW_ROOT)/bin/sdl2-config --libs` \
		-o monster_lib_win.o

dungeon_generator.exe : dungeon_generator.c dungeon_lib_win.o
	$(MINGW_GCC) \
		$(BASE_FLAGS) \
		dungeon_lib_win.o \
		dungeon_generator.c \
		-static-libgcc \
		$(MINGW_DYN_LIBS) \
		`$(MINGW_ROOT)/bin/sdl2-config --libs` \
		-l setupapi \
		`$(MINGW_ROOT)/bin/freetype-config --libs` \
		${MINGW_FT_LIBS} \
		`$(MINGW_ROOT)/bin/sdl2-config --cflags` \
		`$(MINGW_ROOT)/bin/freetype-config --cflags` \
		-o dungeon_generator.exe

dungeon_generator : dungeon_generator.c dungeon_lib_lin.o
	gcc \
		$(BASE_FLAGS) \
		`sdl2-config --libs` \
		`freetype-config --libs` \
		`sdl2-config --cflags` \
		`freetype-config --cflags` \
		-o dungeon_generator \
		dungeon_lib_lin.o \
		dungeon_generator.c

character_creator.exe : character_creator.c dungeon_lib_win.o character_lib.h
	$(MINGW_GCC) \
		$(BASE_FLAGS) \
		dungeon_lib_win.o \
		character_creator.c \
		-static-libgcc \
		$(MINGW_DYN_LIBS) \
		`$(MINGW_ROOT)/bin/sdl2-config --libs` \
		-l setupapi \
		`$(MINGW_ROOT)/bin/freetype-config --libs` \
		${MINGW_FT_LIBS} \
		`$(MINGW_ROOT)/bin/freetype-config --cflags` \
		`$(MINGW_ROOT)/bin/sdl2-config --cflags` \
		-o character_creator.exe

character_creator : character_creator.c dungeon_lib_lin.o character_lib.h
	gcc \
		$(BASE_FLAGS) \
		`sdl2-config --libs` \
		`freetype-config --libs` \
		`sdl2-config --cflags` \
		`freetype-config --cflags` \
		-o character_creator \
		dungeon_lib_lin.o \
		character_creator.c

game_module.exe : \
	game_module.c dungeon_lib_win.o dungeon_audio_win.o character_lib.h \
	monster_lib_win.o
	$(MINGW_GCC) \
		$(BASE_FLAGS) \
		dungeon_lib_win.o \
		dungeon_audio_win.o \
		monster_lib_win.o \
		game_module.c \
		-static-libgcc \
		$(MINGW_DYN_LIBS) \
		`$(MINGW_ROOT)/bin/sdl2-config --libs` \
		-l setupapi \
		`$(MINGW_ROOT)/bin/freetype-config --libs` \
		${MINGW_FT_LIBS} \
		`$(MINGW_ROOT)/bin/sdl2-config --cflags` \
		`$(MINGW_ROOT)/bin/freetype-config --cflags` \
		-o game_module.exe

game_module : \
	game_module.c dungeon_lib_lin.o dungeon_audio_lin.o character_lib.h \
	monster_lib_lin.o
	gcc \
		$(BASE_FLAGS) \
		`sdl2-config --libs` \
		`freetype-config --libs` \
		-l m \
		`sdl2-config --cflags` \
		`freetype-config --cflags` \
		-o game_module \
		dungeon_lib_lin.o \
		dungeon_audio_lin.o \
		monster_lib_lin.o \
		game_module.c

test_audio.exe : test_audio.c dungeon_audio_win.o
	$(MINGW_GCC) \
		$(BASE_FLAGS) \
		test_audio.c \
		-static-libgcc \
		$(MINGW_DYN_LIBS) \
		`$(MINGW_ROOT)/bin/sdl2-config --libs` \
		-l setupapi \
		`$(MINGW_ROOT)/bin/sdl2-config --cflags` \
		dungeon_audio_win.o \
		-o test_audio.exe

test_audio : test_audio.c dungeon_audio_lin.o
	gcc \
		$(BASE_FLAGS) \
		test_audio.c \
		-l m \
		`sdl2-config --libs` \
		`sdl2-config --cflags` \
		dungeon_audio_lin.o \
		-o test_audio
