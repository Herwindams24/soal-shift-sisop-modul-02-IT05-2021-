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
  * [Soal 2.b.](#soal-2b)
  * [Soal 2.c.](#soal-2c)
  * [Soal 2.d.](#soal-2d)
  * [Soal 2.e.](#soal-2e)
  * [Soal 2.f.](#soal-2f)
  * [Soal 2.g.](#soal-2g)
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
Source Code : [soal2.c](https://github.com/Herwindams24/soal-shift-sisop-modul-2-IT05-2021/blob/main/soal2/soal2.c)

### Deskripsi Soal
Loba bekerja di sebuah petshop terkenal, suatu saat dia mendapatkan zip yang berisi banyak sekali foto peliharaan dan Ia diperintahkan untuk mengkategorikan foto-foto peliharaan tersebut. 
### Asumsi Soal
Praktikan diminta untuk membantu Loba dalam mengkategorikan foto-foto peliharaan yang terdapat di dalam Zip.

Adapun beberapa rincian soal yang diminta:
1.  Membuat folder petshop
2.  Meng-ekstrak (*unzip*) dari file .zip yang diberikan
3.  Hapus folder-folder tidak penting yang terdapat di dalam .zip
4.  Melakukan pemotongan nama file pada .zip yang telah diberikan untuk mempermudah langkah kedepannya.
5.  Membuat folder untuk tiap jenis peliharaan
6.  Mengkategorikan foto-foto peliharaan ke dalam folder yang telah dibuat dan di rename dengan nama peliharaan
7.  Membuat keterangan.txt pada masing-masing folder yang berisi nama dan umur semua peliharaan dalam folder tersebut

### Pembahasan

Untuk membuat program C ini, pertama harus melakukan atau mengimport #include terhadap library yang diperlukan. 
Berikut merupakan 8 library yang penulis gunakan:

``` c
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <wait.h>
#include <string.h>
#include <dirent.h>
```

* `<stdlib.h>` library untuk fungsi umum (e.g. `exit()`, `atoi()`)
* `<sys/types.h>` library tipe data khusus (e.g. `pid_t`)
* `<sys/stat.h>` library untuk melakukan mengembalikan status waktu (e.g. `time_t()`)
* `<unistd.h>` library untuk melakukan system call kepada kernel linux (e.g. `fork()`)
* `<stdio.h>` library untuk fungsi input-output (e.g. `printf(), sprintf()`)
* `<wait.h>` library untuk melakukan *wait* (e.g. `wait()`)
* `<string.h>` library yang digunakan untuk mennagani string (e.g. `strncmp()`, `strcat()`)
* `<dirent.h>` library untuk melakukan traverse direktori (e.g. `opendir()`)

### Soal 2.a.
Pada soal 2.a, praktikan diminta untuk membuat directory petshop. Direktori ini dibuat dengan fungsi `wait()`, `fork()` dan `exec()`
### Pembahasan
Pertama-tama, penulis membuat sebuah fungsi `void createDir()` yang nantinya akan dipanggil pada `int main()` untuk membuat directory yang diminta.

``` c
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
```
Selanjutnya fungsi `void createDir()` akan dipanggil pada `int main()`
``` c
int main() {
    createDir();
    extractZip();
    delete();
    sleep(2);
    kelompok();    
}
```

### Soal 2.b.
Pada soal 2.b, praktikan diminta untuk mengekstrak file .zip yang telah diberikan. Proses ekstrak ini dilakukan dengan fungsi `wait()`, `fork()` dan `exec()`
### Pembahasan
Untuk mengekstrak file .zip, penulis membuat sebuah fungsi `void extractZip()` yang nantinya akan dipanggil `int main()` untuk mengesktrak .zip yang diberikan.
``` c
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
```
Selanjutnya fungsi `void extractZip()` akan dipanggil pada `int main()`
``` c
int main() {
    createDir();
    extractZip();
    delete();
    sleep(2);
    kelompok();    
}
```

### Soal 2.c.
Pada soal 2.c, praktikan diminta untuk menghapus folder tidak penting yang terdapat di dalam .zip yang diberikan. Proses penghapusan ini dilakukan dengan fungsi `opendir()`, `readdir`, `strcmp()`, `wait()`, `fork()`, dan `exec()`
### Pembahasan
Untuk menghapus folder tidak penting, penulis membuat sebuah fungsi `void delete()` yang nantinya akan dipanggil pada `int main()` untuk menghapus directory yang diminta.
``` c
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
```
Selanjutnya fungsi `void delete()` akan dipanggil pada `int main()`
``` c
int main() {
    createDir();
    extractZip();
    delete();
    sleep(2);
    kelompok();    
}
```

### Soal 2.d.
Pada soal 2.d, praktikan diminta untuk memotong nama file pada tiap foto peliharaan agar mempermudah proses kedepannya. Proses ini dilakukan dengan fungsi `opendir()`, `readdir`, `strcmp()`, `sprintf()`, `strstr()`, `strtok()`, dan `sleep()`
### Pembahasan
Penulis membuat sebuah fungsi `void kelompok()` yang nantinya akan memotong nama file pada tiap foto peliharaan sehingga didapat jenis hewan, nama, dan umur dari tiap peliharaan. Lalu, hasil pemotongan tadi akan digunakan sebagai parameter pada fungsi-fungsi kedepannya.
``` c
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
```

### Soal 2.e.
Pada soal 2.e, praktikan diminta untuk membuat folder untuk tiap jenis peliharaan. Pembuatan direktori ini dilakukan dengan fungsi `wait()`, `fork()`, dan `exec()`
### Pembahasan
Untuk membuat folder tiap jenis peliharaan, penulis membuat sebuah fungsi `void createKategori(char *name)` yang nantinya akan dipanggil pada `void kelompok()` untuk membuat directory yang diminta.
``` c
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
```
Selanjutnya fungsi `void createKategori(char *name)` akan dipanggil pada `void kelompok()`. Didalam `void kelompok()` dilakukan pemotongan pada nama tiap file sehingga didapatkan seluruh jenis peliharaan yang terdapat di dalam .zip dan dijadikan parameter untuk `void createKategori(char *name)`
``` c
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
```

### Soal 2.f.
Pada soal 2.f, praktikan diminta untuk mengkategorikan foto-foto peliharaan ke dalam folder yang telah dibuat dan di rename dengan nama peliharaan. Proses ini dilakukan dengan fungsi `sprintf()`, `wait()`, `fork()`, dan `exec()`
### Pembahasan
Pada tahap ini, penulis membuat sebuah fungsi `void copyFoto(char *dir, char *nama_file, char *nama)` yang nantinya akan dipanggil pada `void kelompok()` untuk mengkategorikan foto-foto peliharaan ke dalam directory yang disediakan.
``` c
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
```
Selanjutnya fungsi `void copyFoto(char *dir, char *nama_file, char *nama)` akan dipanggil pada `void kelompok()`. Didalam `void kelompok()` dilakukan pemotongan pada nama tiap file sehingga didapatkan nama dari seluruh peliharaan yang terdapat di dalam .zip dan dijadikan parameter untuk `void copyFoto(char *dir, char *nama_file, char *nama)`
``` c
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
```

### Soal 2.g.
Pada soal 2.g, praktikan diminta untuk Membuat keterangan.txt pada masing-masing folder yang berisi nama dan umur semua peliharaan dalam folder tersebut. Proses ini dilakukan dengan fungsi `sprintf()`, `fpirntf()`, `fclose()`, `wait()`, `fork()`, dan `exec()`
### Pembahasan
Pertama-tama, penulis membuat sebuah fungsi `void keterangan(char *dir, char *pet, char *umur)` yang nantinya akan dipanggil pada `void kelompok()` untuk menulis keterangan.txt pada tiap directory.
``` c
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
```
Selanjutnya fungsi `void keterangan(char *dir, char *pet, char *umur)` akan dipanggil pada `void kelompok()`. Didalam `void kelompok()` dilakukan pemotongan pada nama tiap file sehingga didapatkan nama dan umur dari seluruh peliharaan yang terdapat di dalam .zip. Lalu, dijadikan parameter untuk `void keterangan(char *dir, char *pet, char *umur)`
``` c
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
```

## Dokumentasi Program

### Screenshoot Folder Kategori Peliharaan
![image](https://raw.githubusercontent.com/Herwindams24/soal-shift-sisop-modul-2-IT05-2021/main/img/Hasil_Soal2_1.png)

### Screenshoot Isi Folder Kategori Peliharaan
![image](https://raw.githubusercontent.com/Herwindams24/soal-shift-sisop-modul-2-IT05-2021/main/img/Hasil_Soal2_2.png)

### Screenshoot Salah Satu Keterangan.txt
![image](https://raw.githubusercontent.com/Herwindams24/soal-shift-sisop-modul-2-IT05-2021/main/img/Hasil_Soal2_3.png)

---
## Soal 3
## Soal 3
Source Code : [soal3.c](https://github.com/Herwindams24/soal-shift-sisop-modul-2-IT05-2021/blob/main/soal3/soal3.c)

### Deskripsi Soal
Ranora yang sedang menjalani magang di perusahaan “FakeKos Corp.” diminta untuk membuat sebuah program yang mendownload kumpulan foto dari website https://picsum.photos/ dan membuat folder dengan format nama berdasarkan timestamp yang kemudian di zip. 

### Asumsi Soal
Praktikan diminta untuk membuat sebuah program yang menerima parameter untuk menentukan penjalanan program antara dua mode, kemudian ketika berjalan secara automatis melakukan proses download, folder filtering, melakukan zip, mengirimkan status update dengan enkripsi caesar cipher, serta dapat men-generate program killer yang dapat menterminasi semua proses yang sedang berjalan dan akan menghapus dirinya sendiri setelah program dijalankan.

Adapun beberapa rincian soal yang diminta:
1.  Membuat sebuah program yang dimana setiap 40 detik membuat sebuah direktori dengan nama sesuai timestamp [YYYY-mm-dd_HH:ii:ss].
2.  Program dapat melakukan pengunduhan foto dari link yang tersedia pada soal dengan format nama timestamp serta berbentuk persegi dengan ukuran (n%1000)+50 pixel dimana n adalah detik epoch unix.
3.  Membuat sebuah file “status.txt”, dimana di dalamnya berisi pesan “Download Success” yang terenkripsi dengan metode Caesar Cipher shift 5.
4. Men-zip direktori yang telah dibuat dan mendelete direktori lama sehingga hanya tersisa file zip saja.
5. Men-generate program “Killer” yang executable, dimana program bash tersebut menterminasi semua program yang sedang berjalan dan menghapus dirinya sendiri setelah program dijalankan.
6. Program yang dibuat dapat dijalankan dalam dua mode. mode pertama dijalankan dengan menerima arguman -z dimana program utama akan langsung menghentikan semua operasi ketika program killer dijalankan. Mode kedua dijalankan dengan menerima argumen -x dimana program utama menyelesaikan dahulu proses di direktori yang berjalan hingga selesai baru menghentikan operasi.

### Pembahasan

Untuk membuat program C yang berjalan di background, pertama harus melakukan atau mengimport #include terhadap library yang diperlukan. 
Berikut merupakan 12 library yang penulis gunakan:

``` c
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <wait.h>
#include <string.h>
#include <dirent.h>
#include <time.h>
#include <errno.h>
#include <fcntl.h>
```

* `<stdlib.h>` library untuk fungsi umum (e.g. `exit()`, `atoi()`)
* `<sys/types.h>` library tipe data khusus (e.g. `pid_t`)
* `<sys/stat.h>` library untuk melakukan mengembalikan status waktu (e.g. `time_t()`)
* `<unistd.h>` library untuk melakukan system call kepada kernel linux (e.g. `fork()`)
* `<stdio.h>` library untuk fungsi input-output (e.g. `printf(), sprintf()`)
* `<wait.h>` library untuk melakukan *wait* (e.g. `wait()`)
* `<string.h>` library yang digunakan untuk mennagani string (e.g. `strncmp()`, `strcat()`)
* `<dirent.h>` library untuk proses directory traversing dalam program
* `<time.h>` library untuk melakukan manipulasi date dan time (e.g. `time()`, `strftime()`)
* `<errno.h>` library untuk memberikan tambahan error pada sistem (e.g. ECHILD)
* `<fcntl.h>` library untuk proses id dalam proses kunci (e.g. `pid_t()`)

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

### Soal 3.a
membuat sebuah program C yang dimana setiap 40 detik membuat
sebuah direktori dengan nama sesuai timestamp [YYYY-mm-dd_HH:ii:ss].

### Pembahasan
Pada soal 3.a. penulis menyelesaikannya dengan bantuan dari strftime() yang berfungsi untuk memasukkan waktu saat pembuatan file ataupun folder dan menyimpannya dalam variabel buffer. Adapun dalam mengeksekusinya juga digunakan fungsi sleep() untuk mengatur agar program mengeksekusi pembuatan direkturi tiap 40s. Adapun sourcecode yang dibuat dapat dilihat pada kolom dibawah.

```c
while (1)
{
    pid_t child1, child2, child3, child4;
    int status;
 
    time_t now;
    struct tm *t;
    char buffer[80];
 
    time(&now);
    t = localtime(&now);
 
    strftime(buffer, 80, "%Y-%m-%d_%H:%M:%S", t);
    // puts (buffer);
    char *dir = buffer;
    mkdir(dir, 0777);
 
    child1 = fork();
    if (child1 < 0)
        exit(EXIT_FAILURE);
…
 while (wait(&status) > 0)
        ;
    sleep(40);
```
*Note: Tanda `...` merupakan kode program yang tidak ditampilkan untuk memudahkan pembacaan. Kode program lengkap dapat dilihat pada [soal3.c](https://github.com/Herwindams24/soal-shift-sisop-modul-2-IT05-2021/blob/main/soal3/soal3.c)*

### Soal 3.b
Setiap direktori yang sudah dibuat diisi dengan 10 gambar yang didownload dari
https://picsum.photos/, dimana setiap gambar akan didownload setiap 5 detik. Setiap
gambar yang didownload akan diberi nama dengan format timestamp
[YYYY-mm-dd_HH:ii:ss] dan gambar tersebut berbentuk persegi dengan ukuran
(n%1000) + 50 pixel dimana n adalah detik Epoch Unix.

### Pembahasan
Pada soal 3.b. penulis menyelesaikannya dengan bantuan fungsi wget untuk melakukan fungsi download dan sprintf untuk menyimpan link download ke dalam variabel *link*. Kemudian melakukan penamaan dengan memanfaatkan variabel fileti untuk memuat waktu file didownload dan dijadikan nama file tersebut. Adapun berikut sourcecode yang dilakukan untuk menjalankan program yang ada:

```c
    if (child1 == 0)
    {
        for (int i = 0; i < 10; i++)
        {
            if (fork() == 0)
            {
                unsigned long get_time = (unsigned long)time(NULL);
                get_time = (get_time % 1000) + 50;
 
                char link[50];
                sprintf(link, "https://picsum.photos/%ld", get_time);
 
                time_t filet;
                struct tm *fileti;
                char files[80] = "";
                char down_dir[100];
 
                time(&filet);
                fileti = localtime(&filet);
 
                strftime(files, 80, "%Y-%m-%d_%H:%M:%S", fileti);
                sprintf(down_dir, "%s/%s", buffer, files);
                // puts(down_dir);
 
                char *argv[] = {"wget", "-O", down_dir, link, NULL};
                execv("/usr/bin/wget", argv);
                exit(0);
            }
            sleep(5);
        }
        for (int i = 0; i < 10; i++)
            wait(NULL);
    }

```
*Note: Tanda `...` merupakan kode program yang tidak ditampilkan untuk memudahkan pembacaan. Kode program lengkap dapat dilihat pada [soal3.c](https://github.com/Herwindams24/soal-shift-sisop-modul-2-IT05-2021/blob/main/soal3/soal3.c)*

### Soal 3.c
Tiap direktori terisi 10 gambar program akan membuat file “status.txt” yang akan memuat pesan “download success” terinkripsi dengan caesar cipher shift 5 dan kemudian melakukan zip direktori dan mendelete direktori yang ada sehingga menyisakan file zip saja.

### Pembahasan
Pada soal 3.c. penulis mengalami kendala dalam membuat file status.txt yang berisi pesan terenkripsi sehingga tidak dapat dilakukan. Untuk melakukan zip dan delete directory, penulis memasukannya dalam child2 dan memanfaatkan variabel buffer yang sudah dibuat di awal untuk melakukan rename terhadap zip yang sudah dibuat. Adapun sourcecode yang digunakan dalam menjalankan program adalah sebagai berikut:

```c

    child2 = fork();
    if (child2 < 0)
        exit(EXIT_FAILURE);
 
    if (child2 == 0)
    {
        char zip_name[100];
        sprintf(zip_name, "%s.zip", buffer);
 
        char *argv[] = {"zip", "-rm", zip_name, buffer, NULL};
        execv("/usr/bin/zip", argv);
        exit(0);
    }
```
### Soal 3.d
men-generate sebuah program “Killer” yang executable, dimana
program tersebut akan menterminasi semua proses program yang sedang berjalan dan
akan menghapus dirinya sendiri setelah program dijalankan. Karena Ranora menyukai
sesuatu hal yang baru, maka Ranora memiliki ide untuk program “Killer” yang dibuat
nantinya harus merupakan program bash.

### Pembahasan
Pada soal 3.c. penulis mengalami kendala dalam men-generate program killer sehingga program mengalami infinit loop dan terus melakukan download serta pembuatan zip. Penulis mencoba menanamkan program c di dalam program untuk men-generate namun tidak berhasil berjalan. Adapun percobaan source code yang digunakan adalah sebagai berikut:

```c
   sprintf(src, "%s/killer.sh", cwd);
        sprintf(dst, "%s/killer", cwd);
 
        sprintf(files, "%s/killer.sh", cwd);
 
        if (argv[1][1] == 'z')
            strcpy(mode, " char *argv[] = {\"killall\", \"-s\", \"9\", \"soal2\", NULL};");
        else if (argv[1][1] == 'x')
            sprintf(mode, " char *argv[] = {\"kill\", \"%d\", NULL};", pid2);
 
        sprintf(data, "#include <stdlib.h>\n"
                      "#include <sys/types.h>\n"
                      "#include <unistd.h>\n"
                      "#include <stdio.h>\n"
                      "#include <wait.h>\n"
                      "#include <string.h>\n"
 
                      " int main() {"
                      " pid_t child_id;"
                      " int status;"
 
                      " child_id = fork();"
                      " if (child_id < 0) exit(EXIT_FAILURE);"
 
                      " if (child_id == 0) {"
                      " %s"
                      " execv(\"/usr/bin/killall\", argv);"
                      " } else {"
                      " while (wait(&status)>0);"
                      " }"
                      " }",
                mode);
 
        ptr = fopen(files, "w");
        fputs(data, ptr);
        fclose(ptr);
 
        char *argv[] = {"gcc", src, "-o", dst, NULL};
        execv("/usr/bin/gcc", argv);
    }
```

*Note: Tanda `...` merupakan kode program yang tidak ditampilkan untuk memudahkan pembacaan. Kode program lengkap dapat dilihat pada [soal3.c](https://github.com/Herwindams24/soal-shift-sisop-modul-2-IT05-2021/blob/main/soal3/soal3.c)*

### Soal 3.e
Program utama dapat dijalankan di dalam dua mode. Untuk mengaktifkan mode pertama, program harus dijalankan dengan argumen -z, dan Ketika dijalankan dalam mode pertama, program utama akan langsung menghentikan semua operasinya Ketika program Killer dijalankan. Sedangkan untuk mengaktifkan mode kedua, program harus dijalankan dengan
argumen -x, dan Ketika dijalankan dalam mode kedua, program utama akan berhenti namun membiarkan proses di setiap direktori yang masih berjalan hingga selesai (Direktori yang sudah dibuat akan mendownload gambar sampai selesai dan membuat file txt, lalu zip dan delete direktori).


### Pembahasan
Pada soal 3.e. penulis mengalami kendala akibat program killer.sh yang tidak berjalan sehingga ketika diberi parameter program tetap menjalankan infinit loop, adapun penulis mencoba menuliskan sourcecode sebagi berikut sehingga program dapat menerima argumen -z dan -x untuk menjalankan program dan mengeluarkan pesan “argumen hanya -z atau -x” bila diberi argumen lain:

```c

    if (argc != 2 || argv[1][1] != 'z' && argv[1][1] != 'x')
        printf("argumen hanya -z atau -x");
…
        if (argv[1][1] == 'z')
            strcpy(mode, " char *argv[] = {\"killall\", \"-s\", \"9\", \"soal2\", NULL};");
        else if (argv[1][1] == 'x')
            sprintf(mode, " char *argv[] = {\"kill\", \"%d\", NULL};", pid2);
```

*Note: Tanda `...` merupakan kode program yang tidak ditampilkan untuk memudahkan pembacaan. Kode program lengkap dapat dilihat pada [soal3.c](https://github.com/Herwindams24/soal-shift-sisop-modul-2-IT05-2021/blob/main/soal3/soal3.c)*

##Dokumentasi Program

### Screenshoot program saat berjalan dalam terminal
![image](https://github.com/Herwindams24/soal-shift-sisop-modul-2-IT05-2021/blob/main/img/program%20saat%20dijalankan%20soal%203.jpg)

### Screenshoot direktori yang terbentuk setelah program dijalankan
![image](https://github.com/Herwindams24/soal-shift-sisop-modul-2-IT05-2021/blob/main/img/direktori%20yang%20terbentuk%20setelah%20program%20dijalankan%20soal%203.jpg)

### Screenshoot isi zip yang terbentuk
![image](https://github.com/Herwindams24/soal-shift-sisop-modul-2-IT05-2021/blob/main/img/Isi%20zip%20soal%203.jpg)

### Screenshoot isi direktori yang terbentuk dalam direktori zip
![image](https://github.com/Herwindams24/soal-shift-sisop-modul-2-IT05-2021/blob/main/img/isi%20zip%20soal%203%20dalam%20direktori.jpg)

### Screenshoot gambar yang berhasil terunduh dengan ukuran sesuai dengan ketentuan program yang dijalankan
![image](https://github.com/Herwindams24/soal-shift-sisop-modul-2-IT05-2021/blob/main/img/hasil%20gambar%20yang%20terunduh%20soal%203.jpg)

---
## Kendala
Soal:
 * Soal 1: Tidak dapat mengeset waktu dan terdapat kendala dalam mendownload file serta tidak dapat melakukan filtering file .jpg (Sebelum Revisi)
 * Soal 2: Tidak dapat memindahkan folder dengan ketentuan yang sesuai 
 * Soal 3: Tidak dapat mengesekusi fungsi killer dan tidak dapat membuat file status.txt dengan enkripsi Caesar Cipher setiap berhasil membuat zip


