#include <fcntl.h>
#include <unistd.h>

static const int DontWait = -1;
static const size_t BufSz = 256;

int main(int argc, char *argv[]) {

	int fd = STDIN_FILENO;
		if (argc > 1)
			fd = open(argv[1], O_RDONLY);


	if (fd < 0) {
		perror("Failed to open file");
		return 1;
	}

	int efd = epoll_create(5);
	struct epoll_event ev = { 0, { 0 } };

	ev.events = EPOLLIN;

	ev.data.fd = fd;
	if (epoll_ctl(efd, EPOLL_CTL_ADD, fd, &ev) < 0) {
		perror("Failed to add fd to event set");
		return 2;
	}
	int wfds = epoll_wait(efd, &ev, 1, DontWait);
	if (wfds > 0) {
		char buf[256];
		ssize_t sz = read(fd, buf, BufSz);
	if (sz > 0) {
		buf[sz - 1] = '\0';
		printf("Read: %s\n", buf);
	} else if (sz < 0) {
		perror("Error while reading");
		return 3;
	}
	} else if (wfds < 0) {
		perror("Failed while waiting");
		return 4;
	}
	return 0;
}
