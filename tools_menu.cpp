#include "rcbasic_edit_frame.h"
#include "rcbasic_editrc_distribute_dialog.h"
#include "rcbasic_editrc_genKey_dialog.h"

void rcbasic_edit_frame::onDistributeMenuSelect( wxCommandEvent& event )
{
    if(!active_project)
        return;

    rcbasic_editrc_distribute_dialog r(this);
    r.ShowModal();

    //---ACTIVATE CONTEXT PROJECT
    if(active_project)
    {
        wxSetWorkingDirectory(active_project->getLocation());
    }

    //---------

}

void rcbasic_edit_frame::onGenKeystoreMenuSelect( wxCommandEvent& event )
{
    rcbasic_editrc_genKey_dialog gk_dialog(this);
    gk_dialog.ShowModal();
}
