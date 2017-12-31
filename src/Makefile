all: dungeon_lib.cpp dungeon_generator.cpp dungeon_lib_win.o dungeon_lib_lin.o character_creator.cpp
	make \
		dungeon_lib_win.o \
		dungeon_lib_lin.o \
		dungeon_generator.exe \
		dungeon_generator \
		character_creator.exe \
		character_creator \
		game_module.exe \
		game_module

dungeon_lib_win.o : dungeon_lib.cpp
	i686-w64-mingw32-g++ \
		-Wall \
		-c dungeon_lib.cpp \
		-static-libstdc++ \
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
		-std=gnu++11 \
		-o dungeon_lib_win.o

dungeon_lib_lin.o : dungeon_lib.cpp
	g++ \
		-c dungeon_lib.cpp \
		-l SDL2 \
		-l SDL2_ttf \
		-D main=SDL_main \
		-I /usr/include/SDL2 \
		-std=gnu++11 \
		-o dungeon_lib_lin.o

dungeon_generator.exe : dungeon_generator.cpp dungeon_lib_win.o
	i686-w64-mingw32-g++ \
		dungeon_lib_win.o \
		dungeon_generator.cpp \
		-static-libstdc++ \
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

dungeon_generator : dungeon_generator.cpp dungeon_lib_lin.o
	g++ \
		-l SDL2 \
		-l SDL2_ttf \
		-I /usr/include/SDL2 \
		-o dungeon_generator \
		dungeon_lib_lin.o \
		dungeon_generator.cpp

character_creator.exe : character_creator.cpp dungeon_lib_win.o
	i686-w64-mingw32-g++ \
		dungeon_lib_win.o \
		character_creator.cpp \
		-static-libstdc++ \
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

character_creator : character_creator.cpp dungeon_lib_lin.o
	g++ \
		-l SDL2 \
		-l SDL2_ttf \
		-I /usr/include/SDL2 \
		-o character_creator \
		dungeon_lib_lin.o \
		character_creator.cpp

game_module.exe : game_module.cpp dungeon_lib_win.o
	i686-w64-mingw32-g++ \
		dungeon_lib_win.o \
		game_module.cpp \
		-static-libstdc++ \
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

game_module : game_module.cpp dungeon_lib_lin.o
	g++ \
		-l SDL2 \
		-l SDL2_ttf \
		-I /usr/include/SDL2 \
		-o game_module \
		dungeon_lib_lin.o \
		game_module.cpp
