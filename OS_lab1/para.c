if (pid = fork()) {
    //родитель
    wait(pid);
    exit(0);
}
else {
    //потомок do, exec
}

// в процессе можно узнать узнать пид и другую инфу


//атрибуты файлов
inode number - номер файла в системе
access mode - права доступа
type - тип файла (_ регулярный, d каталог, l символическая ссылка, s сокет, b блокориентированное устройство, c байториентированное устройство, p канал)
UID - user
GID - group
size - размер
hardlink count - кол-во жёстких ссылок
FSID - идентификатор файловой системы  // команда mount для смены файловой системы // /dev/hda(master)(hdb(slave), hdc(master), hdd(slave))/
время изменеия файла
изменения прав доступа                                                                                        //sda (тип флешки)

ф-я stat() возвращает данные
junction - жёсткая ссылка на директорию
open("name", o_readonly, )

 U | G | O
RWX RW- R--
 7   6   4

 S_IRU S_IWGRP S_IROTH

 umask() - убирает права (например, S_RWXO)

 C_TRUNC очищает

 fcntl(, int cmd, F_GETFL/F_SETFL)

include <sys/stat.h>

access("", flags) - проверка доступа

F_OK
W_OK
R_OK

chmod chown


4096 байт
в опер памяти страница 64 КБайт

32 или 64 байта кладутся в кеш

матрицу лучше держать транспонированной

mk nod имя_файла, разрешение, id устройства

без папок etc и dev ничего рабоать не будет

opendir
DIR*
dirent
readdir


int main() {
    Dirent *dp;
    int i = 0;
    dIR *dir;
    dir = opendir("./");
    for(;dp=readdir(dir);) {
        cout << dp->d_name << endl;
        i++;
    }
}
rewinddir - сброс чтения??

d_fileno

d_reclen

d_type

d_namlen
d_name
