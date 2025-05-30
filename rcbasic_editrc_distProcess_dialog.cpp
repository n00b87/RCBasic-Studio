#include "rcbasic_editrc_distProcess_dialog.h"
#include <wx/txtstrm.h>
#include <wx/msgdlg.h>
#include <wx/stdpaths.h>

rcbasic_editrc_distProcess_dialog::rcbasic_editrc_distProcess_dialog( wxWindow* parent, wxString dist_cmd, wxString dist_script_path, int num_targets )
:
rc_distProcess_dialog( parent )
{
    isRunning = false;
    dist_pid = -1;
    dist_process = NULL;

    wxFileName dist_script_fname(dist_script_path);
    #ifdef _WIN32
    //dist_script_fname.AppendDir(_("tools"));
    dist_script_fname.AppendDir(_("bin"));
    dist_script_fname.SetFullName(_("dist.bat"));
    #else
    dist_script_fname.AppendDir(_("bin"));
    dist_script_fname.SetFullName(_("rcbasic_dist.sh"));
    #endif // _WIN32

    wxString dist_exec_cmd = dist_script_fname.GetFullPath() +_(" ") + dist_cmd;

    //wxMessageBox(_("ARG --> ") + dist_exec_cmd);
    //return;

    dist_process = new wxProcess(this);
	if(!dist_process)
        Close();

    dist_process->Redirect();

	dist_process->Connect( wxEVT_END_PROCESS, wxProcessEventHandler( rcbasic_editrc_distProcess_dialog::onDistProcessTerminate ), NULL, this );

	wxString pkg_home;
	wxGetEnv(_("RCBASIC_TOOLS"), &pkg_home);
	//wxSetWorkingDirectory(pkg_home);

	wxString p;
	wxGetEnv(_("PATH"), &p);
	//wxPuts(_("PATH: ") + p );
	//wxPuts(_("\nCMD: ")+dist_cmd+_("\n\n"));


	target_count = num_targets;

	//wxMessageBox(_("Num Targets: ") + wxString::Format(_("%i"), target_count));

	m_status_gauge->SetRange(target_count);
	current_count = 0;

	dist_pid = wxExecute(dist_exec_cmd, wxEXEC_ASYNC, dist_process, NULL);

	if(dist_pid < 0)
    {
        if(dist_process)
            delete dist_process;
        dist_process = NULL;
        return;
    }

	isRunning = true;
}

void rcbasic_editrc_distProcess_dialog::onDistProcessUpdateUI( wxUpdateUIEvent& event )
{
// TODO: Implement onDistProcessUpdateUI
    if(!isRunning)
        return;

    wxTextInputStream dist_stream(*dist_process->GetInputStream());

    while(dist_process->IsInputAvailable())
    {
        wxString console_line = dist_stream.ReadLine();

        if(console_line.find(_("RCBASIC PACKAGE SUCCESS:")) != wxString::npos)
        {
            //wxPuts(_("\n\n####FOUND IT#####\n\n"));
            current_count++;
            m_status_gauge->SetValue(current_count);
            //wxPrintf(_("Current Value = %d out of %d\n"), m_status_gauge->GetValue(), m_status_gauge->GetRange());
        }

        m_consoleLog_textCtrl->AppendText(console_line + _("\n"));
    }

}

void rcbasic_editrc_distProcess_dialog::onCancelButtonClick( wxCommandEvent& event )
{
// TODO: Implement onCancelButtonClick
    if(!isRunning)
        Close();

    isRunning = false;
    dist_process->CloseOutput();

    wxExecute(_("taskkill /F /IM rcbasic_studio_run.exe"), wxEXEC_SYNC);

    wxKill(dist_pid);
    if(dist_process)
        delete dist_process;
    dist_process = NULL;

    Close();
}

void rcbasic_editrc_distProcess_dialog::onCloseButtonClick( wxCommandEvent& event )
{
// TODO: Implement onCloseButtonClick
    Close();
}


void rcbasic_editrc_distProcess_dialog::onDistProcessTerminate( wxProcessEvent& event )
{
    isRunning = false;
    dist_process->CloseOutput();

    wxTextInputStream dist_stream(*dist_process->GetInputStream());

    while(dist_process->IsInputAvailable())
    {
        wxString console_line = dist_stream.ReadLine();
        m_consoleLog_textCtrl->AppendText(console_line + _("\n"));
    }

    if(m_status_gauge->GetValue() < (m_status_gauge->GetRange()-1)) //Value starts at 0 so it should be range - 1 if successful
    {
        wxMessageBox(_("ERROR: Did not successfully build app for all selected platforms."));
    }
    else
    {
        m_status_gauge->SetValue(m_status_gauge->GetRange());
    }

    wxKill(dist_pid);
    if(dist_process)
        delete dist_process;
    dist_process = NULL;

    m_cancel_button->Hide();
    m_close_button->Enable();
}
