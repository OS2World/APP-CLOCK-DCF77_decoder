# makefile
# Erstellt von IBM WorkFrame/2 MakeMake um 22:05:06 am 6 Jan 2003
#
# Diese Make-Datei enth„lt folgende Aktionen:
#  Compile::C++ Compiler
#  Link::Linker

.SUFFIXES:

.SUFFIXES: \
    .cpp .obj 

.cpp.obj:
    @echo " Compile::C++ Compiler "
    icc.exe /Tdp /Ti /Rn /Gd /C %s

{n:\ablage\markus\c\dcf77}.cpp.obj:
    @echo " Compile::C++ Compiler "
    icc.exe /Tdp /Ti /Rn /Gd /C %s

all: \
    .\dcf77.exe

.\dcf77.exe: \
    .\os_api.obj \
    .\zeit.obj \
    .\clock.obj \
    .\main.obj
    @echo " Link::Linker "
    icc.exe @<<
     /B" /de"
     /Fedcf77.exe 
     .\os_api.obj
     .\zeit.obj
     .\clock.obj
     .\main.obj
<<

.\os_api.obj: \
    n:\ablage\markus\c\dcf77\os_api.cpp \
    {n:\ablage\markus\c\dcf77;$(INCLUDE);}zeit.h \
    {n:\ablage\markus\c\dcf77;$(INCLUDE);}os_api.h

.\main.obj: \
    n:\ablage\markus\c\dcf77\main.cpp \
    {n:\ablage\markus\c\dcf77;$(INCLUDE);}zeit.h \
    {n:\ablage\markus\c\dcf77;$(INCLUDE);}clock.h \
    {n:\ablage\markus\c\dcf77;$(INCLUDE);}os_api.h

.\clock.obj: \
    n:\ablage\markus\c\dcf77\clock.cpp \
    {n:\ablage\markus\c\dcf77;$(INCLUDE);}zeit.h \
    {n:\ablage\markus\c\dcf77;$(INCLUDE);}clock.h \
    {n:\ablage\markus\c\dcf77;$(INCLUDE);}os_api.h

.\zeit.obj: \
    n:\ablage\markus\c\dcf77\zeit.cpp \
    {n:\ablage\markus\c\dcf77;$(INCLUDE);}zeit.h
