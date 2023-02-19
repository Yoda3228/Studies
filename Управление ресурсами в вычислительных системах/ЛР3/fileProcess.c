#include "fileProcess.h"

#include <stdbool.h>
#include <string.h>
#include <malloc.h>

#include <sys/stat.h>
#include <fcntl.h>

#define EQUAL     0		
#define ERROR    -1		

#define STD_FLAG -1		

#define FNAME_COLUMN 7			
#define INO_COLUMN  1000000		


void memoryInit(Memory* memory, uint32_t tablesize_max) {
	memory->ft_size = 0;
	memory->oft_size = 0;
	memory->fdt_size = 0;

	memory->ft = (FileTable*)malloc(sizeof(FileTable) * tablesize_max);
	memory->oft = (OpenFileTable*)malloc(sizeof(OpenFileTable) * tablesize_max);
	memory->fdt = (FileDescTable*)malloc(sizeof(FileDescTable) * tablesize_max);
}


void memoryFree(Memory* memory) {
	memory->ft_size = 0;
	memory->oft_size = 0;
	memory->fdt_size = 0;

	free(memory->ft);
	free(memory->oft);
	free(memory->fdt);

	memory->ft = NULL;
	memory->oft = NULL;
	memory->fdt = NULL;
}
static void fileTableElemInit(FileTable* table_elem, const char* filename, ino_t ino, int flags) {
	table_elem->filename = filename;	
	table_elem->ino = ino;				
	table_elem->flags = flags;			
	table_elem->nlink = 1;				
}


static void openFileTableElemInit(OpenFileTable* table_elem, const char* filename, int fd, int flags) {
	table_elem->filename = filename;	
	table_elem->fd = fd;				
	table_elem->flags = flags;			
}

static void fileDescTableElemInit(FileDescTable* table_elem, const char* filename, const struct stat* st) {
	table_elem->filename = filename;			
	table_elem->dev = st->st_dev;				
	table_elem->ino = st->st_ino;				
	table_elem->mode = st->st_mode;				
	table_elem->nlink = st->st_nlink;			
	table_elem->uid = st->st_uid;				
	table_elem->gid = st->st_gid;				
	table_elem->rdev = st->st_rdev;				
	table_elem->size = st->st_size;				
	table_elem->blksize = st->st_blksize;		
	table_elem->blocks = st->st_blocks;			
	table_elem->atime = st->st_atime;			
	table_elem->mtime = st->st_mtime;			
	table_elem->ctime = st->st_ctime;			
}

static const char* getSTDName(int std_fd) {
	const char* std_name;

	switch (std_fd) {
	case STDIN_FILENO:  std_name = "stdin";  break;
	case STDOUT_FILENO: std_name = "stdout"; break;
	case STDERR_FILENO: std_name = "stderr"; break;
	}

	return std_name;
}

static const char* getStrFlag(int flag) {
	const char* str_flag;

	switch (flag) {
	case 0: str_flag = "RD"; break;
	case 1: str_flag = "WR"; break;
	case 2: str_flag = "RDWR"; break;
	default: str_flag = " ___"; break;
	}

	return str_flag;
}


int myOpen(Memory* memory, const char* pathname, int flags) 
{
	int fd = open(pathname, flags);		
	uint32_t i;
	if (fd == ERROR)
		fprintf(stderr, "Error: cannot open %s!\n", pathname);
	else {
		struct stat st;

		if (stat(pathname, &st) == ERROR)	
			fprintf(stderr, "Error: stat failure!\n");
		else {

			bool exist = false;
			for (i = 0; i != memory->ft_size && !exist; ++i)
				if (memory->ft[i].ino == st.st_ino && memory->ft[i].flags == flags) {
					++memory->ft[i].nlink;
					exist = true;
				}

			if (!exist || memory->ft_size == 0)
				fileTableElemInit(&memory->ft[memory->ft_size++], pathname, st.st_ino, flags);

			openFileTableElemInit(&memory->oft[memory->oft_size++], pathname, fd, flags);

			exist = false;
			for (i = 0; i != memory->fdt_size && !exist; ++i)
				if (memory->fdt[i].ino == st.st_ino) {
					++memory->fdt[i].nlink;
					exist = true;
				}

			if (!exist || memory->fdt_size == 0)
				fileDescTableElemInit(&memory->fdt[memory->fdt_size++], pathname, &st);
		}
	}

	return fd;		
}

int mySTDOpen(Memory* memory, int std_fd) {
	int fd = ERROR;

	uint32_t i;
	bool found = false;
	for (i = 0; i != memory->oft_size && !found; ++i)
		if (memory->oft[i].fd == std_fd)
			found = true;

	if (!found) {
		struct stat st;
		if (fstat(std_fd, &st) == ERROR)
			fprintf(stderr, "Error: fstat failure!\n");
		else {
			fd = std_fd;						
			const char* std_name = getSTDName(fd);

			fileTableElemInit(&memory->ft[memory->ft_size++], std_name, st.st_ino, STD_FLAG);
			openFileTableElemInit(&memory->oft[memory->oft_size++], std_name, std_fd, STD_FLAG);
			fileDescTableElemInit(&memory->fdt[memory->fdt_size++], std_name, &st);
		}
	}

	return fd;	
}


int myClose(Memory* memory, int fd) {
	const char* filename;
	int flags;
	uint32_t i, j;
	bool found = false;
	for (i = 0; i != memory->oft_size && !found; ++i)
		if (memory->oft[i].fd == fd) {
			filename = memory->oft[i].filename;
			flags = memory->oft[i].flags;

			for ( j = i; j != memory->oft_size - 1; ++j)
				memory->oft[j] = memory->oft[j + 1];
			--memory->oft_size;

			found = true;
		}


	found = false;
	for (i = 0; i != memory->ft_size && !found; ++i)
		if (strcmp(memory->ft[i].filename, filename) == EQUAL && memory->ft[i].flags == flags) {
			if (memory->ft[i].nlink - 1 != 0)
				--memory->ft[i].nlink;
			else {
				for (j = i; j != memory->ft_size - 1; ++j)
					memory->ft[j] = memory->ft[j + 1];
				--memory->ft_size;
			}

			found = true;
		}

	found = false;
	for ( i = 0; i != memory->fdt_size && !found; ++i) {
		if (strcmp(memory->fdt[i].filename, filename) == EQUAL) {
			if (memory->fdt[i].nlink - 1 != 0)
				--memory->fdt[i].nlink;
			else {
				for ( j = i; j != memory->fdt_size - 1; ++j)
					memory->fdt[j] = memory->fdt[j + 1];
				--memory->fdt_size;
			}

			found = true;
		}
	}

	return close(fd);		
}

int mySTDClose(Memory* memory, int std_fd) {
	int close = ERROR;
	bool found = false;
	uint32_t i, j;
	for (i = 0; i != memory->oft_size && !found; ++i) {
		if (memory->oft[i].fd == std_fd) {
			for (j = i; j != memory->oft_size - 1; ++j)
				memory->oft[j] = memory->oft[j + 1];
			--memory->oft_size;

			found = true;
		}
	}
	return close;	
}

void printTables(Memory* memory) {
	uint32_t i;
	puts("\nOpen file table");			
	puts("________________________________________");
	printf("Filename\t| File desc\t| Flag\t|\n");
	printf("================|===============|=======|\n");
	for ( i = 0; i != memory->oft_size; ++i)
		strlen(memory->oft[i].filename) < FNAME_COLUMN ?
		printf("%s\t\t| %d\t\t| %s\t|\n", memory->oft[i].filename, memory->oft[i].fd, getStrFlag(memory->oft[i].flags)) :
		printf("%s\t| %d\t\t| %s\t|\n", memory->oft[i].filename, memory->oft[i].fd, getStrFlag(memory->oft[i].flags));
	puts("________________|_______________|_______|");

	puts("\nFile table");				
	puts("________________________________________________");
	printf("Filename\t| Inode\t\t| Flag\t| NLink |\n");
	printf("================|===============|=======|=======|\n");
	for ( i = 0; i != memory->ft_size; ++i)
		memory->ft[i].ino < INO_COLUMN ?
		printf("%s\t\t| %zd\t\t| %s \t| %zd\t|\n",
			memory->ft[i].filename, memory->ft[i].ino, getStrFlag(memory->ft[i].flags), memory->ft[i].nlink) :
		printf("%s\t| %zd\t| %s \t| %zd\t|\n",
			memory->ft[i].filename, memory->ft[i].ino, getStrFlag(memory->ft[i].flags), memory->ft[i].nlink);
	puts("________________|_______________|_______|_______|");

	puts("\nFile descriptor table");	
	puts("________________________________________________________");
	printf("Filename\t| Inode\t\t| NLink | Mode\t | Size\t|\n");
	printf("================|===============|=======|========|======|\n");
	for ( i = 0; i != memory->fdt_size; ++i)
		memory->fdt[i].ino < INO_COLUMN ?
		printf("%s\t\t| %zd\t\t| %zd\t| %o  | %li\t|\n",
			memory->fdt[i].filename, memory->fdt[i].ino, memory->fdt[i].nlink, memory->fdt[i].mode, memory->fdt[i].size) :
		printf("%s\t| %zd\t| %zd\t| %o | %li\t|\n",
			memory->fdt[i].filename, memory->fdt[i].ino, memory->fdt[i].nlink, memory->fdt[i].mode, memory->fdt[i].size);
	puts("________________|_______________|_______|________|______|");
}
