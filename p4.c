
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main(){

        int fd;
        fd = open("log.txt", O_WRONLY | O_CREAT | O_APPEND, 0644);
        if (fd == -1){
                perror("open");
                return 1;
        }

        char buffer[100];
        ssize_t nr;
        ssize_t nw;
        int i = 0;
        char c;

        printf("enter log message ");
	fflush(stdout); // because the message was not showing
        while (i < 99) {
                nr = read(0, &c, 1);

                if (nr == -1){
                        perror("read");
                        close(fd);
                        return 1;
                }

                if (nr == 0 || c == '\n')
                        break;

                buffer[i++] = c;
        }

        pid_t pid = getpid();

        char result[150];
        int len = 0;

        len = snprintf(result, sizeof(result), "PID=%d: ", pid);

        for (int k = 0; k < i; k++){
                result[len++] = buffer[k];
        }

        result[len++] = '\n';

        nw = write(fd, result, len);
        if (nw == -1){
                perror("write");
                close(fd);
                return 1;
        }

        off_t ret;
        ret = lseek(fd, 0, SEEK_CUR);

        if (ret == -1){
                perror("lseek");
                close(fd);
                return 1;
        }

        // O_APPEND moves the file position to the end before each write
        // that is why SEEK_CUR returns the current file offset after writing.

        printf("final file offset: %ld\n", (long)ret);

        if(close(fd) == -1){
                perror("close");
                return 1;
        }

        return 0;
}

