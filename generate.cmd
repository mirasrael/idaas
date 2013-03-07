%THIRD_PARTY%\thrift\thrift-0.9.0.exe --gen cpp idaas.thrift
del gen-cpp\*.skeleton.cpp /Q
copy gen-cpp\*.cpp libidaas.thrift
copy gen-cpp\*.h libidaas.thrift
rmdir gen-cpp /Q /S