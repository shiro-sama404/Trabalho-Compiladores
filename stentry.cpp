#include "stentry.h"
#include <sstream> // Para std::ostringstream

STEntry::STEntry() : token(nullptr), category(CAT_UNKNOWN), type(SYM_UNKNOWN), reserved(false) {}

STEntry::STEntry(Token* tok) : token(tok), category(CAT_UNKNOWN), type(SYM_UNKNOWN), reserved(false) {}

STEntry::STEntry(Token* tok, SymbolCategory cat, SymbolType t, bool res)
    : token(tok), category(cat), type(t), reserved(res){}