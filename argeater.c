#include <stdbool.h>
#include <string.h>    // memset, strlen
#include <stdio.h>     // printf
#include <stdlib.h>    // for exit()
#include <alloca.h>
#include <assert.h>

#include "argeater.h"
#include "export.h"

/**
 * @brief Compare member @ref chr to match a AE_MAP.
 * @param "map"    Action map to search
 * @param "chr"    Character sought in the action map.
 * @return Matching AE_ITEM if matched, NULL if not.
 */
AE_ITEM *argeater_search_char(AE_MAP *map, char chr)
{
   AE_ITEM *ptr = map->items;
   AE_ITEM *end = ptr + map->count;
   while (ptr < end)
   {
      if (ptr->chr == chr)
         return ptr;
      ++ptr;
   }
   return NULL;
}

/**
 * @brief Compare member @ref name to match a AE_MAP.
 * @param "map"    Action map to search
 * @param "name"   Name string sought in the action map.
 * @return Matching AE_ITEM if matched, NULL if not.
 */
AE_ITEM *argeater_search_name(AE_MAP *map, const char *name)
{
   AE_ITEM *ptr = map->items;
   AE_ITEM *end = ptr + map->count;
   while (ptr < end)
   {
      if (0 == strcmp(ptr->name, name))
         return ptr;
      ++ptr;
   }
   return NULL;
}

int set_item_value_or_die(AE_ITEM *item, const char *value)
{
   if (value == NULL || *value == '\0')
   {
      if (item->chr)
         fprintf(stderr, "Option '-%c' needs a value.\n", item->chr);
      else if (item->name)
         fprintf(stderr, "Option '-%s' needs a value.\n", item->name);

      return 1;
   }

   return (*item->setter)(item->target, value);
}

int set_item_flag(AE_ITEM *item)
{
   return (*item->setter)(item->target, "on");
}


/**
 * @brief Find the next appropriate map item
 * @param "item"    pointer-to-pointer in which to return found items
 * @param "map"     Map of items to match
 * @return true if @r *item is usable, false if it's not usable
 *
 * If the final positional item is repeatable, this function will
 * continue to return the final positional item with each further
 * call.
 *
 * This behavior is at odds with the basic concept of setting the
 * associating a variable with an item: repeat application of the
 * repeatable item will overwrite the associated variable.  Repeating
 * position variables probably must be handled during the running of
 * the program rather than just before.
 */
bool find_next_position_item(AE_ITEM **item, AE_MAP *map)
{
   AE_ITEM *ptr = map->items;
   AE_ITEM *end = ptr + map->count;

   if (*item)
   {
      // If already exhausted the list, return 'recent'
      // to reinforce the message:
      if (*item==end)
         return false;

      // Otherwise, recent must be a member of the map
      assert(*item >= ptr && *item < end);

      // If a member of the map, start consideration with
      // the first map item after 'recent':
      if (*item >= ptr && *item < end)
         ptr = *item + 1;
   }

   while (ptr < end)
   {
      if (AE_IS_POSITION(ptr))
      {
         *item = ptr;
         return true;
      }

      ++ptr;
   }

   // Last chance after not finding any more position items:

   // If no more position items, and the last found item is
   // repeatable, leave *item untouched but inform caller that
   // *item is still valid
   if (*item && AE_IS_REPEAT(*item))
      return true;

   // We've exhausted the supply of positional items:
   *item = end;
   return false;
}

/**
 * @brief Parse @p arg to find matching action item and option value
 * @param "item"    pointer to return action item if found
 * @param "value"   pointer to return value if included
 * @param "map"     action map in which to seek a matching action item
 * @param "arg"     argument string to parse
 * @return true if everything is good, otherwise false
 *
 * Isolate this logic to allow use of a convenient stack char buffer
 * to copy the name part of @p arg for searching the @p map.
 *
 * Two important things to know about this function,
 * 1. Errors will result in an early exit, with the
 *    error type reported to stderr.
 * 2. The @p value, if set, will point to memory in the
 *    submitted @p arg, so the memory to which it points
 *    should be global and static (and safe to use).
 */
bool argeater_item_from_name(AE_ITEM **item,
                            const char **value,
                            AE_MAP *map,
                            const char *arg)
{
   const char *potential_value = NULL;
   const char *option_name = NULL;

   const char *equals = strchr(arg, '=');
   if (equals && equals > arg)
   {
      potential_value = equals+1;
      int namelen = equals - arg;
      char *temp_name = (char*)alloca(namelen + 1);
      memcpy(temp_name, arg, namelen);
      temp_name[namelen] = '\0';
      option_name = temp_name;
   }
   else
      option_name = arg;

   AE_ITEM *found_item = argeater_search_name(map, option_name);
   if (found_item)
   {
      if (AE_IS_VALUE_OPTION(found_item))
      {
         if (potential_value)
         {
            *item = found_item;
            *value = potential_value;
            return true;
         }
         else
            fprintf(stderr, "Option '--%s' must take a value.\n", option_name);
      }
      else if (AE_IS_FLAG_OPTION(found_item))
      {
         if (potential_value == NULL)
         {
            *item = found_item;
            *value = NULL;
            return true;
         }
         else
            fprintf(stderr, "Option '--%s' does not take a value.\n", option_name);
      }
      else
         fprintf(stderr, "Unrecognized option '--%s'\n", option_name);
   }
   else
      fprintf(stderr, "Unrecognized option  '--%s'\n", option_name);

   exit(1);
}

bool argeater_string_setter(const char **target, const char *value)
{
   *target = value;
   return true;
}

bool argeater_int_setter(const char **target, const char *value)
{
   char *end;
   errno = 0;
   long lval = strtol(value, &end, 10);
   if (errno)
   {
      printf("Error interpreting '%s' as an integer: %s.\n", value, strerror(errno));
      return false;
   }
   if (end == value)
   {
      printf("Failed to interpret '%s' as an integer.\n", value);
      return false;
   }

   *(int*)target = (int)lval;
   return true;
}

void argeater_set_missing_actions(AE_MAP *map)
{
   AE_ITEM *ptr = map->items;
   AE_ITEM *end = ptr + map->count;
   while (ptr < end)
   {
      if (ptr->setter == NULL)
         ptr->setter = argeater_string_setter;

      ++ptr;
   }
}

EXPORT bool argeater_process(ACLONE *args, AE_MAP *map)
{
   bool success = true;
   bool reading_options = true;
   AE_ITEM *last_position_item = NULL;

   argeater_set_missing_actions(map);

   ACLONE *aptr = args;
   while (aptr->next && success)
   {
      ACLONE *cptr = aptr->next;
      const char *str = cptr->val;

      // Reset with each loop, these also serve as
      // a flag to indicate how to interpret the current
      // argument
      AE_ITEM *item = NULL;
      const char *arg_name = NULL;
      const char *arg_char = NULL;

      // Handling some sort of option, that is, starting
      // with one or two dashes, but treating "-" as a
      // value for a positional item
      if (reading_options && str[0] == '-' && str[1] != '\0')
      {
         // double-dash prefix
         if (str[1] == '-')
         {
            if (str[2] == '\0')
               reading_options = false;
            else
               arg_name = &str[2];
         }
         // single-dash prefix
         else
            arg_char = &str[1];
      }
      // Assign to next position arg
      else if (find_next_position_item(&last_position_item, map))
         *(last_position_item->target) = str;

      if (arg_name)
      {
         const char *value;
         if (argeater_item_from_name(&item, &value, map, arg_name))
         {
            if (AE_IS_VALUE_OPTION(item))
               success = set_item_value_or_die(item, value);
            else
               success = set_item_flag(item);
         }
      }
      else if (arg_char)
      {
         while (*arg_char)
         {
            item = argeater_search_char(map, *arg_char);
            if (item)
            {
               if (AE_IS_VALUE_OPTION(item))
               {
                  if (*(arg_char+1))
                  {
                     success = set_item_value_or_die(item, (++arg_char));
                     break;
                  }
                  else
                  {
                     cptr = cptr->next;
                     success = set_item_value_or_die(item, cptr->val);
                     aptr->next = cptr;
                  }
               }
               else if (AE_IS_FLAG_OPTION(item))
                  success = set_item_flag(item);
            }
            else
            {
               fprintf(stderr, "Unrecognized option '-%c'\n", *arg_char);
               exit(1);
            }

            ++arg_char;
         }
      }

      aptr = aptr->next;
   }

   return success;
}


