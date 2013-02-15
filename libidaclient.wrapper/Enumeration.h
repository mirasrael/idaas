#pragma once

using System::String;
using System::UInt32;
using System::Byte;

namespace Ida {
	namespace Client {
		public ref class EnumerationConstant {
		public:
			property UInt32 Id;
			property String^ Name;
			property UInt32 Value;
			property Byte Serial;
			property UInt32 Mask;

			EnumerationConstant() {
				Mask = UInt32(-1);
			}
		};

		public ref class Enumeration
		{		
		public:
			Enumeration();

			property unsigned __int32 Id;
			property String^ Name;
			property bool IsBitfield;
			property System::Collections::Generic::List<EnumerationConstant^>^ Constants;

			virtual String^ ToString() override { return Name; } 
		};
	}
}

