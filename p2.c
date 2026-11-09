#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

int main(){
	
	int fd;

	fd = open("data.txt", O_WRONLY | O_CREAT, O_TRUNC, 0644);
	if (fd == -1){
		perror("open");
		return 1;
	}

	char *text = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	if (write(fd, text, 26) == -1){
		perror("write");
		close(fd);
		return 1;
	}

	if (close(fd) == -1){
		perror("close");
		return 1;
	}

	fd = open("data.txt", O_RDWR);
	if (fd == -1){
		perror("open");
		return 1;
	}	

	off_t size;
	size = lseek(fd, 0, SEEK_END);
	
	if (size == -1){
		perror("lseek");
		close(fd);
		return 1;
	}

	printf("current file size: %ld bytes\n", size);

	if (truncate("data.txt", 10) == -1){
		perror("truncate");
		close(fd);
		return 1;
	}

	size = lseek(fd, 0 , SEEK_END);

	if (size == -1) {
		perror("lsek");
		close(fd);
		return 1;
	}

	printf("new file size: %ld bytes\n", size);

	if (lseek(fd, 0, SEEK_SET) == -1){
		perror("lseek");
		close(fd);
		return 1;
	}

	char buffer[100];
	ssize_t nr;
	
	nr = read(fd, buffer, sizeof(buffer) - 1);
	if(close(fd) == -1){
		perror("close");
		return 1;
	}

	return 0;
}
