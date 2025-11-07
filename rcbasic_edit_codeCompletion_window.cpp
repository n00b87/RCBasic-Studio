#include <wx/wx.h>
#include <wx/simplebook.h>
#include "rcbasic_edit_codeCompletion_window.h"

rcbasic_edit_codeCompletion_window::rcbasic_edit_codeCompletion_window( wxWindow* parent, rc_codeComp_symbolDatabase* s_db, rc_codeComp_symbolDatabase* u_db, rc_udt_db* udt_db, bool udt_flag, int udt_index )
:
rc_codeCompletion_window( parent )
{
    show_comp_flag = false;
    resize_d = false;
    dbl_click = false;
    symbol_db = s_db;
    user_db = u_db;
    utype_db = udt_db;
    use_udt_db = udt_flag;
    utype_index = udt_index;
}

void rcbasic_edit_codeCompletion_window::OnSetFocus( wxFocusEvent& event )
{
    event.StopPropagation();
    event.Skip(false);
// TODO: Implement OnSetFocus
}

void rcbasic_edit_codeCompletion_window::OnSetFocus_ListBox( wxFocusEvent& event )
{
    event.StopPropagation();
    event.Skip(false);
}

void rcbasic_edit_codeCompletion_window::OnSetFocus_Doc( wxFocusEvent& event )
{
    event.StopPropagation();
    event.Skip(false);
}

void rcbasic_edit_codeCompletion_window::OnWindowResize( wxSizeEvent& event )
{
// TODO: Implement OnWindowResize
}

void rcbasic_edit_codeCompletion_window::setCompList(wxArrayString cc_list)
{
    m_symbol_listBox->Clear();

    for(int i = 0; i < cc_list.size(); i++)
    {
        m_symbol_listBox->AppendAndEnsureVisible(cc_list[i]);
    }

    if(cc_list.size() > 0)
    {
        m_symbol_listBox->SetSelection(0);
        m_symbol_listBox->EnsureVisible(0);
    }
}

void rcbasic_edit_codeCompletion_window::setCompShow(bool flag)
{
    show_comp_flag = flag;
    //wxMessageBox(_("SHOW SET"));
}

void rcbasic_edit_codeCompletion_window::forceResize()
{
    wxSize t_size = this->GetClientSize();
    if(resize_d)
        this->SetClientSize(t_size + wxSize(5, 5));
    else
        this->SetClientSize(t_size + wxSize(-5, -5));

    resize_d = !resize_d;

    bSizer135->Layout();

    this->Refresh();
    this->Update();
}

wxListBox* rcbasic_edit_codeCompletion_window::getListBox()
{
    return m_symbol_listBox;
}

wxHtmlWindow* rcbasic_edit_codeCompletion_window::getHtmlWindow()
{
    return m_doc_htmlWin;
}

void rcbasic_edit_codeCompletion_window::OnLeftDClick_ListBox( wxMouseEvent& event )
{
    dbl_click = true;
}

bool rcbasic_edit_codeCompletion_window::pointInPanel(wxPoint check_point)
{
    if(this->GetClientRect().Contains(check_point))
        return true;

    //wxMessageBox(_("RETIRN FALSE"));
    return false;
}

void rcbasic_edit_codeCompletion_window::updateDoc(bool is_udt, int udt_index)
{
    wxString src = _("");

    int symbol_index = -1;
    int user_index = -1;
    int field_index = -1;
    wxString current_symbol = _("");

    utype_index = udt_index;
    use_udt_db = is_udt;

    if(m_symbol_listBox->GetSelection() >= 0 & m_symbol_listBox->GetSelection() < m_symbol_listBox->GetCount())
        current_symbol = m_symbol_listBox->GetString(m_symbol_listBox->GetSelection()).Lower().Trim();

    if(is_udt)
    {
        if(udt_index >= 0 && udt_index < utype_db->udt.size())
        {
            for(int i = 0; i < utype_db->udt[udt_index].field.size(); i++)
            {
                if(utype_db->udt[udt_index].field[i].name.Lower().Trim().compare(_(current_symbol))==0)
                {
                    field_index = i;
                    break;
                }
            }
        }
    }
    else if(current_symbol.compare(_(""))!=0 && symbol_db)
    {
        for(int i = 0; i < symbol_db->symbol.size(); i++)
        {
            if(symbol_db->symbol[i].symbol_name.compare(_(current_symbol))==0)
            {
                symbol_index = i;
                src = symbol_db->symbol[i].doc_html;
                break;
            }
        }
    }

    if(current_symbol.compare(_(""))!=0 && user_db && symbol_index < 0 && (!is_udt))
    {
        for(int i = 0; i < user_db->symbol.size(); i++)
        {
            if(user_db->symbol[i].symbol_name.Lower().Trim().compare(_(current_symbol))==0)
            {
                user_index = i;
                src = user_db->symbol[i].doc_html;
                break;
            }
        }
    }

    if(user_index >= 0 || field_index >= 0)
    {
        src += _("<html>\n");
        src += _("<head>\n");
        src += _("<link rel=\"stylesheet\" href=\"style.css\">\n");
        src += _("<meta  content=\"text/html; charset=UTF-8\"  http-equiv=\"content-type\">");
        src += _("<title>[RCBasic Doc] </title>\n");
        src += _("</head>\n");
        src += _("<body>\n");
        src += _("<p><b>FN_DECLARE FN_NAME(_ARGS_)</b></p>\n");
        src += _("<p>\n");
        src += _("FN_DESCRIPTION\n");
        src += _("</p>\n");
        src += _("</body>\n");
        src += _("</html>");

        wxString prefix = _("");
        wxString id_name = ( field_index >= 0 ? utype_db->udt[udt_index].field[field_index].display_name : user_db->symbol[user_index].symbol_name );
        wxString id_args = _("");
        wxString comments = ( field_index >= 0 ? utype_db->udt[udt_index].field[field_index].comment : user_db->symbol[user_index].comments );
        comments.Replace(_("\n"), _("<br>"));

        //std::cout << "ID: " << id_name.ToStdString() << std::endl;

        if(user_index >= 0)
        {
            if(user_db->symbol[user_index].symbol_type == CC_SYMBOL_TYPE_VAR)
            {
                for(int i = 0; i < user_db->symbol[user_index].dimensions; i++)
                {
                    if(i > 0)
                        id_args += _(", ");

                    id_args += _("N");
                }
            }
            else
            {
                for(int i = 0; i < user_db->symbol[user_index].args.size(); i++)
                {
                    if(i > 0)
                        id_args += _(", ");

                    id_args += user_db->symbol[user_index].args[i];
                }
            }
        }
        else
        {
            for(int i = 0; i < utype_db->udt[udt_index].field[field_index].num_dimensions; i++)
            {
                if(i > 0)
                    id_args += _(", ");

                id_args += _("N");
            }
        }

        int symbol_type = ( field_index >= 0 ? CC_SYMBOL_TYPE_VAR : user_db->symbol[user_index].symbol_type);

        switch(symbol_type)
        {
            case CC_SYMBOL_TYPE_FN:
                prefix = _("Function");
                id_args = _("( ") + id_args + _(")");
                break;

            case CC_SYMBOL_TYPE_SUB:
                prefix = _("Sub");
                id_args = _("( ") + id_args + _(")");
                break;

            case CC_SYMBOL_TYPE_VAR:
                prefix = _("");
                if(id_args.length() > 0)
                    id_args = _("[ ") + id_args + _(" ]");
                break;

            case CC_SYMBOL_TYPE_CONSTANT:
                prefix = _("Cont");
                break;
        }


        src.Replace(_("FN_DECLARE"), prefix);
        src.Replace(_("FN_NAME"), id_name);
        src.Replace(_("(_ARGS_)"), id_args);
        src.Replace(_("FN_DESCRIPTION"), comments);

    }

    wxString ds_open = _("<style>");
    wxString ds_close = _("</style>");

    wxString doc_style = _("p { font-size: 10px; }");

    src.Replace(_("<style></style>"), ds_open + doc_style + ds_close);

    m_doc_htmlWin->SetPage(src);
    m_doc_htmlWin->SetHTMLBackgroundColour(wxColour(200, 200, 200));

    //m_symbol_listBox->SetForegroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_GRAYTEXT ) );
	//m_symbol_listBox->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_HIGHLIGHT ) );

    #ifdef __WIN32__
    this->Refresh();
    #else
    bSizer135->Layout();

    //2, 5
    int list_width = m_symbol_listBox->GetClientSize().GetWidth();
    int doc_width = m_doc_htmlWin->GetClientSize().GetWidth();

    this->SetClientSize(wxSize(list_width*3, this->GetClientSize().GetHeight()));
    m_doc_htmlWin->SetClientSize(wxSize(list_width*2, m_doc_htmlWin->GetClientSize().GetHeight()));
    m_doc_htmlWin->Layout();
    #endif // __WIN32__

}

void rcbasic_edit_codeCompletion_window::OnListBoxSelection( wxCommandEvent& event )
{
    updateDoc(use_udt_db, utype_index);
}


void rcbasic_edit_codeCompletion_window::setDBLClick(bool flag)
{
    dbl_click = flag;
}

bool rcbasic_edit_codeCompletion_window::getDBLClick()
{
    return dbl_click;
}


