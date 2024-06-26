#ifndef ARGEATER_H
#define ARGEATER_H

#include <stdbool.h>
#include <alloca.h>

// This struct is used to make a copy of the arguments
// from which consumed arguments can be discarded.
typedef struct arg_clone ACLONE;
struct arg_clone {
   const char *val;
   ACLONE *next;
};

// Fake this to compile before bash development library installed:
#ifndef word_list
typedef struct word_list WORD_LIST;
#endif

typedef enum action_type {
   AET_OPTION = 1,
   AET_VALUE = 2,
   AET_REPEAT = 4
} AE_TYPE;

typedef bool (*action_setter)(const char **target, const char *value);
typedef int (*ae_error_sink)(const char *format, ...);

#define AET_ARGUMENT     (0)
#define AET_FLAG_OPTION  (AET_OPTION)
#define AET_VALUE_OPTION (AET_OPTION | AET_VALUE)

#define AE_IS_OPTION(x)       ( (x)->type & AET_OPTION )
#define AE_IS_POSITION(x)     ( ((x)->type & AET_OPTION) == 0 )
#define AE_IS_REPEAT(x)       ( (x)->type & AET_REPEAT )
#define AE_IS_FLAG_OPTION(x)  ( ((x)->type & AET_OPTION) && !((x)->type & AET_VALUE) )
#define AE_IS_VALUE_OPTION(x) ( ((x)->type & AET_OPTION) && (x)->type & AET_VALUE )

typedef struct action_item AE_ITEM;
struct action_item {
   const char    **target;    ///< variable to set with argument's value
   const char    *name;       ///< for name following a "--"
   char          chr;         ///< for character following a '-'
   AE_TYPE       type;        ///< interpretation flags
   const char    *help_desc;  ///< description of action for help screen
   const char    *help_value; ///< string to use to represent a sample value
   action_setter setter;
};

typedef struct action_map  AE_MAP;
struct action_map {
   AE_ITEM *items;
   int count;
};


/**
 * @defgroup InternalFunctions Internal Funtions
 * @{
 */
int set_item_value_or_die(AE_ITEM *item, const char *value);
int set_item_flag(AE_ITEM *item);

AE_ITEM *argeater_search_char(AE_MAP *map, char chr);
AE_ITEM *argeater_search_name(AE_MAP *map, const char *name);
/** @} */

// implemented in `error_sinks.c`:
/**
 * @defgroup Error Sink functions
 * implemented in @file error_sinks.c
 * @{
 */
int aesink_to_stderr(const char *format, ...);
int aesink_to_stdout(const char *format, ...);
int aesink_to_null(const char *format, ...);
ae_error_sink argeater_set_error_sink(ae_error_sink replacement);
/** @} */

ACLONE *argeater_clone_args(ACLONE *clones, int argc, const char **argv);
ACLONE *argeater_clone_word_list(ACLONE *clones, int clone_count, WORD_LIST* wlist);
int argeater_count_word_list(WORD_LIST *wlist);


bool argeater_process(ACLONE *clones, AE_MAP *map);
void argeater_show_usage(AE_MAP *map, const char *cmd_name);
void argeater_show_options(AE_MAP *map, int indent);
void argeater_show_arguments(AE_MAP *map, int indent);

void argeater_dump_actions(AE_MAP *map);

bool argeater_string_setter(const char **target, const char *value);
bool argeater_bool_setter(const char **target, const char *value);
bool argeater_int_setter(const char **target, const char *value);
bool argeater_ro_stream_setter(const char **target, const char *value);
bool argeater_ro_file_setter(const char **target, const char *value);

#define INIT_MAP(X) { (X), sizeof((X)) / sizeof((X)[0]) }
#define CLONE_ARGS(ARGC,ARGV)\
   (argeater_clone_args(\
      alloca((ARGC)*sizeof(ACLONE)),\
      (ARGC),\
      (ARGV)))

#define CLONES_FROM_LIST_FINAL(wl,c)            \
   argeater_clone_word_list(                    \
      (alloca((c)*sizeof(ACLONE))), (c), (wl))

#define CLONE_WORD_LIST(wl) CLONES_FROM_LIST_FINAL((wl), argeater_count_word_list((wl))+1)

#endif
