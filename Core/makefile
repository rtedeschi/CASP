link: Timestamp.h Plugins.o CASP_Plugin.o Helpers.o LanguageDescriptor.o ControlModule.o Markup.o Main.o 
	@cl /Fe"CASP.exe" obj/*.o

Main.o:
	@cl /c src/Main.cpp /Fo"obj/Main.o"

ControlModule.o:
	@cl /c src/ControlModule.cpp /Fo"obj/ControlModule.o"

LanguageDescriptor.o:
	@cl /c shared/LanguageDescriptor/LanguageDescriptor.cpp /Fo"obj/LanguageDescriptor.o"

Markup.o:
	@cl /c shared/Markup/Markup.cpp /Fo"obj/Markup.o"

Helpers.o:
	@cl /c shared/Helpers/Helpers.cpp /Fo"obj/Helpers.o"

CASP_Plugin.o:
	@cl /c shared/CASP_Plugin/CASP_Plugin.cpp /Fo"obj/CASP_Plugin.o"

Plugins.o:
	@call "tools/build_plugins.bat"

Timestamp.h:
	@echo /*>src/Timestamp.h
	@echo  * It is not advised to edit this file, as it>>src/Timestamp.h
	@echo  * is automatically generated with every build>>src/Timestamp.h
	@echo  */>>src/Timestamp.h
	@echo #ifndef TIMESTAMP>>src/Timestamp.h
	@echo #define TIMESTAMP "%timestamp%">>src/Timestamp.h
	@echo #endif>>src/Timestamp.h