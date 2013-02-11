#pragma once

#include <idaclient\Database.h>
#include <idaclient\Enumeration.h>
#include "Enumeration.h"

using System::Collections::Generic::IEnumerable;
using System::Collections::Generic::IEnumerator;

namespace Ida {
	namespace Client {
		public ref class Enumerations : public IEnumerable<Ida::Client::Enumeration^>
		{
			private:
			::Database *m_database;			
		public:
			Enumerations(::Database *database);

			virtual System::Collections::IEnumerator^ IEnumerable_GetEnumerator() = System::Collections::IEnumerable::GetEnumerator;
			virtual IEnumerator<Ida::Client::Enumeration^>^ GetEnumerator();
		};
	}
}

