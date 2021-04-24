#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <wait.h>
#include <time.h>

void createDir(char name[])
{
    char dir[50] = "/home/kali/modul2/soal1/";
    strcat(dir, name);

    char *argv[] = {"mkdir", "-p", dir, NULL};
    execv("/bin/mkdir", argv);
}

void download(char link[], char nama[])
{

    char down_dir[100] = "/home/kali/modul2/soal1/";
    strcat(down_dir, nama);

    char *argv[] = {"wget", "--no-check-certificate", "-q", link, "-O", down_dir, NULL};
    execv("/usr/bin/wget", argv);
}

int main()
{
    char tgl[50];
    time_t t;
    struct tm *localDate;
    pid_t parentid, sid;
    int status;
    pid_t childid = fork();
    parentid = fork();

    if (parentid < 0)
    {
        exit(EXIT_FAILURE);
    }

    if (parentid > 0)
    {
        exit(EXIT_SUCCESS);
    }

    umask(0);

    sid = setsid();
    if (sid < 0)
    {
        exit(EXIT_FAILURE);
    }

    if ((chdir("/home/kali/modul2/soal1/")) < 0)
    {
        exit(EXIT_FAILURE);
    }

    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    while (1)
    {
        t = time(NULL);
        localDate = localtime(&t);
        strftime(tgl, 50, "%Y-%m-%d %H:%M:%S", localDate);
        if (strcmp(tgl, "2021-04-09 16:22:00") == 0)
        {

            if (childid == 0)
            {
                createDir("Musyik");
            }
            else
            {
                childid = fork();
                if (childid == 0)
                {
                    createDir("Pyoto");
                }
                while (wait(NULL) != childid)
                    ;

                childid = fork();
                if (childid == 0)
                {
                    createDir("Fylm");
                }
                while (wait(NULL) != childid)
                    ;

                childid = fork();
                if (childid == 0)
                {
                    download("https://drive.google.com/uc?id=1FsrAzb9B5ixooGUs0dGiBr-rC7TS9wTD&export=download", "Foto_for_Stevany.zip");
                }
                while (wait(NULL) != childid)
                    ;

                childid = fork();
                if (childid == 0)
                {
                    download("https://drive.google.com/uc?id=1ZG8nRBRPquhYXq_sISdsVcXx5VdEgi-J&export=download", "Musik_for_Stevany.zip");
                }
                while (wait(NULL) != childid)
                    ;

                childid = fork();
                if (childid == 0)
                {
                    download("https://drive.google.com/uc?id=1ktjGgDkL0nNpY-vT7rT7O6ZI47Ke9xcp&export=download", "Film_for_Stevany.zip");
                }
                while (wait(NULL) != childid)
                    ;

                childid = fork();
                if (childid == 0)
                {
                    char *argv[] = {"unzip", "-q", "Foto_for_Stevany.zip", NULL};
                    execv("/usr/bin/unzip", argv);
                }
                while (wait(NULL) != childid)
                    ;

                childid = fork();
                if (childid == 0)
                {
                    char *argv[] = {"unzip", "-q", "Musik_for_Stevany.zip", NULL};
                    execv("/usr/bin/unzip", argv);
                }
                while (wait(NULL) != childid)
                    ;

                childid = fork();
                if (childid == 0)
                {
                    char *argv[] = {"unzip", "-q", "Film_for_Stevany.zip", NULL};
                    execv("/usr/bin/unzip", argv);
                }
                while (wait(NULL) != childid)
                    ;

                childid = fork();
                if (childid == 0)
                {
                    sleep (15);
                    char *argv[] = {"find", 
                                    "/home/kali/modul2/soal1/FOTO", 
                                    "-type", 
                                    "f", 
                                    "!", 
                                    "-iname", 
                                    "\*.jpg", 
                                    "-delete", 
                                    NULL
                                };
                    execv("/usr/bin/find", argv);
                }
                while (wait(NULL) != childid)
                    ;
            }
            break;
        }
    }

    while (1)
    {
        t = time(NULL);
        localDate = localtime(&t);
        strftime(tgl, 50, "%Y-%m-%d %H:%M:%S", localDate);
        if (strcmp(tgl, "2021-04-09 22:22:00") == 0)
        {
            childid = fork();
                if (childid == 0)
                {
                    char *argv[] = {"mv", "MUSIK", "Musyik", NULL};
                    execv("/bin/mv", argv);
                }
            else
            {
                childid = fork();
                if (childid == 0)
                {
                    char *argv[] = {"mv", "FILM", "Fylm", NULL};
                    execv("/bin/mv", argv);
                }
                while (wait(NULL) != childid);

               
                childid = fork();
                if (childid == 0)
                {
                    char *argv[] = {"mv", "FOTO", "Pyoto", NULL};
                    execv("/bin/mv", argv);
                }
                while (wait(NULL) != childid)
                    ;

                childid = fork();
                if (childid == 0)
                {
                    char *argv[] = {"zip", "-r", "Lopyu_Stevany.zip", "Pyoto", "Musyik", "Fylm", NULL};
                    execv("/usr/bin/zip", argv);
                }
                while ((wait(&status)) > 0);

                childid = fork();
                if (childid == 0)
                {
                    char *argv[] = {"rm", "-r", "FILM", "FOTO", "MUSIK", "Pyoto", "Fylm", "Musyik", NULL};
                    execv("/bin/rm", argv);
                }
                while (wait(NULL) != childid)
                    ;
            }
            break;
        }
    }
}
