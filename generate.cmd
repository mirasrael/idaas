%THIRD_PARTY%\thrift\thrift-0.9.0.exe --gen cpp idaas.thrift
del gen-cpp\*.skeleton.cpp /Q
copy gen-cpp\*.cpp idaas
copy gen-cpp\*.h idaas
rmdir gen-cpp /Q /S