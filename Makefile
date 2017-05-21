##
## Makefile for nsUtilLus as a static library
##
## october 2011
##

#########################################################
#							#
# 	Nom du programme et des fichiers sources	#
#							#
#########################################################

PROJECT = epiClassifier

OBJSPATH	= ./objs/

# sources
SRC		= epiClassifierLus.cpp
OBJS    	= epiClassifierLus.o

SRCENGINE	= ./enterpriseLus/requestEngineLus.cpp ./nsdatabaseLus/nsSQL.cpp
OBJENGINE	= ./enterpriseLus/requestEngineLus.o ./nsdatabaseLus/nsSQL.o

SRCENTERPRISE	= ./enterpriseLus/superLus.cpp ./enterpriseLus/personLus.cpp
OBJENTERPRISE	= ./enterpriseLus/superLus.o ./enterpriseLus/personLus.o

SRCUTIL		= ./nsUtilLus/nsdivfctForCgi.cpp
OBJUTIL		= ./nsUtilLus/nsdivfctForCgi.o

SRCCLASSLIB	= ./classlib/date.cpp ./classlib/time.cpp ./include/classlib/time.h
OBJCLASSLIB	= ./classlib/date.o ./classlib/time.o

EPIPATH		= .

SRCNSBB		= $(EPIPATH)/nsbb/nspatpat.cpp $(EPIPATH)/nsbb/nsbbtran.cpp
OBJNSBB		= $(EPIPATH)/nsbb/nspatpat.o $(EPIPATH)/nsbb/nsbbtran.o

SRCEPISOD	= $(EPIPATH)/nsepisod/nsclasser.cpp $(EPIPATH)/nsepisod/eptables.cpp $(EPIPATH)/nsepisod/nsldvuti.cpp \
		$(EPIPATH)/nsepisod/flechiesDB.cpp
OBJEPISOD	= $(EPIPATH)/nsepisod/nsclasser.o $(EPIPATH)/nsepisod/eptables.o $(EPIPATH)/nsepisod/nsldvuti.o $(EPIPATH)/nsepisod/flechiesDB.o

SRCSAVOIR	= $(EPIPATH)/nssavoir/Nsfilgd.cpp $(EPIPATH)/nssavoir/nsvarray.cpp $(EPIPATH)/nssavoir/nslocalise.cpp \
		$(EPIPATH)/nssavoir/nspatho.cpp $(EPIPATH)/nssavoir/nsguide.cpp $(EPIPATH)/nssavoir/nspatbas.cpp \
		$(EPIPATH)/nssavoir/nsconver.cpp
OBJSAVOIR	= $(EPIPATH)/nssavoir/Nsfilgd.o $(EPIPATH)/nssavoir/nsvarray.o $(EPIPATH)/nssavoir/nslocalise.o $(EPIPATH)/nssavoir/nspatho.o \
		$(EPIPATH)/nssavoir/nsguide.o $(EPIPATH)/nssavoir/nspatbas.o $(EPIPATH)/nssavoir/nsconver.o

SRCDCODEUR	= $(EPIPATH)/dcodeur/nsphrase.cpp $(EPIPATH)/dcodeur/nsdkd.cpp $(EPIPATH)/dcodeur/nsgen.cpp \
		$(EPIPATH)/dcodeur/nsgenFr.cpp $(EPIPATH)/dcodeur/nsgenEn.cpp
OBJDCODEUR	= $(EPIPATH)/dcodeur/nsphrase.o $(EPIPATH)/dcodeur/nsdkd.o $(EPIPATH)/dcodeur/nsgen.o $(EPIPATH)/dcodeur/nsgenFr.o \
		$(EPIPATH)/dcodeur/nsgenEn.o

SRCDTILIB	= $(EPIPATH)/dtilib/sockets/sockstream.cpp $(EPIPATH)/dtilib/sockets/sock.cpp $(EPIPATH)/dtilib/async/events.cpp  \
		$(EPIPATH)/dtilib/async/async.cpp
OBJDTILIB	= $(EPIPATH)/dtilib/sockets/sockstream.o $(EPIPATH)/dtilib/sockets/sock.o $(EPIPATH)/dtilib/async/events.o \
		$(EPIPATH)/dtilib/async/async.o

# librairies
LIB	= -lmysqlclient ./pr/target.debug/dist/lib/libnspr4.so

#########################################################
#							#
#		Parametres de compilation		#
#							#
#########################################################

# chemin de l'include
IPATH	= -I. -I./include -I$(EPIPATH)/include -I./pr/target.debug/dist/include/nspr
DEFS    = -D_ENTERPRISE_DLL=1 -D_NO_BDE=1

# option pour obj
OBJOPT	= -Wall -g2 #-Wall -pedantic

EXEOPT	= -g2 # option pour exe
LPATH	= -L. # chemin de la lib

#########################################################
#							#
#		Definitions des macros			#
#							#
#########################################################

CC 	= g++
MAKE 	= make
SHELL	= /bin/sh
RM 	= /bin/rm -f

CFLAGS  = $(OBJOPT) $(IPATH) $(DEFS)
LDFLAGS = $(EXEOPT) $(LPATH)

#########################################################
#							#
#	Instructions de compilation primaires		#
#							#
#########################################################

all		: ENGINE ENTERPRISE NSBB EPISOD DCODEUR SAVOIR UTIL CLASSLIB DTILIB $(PROJECT)
		-$(RM) *.core

$(PROJECT)	: $(OBJS)
		$(CC) $(LDFLAGS) $(LIB) -o $(PROJECT) $(OBJS) $(OBJENGINE) $(OBJENTERPRISE) $(OBJNSBB) $(OBJEPISOD) $(OBJSAVOIR) $(OBJDCODEUR) $(OBJUTIL) $(OBJDTILIB) $(OBJCLASSLIB)

ENGINE		: $(OBJENGINE)

ENTERPRISE	: $(OBJENTERPRISE)

UTIL		: $(OBJUTIL)

NSBB		: $(OBJNSBB)

EPISOD		: $(OBJEPISOD)

SAVOIR		: $(OBJSAVOIR)

DCODEUR		: $(OBJDCODEUR)

DTILIB		: $(OBJDTILIB)

CLASSLIB	: $(OBJCLASSLIB)

clean		:
		-$(RM) $(OBJS) $(OBJENGINE) $(OBJENTERPRISE) $(OBJNSBB) $(OBJEPISOD) $(OBJSAVOIR) $(OBJDCODEUR) $(OBJUTIL) $(OBJDTILIB) $(OBJCLASSLIB) *~ \#*

comp		: fclean
		$(COMP) $(INCL) $(SRC)

.cpp.o:
		$(CC) -c $(CFLAGS) $< -o $*.o

