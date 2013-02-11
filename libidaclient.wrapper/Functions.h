#pragma once

#include <idaclient\Database.h>
#include <idaclient\Function.h>
#include "Function.h"

using System::Collections::Generic::IEnumerable;
using System::Collections::Generic::IEnumerator;

namespace Ida {
	namespace Client {
		public ref class Functions : public IEnumerable<Ida::Client::Function^>
		{
		private:
			::Database *m_database;			
		public:
			Functions(::Database *database);
		public:			
			virtual System::Collections::IEnumerator^ IEnumerable_GetEnumerator() = System::Collections::IEnumerable::GetEnumerator;
			virtual IEnumerator<Ida::Client::Function^>^ GetEnumerator();
		};
	}
}


