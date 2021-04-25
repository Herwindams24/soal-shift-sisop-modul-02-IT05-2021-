# Shift 2 SISOP 2021 - IT05
Penyelesaian Soal Shift 2 Sistem Operasi 2021\
Kelompok IT05
  * Herwinda Marwaa Salsabila (05311840000009)
  * Stefanus Lionel Carlo Nugroho (05311940000027)
  * I Gde Ardha Semaranatha Gunasatwika (05311940000034)

---
## Table of Contents

* [Soal 1](#soal-1)
  * [Soal 1.a.](#soal-1a)
  * [Soal 1.b.](#soal-1b)
  * [Soal 1.c.](#soal-1c)
  * [Soal 1.d.](#soal-1d)
  * [Soal 1.e.](#soal-1e)
  * [Soal 1.f.](#soal-1f)
  * [Soal 1.g.](#soal-1g)
* [Soal 2](#soal-2)
  * [Soal 2.a.](#soal-2a)
* [Soal 3](#soal-3)
  * [Soal 3.a.](#soal-3a)
* [Kendala](#Kendala)
---
## Soal 1
Source Code : [soal1.c](https://github.com/Herwindams24/soal-shift-sisop-modul-2-IT05-2021/blob/main/soal1/soal1.c)

### Deskripsi Soal
Pada hari ulang tahun Stevany, Steven ingin memberikan Stevany zip berisikan hal-hal yang disukai Stevany. Steven ingin isi zipnya menjadi rapi dengan membuat folder masing-masing sesuai extensi.
### Asumsi Soal
Praktikan diminta untuk membuat sebuah program menggunakan Proses Daemon di mana program harus dapat berjalan pada tanggal 09 April 2021 serta pukul 16:22:00 dan pukul 22:22:00 WIB GMT+7. 

Adapun beberapa rincian soal yang diminta:
1.  Membuat 3 folder dengan nama Musyik, Fylm, dan Pyoto
2.  Program dapat melakukan pengunduhan dari link drive yang tersedia pada soal.
3.  Meng-ekstrak (*unzip*) dari file .zip yang telah diunduh 
4.  Hapus file berekstensi selain **.jpg** pada folder hasil ekstrak Foto.zip
5.  Memindahkan file yang telah diekstrak ke dalam folder yang telah dibuat pada point nomor 1.
7.  Empat poin di atas harus dapat berjalan secara otomisasi pada 6 jam sebelum ulang tahun dari Stevany, yang mana ulang tahun dari Stevany adalah 9 April pukul 22:22:00 WIB GMT+7. Sehingga dijalankan pada pukul 16:22:00 WIB GMT+7
8.  Pada saat waktu ulang tahun Stevany semua folder yang telah dibuat dan terisi di *zip* dengan nama Lopyu_Stevany.zip dan selain folder dengan eksternsi `.zip` maka akan terhapus.

### Pembahasan

Untuk membuat program C yang berjalan di background, pertama harus melakukan atau mengimport #include terhadap library yang diperlukan. 
Berikut merupakan 11 library yang penulis gunakan:

``` c
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
```

* `<sys/types.h>` library tipe data khusus (e.g. `pid_t`)
* `<sys/stat.h>` library untuk melakukan mengembalikan status waktu (e.g. `time_t()`)
* `<stdio.h>` library untuk fungsi input-output (e.g. `printf(), sprintf()`)
* `<stdlib.h>` library untuk fungsi umum (e.g. `exit()`, `atoi()`)
* `<fcntl.h>` library untuk proses id dalam proses kunci (e.g. `pid_t()`)
* `<errno.h>` library untuk memberikan tambahan error pada sistem (e.g. ECHILD)
* `<unistd.h>` library untuk melakukan system call kepada kernel linux (e.g. `fork()`)
* `<syslog.h>` library untuk mengirimkan pesan ke system logger
* `<string.h>` library yang digunakan untuk mennagani string (e.g. `strncmp()`, `strcat()`)
* `<time.h>` library untuk melakukan manipulasi date dan time (e.g. `time()`, `strftime()`)
* `<wait.h>` library untuk melakukan *wait* (e.g. `wait()`)

Dikarenakan pada soal proses program C yang dijalankan harus berjalan di latar belakang, maka kami harus membuat program ini menjadi sebuah daemon process sebagai berikut:
``` c
pid_t parentid, sid;
int status;

parentid = fork();

if (parentid < 0)
{
  exit(EXIT_FAILURE);
}
if (parentid > 0)
{
  exit(EXIT_SUCCESS);
}
```

*Note:  Fungsi `fork()` menghasilkan parent process dengan variable parentid yang berisi PID dari child process dan
child process dengan variable parentid berisi nilai 0. Selanjutnya, parent process akan dikeluarkan dengan fungsi exit() bersama dengan statusnya.*

Lalu ubah mode file dengan `umask(0)`
``` c
umask(0);
```

Selanjutnya, membuat Unique Session ID (SID)
 ```c
 sid = setsid();
    if (sid < 0)
    {
        exit(EXIT_FAILURE);
    }
```

*Note: Segala hasil proses dari program ini akan tersedia directory yang ada pada chdir.*

Sebuah daemon tidak boleh menggunakan terminal. Oleh sebab itu kita harus menutup file descriptor standar (STDIN, STDOUT, STDERR).
```c
close(STDIN_FILENO);
close(STDOUT_FILENO);
close(STDERR_FILENO);
```

### Soal 1.a.
Pada soal 1.a, praktikan diminta untuk membuat beberapa directory, diantaranya yaitu Musyik untuk ekstensi .mp3, Fylm untuk ekstensi .mp4, dan Pyoto untuk ekstensi .jpg. Direktori-direktori ini dibuat menggunakan fungsi `fork()` dan `execv`. 
### Pembahasan
Pertama-tama, penulis membuat sebuah fungsi `void createDir(char name[])` yang nantinya akan dipanggil pada while loop untuk membuat directory yang diminta.
```c
void createDir(char name[])
{
    char dir[100] = "/home/kali/modul2/soal1/";
    strcat(dir, name);

    char *argv[] = {"mkdir", "-p", dir, NULL};
    execv("/bin/mkdir", argv);
}
```
Selanjutnya, penulis membuat child process dengan variable `childid` untuk melakukan pembuatan dari directory yang diminta dengan cara memanggil fungsi void `createDir`.
```c
while (1)
    {
        ...
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
            }
              break;
         }
    }
```
*Note: Tanda `...` merupakan kode program yang tidak ditampilkan untuk memudahkan pembacaan. Kode program lengkap dapat dilihat pada [soal1.c](https://github.com/Herwindams24/soal-shift-sisop-modul-2-IT05-2021/blob/main/soal1/soal1.c)*

### Soal 1.b.
Pada soal b, praktikan diminta untuk mendownload musik, film, dan foto dari link drive yang tertera pada Soal Shift 2 bagian Note.
### Pembahasan
Pada penyelesaian kali ini, penulis membuat fungsi `void download` yang di dalamnya terdapat command `wget`, `fork()`, dan `execv()`. Selain itu, pada setiap proses terdapat penggantian nama file.zip dengan nama file yang telah ditentukan, seperti Foto menjadi Pyoto_for_Stevanny.zip, Film menjadi Fylm_for_Stevanny.zip, dan Musik menjadi Musyik_for_Stevanny.zip. 
```c
void download(char link[], char nama[])
{

    char down_dir[100] = "/home/kali/modul2/soal1/";
    strcat(down_dir, nama);

    char *argv[] = {"wget", "--no-check-certificate", "-q", link, "-O", down_dir, NULL};
    execv("/usr/bin/wget", argv);
}
```
Selanjutnya, fungsi `void download` akan dipanggil pada child process.
```c
while(1){
....
      else{
      ...
      childid = fork();
                if (childid == 0)
                {
                    download("https://drive.google.com/uc?id=1FsrAzb9B5ixooGUs0dGiBr-rC7TS9wTD&export=download", "Foto_for_Stevany.zip");
                }
                while (wait(NULL) != childid);

                childid = fork();
                if (childid == 0)
                {
                    download("https://drive.google.com/uc?id=1ZG8nRBRPquhYXq_sISdsVcXx5VdEgi-J&export=download", "Musik_for_Stevany.zip");
                }
                while (wait(NULL) != childid);

                childid = fork();
                if (childid == 0)
                {
                    download("https://drive.google.com/uc?id=1ktjGgDkL0nNpY-vT7rT7O6ZI47Ke9xcp&export=download", "Film_for_Stevany.zip");
                }
                while (wait(NULL) != childid);
    } 
      break;
  }
}
```
*Note: Tanda `...` merupakan kode program yang tidak ditampilkan untuk memudahkan pembacaan. Kode program lengkap dapat dilihat pada [soal1.c](https://github.com/Herwindams24/soal-shift-sisop-modul-2-IT05-2021/blob/main/soal1/soal1.c)*

### Soal 1.c
Praktikan diminta melakukan peng-ekstrakan file yang telah di download.

### Pembahasan
Pada soal 1.c. penulis menyelesaikannya dengan bantuan dari fungsi fork() dan execv() dalam melakukan eksekusinya. Proses dijalankan oleh child process dengan variable childid. Parent process akan menunggu hingga child process selesai bekerja. Oleh karena itu kami menggunakan fungsi wait().

```c
while(1){
 ...
 else{
  ...
  childid = fork();
  if (childid == 0)
  {
    char *argv[] = {"unzip", "-q", "Foto_for_Stevany.zip", NULL};
    execv("/usr/bin/unzip", argv);
  }
  while (wait(NULL) != childid);

  childid = fork();
  if (childid == 0)
  {
    char *argv[] = {"unzip", "-q", "Musik_for_Stevany.zip", NULL};
    execv("/usr/bin/unzip", argv);
  }
  while (wait(NULL) != childid);

  childid = fork();
  if (childid == 0)
  {
    char *argv[] = {"unzip", "-q", "Film_for_Stevany.zip", NULL};
    execv("/usr/bin/unzip", argv);
  }
    while (wait(NULL) != childid);
     }
    break;
  }
}
```

*Note: Tanda `...` merupakan kode program yang tidak ditampilkan untuk memudahkan pembacaan. Kode program lengkap dapat dilihat pada [soal1.c](https://github.com/Herwindams24/soal-shift-sisop-modul-2-IT05-2021/blob/main/soal1/soal1.c)*

### Soal 1.d
Hapus semua file gambar yang berekstensi *.png* maupun *.jpeg* pada folder 'FOTO' yang merupakan hasil unzip pada file Foto_for_Stevany.zip. Hanya sisakan file yang berekstensi *.jpg*.

### Pembahasan
Pada soal 1.d. penulis menyelesaikannya dengan bantuan dari fungsi fork() dan execv() dalam melakukan eksekusinya. Pada baris kode program di bawah ini, terlihat bahwa array dari pointer char berisikan command untuk menemukan 'path' folder FOTO lalu melakukan delete semua file kecuali yang berekstensi *.jpg*. 

```c
while(1){
 ...
 else{
  ...
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
     while (wait(NULL) != childid);
   }
    break;
  }
}
```
*Note: Tanda `...` merupakan kode program yang tidak ditampilkan untuk memudahkan pembacaan. Kode program lengkap dapat dilihat pada [soal1.c](https://github.com/Herwindams24/soal-shift-sisop-modul-2-IT05-2021/blob/main/soal1/soal1.c)*

### Soal 1.e
Pada soal d, penulis diminta untuk memindahkan (*move*) file-file yang telah diunzip ke dalam folder yang telah dibuat pada nomor 1.a. 

### Pembahasan
Seeperti yang ada di video contoh, proses move berjalan pada pukul 22:22 WIB GMT+7. Oleh karena itu, proses ini masuk ke dalam while big-loop yang ke-dua. Untuk eksekusinya, sama seperti proses yang sudah dijelaskan sebelumnya, di sini penulis menggunakan fungsi `fork()`, `wait()`, dan execv untuk melaksanakan eksekusinya.
```c
while (1)
    {
        ...
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
                ...
                }
            break;
        }
    }
```
*Note: Tanda `...` merupakan kode program yang tidak ditampilkan untuk memudahkan pembacaan. Kode program lengkap dapat dilihat pada [soal1.c](https://github.com/Herwindams24/soal-shift-sisop-modul-2-IT05-2021/blob/main/soal1/soal1.c)*

### Soal 1.f

Pada soal 1.f, penulis diminta untuk melakukan otomisasi seluruh program berjalan (kecuali pada program zip file kembali dari file yang telah di download, ekstrak, dan dipindahkan ke folder sebagaimana mestinya) ketika waktu pada 9 April 2021 pukul 16:22:00 WIB GMT+7. 

### Pembahasan
Pertama-tama, pada fungsi utama, penulis desklarasikan 3 variabel. Di mana variabel-variabel ini berfungsi menyimpan nilai tanggal dan nilai waktu. Konversi time_t ke tm sebagai localDate. Variable tm menyimpan timestamp yang sudah terstruktur sesuai dengan localtime.
```c
int main()
{
    char tgl[50];
    time_t t;
    struct tm *localDate;
    ...
}
```
Selanjutnya, di dalam While Big Loop, penulis mengeset waktu saat ini variable t dengan fungsi time(NULL). Di sini penulis juga menggunakan *strftime* untuk format waktu/ tanggal lokal menurut local settings. Lalu buatlah fungsi `if()` dengan parameter `strcmp(tgl, "2021-04-09 16:22:00") == 0`, di mana jika perbandingan dua string tersebut mengembalikan nilai 0, maka jalankan kode yang ada di dalam tubuh if.
```c
while (1)
    {
        t = time(NULL);
        localDate = localtime(&t);
        strftime(tgl, 50, "%Y-%m-%d %H:%M:%S", localDate);
        if (strcmp(tgl, "2021-04-09 16:22:00") == 0)
        {
         ...
         break;
        }
    }
```
*Note: Tanda `...` merupakan kode program yang tidak ditampilkan untuk memudahkan pembacaan. Kode program lengkap dapat dilihat pada [soal1.c](https://github.com/Herwindams24/soal-shift-sisop-modul-2-IT05-2021/blob/main/soal1/soal1.c)*


### Soal 1.g
Pada soal G, penulis diminta untuk menge-zip file dari folder yang telah di pindahkan pada Soal 1.e. dan penghapusan pada semua folder yang ada terkecuali pada file ekstensi `.zip`. Di mana proses ini terjadi pada pukul 22:22:00 WIB GMT+7

### Pembahasan

Pertama-tama, penulis membuat fungsi `while(1){}` kedua dan di dalam fungsi ini, penulis melakukan set waktu pada 09 April 2021 pukul 22:22:00 WIB GMT+7. Sebenarnya, secara konsep maupun cara hampir sama dengan yang ada pada [Soal 1.f.](#soal-1f).
```c
 while (1)
    {
        t = time(NULL);
        localDate = localtime(&t);
        strftime(tgl, 50, "%Y-%m-%d %H:%M:%S", localDate);
        if (strcmp(tgl, "2021-04-09 22:22:00") == 0)
        {...}
     }
```
Selanjutnya, pada dasarnya soal ini masih menggunakan fungsi fork(), wait(), maupun execv. Yang membedakan adalah adanya command zip dengan argumen kedua -rm. Hal ini dimaksudkan proses zip atau peng-compressan selesai maka folder akan dihapus. Pada akhir fungsi terdapat statement `break` yang berfungsi untuk mengakhiri loop.
```c
 while (1)
    {
       ...
        {
        childid = fork();
                if (childid == 0)
                {
                    char *argv[] = {"zip", "-r", "Lopyu_Stevany.zip", "Pyoto", "Musyik", "Fylm", NULL};
                    execv("/usr/bin/zip", argv);
                }

                while ((wait(&status)) > 0);
            else
            {
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
```
*Note: Tanda `...` merupakan kode program yang tidak ditampilkan untuk memudahkan pembacaan. Kode program lengkap dapat dilihat pada [soal1.c](https://github.com/Herwindams24/soal-shift-sisop-modul-2-IT05-2021/blob/main/soal1/soal1.c)*

## Dokumentasi Program

### Screenshoot konfigurasi waktu eksekusi program
![image](https://raw.githubusercontent.com/Herwindams24/soal-shift-sisop-modul-2-IT05-2021/main/img/Konfigurasi%20Waktu%20dan%20Eksekusi_02.png)

### Screenshoot proses run program pada pukul 16:22:00
![image](https://raw.githubusercontent.com/Herwindams24/soal-shift-sisop-modul-2-IT05-2021/main/img/Eksekusi%20Program1.png)

### Screenshoot filtering .jpg
![image](https://raw.githubusercontent.com/Herwindams24/soal-shift-sisop-modul-2-IT05-2021/main/img/FilteringJPG.png)

### Screenshoot hasil eksekusi program pada 9 April 2021 pukul 16:22:00
![image](https://raw.githubusercontent.com/Herwindams24/soal-shift-sisop-modul-2-IT05-2021/main/img/Hasil%20Eksekusi%20Pukul16.png)

### Screenshoot proses run program pada pukul 22:22:00
![image](https://raw.githubusercontent.com/Herwindams24/soal-shift-sisop-modul-2-IT05-2021/main/img/Eksekusi%20Program2.png)

### Screenshoot hasil eksekusi program pada 9 April 2021 pukul 22:22:00
![image](https://raw.githubusercontent.com/Herwindams24/soal-shift-sisop-modul-2-IT05-2021/main/img/Hasil%20Eksekusi%20Pukul%2022.png)

---
## Soal 2

---
## Soal 3

---
## Kendala
Soal:
 * Soal 1: Tidak dapat mengeset waktu dan terdapat kendala dalam mendownload file serta tidak dapat melakukan filtering file .jpg (Sebelum Revisi)
 * Soal 2: Tidak dapat memindahkan folder dengan ketentuan yang sesuai 
 * Soal 3: Tidak dapat mengesekusi fungsi killer dan tidak dapat membuat file status.txt dengan enkripsi Caesar Cipher setiap berhasil membuat zip


