#include "Enumerations.h"

#include <vector>
#include <fstream>
#include <atlstr.h>

using namespace std;

namespace Ida {
	namespace Client {		
		Enumerations::Enumerations(::Database *database) : _database(database)
		{
			_itemStates = gcnew Dictionary<Enumeration^, ItemState>();
		}

		List<Enumeration^>^ Enumerations::Load() {
			try {
				List<Enumeration^>^ enumerations = gcnew List<Enumeration^>();

				shared_ptr<EnumerationsReader> reader = _database->GetEnumerationsReader();
				shared_ptr<IdaEnumeration> idaEnumeration;
				while (0 != (idaEnumeration = reader->Read())) {
					Enumeration^ enumeration = gcnew Enumeration();				
					enumeration->Id = idaEnumeration->id;
					enumeration->Name = gcnew String(idaEnumeration->name.c_str());
					enumeration->IsBitfield = idaEnumeration->is_bitfield;
					enumeration->Constants = gcnew List<EnumerationConstant^>(idaEnumeration->costants.size());
					for (vector<shared_ptr<IdaEnumerationConstant>>::iterator it = idaEnumeration->costants.begin();
						it != idaEnumeration->costants.end(); it++) {
							shared_ptr<IdaEnumerationConstant> &idaConstant = *it;
							EnumerationConstant^ constant = gcnew EnumerationConstant();
							constant->Id = idaConstant->id;
							constant->Name = gcnew String(idaConstant->name.c_str());
							constant->Value = idaConstant->value;
							constant->Mask = idaConstant->bit_mask;
							constant->Serial = idaConstant->serial;
							enumeration->Constants->Add(constant);
					}
					enumerations->Add(enumeration);
				}
				return enumerations;
			} catch(const char *error) {
				throw gcnew System::Exception(gcnew String(error));
			}
		}

		IEnumerator<Enumeration^>^ Enumerations::GetEnumerator() {			
			return Items->GetEnumerator();	
		}

		System::Collections::IEnumerator^ Enumerations::IEnumerable_GetEnumerator() {
			return GetEnumerator();
		}
		
		Enumeration^ Enumerations::New(String^ name, bool isBitfield) {
			Enumeration^ resultEnumeration = nullptr;
			for each(Enumeration^ enumeration in this) {
				if (enumeration->Name->Equals(name)) {
					resultEnumeration = enumeration;
					resultEnumeration->Constants->Clear();
					break;
				}
			}
			if (resultEnumeration == nullptr) {							
				resultEnumeration = gcnew Enumeration();
				resultEnumeration->Name = name;
				resultEnumeration->IsBitfield = isBitfield;
				resultEnumeration->Constants = gcnew List<EnumerationConstant^>();
				Items->Add(resultEnumeration);				
			}
			_itemStates[resultEnumeration] = resultEnumeration->Id != 0 ? ItemState::Updated : ItemState::New;			
			return resultEnumeration;
		}

		shared_ptr<IdaEnumeration> Enumerations::ToNative(Enumeration^ enumeration) {
			shared_ptr<IdaEnumeration> result(new IdaEnumeration());
			result->id = enumeration->Id;
			result->is_bitfield = enumeration->IsBitfield;
			result->name = CString(enumeration->Name);
			for each (EnumerationConstant^ constant in enumeration->Constants) {
				shared_ptr<IdaEnumerationConstant> idaConstant(new IdaEnumerationConstant());
				idaConstant->id = constant->Id;
				idaConstant->name = CString(constant->Name);
				idaConstant->value = constant->Value;
				idaConstant->bit_mask = constant->Mask;
				idaConstant->serial = constant->Serial;
				result->costants.push_back(idaConstant);
			}
			return result;
		}

		bool Enumerations::Delete(Enumeration^ enumeration)
		{
			if (enumeration->Id != 0 && !_database->DeleteEnum(enumeration->Id))
				return false;

			Items->Remove(enumeration);
			return true;
		}

		bool Enumerations::Persist(Enumeration^ enumeration) {
			ItemState state;
			if (!_itemStates->TryGetValue(enumeration, state)) {
				return true;
			}			
			switch(state) {
			case ItemState::New:				
				return _database->CreateEnum(ToNative(enumeration));
			case ItemState::Updated:
				return _database->UpdateEnum(ToNative(enumeration));
			default:
				return false;
			}			
		}
	}
}