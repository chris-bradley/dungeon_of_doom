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
		libcoord_win.o \
		libcoord_lin.o \
		libmap_win.o \
		libmap_lin.o \
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
		`$(MINGW_ROOT)/bin/sdl2-config --libs --cflags` \
		`$(MINGW_ROOT)/bin/freetype-config --libs --cflags` \
		-o dungeon_lib_win.o

dungeon_lib_lin.o : dungeon_lib.c dungeon_lib.h
	gcc \
		$(BASE_FLAGS) \
		-c dungeon_lib.c \
		`sdl2-config --libs --cflags` \
		`freetype-config --libs --cflags` \
		-o dungeon_lib_lin.o

dungeon_audio_lin.o : dungeon_audio.c dungeon_audio.h
	gcc \
		$(BASE_FLAGS) \
		-c \
		dungeon_audio.c \
		`sdl2-config --libs --cflags` \
		-o dungeon_audio_lin.o

dungeon_audio_win.o : dungeon_audio.c dungeon_audio.h
	$(MINGW_GCC) \
		$(BASE_FLAGS) \
		-c \
		dungeon_audio.c \
		$(MINGW_DYN_LIBS) \
		`$(MINGW_ROOT)/bin/sdl2-config --libs --cflags` \
		-o dungeon_audio_win.o

monster_lib_lin.o: monster_lib.h monster_lib.c
	gcc \
		$(BASE_FLAGS) \
		-c \
		monster_lib.c \
		`sdl2-config --cflags --libs` \
		-o monster_lib_lin.o

libcoord_lin.o: libcoord.h libcoord.c
	gcc \
		$(BASE_FLAGS) \
		-c \
		libcoord.c \
		-o libcoord_lin.o

libcoord_win.o: libcoord.h libcoord.c
	$(MINGW_GCC) \
		$(BASE_FLAGS) \
		-c \
		libcoord.c \
		-o libcoord_win.o

libmap_lin.o: libmap.h libmap.c
	gcc \
		$(BASE_FLAGS) \
		-c \
		libmap.c \
		`sdl2-config --libs --cflags` \
		-o libmap_lin.o

libmap_win.o: libmap.h libmap.c
	$(MINGW_GCC) \
		$(BASE_FLAGS) \
		-c \
		libmap.c \
		`$(MINGW_ROOT)/bin/sdl2-config --cflags --libs` \
		${MINGW_FT_LIBS} \
		-o libmap_win.o

monster_lib_win.o: monster_lib.h monster_lib.c
	$(MINGW_GCC) \
		$(BASE_FLAGS) \
		-c \
		monster_lib.c \
		$(MINGW_DYN_LIBS) \
		`$(MINGW_ROOT)/bin/sdl2-config --cflags --libs` \
		-o monster_lib_win.o

dungeon_generator.exe : \
	dungeon_generator.c dungeon_lib_win.o libcoord_win.o libmap_win.o
	$(MINGW_GCC) \
		$(BASE_FLAGS) \
		dungeon_lib_win.o \
		libcoord_win.o \
		libmap_win.o \
		dungeon_generator.c \
		-static-libgcc \
		$(MINGW_DYN_LIBS) \
		`$(MINGW_ROOT)/bin/sdl2-config --libs --cflags` \
		-l setupapi \
		`$(MINGW_ROOT)/bin/freetype-config --libs --cflags` \
		${MINGW_FT_LIBS} \
		-o dungeon_generator.exe

dungeon_generator : \
	dungeon_generator.c dungeon_lib_lin.o libcoord_lin.o libmap_lin.o
	gcc \
		$(BASE_FLAGS) \
		`sdl2-config --libs --cflags` \
		`freetype-config --libs --cflags` \
		-o dungeon_generator \
		dungeon_lib_lin.o \
		libcoord_lin.o \
		libmap_lin.o \
		dungeon_generator.c

character_creator.exe : character_creator.c dungeon_lib_win.o character_lib.h
	$(MINGW_GCC) \
		$(BASE_FLAGS) \
		dungeon_lib_win.o \
		character_creator.c \
		-static-libgcc \
		$(MINGW_DYN_LIBS) \
		`$(MINGW_ROOT)/bin/sdl2-config --libs --cflags` \
		-l setupapi \
		`$(MINGW_ROOT)/bin/freetype-config --libs --cflags` \
		${MINGW_FT_LIBS} \
		-o character_creator.exe

character_creator : character_creator.c dungeon_lib_lin.o character_lib.h
	gcc \
		$(BASE_FLAGS) \
		`sdl2-config --libs --cflags` \
		`freetype-config --libs --cflags` \
		-o character_creator \
		dungeon_lib_lin.o \
		character_creator.c

game_module.exe : \
	game_module.c dungeon_lib_win.o dungeon_audio_win.o character_lib.h \
	monster_lib_win.o libcoord_win.o libmap_win.o
	$(MINGW_GCC) \
		$(BASE_FLAGS) \
		dungeon_lib_win.o \
		dungeon_audio_win.o \
		monster_lib_win.o \
		libcoord_win.o \
		libmap_win.o \
		game_module.c \
		-static-libgcc \
		$(MINGW_DYN_LIBS) \
		`$(MINGW_ROOT)/bin/sdl2-config --libs --cflags` \
		-l setupapi \
		`$(MINGW_ROOT)/bin/freetype-config --libs --cflags` \
		${MINGW_FT_LIBS} \
		-o game_module.exe

game_module : \
	game_module.c dungeon_lib_lin.o dungeon_audio_lin.o character_lib.h \
	monster_lib_lin.o libcoord_lin.o libmap_lin.o
	gcc \
		$(BASE_FLAGS) \
		`sdl2-config --libs --cflags` \
		`freetype-config --libs  --cflags` \
		-l m \
		-o game_module \
		dungeon_lib_lin.o \
		dungeon_audio_lin.o \
		monster_lib_lin.o \
		libcoord_lin.o \
		libmap_lin.o \
		game_module.c

test_audio.exe : test_audio.c dungeon_audio_win.o
	$(MINGW_GCC) \
		$(BASE_FLAGS) \
		test_audio.c \
		-static-libgcc \
		$(MINGW_DYN_LIBS) \
		`$(MINGW_ROOT)/bin/sdl2-config --libs --cflags` \
		-l setupapi \
		dungeon_audio_win.o \
		-o test_audio.exe

test_audio : test_audio.c dungeon_audio_lin.o
	gcc \
		$(BASE_FLAGS) \
		test_audio.c \
		-l m \
		`sdl2-config --libs --cflags` \
		dungeon_audio_lin.o \
		-o test_audio
