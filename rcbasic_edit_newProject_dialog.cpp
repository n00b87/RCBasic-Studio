#include "rcbasic_edit_newProject_dialog.h"
#include "projects.h"

rcbasic_edit_newProject_dialog::rcbasic_edit_newProject_dialog( wxWindow* parent )
:
rc_newProject_dialog( parent )
{
    newProjectValue = false;

    projectNewMain_field->SetValue(_("main.bas"));
}

void rcbasic_edit_newProject_dialog::cancelNewProject( wxCommandEvent& event )
{
// TODO: Implement cancelNewProject
    Close();
}

void rcbasic_edit_newProject_dialog::createNewProject( wxCommandEvent& event )
{
    int main_source_flag = projectCreateMain_radio->GetValue() ? 0 : 1;

    switch(main_source_flag)
    {
        case 0:
        {
            if(projectNewMain_field->GetValue().length() < 5)
            {
                wxMessageBox(_("Invalid name for main source file"));
                return;
            }

            if(projectNewMain_field->GetValue().substr(projectNewMain_field->GetValue().length()-4).compare(_(".bas"))!=0)
            {
                wxMessageBox(_("Main source file must have *.bas extension"));
                return;
            }
        }
        break;

        case 1:
        {
            bool invalid = false;
            if(projectExistingFile_picker->GetFileName().GetExt().compare(_("bas"))!=0)
                invalid = true;

            if(invalid)
            {
                wxMessageBox(_("Must select a valid *.bas file to copy"));
                return;
            }
        }
    }

    newProjectValue = true;
    Close();
// TODO: Implement createNewProject
    /*wxString project_name = projectName_field->GetValue();
    wxString project_location = projectLocation_picker->GetPath();
    int main_source_flag = projectCreateMain_radio->GetValue() ? 0 : 1;
    wxString main_source_value = main_source_flag==0 ? projectNewMain_field->GetValue() : projectExistingFile_picker->GetTextCtrlValue();
    wxString project_author = projectAuthor_field->GetValue();
    wxString project_website = projectWebsite_field->GetValue();
    wxString project_description = projectDescription_field->GetValue();
    */

    //rcbasic_project new_project(project_name, project_location, main_source_flag, main_source_value, project_author, project_website, project_description);

    //wxString str, str2;
    //str.Printf(wxT("Sash Position: %d"), sashPOS);
    //str2.Printf(wxT("Sash Size: %d\n"), sashSize);

    //wxPuts(_("Project_Location: ") + project_location);
    //wxPuts(_("Main Source: ") + main_source_value);

    //open_projects.push_back(new_project);
}

void rcbasic_edit_newProject_dialog::createMainSourceRadioSelected( wxCommandEvent& event )
{
    projectNewMain_field->Enable( true );
    projectExistingFile_picker->Enable( false );
}

void rcbasic_edit_newProject_dialog::copyExistingSourceRadioSelected( wxCommandEvent& event )
{
    projectNewMain_field->Enable( false );
    projectExistingFile_picker->Enable( true );
}

bool rcbasic_edit_newProject_dialog::getNewProjectValue()
{
    return newProjectValue;
}
