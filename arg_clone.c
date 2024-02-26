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

#ifndef WORD_LIST
/* A structure which represents a word. */
typedef struct word_desc {
  char *word;		/* Zero terminated string. */
  int flags;		/* Flags associated with this word. */
} WORD_DESC;

/* A linked list of words. */
struct word_list {
  struct word_list *next;
  WORD_DESC *word;
};
#endif

EXPORT int argeater_count_word_list(WORD_LIST *wlist)
{
   int count = 0;
   while (wlist)
   {
      ++count;
      wlist = wlist->next;
   }
   return count;
}

EXPORT ACLONE *argeater_clone_word_list(ACLONE *clones, int clone_count, WORD_LIST* wl)
{
   ACLONE *ptr = clones;
   ACLONE *end = ptr + clone_count;

   // Must include fake command argument clone to serve
   // as a handle to access the first unused argument
   ACLONE *tmp = ptr++;
   tmp->val = "fake_app";
   tmp->next = ptr;

   while (ptr < end && wl!=NULL)
   {
      tmp = ptr++;
      tmp->val = wl->word->word;
      tmp->next = (ptr<end?ptr:NULL);

      wl = wl->next;
   }

   // Make sure BOTH last ACLONE and last WORD_LIST item reached:
   assert(ptr >= end && wl == NULL);

   return clones;
}
