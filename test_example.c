#include <argeater.h>
#include <alloca.h>
#include <stddef.h>
#include <stdio.h>
#include <stdbool.h>

// declare variables for the action map:
const char *help_flag = NULL;
const char *filename = NULL;
const char *verbose_flag = NULL;
const char *favcolor = NULL;
bool silent = 0;

// attach variables to actions:
AE_ITEM actions[] = {
   { &help_flag,    "help",     'h',  AET_FLAG_OPTION },
   { &filename,     "filename", '\0', AET_ARGUMENT },
   { &verbose_flag, "verbose",  'v',  AET_FLAG_OPTION },
   { &favcolor,     "favcolor", 'c',  AET_VALUE_OPTION },

   // This is a special case.  Using argeater_bool_setter
   // requires the AET_FLAG_OPTION flag to activate the setter.
   // However, the value argument of the setter is ignored by
   // argeater_bool_setter
   { (const char **)&silent, "silent", 's', AET_FLAG_OPTION,
     NULL, NULL, argeater_bool_setter }
};

// Use macro to intializa a AE_MAP instance:
AE_MAP action_map = INIT_MAP(actions);

// Use library functions to build a usage display:
void show_help(AE_MAP *map, const char *cmdname)
{
   argeater_show_usage(map, cmdname);
   argeater_show_options(map, 3);
   argeater_show_arguments(map, 3);
}

int main(int argc, const char **argv)
{
   // Collect arguments into a linked list:
   // ACLONE *clones = argeater_clone_args( alloca(argc * sizeof(ACLONE)), argc, argv);
   ACLONE *clones = CLONE_ARGS(argc, argv);

   // "eat" the arguments to set variables:
   if (argeater_process(clones, &action_map))
   {
      // Read mapped variables and respond according to plan
      if (help_flag)
      {
         // (First CL argument is command name:)
         show_help(&action_map, *argv);
         return 1;
      }

      argeater_dump_actions(&action_map);
      printf("silent is '%d'\n", silent);
   }
   else
   {
      fprintf(stderr, "Failure during argument procession.\n");
      return 1;
   }

   return 0;
}
