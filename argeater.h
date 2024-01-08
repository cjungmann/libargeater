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
   AET_OPTION = 1,
   AET_VALUE = 2,
   AET_REPEAT = 4
} AE_TYPE;

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
   const char  **var;       ///< variable to set with argument's value
   const char  *name;       ///< for name following a "--"
   char        chr;         ///< for character following a '-'
   AE_TYPE     type;        ///< interpretation flags
   const char  *help_desc;  ///< description of action for help screen
   const char  *help_value; ///< string to use to represent a sample value
};

typedef struct action_map  AE_MAP;
struct action_map {
   AE_ITEM *items;
   int count;
};

#define INIT_MAP(x) { (x), sizeof((x)) / sizeof((x)[0]) }

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

int argeater_process(ACLONE *clones, AE_MAP *map);
void argeater_show_usage(AE_MAP *map, const char *cmd_name);
void argeater_show_options(AE_MAP *map, int indent);
void argeater_show_arguments(AE_MAP *map, int indent);

void argeater_dump_actions(AE_MAP *map);


#endif
