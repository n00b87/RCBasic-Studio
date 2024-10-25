#ifndef __rcbasic_editrc_newProject_dialog__
#define __rcbasic_editrc_newProject_dialog__

/**
@file
Subclass of rc_newProject_dialog, which is generated by wxFormBuilder.
*/

#include "rcbasic_edit.h"
#include "projects.h"

//// end generated include

/** Implementing rc_newProject_dialog */
class rcbasic_edit_newProject_dialog : public rc_newProject_dialog
{
	protected:
		// Handlers for rc_newProject_dialog events.
		bool newProjectValue;
		void cancelNewProject( wxCommandEvent& event );
		void createNewProject( wxCommandEvent& event );
		void createMainSourceRadioSelected( wxCommandEvent& event );
		void copyExistingSourceRadioSelected( wxCommandEvent& event );
	public:
		/** Constructor */
		rcbasic_edit_newProject_dialog( wxWindow* parent );
		bool getNewProjectValue();
	//// end generated class members

};

#endif // __rcbasic_editrc_newProject_dialog__
