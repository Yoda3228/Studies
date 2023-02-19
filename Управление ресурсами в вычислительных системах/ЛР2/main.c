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
            if (strcmp(flist[j].name,tmplist.name)<0)
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
        printf("%s \t\n", flist[i].name);
    }
}

int main()
{
    char cwd[1024];
    DIR *cd;
    int n=0, m=0;
    struct filelist flist[256];
    struct dirent * cf;            // имя файла и номер индексного дескриптора
    struct stat fs;                // основные параметры файла
    if (cd = opendir(cwd))
    {
	while (cf = readdir(cd))
	{
		stat(cf->d_name, &fs);
		if (fs.st_mode & S_IFREG)
		{
			flist[n].name = cf->d_name;
			n++;
		}
		else if (fs.st_mode & S_IFDIR)
		{

        };
		

	};
        closedir(cd);
    }
    else printf("Error: %d\n", errno);
    csort(flist,n);
    printf("name \t\n");
    print(flist,n," ");
    return 0;
    };
