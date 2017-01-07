link: Timestamp.h ControlModule.o Main.o
	@cl /Fe"CASP.exe" obj/*.o

Main.o:
	@cl /c src/Main.cpp /Fo"obj/Main.o"

ControlModule.o:
	@cl /c src/ControlModule.cpp /Fo"obj/ControlModule.o"

Timestamp.h:
	@echo /*>src/Timestamp.h
	@echo  * It is not advised to edit this file, as it>>src/Timestamp.h
	@echo  * is automatically generated with every build>>src/Timestamp.h
	@echo  */>>src/Timestamp.h
	@echo #ifndef TIMESTAMP>>src/Timestamp.h
	@echo #define TIMESTAMP "%timestamp%">>src/Timestamp.h
	@echo #endif>>src/Timestamp.h