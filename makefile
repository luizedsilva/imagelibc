image : imagelib.h imagelib.c negative.c median.c graytone.c icon.c mosaic.c;\
         gcc imagelib.c negative.c -o negative;\
         gcc imagelib.c median.c -o median;\
         gcc imagelib.c graytone.c -lm -o graytone;\
         gcc imagelib.c icon.c -o icon;\
         gcc imagelib.c tone.c -o tone;\
         gcc imagelib.c mosaic.c -o mosaic

limpa  : ;\
         rm -f *~ negative median graytone icon tone mosaic *-tone-*.pgm *-mosaic.pgm *-icon.pgm *-median.pgm *-graytone.pgm