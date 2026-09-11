#include <stdio.h>

int main(int argc, char *argv[])
{
    if (argc != 2)
        return 1;
    int i;
    char *buf = NULL;

    i = 0;
    buf = argv[1];

    while (buf[i] != '\0')
    {
        printf("%c", buf[i] - i);
        i++;
    }
    printf("\n");
    return 0;
}