
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C
   
      Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.4.1"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Copy the first part of user declarations.  */

/* Line 189 of yacc.c  */
#line 85 "pac_parse.yy"


#include "pac_action.h"
#include "pac_analyzer.h"
#include "pac_array.h"
#include "pac_attr.h"
#include "pac_case.h"
#include "pac_common.h"
#include "pac_conn.h"
#include "pac_context.h"
#include "pac_cstr.h"
#include "pac_dataptr.h"
#include "pac_dataunit.h"
#include "pac_dbg.h"
#include "pac_decl.h"
#include "pac_embedded.h"
#include "pac_enum.h"
#include "pac_exception.h"
#include "pac_expr.h"
#include "pac_exttype.h"
#include "pac_flow.h"
#include "pac_func.h"
#include "pac_id.h"
#include "pac_inputbuf.h"
#include "pac_let.h"
#include "pac_output.h"
#include "pac_param.h"
#include "pac_paramtype.h"
#include "pac_primitive.h"
#include "pac_record.h"
#include "pac_redef.h"
#include "pac_regex.h"
#include "pac_state.h"
#include "pac_strtype.h"
#include "pac_type.h"
#include "pac_utils.h"
#include "pac_withinput.h"

extern int yyerror(const char msg[]);
extern int yylex();
extern int yychar;
extern char* yytext;
extern int yyleng;
extern void begin_RE();
extern void end_RE();

extern string input_filename;
extern int line_number;
extern Output* header_output;
extern Output* source_output;



/* Line 189 of yacc.c  */
#line 127 "pac_parse.cc"

/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     TOK_TYPE = 258,
     TOK_RECORD = 259,
     TOK_CASE = 260,
     TOK_ENUM = 261,
     TOK_LET = 262,
     TOK_FUNCTION = 263,
     TOK_REFINE = 264,
     TOK_CASEFUNC = 265,
     TOK_CASETYPE = 266,
     TOK_TYPEATTR = 267,
     TOK_HELPERHEADER = 268,
     TOK_HELPERCODE = 269,
     TOK_RIGHTARROW = 270,
     TOK_DEFAULT = 271,
     TOK_OF = 272,
     TOK_PADDING = 273,
     TOK_TO = 274,
     TOK_ALIGN = 275,
     TOK_WITHINPUT = 276,
     TOK_INT8 = 277,
     TOK_INT16 = 278,
     TOK_INT32 = 279,
     TOK_UINT8 = 280,
     TOK_UINT16 = 281,
     TOK_UINT32 = 282,
     TOK_ID = 283,
     TOK_NUMBER = 284,
     TOK_REGEX = 285,
     TOK_STRING = 286,
     TOK_BEGIN_RE = 287,
     TOK_END_RE = 288,
     TOK_ATTR_ALSO = 289,
     TOK_ATTR_BYTEORDER = 290,
     TOK_ATTR_CHECK = 291,
     TOK_ATTR_CHUNKED = 292,
     TOK_ATTR_EXPORTSOURCEDATA = 293,
     TOK_ATTR_IF = 294,
     TOK_ATTR_LENGTH = 295,
     TOK_ATTR_LET = 296,
     TOK_ATTR_LINEBREAKER = 297,
     TOK_ATTR_MULTILINE = 298,
     TOK_ATTR_ONELINE = 299,
     TOK_ATTR_REFCOUNT = 300,
     TOK_ATTR_REQUIRES = 301,
     TOK_ATTR_RESTOFDATA = 302,
     TOK_ATTR_RESTOFFLOW = 303,
     TOK_ATTR_TRANSIENT = 304,
     TOK_ATTR_UNTIL = 305,
     TOK_ANALYZER = 306,
     TOK_CONNECTION = 307,
     TOK_FLOW = 308,
     TOK_STATE = 309,
     TOK_ACTION = 310,
     TOK_WHEN = 311,
     TOK_HELPER = 312,
     TOK_DATAUNIT = 313,
     TOK_FLOWDIR = 314,
     TOK_WITHCONTEXT = 315,
     TOK_LPB_EXTERN = 316,
     TOK_LPB_HEADER = 317,
     TOK_LPB_CODE = 318,
     TOK_LPB_MEMBER = 319,
     TOK_LPB_INIT = 320,
     TOK_LPB_CLEANUP = 321,
     TOK_LPB_EOF = 322,
     TOK_LPB = 323,
     TOK_RPB = 324,
     TOK_EMBEDDED_ATOM = 325,
     TOK_EMBEDDED_STRING = 326,
     TOK_PAC_VAL = 327,
     TOK_PAC_SET = 328,
     TOK_PAC_TYPE = 329,
     TOK_PAC_TYPEOF = 330,
     TOK_PAC_CONST_DEF = 331,
     TOK_END_PAC = 332,
     TOK_EXTERN = 333,
     TOK_PLUSEQ = 334,
     TOK_OR = 335,
     TOK_AND = 336,
     TOK_GE = 337,
     TOK_LE = 338,
     TOK_NEQ = 339,
     TOK_EQUAL = 340,
     TOK_RSHIFT = 341,
     TOK_LSHIFT = 342,
     TOK_OFFSETOF = 343,
     TOK_SIZEOF = 344
   };
#endif
/* Tokens.  */
#define TOK_TYPE 258
#define TOK_RECORD 259
#define TOK_CASE 260
#define TOK_ENUM 261
#define TOK_LET 262
#define TOK_FUNCTION 263
#define TOK_REFINE 264
#define TOK_CASEFUNC 265
#define TOK_CASETYPE 266
#define TOK_TYPEATTR 267
#define TOK_HELPERHEADER 268
#define TOK_HELPERCODE 269
#define TOK_RIGHTARROW 270
#define TOK_DEFAULT 271
#define TOK_OF 272
#define TOK_PADDING 273
#define TOK_TO 274
#define TOK_ALIGN 275
#define TOK_WITHINPUT 276
#define TOK_INT8 277
#define TOK_INT16 278
#define TOK_INT32 279
#define TOK_UINT8 280
#define TOK_UINT16 281
#define TOK_UINT32 282
#define TOK_ID 283
#define TOK_NUMBER 284
#define TOK_REGEX 285
#define TOK_STRING 286
#define TOK_BEGIN_RE 287
#define TOK_END_RE 288
#define TOK_ATTR_ALSO 289
#define TOK_ATTR_BYTEORDER 290
#define TOK_ATTR_CHECK 291
#define TOK_ATTR_CHUNKED 292
#define TOK_ATTR_EXPORTSOURCEDATA 293
#define TOK_ATTR_IF 294
#define TOK_ATTR_LENGTH 295
#define TOK_ATTR_LET 296
#define TOK_ATTR_LINEBREAKER 297
#define TOK_ATTR_MULTILINE 298
#define TOK_ATTR_ONELINE 299
#define TOK_ATTR_REFCOUNT 300
#define TOK_ATTR_REQUIRES 301
#define TOK_ATTR_RESTOFDATA 302
#define TOK_ATTR_RESTOFFLOW 303
#define TOK_ATTR_TRANSIENT 304
#define TOK_ATTR_UNTIL 305
#define TOK_ANALYZER 306
#define TOK_CONNECTION 307
#define TOK_FLOW 308
#define TOK_STATE 309
#define TOK_ACTION 310
#define TOK_WHEN 311
#define TOK_HELPER 312
#define TOK_DATAUNIT 313
#define TOK_FLOWDIR 314
#define TOK_WITHCONTEXT 315
#define TOK_LPB_EXTERN 316
#define TOK_LPB_HEADER 317
#define TOK_LPB_CODE 318
#define TOK_LPB_MEMBER 319
#define TOK_LPB_INIT 320
#define TOK_LPB_CLEANUP 321
#define TOK_LPB_EOF 322
#define TOK_LPB 323
#define TOK_RPB 324
#define TOK_EMBEDDED_ATOM 325
#define TOK_EMBEDDED_STRING 326
#define TOK_PAC_VAL 327
#define TOK_PAC_SET 328
#define TOK_PAC_TYPE 329
#define TOK_PAC_TYPEOF 330
#define TOK_PAC_CONST_DEF 331
#define TOK_END_PAC 332
#define TOK_EXTERN 333
#define TOK_PLUSEQ 334
#define TOK_OR 335
#define TOK_AND 336
#define TOK_GE 337
#define TOK_LE 338
#define TOK_NEQ 339
#define TOK_EQUAL 340
#define TOK_RSHIFT 341
#define TOK_LSHIFT 342
#define TOK_OFFSETOF 343
#define TOK_SIZEOF 344




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 214 of yacc.c  */
#line 138 "pac_parse.yy"

	ActionParam		*actionparam;
	ActionParamType		*actionparamtype;
	AnalyzerElement		*aelem;
	AnalyzerElementList	*aelemlist;
	Attr			*attr;
	AttrList		*attrlist;
	ConstString		*cstr;
	CaseExpr		*caseexpr;
	CaseExprList		*caseexprlist;
	CaseField		*casefield;
	CaseFieldList 		*casefieldlist;
	ContextField		*contextfield;
	ContextFieldList 	*contextfieldlist;
	Decl			*decl;
	EmbeddedCode		*embedded_code;
	Enum			*enumitem;
	EnumList		*enumlist;
	Expr			*expr;
	ExprList 		*exprlist;
	Field 			*field;
	FieldList 		*fieldlist;
	Function		*function;
	ID			*id;
	InputBuffer		*input;
	LetFieldList		*letfieldlist;
	LetField		*letfield;
	Number			*num;
	PacPrimitive		*pacprimitive;
	Param 			*param;
	ParamList 		*paramlist;
	RecordFieldList 	*recordfieldlist;
	RecordField		*recordfield;
	RegEx			*regex;
	StateVar		*statevar;
	StateVarList		*statevarlist;
	const char		*str;
	Type 			*type;
	int			val;



/* Line 214 of yacc.c  */
#line 384 "pac_parse.cc"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 264 of yacc.c  */
#line 396 "pac_parse.cc"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int yyi)
#else
static int
YYID (yyi)
    int yyi;
#endif
{
  return yyi;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)				\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack_alloc, Stack, yysize);			\
	Stack = &yyptr->Stack_alloc;					\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  2
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   1277

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  114
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  68
/* YYNRULES -- Number of rules.  */
#define YYNRULES  192
/* YYNRULES -- Number of states.  */
#define YYNSTATES  426

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   344

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   104,     2,     2,     2,   102,    93,     2,
     107,   108,   100,    98,    82,    99,   111,   101,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    84,    81,
      87,    79,    88,    83,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   109,     2,   110,    95,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   112,    94,   113,   103,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    80,    85,    86,    89,    90,    91,
      92,    96,    97,   105,   106
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     4,     8,    11,    13,    14,    21,    22,
      28,    31,    32,    39,    40,    45,    46,    52,    53,    61,
      62,    70,    71,    79,    87,    95,   103,   111,   119,   123,
     127,   131,   137,   138,   140,   142,   144,   146,   148,   150,
     152,   156,   160,   161,   165,   172,   176,   181,   184,   188,
     189,   193,   195,   196,   200,   201,   204,   205,   208,   210,
     212,   217,   219,   224,   228,   235,   237,   242,   244,   246,
     250,   251,   254,   257,   259,   265,   270,   275,   278,   280,
     282,   286,   287,   290,   296,   302,   303,   305,   309,   311,
     313,   315,   320,   324,   329,   334,   338,   343,   346,   350,
     354,   358,   362,   366,   369,   373,   377,   381,   385,   389,
     393,   397,   401,   405,   409,   413,   416,   420,   424,   430,
     437,   439,   441,   443,   447,   448,   452,   456,   460,   462,
     465,   469,   471,   473,   477,   479,   481,   482,   485,   489,
     493,   497,   501,   507,   517,   520,   525,   535,   536,   540,
     544,   548,   550,   554,   555,   558,   561,   564,   568,   572,
     576,   582,   583,   587,   591,   595,   596,   601,   608,   610,
     611,   613,   617,   619,   620,   622,   626,   629,   631,   633,
     636,   640,   645,   649,   654,   657,   659,   664,   666,   668,
     670,   673,   674
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     115,     0,    -1,    -1,   115,   116,   127,    -1,   117,   177,
      -1,   126,    -1,    -1,     3,   128,   118,   134,    79,   139,
      -1,    -1,     7,   128,   119,   138,   137,    -1,     8,   133,
      -1,    -1,     6,   128,   120,   112,   159,   113,    -1,    -1,
      78,     3,   128,   121,    -1,    -1,    51,   128,   122,    60,
     129,    -1,    -1,    51,   128,   123,   134,   112,   162,   113,
      -1,    -1,    52,   128,   124,   134,   112,   162,   113,    -1,
      -1,    53,   128,   125,   134,   112,   163,   113,    -1,     9,
      11,   128,    80,   112,   149,   113,    -1,     9,    10,   128,
      80,   112,   157,   113,    -1,     9,    51,   128,    80,   112,
     164,   113,    -1,     9,    52,   128,    80,   112,   164,   113,
      -1,     9,    53,   128,    80,   112,   164,   113,    -1,    62,
     170,    69,    -1,    63,   170,    69,    -1,    61,   170,    69,
      -1,     9,    12,   128,    80,   178,    -1,    -1,    81,    -1,
      28,    -1,    52,    -1,    51,    -1,    53,    -1,     8,    -1,
       3,    -1,   112,   130,   113,    -1,   130,   131,    81,    -1,
      -1,   128,    84,   142,    -1,   128,   107,   135,   108,    84,
     141,    -1,   132,    79,   154,    -1,   132,    68,   170,    69,
      -1,   132,    81,    -1,   107,   135,   108,    -1,    -1,   135,
      82,   136,    -1,   136,    -1,    -1,   128,    84,   141,    -1,
      -1,    79,   154,    -1,    -1,    84,   141,    -1,   140,    -1,
     141,    -1,     4,   112,   143,   113,    -1,   142,    -1,   142,
     109,   154,   110,    -1,   142,   109,   110,    -1,     5,   148,
      17,   112,   149,   113,    -1,   128,    -1,   128,   107,   153,
     108,    -1,   156,    -1,   155,    -1,   143,   144,    81,    -1,
      -1,   145,   177,    -1,   147,   141,    -1,   146,    -1,   147,
      18,   109,   154,   110,    -1,   147,    18,    19,   154,    -1,
     147,    18,    20,   154,    -1,   128,    84,    -1,    84,    -1,
     154,    -1,   149,   150,    81,    -1,    -1,   151,   177,    -1,
     153,    15,   128,    84,   141,    -1,    16,    15,   128,    84,
     141,    -1,    -1,   153,    -1,   153,    82,   154,    -1,   154,
      -1,   128,    -1,    29,    -1,   154,   109,   154,   110,    -1,
     154,   111,   128,    -1,   106,   107,   128,   108,    -1,   105,
     107,   128,   108,    -1,   107,   154,   108,    -1,   154,   107,
     152,   108,    -1,    99,   154,    -1,   154,    98,   154,    -1,
     154,    99,   154,    -1,   154,   100,   154,    -1,   154,   101,
     154,    -1,   154,   102,   154,    -1,   103,   154,    -1,   154,
      93,   154,    -1,   154,    94,   154,    -1,   154,    95,   154,
      -1,   154,    97,   154,    -1,   154,    96,   154,    -1,   154,
      92,   154,    -1,   154,    91,   154,    -1,   154,    89,   154,
      -1,   154,    90,   154,    -1,   154,    88,   154,    -1,   154,
      87,   154,    -1,   104,   154,    -1,   154,    86,   154,    -1,
     154,    85,   154,    -1,   154,    83,   154,    84,   154,    -1,
       5,   154,    17,   112,   157,   113,    -1,   155,    -1,   156,
      -1,    31,    -1,    32,    30,    33,    -1,    -1,   157,   158,
      81,    -1,   153,    15,   154,    -1,    16,    15,   154,    -1,
     160,    -1,   160,    82,    -1,   160,    82,   161,    -1,   161,
      -1,   128,    -1,   128,    79,   154,    -1,   164,    -1,   164,
      -1,    -1,   164,   165,    -1,    64,   170,    69,    -1,    65,
     170,    69,    -1,    66,   170,    69,    -1,    67,   170,    69,
      -1,    59,    79,   128,   172,    81,    -1,    58,    79,   128,
     172,    60,   107,   152,   108,    81,    -1,     8,   133,    -1,
      54,   112,   166,   113,    -1,    55,   128,    56,   107,   168,
     108,    68,   170,    69,    -1,    -1,   166,   167,    81,    -1,
     128,    84,   142,    -1,   128,    90,   169,    -1,   128,    -1,
     128,   111,   128,    -1,    -1,   170,    70,    -1,   170,    71,
      -1,   170,   171,    -1,    72,   154,    77,    -1,    73,   154,
      77,    -1,    74,   154,    77,    -1,    76,   128,    79,   154,
      77,    -1,    -1,   107,   152,   108,    -1,   173,   174,    81,
      -1,   173,   175,    81,    -1,    -1,   128,   138,   137,   177,
      -1,   128,    84,   142,    21,   176,   177,    -1,   154,    -1,
      -1,   178,    -1,   178,   179,   180,    -1,   180,    -1,    -1,
      82,    -1,    35,    79,   154,    -1,    36,   154,    -1,    37,
      -1,    38,    -1,    39,   154,    -1,    40,    79,   154,    -1,
      41,   112,   173,   113,    -1,    42,    79,   154,    -1,    43,
     107,   154,   108,    -1,    44,   181,    -1,    45,    -1,    46,
     107,   152,   108,    -1,    47,    -1,    48,    -1,    49,    -1,
      50,   154,    -1,    -1,   107,   154,   108,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   182,   182,   185,   190,   195,   201,   201,   206,   206,
     210,   215,   215,   219,   219,   224,   224,   228,   228,   232,
     232,   236,   236,   240,   244,   248,   252,   256,   262,   266,
     270,   274,   280,   281,   284,   288,   292,   296,   300,   304,
     310,   316,   322,   327,   333,   339,   344,   349,   355,   360,
     365,   370,   376,   381,   388,   391,   398,   401,   407,   414,
     418,   425,   429,   433,   437,   444,   448,   452,   456,   462,
     468,   473,   480,   484,   490,   495,   500,   507,   511,   517,
     523,   529,   534,   541,   545,   552,   555,   561,   566,   573,
     577,   581,   585,   589,   593,   597,   601,   607,   611,   615,
     619,   623,   627,   631,   635,   639,   643,   647,   651,   655,
     659,   663,   667,   671,   675,   679,   683,   687,   691,   695,
     699,   703,   709,   715,   722,   725,   732,   736,   742,   746,
     752,   757,   764,   768,   774,   780,   788,   791,   798,   802,
     806,   810,   814,   818,   826,   830,   834,   841,   844,   851,
     857,   863,   867,   874,   877,   882,   887,   894,   898,   902,
     906,   913,   916,   922,   927,   933,   938,   945,   953,   960,
     963,   969,   975,   983,   984,   987,   991,   995,   999,  1003,
    1007,  1011,  1015,  1019,  1023,  1027,  1031,  1035,  1039,  1043,
    1047,  1054,  1057
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "TOK_TYPE", "TOK_RECORD", "TOK_CASE",
  "TOK_ENUM", "TOK_LET", "TOK_FUNCTION", "TOK_REFINE", "TOK_CASEFUNC",
  "TOK_CASETYPE", "TOK_TYPEATTR", "TOK_HELPERHEADER", "TOK_HELPERCODE",
  "TOK_RIGHTARROW", "TOK_DEFAULT", "TOK_OF", "TOK_PADDING", "TOK_TO",
  "TOK_ALIGN", "TOK_WITHINPUT", "TOK_INT8", "TOK_INT16", "TOK_INT32",
  "TOK_UINT8", "TOK_UINT16", "TOK_UINT32", "TOK_ID", "TOK_NUMBER",
  "TOK_REGEX", "TOK_STRING", "TOK_BEGIN_RE", "TOK_END_RE", "TOK_ATTR_ALSO",
  "TOK_ATTR_BYTEORDER", "TOK_ATTR_CHECK", "TOK_ATTR_CHUNKED",
  "TOK_ATTR_EXPORTSOURCEDATA", "TOK_ATTR_IF", "TOK_ATTR_LENGTH",
  "TOK_ATTR_LET", "TOK_ATTR_LINEBREAKER", "TOK_ATTR_MULTILINE",
  "TOK_ATTR_ONELINE", "TOK_ATTR_REFCOUNT", "TOK_ATTR_REQUIRES",
  "TOK_ATTR_RESTOFDATA", "TOK_ATTR_RESTOFFLOW", "TOK_ATTR_TRANSIENT",
  "TOK_ATTR_UNTIL", "TOK_ANALYZER", "TOK_CONNECTION", "TOK_FLOW",
  "TOK_STATE", "TOK_ACTION", "TOK_WHEN", "TOK_HELPER", "TOK_DATAUNIT",
  "TOK_FLOWDIR", "TOK_WITHCONTEXT", "TOK_LPB_EXTERN", "TOK_LPB_HEADER",
  "TOK_LPB_CODE", "TOK_LPB_MEMBER", "TOK_LPB_INIT", "TOK_LPB_CLEANUP",
  "TOK_LPB_EOF", "TOK_LPB", "TOK_RPB", "TOK_EMBEDDED_ATOM",
  "TOK_EMBEDDED_STRING", "TOK_PAC_VAL", "TOK_PAC_SET", "TOK_PAC_TYPE",
  "TOK_PAC_TYPEOF", "TOK_PAC_CONST_DEF", "TOK_END_PAC", "TOK_EXTERN",
  "'='", "TOK_PLUSEQ", "';'", "','", "'?'", "':'", "TOK_OR", "TOK_AND",
  "'<'", "'>'", "TOK_GE", "TOK_LE", "TOK_NEQ", "TOK_EQUAL", "'&'", "'|'",
  "'^'", "TOK_RSHIFT", "TOK_LSHIFT", "'+'", "'-'", "'*'", "'/'", "'%'",
  "'~'", "'!'", "TOK_OFFSETOF", "TOK_SIZEOF", "'('", "')'", "'['", "']'",
  "'.'", "'{'", "'}'", "$accept", "decls", "decl", "decl_with_attr", "$@1",
  "$@2", "$@3", "$@4", "$@5", "$@6", "$@7", "$@8", "decl_without_attr",
  "optsemicolon", "tok_id", "analyzercontext", "contextfieldlist",
  "contextfield", "funcproto", "function", "optparams", "paramlist",
  "param", "optinit", "opttype", "type", "type3", "type2", "type1",
  "recordfieldlist", "recordfield", "recordfield0", "padding",
  "optfieldid", "caseindex", "casefieldlist", "casefield", "casefield0",
  "optexprlist", "exprlist", "expr", "cstr", "regex", "caseexprlist",
  "caseexpr", "enumlist", "enumlist1", "enumitem", "conn", "flow",
  "sahlist", "sah", "statevarlist", "statevar", "actionparam",
  "actionparamtype", "embedded_code", "embedded_pac_primitive", "optargs",
  "letfieldlist", "letfield", "withinputfield", "input", "optattrs",
  "attrlist", "optcomma", "attr", "optlinebreaker", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   331,   332,   333,    61,
     334,    59,    44,    63,    58,   335,   336,    60,    62,   337,
     338,   339,   340,    38,   124,    94,   341,   342,    43,    45,
      42,    47,    37,   126,    33,   343,   344,    40,    41,    91,
      93,    46,   123,   125
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,   114,   115,   115,   116,   116,   118,   117,   119,   117,
     117,   120,   117,   121,   117,   122,   117,   123,   117,   124,
     117,   125,   117,   117,   117,   117,   117,   117,   126,   126,
     126,   126,   127,   127,   128,   128,   128,   128,   128,   128,
     129,   130,   130,   131,   132,   133,   133,   133,   134,   134,
     135,   135,   135,   136,   137,   137,   138,   138,   139,   140,
     140,   141,   141,   141,   141,   142,   142,   142,   142,   143,
     143,   144,   145,   145,   146,   146,   146,   147,   147,   148,
     149,   149,   150,   151,   151,   152,   152,   153,   153,   154,
     154,   154,   154,   154,   154,   154,   154,   154,   154,   154,
     154,   154,   154,   154,   154,   154,   154,   154,   154,   154,
     154,   154,   154,   154,   154,   154,   154,   154,   154,   154,
     154,   154,   155,   156,   157,   157,   158,   158,   159,   159,
     160,   160,   161,   161,   162,   163,   164,   164,   165,   165,
     165,   165,   165,   165,   165,   165,   165,   166,   166,   167,
     168,   169,   169,   170,   170,   170,   170,   171,   171,   171,
     171,   172,   172,   173,   173,   173,   174,   175,   176,   177,
     177,   178,   178,   179,   179,   180,   180,   180,   180,   180,
     180,   180,   180,   180,   180,   180,   180,   180,   180,   180,
     180,   181,   181
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     3,     2,     1,     0,     6,     0,     5,
       2,     0,     6,     0,     4,     0,     5,     0,     7,     0,
       7,     0,     7,     7,     7,     7,     7,     7,     3,     3,
       3,     5,     0,     1,     1,     1,     1,     1,     1,     1,
       3,     3,     0,     3,     6,     3,     4,     2,     3,     0,
       3,     1,     0,     3,     0,     2,     0,     2,     1,     1,
       4,     1,     4,     3,     6,     1,     4,     1,     1,     3,
       0,     2,     2,     1,     5,     4,     4,     2,     1,     1,
       3,     0,     2,     5,     5,     0,     1,     3,     1,     1,
       1,     4,     3,     4,     4,     3,     4,     2,     3,     3,
       3,     3,     3,     2,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     2,     3,     3,     5,     6,
       1,     1,     1,     3,     0,     3,     3,     3,     1,     2,
       3,     1,     1,     3,     1,     1,     0,     2,     3,     3,
       3,     3,     5,     9,     2,     4,     9,     0,     3,     3,
       3,     1,     3,     0,     2,     2,     2,     3,     3,     3,
       5,     0,     3,     3,     3,     0,     4,     6,     1,     0,
       1,     3,     1,     0,     1,     3,     2,     1,     1,     2,
       3,     4,     3,     4,     2,     1,     4,     1,     1,     1,
       2,     0,     3
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       2,     0,     1,     0,     0,     0,     0,     0,     0,     0,
       0,   153,   153,   153,     0,    32,   169,     5,    39,    38,
      34,    36,    35,    37,     6,    11,     8,     0,     0,    10,
       0,     0,     0,     0,     0,     0,    17,    19,    21,     0,
       0,     0,     0,    33,     3,     0,     0,   177,   178,     0,
       0,     0,     0,     0,   191,   185,     0,   187,   188,   189,
       0,     4,   173,   172,    49,     0,    56,    52,   153,     0,
      47,     0,     0,     0,     0,     0,     0,     0,    49,    49,
      49,    30,   154,   155,     0,     0,     0,     0,   156,    28,
      29,    13,     0,     0,    90,   122,     0,     0,     0,     0,
       0,     0,     0,    89,   176,   120,   121,   179,     0,   165,
       0,     0,     0,   184,    85,   190,   174,     0,    52,     0,
       0,     0,    54,     0,     0,    51,     0,    45,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    14,   175,     0,     0,    97,   103,   115,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    85,     0,     0,   180,     0,   182,     0,     0,     0,
      86,    88,   171,     0,     0,   132,     0,   128,   131,     0,
      65,    57,    61,    68,    67,     0,     9,     0,     0,     0,
      46,   124,    81,   173,   136,   136,   136,    42,    16,   136,
     136,   136,   157,   158,   159,     0,     0,   123,     0,     0,
      95,     0,   117,   116,   114,   113,   111,   112,   110,   109,
     104,   105,   106,   108,   107,    98,    99,   100,   101,   102,
       0,     0,    92,   181,    56,     0,     0,   183,   192,   186,
       0,    48,     0,     7,    58,    59,     0,    12,   129,     0,
      79,     0,     0,    55,    53,    50,     0,     0,     0,     0,
       0,     0,     0,     0,   134,     0,     0,   135,     0,   124,
      94,    93,     0,    96,    91,     0,    54,   163,   164,    87,
      70,   133,   130,     0,     0,    63,     0,    44,     0,    24,
       0,     0,     0,    23,     0,   169,     0,     0,     0,     0,
       0,     0,   153,   153,   153,   153,    25,   137,    26,    27,
      40,     0,     0,    18,    20,    22,   160,     0,   118,    61,
     169,     0,    81,    66,    62,     0,     0,   125,     0,    80,
      82,     0,   144,   147,     0,     0,     0,     0,     0,     0,
       0,     0,    41,   119,     0,   166,    78,    60,     0,     0,
     169,    73,     0,     0,   127,   126,     0,     0,     0,     0,
     161,   161,   138,   139,   140,   141,    43,   168,   169,    77,
      69,    71,     0,    72,    64,     0,     0,   145,     0,     0,
       0,    85,     0,     0,   167,     0,     0,     0,    84,    83,
       0,   148,     0,     0,     0,     0,   142,    75,    76,     0,
     149,     0,     0,   162,    85,    74,   151,   150,   153,     0,
       0,     0,     0,   152,   146,   143
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,    15,    16,    64,    66,    65,   142,    77,    78,
      79,    80,    17,    44,   103,   208,   272,   322,    28,    29,
     119,   124,   125,   196,   122,   253,   254,   191,   192,   331,
     359,   360,   361,   362,   259,   268,   304,   305,   179,   180,
     181,   105,   106,   267,   301,   186,   187,   188,   273,   276,
     274,   317,   368,   389,   403,   417,    39,    88,   392,   175,
     245,   246,   378,    61,    62,   117,    63,   113
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -277
static const yytype_int16 yypact[] =
{
    -277,   318,  -277,   311,   311,   311,   311,   165,   311,   311,
     311,  -277,  -277,  -277,    13,   -61,  1163,  -277,  -277,  -277,
    -277,  -277,  -277,  -277,  -277,  -277,  -277,   -74,   -11,  -277,
     311,   311,   311,   311,   311,   311,   -26,  -277,  -277,   598,
    1145,  1153,   311,  -277,  -277,   -43,   559,  -277,  -277,   559,
     -39,   -69,   -32,   -57,   -52,  -277,   -49,  -277,  -277,  -277,
     559,  -277,   520,  -277,   -44,   -45,   -36,   311,  -277,   559,
    -277,    -7,     3,     6,    12,    17,    19,    18,   -44,   -44,
     -44,  -277,  -277,  -277,   559,   559,   559,   311,  -277,  -277,
    -277,  -277,   559,   559,  -277,  -277,    39,   559,   559,   559,
     -30,     2,   559,  -277,   983,  -277,  -277,   983,   559,  -277,
     559,   559,   559,  -277,   559,   983,  -277,  1163,   311,    41,
     311,   707,    44,    47,   -67,  -277,  1161,   983,    16,    21,
    1163,    22,    30,    31,    32,    55,    59,    62,   678,   705,
     732,    53,  -277,   983,   530,   112,   468,   -35,   -35,   311,
     311,   786,   559,   559,   559,   559,   559,   559,   559,   559,
     559,   559,   559,   559,   559,   559,   559,   559,   559,   559,
     559,   559,   559,   311,   983,     9,   983,   813,   840,    72,
      97,   983,  -277,   -64,   647,   104,    76,   108,  -277,   559,
      86,  -277,    90,  -277,  -277,   559,  -277,   707,   311,   113,
    -277,  -277,  -277,   640,  -277,  -277,  -277,  -277,  -277,  -277,
    -277,  -277,  -277,  -277,  -277,   559,    91,  -277,    92,    94,
    -277,   869,  1036,  1061,  1086,  1086,  1086,  1086,  1086,  1086,
     645,   645,   645,   380,   380,   468,   468,   -35,   -35,   -35,
      96,   896,  -277,  -277,   121,   125,   126,  -277,  -277,  -277,
     559,  -277,   101,  -277,  -277,  -277,   559,  -277,   311,   191,
     983,   559,   490,   983,  -277,  -277,   707,   248,   370,    71,
     174,   494,    88,   106,   392,   107,   114,   392,   759,  -277,
    -277,  -277,   559,  -277,  -277,   707,    44,  -277,  -277,   983,
    -277,   983,  -277,   102,   -59,  -277,   925,  -277,   210,  -277,
      20,   145,   215,  -277,   150,  1163,    23,   311,   122,   311,
     163,   164,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,
    -277,   160,   167,  -277,  -277,  -277,  -277,   402,  1010,     1,
    1163,   184,  -277,  -277,  -277,   559,   559,  -277,   311,  -277,
    -277,   311,  -277,  -277,   193,   311,   311,  1169,  1177,  1185,
    1193,   242,  -277,  -277,   559,  -277,  -277,  -277,   173,   178,
    1163,  -277,   677,   433,   983,   983,   176,   177,   170,   155,
     159,   159,  -277,  -277,  -277,  -277,  -277,   983,  1163,  -277,
    -277,  -277,    -6,  -277,  -277,   707,   707,  -277,   183,   190,
     311,   559,   212,   194,  -277,   559,   559,   559,  -277,  -277,
     242,  -277,   195,   182,   188,   179,  -277,   983,   983,   954,
    -277,   311,   216,  -277,   559,  -277,   180,  -277,  -277,   197,
     311,  1201,   217,  -277,  -277,  -277
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,
    -277,  -277,  -277,  -277,    -3,  -277,  -277,  -277,  -277,    -5,
      33,   189,   110,    34,    65,  -277,  -277,  -176,  -276,  -277,
    -277,  -277,  -277,  -277,  -277,   -15,  -277,  -277,  -167,  -242,
      -4,   -97,   -70,    43,  -277,  -277,  -277,    75,   119,  -277,
    -152,  -277,  -277,  -277,  -277,  -277,    -2,  -277,   -48,  -277,
    -277,  -277,  -277,  -259,   204,  -277,   219,  -277
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -171
static const yytype_int16 yytable[] =
{
      24,    25,    26,    27,   240,    36,    37,    38,   255,   329,
      40,    41,    18,   395,   396,   198,    42,    19,   198,   294,
      43,   264,   354,   250,   193,   300,   306,    71,    72,    73,
      74,    75,    76,    67,   -15,   336,    92,    20,   341,    91,
     108,   199,   104,   109,   251,   107,   340,   110,   121,   333,
     111,   194,   269,   270,   271,   112,   115,    68,   114,   277,
      21,    22,    23,   118,   123,   127,   126,   120,    69,   145,
      70,   355,   171,   128,   172,   376,   173,   149,   134,   307,
     138,   139,   140,   129,   141,   300,   130,   193,   143,   144,
     297,    18,   131,   146,   147,   148,    19,   132,   151,   133,
     193,   381,   250,   397,   174,   250,   176,   177,   178,   150,
     262,   135,   136,   137,   194,   123,    20,   185,   190,   394,
     184,   306,   243,   195,   410,   308,   309,   194,   201,   310,
     311,   197,   215,   202,   204,   312,   313,   314,   315,    21,
      22,    23,   205,   206,   207,   217,   218,   219,   221,   222,
     223,   224,   225,   226,   227,   228,   229,   230,   231,   232,
     233,   234,   235,   236,   237,   238,   239,   209,   241,   193,
     242,   210,   244,    18,   211,    30,    31,    32,    19,   250,
     249,   190,   307,   256,   316,   260,   383,    18,   193,   257,
     258,   263,    19,   261,   190,   123,   194,   266,    20,   262,
     280,   320,   281,   279,   283,   285,   287,   288,   293,   398,
     399,   278,    20,   290,   332,   194,    33,    34,    35,   323,
     324,    21,    22,    23,   404,   335,   337,   325,   308,   309,
     338,   339,   310,   311,   343,    21,    22,    23,   312,   313,
     314,   315,   345,   346,   351,    18,   289,   419,   352,   369,
      19,    18,   291,    93,   193,   185,    19,   379,   296,   380,
     385,   386,   390,   190,   298,   193,   391,   400,   356,   321,
      20,   401,   405,    95,    96,   406,    20,    94,   328,    95,
      96,   194,   190,   387,   418,   411,   414,   318,   193,   193,
     412,   420,   194,    21,    22,    23,   413,   357,   425,    21,
      22,    23,   342,   193,    27,   422,   344,   183,   265,   286,
     347,   348,   349,   350,    18,   194,   194,   363,     2,    19,
     330,     3,   327,   393,     4,     5,     6,     7,   358,   275,
     194,   364,   365,   292,   203,   366,   182,     0,   367,    20,
       0,     0,   370,   371,     0,     0,     0,    97,   190,     0,
     377,    98,    99,   100,   101,   102,     0,     0,     0,   190,
       0,   299,    21,    22,    23,   388,     0,     0,     0,     8,
       9,    10,     0,    18,     0,    93,     0,     0,    19,    11,
      12,    13,   190,   190,     0,     0,   302,   402,     0,     0,
       0,   407,   408,   409,     0,     0,    14,   190,    20,    94,
     307,    95,    96,     0,     0,    18,     0,    93,   416,     0,
      19,     0,     0,     0,     0,     0,   421,   423,   298,     0,
       0,    21,    22,    23,     0,     0,     0,     0,     0,     0,
      20,    94,     0,    95,    96,     0,    18,     0,    93,     0,
       0,    19,     0,     0,     0,     0,   308,   309,     0,   302,
     310,   311,     0,    21,    22,    23,   312,   313,   314,   315,
       0,    20,    94,     0,    95,    96,     0,     0,     0,    97,
       0,     0,     0,    98,    99,   100,   101,   102,   166,   167,
     168,   169,   170,   303,    21,    22,    23,   171,     0,   172,
       0,   173,     0,    18,     0,    93,     0,     0,    19,     0,
       0,    97,   307,     0,     0,    98,    99,   100,   101,   102,
       0,     0,     0,     0,     0,   353,     0,     0,    20,    94,
    -170,    95,    96,  -170,     0,     0,  -170,  -170,  -170,  -170,
       0,     0,    97,     0,     0,     0,    98,    99,   100,   101,
     102,    21,    22,    23,     0,     0,   384,   216,   308,   309,
       0,     0,   310,   311,     0,     0,     0,     0,   312,   313,
     314,   315,    18,     0,    93,     0,     0,    19,   168,   169,
     170,  -170,  -170,  -170,     0,   171,     0,   172,     0,   173,
       0,  -170,  -170,  -170,     0,     0,     0,    20,    94,    97,
      95,    96,     0,    98,    99,   100,   101,   102,  -170,     0,
     295,  -170,   116,     0,     0,     0,     0,   319,     0,     0,
      21,    22,    23,   152,     0,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,     0,     0,     0,     0,   171,     0,   172,
     -31,   173,     0,   -31,     0,     0,   -31,   -31,   -31,   -31,
      18,   252,   189,     0,     0,    19,     0,     0,    97,     0,
       0,     0,    98,    99,   100,   101,   102,    81,    82,    83,
      84,    85,    86,     0,    87,    20,     0,     0,    95,    96,
      18,     0,   189,     0,     0,    19,     0,     0,     0,     0,
       0,   -31,   -31,   -31,     0,   382,     0,     0,    21,    22,
      23,   -31,   -31,   -31,     0,    20,     0,     0,    95,    96,
      18,     0,   189,     0,     0,    19,     0,     0,   -31,     0,
       0,   -31,   116,     0,     0,     0,     0,     0,    21,    22,
      23,     0,     0,     0,     0,    20,     0,     0,    95,    96,
       0,   164,   165,   166,   167,   168,   169,   170,     0,     0,
       0,     0,   171,     0,   172,   212,   173,     0,    21,    22,
      23,   152,     0,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   165,   166,   167,   168,   169,
     170,     0,   213,     0,     0,   171,     0,   172,   152,   173,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   170,     0,   214,
       0,     0,   171,     0,   172,   152,   173,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,   165,
     166,   167,   168,   169,   170,     0,   326,     0,     0,   171,
       0,   172,   152,   173,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,   167,   168,
     169,   170,     0,     0,     0,     0,   171,     0,   172,   152,
     173,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   165,   166,   167,   168,   169,   170,     0,
       0,     0,     0,   171,   220,   172,   152,   173,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   169,   170,     0,     0,     0,     0,
     171,   247,   172,   152,   173,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,     0,     0,     0,     0,   171,   248,   172,
       0,   173,   152,   282,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,   167,   168,
     169,   170,     0,     0,     0,     0,   171,     0,   172,   152,
     173,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   165,   166,   167,   168,   169,   170,     0,
       0,     0,     0,   171,     0,   172,   284,   173,   152,     0,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   170,     0,     0,
       0,     0,   171,     0,   172,   334,   173,   152,     0,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,   165,   166,   167,   168,   169,   170,     0,     0,     0,
       0,   171,     0,   172,   415,   173,   152,     0,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   169,   170,     0,     0,     0,     0,
     171,     0,   172,     0,   173,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,     0,     0,     0,     0,   171,     0,   172,
       0,   173,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   165,   166,   167,   168,   169,   170,     0,
       0,     0,     0,   171,     0,   172,     0,   173,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,   169,   170,     0,     0,     0,     0,   171,     0,
     172,     0,   173,  -171,  -171,  -171,  -171,  -171,  -171,   161,
     162,   163,   164,   165,   166,   167,   168,   169,   170,     0,
       0,     0,     0,   171,     0,   172,     0,   173,    45,    46,
      47,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    58,    59,    60,    89,    82,    83,    84,    85,    86,
       0,    87,    90,    82,    83,    84,    85,    86,     0,    87,
     200,    82,    83,    84,    85,    86,     0,    87,   372,    82,
      83,    84,    85,    86,     0,    87,   373,    82,    83,    84,
      85,    86,     0,    87,   374,    82,    83,    84,    85,    86,
       0,    87,   375,    82,    83,    84,    85,    86,     0,    87,
     424,    82,    83,    84,    85,    86,     0,    87
};

static const yytype_int16 yycheck[] =
{
       3,     4,     5,     6,   171,     8,     9,    10,   184,   285,
      12,    13,     3,    19,    20,    82,     3,     8,    82,   261,
      81,   197,    21,    82,   121,   267,   268,    30,    31,    32,
      33,    34,    35,   107,    60,    15,    79,    28,    15,    42,
      79,   108,    46,   112,   108,    49,   305,    79,    84,   108,
     107,   121,   204,   205,   206,   107,    60,    68,   107,   211,
      51,    52,    53,   107,    67,    69,    68,   112,    79,    30,
      81,   330,   107,    80,   109,   351,   111,   107,    60,     8,
      84,    85,    86,    80,    87,   327,    80,   184,    92,    93,
     266,     3,    80,    97,    98,    99,     8,    80,   102,    80,
     197,   360,    82,   109,   108,    82,   110,   111,   112,   107,
     109,    78,    79,    80,   184,   118,    28,   120,   121,   378,
      79,   363,   113,    79,   400,    54,    55,   197,   112,    58,
      59,    84,    79,   112,   112,    64,    65,    66,    67,    51,
      52,    53,   112,   112,   112,    33,   149,   150,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,   165,   166,   167,   168,   169,   170,   112,   172,   266,
     173,   112,   175,     3,   112,    10,    11,    12,     8,    82,
     108,   184,     8,    79,   113,   189,   362,     3,   285,   113,
      82,   195,     8,   107,   197,   198,   266,    84,    28,   109,
     108,   113,   108,   112,   108,    84,    81,    81,    17,   385,
     386,   215,    28,   112,   112,   285,    51,    52,    53,   113,
     113,    51,    52,    53,   391,    15,    81,   113,    54,    55,
      15,    81,    58,    59,   112,    51,    52,    53,    64,    65,
      66,    67,    79,    79,    84,     3,   250,   414,    81,    56,
       8,     3,   256,     5,   351,   258,     8,    84,   262,    81,
      84,    84,   107,   266,    16,   362,   107,    84,    84,   272,
      28,    81,    60,    31,    32,    81,    28,    29,   282,    31,
      32,   351,   285,   113,    68,    90,   107,   113,   385,   386,
     108,   111,   362,    51,    52,    53,   108,   113,    81,    51,
      52,    53,   307,   400,   307,   108,   309,   118,   198,   244,
     312,   313,   314,   315,     3,   385,   386,   332,     0,     8,
     286,     3,   279,   371,     6,     7,     8,     9,   331,   210,
     400,   335,   336,   258,   130,   338,   117,    -1,   341,    28,
      -1,    -1,   345,   346,    -1,    -1,    -1,    99,   351,    -1,
     354,   103,   104,   105,   106,   107,    -1,    -1,    -1,   362,
      -1,   113,    51,    52,    53,   368,    -1,    -1,    -1,    51,
      52,    53,    -1,     3,    -1,     5,    -1,    -1,     8,    61,
      62,    63,   385,   386,    -1,    -1,    16,   390,    -1,    -1,
      -1,   395,   396,   397,    -1,    -1,    78,   400,    28,    29,
       8,    31,    32,    -1,    -1,     3,    -1,     5,   411,    -1,
       8,    -1,    -1,    -1,    -1,    -1,   418,   420,    16,    -1,
      -1,    51,    52,    53,    -1,    -1,    -1,    -1,    -1,    -1,
      28,    29,    -1,    31,    32,    -1,     3,    -1,     5,    -1,
      -1,     8,    -1,    -1,    -1,    -1,    54,    55,    -1,    16,
      58,    59,    -1,    51,    52,    53,    64,    65,    66,    67,
      -1,    28,    29,    -1,    31,    32,    -1,    -1,    -1,    99,
      -1,    -1,    -1,   103,   104,   105,   106,   107,    98,    99,
     100,   101,   102,   113,    51,    52,    53,   107,    -1,   109,
      -1,   111,    -1,     3,    -1,     5,    -1,    -1,     8,    -1,
      -1,    99,     8,    -1,    -1,   103,   104,   105,   106,   107,
      -1,    -1,    -1,    -1,    -1,   113,    -1,    -1,    28,    29,
       0,    31,    32,     3,    -1,    -1,     6,     7,     8,     9,
      -1,    -1,    99,    -1,    -1,    -1,   103,   104,   105,   106,
     107,    51,    52,    53,    -1,    -1,   113,    17,    54,    55,
      -1,    -1,    58,    59,    -1,    -1,    -1,    -1,    64,    65,
      66,    67,     3,    -1,     5,    -1,    -1,     8,   100,   101,
     102,    51,    52,    53,    -1,   107,    -1,   109,    -1,   111,
      -1,    61,    62,    63,    -1,    -1,    -1,    28,    29,    99,
      31,    32,    -1,   103,   104,   105,   106,   107,    78,    -1,
     110,    81,    82,    -1,    -1,    -1,    -1,   113,    -1,    -1,
      51,    52,    53,    83,    -1,    85,    86,    87,    88,    89,
      90,    91,    92,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   102,    -1,    -1,    -1,    -1,   107,    -1,   109,
       0,   111,    -1,     3,    -1,    -1,     6,     7,     8,     9,
       3,     4,     5,    -1,    -1,     8,    -1,    -1,    99,    -1,
      -1,    -1,   103,   104,   105,   106,   107,    69,    70,    71,
      72,    73,    74,    -1,    76,    28,    -1,    -1,    31,    32,
       3,    -1,     5,    -1,    -1,     8,    -1,    -1,    -1,    -1,
      -1,    51,    52,    53,    -1,    18,    -1,    -1,    51,    52,
      53,    61,    62,    63,    -1,    28,    -1,    -1,    31,    32,
       3,    -1,     5,    -1,    -1,     8,    -1,    -1,    78,    -1,
      -1,    81,    82,    -1,    -1,    -1,    -1,    -1,    51,    52,
      53,    -1,    -1,    -1,    -1,    28,    -1,    -1,    31,    32,
      -1,    96,    97,    98,    99,   100,   101,   102,    -1,    -1,
      -1,    -1,   107,    -1,   109,    77,   111,    -1,    51,    52,
      53,    83,    -1,    85,    86,    87,    88,    89,    90,    91,
      92,    93,    94,    95,    96,    97,    98,    99,   100,   101,
     102,    -1,    77,    -1,    -1,   107,    -1,   109,    83,   111,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,    -1,    77,
      -1,    -1,   107,    -1,   109,    83,   111,    85,    86,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,    -1,    77,    -1,    -1,   107,
      -1,   109,    83,   111,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   102,    -1,    -1,    -1,    -1,   107,    -1,   109,    83,
     111,    85,    86,    87,    88,    89,    90,    91,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   101,   102,    -1,
      -1,    -1,    -1,   107,   108,   109,    83,   111,    85,    86,
      87,    88,    89,    90,    91,    92,    93,    94,    95,    96,
      97,    98,    99,   100,   101,   102,    -1,    -1,    -1,    -1,
     107,   108,   109,    83,   111,    85,    86,    87,    88,    89,
      90,    91,    92,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   102,    -1,    -1,    -1,    -1,   107,   108,   109,
      -1,   111,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   102,    -1,    -1,    -1,    -1,   107,    -1,   109,    83,
     111,    85,    86,    87,    88,    89,    90,    91,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   101,   102,    -1,
      -1,    -1,    -1,   107,    -1,   109,   110,   111,    83,    -1,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,    -1,    -1,
      -1,    -1,   107,    -1,   109,   110,   111,    83,    -1,    85,
      86,    87,    88,    89,    90,    91,    92,    93,    94,    95,
      96,    97,    98,    99,   100,   101,   102,    -1,    -1,    -1,
      -1,   107,    -1,   109,   110,   111,    83,    -1,    85,    86,
      87,    88,    89,    90,    91,    92,    93,    94,    95,    96,
      97,    98,    99,   100,   101,   102,    -1,    -1,    -1,    -1,
     107,    -1,   109,    -1,   111,    85,    86,    87,    88,    89,
      90,    91,    92,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   102,    -1,    -1,    -1,    -1,   107,    -1,   109,
      -1,   111,    86,    87,    88,    89,    90,    91,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   101,   102,    -1,
      -1,    -1,    -1,   107,    -1,   109,    -1,   111,    87,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,    -1,    -1,    -1,    -1,   107,    -1,
     109,    -1,   111,    87,    88,    89,    90,    91,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   101,   102,    -1,
      -1,    -1,    -1,   107,    -1,   109,    -1,   111,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    69,    70,    71,    72,    73,    74,
      -1,    76,    69,    70,    71,    72,    73,    74,    -1,    76,
      69,    70,    71,    72,    73,    74,    -1,    76,    69,    70,
      71,    72,    73,    74,    -1,    76,    69,    70,    71,    72,
      73,    74,    -1,    76,    69,    70,    71,    72,    73,    74,
      -1,    76,    69,    70,    71,    72,    73,    74,    -1,    76,
      69,    70,    71,    72,    73,    74,    -1,    76
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,   115,     0,     3,     6,     7,     8,     9,    51,    52,
      53,    61,    62,    63,    78,   116,   117,   126,     3,     8,
      28,    51,    52,    53,   128,   128,   128,   128,   132,   133,
      10,    11,    12,    51,    52,    53,   128,   128,   128,   170,
     170,   170,     3,    81,   127,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,   177,   178,   180,   118,   120,   119,   107,    68,    79,
      81,   128,   128,   128,   128,   128,   128,   122,   123,   124,
     125,    69,    70,    71,    72,    73,    74,    76,   171,    69,
      69,   128,    79,     5,    29,    31,    32,    99,   103,   104,
     105,   106,   107,   128,   154,   155,   156,   154,    79,   112,
      79,   107,   107,   181,   107,   154,    82,   179,   107,   134,
     112,    84,   138,   128,   135,   136,   170,   154,    80,    80,
      80,    80,    80,    80,    60,   134,   134,   134,   154,   154,
     154,   128,   121,   154,   154,    30,   154,   154,   154,   107,
     107,   154,    83,    85,    86,    87,    88,    89,    90,    91,
      92,    93,    94,    95,    96,    97,    98,    99,   100,   101,
     102,   107,   109,   111,   154,   173,   154,   154,   154,   152,
     153,   154,   180,   135,    79,   128,   159,   160,   161,     5,
     128,   141,   142,   155,   156,    79,   137,    84,    82,   108,
      69,   112,   112,   178,   112,   112,   112,   112,   129,   112,
     112,   112,    77,    77,    77,    79,    17,    33,   128,   128,
     108,   154,   154,   154,   154,   154,   154,   154,   154,   154,
     154,   154,   154,   154,   154,   154,   154,   154,   154,   154,
     152,   154,   128,   113,   128,   174,   175,   108,   108,   108,
      82,   108,     4,   139,   140,   141,    79,   113,    82,   148,
     154,   107,   109,   154,   141,   136,    84,   157,   149,   164,
     164,   164,   130,   162,   164,   162,   163,   164,   154,   112,
     108,   108,    84,   108,   110,    84,   138,    81,    81,   154,
     112,   154,   161,    17,   153,   110,   154,   141,    16,   113,
     153,   158,    16,   113,   150,   151,   153,     8,    54,    55,
      58,    59,    64,    65,    66,    67,   113,   165,   113,   113,
     113,   128,   131,   113,   113,   113,    77,   157,   154,   142,
     137,   143,   112,   108,   110,    15,    15,    81,    15,    81,
     177,    15,   133,   112,   128,    79,    79,   170,   170,   170,
     170,    84,    81,   113,    21,   177,    84,   113,   128,   144,
     145,   146,   147,   149,   154,   154,   128,   128,   166,    56,
     128,   128,    69,    69,    69,    69,   142,   154,   176,    84,
      81,   177,    18,   141,   113,    84,    84,   113,   128,   167,
     107,   107,   172,   172,   177,    19,    20,   109,   141,   141,
      84,    81,   128,   168,   152,    60,    81,   154,   154,   154,
     142,    90,   108,   108,   107,   110,   128,   169,    68,   152,
     111,   170,   108,   128,    69,    81
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
#else
static void
yy_stack_print (yybottom, yytop)
    yytype_int16 *yybottom;
    yytype_int16 *yytop;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
}
#endif /* YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}

/* Prevent warnings from -Wmissing-prototypes.  */
#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */


/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*-------------------------.
| yyparse or yypush_parse.  |
`-------------------------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{


    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       `yyss': related to states.
       `yyvs': related to semantic values.

       Refer to the stacks thru separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yytoken = 0;
  yyss = yyssa;
  yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */
  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss_alloc, yyss);
	YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:

/* Line 1455 of yacc.c  */
#line 182 "pac_parse.yy"
    {
				// Put initialization here
				}
    break;

  case 3:

/* Line 1455 of yacc.c  */
#line 186 "pac_parse.yy"
    {
				}
    break;

  case 4:

/* Line 1455 of yacc.c  */
#line 191 "pac_parse.yy"
    {
				(yyval.decl) = (yyvsp[(1) - (2)].decl);
				(yyvsp[(1) - (2)].decl)->AddAttrs((yyvsp[(2) - (2)].attrlist));
				}
    break;

  case 5:

/* Line 1455 of yacc.c  */
#line 196 "pac_parse.yy"
    {
				(yyval.decl) = (yyvsp[(1) - (1)].decl);
				}
    break;

  case 6:

/* Line 1455 of yacc.c  */
#line 201 "pac_parse.yy"
    { current_decl_id = (yyvsp[(2) - (2)].id); }
    break;

  case 7:

/* Line 1455 of yacc.c  */
#line 202 "pac_parse.yy"
    {
				TypeDecl* decl = new TypeDecl((yyvsp[(2) - (6)].id), (yyvsp[(4) - (6)].paramlist), (yyvsp[(6) - (6)].type));
				(yyval.decl) = decl;
				}
    break;

  case 8:

/* Line 1455 of yacc.c  */
#line 206 "pac_parse.yy"
    { current_decl_id = (yyvsp[(2) - (2)].id); }
    break;

  case 9:

/* Line 1455 of yacc.c  */
#line 207 "pac_parse.yy"
    {
				(yyval.decl) = new LetDecl((yyvsp[(2) - (5)].id), (yyvsp[(4) - (5)].type), (yyvsp[(5) - (5)].expr));
				}
    break;

  case 10:

/* Line 1455 of yacc.c  */
#line 211 "pac_parse.yy"
    {
				current_decl_id = (yyvsp[(2) - (2)].function)->id();
				(yyval.decl) = new FuncDecl((yyvsp[(2) - (2)].function));
				}
    break;

  case 11:

/* Line 1455 of yacc.c  */
#line 215 "pac_parse.yy"
    { current_decl_id = (yyvsp[(2) - (2)].id); }
    break;

  case 12:

/* Line 1455 of yacc.c  */
#line 216 "pac_parse.yy"
    {
				(yyval.decl) = new EnumDecl((yyvsp[(2) - (6)].id), (yyvsp[(5) - (6)].enumlist));
				}
    break;

  case 13:

/* Line 1455 of yacc.c  */
#line 219 "pac_parse.yy"
    { current_decl_id = (yyvsp[(3) - (3)].id); }
    break;

  case 14:

/* Line 1455 of yacc.c  */
#line 220 "pac_parse.yy"
    {
				Type *extern_type = new ExternType((yyvsp[(3) - (4)].id), ExternType::PLAIN);
				(yyval.decl) = new TypeDecl((yyvsp[(3) - (4)].id), 0, extern_type);
				}
    break;

  case 15:

/* Line 1455 of yacc.c  */
#line 224 "pac_parse.yy"
    { current_decl_id = (yyvsp[(2) - (2)].id); }
    break;

  case 16:

/* Line 1455 of yacc.c  */
#line 225 "pac_parse.yy"
    {
				(yyval.decl) = new AnalyzerContextDecl((yyvsp[(2) - (5)].id), (yyvsp[(5) - (5)].contextfieldlist));
				}
    break;

  case 17:

/* Line 1455 of yacc.c  */
#line 228 "pac_parse.yy"
    { current_decl_id = (yyvsp[(2) - (2)].id); }
    break;

  case 18:

/* Line 1455 of yacc.c  */
#line 229 "pac_parse.yy"
    {
				(yyval.decl) = new ConnDecl((yyvsp[(2) - (7)].id), (yyvsp[(4) - (7)].paramlist), (yyvsp[(6) - (7)].aelemlist));
				}
    break;

  case 19:

/* Line 1455 of yacc.c  */
#line 232 "pac_parse.yy"
    { current_decl_id = (yyvsp[(2) - (2)].id); }
    break;

  case 20:

/* Line 1455 of yacc.c  */
#line 233 "pac_parse.yy"
    {
				(yyval.decl) = new ConnDecl((yyvsp[(2) - (7)].id), (yyvsp[(4) - (7)].paramlist), (yyvsp[(6) - (7)].aelemlist));
				}
    break;

  case 21:

/* Line 1455 of yacc.c  */
#line 236 "pac_parse.yy"
    { current_decl_id = (yyvsp[(2) - (2)].id); }
    break;

  case 22:

/* Line 1455 of yacc.c  */
#line 237 "pac_parse.yy"
    {
				(yyval.decl) = new FlowDecl((yyvsp[(2) - (7)].id), (yyvsp[(4) - (7)].paramlist), (yyvsp[(6) - (7)].aelemlist));
				}
    break;

  case 23:

/* Line 1455 of yacc.c  */
#line 241 "pac_parse.yy"
    {
				(yyval.decl) = ProcessCaseTypeRedef((yyvsp[(3) - (7)].id), (yyvsp[(6) - (7)].casefieldlist));
				}
    break;

  case 24:

/* Line 1455 of yacc.c  */
#line 245 "pac_parse.yy"
    {
				(yyval.decl) = ProcessCaseExprRedef((yyvsp[(3) - (7)].id), (yyvsp[(6) - (7)].caseexprlist));
				}
    break;

  case 25:

/* Line 1455 of yacc.c  */
#line 249 "pac_parse.yy"
    {
				(yyval.decl) = ProcessAnalyzerRedef((yyvsp[(3) - (7)].id), Decl::CONN, (yyvsp[(6) - (7)].aelemlist));
				}
    break;

  case 26:

/* Line 1455 of yacc.c  */
#line 253 "pac_parse.yy"
    {
				(yyval.decl) = ProcessAnalyzerRedef((yyvsp[(3) - (7)].id), Decl::CONN, (yyvsp[(6) - (7)].aelemlist));
				}
    break;

  case 27:

/* Line 1455 of yacc.c  */
#line 257 "pac_parse.yy"
    {
				(yyval.decl) = ProcessAnalyzerRedef((yyvsp[(3) - (7)].id), Decl::FLOW, (yyvsp[(6) - (7)].aelemlist));
				}
    break;

  case 28:

/* Line 1455 of yacc.c  */
#line 263 "pac_parse.yy"
    {
				(yyval.decl) = new HelperDecl(HelperDecl::HEADER, 0, (yyvsp[(2) - (3)].embedded_code));
				}
    break;

  case 29:

/* Line 1455 of yacc.c  */
#line 267 "pac_parse.yy"
    {
				(yyval.decl) = new HelperDecl(HelperDecl::CODE, 0, (yyvsp[(2) - (3)].embedded_code));
				}
    break;

  case 30:

/* Line 1455 of yacc.c  */
#line 271 "pac_parse.yy"
    {
				(yyval.decl) = new HelperDecl(HelperDecl::EXTERN, 0, (yyvsp[(2) - (3)].embedded_code));
				}
    break;

  case 31:

/* Line 1455 of yacc.c  */
#line 275 "pac_parse.yy"
    {
				(yyval.decl) = ProcessTypeAttrRedef((yyvsp[(3) - (5)].id), (yyvsp[(5) - (5)].attrlist));
				}
    break;

  case 34:

/* Line 1455 of yacc.c  */
#line 285 "pac_parse.yy"
    {
				(yyval.id) = (yyvsp[(1) - (1)].id);
				}
    break;

  case 35:

/* Line 1455 of yacc.c  */
#line 289 "pac_parse.yy"
    {
				(yyval.id) = new ID("connection");
				}
    break;

  case 36:

/* Line 1455 of yacc.c  */
#line 293 "pac_parse.yy"
    {
				(yyval.id) = new ID("analyzer");
				}
    break;

  case 37:

/* Line 1455 of yacc.c  */
#line 297 "pac_parse.yy"
    {
				(yyval.id) = new ID("flow");
				}
    break;

  case 38:

/* Line 1455 of yacc.c  */
#line 301 "pac_parse.yy"
    {
				(yyval.id) = new ID("function");
				}
    break;

  case 39:

/* Line 1455 of yacc.c  */
#line 305 "pac_parse.yy"
    {
				(yyval.id) = new ID("type");
				}
    break;

  case 40:

/* Line 1455 of yacc.c  */
#line 311 "pac_parse.yy"
    {
				(yyval.contextfieldlist) = (yyvsp[(2) - (3)].contextfieldlist);
				}
    break;

  case 41:

/* Line 1455 of yacc.c  */
#line 317 "pac_parse.yy"
    {
				(yyvsp[(1) - (3)].contextfieldlist)->push_back((yyvsp[(2) - (3)].contextfield));
				(yyval.contextfieldlist) = (yyvsp[(1) - (3)].contextfieldlist);
				}
    break;

  case 42:

/* Line 1455 of yacc.c  */
#line 322 "pac_parse.yy"
    {
				(yyval.contextfieldlist) = new ContextFieldList();
				}
    break;

  case 43:

/* Line 1455 of yacc.c  */
#line 328 "pac_parse.yy"
    {
				(yyval.contextfield) = new ContextField((yyvsp[(1) - (3)].id), (yyvsp[(3) - (3)].type));
				}
    break;

  case 44:

/* Line 1455 of yacc.c  */
#line 334 "pac_parse.yy"
    {
				(yyval.function) = new Function((yyvsp[(1) - (6)].id), (yyvsp[(6) - (6)].type), (yyvsp[(3) - (6)].paramlist));
				}
    break;

  case 45:

/* Line 1455 of yacc.c  */
#line 340 "pac_parse.yy"
    {
				(yyvsp[(1) - (3)].function)->set_expr((yyvsp[(3) - (3)].expr));
				(yyval.function) = (yyvsp[(1) - (3)].function);
				}
    break;

  case 46:

/* Line 1455 of yacc.c  */
#line 345 "pac_parse.yy"
    {
				(yyvsp[(1) - (4)].function)->set_code((yyvsp[(3) - (4)].embedded_code));
				(yyval.function) = (yyvsp[(1) - (4)].function);
				}
    break;

  case 47:

/* Line 1455 of yacc.c  */
#line 350 "pac_parse.yy"
    {
				(yyval.function) = (yyvsp[(1) - (2)].function);
				}
    break;

  case 48:

/* Line 1455 of yacc.c  */
#line 356 "pac_parse.yy"
    {
				(yyval.paramlist) = (yyvsp[(2) - (3)].paramlist);
				}
    break;

  case 49:

/* Line 1455 of yacc.c  */
#line 360 "pac_parse.yy"
    {
				(yyval.paramlist) = 0;
				}
    break;

  case 50:

/* Line 1455 of yacc.c  */
#line 366 "pac_parse.yy"
    {
				(yyvsp[(1) - (3)].paramlist)->push_back((yyvsp[(3) - (3)].param));
				(yyval.paramlist) = (yyvsp[(1) - (3)].paramlist);
				}
    break;

  case 51:

/* Line 1455 of yacc.c  */
#line 371 "pac_parse.yy"
    {
				(yyval.paramlist) = new ParamList();
				(yyval.paramlist)->push_back((yyvsp[(1) - (1)].param));
				}
    break;

  case 52:

/* Line 1455 of yacc.c  */
#line 376 "pac_parse.yy"
    {
				(yyval.paramlist) = new ParamList();
				}
    break;

  case 53:

/* Line 1455 of yacc.c  */
#line 382 "pac_parse.yy"
    {
				(yyval.param) = new Param((yyvsp[(1) - (3)].id), (yyvsp[(3) - (3)].type));
				}
    break;

  case 54:

/* Line 1455 of yacc.c  */
#line 388 "pac_parse.yy"
    {
				(yyval.expr) = 0;
				}
    break;

  case 55:

/* Line 1455 of yacc.c  */
#line 392 "pac_parse.yy"
    {
				(yyval.expr) = (yyvsp[(2) - (2)].expr);
				}
    break;

  case 56:

/* Line 1455 of yacc.c  */
#line 398 "pac_parse.yy"
    {
				(yyval.type) = 0;
				}
    break;

  case 57:

/* Line 1455 of yacc.c  */
#line 402 "pac_parse.yy"
    {
				(yyval.type) = (yyvsp[(2) - (2)].type);
				}
    break;

  case 58:

/* Line 1455 of yacc.c  */
#line 408 "pac_parse.yy"
    {
				(yyval.type) = (yyvsp[(1) - (1)].type);
				}
    break;

  case 59:

/* Line 1455 of yacc.c  */
#line 415 "pac_parse.yy"
    {
				(yyval.type) = (yyvsp[(1) - (1)].type);
				}
    break;

  case 60:

/* Line 1455 of yacc.c  */
#line 419 "pac_parse.yy"
    {
				(yyval.type) = new RecordType((yyvsp[(3) - (4)].recordfieldlist));
				}
    break;

  case 61:

/* Line 1455 of yacc.c  */
#line 426 "pac_parse.yy"
    {
				(yyval.type) = (yyvsp[(1) - (1)].type);
				}
    break;

  case 62:

/* Line 1455 of yacc.c  */
#line 430 "pac_parse.yy"
    {
				(yyval.type) = new ArrayType((yyvsp[(1) - (4)].type), (yyvsp[(3) - (4)].expr));
				}
    break;

  case 63:

/* Line 1455 of yacc.c  */
#line 434 "pac_parse.yy"
    {
				(yyval.type) = new ArrayType((yyvsp[(1) - (3)].type));
				}
    break;

  case 64:

/* Line 1455 of yacc.c  */
#line 438 "pac_parse.yy"
    {
				(yyval.type) = new CaseType((yyvsp[(2) - (6)].expr), (yyvsp[(5) - (6)].casefieldlist));
				}
    break;

  case 65:

/* Line 1455 of yacc.c  */
#line 445 "pac_parse.yy"
    {
				(yyval.type) = Type::LookUpByID((yyvsp[(1) - (1)].id));
				}
    break;

  case 66:

/* Line 1455 of yacc.c  */
#line 449 "pac_parse.yy"
    {
				(yyval.type) = new ParameterizedType((yyvsp[(1) - (4)].id), (yyvsp[(3) - (4)].exprlist));
				}
    break;

  case 67:

/* Line 1455 of yacc.c  */
#line 453 "pac_parse.yy"
    {
				(yyval.type) = new StringType((yyvsp[(1) - (1)].regex));
				}
    break;

  case 68:

/* Line 1455 of yacc.c  */
#line 457 "pac_parse.yy"
    {
				(yyval.type) = new StringType((yyvsp[(1) - (1)].cstr));
				}
    break;

  case 69:

/* Line 1455 of yacc.c  */
#line 463 "pac_parse.yy"
    {
				(yyvsp[(1) - (3)].recordfieldlist)->push_back((yyvsp[(2) - (3)].recordfield));
				(yyval.recordfieldlist) = (yyvsp[(1) - (3)].recordfieldlist);
				}
    break;

  case 70:

/* Line 1455 of yacc.c  */
#line 468 "pac_parse.yy"
    {
				(yyval.recordfieldlist) = new RecordFieldList();
				}
    break;

  case 71:

/* Line 1455 of yacc.c  */
#line 474 "pac_parse.yy"
    {
				(yyvsp[(1) - (2)].recordfield)->AddAttr((yyvsp[(2) - (2)].attrlist));
				(yyval.recordfield) = (yyvsp[(1) - (2)].recordfield);
				}
    break;

  case 72:

/* Line 1455 of yacc.c  */
#line 481 "pac_parse.yy"
    {
				(yyval.recordfield) = new RecordDataField((yyvsp[(1) - (2)].id), (yyvsp[(2) - (2)].type));
				}
    break;

  case 73:

/* Line 1455 of yacc.c  */
#line 485 "pac_parse.yy"
    {
				(yyval.recordfield) = (yyvsp[(1) - (1)].recordfield);
				}
    break;

  case 74:

/* Line 1455 of yacc.c  */
#line 491 "pac_parse.yy"
    {
				(yyval.recordfield) = new RecordPaddingField(
					(yyvsp[(1) - (5)].id), PAD_BY_LENGTH, (yyvsp[(4) - (5)].expr));
				}
    break;

  case 75:

/* Line 1455 of yacc.c  */
#line 496 "pac_parse.yy"
    {
				(yyval.recordfield) = new RecordPaddingField(
					(yyvsp[(1) - (4)].id), PAD_TO_OFFSET, (yyvsp[(4) - (4)].expr));
				}
    break;

  case 76:

/* Line 1455 of yacc.c  */
#line 501 "pac_parse.yy"
    {
				(yyval.recordfield) = new RecordPaddingField(
					(yyvsp[(1) - (4)].id), PAD_TO_NEXT_WORD, (yyvsp[(4) - (4)].expr));
				}
    break;

  case 77:

/* Line 1455 of yacc.c  */
#line 508 "pac_parse.yy"
    {
				(yyval.id) = (yyvsp[(1) - (2)].id);
				}
    break;

  case 78:

/* Line 1455 of yacc.c  */
#line 512 "pac_parse.yy"
    {
				(yyval.id) = ID::NewAnonymousID("anonymous_field_");
				}
    break;

  case 79:

/* Line 1455 of yacc.c  */
#line 518 "pac_parse.yy"
    {
				(yyval.expr) = (yyvsp[(1) - (1)].expr);
				}
    break;

  case 80:

/* Line 1455 of yacc.c  */
#line 524 "pac_parse.yy"
    {
				(yyvsp[(1) - (3)].casefieldlist)->push_back((yyvsp[(2) - (3)].casefield));
				(yyval.casefieldlist) = (yyvsp[(1) - (3)].casefieldlist);
				}
    break;

  case 81:

/* Line 1455 of yacc.c  */
#line 529 "pac_parse.yy"
    {
				(yyval.casefieldlist) = new CaseFieldList();
				}
    break;

  case 82:

/* Line 1455 of yacc.c  */
#line 535 "pac_parse.yy"
    {
				(yyvsp[(1) - (2)].casefield)->AddAttr((yyvsp[(2) - (2)].attrlist));	
				(yyval.casefield) = (yyvsp[(1) - (2)].casefield);
				}
    break;

  case 83:

/* Line 1455 of yacc.c  */
#line 542 "pac_parse.yy"
    {
				(yyval.casefield) = new CaseField((yyvsp[(1) - (5)].exprlist), (yyvsp[(3) - (5)].id), (yyvsp[(5) - (5)].type));
				}
    break;

  case 84:

/* Line 1455 of yacc.c  */
#line 546 "pac_parse.yy"
    {
				(yyval.casefield) = new CaseField(0, (yyvsp[(3) - (5)].id), (yyvsp[(5) - (5)].type));
				}
    break;

  case 85:

/* Line 1455 of yacc.c  */
#line 552 "pac_parse.yy"
    {
				(yyval.exprlist) = 0;
				}
    break;

  case 86:

/* Line 1455 of yacc.c  */
#line 556 "pac_parse.yy"
    {
				(yyval.exprlist) = (yyvsp[(1) - (1)].exprlist);
				}
    break;

  case 87:

/* Line 1455 of yacc.c  */
#line 562 "pac_parse.yy"
    {
				(yyvsp[(1) - (3)].exprlist)->push_back((yyvsp[(3) - (3)].expr));
				(yyval.exprlist) = (yyvsp[(1) - (3)].exprlist);
				}
    break;

  case 88:

/* Line 1455 of yacc.c  */
#line 567 "pac_parse.yy"
    {
				(yyval.exprlist) = new ExprList();
				(yyval.exprlist)->push_back((yyvsp[(1) - (1)].expr));
				}
    break;

  case 89:

/* Line 1455 of yacc.c  */
#line 574 "pac_parse.yy"
    {
				(yyval.expr) = new Expr((yyvsp[(1) - (1)].id));
				}
    break;

  case 90:

/* Line 1455 of yacc.c  */
#line 578 "pac_parse.yy"
    {
				(yyval.expr) = new Expr((yyvsp[(1) - (1)].num));
				}
    break;

  case 91:

/* Line 1455 of yacc.c  */
#line 582 "pac_parse.yy"
    {
				(yyval.expr) = new Expr(Expr::EXPR_SUBSCRIPT, (yyvsp[(1) - (4)].expr), (yyvsp[(3) - (4)].expr));
				}
    break;

  case 92:

/* Line 1455 of yacc.c  */
#line 586 "pac_parse.yy"
    {
				(yyval.expr) = new Expr(Expr::EXPR_MEMBER, (yyvsp[(1) - (3)].expr), new Expr((yyvsp[(3) - (3)].id)));
				}
    break;

  case 93:

/* Line 1455 of yacc.c  */
#line 590 "pac_parse.yy"
    {
				(yyval.expr) = new Expr(Expr::EXPR_SIZEOF, new Expr((yyvsp[(3) - (4)].id)));
				}
    break;

  case 94:

/* Line 1455 of yacc.c  */
#line 594 "pac_parse.yy"
    {
				(yyval.expr) = new Expr(Expr::EXPR_OFFSETOF, new Expr((yyvsp[(3) - (4)].id)));
				}
    break;

  case 95:

/* Line 1455 of yacc.c  */
#line 598 "pac_parse.yy"
    {
				(yyval.expr) = new Expr(Expr::EXPR_PAREN, (yyvsp[(2) - (3)].expr));
				}
    break;

  case 96:

/* Line 1455 of yacc.c  */
#line 602 "pac_parse.yy"
    {
				(yyval.expr) = new Expr(Expr::EXPR_CALL, 
				              (yyvsp[(1) - (4)].expr), 
				              new Expr((yyvsp[(3) - (4)].exprlist)));
				}
    break;

  case 97:

/* Line 1455 of yacc.c  */
#line 608 "pac_parse.yy"
    {
				(yyval.expr) = new Expr(Expr::EXPR_NEG, (yyvsp[(2) - (2)].expr));
				}
    break;

  case 98:

/* Line 1455 of yacc.c  */
#line 612 "pac_parse.yy"
    {
				(yyval.expr) = new Expr(Expr::EXPR_PLUS, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr));
				}
    break;

  case 99:

/* Line 1455 of yacc.c  */
#line 616 "pac_parse.yy"
    {
				(yyval.expr) = new Expr(Expr::EXPR_MINUS, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr));
				}
    break;

  case 100:

/* Line 1455 of yacc.c  */
#line 620 "pac_parse.yy"
    {
				(yyval.expr) = new Expr(Expr::EXPR_TIMES, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr));
				}
    break;

  case 101:

/* Line 1455 of yacc.c  */
#line 624 "pac_parse.yy"
    {
				(yyval.expr) = new Expr(Expr::EXPR_DIV, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr));
				}
    break;

  case 102:

/* Line 1455 of yacc.c  */
#line 628 "pac_parse.yy"
    {
				(yyval.expr) = new Expr(Expr::EXPR_MOD, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr));
				}
    break;

  case 103:

/* Line 1455 of yacc.c  */
#line 632 "pac_parse.yy"
    {
				(yyval.expr) = new Expr(Expr::EXPR_BITNOT, (yyvsp[(2) - (2)].expr));
				}
    break;

  case 104:

/* Line 1455 of yacc.c  */
#line 636 "pac_parse.yy"
    {
				(yyval.expr) = new Expr(Expr::EXPR_BITAND, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr));
				}
    break;

  case 105:

/* Line 1455 of yacc.c  */
#line 640 "pac_parse.yy"
    {
				(yyval.expr) = new Expr(Expr::EXPR_BITOR, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr));
				}
    break;

  case 106:

/* Line 1455 of yacc.c  */
#line 644 "pac_parse.yy"
    {
				(yyval.expr) = new Expr(Expr::EXPR_BITXOR, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr));
				}
    break;

  case 107:

/* Line 1455 of yacc.c  */
#line 648 "pac_parse.yy"
    {
				(yyval.expr) = new Expr(Expr::EXPR_LSHIFT, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr));
				}
    break;

  case 108:

/* Line 1455 of yacc.c  */
#line 652 "pac_parse.yy"
    {
				(yyval.expr) = new Expr(Expr::EXPR_RSHIFT, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr));
				}
    break;

  case 109:

/* Line 1455 of yacc.c  */
#line 656 "pac_parse.yy"
    {
				(yyval.expr) = new Expr(Expr::EXPR_EQUAL, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr));
				}
    break;

  case 110:

/* Line 1455 of yacc.c  */
#line 660 "pac_parse.yy"
    {
				(yyval.expr) = new Expr(Expr::EXPR_NEQ, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr));
				}
    break;

  case 111:

/* Line 1455 of yacc.c  */
#line 664 "pac_parse.yy"
    {
				(yyval.expr) = new Expr(Expr::EXPR_GE, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr));
				}
    break;

  case 112:

/* Line 1455 of yacc.c  */
#line 668 "pac_parse.yy"
    {
				(yyval.expr) = new Expr(Expr::EXPR_LE, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr));
				}
    break;

  case 113:

/* Line 1455 of yacc.c  */
#line 672 "pac_parse.yy"
    {
				(yyval.expr) = new Expr(Expr::EXPR_GT, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr));
				}
    break;

  case 114:

/* Line 1455 of yacc.c  */
#line 676 "pac_parse.yy"
    {
				(yyval.expr) = new Expr(Expr::EXPR_LT, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr));
				}
    break;

  case 115:

/* Line 1455 of yacc.c  */
#line 680 "pac_parse.yy"
    {
				(yyval.expr) = new Expr(Expr::EXPR_NOT, (yyvsp[(2) - (2)].expr));
				}
    break;

  case 116:

/* Line 1455 of yacc.c  */
#line 684 "pac_parse.yy"
    {
				(yyval.expr) = new Expr(Expr::EXPR_AND, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr));
				}
    break;

  case 117:

/* Line 1455 of yacc.c  */
#line 688 "pac_parse.yy"
    {
				(yyval.expr) = new Expr(Expr::EXPR_OR, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr));
				}
    break;

  case 118:

/* Line 1455 of yacc.c  */
#line 692 "pac_parse.yy"
    {
				(yyval.expr) = new Expr(Expr::EXPR_COND, (yyvsp[(1) - (5)].expr), (yyvsp[(3) - (5)].expr), (yyvsp[(5) - (5)].expr));
				}
    break;

  case 119:

/* Line 1455 of yacc.c  */
#line 696 "pac_parse.yy"
    {
				(yyval.expr) = new Expr((yyvsp[(2) - (6)].expr), (yyvsp[(5) - (6)].caseexprlist));
				}
    break;

  case 120:

/* Line 1455 of yacc.c  */
#line 700 "pac_parse.yy"
    {
				(yyval.expr) = new Expr((yyvsp[(1) - (1)].cstr));
				}
    break;

  case 121:

/* Line 1455 of yacc.c  */
#line 704 "pac_parse.yy"
    {
				(yyval.expr) = new Expr((yyvsp[(1) - (1)].regex));
				}
    break;

  case 122:

/* Line 1455 of yacc.c  */
#line 710 "pac_parse.yy"
    {
				(yyval.cstr) = new ConstString((yyvsp[(1) - (1)].str));
				}
    break;

  case 123:

/* Line 1455 of yacc.c  */
#line 716 "pac_parse.yy"
    { 
				(yyval.regex) = new RegEx((yyvsp[(2) - (3)].str)); 
				}
    break;

  case 124:

/* Line 1455 of yacc.c  */
#line 722 "pac_parse.yy"
    {
				(yyval.caseexprlist) = new CaseExprList();
				}
    break;

  case 125:

/* Line 1455 of yacc.c  */
#line 726 "pac_parse.yy"
    {
				(yyvsp[(1) - (3)].caseexprlist)->push_back((yyvsp[(2) - (3)].caseexpr));
				(yyval.caseexprlist) = (yyvsp[(1) - (3)].caseexprlist);
				}
    break;

  case 126:

/* Line 1455 of yacc.c  */
#line 733 "pac_parse.yy"
    {
				(yyval.caseexpr) = new CaseExpr((yyvsp[(1) - (3)].exprlist), (yyvsp[(3) - (3)].expr));
				}
    break;

  case 127:

/* Line 1455 of yacc.c  */
#line 737 "pac_parse.yy"
    {
				(yyval.caseexpr) = new CaseExpr(0, (yyvsp[(3) - (3)].expr));
				}
    break;

  case 128:

/* Line 1455 of yacc.c  */
#line 743 "pac_parse.yy"
    {
				(yyval.enumlist) = (yyvsp[(1) - (1)].enumlist);
				}
    break;

  case 129:

/* Line 1455 of yacc.c  */
#line 747 "pac_parse.yy"
    {
				(yyval.enumlist) = (yyvsp[(1) - (2)].enumlist);
				}
    break;

  case 130:

/* Line 1455 of yacc.c  */
#line 753 "pac_parse.yy"
    {
				(yyvsp[(1) - (3)].enumlist)->push_back((yyvsp[(3) - (3)].enumitem));
				(yyval.enumlist) = (yyvsp[(1) - (3)].enumlist);
				}
    break;

  case 131:

/* Line 1455 of yacc.c  */
#line 758 "pac_parse.yy"
    {
				(yyval.enumlist) = new EnumList();
				(yyval.enumlist)->push_back((yyvsp[(1) - (1)].enumitem));
				}
    break;

  case 132:

/* Line 1455 of yacc.c  */
#line 765 "pac_parse.yy"
    {
				(yyval.enumitem) = new Enum((yyvsp[(1) - (1)].id));
				}
    break;

  case 133:

/* Line 1455 of yacc.c  */
#line 769 "pac_parse.yy"
    {
				(yyval.enumitem) = new Enum((yyvsp[(1) - (3)].id), (yyvsp[(3) - (3)].expr));
				}
    break;

  case 134:

/* Line 1455 of yacc.c  */
#line 775 "pac_parse.yy"
    {
				(yyval.aelemlist) = (yyvsp[(1) - (1)].aelemlist);
				}
    break;

  case 135:

/* Line 1455 of yacc.c  */
#line 781 "pac_parse.yy"
    {
				(yyval.aelemlist) = (yyvsp[(1) - (1)].aelemlist);
				}
    break;

  case 136:

/* Line 1455 of yacc.c  */
#line 788 "pac_parse.yy"
    {
				(yyval.aelemlist) = new AnalyzerElementList();
				}
    break;

  case 137:

/* Line 1455 of yacc.c  */
#line 792 "pac_parse.yy"
    {
				(yyvsp[(1) - (2)].aelemlist)->push_back((yyvsp[(2) - (2)].aelem));
				(yyval.aelemlist) = (yyvsp[(1) - (2)].aelemlist);
				}
    break;

  case 138:

/* Line 1455 of yacc.c  */
#line 799 "pac_parse.yy"
    {
				(yyval.aelem) = new AnalyzerHelper(AnalyzerHelper::MEMBER_DECLS, (yyvsp[(2) - (3)].embedded_code));
				}
    break;

  case 139:

/* Line 1455 of yacc.c  */
#line 803 "pac_parse.yy"
    {
				(yyval.aelem) = new AnalyzerHelper(AnalyzerHelper::INIT_CODE, (yyvsp[(2) - (3)].embedded_code));
				}
    break;

  case 140:

/* Line 1455 of yacc.c  */
#line 807 "pac_parse.yy"
    {
				(yyval.aelem) = new AnalyzerHelper(AnalyzerHelper::CLEANUP_CODE, (yyvsp[(2) - (3)].embedded_code));
				}
    break;

  case 141:

/* Line 1455 of yacc.c  */
#line 811 "pac_parse.yy"
    {
				(yyval.aelem) = new AnalyzerHelper(AnalyzerHelper::EOF_CODE, (yyvsp[(2) - (3)].embedded_code));
				}
    break;

  case 142:

/* Line 1455 of yacc.c  */
#line 815 "pac_parse.yy"
    {
				(yyval.aelem) = new AnalyzerFlow((AnalyzerFlow::Direction) (yyvsp[(1) - (5)].val), (yyvsp[(3) - (5)].id), (yyvsp[(4) - (5)].exprlist));
				}
    break;

  case 143:

/* Line 1455 of yacc.c  */
#line 819 "pac_parse.yy"
    {
				(yyval.aelem) = new AnalyzerDataUnit(
					(AnalyzerDataUnit::DataUnitType) (yyvsp[(1) - (9)].val), 
					(yyvsp[(3) - (9)].id), 
					(yyvsp[(4) - (9)].exprlist),
					(yyvsp[(7) - (9)].exprlist));
				}
    break;

  case 144:

/* Line 1455 of yacc.c  */
#line 827 "pac_parse.yy"
    {
				(yyval.aelem) = new AnalyzerFunction((yyvsp[(2) - (2)].function));
				}
    break;

  case 145:

/* Line 1455 of yacc.c  */
#line 831 "pac_parse.yy"
    {
				(yyval.aelem) = new AnalyzerState((yyvsp[(3) - (4)].statevarlist));
				}
    break;

  case 146:

/* Line 1455 of yacc.c  */
#line 835 "pac_parse.yy"
    {
				(yyval.aelem) = new AnalyzerAction((yyvsp[(2) - (9)].id), (AnalyzerAction::When) (yyvsp[(3) - (9)].val), (yyvsp[(5) - (9)].actionparam), (yyvsp[(8) - (9)].embedded_code));
				}
    break;

  case 147:

/* Line 1455 of yacc.c  */
#line 841 "pac_parse.yy"
    {
				(yyval.statevarlist) = new StateVarList();
				}
    break;

  case 148:

/* Line 1455 of yacc.c  */
#line 845 "pac_parse.yy"
    {
				(yyvsp[(1) - (3)].statevarlist)->push_back((yyvsp[(2) - (3)].statevar));
				(yyval.statevarlist) = (yyvsp[(1) - (3)].statevarlist);
				}
    break;

  case 149:

/* Line 1455 of yacc.c  */
#line 852 "pac_parse.yy"
    {
				(yyval.statevar) = new StateVar((yyvsp[(1) - (3)].id), (yyvsp[(3) - (3)].type));
				}
    break;

  case 150:

/* Line 1455 of yacc.c  */
#line 858 "pac_parse.yy"
    {
				(yyval.actionparam) = new ActionParam((yyvsp[(1) - (3)].id), (yyvsp[(3) - (3)].actionparamtype));
				}
    break;

  case 151:

/* Line 1455 of yacc.c  */
#line 864 "pac_parse.yy"
    {
				(yyval.actionparamtype) = new ActionParamType((yyvsp[(1) - (1)].id));
				}
    break;

  case 152:

/* Line 1455 of yacc.c  */
#line 868 "pac_parse.yy"
    {
				(yyval.actionparamtype) = new ActionParamType((yyvsp[(1) - (3)].id), (yyvsp[(3) - (3)].id));
				}
    break;

  case 153:

/* Line 1455 of yacc.c  */
#line 874 "pac_parse.yy"
    {
				(yyval.embedded_code) = new EmbeddedCode();
				}
    break;

  case 154:

/* Line 1455 of yacc.c  */
#line 878 "pac_parse.yy"
    {
				(yyvsp[(1) - (2)].embedded_code)->Append((yyvsp[(2) - (2)].val));
				(yyval.embedded_code) = (yyvsp[(1) - (2)].embedded_code);
				}
    break;

  case 155:

/* Line 1455 of yacc.c  */
#line 883 "pac_parse.yy"
    {
				(yyvsp[(1) - (2)].embedded_code)->Append((yyvsp[(2) - (2)].str));
				(yyval.embedded_code) = (yyvsp[(1) - (2)].embedded_code);
				}
    break;

  case 156:

/* Line 1455 of yacc.c  */
#line 888 "pac_parse.yy"
    {
				(yyvsp[(1) - (2)].embedded_code)->Append((yyvsp[(2) - (2)].pacprimitive));
				(yyval.embedded_code) = (yyvsp[(1) - (2)].embedded_code);
				}
    break;

  case 157:

/* Line 1455 of yacc.c  */
#line 895 "pac_parse.yy"
    {
				(yyval.pacprimitive) = new PPVal((yyvsp[(2) - (3)].expr));
				}
    break;

  case 158:

/* Line 1455 of yacc.c  */
#line 899 "pac_parse.yy"
    {
				(yyval.pacprimitive) = new PPSet((yyvsp[(2) - (3)].expr));
				}
    break;

  case 159:

/* Line 1455 of yacc.c  */
#line 903 "pac_parse.yy"
    {
				(yyval.pacprimitive) = new PPType((yyvsp[(2) - (3)].expr));
				}
    break;

  case 160:

/* Line 1455 of yacc.c  */
#line 907 "pac_parse.yy"
    {
				(yyval.pacprimitive) = new PPConstDef((yyvsp[(2) - (5)].id), (yyvsp[(4) - (5)].expr));
				}
    break;

  case 161:

/* Line 1455 of yacc.c  */
#line 913 "pac_parse.yy"
    {
				(yyval.exprlist) = 0;
				}
    break;

  case 162:

/* Line 1455 of yacc.c  */
#line 917 "pac_parse.yy"
    {
				(yyval.exprlist) = (yyvsp[(2) - (3)].exprlist);
				}
    break;

  case 163:

/* Line 1455 of yacc.c  */
#line 923 "pac_parse.yy"
    {
				(yyvsp[(1) - (3)].fieldlist)->push_back((yyvsp[(2) - (3)].field));
				(yyval.fieldlist) = (yyvsp[(1) - (3)].fieldlist);
				}
    break;

  case 164:

/* Line 1455 of yacc.c  */
#line 928 "pac_parse.yy"
    {
				(yyvsp[(1) - (3)].fieldlist)->push_back((yyvsp[(2) - (3)].field));
				(yyval.fieldlist) = (yyvsp[(1) - (3)].fieldlist);
				}
    break;

  case 165:

/* Line 1455 of yacc.c  */
#line 933 "pac_parse.yy"
    {
				(yyval.fieldlist) = new FieldList();
				}
    break;

  case 166:

/* Line 1455 of yacc.c  */
#line 939 "pac_parse.yy"
    {
				(yyval.field) = new LetField((yyvsp[(1) - (4)].id), (yyvsp[(2) - (4)].type), (yyvsp[(3) - (4)].expr));
				(yyval.field)->AddAttr((yyvsp[(4) - (4)].attrlist));
				}
    break;

  case 167:

/* Line 1455 of yacc.c  */
#line 946 "pac_parse.yy"
    {
				(yyval.field) = new WithInputField((yyvsp[(1) - (6)].id), (yyvsp[(3) - (6)].type), (yyvsp[(5) - (6)].input));
				(yyval.field)->AddAttr((yyvsp[(6) - (6)].attrlist));
				}
    break;

  case 168:

/* Line 1455 of yacc.c  */
#line 954 "pac_parse.yy"
    {
				(yyval.input) = new InputBuffer((yyvsp[(1) - (1)].expr));
				}
    break;

  case 169:

/* Line 1455 of yacc.c  */
#line 960 "pac_parse.yy"
    {
				(yyval.attrlist) = 0;
				}
    break;

  case 170:

/* Line 1455 of yacc.c  */
#line 964 "pac_parse.yy"
    {
				(yyval.attrlist) = (yyvsp[(1) - (1)].attrlist);
				}
    break;

  case 171:

/* Line 1455 of yacc.c  */
#line 970 "pac_parse.yy"
    {
				if ( (yyvsp[(3) - (3)].attr) )
					(yyvsp[(1) - (3)].attrlist)->push_back((yyvsp[(3) - (3)].attr));
				(yyval.attrlist) = (yyvsp[(1) - (3)].attrlist);
				}
    break;

  case 172:

/* Line 1455 of yacc.c  */
#line 976 "pac_parse.yy"
    {
				(yyval.attrlist) = new AttrList();
				if ( (yyvsp[(1) - (1)].attr) )
					(yyval.attrlist)->push_back((yyvsp[(1) - (1)].attr));
				}
    break;

  case 175:

/* Line 1455 of yacc.c  */
#line 988 "pac_parse.yy"
    {
				(yyval.attr) = new Attr(ATTR_BYTEORDER, (yyvsp[(3) - (3)].expr));
				}
    break;

  case 176:

/* Line 1455 of yacc.c  */
#line 992 "pac_parse.yy"
    {
				(yyval.attr) = new Attr(ATTR_CHECK, (yyvsp[(2) - (2)].expr));
				}
    break;

  case 177:

/* Line 1455 of yacc.c  */
#line 996 "pac_parse.yy"
    {
				(yyval.attr) = new Attr(ATTR_CHUNKED);
				}
    break;

  case 178:

/* Line 1455 of yacc.c  */
#line 1000 "pac_parse.yy"
    {
				(yyval.attr) = new Attr(ATTR_EXPORTSOURCEDATA);
				}
    break;

  case 179:

/* Line 1455 of yacc.c  */
#line 1004 "pac_parse.yy"
    {
				(yyval.attr) = new Attr(ATTR_IF, (yyvsp[(2) - (2)].expr));
				}
    break;

  case 180:

/* Line 1455 of yacc.c  */
#line 1008 "pac_parse.yy"
    {
				(yyval.attr) = new Attr(ATTR_LENGTH, (yyvsp[(3) - (3)].expr));
				}
    break;

  case 181:

/* Line 1455 of yacc.c  */
#line 1012 "pac_parse.yy"
    {
				(yyval.attr) = new LetAttr((yyvsp[(3) - (4)].fieldlist));
				}
    break;

  case 182:

/* Line 1455 of yacc.c  */
#line 1016 "pac_parse.yy"
    {
				(yyval.attr) = new Attr(ATTR_LINEBREAKER, (yyvsp[(3) - (3)].expr));
				}
    break;

  case 183:

/* Line 1455 of yacc.c  */
#line 1020 "pac_parse.yy"
    {
				(yyval.attr) = new Attr(ATTR_MULTILINE, (yyvsp[(3) - (4)].expr));
				}
    break;

  case 184:

/* Line 1455 of yacc.c  */
#line 1024 "pac_parse.yy"
    {
				(yyval.attr) = new Attr(ATTR_ONELINE, (yyvsp[(2) - (2)].expr));
				}
    break;

  case 185:

/* Line 1455 of yacc.c  */
#line 1028 "pac_parse.yy"
    {
				(yyval.attr) = new Attr(ATTR_REFCOUNT);
				}
    break;

  case 186:

/* Line 1455 of yacc.c  */
#line 1032 "pac_parse.yy"
    {
				(yyval.attr) = new Attr(ATTR_REQUIRES, (yyvsp[(3) - (4)].exprlist));
				}
    break;

  case 187:

/* Line 1455 of yacc.c  */
#line 1036 "pac_parse.yy"
    {
				(yyval.attr) = new Attr(ATTR_RESTOFDATA);
				}
    break;

  case 188:

/* Line 1455 of yacc.c  */
#line 1040 "pac_parse.yy"
    {
				(yyval.attr) = new Attr(ATTR_RESTOFFLOW);
				}
    break;

  case 189:

/* Line 1455 of yacc.c  */
#line 1044 "pac_parse.yy"
    {
				(yyval.attr) = new Attr(ATTR_TRANSIENT);
				}
    break;

  case 190:

/* Line 1455 of yacc.c  */
#line 1048 "pac_parse.yy"
    {
				(yyval.attr) = new Attr(ATTR_UNTIL, (yyvsp[(2) - (2)].expr));
				}
    break;

  case 191:

/* Line 1455 of yacc.c  */
#line 1054 "pac_parse.yy"
    {
				(yyval.expr) = 0;
				}
    break;

  case 192:

/* Line 1455 of yacc.c  */
#line 1058 "pac_parse.yy"
    {
				(yyval.expr) = (yyvsp[(2) - (3)].expr);
				}
    break;



/* Line 1455 of yacc.c  */
#line 3868 "pac_parse.cc"
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (yymsg);
	  }
	else
	  {
	    yyerror (YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;


      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  *++yyvsp = yylval;


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined(yyoverflow) || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval);
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}



/* Line 1675 of yacc.c  */
#line 1063 "pac_parse.yy"


const ID* current_decl_id = 0;

int yyerror(const char msg[])
	{
	char* msgbuf = 
		new char[strlen(msg) + yyleng + 64];

	if ( ! yychar || ! yytext || yytext[0] == '\0' )
		sprintf(msgbuf, "%s, at end of file", msg);

	else if ( yytext[0] == '\n' )
		sprintf(msgbuf, "%s, on previous line", msg);

	else
		sprintf(msgbuf, "%s, at or near \"%s\"", msg, yytext);

	/*
	extern int column;
	sprintf(msgbuf, "%*s\n%*s\n", column, "^", column, msg);
	*/

	if ( ! input_filename.empty() )
		fprintf(stderr, "%s:%d: ", input_filename.c_str(), line_number);
	else
		fprintf(stderr, "line %d: ", line_number);
	fprintf(stderr, "%s", msgbuf);
	fprintf(stderr, " (yychar=%d)", yychar);
	fprintf(stderr, "\n");

	return 0;
        }

