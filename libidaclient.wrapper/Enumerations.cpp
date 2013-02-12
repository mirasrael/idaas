#include "Enumerations.h"

#include <vector>
#include <fstream>

using System::Collections::Generic::List;
using namespace std;

namespace Ida {
	namespace Client {
		static int __cdecl Enumerations_OnNextFunction(shared_ptr<IdaEnumeration> &enumeration, void *ud)
		{
			reinterpret_cast<vector<shared_ptr<IdaEnumeration>>*>(ud)->push_back(enumeration);
			return 0;
		}

		Enumerations::Enumerations(::Database *database) : m_database(database)
		{
		}

		IEnumerator<Enumeration^>^ Enumerations::GetEnumerator() {
			vector<shared_ptr<IdaEnumeration>> enumerations;
			int result = m_database->EnumEnumerations(Enumerations_OnNextFunction, &enumerations);
			List<Enumeration^>^ enumerations2 = gcnew List<Enumeration^>();
			for (vector<shared_ptr<IdaEnumeration>>::iterator it = enumerations.begin(); it != enumerations.end(); it++) {
				Enumeration^ enumeration = gcnew Enumeration();				
				enumeration->Id = (*it)->id;
				enumeration->Name = gcnew String((*it)->name.c_str());
				enumeration->IsBitfield = (*it)->is_bitfield;
				enumerations2->Add(enumeration);
			}
			return enumerations2->GetEnumerator();
		}

		System::Collections::IEnumerator^ Enumerations::IEnumerable_GetEnumerator() {
			return GetEnumerator();
		}
	}
}