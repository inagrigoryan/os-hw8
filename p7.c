#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>


ssize_t read_full (int fd, char *buf, size_t len);

int main(){

	int chunk = 4096;

	char path1[256];
	char path2[256];
	printf("enter the first file path ");
	scanf("%255s", path1);
	printf("enter the second file path ");
        scanf("%255s", path2);

	int fd1 = open(path1, O_RDONLY);
	if (fd1 == -1){
		perror("open first");
		return 1;
	}

	int fd2 = open(path2, O_RDONLY);
	if (fd2 == -1){
		perror("open second");
		return 1;
	}

	char buf1[chunk];
	char buf2[chunk];
	off_t offset = 0;

	while (1) {
		ssize_t n1 = read_full(fd1, buf1, chunk);
		ssize_t n2 = read_full(fd2, buf2, chunk);

		if (n1 == -1 || n2 == -1){
			perror("read");
			close(fd1);
			close(fd2);
			return 1;
		}

		ssize_t smaller = (n1 < n2) ? n1 : n2;
		for (int i = 0; i < smaller; i++){
			if (buf1[i] != buf2[i]){
				printf("files differ at byte %ld\n", (long) (offset + i));
				close(fd1);
				close(fd1);
				return 1;
			}
		}
		if (n1 != n2){
			printf("Files differ at byte %ld\n", (long) (offset + smaller));
			close(fd1);
			close(fd2);
			return 1;

		}

		if (n1 == 0)
			break;

		offset += n1;
	}

	printf("Files are identical\n");

	if (close(fd1) == -1)
        	perror("close first");
	if (close(fd2) == -1)
        	perror("close second");



	return 0;
}


ssize_t read_full (int fd, char *buf, size_t len){

	size_t cur = 0;
	while (cur < len){
		ssize_t nr = read(fd, buf + cur, len - cur);
		if (nr == -1)
			return -1;
		if (nr == 0)
			break;
		cur += nr;
	}
	return cur;
}
