#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>

/**
 *  структура записи о файле
 *   */
struct filelist
{
    char *name;     // имя файла
    int filesize;   // размер файла
};

/**
 *   сортировка
 *    */
void csort(struct filelist flist[256], int n)
{
    int i, j, min;
    struct filelist tmplist;
    for (i=0;i<n-1;i++)     // цикл по записям
    {
        tmplist=flist[i];
        min=i;
        for (j=i;j<n;j++)
        {
            if (flist[j].filesize<tmplist.filesize)
            {
                tmplist=flist[j];
                min=j;
            };
        };
        flist[min]=flist[i];
        flist[i]=tmplist;
    }
}
/**
 *     вывод списка
 *      */
void print(struct filelist flist[256], int n, char *type)
{
    int i;
    for (i=0;i<n;i++)
    {
        if(strcmp(flist[i].name,"..")!=0 && strcmp(flist[i].name,".")!=0)
        printf ("%-20s%-20i%-20s\n",flist[i].name, flist[i].filesize, type);
        else
        printf ("%-20s%-20s%-20s\n",flist[i].name,"", type);
    }
}

int main(int argc, char *argv[])
{
    char cwd[1024];
    DIR *cd;
    int n=0, m=0;
    struct filelist flist[256], dirlist[256];
    struct dirent * cf;            // имя файла и номер индексного дескриптора
    struct stat fs;                // основные параметры файла
    if(argc > 1)
    {
        strcpy(cwd,argv[1]);
    }else{
        getcwd (cwd, sizeof(cwd));  // текущая директория
    }
    strcat (cwd, "/");            // добавление '/' в конец имени каталога
    if (cd=opendir(cwd))        // открытие текущей директории
    {
        while(cf=readdir(cd))   // чтение текущей директории
        {
            stat(cf->d_name, &fs);
            if (fs.st_mode&S_IFREG) // записываем в массив файл (не каталог)
            {
                flist[n].name=cf->d_name;
                flist[n].filesize=fs.st_size;
                n++;
            }
            else if (fs.st_mode&S_IFDIR) // записываем в массив каталог
            {
                dirlist[m].name=cf->d_name;
                if(strcmp(dirlist[m].name,"..")!=0 && strcmp(dirlist[m].name,".")!=0)
                    dirlist[m].filesize=fs.st_size;
                else
                dirlist[m].filesize=-1;
                m++;
            };
        };
        closedir(cd);
    }
    else
    {
        printf("Error: %d\n", errno);    // не удалось открыть каталог
        return 1;
    }
    csort(dirlist,m); //сортировка папок
    csort(flist,n);                    // сортировка
    printf("%-20s%-20s%-20s\n","name","size","type");// печать шапки
    print(dirlist,m,"d");              // список директорий
    print(flist,n,"f");              // список в файл
    return 0;
}

