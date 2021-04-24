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
* [Soal 2](#soal-2)
  * [Soal 2.a.](#soal-2a)
* [Soal 3](#soal-3)
  * [Soal 3.a.](#soal-3a)
---
## Soal 1
Source Code : [soal1.c](https://github.com/Herwindams24/soal-shift-sisop-modul-2-IT05-2021/blob/main/soal1/soal1.c)

### Deskripsi Soal
Pada hari ulang tahun Stevany, Steven ingin memberikan Stevany zip berisikan hal-hal yang disukai Stevany. Steven ingin isi zipnya menjadi rapi dengan membuat folder masing-masing sesuai extensi.
### Asumsi Soal
Praktikan diminta untuk membuat sebuah program menggunakan Proses Daemon di mana program harus dapat berjalan pada tanggal 09 April 2021 serta pukul 16:22:00 dan pukul 22:22:00 WIB GMT+7. 

Adapun beberapa rincian soal yang diminta:
1. Membuat 3 folder dengan nama Musyik, Fylm, dan Pyoto
2. Program dapat melakukan pengunduhan dari link drive yang tersedia pada soal.
3. Meng-ekstrak (*unzip*) dari file .zip yang telah diunduh 
4. Memindahkan file yang telah diekstrak ke dalam folder yang telah dibuat pada point nomor 1.
5. Empat poin di atas harus dapat berjalan secara otomisasi pada 6 jam sebelum ulang tahun dari Stevany, yang mana ulang tahun dari Stevany adalah 9 April pukul 22:22:00 WIB GMT+7. Sehingga dijalankan pada pukul 16:22:00 WIB GMT+7
6. Pada saat waktu ulang tahun Stevany semua folder yang telah dibuat dan terisi di *zip* dengan nama Lopyu_Stevany.zip dan selain folder dengan eksternsi `.zip` maka akan terhapus.

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
* `<sys/stat.h>` Library untuk melakukan mengembalikan status waktu (e.g. `time_t()`)
* `<stdio.h>` library untuk fungsi input-output (e.g. `printf(), sprintf()`)
* `<stdlib.h>` library untuk fungsi umum (e.g. `exit(), atoi()`)
* `<fcntl.h>` library untuk proses id dalam proses kunci (e.g. `pid_t()`)
* `<errno.h>` library brary untuk memberikan tambahan error pada sistem (e.g. ECHILD)
* `<unistd.h>` library untuk melakukan system call kepada kernel linux (e.g. `fork()`)
* `<syslog.h>` library untuk mengirimkan pesan ke system logger
* `<string.h>` library yang digunakan untuk mennagani string (e.g. `strncmp(), strcat()`)
* `<time.h>` library untuk melakukan manipulasi date dan time (e.g. `time(), strftime()`)
* `<wait.h>` Library untuk melakukan *wait* (e.g. `wait()`)

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

### Soal 1.a.

### Pembahasan

---
## Soal 2

---
## Soal 3

---
## Kendala
Soal:
 * Soal 1: Tidak dapat mengeset waktu dan terdapat kendala dalam mendownload file
 * Soal 2: Tidak dapat memindahkan folder dengan ketentuan yang sesuai 
 * Soal 3: Tidak dapat mengesekusi fungsi killer dan tidak dapat membuat file status.txt dengan enkripsi Caesar Cipher setiap berhasil membuat zip


