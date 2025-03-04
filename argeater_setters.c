#include <stdbool.h>
#include <errno.h>
#include <string.h>   // strerror()
#include <stdio.h>
#include <fcntl.h>    // open()
#include <stdlib.h>   // strtol()
#include "export.h"

#include "argeater.h"

// Found in `error_sinks.c`
extern ae_error_sink AE_ESINK;

EXPORT bool argeater_string_setter(const char **target, const char *value)
{
   *target = value;
   return true;
}

EXPORT bool argeater_bool_setter(const char **target, const char *value)
{
   *(bool*)target = 1;
   return true;
}

EXPORT bool argeater_int_setter(const char **target, const char *value)
{
   char *end;
   errno = 0;
   long lval = strtol(value, &end, 10);
   if (errno)
   {
      (*AE_ESINK)("Error interpreting '%s' as an integer: %s.\n", value, strerror(errno));
      return false;
   }
   if (end == value)
   {
      (*AE_ESINK)("Failed to interpret '%s' as an integer.\n", value);
      return false;
   }

   *(int*)target = (int)lval;
   return true;
}

EXPORT bool argeater_ro_stream_setter(const char **target, const char *value)
{
   FILE *stream = fopen(value, "r");
   if (stream)
   {
      *(FILE**)target = stream;
      return true;
   }
   else
      (*AE_ESINK)("Failed to open '%s': %s.\n", value, strerror(errno));

   return false;
}

EXPORT bool argeater_ro_file_setter(const char **target, const char *value)
{
   int fh = open(value, O_RDONLY);
   if (fh)
   {
      *(int*)target = fh;
      return true;
   }
   else
      (*AE_ESINK)("Failed to open '%s': %s.\n", value, strerror(errno));

   return false;
}
