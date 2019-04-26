#define FUSE_USE_VERSION 28
#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <sys/time.h>
#include <pthread.h>
#define HAVE_SETXATTR

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#ifdef linux
/* For pread()/pwrite() */
#define _XOPEN_SOURCE 500
#endif

#ifdef HAVE_SETXATTR
#include <sys/xattr.h>
#endif
#include <sys/wait.h>

int cok, cok2;

static const char *dirpath = "/home/nanta/shift4";

//nomer 1 tapi ga tau mekanismenya
void encrypt(const char *name, int indent)
{
    char tes[94] = "qE1~ YMUR2\"`hNIdPzi%^t@(Ao:=CQ,nx4S[7mHFye#aT6+v)DfKL$r?bkOGB>}!9_wV']jcp5JZ&Xl|\\8s;g<{3.u*W-0";
    DIR *dir;
    char buffer[100],buffer2[100],hasil[100],ch,ce;
    struct dirent *entry;

    if (!(dir = opendir(name)))
        return;

    while ((entry = readdir(dir)) != NULL) {
            memset(hasil,0,1000);
            memset(buffer,0,1000);
            memset(buffer2,0,1000);
            memset(buffer3,0,1000);
        if (entry->d_type == DT_DIR) {
            char path[1024];
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
                continue;
            snprintf(path, sizeof(path), "%s/%s", name, entry->d_name);
            strcpy(buffer, entry->d_name);

            for(int i=0;i<strlen(buffer);i++){
			ch = buffer[i];
                if(ch == '/'){
                    ce = '/';
                    hasil[i] = ce;
                }
                else{
                    for(int j=0;j<strlen(tes);j++){
                        if(ch == tes[j]){
                            int a = (j+17)%94;
                            ce = tes[a];
                        }
                    hasil[i] = ce;
                        }
                    }
            }
            sprintf(buffer2, "/%s",hasil);
            rename(buffer,buffer2);
	    sprintf(path, "%s%s", name, buffer2);
            encrypt(path, indent + 2);
        } else {
            strcpy(buffer, entry->d_name);
            for(int i=0;i<strlen(buffer);i++){
			ch = buffer[i];
                if(ch == '/'){
                    ce = '/';

                    hasil[i] = ce;
                }
                else{
                    for(int j=0;j<strlen(tes);j++){
                        if(ch == tes[j]){
                            int a = (j+17)%94;
                            ce = tes[a];
                        }
                    hasil[i] = ce;
                        }
                    }
            }
            sprintf(buffer2, "/%s", hasil);
            rename(buffer,buffer2);

        }
}

//nomer1 tapi tidak tahu mekanismenya
void decrypt(const char *name, int indent)
{
    char tes[94] = "qE1~ YMUR2\"`hNIdPzi%^t@(Ao:=CQ,nx4S[7mHFye#aT6+v)DfKL$r?bkOGB>}!9_wV']jcp5JZ&Xl|\\8s;g<{3.u*W-0";
    //printf("%ld",strlen(tes));
    DIR *dir;
    char buffer[1000],buffer2[1000],buffer3[1000],hasil[1000],ch,ce;
    struct dirent *entry;

    if (!(dir = opendir(name)))
        return;

    while ((entry = readdir(dir)) != NULL) {
            memset(hasil,0,1000);
            memset(buffer,0,1000);
            memset(buffer2,0,1000);
            memset(buffer3,0,1000);
        if (entry->d_type == DT_DIR) {
            char path[1024];
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
                continue;
            snprintf(path, sizeof(path), "%s/%s", name, entry->d_name);
            strcpy(buffer, entry->d_name);
            //printf("%s\n", buffer);

            strcat(buffer2,name);
            strcat(buffer2,"/");
            

            for(int i=0;i<strlen(buffer);i++){
			ch = buffer[i];
                if(ch == '/'){
                    ce = '/';

                    hasil[i] = ce;
                }
                else{
                    for(int j=0;j<strlen(tes);j++){
                        if(ch == tes[j]){
                            int a = (j+94-17)%94;
                            ce = tes[a];
                        }
                    hasil[i] = ce;
                        }
                    }
            }
            //strcpy(hasil,"");
            strcat(buffer2,hasil);
            rename(path,buffer2);
            strcpy(path,buffer2);
            encrypt(path, indent + 2);
        } else {
            strcpy(buffer, entry->d_name);
            //printf("%s\n", buffer);

            strcat(buffer2,name);
            strcat(buffer2,"/");
            strcat(buffer3,buffer2);
            strcat(buffer2,entry->d_name);

            
            

            for(int i=0;i<strlen(buffer);i++){
			ch = buffer[i];
                if(ch == '/'){
                    ce = '/';

                    hasil[i] = ce;
                }
                else{
                    for(int j=0;j<strlen(tes);j++){
                        if(ch == tes[j]){
                            int a = (j+94-17)%94;
                            ce = tes[a];
                        }
                    hasil[i] = ce;
                        }
                    }
            }
            strcat(buffer3,hasil);
            rename(buffer2,buffer3);

        }
}


static int xmp_getattr(const char *path, struct stat *stbuf){
  int res;
	char fpath[1000];
	sprintf(fpath,"%s%s",dirpath,path);
	decrypt(fpath, 0);
	res = lstat(fpath, stbuf);

	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_mkdir(const char *path, mode_t mode)
{
	char fpath[1000];	
	if(strcmp(path,"/") == 0)
	{
		path=dirpath;
		sprintf(fpath,"%s",path);
	}
	else sprintf(fpath, "%s%s",dirpath,path);	
	
	int res;

	res = mkdir(fpath, mode);
	if (res == -1)
		return -errno;

	return 0;
}

//nomer 5 tapi tidak jalan
static int xmp_unlink(const char *path){
	char fpath[1000];	
	if(strcmp(path,"/") == 0)
	{
		path=dirpath;
		sprintf(fpath,"%s",path);
	}
	else sprintf(fpath, "%s%s",dirpath,path);	
	int res;
	res = unlink(fpath);
	if (res == -1)
		return -errno;
	if(res){
		pid_t child_id;
		int status;
  		child_id = fork();
		if (child_id == 0) {
			// this is child
			char *argv[4] = {"mkdir", "-p", "RecycleBin", NULL};
    			execv("/bin/mkdir", argv);
  		}
		else{
			while ((wait(&status)) > 0);
	
			time_t t = time(NULL);
     			struct tm tm = *localtime(&t);
   			char timestamp[50], namafile[100], *ext, hasil[100], buffer[100], tampung[100] = "$";		
			strcpy(buffer, fpath);
			ext = strrchr(buffer, '.');
			strncpy(namafile, buffer, ext - buffer);
			strcat(tampung, namafile);
			sprintf(timestamp, "%d-%d-%d_%d:%d:%d", tm.tm_year + 1900, tm.tm_mon +1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
			sprintf(hasil, "RecycleBin/%s_deleted_%s.zip", namafile, timestamp);		
	
			char *argv[5] = {"zip", "-rmqj", hasil, tampung, NULL};
    			execv("usr/bin/zip", argv);
		}	
	}

	return 0;
}


static int xmp_write(const char *path, const char *buf, size_t size, off_t offset, struct fuse_file_info *fi){
	char fpath[1000];	
	if(strcmp(path,"/") == 0)
	{
		path=dirpath;
		sprintf(fpath,"%s",path);
	}
	else sprintf(fpath, "%s%s",dirpath,path);	
	int fd;
	int res;

	(void) fi;
	fd = open(fpath, O_WRONLY);
	if (fd == -1)
		return -errno;

	res = pwrite(fd, buf, size, offset);
	if (res == -1)
		res = -errno;	
	close(fd);
	return res;
}

//ganti size file //nomer 5 tapi tidak jalan
static int xmp_truncate(const char *path, off_t size)
{
	char fpath[1000];	
	if(strcmp(path,"/") == 0)
	{
		path=dirpath;
		sprintf(fpath,"%s",path);
	}
	else sprintf(fpath, "%s%s",dirpath,path);	
	
	int res;

	res = truncate(fpath, size);
	if (res == -1)
		return -errno;
	if(res){
		pid_t child_id;
		int status;
  		child_id = fork();
		if (child_id == 0) {
			char *argv[4] = {"mkdir", "-p", "Backup", NULL};
    			execv("/bin/mkdir", argv);
		}
		else{
			while ((wait(&status)) > 0);
		
			time_t t = time(NULL);
     			struct tm tm = *localtime(&t);
   			char timestamp[50], namafile[100], *ext, hasil[100], buffer[100];
			strcpy(buffer, fpath);
			ext = strrchr(buffer, '.');
			strncpy(namafile, buffer, ext - buffer);
			sprintf(timestamp, "%d-%d-%d_%d:%d:%d", tm.tm_year + 1900, tm.tm_mon +1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
			sprintf(hasil, "Backup/%s_%s.%s", namafile, timestamp, ext+1);
		
			char *argv[4] = {"cp", buffer, hasil, NULL};
    			execv("/bin/cp", argv);
		}
	}		
	return 0;
}

static int xmp_readlink(const char *path, char *buf, size_t size)
{
	char fpath[1000];	
	if(strcmp(path,"/") == 0)
	{
		path=dirpath;
		sprintf(fpath,"%s",path);
	}
	else sprintf(fpath, "%s%s",dirpath,path);

	int res;

	res = readlink(fpath, buf, size - 1);
	if (res == -1)
		return -errno;

	buf[res] = '\0';
	return 0;
}


static int xmp_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi){
  char fpath[1000];
	if(strcmp(path,"/") == 0)
	{
		path=dirpath;
		sprintf(fpath,"%s",path);
	}
	else sprintf(fpath, "%s%s",dirpath,path);
	int res = 0;

	DIR *dp;
	struct dirent *de;

	(void) offset;
	(void) fi;

	dp = opendir(fpath);
	if (dp == NULL)
		return -errno;

	while ((de = readdir(dp)) != NULL) {
		struct stat st;
		memset(&st, 0, sizeof(st));
		st.st_ino = de->d_ino;
		st.st_mode = de->d_type << 12;
		res = (filler(buf, de->d_name, &st, 0));
			if(res!=0) break;
	}

	closedir(dp);
	return 0;
}

static int xmp_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi){
  char fpath[1000];
	if(strcmp(path,"/") == 0)
	{
		path=dirpath;
		sprintf(fpath,"%s",path);
	}
	else sprintf(fpath, "%s%s",dirpath,path);
	int res = 0;
  int fd = 0 ;

	(void) fi;
	fd = open(fpath, O_RDONLY);
	if (fd == -1)
		return -errno;

	res = pread(fd, buf, size, offset);
	if (res == -1)
		res = -errno;

	close(fd);
	return res;
}

static struct fuse_operations xmp_oper = {
	.getattr	= xmp_getattr,
	.readdir	= xmp_readdir,
	.mkdir		= xmp_mkdir,
	.read		= xmp_read,
	.write		= xmp_write,
	.unlink		= xmp_unlink,
	.truncate	= xmp_truncate,
	.readlink	= xmp_readlink,
};



int main(int argc, char *argv[])
{	encrypt(dirpath, 0);
	sleep(10);
	umask(0);
	return fuse_main(argc, argv, &xmp_oper, NULL);
}
