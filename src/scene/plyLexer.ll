%{
#include <cstdio>
#include <fstream>
#include <sstream>

#include "plyParser.hpp"
%}

/* %option prefix="ply" */
%option bison-bridge
%option warn noyywrap reentrant

ID [_[:alpha:]][_[:alnum:]]*
VERSION_STRING ([[:digit:]]+".")*[[:digit:]]

%x COMMENT

%% 

"ply"  { return PLY_TOKEN; }

"format"  { return FORMAT_TOKEN; }
"ascii "{VERSION_STRING}  { return VERSION_TOKEN; }

"comment "  { BEGIN(COMMENT);  return COMMENT_TOKEN; }
<COMMENT>[^\n]*  { BEGIN(INITIAL);  return COMMENT_STRING_TOKEN; }

"element"  { return ELEMENT_TOKEN; }
"vertex"  { return VERTEX_TOKEN; }
"face"  { return FACE_TOKEN; }

"property"  { return PROPERTY_TOKEN; }
"list"  { return LIST_TOKEN; }

"float"  { return FLOAT_TOKEN; }
"int"  { return INT_TOKEN; }
"uchar"  { return UCHAR_TOKEN; }

"x"  { return X_TOKEN; }
"y"  { return Y_TOKEN; }
"z"  { return Z_TOKEN; }

"vertex_indices"  { return VERTEX_INDICES_TOKEN; }

"end_header"  { return END_HEADER_TOKEN; }

"-"?[[:digit:]]+ {
  try {
    yylval->integer = std::stoi(yytext);
  } catch (...) {
    return ERROR_TOKEN;
  }
  return INTEGER_TOKEN;
}

"-"?[[:digit:]]+"."[[:digit:]]* {
  try {
    yylval->floatingPoint = std::stod(yytext);
  } catch (...) {
    return ERROR_TOKEN;
  }
  return FLOATING_POINT_TOKEN;
}

{ID} {
  yylval->cstring = yytext;
  return IDENTIFIER_TOKEN;
}

[\r\n]+  { return NEWLINE_TOKEN; }

[ \t]+  { /* ignore whitespace */ }

%%
