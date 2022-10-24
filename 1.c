#include <stdio.h>
#include <string.h>
#include <memory.h>
#include <fcntl.h>
#include <sys/types.h>
#include <errno.h>
#include <stdint.h>
#include <sys/time.h>
#include <stdlib.h>
#include <unistd.h>

int execute(const char* readingPath, const char* writingPath, size_t* result) {
	int readingFildes;
	int writingFildes;
	
	if ((readingFildes = open(readingPath, O_RDONLY)) == -1) {
		return -1;
	}
	
	if ((writingFildes = open(writingPath, O_WRONLY | O_CREAT | O_TRUNC, 0644)) == -1) {
		close(readingFildes);
		return -1;
	}
	
	char buffer[512];
	size_t writingBuffer = 0;
	size_t readingBuffer = 0;
	
	while ((readingBuffer = read(readingFildes, buffer, 512)) > 0) {
		for (size_t i = 0; i < readingBuffer; i++) {
			if (buffer[i] >= 'a' && buffer[i] <= 'z') {
				buffer[i] += 'A' - 'a';
			}
		}
		
		writingBuffer += readingBuffer;
		
		if (write(writingFildes, buffer, readingBuffer) < readingBuffer) {
			if (result) {
				*result = writingBuffer;
			}
			
			close(writingFildes);
			close(readingFildes);
			
			return -1;
		}
	}
	
	close(writingFildes);
	close(readingFildes);
	
	if (result) {
		*result = writingBuffer;
	}

	if (readingBuffer == -1) {
		return readingBuffer;
	} else {
		return 0;
	}
}

int main(int argc, char **argv) {
	if (argc < 3) {
		printf("You should input names of two files as arguments");
		return -1;
	}
	
	size_t result = 0;
	int isFailed = execute(argv[1], argv[2], &result);
	
	if (isFailed) {
		printf("An error occurred while the execution:  %s", strerror(errno));
	}

	printf("Result = %i", result);
	
	return 0;
}
