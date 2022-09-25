#include "DeleteCompletely.h"

static void snCreateFile(int numberOfFile)
{
    FILE *fp = NULL;
    char fileName[256];

    for(int x = 0; x < numberOfFile; ++x) {
        sprintf(fileName, "%08d.md", (x+1));
        fp = fopen(fileName, "w");
        fclose(fp);
    }
}

int main(int argc, char **argv)
{
    Initializer();
    char **p = NULL;
    int numberOfFile;
    
    if (argc != 2) {
        printf("%s [�ļ�����]\n", argv[0]);
        printf("������: ");
        scanf("%d", &numberOfFile);
        snCreateFile(numberOfFile);
    } else {
        numberOfFile = strtoll(argv[1], p, 10);
        snCreateFile(numberOfFile);
    }

    return 0;
}



