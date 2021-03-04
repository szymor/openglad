CFLAGS=-g `sdl2-config --libs --cflags` -lSDL2_mixer -lpng -lyaml -lzip -lphysfs
GLADFILES=button.o effect.o game.o glad.o graphlib.o guy.o help.o intro.o living.o obmap.o pal32.o picker.o pixie.o pixien.o radar.o screen.o smooth.o sound.o stats.o text.o treasure.o video.o view.o walker.o weap.o input.o sai2x.o gloader.o gparser.o level_editor.o util.o io.o level_data.o save_data.o pixie_data.o results_screen.o campaign_picker.o level_picker.o
GLADFILES += ../util/savepng.o
GLADFILES += external/micropather/micropather.o
GLADFILES += external/yam/yam.o
GLADFILES += external/physfs/extras/physfsrwops.o
#GLADFILES += external/libzip/zip_open.o external/libzip/zip_dir_add.o external/libzip/zip_strerror.o external/libzip/zip_source_file.o external/libzip/zip_file_add.o external/libzip/zip_source_free.o external/libzip/zip_close.o external/libzip/zip_get_num_entries.o external/libzip/zip_stat_index.o external/libzip/zip_fopen_index.o external/libzip/zip_fread.o external/libzip/zip_fclose.o

# openscen is stale
SCENFILES=scen.o effect.o game.o gladpack.o graphlib.o guy.o help.o intro.o living.o loader.o obmap.o pal32.o parser.o pixie.o pixien.o radar.o screen.o smooth.o sound.o stats.o text.o treasure.o video.o view.o walker.o weap.o input.o sai2x.o

INCLUDES = -I.. -I/usr/include/SDL2
INCLUDES += -Iexternal/micropather
INCLUDES += -Iexternal/yam
INCLUDES += -Iexternal/physfs -Iexternal/physfs/extras

all: openglad

openglad: $(GLADFILES)
	g++ $(CFLAGS) -o ../openglad $(GLADFILES)

openscen: $(SCENFILES)
	g++ $(CFLAGS) -DOPENSCEN -o ../openscen $(SCENFILES)

clean:
	rm -f *.o
	rm -f ../openglad
	rm -f ../openscen

.cpp.o:
	g++ -g -c $(INCLUDES) $<

# SDL_SavePNG
../util/%.o: ../util/%.c
	gcc -g -c $(INCLUDES) $< -o $@

# micropather
external/micropather/%.o: external/micropather/%.cpp
	g++ -g -c $(INCLUDES) $< -o $@

# yam
external/yam/%.o: external/yam/%.cpp
	g++ -g -c $(INCLUDES) $< -o $@

# physfs
#external/physfs/%.o: external/physfs/%.c
#	gcc -g -c $(INCLUDES) $< -o $@

external/physfs/extras/%.o: external/physfs/extras/%.c
	gcc -g -c $(INCLUDES) $< -o $@

# libzip
#external/libzip/%.o: external/libzip/%.c
#	gcc -g -c $(INCLUDES) $< -o $@
