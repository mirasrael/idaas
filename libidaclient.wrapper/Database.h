#pragma once

#include <idaclient/Database.h>

#include "Functions.h"
#include "Enumerations.h"

using System::String;

namespace Ida {
	namespace Client {
		public ref class Database
		{
		private:
			::Database *m_database;
			Database(::Database *database);
		private: //fields
			Enumerations^ _enumerations;
			Functions^ _functions;
		public:			
			~Database(void);
			static Database^ Open(String^ path);			
			property Functions^ Functions {
				Ida::Client::Functions^ get() { return _functions; }
			}
			property Enumerations^ Enumerations {
				Ida::Client::Enumerations^ get() { return _enumerations; }
			}
		};
	}
}