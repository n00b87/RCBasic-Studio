#include "parserThread.h"
#include "rcbasic_symbol.h"
#include "eval.h"
#include "rcbasic_edit_frame.h"
#include <wx/stopwatch.h>

DEFINE_EVENT_TYPE(wxEVT_MYTHREAD)

wxSemaphore* page_sem;

bool page_lock()
{
    page_sem->Wait();
}

bool page_unlock()
{
    page_sem->Post();
}


parserThread::parserThread(wxEvtHandler* pParent, int param, wxFrame* p_frame) : wxThread(wxTHREAD_DETACHED), m_pParent(pParent)
{
    //pass parameters into the thread
    m_param = param;
    //file_text = txt + _("\n");
    parent_frame = p_frame;

    rcbasic_edit_frame* frame = (rcbasic_edit_frame*) parent_frame;
    sym_list = new std::vector<rcbasic_symbol>;
    dbg_init = true;
    //s_list = frame->getSymbols();
}


void parserThread::readContents()
{
    contents.clear();
    rcbasic_edit_frame* frame = (rcbasic_edit_frame*) parent_frame;
    notebook_mutex.Lock();
    if(frame->getCurrentFile())
    {
        if(frame->getCurrentFile()->getTextCtrl())
        {
            file_text = frame->getCurrentFile()->getTextCtrl()->GetText() + _(" \n");
        }
        else
        {
            notebook_mutex.Unlock();
            return;
        }
    }
    else
    {
        notebook_mutex.Unlock();
        return;
    }

    notebook_mutex.Unlock();

    wxString current_line = _("");
    wxString current_char;

    for(int i = 0; i < file_text.length(); i++)
    {
        current_char = file_text.substr(i,1);
        if(current_char.compare(_("\n"))==0)
        {
            contents.push_back(current_line);
            current_line = _("");
        }
        else
        {
            current_line += current_char;
        }
    }
}


bool parserThread::inSymbolList(rcbasic_symbol sym)
{

    for(int i = 0; i < s_list.size(); i++)
    {

        if(sym.id.compare(s_list[i].id)==0 && sym.line==s_list[i].line && sym.dimensions==s_list[i].dimensions)
        {
            return true;
        }
    }

    return false;
}

int parserThread::addSymbol(rcbasic_symbol sym)
{
    int insert_pos = 0;
    wxString sym_id = sym.id.Upper();
    for(int i = 0; i < sym_list->size(); i++)
    {
        rcbasic_symbol list_item = sym_list[0][i];
        int id_sym_cmp = list_item.upper_id.compare(sym_id);
        if(id_sym_cmp==0 && list_item.token_type == sym.token_type)
        {
            //delete sym;
            return i;
        }

        if(id_sym_cmp < 0)
            insert_pos++;
        else
            break;

    }
    //sym_list->push_back(sym);
    sym_list->insert(sym_list->begin()+insert_pos, sym);

    return insert_pos;
}

void* parserThread::Entry()
{
    wxCommandEvent evt(wxEVT_MYTHREAD, GetId());

    rcbasic_edit_frame* frame = (rcbasic_edit_frame*) parent_frame;

    bool current_file_flag = false;

    while(true)
    {
        if(TestDestroy())
        {
            break;
        }

        notebook_mutex.Lock();
        current_file_flag = (frame->getCurrentFile()!=NULL);

        if(current_file_flag)
        {
            current_file_flag = (frame->getCurrentFile()->getTextCtrl() && !frame->symbolUpdateInProgress);

            if(current_file_flag)
            {
                frame->symbolUpdateInProgress = true;
                frame->pre_parsed_page = frame->getCurrentFile()->getTextCtrl();
                notebook_mutex.Unlock();
                runParser(evt);
            }
            else
            {
                notebook_mutex.Unlock();
            }
        }
        else
        {
            frame->symbolUpdateInProgress = true;
            frame->pre_parsed_page = NULL;
            notebook_mutex.Unlock();
            //contents.clear();
            wxPostEvent(m_pParent, evt);
        }

        wxMilliSleep(400);
    }

    return NULL;
}

bool parserThread::runParser(wxCommandEvent evt)
{
    if(sym_list)
    {
        //delete sym_list;
        //sym_list = NULL;
        sym_list->clear();
    }

    rcbasic_edit_frame* frame = (rcbasic_edit_frame*) parent_frame;

    s_list = frame->getSymbols();

    readContents();

    int contents_changed = 0;

    bool fn_define = false;
    bool udt_define = false;

    sw_timer.Start(0);

    int parent_token_index = -1;
    std::string doc_string = "";


    for(int i = 0; i < contents.size(); i++)
    {
        if(TestDestroy())
            break;


        bool eval_return = rc_eval(std::string(contents[i].mb_str()), &fn_define, &udt_define, &doc_string);

        if(parent_token_index >= 0 && parent_token_index < sym_list->size())
        {
            if(sym_list[0][parent_token_index].symbol_type == CC_SYMBOL_TYPE_FN || sym_list[0][parent_token_index].symbol_type == CC_SYMBOL_TYPE_SUB)
            {
                if(!fn_define)
                {
                    sym_list[0][parent_token_index].end_line = i+1;
                }
                else if(doc_string.length() > 0 && (!eval_return))
                {
                    sym_list[0][parent_token_index].comments += wxString(doc_string) + _("\n");
                }
            }
            else if(sym_list[0][parent_token_index].symbol_type == CC_SYMBOL_TYPE_UDT)
            {
                if(doc_string.length() > 0 && (!eval_return))
                {
                    sym_list[0][parent_token_index].comments += wxString(doc_string) + _("\n");
                }
            }
        }

        //wxPuts(_("EVAL RAN"));
        for(int t_count = 0; t_count < id_tokens.size(); t_count++)
        {
            //wxPrintf( wxString(id_tokens[t_count].name.c_str(), wxConvUTF8) + _("[%d]:%d\n"), id_tokens[t_count].dimensions, i+1 );
            rcbasic_symbol sym;// = new rcbasic_symbol();
            sym.id = id_tokens[t_count].name;
            sym.display_name = sym.id;
            sym.upper_id = sym.id.Upper();
            sym.line = i;
            sym.user_type = id_tokens[t_count].user_type;
            sym.dimensions = id_tokens[t_count].dimensions;
            sym.token_type = id_tokens[t_count].token_type;
            sym.symbol_type = id_tokens[t_count].symbol_type;
            sym.in_list = id_tokens[t_count].is_in_list;
            sym.args.Clear();

            sym.comments = wxString(doc_string);

            sym.start_line = -1;
            sym.end_line = -1;

            for(int arg_num = 0; arg_num < id_tokens[t_count].args.size(); arg_num++)
                sym.args.push_back(wxString(id_tokens[t_count].args[arg_num]));

            sym.sub_sym.clear();

            if(!inSymbolList(sym))
                contents_changed = 1;

            notebook_mutex.Lock();

            if(id_tokens[t_count].is_sub_token)
            {
                if(parent_token_index >= 0 && parent_token_index < sym_list->size())
                {
                    //if(sym_list[0][parent_token_index].symbol_type == CC_SYMBOL_TYPE_UDT)
                      //  sym.comments = wxString(doc_string);

                    sym_list[0][parent_token_index].sub_sym.push_back(sym);
                }
            }
            else
            {
                if(sym.symbol_type == CC_SYMBOL_TYPE_FN || sym.symbol_type == CC_SYMBOL_TYPE_SUB)
                    sym.start_line = i+1;

                parent_token_index = addSymbol(sym);
            }

            notebook_mutex.Unlock();

            if(sw_timer.Time()>5)
            {
                wxMilliSleep(5);
                sw_timer.Start(0);
            }
        }

        //if(i%200==0)
        if(sw_timer.Time()>1)
        {
            wxMilliSleep(5);
            sw_timer.Start(0);
        }

    }

    notebook_mutex.Lock();
    if(sym_list->size() != s_list.size())
        contents_changed = 1;

    //can be used to set some identifier for the data
    evt.SetInt(contents_changed);

    frame->parsed_page = frame->pre_parsed_page;
    evt.SetClientData((void*)sym_list);

    wxPostEvent(m_pParent, evt);

    notebook_mutex.Unlock();


    return true;
}

void parserThread::OnExit()
{
    rcbasic_edit_frame* frame = (rcbasic_edit_frame*) parent_frame;
    if(frame->getSymSem())
        frame->getSymSem()->Post();
}
