#include <stdio.h>
#include <fcntl.h> 
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

extern int errno;

int main(){

	char source[100];
	char dest[100];

	printf("enter source file path ");
	scanf("%99s", source);

	printf("enter destination file path ");
        scanf("%99s", dest);

	int fd_s = open(source, O_RDONLY);
	if (fd_s == -1) {
		perror("opening source file");
		return 1;
	}

	int fd_d = open(dest, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if(fd_d == -1){
		perror("opening destination file");
		close(fd_s);
		return 1;
	}

	char buffer[100];
	ssize_t nr;
	ssize_t nw;
	ssize_t total = 0;
	
	while ((nr = read(fd_s, buffer, sizeof(buffer))) != 0){
		if (nr == -1){
			if(errno == EINTR)
				continue;

			perror("read");
			close(fd_s);
			close(fd_d);
			return 1;
		}

		nw = write(fd_d, buffer, nr);

		if (nw == -1){
			perror("write");
			close(fd_s);
			close(fd_d);
			return 1;
		}

		total += nw;
	}

	if (close(fd_s) == -1)
		perror("close source");

	if (close(fd_d) == -1)
		perror("close destination");

	printf("total bytes copied = %ld\n", total);

	return 0;
}
