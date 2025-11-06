#ifndef EVAL_H_INCLUDED
#define EVAL_H_INCLUDED

#include <iostream>
#include "tokenizer.h"
//#include <wx/wx.h>
using namespace std;

#define TOKEN_TYPE_VARIABLE 0
#define TOKEN_TYPE_FUNCTION 1
#define TOKEN_TYPE_UDT      2
#define TOKEN_TYPE_CONSTANT 3

#define RC_SYMBOL_TYPE_VAR  0
#define RC_SYMBOL_TYPE_FN   1
#define RC_SYMBOL_TYPE_SUB  2
#define RC_SYMBOL_TYPE_UDT  3
#define RC_SYMBOL_TYPE_CONSTANT  4

struct rcbasic_id_token
{
    string name;
    int dimensions;
    int token_type;
    string d1;
    string d2;
    string d3;
    bool is_in_list;
    string user_type;
    vector<string> args;
    string header;
    string comment;
    int symbol_type;
    bool is_sub_token;
    int start_line;
    int end_line;
};

vector<rcbasic_id_token> id_tokens;

void add_id_token(rcbasic_id_token t)
{
    t.is_in_list = true;
    t.is_sub_token = false;
    for(int i = 0; i < id_tokens.size(); i++)
        if(id_tokens[i].name.compare(t.name)==0)
            return;
    id_tokens.push_back(t);
}

void add_id_token_other(rcbasic_id_token t)
{
    t.is_in_list = false;
    t.is_sub_token = true;
    for(int i = 0; i < id_tokens.size(); i++)
        if(id_tokens[i].name.compare(t.name)==0)
            return;
    id_tokens.push_back(t);
}

bool rc_eval(string line, bool* isInFunction, bool* isInType, std::string* doc_string)
{
    //adding a few extra characters to line to avoid a memory leak
    line += "             ";

    bool fn_define = *isInFunction;
    bool udt_define = *isInType;

    clearTokens();
    id_tokens.clear();

    *doc_string = "";

    if(!tokens(line, doc_string))
    {
        return false;
    }

    if(tmp_token.size() == 0)
        return false;


    int i = 0;

    int dim_scope = 0;
    bool is_dim_expr = false;
    bool is_dim_expr_other = false;
    string dim_token = "";

    bool dim_define = false;

    rcbasic_id_token dim_id;
    rcbasic_id_token fn_arg_id;

    fn_arg_id.symbol_type = RC_SYMBOL_TYPE_VAR;
    fn_arg_id.dimensions = 0;
    fn_arg_id.user_type = "";
    fn_arg_id.is_sub_token = true;

    vector<rcbasic_id_token> fn_args;

    dim_id.name = "";
    dim_id.dimensions = 0;
    dim_id.user_type = "";
    dim_id.symbol_type = -1;

    tmp_token.push_back("<:>");

    int expr_token_index = 0;

    bool udt_expr = false;
    bool fn_expr = false;
    bool const_expr = false;
    bool for_expr = false;

    rcbasic_id_token fn_id;


    for(i = 0; i < tmp_token.size(); i++)
    {
        dim_id.start_line = -1;
        dim_id.end_line = -1;

        if(expr_token_index == 0)
        {
            expr_token_index++;

            if(tmp_token.size() > (i + 1))
            {
                if(tmp_token[i].substr(0, 4).compare("<id>")==0 && tmp_token[i+1].compare("<equal>")==0 && (!fn_define))
                {
                    dim_id.name = tmp_token[i].substr(4);
                    dim_id.dimensions = 0;
                    dim_id.token_type = TOKEN_TYPE_VARIABLE;
                    add_id_token(dim_id);
                    dim_id.symbol_type = RC_SYMBOL_TYPE_VAR;

                    dim_id.name = "";
                    dim_id.dimensions = 0;
                    dim_id.user_type = "";
                }
                else if(tmp_token[i].substr(0, 4).compare("<id>")==0 && tmp_token[i+1].compare("<equal>")==0)
                {
                    dim_id.name = tmp_token[i].substr(4);
                    dim_id.dimensions = 0;
                    dim_id.token_type = TOKEN_TYPE_VARIABLE;
                    add_id_token_other(dim_id);
                    dim_id.symbol_type = RC_SYMBOL_TYPE_VAR;

                    dim_id.name = "";
                    dim_id.dimensions = 0;
                    dim_id.user_type = "";
                }
                else if( (tmp_token[i].compare("<function>")==0 || tmp_token[i].compare("<subp>")==0) && tmp_token[i+1].substr(0,4).compare("<id>")==0 )
                {
                    fn_define = true;
                    fn_expr = true;

                    dim_id.token_type = TOKEN_TYPE_FUNCTION;
                    dim_id.dimensions = 0;

                    if(tmp_token[i].compare("<function>")==0)
                        dim_id.symbol_type = RC_SYMBOL_TYPE_FN;
                    else
                        dim_id.symbol_type = RC_SYMBOL_TYPE_SUB;

                    dim_define = false;
                    is_dim_expr = false;
                    dim_scope = 0;
                    dim_id.name = tmp_token[i+1].substr(4);
                    //wxPuts(_("found function: ") + dim_id.name);
                }
                else if( tmp_token[i].compare("<for>")==0 && tmp_token[i+1].substr(0,4).compare("<id>")==0 )
                {
                    for_expr = true;

                    dim_id.token_type = TOKEN_TYPE_VARIABLE;
                    dim_id.dimensions = 0;

                    dim_id.symbol_type = RC_SYMBOL_TYPE_VAR;

                    dim_define = false;

                    is_dim_expr = false;
                    is_dim_expr_other = false;

                    if(fn_define)
                        is_dim_expr_other = true;
                    else
                        is_dim_expr = true;

                    dim_scope = 0;
                    dim_id.name = tmp_token[i+1].substr(4);
                    //wxPuts(_("found function: ") + dim_id.name);
                }
                else if( tmp_token[i].compare("<type>")==0 && tmp_token[i+1].substr(0,4).compare("<id>")==0 )
                {
                    udt_define = true;
                    udt_expr = true;

                    dim_id.token_type = TOKEN_TYPE_UDT;
                    dim_id.dimensions = 0;
                    dim_id.symbol_type = RC_SYMBOL_TYPE_UDT;

                    dim_define = false;
                    is_dim_expr = false;
                    dim_scope = 0;
                    dim_id.name = tmp_token[i+1].substr(4);
                    //wxPuts(_("found type: ") + dim_id.name);
                }
                else if( tmp_token[i].compare("<const>")==0 && tmp_token[i+1].substr(0,4).compare("<id>")==0 )
                {
                    const_expr = true;

                    dim_id.token_type = TOKEN_TYPE_CONSTANT;
                    dim_id.dimensions = 0;
                    dim_id.symbol_type = RC_SYMBOL_TYPE_CONSTANT;

                    dim_define = false;
                    is_dim_expr = false;
                    dim_scope = 0;
                    dim_id.name = tmp_token[i+1].substr(4);
                    dim_id.user_type = "";

                    for(int ci=i+2; ci < tmp_token.size(); ci++)
                        tmp_token[ci] = "";
                        tmp_token.push_back("<:>");
                    //wxPuts(_("found const: ") + dim_id.name);
                }
                else if(tmp_token[i].compare("<end>")==0 && (tmp_token[i+1].compare("<function>")==0 || tmp_token[i+1].compare("<subp>")==0))
                {
                    //wxPuts(_("END FUNC"));
                    fn_define = false;
                    i++;
                    continue;
                }
                else if(tmp_token[i].compare("<end>")==0 && tmp_token[i+1].compare("<type>")==0)
                {
                    //wxPuts(_("END FUNC"));
                    udt_define = false;
                    i++;
                    continue;
                }
            }
        }

        if(tmp_token[i].compare("<dim>")==0 && (!(fn_define || udt_define)))
        {
            // TODO: Need to add support for AS <TYPE>
            dim_token = tmp_token[i];
            dim_scope = 0;
            is_dim_expr = true;
            dim_define = true;
            dim_id.token_type = TOKEN_TYPE_VARIABLE;
            dim_id.symbol_type = RC_SYMBOL_TYPE_VAR;
            continue;
        }
        else if(tmp_token[i].compare("<dim>")==0)
        {
            dim_token = tmp_token[i];
            dim_scope = 0;
            is_dim_expr_other = true;
            dim_define = true;
            dim_id.token_type = TOKEN_TYPE_VARIABLE;
            dim_id.symbol_type = RC_SYMBOL_TYPE_VAR;
            continue;
        }
        else if(tmp_token[i].compare("<redim>")==0)
        {
            dim_token = tmp_token[i];
            dim_id.symbol_type = RC_SYMBOL_TYPE_VAR;
            dim_scope = 0;
            is_dim_expr = true;
            dim_define = false;
            continue;
        }
        else if(tmp_token[i].compare("<as>")==0)
        {
            if(fn_expr && dim_scope == 1 && ((i+1) < tmp_token.size()))
            {
                if(tmp_token[i+1].substr(0,4).compare("<id>")==0)
                    fn_arg_id.user_type = tmp_token[i+1].substr(4);

                tmp_token[i+1] = "";
            }
            else if((i+1) < tmp_token.size())
            {
                if(tmp_token[i+1].substr(0,4).compare("<id>")==0)
                    dim_id.user_type = tmp_token[i+1].substr(4);

                tmp_token[i+1] = "";
            }
        }
        else if(tmp_token[i].substr(0,4).compare("<id>")==0)
        {
            if(fn_expr && dim_scope == 1)
            {
                string fn_arg = "";
                if(i > 0)
                    if(tmp_token[i-1].compare("<byref>")==0)
                        fn_arg = "ByRef ";

                fn_arg += tmp_token[i].substr(4);
                fn_arg_id.name = fn_arg;

                dim_id.args.push_back(fn_arg);
            }
        }


        if(tmp_token[i].compare("<square>")==0)
        {
            dim_scope++;

            if(dim_scope==1 && dim_id.token_type==TOKEN_TYPE_VARIABLE)
                dim_id.dimensions = 1;
        }

        if(tmp_token[i].compare("<par>")==0)
        {
            dim_scope++;

            if(dim_scope==1 && dim_id.token_type==TOKEN_TYPE_FUNCTION)
                dim_id.dimensions = 1;
        }


        if(tmp_token[i].compare("</par>")==0 || tmp_token[i].compare("</square>")==0)
            dim_scope--;

        if(tmp_token[i].compare("<comma>")==0 && dim_scope==1)
            dim_id.dimensions++;

        if(is_dim_expr==true && dim_scope==0)
        {
            if(tmp_token[i].compare("<comma>")==0 || tmp_token[i].compare("<:>")==0)
            {
                if((dim_define || for_expr) && dim_id.name.compare("")!=0)
                    add_id_token(dim_id);

                dim_id.name = "";
                dim_id.dimensions = 0;
                dim_id.symbol_type = -1;
                dim_id.args.clear();

                tmp_token[i] = "<:>";
                tmp_token.insert(tmp_token.begin()+ (i+1), dim_token);

                for_expr = false;
            }
            else if(tmp_token[i].substr(0,4).compare("<id>")==0)
            {
                dim_id.name = tmp_token[i].substr(4);
            }
        }
        else if(is_dim_expr_other==true && dim_scope==0)
        {
            if(tmp_token[i].compare("<comma>")==0 || tmp_token[i].compare("<:>")==0)
            {
                if((dim_define || for_expr) && dim_id.name.compare("")!=0)
                    add_id_token_other(dim_id);

                dim_id.name = "";
                dim_id.dimensions = 0;
                dim_id.symbol_type = -1;
                dim_id.args.clear();

                tmp_token[i] = "<:>";
                tmp_token.insert(tmp_token.begin() + (i+1), dim_token);

                for_expr = false;
            }
            else if(tmp_token[i].substr(0,4).compare("<id>")==0)
            {
                dim_id.name = tmp_token[i].substr(4);
            }
        }
        else if(fn_expr==true && (dim_scope==1 || tmp_token[i].compare("</par>")==0))
        {
            if(tmp_token[i].compare("<comma>")==0 || tmp_token[i].compare("</par>")==0)
            {
                if(fn_arg_id.name.length() > 0)
                    fn_args.push_back(fn_arg_id); // I am not passing fn_arg_id to rcbasic_add_token_other() yet since the function still hasn't been added

                fn_arg_id.name = "";
                fn_arg_id.user_type = "";
            }
            else if(tmp_token[i].substr(0,4).compare("<id>")==0)
            {
                fn_arg_id.name = tmp_token[i].substr(4);
            }
        }


        if(tmp_token[i].compare("<:>")==0)
        {
            expr_token_index = 0;

            if((!(fn_define||udt_define)) && (dim_define || for_expr) && dim_id.name.compare("")!=0)
                add_id_token(dim_id);
            else if(fn_expr && dim_id.name.compare("")!=0)
            {
                add_id_token(dim_id);

                for(int fn_arg_num = 0; fn_arg_num < fn_args.size(); fn_arg_num++)
                {
                    add_id_token_other(fn_args[fn_arg_num]);
                }

                fn_args.clear();
                fn_arg_id.name = "";
                fn_arg_id.user_type = "";
            }
            else if(udt_expr && dim_id.name.compare("")!=0)
                add_id_token(dim_id);
            else if(const_expr && dim_id.name.compare("")!=0)
                add_id_token(dim_id);

            dim_id.name = "";
            dim_id.dimensions = 0;
            dim_id.symbol_type = -1;
            dim_id.args.clear();

            dim_define = false;
            is_dim_expr = false;
            is_dim_expr_other = false;
            fn_expr = false;
            udt_expr = false;
            const_expr = false;
            for_expr = false;
        }
    }

    isInFunction[0] = fn_define;
    isInType[0] = udt_define;

    return true;
}

#endif // EVAL_H_INCLUDED
