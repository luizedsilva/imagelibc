#define CREATOR "# CREATOR: Image Processing usind C-Ansi - ByDu\n"

typedef int *image;

#define ERROR(x, y) \
    if (x)          \
    y

void errormsg(char *str, ...);
image img_alloc(int nr, int nc);
int img_free(image *Im);
void img_info(char *name, int nr, int nc, int ml);
image img_readpgm(char *name, int *nr, int *nc, int *ml);
void img_writepgm(image img, char *name, int nr, int nc, int ml);