#ifndef RCBASIC_SYMBOL_H_INCLUDED
#define RCBASIC_SYMBOL_H_INCLUDED

#include <wx/wx.h>
#include <wx/treectrl.h>
#include <vector>
#include "rcbasic_symbol.h"

#define CC_SYMBOL_TYPE_VAR  0
#define CC_SYMBOL_TYPE_FN   1
#define CC_SYMBOL_TYPE_SUB  2
#define CC_SYMBOL_TYPE_UDT  3
#define CC_SYMBOL_TYPE_CONSTANT  4

struct rcbasic_symbol
{
    wxString id;
    int line;
    int dimensions;
    int token_type;
    bool in_list;
    wxString upper_id; //storing id uppercase to speed up sort
    wxString user_type;
    wxString display_name;
    int symbol_type;
    wxString var_udt;
    wxString comments;
    wxArrayString args;
    int start_line;
    int end_line;
    std::vector<rcbasic_symbol> sub_sym;
};

class rc_symbol_treeItem_data : public wxTreeItemData
{
    public:
        rcbasic_symbol symbol;
        rc_symbol_treeItem_data(rcbasic_symbol sym)
        {
            symbol = sym;
        }
};

struct rc_codeComp_symbolDoc
{
    int symbol_type;
    wxString display_name;
    wxString symbol_name;
    wxString user_type;
    wxString doc_html;
    wxString comments;
    wxArrayString args;
    int start_line;
    int end_line;
    int dimensions;
};

struct rc_codeComp_symbolDatabase
{
    std::vector<rc_codeComp_symbolDoc> symbol;
};

struct rc_udt_field
{
    wxString name;
    wxString display_name;
    wxString type_name;
    wxString comment;
    int num_dimensions;
};

struct rc_udt
{
    wxString type_name;
    std::vector<rc_udt_field> field;
};

struct rc_udt_db
{
    std::vector<rc_udt> udt;
};

#endif // RCBASIC_SYMBOL_H_INCLUDED
