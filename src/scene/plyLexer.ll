%{
#include <cstdio>
#include <fstream>
#include <sstream>

#include "plyParser.hpp"
%}

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

"end_header"  { return END_HEADER_TOKEN; }

"-"?[[:digit:]]+  { return INTEGER_TOKEN; }
"-"?[[:digit:]]+"."[[:digit:]]*  { return FLOATING_POINT_TOKEN; }

{ID} { return IDENTIFIER_TOKEN; }

[\r\n]+  { return NEWLINE_TOKEN; }

[:space:]+  { /* ignore whitespace */ }

%%
