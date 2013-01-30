IDA (Interactive Disassembler) as a service
=====

Target of the project is to significantly simplify creation of "plug-ins" on different languages rather than native supported by IDA (C/C++, IDC, Python with idapython plug-in) and using full power of development instruments (like debuging, test covering, using generators and third-party libraries).

Now if you want to add some advanced logic (like automatic structures/functions detection, calculating and marking constants etc) you should write your own plug-in or find some with similar functionality (but it is not always possible).

And imagine now that you can do it in on your favorite language with just a few steps:
1) Install idaas plug-in for IDA;
2) Generate client for your favorite language with amazing thrift tool by idaas.thrift;
3) Add generated client to your project.

You got it!

You may still not access to full power of IDA API, but this project is open source and you always can contribute and extend remote API with necessary features or fork in special cases.
This open ways to produce frameworks and libraries based on remote API and, even more, because remote API creates new abstraction layer it theorethecally may be extended on other disassemblers. Also, it may be composed to develope in remote mode and release in plug-in mode with same API.

It is a picture of the future. Initially I started this project, because lack of support my favorite programming languages, so I tried first IDC, but it is very limited and hard to support when classes goes after few tens. After that I found idapython and even studied Python from stratch (but I don't like Python =)). Its started well, but with amount of code growth also growth number of bugs and dealing with them makes real pain. So, finally I decide to move logic outside of IDA and use it as service. After that I thought that it may be interesting for other people who can join to development and help with improving this project or may be it just helps somebody.

The project is on very early stage and I haven't enough time for active work on it and you can have hard times to get it and start working on it (if you decide to contribute). So, first thing that can help alot is: CMake config to detach it from Visual Studio and good project setup instructions. If you are insterested and want to contribute you help will be accepted with gratitude.