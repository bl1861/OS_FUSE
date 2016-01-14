/*
  FUSE: Filesystem in Userspace
  Copyright (C) 2001-2007  Miklos Szeredi <miklos@szeredi.hu>

  This program can be distributed under the terms of the GNU GPL.
  See the file COPYING.

*/


#define FUSE_USE_VERSION 26

#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>

/* DIR, dirent are defined in dirent.h */
#include <dirent.h>

/* atoi is defined in stdlib.h */
#include <stdlib.h>

/* the path of process directory */
static const char *proc_path = "/proc";

/* assume total number of file is less than 300 and the path size is less than 50
* filename[n] is /pid;	
*/
char filename[300][50];

/* record number of processes in /proc */
int numProc = 0;


/* record the PID in the system and return the number of processes in system */
int record_pid(){
	DIR *dir;
	struct dirent *ent;
	
	numProc = 0;	
	if((dir = opendir(proc_path)) != NULL){
		while((ent = readdir(dir)) != NULL){
			int value = atoi(ent->d_name);
			if(value == 0){
				continue;
			}
			if(ent->d_type == DT_DIR){
				strcpy(filename[numProc], ent->d_name);
				numProc++;
			}
		}	
	}	
	else{
		perror("");
		return -1;
	}
	return numProc;
}

/* to check if *path is /pid */
int check_proc(const char *path){
	int i;
	for(i = 0; i < numProc; i++){
		if(strcmp(path, filename[i]) == 0){
			return 0;
		}
	}
	return 1;
}

/* If the *path is the path of a process, return the size of the process status*/
size_t fsize(const char *path){
	FILE *fp;
	char str[50];
	char content[100000];
	sprintf(str, "%s%s%cstatus", proc_path, path, 47); 	
	size_t len = 0;
	fp = fopen(str, "r");
	if(fp != NULL){
		len = fread(content, sizeof(char), 100000, fp);
		fclose(fp);
	}
	else{
		printf("Can't open %s\n", str);
	}
	return len;
}


static int hello_getattr(const char *path, struct stat *stbuf)
{
	int res = 0;

	memset(stbuf, 0, sizeof(struct stat));

	if (strcmp(path, "/") == 0) {
		stbuf->st_mode = S_IFDIR | 0755;
		stbuf->st_nlink = 2;
	} 
	else if (check_proc(path + 1) == 0){
		size_t len = fsize(path);

		/* set process read_only */
		stbuf->st_mode = S_IFREG | 0444;
		stbuf->st_nlink = 1;
		stbuf->st_size = len;
	}
	else
		res = -ENOENT;

	/* set the total blocks of process*/
	stbuf->st_blocks = stbuf->st_size % 1024 == 0? stbuf->st_size / 1024: stbuf->st_size / 1024 + 1;

	return res;
}



static int hello_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
			 off_t offset, struct fuse_file_info *fi)
{
	(void) offset;
	(void) fi;

	if (strcmp(path, "/") != 0)
		return -ENOENT;

	filler(buf, ".", NULL, 0);
	filler(buf, "..", NULL, 0);

	/* put PIDs in the filler */
	int num = record_pid();
	int i;
	for(i = 0; i < num; i++){
		filler(buf, filename[i], NULL, 0);
	}

	return 0;
}


static int hello_open(const char *path, struct fuse_file_info *fi)
{
	/* user can only open /pid */
	if (check_proc(path + 1) != 0)
		return -ENOENT;

	if ((fi->flags & 3) != O_RDONLY)
		return -EACCES;

	return 0;
}

static int hello_read(const char *path, char *buf, size_t size, off_t offset,
		      struct fuse_file_info *fi)
{
	size_t len;

	(void) fi;


	if(check_proc(path + 1) == 0){

		/* assume the length of path is less than 50 */
		char path_buffer[50] = "";
		sprintf(path_buffer, "%s%s%cstatus", proc_path, path, 47);

		FILE *fp;
		fp = fopen(path_buffer,"r");
		if(fp != NULL){
			size_t length = 1000;
			char *line = NULL;
			int read;

			/* assume size of line in file is less than 10000 */
			char content[10000] = "";
			while((read = getline(&line, &length, fp)) != -1){
				printf("%s\n", line);
				strcat(content, line);
			}

			len = strlen(content);
			if (offset < len) {
				if (offset + size > len)
					size = len - offset;
				memcpy(buf, content + offset, size);
			} 
			else
				size = 0;
		}
		else{
			printf("can't open file %s", path_buffer);
		}
	
	} 
	else{
		return -ENOENT;
	}

	return size;
}


static struct fuse_operations hello_oper = {
	.getattr	= hello_getattr,
	.readdir	= hello_readdir,
	.open		= hello_open,
	.read		= hello_read,
};


int main(int argc, char *argv[])
{
	return fuse_main(argc, argv, &hello_oper, NULL);
}




