#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>


int main(){

	int fd;
	fd = open("sparse.bin", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1){
		perror("open");
		return 1;
	}

	ssize_t nr = 0;
	nr = write(fd, "START", 5);
	if (nr == -1){
		perror("write START");
		close(fd);
		return 1;
	}

	off_t l_ret;
	l_ret = lseek(fd, (off_t) (1024*1024), SEEK_CUR);
	if (l_ret == -1){
		perror("lseel");
		close(fd);
		return 1;
	}

	nr = write(fd, "END", 3);
	if (nr == -1){
		perror("write END");
		close(fd);
		return 1;
	}

	if (close(fd) == -1)
		perror("close");

	fd = open("sparse.bin", O_RDONLY);
	if(fd == -1){
		perror("open read");
		return 1;
	}

	l_ret = lseek(fd, (off_t)0, SEEK_END);
	if (l_ret == -1){
		perror("lseek end");
		close(fd);
		return 1;
	}

	printf("size of sparse.bin = %ld bytes\n", l_ret);

	if (close(fd) == -1)
		perror("close");

	// apparent size is the logical length of the file. the filesystem does not allocate disk block for the skipped region, which is why it appears as a "hole". du counts the blocks actually allocated, which are the "START" and "EMD" blocks. So it shows 8KB, while the actual size is 1MB.

	return 0;
}
