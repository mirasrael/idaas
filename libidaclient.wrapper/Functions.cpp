#include "Functions.h"
#include <vector>
#include <fstream>

using System::Collections::Generic::List;
using namespace std;

namespace Ida {
	namespace Client {
		static int __cdecl Functions_OnNextFunction(shared_ptr<IdaFunction> &function, void *ud)
		{
			reinterpret_cast<vector<shared_ptr<IdaFunction>>*>(ud)->push_back(function);
			return 0;
		}

		Functions::Functions(::Database *database) : m_database(database)
		{
		}

		IEnumerator<Function^>^ Functions::GetEnumerator() {
			vector<shared_ptr<IdaFunction>> functions;			
			int result = m_database->EnumFunctions(Functions_OnNextFunction, &functions);			
			List<Function^>^ functions2 = gcnew List<Function^>();
			for (vector<shared_ptr<IdaFunction>>::iterator it = functions.begin(); it != functions.end(); it++) {
				Function^ function = gcnew Function();				
				functions2->Add(function);
			}
			return functions2->GetEnumerator();
		}

		System::Collections::IEnumerator^ Functions::IEnumerable_GetEnumerator() {
			return GetEnumerator();
		}
	}
}