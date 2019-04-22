image : imagelib.h imagelib.c negative.c;\
         gcc imagelib.c negative.c -lm -o negative;\
         gcc imagelib.c median.c -lm -o median

limpa  : ;\
         rm -f *~ negative