CFLAGS=-g `sdl2-config --cflags`
LDFLAGS= -g `sdl2-config --libs` -lSDL2_mixer -lpng -lyaml -lzip
GLADFILES=button.o effect.o game.o glad.o graphlib.o guy.o help.o intro.o living.o obmap.o pal32.o picker.o pixie.o pixien.o radar.o screen.o smooth.o sound.o stats.o text.o treasure.o video.o view.o walker.o weap.o input.o sai2x.o gloader.o gparser.o level_editor.o util.o io.o level_data.o save_data.o pixie_data.o results_screen.o campaign_picker.o level_picker.o
GLADFILES += ../util/savepng.o
GLADFILES += external/micropather/micropather.o
GLADFILES += external/yam/yam.o
GLADFILES += external/physfs/src/physfs.o external/physfs/extras/physfsrwops.o external/physfs/src/physfs_platform_posix.o external/physfs/src/physfs_unicode.o external/physfs/src/physfs_archiver_7z.o external/physfs/src/physfs_archiver_zip.o external/physfs/src/physfs_archiver_grp.o external/physfs/src/physfs_archiver_qpak.o external/physfs/src/physfs_archiver_hog.o external/physfs/src/physfs_archiver_mvl.o external/physfs/src/physfs_archiver_wad.o external/physfs/src/physfs_archiver_slb.o external/physfs/src/physfs_archiver_dir.o external/physfs/src/physfs_archiver_iso9660.o external/physfs/src/physfs_archiver_unpacked.o external/physfs/src/physfs_archiver_vdf.o external/physfs/src/physfs_byteorder.o external/physfs/src/physfs_platform_unix.o

# openscen is stale
SCENFILES=scen.o effect.o game.o gladpack.o graphlib.o guy.o help.o intro.o living.o loader.o obmap.o pal32.o parser.o pixie.o pixien.o radar.o screen.o smooth.o sound.o stats.o text.o treasure.o video.o view.o walker.o weap.o input.o sai2x.o

CFLAGS += -I..
CFLAGS += -Iexternal/micropather
CFLAGS += -Iexternal/yam
CFLAGS += -Iexternal/physfs -Iexternal/physfs/extras

all: openglad

openglad: $(GLADFILES)
	g++ $(LDFLAGS) -o ../openglad $(GLADFILES)

openscen: $(SCENFILES)
	g++ $(LDFLAGS) -DOPENSCEN -o ../openscen $(SCENFILES)

clean:
	rm -f *.o
	rm -f ../openglad
	rm -f ../openscen

.cpp.o:
	g++ -g -c $(CFLAGS) $<

# SDL_SavePNG
../util/%.o: ../util/%.c
	gcc -g -c $(CFLAGS) $< -o $@

# micropather
external/micropather/%.o: external/micropather/%.cpp
	g++ -g -c $(CFLAGS) $< -o $@

# yam
external/yam/%.o: external/yam/%.cpp
	g++ -g -c $(CFLAGS) $< -o $@

# physfs + extras
external/physfs/src/%.o: external/physfs/%.c
	gcc -g -c $(CFLAGS) $< -o $@

external/physfs/extras/%.o: external/physfs/extras/%.c
	gcc -g -c $(CFLAGS) $< -o $@
