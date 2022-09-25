#include "DeleteCompletely.h"

static void process(snDel *set, struct stat statPath)
{
    stat(set->filePath, &statPath);
    if(exists(set->filePath) && S_ISREG(statPath.st_mode)) {
        set->fileSize      = getFileSize(set->filePath);
        set->ProcN.total   = set->fileSize / BlockSize;
        set->ProcN.surplus = set->fileSize % BlockSize;
        set->randData      = (u8 *)malloc(BlockSize);
        writeRemove(set);
    } else {
        char warningText[BlockSize];
        char warningTitle[BlockSize];
        sprintf(warningTitle, "���Գ���%s�ľ��棡", __argv[0]);
        sprintf(warningText, "�ļ�%s�����ã����飡\n", set->filePath);
        MessageBoxA(NULL, warningText, warningTitle, MB_ICONERROR | MB_OK);
    }
}

int main(int argc, char **argv)
{
    Initializer();        // ��ʼ������
    snDel *set = (snDel *)malloc(sizeof(snDel));
    struct stat statPath; // ���ڸ�stat��������
    switch(argc) {
        case 1:
            set->filePath = (char *)malloc(BlockSize);
            input("�ļ�·��: ", set->filePath);
            process(set, statPath);
            break;
        default:
            for(int x = 1; x < argc; ++x) {
                set->filePath = argv[x];
                process(set, statPath);
            }
            break;
    }
    free(set);
    return 0;
}
