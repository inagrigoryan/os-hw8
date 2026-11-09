#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

int main(){

	char path[100];
	printf("enter path to existing file: ");
	scanf("%99s", path);
	
	int fd;

	fd = open(path, O_RDONLY);

	if (fd == -1){
		perror("open");
		return 1;
	}


	off_t size = lseek(fd, 0, SEEK_END);
	if (size == -1){
		perror("lseek");
		close(fd);
		return 1;
	}

	char c;

	for (int i = size - 1; i >= 0; i--){
		if (lseek(fd, i, SEEK_SET) == -1){
			perror("lseek");
			close(fd);
			return 1;
		}

		if (read(fd, &c, 1) == -1){
			perror("read");
			close(fd);
			return 1;
		}

		if (write(1, &c, 1) == -1){
			perror("write");
			close(fd);
			return 1;
		}
	
	}

	if (write(1, "\n", 1) == -1){
		perror("write");
		close(fd);
		return 1;
	}

	if (close(fd) == -1){
		perror("close");
		return 1;
	}
	
	return 0;
}
