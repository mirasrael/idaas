#pragma once

#pragma warning(disable: 4996)
#include <ida.hpp>
#pragma warning(default: 4996)

extern "C" int idaapi GetDatabaseServerPort();
extern "C" int idaapi CreateConnection();
extern "C" int idaapi CloseConnection();
