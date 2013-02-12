#pragma once

using namespace System;

namespace Ida {
	namespace Client {
		public ref class Enumeration
		{				
		public:
			Enumeration();

			property unsigned __int32 Id;
			property String^ Name;
			property bool IsBitfield;			
		};
	}
}

