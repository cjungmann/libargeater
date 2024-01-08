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

const char *simple = NULL;
const char *simple_named = NULL;
const char *simple_flag = NULL;
const char *simple_option = NULL;

AE_ITEM ma_items[] = {
   { &show_help, "help",     'h', AET_FLAG_OPTION,   NULL, "This screen" },
   { &simple },
   { &simple_named, "simple" },
   { &simple_flag, "simple_flag", 's', AET_FLAG_OPTION },
   { &simple_option, "simple_option", 'o', AET_VALUE_OPTION },
   { &indexed,   "index",    'i', AET_FLAG_OPTION,  "show line indexes" },
   { &flag1,     "flag1",    '1', AET_FLAG_OPTION,  "set first flag" },
   { &flag2,     "flag2",    '2', AET_FLAG_OPTION,  "set second flag" },
   { &flag3,     "flag3",    '3', AET_FLAG_OPTION,  "set third flag" },
   { &filename,  "filename", 'f', AET_VALUE_OPTION, "file to read" },
   { &margins,   "margins",  'm', AET_VALUE_OPTION, "content margins", "top,right,bottom,left" },
   { &username,  "name" },
   { &userstate, "state",    0,   0,                 "State in which user resides" }
};

AE_MAP arg_map = INIT_MAP(ma_items);

int main(int argc, const char **argv)
{
   // argeater(argc, argv, &arg_map, &arg_map, NULL);

   ACLONE *clones = (ACLONE*)alloca(sizeof(ACLONE)*argc);
   argeater_clone_args(clones, argc, argv);

   argeater_process(clones, &arg_map);
   if (show_help)
   {
      argeater_show_usage(&arg_map, *argv);
      printf("Options:\n");
      argeater_show_options(&arg_map, 3);
      printf("Arguments:\n");
      argeater_show_arguments(&arg_map, 3);
   }
   else
      argeater_dump_actions(&arg_map);

   return 0;
}
