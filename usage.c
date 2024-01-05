#include <stdio.h>     // for printf()
#include <ctype.h>     // for toupper()
#include <string.h>    // for printf
#include <stdlib.h>    // for exit()
#include <alloca.h>

#include "argeater.h"

/**
 * @brief Print AE_ITEM::help_value or uppercased AE_ITEM::name.
 * @param "item"   reference item
 */
void print_action_content(AE_ITEM *item)
{
   const char *val = item->help_value;
   if (val == NULL)
   {
      if (item->name)
      {
         int len = strlen(item->name);
         char *temp_name = (char*)alloca(len+1);
         char *tptr = temp_name;
         const char *sptr = item->name;
         while (*sptr)
            *tptr++ = toupper(*sptr++);
         *tptr = '\0';
         val = temp_name;
      }
      else
      {
         fprintf(stderr,
                 "AE_ITEM::name or ::help_value must be set for "
                 "print_action_content.\n");
         exit(1);
      }
   }

   printf(val);
}

void show_action(AE_ITEM *item, int indent)
{
   if (indent)
      printf("%*s", indent, "");

   const char *space = item->name ? ", " : "";
   if (item->chr)
      printf("-%c%s", item->chr, space);
   if (item->name)
   {
      printf("--%s", item->name);
      if (AE_IS_VALUE_OPTION(item))
      {
         printf("=");
         print_action_content(item);
      }
   }
   printf("\n");
}

void argeater_show_usage(AE_MAP *map, const char *cmd_name)
{
   printf("USAGE: %s", cmd_name);

   int position_actions = 0;
   int option_actions = 0;

   AE_ITEM *ptr = map->items;
   AE_ITEM *end = ptr + map->count;

   // Count action types to improve formatting
   while (ptr < end)
   {
      if (AE_IS_POSITION(ptr))
         ++position_actions;
      else
         ++option_actions;

      ++ptr;
   }

   if (option_actions)
      printf(" [OPTIONS]");

   if (position_actions)
   {
      ptr = map->items;
      while (ptr < end)
      {
         if (AE_IS_POSITION(ptr))
         {
            const char *act = ptr->help_value;
            if (act == NULL)
               act = ptr->name;

            if (act)
            {
               printf(" ");
               print_action_content(ptr);
            }
            else
               printf(" UNNAMED");
         }
         ++ptr;
      }
   }

   printf("\n");
}

void argeater_show_options(AE_MAP *map, int indent)
{
   AE_ITEM *ptr = map->items;
   AE_ITEM *end = ptr + map->count;

   while (ptr < end)
   {
      if (AE_IS_OPTION(ptr))
      {
         show_action(ptr, indent);
         if (ptr->help_desc)
            printf("%*s   %s\n", indent, "",  ptr->help_desc);
      }

      ++ptr;
   }
}

void argeater_show_arguments(AE_MAP *map, int indent)
{
   AE_ITEM *ptr = map->items;
   AE_ITEM *end = ptr + map->count;

   while (ptr < end)
   {
      if (AE_IS_POSITION(ptr))
      {
         printf("%*s", indent, "");
         if (ptr->name || ptr->help_value)
            print_action_content(ptr);
         else
            printf("UNNAMED");
         
         printf("\n");
         if (ptr->help_desc)
            printf("%*s   %s\n", indent, "",  ptr->help_desc);
      }

      ++ptr;
   }
}

#define UNNAMED "UNNAMED"

void argeater_dump_actions(AE_MAP *map)
{
   AE_ITEM *end = map->items + map->count;
   int max_name_len = 0;

   // For formatting, get largest name length
   for (AE_ITEM *ptr = map->items; ptr < end; ++ptr)
   {
      int cur_name_len = 0;
      if (ptr->name)
         cur_name_len = strlen(ptr->name);
      else
         cur_name_len = strlen(UNNAMED);

      if (cur_name_len > max_name_len)
         max_name_len = cur_name_len;
   }

   for (AE_ITEM *ptr = map->items; ptr < end; ++ptr)
   {
      const char *str = *ptr->var;
      if (str == NULL)
         str = "NULL";

      const char *name = ptr->name;
      if (name == NULL)
         name = UNNAMED;

      printf("%-*s: '%s'\n", max_name_len, name, str);
   }
}
