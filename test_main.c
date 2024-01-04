#include <stdio.h>
#include <alloca.h>
#include "argeater.h"

const char *show_help = NULL;
const char *indexed = NULL;
const char *flag1 = NULL;
const char *flag2 = NULL;
const char *flag3 = NULL;
const char *filename = NULL;
const char *margins = NULL;
const char *username = NULL;
const char *userstate = NULL;

AE_ITEM ma_items[] = {
   { 'h', "help",     ATYPE_FLAG_OPTION,   &show_help, NULL, "This screen" },
   { 'i', "index",    ATYPE_FLAG_OPTION,   &indexed,   NULL, "show line indexes" },
   { '1', "flag1",    ATYPE_FLAG_OPTION,   &flag1,     NULL, "set first flag" },
   { '2', "flag2",    ATYPE_FLAG_OPTION,   &flag2,     NULL, "set second flag" },
   { '3', "flag3",    ATYPE_FLAG_OPTION,   &flag3,     NULL, "set third flag" },
   { 'f', "filename", ATYPE_VALUE_OPTION,  &filename,  NULL, "file to read" },
   { 'm', "margins",  ATYPE_VALUE_OPTION,  &margins,
     "top,right,bottom,left", "content margins" },
   { 0,   "name",     0,                   &username,  NULL, "User name" },
   { 0,   "state",    0,                   &userstate, "USER_STATE" }
};

AE_MAP arg_map = { ma_items, sizeof(ma_items) / sizeof(ma_items[0]) };

int main(int argc, const char **argv)
{
   // argeater(argc, argv, &arg_map, &arg_map, NULL);

   ACLONE *clones = (ACLONE*)alloca(sizeof(ACLONE)*argc);
   argeater_clone_args(clones, argc, argv);

   argeater_process(clones, &arg_map, NULL);
   if (show_help)
   {
      argeater_show_usage(&arg_map, *argv);
      printf("Options:\n");
      argeater_show_options(&arg_map, 3);
      printf("Arguments:\n");
      argeater_show_arguments(&arg_map, 3);
   }
   else
   {
      printf("indexed: '%s'\n", indexed);
      printf("flag1: '%s'\n", flag1);
      printf("flag2: '%s'\n", flag2);
      printf("flag3: '%s'\n", flag3);
      printf("filename: '%s'\n", filename);
      printf("name: '%s'\n", username);
      printf("state: '%s'\n", userstate);
   }

   return 0;
}
