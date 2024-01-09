#include <string.h>
#include <assert.h>
#include "argeater.h"
#include "export.h"

EXPORT ACLONE *argeater_clone_args(ACLONE *clones, int argc, const char **argv)
{
   ACLONE *lptr = clones;
   ACLONE *lend = lptr + argc;

   const char **sptr = argv;
   const char **send = sptr + argc;

   while (lptr < lend)
   {
      assert(sptr < send);

      // Save arg reference
      memset(lptr, 0, sizeof(ACLONE));
      lptr->val = *sptr;
      // Attach to previous link (if not the first entry):
      if (lptr > clones)
         (lptr-1)->next = lptr;

      ++lptr;
      ++sptr;
   }

   return clones;
}
