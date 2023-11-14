#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/mount.h>

int main()
{
   const char* src  = "/home/nefanov";
   const char* tgt = "/var/tmp";

   const unsigned long mntflags = MS_REC | MS_BIND;// (mount (src, tgt, NULL, MS_SILENT | MS_BIND | (recursive ? MS_REC : 0), NULL);

   int result = mount(src, tgt, NULL, mntflags, NULL);

   if (result == 0)
   {
      printf("Mount created at %s...\n", tgt);
      printf("Press <return> to unmount the volume: ");
      getchar();

      umount(tgt);
   }
   else
   {
      printf("Error : Failed to mount %s\n"
             "Reason: %s [%d]\n",
             src, strerror(errno), errno);
      return -1;
   }

   return 0;
}
