link: ControlModule.o Main.o
	cl /Fe"dist/build_%DATE:~-4%-%DATE:~4,2%-%DATE:~7,2%_%TIME:~0,2%.%TIME:~3,2%.%TIME:~6,2%.exe" obj/*.o
	cl /Fe"Latest_Build.exe" obj/*.o
Main.o:
	cl /c src/Main.cpp /Fo"obj/Main.o"

ControlModule.o:
	cl /c src/ControlModule.cpp /Fo"obj/ControlModule.o"