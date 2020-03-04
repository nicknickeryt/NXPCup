#include "stdio.h"
#include "string.h"
#include "stdint.h"


int main(int argc, char **argv)
{
    if (argc < 2)
    {
        printf("No filename given!\n");
        return -1;
    }

    char filename[32];
    filename[1] = '_';
    sprintf(filename + 2, "%s", argv[1]);

    size_t read_bytes;

    FILE *f_in = fopen(filename + 2, "rb");
    filename[0] = 'r';
    FILE *f_r = fopen(filename, "wb");
    filename[0] = 'g';
    FILE *f_g = fopen(filename, "wb");
    filename[0] = 'b';
    FILE *f_b = fopen(filename, "wb");

    FILE *f_gbo = fopen("green_bytes_only", "wb");

    if (f_in == NULL || f_r == NULL || f_g == NULL || f_b == NULL || NULL == f_gbo)
    {
        printf("Couldn't open file!\n");
        fclose(f_in);
        fclose(f_r);
        fclose(f_g);
        fclose(f_b);
        fclose(f_gbo);

        return -1;
    }

    uint8_t arr[3];
    uint8_t r_arr[3] = {0, 0, 0};
    uint8_t g_arr[3] = {0, 0, 0};
    uint8_t b_arr[3] = {0, 0, 0};

    int width = 316;
    int height = 208;

    fprintf(f_r, "P6\n%d %d\n255\n", width, height);
    fprintf(f_g, "P6\n%d %d\n255\n", width, height);
    fprintf(f_b, "P6\n%d %d\n255\n", width, height);

    fread(arr, 1, 3, f_in);
    fread(arr, 1, 3, f_in);
    fread(arr, 1, 3, f_in);
    fread(arr, 1, 3, f_in);
    fread(arr, 1, 3, f_in);
    while (read_bytes = fread(arr, 1, 3, f_in))
    {
        if (read_bytes != 3)
        {
            printf("Read %u bytes, but it should be 3\n", read_bytes);
        }
        r_arr[0] = arr[0];
        g_arr[1] = arr[1];
        b_arr[2] = arr[2];
        fwrite(r_arr, 1, 3, f_r);
        fwrite(g_arr, 1, 3, f_g);
        fwrite(b_arr, 1, 3, f_b);

        fwrite(g_arr + 1, 1, 1, f_gbo);
        fwrite(g_arr + 1, 1, 1, f_gbo);
    }
    fclose(f_in);
    fclose(f_r);
    fclose(f_g);
    fclose(f_b);
    fclose(f_gbo);

    return 0;
}
