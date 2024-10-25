#ifndef __rcbasic_editrc_gotoLine_dialog__
#define __rcbasic_editrc_gotoLine_dialog__

/**
@file
Subclass of rc_gotoLine_dialog, which is generated by wxFormBuilder.
*/

#include "rcbasic_edit.h"
#include "rcbasic_edit_frame.h"

//// end generated include

/** Implementing rc_gotoLine_dialog */
class rcbasic_edit_gotoLine_dialog : public rc_gotoLine_dialog
{
	public:
		/** Constructor */
		rcbasic_edit_gotoLine_dialog( wxWindow* parent );
	//// end generated class members

	private:
        rcbasic_edit_frame* parent_frame;
        rcbasic_edit_txtCtrl* current_file;

    protected:
        void onCancelButtonClick( wxCommandEvent& event );
		void onOKButtonClick( wxCommandEvent& event );

};

#endif // __rcbasic_editrc_gotoLine_dialog__