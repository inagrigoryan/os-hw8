#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>


int main(){

	int fd;
	fd = open("numbers.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if(fd == -1){
		perror("open");
		return 1;
	}

	for (int i =1; i <=10; i++){
		char line[4];
		snprintf(line, sizeof(line), "%d\n", i);
		ssize_t nr = write(fd, line, strlen(line));
		if (nr == -1){
			perror("write");
			close(fd);
			return 1;
		}
	}

	if (close(fd) == -1)
		perror("close");

	fd = open("numbers.txt", O_RDWR);
	if (fd == -1){
		perror("open rdwr");
		return 1;
	}
	
	char c;
	ssize_t nr;
	int newlines = 0;
	off_t pos = 0;
	off_t line4_start = -1;
	off_t line4_end = -1;

	while ((nr = read(fd, &c, 1)) == 1){
		pos++;
		if (c == '\n'){
			newlines++;
			if (newlines == 3)
				line4_start = pos;
			if (newlines == 4){
				line4_end = pos;
				break;
			}
		}
	}

	if (nr == -1){
		perror("read");
		close(fd);
		return 1;
	}
	

	off_t l_ret = lseek(fd, line4_end, SEEK_SET);
	if (l_ret == -1){
		perror("lseek");
		close(fd);
		return 1;
	}

	char rest[256];
	ssize_t rest_len = read(fd, rest, sizeof(rest));
	if (rest_len == -1){
		perror("read rest");
		close(fd);
		return 1;
	}

	l_ret = lseek(fd, line4_start, SEEK_SET);
	if (l_ret == -1){
                perror("lseek");
                close(fd);
                return 1;
        }

	const char *newtext = "100\n";
	nr = write(fd, newtext, strlen(newtext));
	if (nr == -1){
        	perror("write 100");
        	close(fd);
        	return -1;
    	}

	nr = write(fd, rest, rest_len);
	if (nr == -1){
        	perror("write rest");
        	close(fd);
        	return -1;
	}

	if (truncate("numbers.txt", line4_start + strlen(newtext) + rest_len) == -1)
		perror("truncate");

	l_ret = lseek(fd, (off_t) 0, SEEK_SET);
	if (l_ret == (off_t)-1){
        	perror("lseek start");
        	close(fd);
        	return -1;
    	}

	printf("final result\n");
	char out[256];
	while ((nr = read(fd, out, sizeof(out))) > 0){
		write(1, out, nr);
	}
	if (nr == -1)
		perror("read final");
	if (close(fd) == -1)
		perror("close");


	return 0;
}
