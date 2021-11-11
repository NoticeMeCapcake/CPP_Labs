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


что такое pipe - каналы
Pipe
Fifo тип pipe есть имя
mkfifo("", mode)
open(fifo)

-Mkfifo
-fork
-open

создание pipe - int Pipe(int fd[2] - дескрипторы pipe на чтение и запись) // 0 или -1


-Pipe
-fork
  /         \
close(fd[1]) close(fd[0])

-pipe fd1
-pipe fd2
-fork
close 1,0|close 0,1



сигналы

SIG ALRM - ПРОБУДИТЬ
SIG FPE - МАТ ОШИБКИ
CTRL_C SIG INT - ПРЕРВАТЬ ПРОЦЕСС
SIG PIPE - ЗАВЕРШЕНИЕ РАБОТЫ
SIG SEGV - SEGMENTATION FAULT
SIG TERM
SIG CHLD
SIG USR
SIG HUB - РАЗРЫВ СВЯЗИ С УПРАВ КАНАЛОМ

сделать сигнал void (*signal(int signum, void * handler(int)))(int)
include signal.h
int int main(int argc, char const *argv[]) {
    void (*old_behaviour(int) = signal(SIGINT, SIG_IGN))
    signal(SIGINT, old_behaviour);
    return 0;
}
popen() -- конвеер (перенаправление)

freopen() -- открыть файл, не закрывая

File *frw = freopen("foo", "w", std::out)
