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

    DIR *dir = opendir(name);
    if (dir) {
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
    sprintf(src, "%s", nama_file);

    char dst[40];
    sprintf(dst, "%s/%s.jpg", dir, nama);

    int status;
    
    if (fork() == 0) {
        char *argv[] = {"cp", "-T", src, dst, NULL};

        execv("/bin/cp", argv);
        exit(0); 
    } 
    wait(NULL);
}

void split()
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
            //printf("%s\n", dent->d_name);
            char nama_file[50];
            sprintf(nama_file, "%s", dent->d_name);

            char *split;
            char *folder[3];
            split = strtok(dent->d_name, ";_");
            while (split != NULL) {
                int i;
                for (i = 0; i < 3; ++i) {
                    folder[i] = split;
                    split = strtok(NULL, ";_");
                }
                
                char *pet_dir = folder[0];
                char *pet_name = folder[1];
                // char *pet_age = folder[2];

                // char *dot_jpg;
                // dot_jpg = strstr(pet_age, ".jpg");
                // if (dot_jpg != NULL) {
                //     int pos = dot_jpg - pet_age;
                //     sprintf(pet_age, "%.*s", pos, pet_age);
                // }
                createKategori(pet_dir);
                sleep(1);
                copyFoto(pet_dir, nama_file, pet_name);

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
    split();    
}
