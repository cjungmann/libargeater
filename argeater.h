#ifndef ARGEATER_H
#define ARGEATER_H

// This struct is used to make a copy of the arguments
// from which consumed arguments can be discarded.
typedef struct arg_clone ACLONE;
struct arg_clone {
   const char *val;
   ACLONE *next;
};

typedef enum action_type {
   ATYPE_OPTION = 1,
   ATYPE_VALUE = 2,
   ATYPE_REPEAT = 4
} AE_TYPE;

#define ATYPE_FLAG_OPTION (ATYPE_OPTION)
#define ATYPE_VALUE_OPTION (ATYPE_OPTION | ATYPE_VALUE)

#define AE_IS_OPTION(x)       ( (x)->type & ATYPE_OPTION )
#define AE_IS_POSITION(x)     ( ((x)->type & ATYPE_OPTION) == 0 )
#define AE_IS_REPEAT(x)       ( (x)->type & ATYPE_REPEAT )
#define AE_IS_FLAG_OPTION(x)  ( ((x)->type & ATYPE_OPTION) && !((x)->type & ATYPE_VALUE) )
#define AE_IS_VALUE_OPTION(x) ( ((x)->type & ATYPE_OPTION) && (x)->type & ATYPE_VALUE )

typedef struct action_item AE_ITEM;
struct action_item {
   char        chr;         ///< for character following a '-'
   const char  *name;       ///< for name following a "--"
   AE_TYPE     type;        ///< interpretation flags
   const char  **var;       ///< variable to set with argument's value
   const char  *help_value; ///< string to use in help screen
   const char  *help_desc;  ///< description of action for help screen
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
void set_item_value_or_die(AE_ITEM *item, const char *value);
void set_item_flag(AE_ITEM *item);

AE_ITEM *argeater_search_char(AE_MAP *map, char chr);
AE_ITEM *argeater_search_name(AE_MAP *map, const char *name);
/** @} */



void argeater_clone_args(ACLONE *clones, int argc, const char **argv);

int argeater_process(ACLONE *clones, AE_MAP *map, void *data);
void argeater_show_usage(AE_MAP *map, const char *cmd_name);
void argeater_show_options(AE_MAP *map, int indent);
void argeater_show_arguments(AE_MAP *map, int indent);


#endif
