#include "rcbasic_edit_codeHint_window.h"
#include "rcbasic_symbol.h"

rcbasic_edit_codeHint_window::rcbasic_edit_codeHint_window( wxWindow* parent )
:
rc_codeHint_window( parent )
{

}

void rcbasic_edit_codeHint_window::OnSetFocus( wxFocusEvent& event )
{
// TODO: Implement OnSetFocus
}

void rcbasic_edit_codeHint_window::OnWindowResize( wxSizeEvent& event )
{
// TODO: Implement OnWindowResize
}

void rcbasic_edit_codeHint_window::OnSetFocus_Doc( wxFocusEvent& event )
{
// TODO: Implement OnSetFocus_Doc
}

bool rcbasic_edit_codeHint_window::pointInPanel(wxPoint check_point)
{
    if(this->GetClientRect().Contains(check_point))
        return true;

    return false;
}

void rcbasic_edit_codeHint_window::updateDoc()
{
    wxString src = _("");
    src += _("<html>\n");
	src += _("<head>\n");
    src += _("<link rel=\"stylesheet\" href=\"style.css\">\n");
    src += _("<meta  content=\"text/html; charset=UTF-8\"  http-equiv=\"content-type\">");
    src += _("<title>[RCBasic Doc] </title>\n");
	src += _("</head>\n");
	src += _("<body>\n");
    src += _("<tt>_PREFIX_ _NAME_(_ARGS_)</tt>\n");
	src += _("</body>\n");
    src += _("</html>");

    src.Replace(_("_PREFIX_"), s_prefix);
    src.Replace(_("_NAME_"), s_name);

    wxString replace_args = _("");

    bool arg_num_highlighted = false;

    if(s_prefix.Lower().compare(_("sub"))==0 || s_prefix.Lower().compare(_("function"))==0)
    {
        replace_args = _("(");

        for(int i = 0; i < s_args.size(); i++)
        {
            // <font color="green">
            if(i == s_arg_num)
            {
                arg_num_highlighted = true;
                //std::cout << "wxColor: " << wxColour(190, 190, 190).GetAsString(wxC2S_HTML_SYNTAX).ToStdString() << std::endl;
                replace_args += _("<font color=\"blue\"><b>") + s_args[i] + _("</b></font>");
            }
            else
            {
                replace_args += s_args[i];
            }

            if((i+1) < s_args.size())
            {
                replace_args += _(", ");
            }
        }

        replace_args += _(")");
    }

    src.Replace(_("(_ARGS_)"), replace_args);

    m_doc_htmlWin->SetPage(src);
    m_doc_htmlWin->SetHTMLBackgroundColour(wxColour(200, 200, 200));

    wxString full_str = s_prefix + _(" ") + s_name + _(" ") + replace_args;

    // these are approximations
    float f_width = 9;
    int padding = 50;

    int str_len = full_str.length();

    if(arg_num_highlighted)
        str_len -= 26;

    float ln_width_f = f_width * ((float)str_len) + (float)padding;
    int ln_width = (int)ln_width_f;

    //std::cout << "TST: " << ln_width << ", " << str_len << std::endl;

    #ifndef __WIN32__
    if(this->GetClientSize().GetWidth() < ln_width)
    #endif // __WIN32__
    {
        m_doc_htmlWin->SetClientSize( ln_width, m_doc_htmlWin->GetClientSize().GetHeight());
        this->SetClientSize( ln_width, this->GetClientSize().GetHeight());

        //m_doc_htmlWin->Layout();

        this->Refresh();
        //this->Update();

    }
    //else
    //{
    //    this->Refresh();
    //}
}

wxHtmlWindow* rcbasic_edit_codeHint_window::getHtmlWindow()
{
    return m_doc_htmlWin;
}


void rcbasic_edit_codeHint_window::setCodeHint(int symbol_type, wxString symbol_name, wxArrayString symbol_args, int arg_num)
{
    switch(symbol_type)
    {
        case CC_SYMBOL_TYPE_VAR:
            s_prefix = _("");
            break;

        case CC_SYMBOL_TYPE_FN:
            s_prefix = _("FUNCTION");
            break;

        case CC_SYMBOL_TYPE_SUB:
            s_prefix = _("SUB");
            break;

        case CC_SYMBOL_TYPE_UDT:
            s_prefix = _("TYPE");
            break;
    }

    s_name = symbol_name;
    s_args = symbol_args;
    s_arg_num = arg_num;
}
