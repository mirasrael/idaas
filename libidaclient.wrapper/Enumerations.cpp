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
			try {
				List<Enumeration^>^ enumerations = gcnew List<Enumeration^>();

				shared_ptr<EnumerationsReader> reader = m_database->GetEnumerationsReader();
				shared_ptr<IdaEnumeration> idaEnumeration;
				while (0 != (idaEnumeration = reader->Read())) {
					Enumeration^ enumeration = gcnew Enumeration();				
					enumeration->Id = idaEnumeration->id;
					enumeration->Name = gcnew String(idaEnumeration->name.c_str());
					enumeration->IsBitfield = idaEnumeration->is_bitfield;
					enumerations->Add(enumeration);
				}
				return enumerations->GetEnumerator();
			} catch(const char *error) {
				throw gcnew Exception(gcnew String(error));
			}									
		}

		System::Collections::IEnumerator^ Enumerations::IEnumerable_GetEnumerator() {
			return GetEnumerator();
		}
	}
}