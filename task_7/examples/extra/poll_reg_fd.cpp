#include <iostream>

#include <poll.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>


using namespace std;

ssize_t read_out_to_the_end(int fd){
   char chunk[1024];
   ssize_t ret = 0, n;
   while((n = ::read(fd, chunk, sizeof chunk)) > 0){
      ret += n;
      cerr << "read chunk: " << n << " | ";
      cerr.write(chunk, n);
      cerr << endl;
   }
   if (n < 0) {
       cerr << "err in read" << endl;
   }
   else if (ret == 0){
      cerr << "nothing to read" << endl;
   }
   return ret;
}

int main() {
   int bininfd = open("a.out", O_RDONLY | O_CREAT);//, 0644/*S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH*/);
   if (bininfd < 0) {
      perror("err in open(binin)");
      return -1;
   }

   struct pollfd pollfds[] = {
         {bininfd, POLLIN, 0},
   };
   auto&[pfd] = pollfds;

   while (1) {
      pfd.revents = 0;  // cleanup, shouldn't it be redundant
      int pollret = poll(pollfds, 1, -1);
      if (pollret > 0) {
         if (pfd.revents & POLLIN) {
            cerr << "(pfd.revents & POLLIN)" << endl;
            read_out_to_the_end(pfd.fd);
         }
      } else if (pollret == 0) {
         cerr << "poll timed out" << endl;
         continue;
      } else {
         cerr << "check for error" << endl;
         continue;
      }
   }
}
