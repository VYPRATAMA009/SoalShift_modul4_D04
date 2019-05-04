#define FUSE_USE_VERSION 30
#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <sys/time.h>
#include <grp.h>
#include <pwd.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <sys/wait.h>

static const char *dirpath = "/home/nanta/shift4";
int flag;
char hasildekrip[100];
char hasilenkrip[100];

//nomer 1 enkripsi di direktori utama
void encrypt(const char *text){
	char tes[94] = "qE1~ YMUR2\"`hNIdPzi%^t@(Ao:=CQ,nx4S[7mHFye#aT6+v)DfKL$r?bkOGB>}!9_wV']jcp5JZ&Xl|\\8s;g<{3.u*W-0";
	char ch, ce, buffer[100], buffer2[100], hasil[100];
	int i, j;
	DIR *dir;		//dir sama rent itu untuk open direktori
	struct dirent *rent;

	if(!(dir = opendir(text))){
		return;
	}
	while((rent = readdir(dir)) != NULL){
		if(rent->d_type == DT_DIR){	//jika direktori itu tipenya folder (direktori)
			char path[1024];	
			memset(buffer, '\0', 100);	// kosongkan array buffer
			memset(buffer2, '\0', 100);// kosongkan array buffer2
			memset(hasil, '\0', 100);	// kosongkan array hasil
			memset(hasilenkrip, '\0', 100);
			if (strcmp(rent->d_name, ".") == 0 || strcmp(rent->d_name, "..") == 0) //jika namanya itu diabaikan karena bukan folder
                		continue;
			snprintf(path, sizeof(path), "%s/%s", text, rent->d_name);	//kita tampung alamat dari direktori shift4 dan nama folder-nya di path yang nantinya akan dienkripsi
			strcpy(buffer, rent->d_name);	//kita tampung nama folder pada buffer
			strcat(buffer2, text);		//kita tempel nama dari alamat direktori shift4 di buffer 2
			strcat(buffer2, "/");		//kita tempel tanda "/" di buffer2
			
			//algoritma caesar chiper dengan list dan key pada soal, membandingkan character pada array buffer dengan list, lalu di enkripsi, dan hasilnya ditampung pada array hasil
			for(i=0;i<strlen(buffer);i++){	
				ch = buffer[i];
                		if(ch == '/'){
                    			ce = '/';
                    			hasil[i] = ce;
					hasilenkrip[i] = ce;
		    			continue;
                		}
                		else{
                    			for(j=0;j<strlen(tes);j++){
                        			if(ch == tes[j]){
                            				int a = (j+17)%94;
                            				ce = tes[a];
                        			}
                    				hasil[i] = ce;
						hasilenkrip[i] = ce;
                    			}
               			}
            		}
			strcat(buffer2, hasil);	//setelah hasil enkripsi ditampung di array hasil, selanjutnya buffer2 kita tempel dengan array hasil
			rename(path, buffer2); //lalu kita rename direktori pada path dengan array buffer2
			snprintf(path, sizeof(path), "%s/%s", text, hasil); //lalu kita mengganti nama direktori dari path yang lama dengan enkripsi dari array hasil yang nantinya akan dicek dengan rekursif
			encrypt(path);	//pengecekan rekursif			
		}
		else{		//jika direktori itu tipe-nya adalah file maka akan langsung dienkripsi
			char path[1024];
			snprintf(path, sizeof(path), "%s/%s", text, rent->d_name);	//sama seperti yang di atas algoritmanya
			strcpy(buffer, rent->d_name);
			strcat(buffer2, text);
			strcat(buffer2, "/");

			for(i=0;i<strlen(buffer);i++){
				ch = buffer[i];
                		if(ch == '/'){
                    			ce = '/';
                    			hasil[i] = ce;
					hasilenkrip[i] = ce;
		    			continue;
                		}
                		else{
                    			for(j=0;j<strlen(tes);j++){
                        			if(ch == tes[j]){
                            				int a = (j+17)%94;
                            				ce = tes[a];
                        			}
                    				hasil[i] = ce;
						hasilenkrip[i] = ce;
                    			}
               			}
            		}
			strcat(buffer2, hasil);
			rename(path, buffer2);
		}
	}
closedir(dir);		
}

//nomer 1 dekripsi pada direktori mount 
void decrypt(char *text){
	char tes[94] = "qE1~ YMUR2\"`hNIdPzi%^t@(Ao:=CQ,nx4S[7mHFye#aT6+v)DfKL$r?bkOGB>}!9_wV']jcp5JZ&Xl|\\8s;g<{3.u*W-0";
	char ch, ce;
	for(int i=0;i<strlen(text);i++){
		ch = text[i];
                if(ch == '/'){
                    ce = '/';
                    hasildekrip[i] = ce;
		    continue;
                }
                else{
                    for(int j=0;j<strlen(tes);j++){
                        if(ch == tes[j]){
                            int a = (j+94-17)%94;
                            ce = tes[a];
                        }
                    	hasildekrip[i] = ce;
                    }
               }
            }
}

//nomer 4 youtuber
void* ekstension(void *ptr)
{
	char *path = (char*)ptr;
	if (strstr(path, "/YOUTUBER") != NULL)	//mencari string YOUTUBER pada path, jika ada maka
	{
		while(flag==0){}		//menunggu flag = 1
		char oldname[1100];
		char newname[1100];
		sprintf(oldname, "%s%s",dirpath, path);		//menampung char oldname dengan dirpath dan path yang ada string YOUTUBER
		strcpy(newname,oldname);			// copy string oldname ke new name
		char ext[] = ".iz1";				//buat extensi ".iz1" pada char *ext
		//encrypt(ext,17);
		strcat(newname, ext);				//tempel extensi pada newname
		rename(oldname, newname);			//rename oldname dengan newname
	}
	return NULL;
}


static int xmp_getattr(const char *path, struct stat *stbuf)
{
  	int res;
	char fpath[1000];
	sprintf(fpath,"%s%s",dirpath, path);

	res = lstat(fpath, stbuf);

	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
		       off_t offset, struct fuse_file_info *fi)
{  	
	char fpath[1000];
	sprintf(fpath, "%s%s",dirpath, path);
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
		

		//nomer 1 dekrip
		char buffer[100];
		memset(buffer, '\0', 100);
		strcpy(buffer, de->d_name);

		//nomer3
		gid_t gid;
		uid_t uid;
		char pathnow[1000];
		strcpy(pathnow, fpath);
		strcpy(pathnow+strlen(pathnow),buffer);
		
		stat(pathnow,&st); //mendapatkan group dan owner dari pathnow

		gid = st.st_gid;
		uid = st.st_uid;

		struct group *grp;
		struct passwd *usr;
		grp = getgrgid(gid);
		usr = getpwuid(uid);
		
		//cek jika group dan owner sama dengan soal, maka akan dibuat filemiris.txt yang menyimpan namafile, group id, owner dan waktu akses terakhir
		if((strcmp(usr->pw_name, "chipset") == 0 || strcmp(usr->pw_name, "ic_controller") == 0) && strcmp(grp->gr_name, "rusak") == 0){
			FILE *cek, *target;
			cek = fopen(pathnow,"r");
			if(cek==NULL){
				target = fopen("/home/nanta/shift4/filemiris.txt","w");
				if(target==NULL){
					struct tm mod = *localtime(&(st.st_atime));	//mendapatkan waktu akses terakhir
					char isiFIle[1100];				
					sprintf(isiFIle, "%s %d %d %d:%d:%d %d-%d-%d", buffer,gid,uid,mod.tm_hour,mod.tm_min,mod.tm_sec,mod.tm_mday,mod.tm_mon,mod.tm_year+1900);	//tampung waktu akses di isiFile
				fputs(isiFIle,target);			//isikan target dengan waktu akses
				fputs(usr->pw_name, target);		//isi target dengan owner
				fputs(grp->gr_name, target);		//isi target dengan group
				fclose(target);	
				//printf("ini isinya cuk : %s\n\n\n", isiFIle);
				remove(pathnow);
				}
			}
			fclose(cek);
		}
		
		if((strcmp(buffer,".")!=0) && (strcmp(buffer,"..")!=0)) {
			memset(hasildekrip, '\0', 100);			
			decrypt(buffer);		//fungsi dekrip dipanggil di readdir, tapi pas dibuka di explorer, empty, tapi di ls keluar
			res = (filler(buf, hasildekrip, &st, 0));
				if(res!=0) break;		
		}
		
	}

	closedir(dp);
	return 0;
}

//nomer4
static int xmp_create(const char *path, mode_t mode, struct fuse_file_info *fi)
{
	char fpath[1000];
	char buffer[1000];
	
	strcpy(buffer,path);
	
	if (strstr(buffer, "/YOUTUBER") != NULL) {	//jika pada buffer ada string YOUTUBER, maka mode (permission) menjadi 640
		mode = 640;
	}

	if(strcmp(path,"/") == 0)
	{
		sprintf(fpath,"%s",dirpath);
	}
	else{
		sprintf(fpath, "%s%s",dirpath,path);
	}
	
	int fd;
	
	fd = creat(fpath, mode);
	if (fd == -1)
		return -errno;
	close(fd);
	flag = 0;
	pthread_t tid;	//membuat thread untuk memanggil fungsi nomer4
	pthread_create(&tid, NULL, &ekstension, fpath);

	return 0;
}


static int xmp_read(const char *path, char *buf, size_t size, off_t offset,
		    struct fuse_file_info *fi)
{
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

//masih nomer4
static int xmp_utimens(const char *path, const struct timespec ts[2])
{
	char fpath[1000];
	
	if(strcmp(path,"/") == 0)
	{
		sprintf(fpath,"%s",dirpath);
	}
	else{
		sprintf(fpath, "%s%s",dirpath,path);
	}

	int res;

	res = utimensat(0, fpath, ts, AT_SYMLINK_NOFOLLOW);
	flag = 1;	
	if (res == -1)
		return -errno;

	return 0;
}

//masih nomer4
static int xmp_chmod(const char *path, mode_t mode)
{
	char fpath[1000];
	char buffer[1000];
	
	strcpy(buffer,path);

	int len = strlen(buffer);
	if (strstr(buffer, "/YOUTUBER/") != NULL && buffer[len - 1] == '1' && buffer[len - 2] == 'z' && buffer[len - 3] == 'i' && buffer[len - 4] == '.') 	{		//jika ekstensi file .iz1, maka tidak boleh dibuah permission-nya
    	printf("File ekstensi iz1 tidak boleh diubah permissionnya.\n");
    	return -1;
	}

	if(strcmp(path,"/") == 0)
	{
		sprintf(fpath,"%s",dirpath);
	}
	else{
		sprintf(fpath, "%s%s",dirpath, path);
	}

	int res;

	res = chmod(fpath, mode);
	if (res == -1)
		return -errno;

	return 0;
}

//nomer 5
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
	if(res){		//ketika file didelete maka akan membuat fork
		pid_t child_id;
		int status;
  		child_id = fork();
		if (child_id == 0) {
			// this is child	membuat folder RecycleBin
			char *argv[4] = {"mkdir", "-p", "RecycleBin", NULL};
    			execv("/bin/mkdir", argv);
  		}
		else{
			while ((wait(&status)) > 0);
	
			time_t t = time(NULL);
     			struct tm tm = *localtime(&t);
   			char timestamp[50], namafile[100], *ext, hasil[100], buffer[100], tampung[100] = "$";		
			strcpy(buffer, fpath);
			ext = strrchr(buffer, '.');	//mendapatkan extensi dari file
			strncpy(namafile, buffer, ext - buffer); //menghapus extensi dari file
			strcat(tampung, namafile);
			sprintf(timestamp, "%d-%d-%d_%d:%d:%d", tm.tm_year + 1900, tm.tm_mon +1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec); //mendapatkan timestamp
			sprintf(hasil, "RecycleBin/%s_deleted_%s.zip", namafile, timestamp);		
	
			char *argv[5] = {"zip", "-rmqj", hasil, tampung, NULL};		//zip file yang dihapus dan backupnya
    			execv("usr/bin/zip", argv);
		}	
	}

	return 0;
}

//nomer 5 ketika melakukan edit FIle
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
	if(res){	//ketika save telah diklik maka akan membuat fork
		pid_t child_id;
		int status;
  		child_id = fork();
		if (child_id == 0) {
			char *argv[4] = {"mkdir", "-p", "Backup", NULL};	//membuat folder backup
    			execv("/bin/mkdir", argv);
		}
		else{
			while ((wait(&status)) > 0);
		
			time_t t = time(NULL);
     			struct tm tm = *localtime(&t);
   			char timestamp[50], namafile[100], *ext, hasil[100], buffer[100];
			strcpy(buffer, fpath);		//mendapatkan direktori
			ext = strrchr(buffer, '.');	//mendapatkan extensi dari file
			strncpy(namafile, buffer, ext - buffer);	//menghapus extensi dari file
			sprintf(timestamp, "%d-%d-%d_%d:%d:%d", tm.tm_year + 1900, tm.tm_mon +1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec); //mendapatkan timestamp
			sprintf(hasil, "Backup/%s_%s.%s", namafile, timestamp, ext+1);	//menampung format nama pada array hasil
		
			char *argv[4] = {"cp", buffer, hasil, NULL};		//lalu copas isi dari buffer ke file hasil
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

static int xmp_mkdir(const char *path, mode_t mode)
{
	char fpath[1000];
	char buffer[1000];
	
	strcpy(buffer,path);

	if (strstr(buffer, "/YOUTUBER") != NULL) {		//jika ada folder baru di dalam YOUTUBER maka permissionnya adalah 750
		mode = 750;
	}

	if(strcmp(path,"/") == 0)
	{
		sprintf(fpath,"%s",dirpath);
	}
	else{
		sprintf(fpath, "%s%s",dirpath,path);
	}
    int res;

    res = mkdir(fpath, mode);
    if(res == -1)
        return -errno;

    return 0;
}



static struct fuse_operations xmp_oper = {
	.getattr	= xmp_getattr,
	.readdir	= xmp_readdir,
	.read		= xmp_read,
	.mkdir		= xmp_mkdir,
	.create     	= xmp_create,
	.utimens	= xmp_utimens,
	.chmod		= xmp_chmod,
	.write		= xmp_write,
	.unlink		= xmp_unlink,
	.truncate	= xmp_truncate,
	.readlink	= xmp_readlink,
};

int main(int argc, char *argv[])
{	
	
	encrypt(dirpath);
	umask(0);
	return fuse_main(argc, argv, &xmp_oper, NULL);
}

