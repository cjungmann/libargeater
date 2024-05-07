/**
 * @file test_simple.c
 * This file contains code that was copied directly from the
 * examples section of the man page, with some modification
 * to integrate code from the *action_setter* example.
 *
 * *action_setter* code fragments can be identified by a
 * leading comment mentioning the example.
 */


#include <argeater.h>
#include <alloca.h>
#include <stddef.h>
#include <stdio.h>

#include <stdarg.h>

// from *action_setter* example
#include <stdlib.h>  // for strtol

// declare variables for the action map:
const char *help_flag = NULL;
const char *filename = NULL;
const char *verbose_flag = NULL;
const char *favcolor = NULL;
static int ival = 0;

// from *action_setter* example
int debug_level = 0;

// Replacement error sink function
int red_printf(const char *format, ...)
{
   int rval = 0;
   va_list args;
   va_start(args, format);
   printf("\x1b[31;1m");
   rval = vprintf(format, args);
   printf("\x1b[m");
   fflush(stdout);
   va_end(args);
   return rval;
}

// from *action_setter_example
// Implement the handler function.
bool set_debug_level(const char **target, const char *arg)
{
   char *end;
   int level = (int)strtol(arg, &end, 10);
   if (end == arg)
      fprintf(stderr, "'%s' is not a number.\n", arg);
   else if (level<0 || level>3)
      fprintf(stderr, "'%d' is not a valid debug level.\n", level);
   else
   {
      *(int*)target = level;
      return true;
   }

   return false;
}

// attach variables to actions:
AE_ITEM actions[] = {
   { &help_flag,    "help",     'h',  AET_FLAG_OPTION },
   { &filename,     "filename", '\0', AET_ARGUMENT },
   { &verbose_flag, "verbose",  'v',  AET_FLAG_OPTION },
   { &favcolor,     "favcolor", 'c',  AET_VALUE_OPTION },

   // Validate utility of argeater_set_error_sink/red_printf:
   { (const char **)&ival, "intval", 'i', AET_VALUE_OPTION,
     "Enter an integer value", NULL, argeater_int_setter },

   // from *action_setter* example:
   { (const char**)&debug_level,
     "deblev",
     'd',
     AET_VALUE_OPTION,
     "The debug level can be 0, 1, 2, or 3",
     "debug level",
     set_debug_level }
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
   if (argc < 2)
   {
      show_help(&action_map, *argv);
      return 1;
   }

   argeater_set_error_sink(red_printf);

   // Collect arguments into a linked list:
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
      else
         argeater_dump_actions(&action_map);
   }
   else
      fprintf(stderr, "Error processing arguments.\n");

   return 0;
}
