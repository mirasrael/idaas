#include "Database.h"
#include <atlstr.h>

using System::String;

namespace Ida {
	namespace Client {
		Database::Database(::Database *database) : m_database(database)
		{
			_enumerations = gcnew Ida::Client::Enumerations(m_database);			
			_functions = gcnew Ida::Client::Functions(m_database);
		}

		Database::~Database(void) {
			if (m_database != nullptr) {
				delete m_database;
			}
		}

		Database^ Database::Open(String^ path) {
			::Database *database = ::Database::Open(CString(path));
			if (database == nullptr) {
				throw gcnew System::ArgumentException(String::Format("Can't open database for {0}", path));
			}
			return gcnew Database(database);
		}

		void Database::Close() {
			m_database->Close();
		}
	}
}
