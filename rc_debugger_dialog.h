#ifndef __rc_debugger_dialog__
#define __rc_debugger_dialog__

/**
@file
Subclass of rc_debugger, which is generated by wxFormBuilder.
*/

#include "rcbasic_edit.h"
#include <wx/process.h>
#include <vector>
#include <stdint.h>

//// end generated include

#define RC_DBG_VAR_TYPE_NUM 0
#define RC_DBG_VAR_TYPE_STR 1

#define RC_DBG_ACCESS_GET 0
#define RC_DBG_ACCESS_SET 1

#define RC_DBG_STATUS_GOOD 0
#define RC_DBG_STATUS_ERROR 1

struct dbg_var
{
    wxString name;
    wxString scope;
    int data_type;
    int access_type;
    int status;
    int dimensions;
    uint64_t dim[3];
    wxString var_value;
    uint64_t index;
};

/** Implementing rc_debugger */
class rc_debugger_dialog : public rc_debugger
{
	protected:
		// Handlers for rc_debugger events.
		void onPause( wxCommandEvent& event );
		void onStep( wxCommandEvent& event );
		void onRun( wxCommandEvent& event );
		void onSuper( wxCommandEvent& event );
		void onEnd( wxCommandEvent& event );
		void onClose( wxCommandEvent& event );
		void onDebugUpdate( wxUpdateUIEvent& event );

		void onDebugProcessTerminate( wxProcessEvent& event );
	public:
		/** Constructor */
		rc_debugger_dialog( wxWindow* parent, wxFileName dbg_cbc, wxFileName dbg_runtime  );

		void loadDebugData(wxString sym_file, wxString inc_file);
		void updateDebugData();
		void updateDebugUI();

		void term_process();

		wxProcess* rc_debug_process;
		long rc_debug_pid = -1;

		bool step = false;
		bool auto_run = false;
		bool super_run = false;
		bool ends_on_error = false;
		wxString error_msg;
		bool is_running = false;
		std::vector<wxString> src_files;
		std::vector<dbg_var> dbg_vars;
		std::vector<dbg_var> used_vars;
		uint64_t current_line = 0;
		wxString current_src_file = _("");

		wxColour msg_bkg_color[2];

		wxString cwd;

		wxFileName dbg_rt;
	//// end generated class members

};

#endif // __rc_debugger_dialog__
