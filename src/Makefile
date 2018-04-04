all: dungeon_lib.c dungeon_generator.c dungeon_lib_win.o dungeon_lib_lin.o character_creator.c
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
		-L/usr/i686-w64-mingw32/sys-root/mingw/lib \
		-l mingw32 \
		-l SDL2main \
		-l SDL2 \
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
		-I /usr/i686-w64-mingw32/sys-root/mingw/include/SDL2 \
		-D main=SDL_main \
		-o dungeon_lib_win.o

dungeon_lib_lin.o : dungeon_lib.c dungeon_lib.h
	gcc \
		-g \
		-c dungeon_lib.c \
		-l SDL2 \
		-l SDL2_ttf \
		-D main=SDL_main \
		-I /usr/include/SDL2 \
		-o dungeon_lib_lin.o

dungeon_generator.exe : dungeon_generator.c dungeon_lib_win.o
	i686-w64-mingw32-gcc \
		-g \
		dungeon_lib_win.o \
		dungeon_generator.c \
		-static-libgcc \
		-Wall \
		-Wl,-Bstatic \
		-L/usr/i686-w64-mingw32/sys-root/mingw/lib \
		-l mingw32 \
		-l SDL2main \
		-l SDL2 \
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
		-I /usr/i686-w64-mingw32/sys-root/mingw/include \
		-I /usr/i686-w64-mingw32/sys-root/mingw/include/freetype2 \
		-I /usr/i686-w64-mingw32/sys-root/mingw/include/SDL2 \
		-D main=SDL_main \
		-o dungeon_generator.exe

dungeon_generator : dungeon_generator.c dungeon_lib_lin.o
	gcc \
		-g \
		-l SDL2 \
		-l SDL2_ttf \
		-I /usr/include/SDL2 \
		-o dungeon_generator \
		dungeon_lib_lin.o \
		dungeon_generator.c

character_creator.exe : character_creator.c dungeon_lib_win.o
	i686-w64-mingw32-gcc \
		-g \
		dungeon_lib_win.o \
		character_creator.c \
		-static-libgcc \
		-Wall \
		-Wl,-Bstatic \
		-L/usr/i686-w64-mingw32/sys-root/mingw/lib \
		-l mingw32 \
		-l SDL2main \
		-l SDL2 \
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
		-I /usr/i686-w64-mingw32/sys-root/mingw/include \
		-I /usr/i686-w64-mingw32/sys-root/mingw/include/freetype2 \
		-I /usr/i686-w64-mingw32/sys-root/mingw/include/SDL2 \
		-D main=SDL_main \
		-o character_creator.exe

character_creator : character_creator.c dungeon_lib_lin.o
	gcc \
		-g \
		-l SDL2 \
		-l SDL2_ttf \
		-I /usr/include/SDL2 \
		-o character_creator \
		dungeon_lib_lin.o \
		character_creator.c

game_module.exe : game_module.c dungeon_lib_win.o
	i686-w64-mingw32-gcc \
		-g \
		dungeon_lib_win.o \
		game_module.c \
		-static-libgcc \
		-Wall \
		-Wl,-Bstatic \
		-L/usr/i686-w64-mingw32/sys-root/mingw/lib \
		-l mingw32 \
		-l SDL2main \
		-l SDL2 \
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
		-I /usr/i686-w64-mingw32/sys-root/mingw/include \
		-I /usr/i686-w64-mingw32/sys-root/mingw/include/freetype2 \
		-I /usr/i686-w64-mingw32/sys-root/mingw/include/SDL2 \
		-D main=SDL_main \
		-o game_module.exe

game_module : game_module.c dungeon_lib_lin.o
	gcc \
		-g \
		-l SDL2 \
		-l SDL2_ttf \
		-I /usr/include/SDL2 \
		-o game_module \
		dungeon_lib_lin.o \
		game_module.c

test_audio.exe : test_audio.c
	        i686-w64-mingw32-gcc \
			-g \
			test_audio.c \
			-static-libgcc \
			-Wall \
			-Wl,-Bstatic \
			-L/usr/i686-w64-mingw32/sys-root/mingw/lib \
			-l mingw32 \
			-l SDL2main \
			-l SDL2 \
			-Wl,-Bdynamic \
			-l winmm \
			-l ole32 \
			-l gdi32 \
			-l imm32 \
			-l version \
			-l oleaut32 \
			-I /usr/i686-w64-mingw32/sys-root/mingw/include \
			-I /usr/i686-w64-mingw32/sys-root/mingw/include/SDL2 \
			-D main=SDL_main \
			-o test_audio.exe

test_audio : test_audio.c
	        gcc \
			-g \
			test_audio.c \
			-Wall \
			-l SDL2 \
			-I /usr/include/SDL2 \
			-o test_audio.exe
