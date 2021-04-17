#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <wait.h>
#include <string.h>
#include <dirent.h>

void createDir(char name[])
{
    char dir[50] = "/home/kali/modul2/soal1/";
    strcat(dir, name);

    char *argv[] = {"mkdir", "-p", dir, NULL};
    execv("/bin/mkdir", argv);
}

void download(char nama[], char link[])
{
    char down_dir[50] = "/home/kali/modul2/soal1/";
    strcat(down_dir, nama);

    char *argv[] = {"wget", "-O", down_dir, link, NULL};
    execv("/usr/bin/wget", argv);
}

void extractZip(char fileName[])
{
    char dir[50] = "/home/kali/modul2/soal1/";
    char temp[50] = "/home/kali/modul2/soal1/";
    strcat(temp, fileName); //menggabungkan string

    char *argv[] = {"unzip", temp, "-d", dir, NULL};
    execv("/usr/bin/unzip", argv);
}

void copy(char name[], char tujuan[])
{
    char src[50] = "/home/kali/modul2/soal1/";
    char dst[50] = "/home/kali/modul2/soal1/";

    strcat(src, name);
    strcat(dst, tujuan);

    char *argv[] = {"cp", "-r", "-T", src, dst, NULL};
    execv("/bin/cp", argv);
}

int main(int argc, char *argv[])
{

    pid_t pid, sid;

    //Bunuh Parent Process
    pid = fork();

    if (pid < 0)
    {
        exit(EXIT_FAILURE);
    }

    if (pid > 0)
    {
        exit(EXIT_SUCCESS);
    }

    //Buat Unique Session ID (SID)
    sid = setsid();
    if (sid < 0)
    {
        exit(EXIT_FAILURE);
    }

    // close(STDIN_FILENO);
    // close(STDOUT_FILENO);
    // close(STDERR_FILENO);

    while (1)
    {
        pid_t child_id11; //berjalan saat ultah
        int status11;

        child_id11 = fork();
        if (child_id11 < 0)
        {
            exit(EXIT_FAILURE);
        }

        if (child_id11 == 0)
        {
            pid_t child_id8; //berjalan 6 jam sebelumnya
            int status8;

            child_id8 = fork();
            if (child_id8 < 0)
            {
                exit(EXIT_FAILURE);
            }

            if (child_id8 == 0)
            {
                pid_t child_id5;
                int status5;

                child_id5 = fork();
                if (child_id5 < 0)
                {
                    exit(EXIT_FAILURE);
                }

                if (child_id5 == 0)
                {
                    pid_t child_id2;
                    int status2;

                    child_id2 = fork();
                    if (child_id2 < 0)
                    {
                        exit(EXIT_FAILURE);
                    }

                    if (child_id2 == 0)
                    {
                        pid_t child_id;
                        int status;

                        child_id = fork();
                        if (child_id < 0)
                        {
                            exit(EXIT_FAILURE);
                        }

                        if (child_id == 0)
                        {
                            sleep(5);
                            createDir("Musyik");
                        }
                        else
                        {
                            while ((wait(&status)) > 0)
                                ;
                            pid_t child_id1;
                            int status1;
                            child_id1 = fork();

                            if (child_id1 < 0)
                            {
                                exit(EXIT_FAILURE);
                            }
                            if (child_id1 == 0)
                            {
                                sleep(2);
                                createDir("Fylm");
                            }
                            else
                            {
                                while ((wait(&status1)) > 0)
                                    ;
                                sleep(2);
                                createDir("Pyoto");
                            }
                        }
                    }
                    else
                    {
                        while ((wait(&status2)) > 0)
                            ;
                        pid_t child_id3;

                        child_id3 = fork();
                        if (child_id3 < 0)
                        {
                            exit(EXIT_FAILURE);
                        }

                        if (child_id3 == 0)
                        {
                            sleep(5);
                            download("Musik_for_Stevany.zip", "https://drive.google.com/uc?id=1ZG8nRBRPquhYXq_sISdsVcXx5VdEgi-J&export=download");
                        }
                        else
                        {
                            pid_t child_id4;
                            int status4;
                            child_id4 = fork();

                            if (child_id4 < 0)
                            {
                                exit(EXIT_FAILURE);
                            }
                            if (child_id4 == 0)
                            {
                                sleep(5);
                                download("Foto_for_Stevany.zip", "https://drive.google.com/uc?id=1FsrAzb9B5ixooGUs0dGiBr-rC7TS9wTD&export=download");
                            }
                            else
                            {
                                while ((wait(&status4)) > 0)
                                    ;
                                sleep(5);
                                download("Film_for_Stevany.zip", "https://drive.google.com/uc?id=1ktjGgDkL0nNpY-vT7rT7O6ZI47Ke9xcp&export=download");
                            }
                        }
                    }
                }
                else
                {
                    while ((wait(&status5)) > 0)
                        ;
                    pid_t child_id6;
                    int status6;

                    child_id6 = fork();
                    if (child_id6 < 0)
                    {
                        exit(EXIT_FAILURE);
                    }

                    if (child_id6 == 0)
                    {
                        sleep(5);
                        extractZip("Musik_for_Stevany");
                    }
                    else
                    {
                        pid_t child_id7;
                        child_id7 = fork();

                        if (child_id7 < 0)
                        {
                            exit(EXIT_FAILURE);
                        }
                        if (child_id7 == 0)
                        {
                            sleep(5);
                            extractZip("Foto_for_Stevany");
                        }
                        else
                        {
                            while ((wait(&status6)) > 0)
                                ;
                            sleep(5);
                            extractZip("Film_for_Stevany");
                        }
                    }
                }
            }
            else
            {
                while ((wait(&status8)) > 0)
                    ;
                pid_t child_id9;
                int status9;

                child_id9 = fork();
                if (child_id9 < 0)
                {
                    exit(EXIT_FAILURE);
                }

                if (child_id9 == 0)
                {
                    sleep(5);
                    copy("FILM", "Fylm");
                }
                else
                {
                    pid_t child_id10;
                    child_id10 = fork();

                    if (child_id10 < 0)
                    {
                        exit(EXIT_FAILURE);
                    }
                    if (child_id10 == 0)
                    {
                        sleep(5);
                        copy("FOTO", "Pyoto");
                    }
                    else
                    {
                        while ((wait(&status9)) > 0)
                            ;
                        sleep(5);
                        copy("MUSIK", "Musyik");
                    }
                }

            } //berjalan 6 jam sebelumnya
        }
        else
        {
            while ((wait(&status11)) > 0)
                ;
            pid_t child_id12;
            int status12;

            child_id12 = fork();
            if (child_id12 < 0)
            {
                exit(EXIT_FAILURE);
            }

            if (child_id12 == 0)
            {
                sleep(5);
                chdir("/home/kali/modul2/soal1/");
                char *argv[] = {"zip", "-r", "Lopyu_Stevany.zip", "Fylm", "Musyik", "Pyoto", NULL};
                execv("/usr/bin/zip", argv);
                exit(0);
            }
            else
            {
                while ((wait(&status12)) > 0)
                    ;
                chdir("/home/kali/modul2/soal1/");
                sleep(5);
                char *argv[] = {"rm", "-r", "-f", "FILM", "FOTO", "MUSIK", "Fylm", "Musyik", "Pyoto", NULL};
                execv("/bin/rm", argv);
            }

        } //berjalan saat ultah
    }
}
