#pragma warning(disable: 4996)
#include <ida.hpp>
#include <diskio.hpp>
#include <idp.hpp>
#include <loader.hpp>
#pragma warning(default: 4996)
#include "Logging.h"
#include "TcpServer.h"

extern "C" {
	char *infFilePath = NULL;
	
	int CreateDescriptor(const char *path) {
		char buffer[1000];
		qsnprintf(buffer, sizeof(buffer) -1, "Event: Init %s\n", database_idb);
		msg(buffer);
		
		size_t infFilePathLen = qstrlen(path) + 6;
		infFilePath = (char *) qalloc(infFilePathLen + 1);		
		qstrncpy(infFilePath, path, infFilePathLen + 1);		
		qstrncat(infFilePath, ".idaas", infFilePathLen + 1);
		qsnprintf(buffer, sizeof(buffer) -1, "Event: Write %s\n", infFilePath);
		msg(buffer);		
		FILE *infFile = fopenWT(infFilePath);		
		qfprintf(infFile, "%s", DEFAULT_PORT);
		qfclose(infFile);
		
		return 0;
	}

	int DestroyDescriptor() {
		char buffer[1000];
		qsnprintf(buffer, sizeof(buffer) -1, "Event: Close %s\n", database_idb);
		msg(buffer);

		if (infFilePath != NULL) {
			#pragma warning(disable: 4996)
			unlink(infFilePath);
			#pragma warning(default: 4996)
			qfree(infFilePath);
			infFilePath = NULL;
		}
		return 0;
	}

	int idaapi HOOK_UI_Callback(void *user_data, int notification_code, va_list va) {		
		switch (notification_code)
		{
		case ui_database_inited:
			CreateDescriptor(database_idb);		
			break;
		case ui_term:		
			DestroyDescriptor();
			break;
		default:
			break;
		}	
		return 0;
	}

	int idaapi IDAP_init(void)
	{
		// Do checks here to ensure your plug-in is being used within
		// an environment it was written for. Return PLUGIN_SKIP if the 	
		// checks fail, otherwise return PLUGIN_KEEP.
			
		CreateConnection();
		hook_to_notification_point(HT_UI, HOOK_UI_Callback, NULL);

		return PLUGIN_KEEP;
	}

	void idaapi IDAP_term(void)
	{
		unhook_from_notification_point(HT_UI, HOOK_UI_Callback);
		setLoggingEnabled(false);
		CloseConnection();

		// Stuff to do when exiting, generally you'd put any sort
		// of clean-up jobs here.
		return;
	}

	// The plugin can be passed an integer argument from the plugins.cfg
	// file. This can be useful when you want the one plug-in to do
	// something different depending on the hot-key pressed or menu
	// item selected.
	void idaapi IDAP_run(int arg)
	{
		// The "meat" of your plug-in
		msg("Hello world!");
		return;
	}

	// There isn't much use for these yet, but I set them anyway.
	char IDAP_comment[] 	= "This is my test plug-in";
	char IDAP_help[] 		= "My plugin";

	// The name of the plug-in displayed in the Edit->Plugins menu. It can 
	// be overridden in the user's plugins.cfg file.
	char IDAP_name[] 		= "My plugin";

	// The hot-key the user can use to run your plug-in.
	char IDAP_hotkey[] 	= "Alt-Y";

	// The all-important exported PLUGIN object
	plugin_t PLUGIN =
	{
	  IDP_INTERFACE_VERSION,	// IDA version plug-in is written for
	  PLUGIN_FIX | PLUGIN_MOD,					// Flags (see below)
	  IDAP_init,			// Initialisation function
	  IDAP_term,			// Clean-up function
	  IDAP_run,				// Main plug-in body
	  IDAP_comment,			// Comment – unused
	  IDAP_help,			// As above – unused
	  IDAP_name,			// Plug-in name shown in 
							// Edit->Plugins menu
	  IDAP_hotkey			// Hot key to run the plug-in
	};
}