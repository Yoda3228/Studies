#ifndef FILEPROCESS_H
#define FILEPROCESS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <unistd.h>
#include <fcntl.h>

typedef struct FileTable {
	const char* filename;	// Название файла
	ino_t ino;				// Номер inode
	int flags;				// Режим открытия файла
	nlink_t nlink;			// Количество ссылок
} FileTable;

typedef struct OpenFileTable {
	const char* filename;	// Название файла
	int fd;					// Описатель файла
	int flags;				// Флаги файла
} OpenFileTable;

typedef struct FileDescTable {
	const char* filename;	
	dev_t dev;				
	ino_t ino;				
	mode_t mode;			
	nlink_t nlink;			
	uid_t uid;				
	gid_t gid;				
	dev_t rdev;				
	off_t size;				
	blksize_t blksize;		
	blkcnt_t blocks;		
	time_t atime;			
	time_t mtime;			
	time_t ctime;			
} FileDescTable;

typedef struct Memory {
	FileTable* ft;		
	OpenFileTable* oft;	
	FileDescTable* fdt;	

	uint32_t ft_size;	
	uint32_t oft_size;	
	uint32_t fdt_size;	
} Memory;

void memoryInit(Memory* memory, uint32_t tablesize_max);
void memoryFree(Memory* memory);

int myOpen(Memory* memory, const char* pathname, int flags);
int mySTDOpen(Memory* memory, int std_fd);

int myClose(Memory* memory, int fd);
int mySTDClose(Memory* memory, int std_fd);

void printTables(Memory* memory);

#endif
