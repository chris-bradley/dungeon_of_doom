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
		-o dungeon_lib_win.o

dungeon_lib_lin.o : dungeon_lib.cpp
	g++ \
		-c dungeon_lib.cpp \
		-l SDL2 \
		-l SDL2_ttf \
		-D main=SDL_main \
		-I /usr/include/SDL2 \
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

