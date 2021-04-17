#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <wait.h>
#include <string.h>
#include <dirent.h>

void createDir()
{
    char dir[50] = "/home/ardha27/modul2/soal2/petshop";

    if(fork() == 0)
      {
          char *argv[] = {"mkdir", "-p", dir, NULL};
          execv("/bin/mkdir", argv);
          exit(0);
      }
    wait(NULL);
}

void kategori()
{
    DIR *dirp;
    struct dirent *dent;
    dirp = opendir("/home/ardha27/modul2/soal2/petshop");
    while ((dent = readdir(dirp)) != NULL)
    {
            int i;
            char file[20];
            memset(file, 0, sizeof file);
            for (i = 0; dent->d_name[i] != 0 && dent->d_name[i] != ';'; i++){
                file[i] = dent->d_name[i];
            }
            char dir[50] = "/home/ardha27/modul2/soal2/petshop/";
            strcat(dir, file);

                if(fork() == 0)
                {
                    char *argv[] = {"mkdir", "-p", dir, NULL};
                    execv("/bin/mkdir", argv);
                    exit(0);
                } 
    }
    wait(NULL);
}

void extractZip()
{
    char dir[50] = "/home/ardha27/modul2/soal2/petshop";
    char temp[50] = "/home/ardha27/modul2/soal2/pets.zip";
 
    if(fork() == 0)
      {
          char *argv[] = {"unzip", temp, "-d", dir, NULL};
          execv("/usr/bin/unzip", argv);
          exit(0);
      }
    wait(NULL);
}

void delete() {
    DIR * dirp;
    struct dirent * dent;
    dirp = opendir("/home/ardha27/modul2/soal2/petshop");

    while ((dent = readdir(dirp)) != NULL) {
        if(strcmp(dent->d_name,".")==0 || strcmp(dent->d_name,"..")==0 || (*dent->d_name) == '.' ) {
            //kosong
            }else if (dent->d_type == DT_DIR) {
                chdir("/home/ardha27/modul2/soal2/petshop");
                if(fork() == 0)
                {
                    char *argv[] = {"rm", "-r", dent->d_name, NULL};
                    execv("/bin/rm", argv);
                    exit(0);
                } 
            }
    }
    wait(NULL);
    chdir("/");
}

int main() {
    createDir();
    extractZip();
    delete();
    kategori();    
}
