
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton interface for Bison's Yacc-like parsers in C
   
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

/* Line 1676 of yacc.c  */
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



/* Line 1676 of yacc.c  */
#line 273 "pac_parse.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


