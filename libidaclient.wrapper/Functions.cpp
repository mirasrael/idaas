#include "Functions.h"
#include <vector>
#include <fstream>

using System::Collections::Generic::List;
using namespace std;

namespace Ida {
	namespace Client {
		static int __cdecl Functions_OnNextFunction(IdaFunction *function, void *ud)
		{
			vector<IdaFunction>* functions = (vector<IdaFunction>*)ud;
			IdaFunction copy;
			functions->push_back(copy);
			return 0;
		}

		Functions::Functions(::Database *database) : m_database(database)
		{
		}

		IEnumerator<Function^>^ Functions::GetEnumerator() {
			vector<IdaFunction> functions;			
			int result = m_database->EnumFunctions(Functions_OnNextFunction, &functions);			
			List<Function^>^ functions2 = gcnew List<Function^>();
			for (vector<IdaFunction>::iterator it = functions.begin(); it != functions.end(); it++) {
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