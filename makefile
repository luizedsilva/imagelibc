image : imagelib.h imagelib.c negative.c;\
         gcc imagelib.c negative.c -o negative;\
         gcc imagelib.c median.c -o median;\
         gcc imagelib.c graytone.c -lm -o graytone

limpa  : ;\
         rm -f *~ negative