#include "Enumerations.h"

#include <vector>
#include <fstream>

using System::Collections::Generic::List;
using namespace std;

namespace Ida {
	namespace Client {
		static int __cdecl Enumerations_OnNextFunction(IdaEnumeration *enumeration, void *ud)
		{
			vector<IdaEnumeration>* functions = (vector<IdaEnumeration>*)ud;
			IdaEnumeration copy;
			copy.name = enumeration->name;
			copy.is_bitfield = enumeration->is_bitfield;
			functions->push_back(copy);
			return 0;
		}

		Enumerations::Enumerations(::Database *database) : m_database(database)
		{
		}

		IEnumerator<Enumeration^>^ Enumerations::GetEnumerator() {
			vector<IdaEnumeration> enumerations;
			int result = m_database->EnumEnumerations(Enumerations_OnNextFunction, &enumerations);
			List<Enumeration^>^ enumerations2 = gcnew List<Enumeration^>();
			for (vector<IdaEnumeration>::iterator it = enumerations.begin(); it != enumerations.end(); it++) {
				Enumeration^ enumeration = gcnew Enumeration();				
				enumeration->Name = gcnew String(it->name.c_str());
				enumeration->IsBitfield = it->is_bitfield;
				enumerations2->Add(enumeration);
			}
			return enumerations2->GetEnumerator();
		}

		System::Collections::IEnumerator^ Enumerations::IEnumerable_GetEnumerator() {
			return GetEnumerator();
		}
	}
}