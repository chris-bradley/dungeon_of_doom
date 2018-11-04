MINGW_ROOT = /usr/i686-w64-mingw32/sys-root/mingw/

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

dungeon_lib_win.o : dungeon_lib.c dungeon_lib.h
	i686-w64-mingw32-gcc \
		-g \
		-Wall \
		-c dungeon_lib.c \
		-static-libgcc \
		-Wall \
		-Wl,-Bstatic \
		`$(MINGW_ROOT)/bin/sdl2-config --libs` \
		-l SDL2_ttf \
		-l freetype \
		-l iconv \
		-l png \
		-l bz2 \
		-l z \
		-Wl,-Bdynamic \
		-l winmm \
		-l ole32 \
		-l gdi32 \
		-l imm32 \
		-l version \
		-l oleaut32 \
		`$(MINGW_ROOT)/bin/sdl2-config --cflags` \
		-o dungeon_lib_win.o

dungeon_lib_lin.o : dungeon_lib.c dungeon_lib.h
	gcc \
		-g \
		-c dungeon_lib.c \
		`sdl2-config --libs` \
		-l SDL2_ttf \
		-D main=SDL_main \
		`sdl2-config --cflags` \
		-o dungeon_lib_lin.o

dungeon_audio_lin.o : dungeon_audio.c dungeon_audio.h
	gcc \
		-g \
		-c \
		dungeon_audio.c \
		-Wall \
		`sdl2-config --libs` \
		`sdl2-config --cflags` \
		-o dungeon_audio_lin.o

dungeon_audio_win.o : dungeon_audio.c dungeon_audio.h
	i686-w64-mingw32-gcc \
		-g \
		-c \
		dungeon_audio.c \
		-Wall \
		-Wl,-Bstatic \
		`$(MINGW_ROOT)/bin/sdl2-config --libs` \
		-Wl,-Bdynamic \
		-l winmm \
		-l ole32 \
		-l gdi32 \
		-l imm32 \
		-l version \
		-l oleaut32 \
		`$(MINGW_ROOT)/bin/sdl2-config --cflags` \
		-o dungeon_audio_win.o

dungeon_generator.exe : dungeon_generator.c dungeon_lib_win.o
	i686-w64-mingw32-gcc \
		-g \
		dungeon_lib_win.o \
		dungeon_generator.c \
		-static-libgcc \
		-Wall \
		-Wl,-Bstatic \
		`$(MINGW_ROOT)/bin/sdl2-config --libs` \
		-l SDL2_ttf \
		-l freetype \
		-l iconv \
		-l png \
		-l bz2 \
		-l z \
		-Wl,-Bdynamic \
		-l winmm \
		-l ole32 \
		-l gdi32 \
		-l imm32 \
		-l version \
		-l oleaut32 \
		-I $(MINGW_ROOT)/include \
		-I $(MINGW_ROOT)/include/freetype2 \
		`$(MINGW_ROOT)/bin/sdl2-config --cflags` \
		-o dungeon_generator.exe

dungeon_generator : dungeon_generator.c dungeon_lib_lin.o
	gcc \
		-g \
		`sdl2-config --libs` \
		-l SDL2_ttf \
		`sdl2-config --cflags` \
		-o dungeon_generator \
		dungeon_lib_lin.o \
		dungeon_generator.c

character_creator.exe : character_creator.c dungeon_lib_win.o character_lib.h
	i686-w64-mingw32-gcc \
		-g \
		dungeon_lib_win.o \
		character_creator.c \
		-static-libgcc \
		-Wall \
		-Wl,-Bstatic \
		`$(MINGW_ROOT)/bin/sdl2-config --libs` \
		-l SDL2_ttf \
		-l freetype \
		-l iconv \
		-l png \
		-l bz2 \
		-l z \
		-Wl,-Bdynamic \
		-l winmm \
		-l ole32 \
		-l gdi32 \
		-l imm32 \
		-l version \
		-l oleaut32 \
		-I $(MINGW_ROOT)/include \
		-I $(MINGW_ROOT)/include/freetype2 \
		`$(MINGW_ROOT)/bin/sdl2-config --cflags` \
		-o character_creator.exe

character_creator : character_creator.c dungeon_lib_lin.o character_lib.h
	gcc \
		-g \
		`sdl2-config --libs` \
		-l SDL2_ttf \
		`sdl2-config --cflags` \
		-o character_creator \
		dungeon_lib_lin.o \
		character_creator.c

game_module.exe : \
	game_module.c dungeon_lib_win.o dungeon_audio_win.o character_lib.h
	i686-w64-mingw32-gcc \
		-g \
		dungeon_lib_win.o \
		dungeon_audio_win.o \
		game_module.c \
		-static-libgcc \
		-Wall \
		-Wl,-Bstatic \
		`$(MINGW_ROOT)/bin/sdl2-config --libs` \
		-l SDL2_ttf \
		-l freetype \
		-l iconv \
		-l png \
		-l bz2 \
		-l z \
		-Wl,-Bdynamic \
		-l winmm \
		-l ole32 \
		-l gdi32 \
		-l imm32 \
		-l version \
		-l oleaut32 \
		-I $(MINGW_ROOT)/include \
		-I $(MINGW_ROOT)/include/freetype2 \
		`$(MINGW_ROOT)/bin/sdl2-config --cflags` \
		-o game_module.exe

game_module : \
	game_module.c dungeon_lib_lin.o dungeon_audio_lin.o character_lib.h
	gcc \
		-g \
		`sdl2-config --libs` \
		-l SDL2_ttf \
		-l m \
		`sdl2-config --cflags` \
		-o game_module \
		dungeon_lib_lin.o \
		dungeon_audio_lin.o \
		game_module.c

test_audio.exe : test_audio.c dungeon_audio_win.o
	        i686-w64-mingw32-gcc \
			-g \
			test_audio.c \
			-static-libgcc \
			-Wall \
			-Wl,-Bstatic \
			`$(MINGW_ROOT)/bin/sdl2-config --libs` \
			-Wl,-Bdynamic \
			-l winmm \
			-l ole32 \
			-l gdi32 \
			-l imm32 \
			-l version \
			-l oleaut32 \
			-I $(MINGW_ROOT)/include \
		        `$(MINGW_ROOT)/bin/sdl2-config --cflags` \
			dungeon_audio_win.o \
			-o test_audio.exe

test_audio : test_audio.c dungeon_audio_lin.o
	        gcc \
			-g \
			test_audio.c \
			-Wall \
			`sdl2-config --libs` \
			`sdl2-config --cflags` \
			dungeon_audio_lin.o \
			-o test_audio
