#pragma once

#include <idaclient\Database.h>
#include <idaclient\Enumeration.h>
#include "Enumeration.h"

using System::Collections::Generic::IEnumerable;
using System::Collections::Generic::IEnumerator;
using System::Collections::Generic::List;
using System::Collections::Generic::Dictionary;

namespace Ida {
	namespace Client {
		public enum class ItemState {
			New,
			Updated
		};

		public ref class Enumerations : public IEnumerable<Enumeration^>
		{			
		private: //fields
			List<Enumeration^>^ _items;
			Dictionary<Enumeration^, ItemState>^ _itemStates;

		private: //properties
			property List<Enumeration^>^ Items
			{
				System::Collections::Generic::List<Enumeration^>^ get() { return _items == nullptr ? (_items = Load()) : _items; }
			}

		private: //methods
			::Database *_database;
			List<Enumeration^>^ Load();
			shared_ptr<IdaEnumeration> ToNative(Enumeration^ enumeration);

		public:
			Enumerations(::Database *database);

			virtual System::Collections::IEnumerator^ IEnumerable_GetEnumerator() = System::Collections::IEnumerable::GetEnumerator;
			virtual IEnumerator<Ida::Client::Enumeration^>^ GetEnumerator();
			Enumeration^ New(String^ name) { return New(name, false); } 
			Enumeration^ New(String^ name, bool isBitfield);
			bool Persist(Enumeration^ enumeration);
			bool Delete(Enumeration^ enumeration);
		};
	}
}

