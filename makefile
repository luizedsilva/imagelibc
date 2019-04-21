image : imagelib.h imagelib.c negative.c;\
         gcc imagelib.c negative.c -lm -o negative

limpa  : ;\
         rm -f *~ negative