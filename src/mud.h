/*
 * This is the main headerfile
 */

#ifndef MUD_H
#define MUD_H

#include <zlib.h>
#include <pthread.h>
#include <arpa/telnet.h>

#include "list.h"
#include "stack.h"

/************************
 * Standard definitions *
 ************************/

/* define TRUE and FALSE */
#ifndef FALSE
#define FALSE   0
#endif
#ifndef TRUE
#define TRUE    1
#endif

#define eTHIN   0
#define eBOLD   1

/* A few globals */
#define PULSES_PER_SECOND     5                   /* must divide 1000 : 4, 5 or 8 works */
#define MAX_BUFFER         32768                  /* seems like a decent amount         */
#define MAX_OUTPUT         32768                  /* well shoot me if it isn't enough   */
#define MAX_HELP_ENTRY     32768                  /* roughly 40 lines of blocktext      */
#define MAX_KEY_HASH       1024
#define MUDPORT            1801                   /* just set whatever port you want    */
#define FILE_TERMINATOR    "EOF"                  /* end of file marker                 */
#define COPYOVER_FILE      "../txt/copyover.dat"  /* tempfile to store copyover data    */
#define EXE_FILE           "../src/arcanum"     /* the name of the mud binary         */

#define CLEAR_SCREEN	   "\033[H\033[J"

/* Connection states */
#define STATE_NEW_NAME         0
#define STATE_NEW_PASSWORD     1
#define STATE_VERIFY_PASSWORD  2
#define STATE_ASK_PASSWORD     3
#define STATE_ASK_GENDER       4
#define STATE_INTO_GAME	       5
#define STATE_PLAYING          6
#define STATE_CLOSED           7

/* Thread states - please do not change the order of these states    */
#define TSTATE_LOOKUP          0  /* Socket is in host_lookup        */
#define TSTATE_DONE            1  /* The lookup is done.             */
#define TSTATE_WAIT            2  /* Closed while in thread.         */
#define TSTATE_CLOSED          3  /* Closed, ready to be recycled.   */

/* player levels */
#define LEVEL_GUEST            1  /* Dead players and actual guests  */
#define LEVEL_PLAYER           2  /* Almost everyone is this level   */
#define LEVEL_ADMIN            3  /* Any admin without shell access  */
#define LEVEL_GOD              4  /* Any admin with shell access     */

/* Communication Ranges */
#define COMM_LOCAL             0  /* same room only                  */
#define COMM_LOG              10  /* admins only                     */

// Room Sectors
#define SECTOR_FOREST		 0
#define SECTOR_CAVE		 1
#define SECTOR_CITY		 2
#define SECTOR_MOUNTAIN		 3
#define SECTOR_DESERT		 4
#define SECTOR_SWAMP		 5
#define SECTOR_PLAIN		 6
#define SECTOR_SHORE		 7
#define SECTOR_INTERIOR		 8
#define SECTOR_WATER		 9
#define SECTOR_UNDERWATER	10
#define MAX_SECTORS		11

/* define simple types */
typedef  unsigned char     bool;
typedef  short int         sh_int;


/******************************
 * End of standard definitons *
 ******************************/

/***********************
 * Defintion of Macros *
 ***********************/

#define LOWER(c)        ((c) >= 'A' && (c) <= 'Z' ? (c)+'a'-'A' : (c))
#define UMIN(a, b)		((a) < (b) ? (a) : (b))
#define IS_ADMIN(ch)          ((ch->level) > LEVEL_PLAYER ? TRUE : FALSE)
#define IREAD(sKey, sPtr)             \
{                                     \
  if (!strcasecmp(sKey, word))        \
  {                                   \
    int sValue = fread_number(fp);    \
    sPtr = sValue;                    \
    found = TRUE;                     \
    break;                            \
  }                                   \
}
#define SREAD(sKey, sPtr)             \
{                                     \
  if (!strcasecmp(sKey, word))        \
  {                                   \
    sPtr = fread_string(fp);          \
    found = TRUE;                     \
    break;                            \
  }                                   \
}

/***********************
 * End of Macros       *
 ***********************/

/******************************
 * Bit flag Support           *
 ******************************/
#define IS_SET(flag, bit)       ((flag) & (bit))
#define SET_BIT(var, bit)       ((var) |= (bit))
#define REMOVE_BIT(var, bit)    ((var) &= ~(bit))
#define TOGGLE_BIT(var, bit)    ((var) ^= (bit))
/* Easy reading */
#define A                       0x1
#define B                       0x2
#define C                       0x4
#define D                       0x8
#define E                       0x10
#define F                       0x20
#define G                       0x40
#define H                       0x80
#define I                       0x100
#define J                       0x200
#define K                       0x400
#define L                       0x800
#define M                       0x1000
#define N                       0x2000
#define O                       0x4000
#define P                       0x8000
#define Q                       0x10000
#define R                       0x20000
#define S                       0x40000
#define T                       0x80000
#define U                       0x100000
#define V                       0x200000
#define W                       0x400000
#define X                       0x800000
#define Y                       0x1000000
#define Z                       0x2000000
#define aa			0x8000000
#define bb			0x10000000
#define cc			0x20000000
#define dd			0x40000000
#define ee			0x80000000

/******************************
 * New structures             *
 ******************************/

/* type defintions */
typedef struct  dSocket       D_SOCKET;
typedef struct  char_data     CHAR_DATA;
typedef struct  help_data     HELP_DATA;
typedef struct  lookup_data   LOOKUP_DATA;
typedef struct  event_data    EVENT_DATA;

/* the actual structures */
struct dSocket
{
  CHAR_DATA      * player;
  LIST          * events;
  char          * hostname;
  char            inbuf[MAX_BUFFER];
  char            outbuf[MAX_OUTPUT];
  char            next_command[MAX_BUFFER];
  bool            bust_prompt;
  sh_int          lookup_status;
  sh_int          state;
  sh_int          control;
  sh_int          top_output;
  unsigned char   compressing;                 /* MCCP support */
  z_stream      * out_compress;                /* MCCP support */
  unsigned char * out_compress_buf;            /* MCCP support */
};

struct char_data
{
  //Shared data for PC and NPC
  D_SOCKET      * socket;
  LIST          * events;
  char          * name;
  char          * password;
  sh_int          level;
};

struct help_data
{
  time_t          load_time;
  char          * keyword;
  char          * text;
};

struct lookup_data
{
  D_SOCKET       * dsock;   /* the socket we wish to do a hostlookup on */
  char           * buf;     /* the buffer it should be stored in        */
};

struct typCmd
{
  char      * cmd_name;
  void     (* cmd_funct)(CHAR_DATA *ch, char *arg);
  sh_int      level;
};

typedef struct buffer_type
{
  char   * data;        /* The data                      */
  int      len;         /* The current len of the buffer */
  int      size;        /* The allocated size of data    */
} BUFFER;

/* here we include external structure headers */
#include "event.h"

/******************************
 * End of new structures      *
 ******************************/

/***************************
 * Global Variables        *
 ***************************/

extern  STACK       *   dsock_free;       /* the socket free list               */
extern  LIST        *   dsock_list;       /* the linked list of active sockets  */
extern  STACK       *   char_free;     /* the char free list               */
extern  LIST        *   char_list;     /* the char list of active chars  */
extern  LIST        *   help_list;        /* the linked list of help files      */
extern  const struct    typCmd tabCmd[];  /* the command table                  */
extern  bool            shut_down;        /* used for shutdown                  */
extern  char        *   greeting;         /* the welcome greeting               */
extern  char        *   motd;             /* the MOTD help file                 */
extern  int             control;          /* boot control socket thingy         */
extern  time_t          current_time;     /* let's cut down on calls to time()  */



/*************************** 
 * End of Global Variables *
 ***************************/

/***********************
 *    MCCP support     *
 ***********************/

extern const unsigned char compress_will[];
extern const unsigned char compress_will2[];

#define TELOPT_COMPRESS       85
#define TELOPT_COMPRESS2      86
#define COMPRESS_BUF_SIZE   8192

/***********************
 * End of MCCP support *
 ***********************/

/***********************************
 * Prototype function declerations *
 ***********************************/

/* more compact */
#define  D_S         D_SOCKET
#define  C_D         CHAR_DATA

#define  buffer_new(size)             __buffer_new     ( size)
#define  buffer_strcat(buffer,text)   __buffer_strcat  ( buffer, text )

char  *crypt                  ( const char *key, const char *salt );

/*
 * socket.c
 */
int   init_socket             ( void );
bool  new_socket              ( int sock );
void  close_socket            ( D_S *dsock, bool reconnect );
bool  read_from_socket        ( D_S *dsock );
bool  text_to_socket          ( D_S *dsock, const char *txt );  /* sends the output directly */
void  text_to_buffer          ( D_S *dsock, const char *txt );  /* buffers the output        */
void  text_to_char          ( C_D *ch, const char *txt );   /* buffers the output        */
void  next_cmd_from_buffer    ( D_S *dsock );
bool  flush_output            ( D_S *dsock );
void  handle_new_connections  ( D_S *dsock, char *arg );
void  clear_socket            ( D_S *sock_new, int sock );
void  recycle_sockets         ( void );
void *lookup_address          ( void *arg );

/*
 * interpret.c
 */
void  handle_cmd_input        ( D_S *dsock, char *arg );

/*
 * io.c
 */
void    log_string            ( const char *txt, ... );
void    bug                   ( const char *txt, ... );
time_t  last_modified         ( char *helpfile );
char   *read_help_entry       ( const char *helpfile );     /* pointer         */
char   *fread_line            ( FILE *fp );                 /* pointer         */
char   *fread_string          ( FILE *fp );                 /* allocated data  */
char   *fread_word            ( FILE *fp );                 /* pointer         */
int     fread_number          ( FILE *fp );                 /* just an integer */

/* 
 * strings.c
 */
char   *one_arg               ( char *fStr, char *bStr );
char   *strdup                ( const char *s );
int     strcasecmp            ( const char *s1, const char *s2 );
bool    is_prefix             ( const char *aStr, const char *bStr );
char   *capitalize            ( char *txt );
BUFFER *__buffer_new          ( int size );
void    __buffer_strcat       ( BUFFER *buffer, const char *text );
void    buffer_free           ( BUFFER *buffer );
void    buffer_clear          ( BUFFER *buffer );
int     bprintf               ( BUFFER *buffer, char *fmt, ... );
bool compares(const char *aStr, const char *bStr);

/*
 * help.c
 */
bool  check_help              ( C_D *ch, char *helpfile );
void  load_helps              ( void );

/*
 * utils.c
 */
bool  check_name              ( const char *name );
void  clear_char            ( C_D *ch );
void  free_char             ( C_D *ch );
void  communicate             ( C_D *ch, char *txt, int range );
void  load_muddata            ( bool fCopyOver );
char *get_time                ( void );
void  copyover_recover        ( void );
C_D  *check_reconnect         ( char *player );
int   rand_number(int min, int max);


/*
 * action_safe.c
 */
void  cmd_say                 ( C_D *ch, char *arg );
void  cmd_quit                ( C_D *ch, char *arg );
void  cmd_shutdown            ( C_D *ch, char *arg );
void  cmd_commands            ( C_D *ch, char *arg );
void  cmd_who                 ( C_D *ch, char *arg );
void  cmd_help                ( C_D *ch, char *arg );
void  cmd_compress            ( C_D *ch, char *arg );
void  cmd_save                ( C_D *ch, char *arg );
void  cmd_copyover            ( C_D *ch, char *arg );
void  cmd_linkdead            ( C_D *ch, char *arg );

/*
 * mccp.c
 */
bool  compressStart           ( D_S *dsock, unsigned char teleopt );
bool  compressEnd             ( D_S *dsock, unsigned char teleopt, bool forced );

/*
 * save.c
 */
void  save_player             ( C_D *ch );
C_D  *load_player             ( char *player );
C_D  *load_profile            ( char *player );

/*
 * color.c
 */
int substitute_color(char *input, char *output, int colors);


/*******************************
 * End of prototype declartion *
 *******************************/

#endif  /* MUD_H */
