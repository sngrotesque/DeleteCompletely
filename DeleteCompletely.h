#include <time.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <windows.h>
#include <sys/stat.h>

#define BlockSize (size_t)4096
#define RandFileNameText \
"#+0123456789=ABCDEFGHIJKLMNOPQRSTUVWXYZ_abcdefghijklmnopqrstuvwxyz"
#define fileSizeMB(x) (double)(x / (size_t)(1024 * 1024))
#define fileSizeKB(x) (double)(x / (size_t)1024)

typedef uint8_t u8;
struct count {
    size_t total;
    size_t surplus;
};
typedef struct {
    struct count ProcN;
    u8 *randData;
    char *filePath;
    size_t fileSize;
} snDel;

// * ��ȡ�ļ���С * //
static int64_t getFileSize(const char *fn)
{
    FILE *stream = fopen(fn, "rb");
    int64_t file_size = -1;
    int64_t cur_offset = ftello64(stream);
    fseeko64(stream, 0, SEEK_END);
    file_size = ftello64(stream);
    fseeko64(stream, cur_offset, SEEK_SET);
    fclose(stream);
    return file_size;
}

// * �ж��ļ��Ƿ���� * //
static bool exists(const char *fn)
{
    int result = access(fn, F_OK);
    if(result == 0) return true;
    else return false;
}

// * ��������� * //
static void setRandomSeed()
{
    clockid_t a = 0;
    struct timespec p = {0, 0};
    clock_gettime(a, &p);
    srand((unsigned)p.tv_nsec);
}

// * ��������� * //
static uint32_t randint(uint32_t min, uint32_t max)
{
    return rand() % (max - min + 1) + min;
}

// * ��ʼ������ * //
static void Initializer()
{
    setRandomSeed();
    system("chcp 936 > nul > nul");
}

// * ���뺯��(Ŀǰ�����ַ���) * //
static void input(const char *text, void *ptr)
{
    u8 inputBuffer;
    printf("<<<< %s", text);
    scanf("%[^\n]s", (u8 *)ptr);
    if (*((u8 *)ptr) != 0x0d && *((u8 *)ptr) != 0x0a) {
        scanf("%c", &inputBuffer);
    }
}

// * �������ļ���ɾ�� * //
static void RenameRemove(const char *oldName, const char *newName)
{
    char tempFilePath[BlockSize] = {0};
    char warningTitle[BlockSize] = {0};
    char warningText[BlockSize<<1] = {0};

    int fileNameLength = strlen(oldName);
    int x, slashIndex = EOF;
    int errCode[2], boxCode;

    for(x = 0; x < fileNameLength; ++x) {
        if(oldName[x] == '/' || oldName[x] == '\\')
            slashIndex = x;
    }
    memcpy(tempFilePath, oldName, slashIndex + 1);
    strcat(tempFilePath, newName);
    
    do {
        errCode[0] = rename(oldName, tempFilePath);
        errCode[1] = remove(tempFilePath);
        switch(errCode[0] | errCode[1]) {
            case EOF:
                sprintf(warningText,
                    "�ļ����ɱ��޸ģ������ļ��Ƿ���������ռ�û���ɾ����\n"
                    "������Ϣ:\n"
                    "    Դ�ļ�: %s\n"
                    "    Rename���صĴ������: %d\n"
                    "    Remove���صĴ������: %d\n"
                    "    ���ļ�: %s\n",
                    oldName,
                    errCode[0],
                    errCode[1],
                    tempFilePath);
                sprintf(warningTitle, "���Գ���%s�ľ��棡", __argv[0]);
                boxCode = MessageBoxA(NULL, warningText, warningTitle,
                    MB_ICONERROR | MB_RETRYCANCEL);
                break;
            default:
                boxCode = 2;
                break;
        }
    }while(boxCode == 4);
}

static void writeUselessData(snDel *set)
{
    FILE *fp = fopen(set->filePath, "wb");
    for(size_t x = 0; x < set->ProcN.total; ++x) {
        fwrite(set->randData, 1, BlockSize, fp);}
    fwrite(set->randData, 1, set->ProcN.surplus, fp);
    fclose(fp);
}

static void writeRemove(snDel *set)
{
    size_t serialNumber;
    size_t randIndex;
    size_t randFNIndex;
    size_t fileNameLength = 16;
    size_t randFileNameTextLen = strlen(RandFileNameText) - 1;
    char *randFileName = (char *)calloc(fileNameLength + 1, 1);

    for(serialNumber = 0; serialNumber < 3; ++serialNumber) {
        for(randIndex = 0; randIndex < BlockSize; ++randIndex) {
            set->randData[randIndex] = randint(0x00, 0xff);
        }
        writeUselessData(set);
    }
    FILE *fp = fopen(set->filePath, "wb");
    fwrite("", 1, 0, fp);
    fclose(fp);

    for(randFNIndex = 0; randFNIndex < fileNameLength; ++randFNIndex) {
        randFileName[randFNIndex] = RandFileNameText[randint(0, randFileNameTextLen)];
    }

    RenameRemove(set->filePath, randFileName);
}

