#pragma once

using namespace System;

namespace Ida {
	namespace Client {
		public ref class Enumeration
		{				
		public:
			Enumeration();

			property String^ Name;
			property bool IsBitfield;
		};
	}
}

