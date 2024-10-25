#include <stdio.h>
#include <stdarg.h>
#include "argeater.h"
#include "export.h"

int aesink_to_stderr(const char *format, ...)
{
   int rval = 0;
   va_list args;
   va_start(args, format);

   rval = vfprintf(stderr, format, args);

   va_end(args);

   return rval;
}

int aesink_to_stdout(const char *format, ...)
{
   int rval = 0;
   va_list args;
   va_start(args, format);

   rval = vprintf(format, args);

   va_end(args);

   return rval;
}

int aesink_to_null(const char *format, ...)
{
   return 0;
}


ae_error_sink AE_ESINK = aesink_to_stderr;


EXPORT ae_error_sink argeater_set_error_sink(ae_error_sink replacement)
{
   ae_error_sink cur = AE_ESINK;
   AE_ESINK = replacement;
   return cur;
}
