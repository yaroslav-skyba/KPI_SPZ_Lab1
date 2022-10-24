#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/time.h>
#include <string.h>
#include <errno.h>
#include <stdint.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/select.h>
#include <memory.h>

void execute(const char *data) {
	fd_set fdset;
	size_t readBytesCount;
	char buffer[1025];
	
	while (1) {
		FD_ZERO(&fdset);
		FD_SET(STDIN_FILENO, &fdset);
		
		struct timeval timeout = {
			.tv_sec = 5,
			.tv_usec = 0,
		};
		
		switch (select(1, &fdset, NULL, NULL, &timeout)) {
			case -1:
				dprintf(STDERR_FILENO, "An error occurred while the execution: %s\n", strerror(errno));
				break;
			case 0:
				dprintf(STDERR_FILENO, "5 second: %s\n", data);
				break;
			default:
				if ((readBytesCount = read(STDIN_FILENO, buffer, 1025)) > 0) {
					size_t dataLength = strlen(data);

					if ((write(STDOUT_FILENO, data, dataLength) < dataLength) || (write(STDOUT_FILENO, buffer, readBytesCount) < readBytesCount) ||
						(write(STDOUT_FILENO, " ", 1) < 1)) {
						exit(-1);
					}
				} else {
					exit(-1);
				}
				
				break;
		}
	}
}

int main(int argc, char **argv, char **envp) {
	if (argc < 2) {
		printf("You should input data");
		return -1;
	}

	execute(argv[1]);
	
	return 0;
}
