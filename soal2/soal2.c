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

void createKategori(char *name)
{
    char folder[50] = "/home/ardha27/modul2/soal2/petshop/";
    sprintf(folder, "%s", name);

    DIR *dirp = opendir(name);
    if (dirp) {
        return;
    }

    if(fork() == 0)
      {
          char *argv[] = {"mkdir", "-p", folder, NULL};
          execv("/bin/mkdir", argv);
          exit(0);
      }
    wait(NULL);
}

void copyFoto(char *dir, char *nama_file, char *nama) {
    chdir("/home/ardha27/modul2/soal2/petshop/");
    char src[30];
    char dst[40];
    sprintf(src, "%s", nama_file);

    sprintf(dst, "%s/%s.jpg", dir, nama);

    if (fork() == 0) {
        char *argv[] = {"cp", "-T", src, dst, NULL};

        execv("/bin/cp", argv);
        exit(0); 
    } 
    wait(NULL);
}

void keterangan(char *dir, char *pet, char *umur) {
    char folder[50];
    sprintf(folder, "%s/keterangan.txt", dir);

    if (fork() == 0) {
        FILE *dirp;
        dirp = fopen(folder, "a");
        fprintf(dirp, "nama : %s\numur : %s\n\n", pet, umur);
        fclose(dirp);
        exit(0);
    }
    wait(NULL);
}

void kelompok()
{
    DIR *dirp;
    struct dirent *dent;
    dirp = opendir("/home/ardha27/modul2/soal2/petshop");
    while (dent = readdir(dirp))
    {
        if(strcmp(dent->d_name,".") == 0 || strcmp(dent->d_name,"..") == 0) {

        } else if (dent->d_type == DT_DIR) {
            return;
        } else {
            
            char nama_file[50];
            char *split;
            char *folder[3];
            
            sprintf(nama_file, "%s", dent->d_name);
            split = strtok(dent->d_name, ";_");

            while (split != NULL) {
                int i;
                for (i = 0; i < 3; ++i) {
                    folder[i] = split;
                    split = strtok(NULL, ";_");
                }
                
                char *jenis = folder[0];
                char *nama = folder[1];
                char *umur = folder[2];

                char *dot;
                dot = strstr(umur, ".jpg");
                if (dot != NULL) {
                    int pos = dot - umur;
                    sprintf(umur, "%.*s", pos, umur);
                }
                createKategori(jenis);
                sleep(1);
                copyFoto(jenis, nama_file, nama);
                keterangan(jenis, nama, umur);

            }

        }

    }
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
        if(strcmp(dent->d_name,".") == 0 || strcmp(dent->d_name,"..") == 0) {
            //kosong
            }else if (dent->d_type == DT_DIR) {
                chdir("/home/ardha27/modul2/soal2/petshop");
                if(fork() == 0)
                {
                    char *argv[] = {"rm", "-rf", dent->d_name, NULL};
                    execv("/bin/rm", argv);
                    exit(0);
                }
            }
    }
    wait(NULL);
}

int main() {
    createDir();
    extractZip();
    delete();
    sleep(2);
    kelompok();    
}
