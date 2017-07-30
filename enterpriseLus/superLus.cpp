/* ========================================================================
 * Copyright 2011 Philippe Ameline
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 * ========================================================================
 */

// -----------------------------------------------------------------------------
// superLus.cpp
// -----------------------------------------------------------------------------
// Supervisor and Contexts
// -----------------------------------------------------------------------------
// $Revision: 1.0 $
// $Author: pameline $
// $Date: 2009/09/03 08:29:41 $
// -----------------------------------------------------------------------------


#include <stdio.h>
#include <fstream>
#include <string>
#include <cstdarg>
#include <nspr/prtime.h>
#include <map>

#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <libconfig.h++>

using std::string ;

// using namespace std ;

#include "enterpriseLus/superLus.h"

#include "enterpriseLus/nsdivfctForCgi.h"

#ifdef FOR_WINDOWS
#include "pilot/Pilot.hpp"
#include "pilot/NautilusPilot.hpp"
#include "pilot/JavaSystem.hpp"
#endif

#include "enterpriseLus/personLus.h"
#include "nsdatabaseLus/nsSQL.h"

#include "nssavoir/nsfilgd.h"

#ifndef __linux__
#include "nssavoir/nsgraphe.h"
#endif

using namespace std;
using namespace libconfig;

/*
#include "nsbb/nsdefArch.h"
#include "ns_ob1/nautilus-bbk.h"
*/

//***************************************************************************
//   NSSuper
//***************************************************************************

//---------------------------------------------------------------------------
//  Constructeur.
//
//  Cree une instance d'objet de la classe NSSuper.
//	 Initialise statut.
//
//---------------------------------------------------------------------------
NSSuper::NSSuper(short sStatus)
{
    try
    {
        _statut              = sStatus ;

        _sNumVersion         = std::string("0.00.0002") ;

        _pDico               = (NSDico *) 0 ;
        _pFilGuide           = (NSFilGuide *) 0 ;
        _pFilDecode          = (NSFilGuide *) 0 ;

        _iTraceConsole       = trNone ;
        _iTrace              = trWarning  ;

        _ontologyManager     = (ontologyBaseManager*) 0 ;

        // _pLocalStrings       = new NSLocalChapterArray(NULL) ;

        loadLocalisationString("") ;

#ifndef __linux__
        _pArcManager         = new NSArcManager(this) ;
#endif
        _pClassifExpert      = (classifExpert *) 0 ;

        // _aContextArray       = new NSContextArray ;

        // _bVerboseErrorMessages = true ;

        _bToDoLocked         = false ;

        // m_hStdOut	= (HANDLE) 0 ;
    }
    catch (...)
    {
        erreur("Exception NSSuper ctor.", standardError) ;
    }
}

void
NSSuper::InitPointers()
{
    _pDico       = new NSDico(NULL) ;
    _pFilGuide   = new NSFilGuide(this, GUIDE) ;
    _pFilDecode  = new NSFilGuide(this, DECODE) ;
}

//---------------------------------------------------------------------------
//  Constructeur copie
//---------------------------------------------------------------------------
NSSuper::NSSuper(NSSuper& srcNSSuper)
    :	_statut(srcNSSuper._statut),
      _pDico(srcNSSuper._pDico),
      _pFilGuide(srcNSSuper._pFilGuide)
{
    _bToDoLocked    = false ;
}

//---------------------------------------------------------------------------
//  Operateur =
//---------------------------------------------------------------------------
NSSuper&
NSSuper::operator=(NSSuper& srcNSSuper)
{
    if (this == &srcNSSuper)
        return *this ;

    _statut           = srcNSSuper._statut ;
    _pDico    		    = srcNSSuper._pDico ;
    _pFilGuide   	    = srcNSSuper._pFilGuide ;

    _bToDoLocked       = false ;

    return (*this) ;
}

//---------------------------------------------------------------------------
//  Destructeur
//---------------------------------------------------------------------------
NSSuper::~NSSuper()
{
    // closeBlackboard(NULL) ;

    delete _pDico ;
    _pDico = 0 ;
    delete _pFilGuide ;
    delete _pFilDecode ;
#ifndef __linux__
    delete _pArcManager ;
#endif
}

//
// Fonction trace declaree dans nssuper.h pour etre accessible par NS_SGBD
//
//---------------------------------------------------------------------------------------
//  Function: 	  NSSuper::trace(string** pTracePtr, int nbString)
//  Arguments:	  pTracePtr : pointeur sur un tableau de pointeurs sur les string
//					  				  a tracer (si bTrace == true)
//					  nbString :  nombre de chaines - optionnel, la trace s'arrete
//									  au premier pointeur vide
//  Description: Inscrit les strings dans le fichier de trace avec l'heure systeme
//  Returns:     Rien
//---------------------------------------------------------------------------------------
void
NSSuper::trace(string pTracePtr[], int nbString, TRACETYPE iTraceLevel)
{
    try
    {
        if (iTraceLevel > _iTrace)
            return ;

        if ((NULL == pTracePtr) || (string("") == pTracePtr[0]))
            return ;

        string sTraceFileName = string("traceNau.inf") ;

        ofstream outFile ;
        outFile.open(sTraceFileName.c_str(), ios::app) ;
        if (!outFile)
            return ;

        // calcul de l'heure systeme - get system time
        // calcul de l'heure système - get system time
        /*
    struct  date dateSys;
    struct  time heureSys;
  */
        char    msg[255] ;

        PRTime         now = PR_Now() ;
        PRExplodedTime et_tmp ;
        PR_ExplodeTime(now, PR_GMTParameters, &et_tmp) ;
        sprintf(msg, "%02d/%02d/%4d - %02d:%02d:%02d,%02d>", et_tmp.tm_mday,
                et_tmp.tm_month,
                et_tmp.tm_year,
                et_tmp.tm_hour,
                et_tmp.tm_min,
                et_tmp.tm_sec,
                et_tmp.tm_usec) ;
        /*
    getdate(&dateSys) ;
    gettime(&heureSys) ;
    sprintf(msg, "%02d/%02d/%4d - %02d:%02d:%02d,%02d>", dateSys.da_day, dateSys.da_mon,
                dateSys.da_year, heureSys.ti_hour, heureSys.ti_min,
                heureSys.ti_sec, heureSys.ti_hund) ;
*/

        int iNbre = nbString ;
        if (-1 == iNbre)
            iNbre = 50 ;

        outFile << string(msg) ;

        for (int i = 0 ; (i < iNbre) && (string("") != pTracePtr[i]) ; i++)
        {
            for (int j = 0 ; j < iTraceLevel ; j++)
                outFile << string("\t") ;
            outFile << pTracePtr[i] ;
        }

        outFile << string("\n") ;
        outFile.close() ;
    }
    catch (...)
    {
        erreur("Exception NSSuper::trace.", standardError) ;
    }
}

void
NSSuper::CheckZeroObjectCount(string sObjectName, long lInstanceCounter)
{
    if ((long)0 == lInstanceCounter)
        return ;

    char buf[255] = {0} ;
    sprintf(buf, "%ld", lInstanceCounter) ;

    string sTrace = string("Memory leak for object ") + sObjectName +
            string(" (Instance counter = ") + string(buf) + string(")") ;
    trace(&sTrace, 1) ;
}

bool
NSSuper::loadLocalisationString(std::string sLangage, NSLocalChapterArray* pLocStrings)
{
    /*
  NSLocalChapterArray* pActualLocStrings = pLocalStrings ;
  if (NULL != pLocStrings)
    pActualLocStrings = pLocStrings ;
  else if ((sLangage == sLang) && (false == pLocalStrings->empty()))
        return true ;

    ifstream inFile1 ;

    if (sLangage != "")
    {
        string sFichier = string("localisation_") + sLangage + string(".dat") ;
        inFile1.open(sFichier.c_str()) ;
    }
    else
    {
        inFile1.open("localisation.dat") ;
        if (!inFile1)
        return false ;
    }

    if (!inFile1)
    {
        ifstream inFile2 ;

        string sFichier = string("localisation.dat") ;
        inFile2.open(sFichier.c_str()) ;
        if (!inFile2)
            return false ;

        pActualLocStrings->init(&inFile2) ;

        inFile2.close() ;
    }
    else
    {
        pActualLocStrings->init(&inFile1) ;

        inFile1.close() ;
    }

  if (NULL != pLocStrings)
    sLang = sLangage ;

*/
    return true ;
}

std::string
NSSuper::getText(std::string sChapter, std::string sCode, std::string sLang, NSContexte* pCtx)
{
    /*
  if (string("") == sLang)
  {
      if (NULL != pLocalStrings)
          return pLocalStrings->getLocalText(sChapter, sCode, true) ;
    else
      return string("") ;
  }

  if (NULL == pCtx)
    return string("") ;

  NSLocalChapterArray aLocStrings(pCtx) ;
  if (false == loadLocalisationString(sLang, &aLocStrings))
    return string("") ;

  return aLocStrings.getLocalText(sChapter, sCode, true) ;
*/

    return std::string("") ;
}

bool NSSuper::InitDatabase()
{
    Config cfg;
    // Read the file. If there is an error, report it and exit.
    try
    {
        cfg.readFile("./config/sql.cfg");
    }
    catch(const FileIOException &fioex)
    {
        std::cerr << "I/O error while reading file." << std::endl;
        return(EXIT_FAILURE);
    }
    catch(const ParseException &pex)
    {
        std::cerr << "Parse error at " << pex.getFile() << ":" << pex.getLine()
                  << " - " << pex.getError() << std::endl;
        return(EXIT_FAILURE);
    }

    std::map<std::string, std::string> credentials;
    std::vector<string> keys = {"host", "database", "user", "password"};

    for (string key : keys)
    {
        try
        {
            credentials[key] = cfg.lookup(key).c_str();
            std::cout << "Database " << key << ": " << credentials[key] << endl;
        }
        catch(const SettingNotFoundException &nfex)
        {
            cerr << "No " << key << " setting in configuration file." << endl;
        }
    }

    try
    {
        std::string ps = std::string("Entering InitDatabase") ;
        trace(&ps, 1, trSteps) ;

        _ontologyManager = new ontologyBaseManager(credentials["host"],
                credentials["user"],
                credentials["password"],
                credentials["database"]);
        if ((ontologyBaseManager *) NULL == _ontologyManager)
            return false ;

        bool bBaseOpened = _ontologyManager->openBase() ;
        if (bBaseOpened)
        {
            ps = std::string("Database connexion done");
            std::cout << ps << std::endl;
            trace(&ps, 1, trSteps) ;

            _ontologyManager->closeBase();

            return true ;
        }

        ps = std::string("Database connexion failed ") + _ontologyManager->getMysqlError();
        std::cout << ps << std::endl;
        trace(&ps, 1, trError) ;

        return false ;
    }
    catch (...)
    {
        erreur("Exception Supervisor::InitDatabase.", standardError) ;
        return false ;
    }
}
#ifdef FOR_WINDOWS
bool
NSSuper::InitialiseJava(){	string ps = string("Java init - begin") ;	trace(&ps, 1, trSteps) ;	ps = string("Java init : opening file pilot.dat") ;	trace(&ps, 1, trSubSteps) ;	ifstream inFile ;	inFile.open("pilot.dat") ;	if (!inFile)
    {
        string sErreur = getText("pilotManagement", "cannotOpenPilot.dat") ;
        trace(&sErreur, 1, NSSuper::trError) ;
        _bJavaOk = false ;
        return false ;
    }
    inFile.close() ;
                            ps = string("Java init : JavaSystem::Init") ;	trace(&ps, 1, trSubSteps) ;	//=== Pilot initialisations
                                                    char* propertyFile = NULL ; //by default Pilot.properties
                                                                            if (JavaSystem::Init(this, "pilot.properties", false))
                                                                            {
                                                                                string sErreur = getText("pilotManagement", "cannotInitJavaMachine") ;
                                                                                trace(&sErreur, 1, NSSuper::trError) ;
                                                                                _bJavaOk = false ;
                                                                                return false ;	}

                                                                            ps = string("Java init : Pilot::Init") ;
                                                                                                    trace(&ps, 1, trSubSteps) ;

                                                                                                                            if (propertyFile)
                                                                                                                                ps = string("property file : ") + string(propertyFile) ;
                                                                                                                            else
                                                                                                                                ps = string("property file : NULL") ;
                                                                                                                                                    trace(&ps, 1, trDetails) ;

                                                                                                                                                                            int stat = Pilot::Init(propertyFile) ;
                                                                                                                                                                                                    if (stat < 0)
                                                                                                                                                                                                    {
                                                                                                                                                                                                        string sErreur = getText("pilotManagement", "cannotInitPilot") ;
                                                                                                                                                                                                        trace(&sErreur, 1, NSSuper::trError) ;
                                                                                                                                                                                                        switch (stat)
                                                                                                                                                                                                        {
                                                                                                                                                                                                        case -1  : ps = string("Cannot locate the pilot/Pilot class. Exiting....") ; break ;
                                                                                                                                                                                                        case -2  : ps = string("Cannot locate the setParametersFromFile method. Exiting...") ; break ;
                                                                                                                                                                                                        case -3  : ps = string("Cannot locate the init method. Exiting...") ; break ;
                                                                                                                                                                                                        case -4  : ps = string("jenv is NULL. Exiting...") ; break ;
                                                                                                                                                                                                        default  : ps = string("Unknown error.") ;
                                                                                                                                                                                                        }
                                                                                                                                                                                                        trace(&ps, 1, trError) ;
                                                                                                                                                                                                        _bJavaOk = false ;
                                                                                                                                                                                                        return false ;
                                                                                                                                                                                                    }

                                                                                                                                                                                                    char szInfo[33] ;
                                                                                                                                                                                                                            itoa(stat, szInfo, 10) ;
                                                                                                                                                                                                                                                    ps = string("Pilot init - nb of agents : ") + string(szInfo) ;
                                                                                                                                                                                                                                                                            trace(&ps, 1, trDetails) ;

                                                                                                                                                                                                                                                                                                    //use defaultServiceDirectory
                                                                                                                                                                                                                                                                                                    stat = Pilot::AddServicesFromDirectory(NULL) ;
                                                                                                                                                                                                                                                                                                                            if (!stat)
                                                                                                                                                                                                                                                                                                                            {
                                                                                                                                                                                                                                                                                                                                string sErreur = getText("pilotManagement", "cannotAddServices") ;
                                                                                                                                                                                                                                                                                                                                trace(&sErreur, 1, NSSuper::trError) ;
                                                                                                                                                                                                                                                                                                                            }
                                                                                                                                                                                                                                                                                                                            itoa(stat, szInfo, 10) ;	ps = string("Pilot init - nb of services : ") + string(szInfo) ;
                                                                                                                                                                                                                                                                                                                                                    trace(&ps, 1, trDetails) ;
                                                                                                                                                                                                                                                                                                                                                                            ps = string("Java init : NautilusPilot::Init") ;
                                                                                                                                                                                                                                                                                                                                                                                                    trace(&ps, 1, trSubSteps) ;

                                                                                                                                                                                                                                                                                                                                                                                                                            stat = NautilusPilot::Init() ;
                                                                                                                                                                                                                                                                                                                                                                                                                                                    if (stat)
                                                                                                                                                                                                                                                                                                                                                                                                                                                    {
                                                                                                                                                                                                                                                                                                                                                                                                                                                        itoa(stat, szInfo, 10) ;
                                                                                                                                                                                                                                                                                                                                                                                                                                                        string sErreur = getText("pilotManagement", "cannotInitNautilusPilot") + string(" ") + szInfo ;
                                                                                                                                                                                                                                                                                                                                                                                                                                                        trace(&sErreur, 1, NSSuper::trError) ;

                                                                                                                                                                                                                                                                                                                                                                                                                                                        if ( stat < 0 )
                                                                                                                                                                                                                                                                                                                                                                                                                                                        {
                                                                                                                                                                                                                                                                                                                                                                                                                                                            string sErreur = getText("pilotManagement", "cannotInitPilot") ;
                                                                                                                                                                                                                                                                                                                                                                                                                                                            trace(&sErreur, 1, NSSuper::trError) ;
                                                                                                                                                                                                                                                                                                                                                                                                                                                            switch (stat)
                                                                                                                                                                                                                                                                                                                                                                                                                                                            {
                                                                                                                                                                                                                                                                                                                                                                                                                                                            case -1  : ps = string("Cannot locate the nautilus/NautilusPilot class. Exiting...") ; break ;
                                                                                                                                                                                                                                                                                                                                                                                                                                                            case -2  : ps = string("Cannot locate the nautilus/NautilusGraph class. Exiting...") ; break ;
                                                                                                                                                                                                                                                                                                                                                                                                                                                            case -3  : ps = string("Cannot locate the graphServer/Node class. Exiting...") ; break ;
                                                                                                                                                                                                                                                                                                                                                                                                                                                            case -4  : ps = string("Cannot locate the graphServer/Mapping class. Exiting...") ; break ;
                                                                                                                                                                                                                                                                                                                                                                                                                                                            case -5  : ps = string("Cannot locate the domlightwrapper/DOMElement class. Exiting...") ; break ;
                                                                                                                                                                                                                                                                                                                                                                                                                                                            case -6  : ps = string("Cannot locate the NautilusPilot readGraph method. Exiting...") ; break ;
                                                                                                                                                                                                                                                                                                                                                                                                                                                            case -7  : ps = string("Cannot locate the NautilusPilot writeGraph method. Exiting...") ; break ;
                                                                                                                                                                                                                                                                                                                                                                                                                                                            case -8  : ps = string("Cannot locate the NautilusPilot invokeService method V1. Exiting...") ; break ;
                                                                                                                                                                                                                                                                                                                                                                                                                                                            case -9  : ps = string("Cannot locate the NautilusPilot invokeService method V2. Exiting...") ; break ;
                                                                                                                                                                                                                                                                                                                                                                                                                                                            case -10 : ps = string("Cannot locate the NautilusGraph constructor method. Exiting...") ; break ;
                                                                                                                                                                                                                                                                                                                                                                                                                                                            case -11 : ps = string("Cannot locate the NautilusGraph getTreeId method. Exiting...") ; break ;
                                                                                                                                                                                                                                                                                                                                                                                                                                                            case -12 : ps = string("Cannot locate the NautilusGraph getGraphId method. Exiting...") ; break ;
                                                                                                                                                                                                                                                                                                                                                                                                                                                            case -13 : ps = string("Cannot locate the the java/lang/String class. Exiting...") ; break ;
                                                                                                                                                                                                                                                                                                                                                                                                                                                            case -14 : ps = string("Cannot locate the NautilusPilot init method. Exiting...") ; break ;
                                                                                                                                                                                                                                                                                                                                                                                                                                                            case -15 : ps = string("Cannot get FieldId for mnemo. Exiting...") ; break ;
                                                                                                                                                                                                                                                                                                                                                                                                                                                            case -16 : ps = string("Cannot get FieldId for stack. Exiting...") ; break ;
                                                                                                                                                                                                                                                                                                                                                                                                                                                            case -17 : ps = string("Cannot locate the DOMElement getChildrenElements method. Exiting...") ; break ;
                                                                                                                                                                                                                                                                                                                                                                                                                                                            default  : ps = string("Unknown error.") ;
                                                                                                                                                                                                                                                                                                                                                                                                                                                            }
                                                                                                                                                                                                                                                                                                                                                                                                                                                            trace(&ps, 1, trError) ;
                                                                                                                                                                                                                                                                                                                                                                                                                                                        }
                                                                                                                                                                                                                                                                                                                                                                                                                                                        _bJavaOk = false ;
                                                                                                                                                                                                                                                                                                                                                                                                                                                        return false ;
                                                                                                                                                                                                                                                                                                                                                                                                                                                    }
                                                                                                                                                                                                                                                                                                                                                                                                                                                    _bJavaOk = true ;


                                                                                                                                                                                                                                                                                                                                                                                                                                                                            return true ;
                         }
#endif    // #ifdef FOR_WINDOWS

// -----------------------------------------------------------------------------
// Fonction     : NSSuper::estEgal(string *pChaine, string *pModele)
// -----------------------------------------------------------------------------
// Description  : Teste l'equivalence des deux chaines.
// -----------------------------------------------------------------------------
// Retour       :	true si les chaines sont équivalentes, false sinon
// -----------------------------------------------------------------------------
bool
NSSuper::estEgal(std::string* pChaine, std::string* pModele)
{
    // Cas simple : égalité stricte
    if (*pChaine == *pModele)
        return true ;

    // On compare les deux chaines de gauche à droite, jusqu'à trouver
    // un élément du modèle qui ne se ramène pas à un élément de la chaine
    // par une relation "est un"
    std::string Chaine = *pChaine ;
    std::string Modele = *pModele ;

    // On "simplifie" les chaines en supprimant les redondances
    // (par exemple "ETT/Valve cardiaque/Mitrale" devient "ETT/Mitrale")
    return false ;
}

void
NSSuper::initInstanceCounters()
{
    /*
    // Counters from NautilusPilot.hpp
    NSBasicAttribute::initNbInstance() ;
    NSBasicAttributeArray::initNbInstance() ;
    NSPersonsAttributesArray::initNbInstance() ;

  // counters from nsmanager.h
    NSDataTree::initNbInstance() ;
    NSDataTreeArray::initNbInstance() ;
    NSDataGraph::initNbInstance() ;
    NSObjectGraphManager::initNbInstance() ;
    NSPersonGraphManager::initNbInstance() ;

  // counters from nspatpat.h
    NSPatPathoInfo::initNbInstance() ;
    NSPatPathoArray::initNbInstance() ;
    NSVectPatPathoArray::initNbInstance() ;
*/
}

void
NSSuper::creePilot()
{
    /*
try
{
    std::string ps = std::string("Starting, release : ") + _sNumVersion ;
  trace(&ps, 1) ;

  _pPilot = new NautilusPilot(this) ;

  _pPilot->setEpisodusVersion(_sNumVersion) ;

  _pPilot->initAgentList() ;

  _pPilot->setServiceList() ;
}
catch (...)
{
  erreur("Exception Supervisor::creePilot.", standardError) ;
  return ;
}
*/
}

/*
void
NSContexte::setUser(Person* pUti)
{
    _pUser = pUti ;
}
*/

NSToDoTask::NSToDoTask()
{
    _sWhatToDo = string("") ;
    _sParam1   = string("") ;
    _sParam2   = string("") ;
    _pPointer1 = (void *) 0 ;
    _pPointer2 = (void *) 0 ;
    _deleteP1  = false ;
    _deleteP2  = false ;
}

NSToDoTask::NSToDoTask(NSToDoTask& src)
{
    initFromTask(&src) ;
}

NSToDoTask::~NSToDoTask()
{
    // FIXME
    // en fonction de deleteP1 faire un delete sur pPointer1
    // verifier qu'on passe dans le destructeur reel de l'objet pPointer1
    if (_deleteP1 && _pPointer1)
        delete (void *) _pPointer1 ;

    // en fonction de deleteP2 faire un delete sur pPointer2
    if (_deleteP2 && _pPointer2)
        delete (void *) _pPointer2 ;
}

void
NSToDoTask::sendMe(NSSuper *pSuper, bool bSend)
{
    if (NULL == pSuper)
        return ;

    /* Try to find a non-window specific function
  TMyApp *pNSApplication = pSuper->getApplication() ;

  while (pNSApplication && pSuper->isToDoLocked())
        pNSApplication->PumpWaitingMessages() ;
*/

    pSuper->lockToDo() ;
    pSuper->addToDoTask(this) ;
    pSuper->unlockToDo() ;

    string ps = string("addToDo ") + _sWhatToDo +
            string(" sP1=") + _sParam1 +
            string(" sP2=") + _sParam2 ;
    pSuper->trace(&ps, 1, NSSuper::trSubDetails) ;

    /* Try to find a non-window specific function
    if (bSend)
        pNSApplication->GetMainWindow()->PostMessage(WM_COMMAND, IDM_TODO) ;
*/
}

void
NSToDoTask::sendBB1BBInterface(BB1BBInterfaceForKs* pKsDesc, NSSuper *pSuper, bool bSend, bool bDeleteKsDesc)
{
    if ((NULL == pSuper) || (NULL == pKsDesc))
        return ;

    reinit() ;

    _sWhatToDo = "KS_Archetype" ;
    _pPointer1 = pKsDesc ;
    _deleteP1  = bDeleteKsDesc ;

    sendMe(pSuper, bSend) ;
}

void
NSToDoTask::sendBB1BBDecisionTree(BB1BBInterfaceForKs* pKsDesc, NSSuper *pSuper, bool bSend, bool bDeleteKsDesc)
{
    if ((NULL == pSuper) || (NULL == pKsDesc))
        return ;

    reinit() ;

    _sWhatToDo = "KS_DecisionTree" ;
    _pPointer1 = pKsDesc ;
    _deleteP1  = bDeleteKsDesc ;

    sendMe(pSuper, bSend) ;
}

NSToDoTask&
NSToDoTask::operator=(NSToDoTask& src)
{
    if (this == &src)
        return *this ;

    initFromTask(&src) ;

    return (*this) ;
}

void
NSToDoTask::initFromTask(NSToDoTask *pSrc)
{
    if (NULL == pSrc)
        return ;

    _sWhatToDo = pSrc->_sWhatToDo ;
    _sParam1   = pSrc->_sParam1 ;
    _sParam2   = pSrc->_sParam2 ;
    _pPointer1 = pSrc->_pPointer1 ;
    _pPointer2 = pSrc->_pPointer2 ;

    //
    // Tricky thing: only one object must delete the pointer
    //               we assume it is the new one
    //
    _deleteP1  = pSrc->_deleteP1 ;
    if (_deleteP1)
        pSrc->_deleteP1 = false ;
    _deleteP2  = pSrc->_deleteP2 ;
    if (_deleteP2)
        pSrc->_deleteP2 = false ;
}

void
NSToDoTask::reinit()
{
    _sWhatToDo = string("") ;
    _sParam1   = string("") ;
    _sParam2   = string("") ;

    if (_pPointer1)
    {
        if (_deleteP1)
            delete (void *) _pPointer1 ;
        _pPointer1 = 0 ;
    }

    if (_pPointer2)
    {
        if (_deleteP2)
            delete (void *) _pPointer2 ;
        _pPointer2 = 0 ;
    }

    _deleteP1 = false ;
    _deleteP2 = false ;
}

void
NSSuper::StartDebugConsole(int nWidth, int nHeight, const char* /*pszfname*/)
{
    /*
  if (m_hStdOut)
    return ;

  AllocConsole() ;
  SetConsoleTitle("Debug Window") ;
  m_hStdOut = GetStdHandle(STD_OUTPUT_HANDLE) ;

  COORD co = {(short) nWidth, (short) nHeight } ;
  SetConsoleScreenBufferSize(m_hStdOut, co) ;

  co.X = co.Y = 0 ;
  SetConsoleCursorPosition(m_hStdOut,co) ;
*/
}

bool
NSSuper::typeDocument(std::string sType, CLASSDOCTYPES categ)
{
    bool res = false ;

    DBITBLNAME sensCle = "FLECHE" ;

    switch (categ)
    {
    case isTree         : res = _pFilGuide->VraiOuFaux(sType, string("ES"), string("0CLDA"), sensCle) ; break ;
    case isFile         : res = _pFilGuide->VraiOuFaux(sType, string("ES"), string("0CLDF"), sensCle) ; break ;
    case isCompta       : res = _pFilGuide->VraiOuFaux(sType, string("ES"), string("0CLDC"), sensCle) ; break ;
    case isHTML         : res = _pFilGuide->VraiOuFaux(sType, string("ES"), string("0CLDH"), sensCle) ; break ;
    case isText         : res = _pFilGuide->VraiOuFaux(sType, string("ES"), string("ZTXT0"), sensCle) ; break ;
    case isImage        : res = _pFilGuide->VraiOuFaux(sType, string("ES"), string("ZIMA0"), sensCle) ; break ;
    case isImageFixe    : res = _pFilGuide->VraiOuFaux(sType, string("ES"), string("ZIF00"), sensCle) ; break ;
    case isImageAnimee  : res = _pFilGuide->VraiOuFaux(sType, string("ES"), string("ZIA00"), sensCle) ; break ;
    case isURL          : return false ;
    }
    return res ;
}

void
NSSuper::voidDebugPrintf(TRACETYPE traceLevel, const char *szfmt,...)
{
    if (_iTraceConsole > traceLevel)
        return ;

    char s[300];
    va_list argptr;

    va_start(argptr, szfmt) ;
    /* int cnt = */ vsprintf(s, szfmt, argptr) ;
    va_end(argptr) ;

    /*
  DWORD cCharsWritten ;
  if (m_hStdOut)
  {
    DebugNewLine() ;
    WriteConsole(m_hStdOut, s, strlen(s), &cCharsWritten, NULL) ;
  }
*/
}

void
NSSuper::DebugNewLine()
{
    /*
    CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
  if (! GetConsoleScreenBufferInfo(m_hStdOut, &csbiInfo))
    return ;
  csbiInfo.dwCursorPosition.X = 0 ;

  // If it is the last line in the screen buffer, scroll
  // the buffer up.
  if ((csbiInfo.dwSize.Y - 1) == csbiInfo.dwCursorPosition.Y)
  {
    SMALL_RECT srctWindow ;
    if (csbiInfo.srWindow.Top > 0)
    {
      srctWindow.Top    = -1 ;      // move top up by one row
      srctWindow.Bottom = -1 ;   // move bottom up by one row
      srctWindow.Left   = 0 ;      // no change
      srctWindow.Right  = 0 ;     // no change
      if (! SetConsoleWindowInfo(
          m_hStdOut,      // screen buffer handle
          FALSE,        // deltas, not absolute
          &srctWindow)) // specifies new location
        return ;
    }
  }
  // Otherwise, advance the cursor to the next line.
  else
    csbiInfo.dwCursorPosition.Y++ ;
  if (! SetConsoleCursorPosition(m_hStdOut, csbiInfo.dwCursorPosition))
    return ;
*/
}

/*
void
NSSuper::addToDo(NSToDoTask* pTask, bool bSend)
{
    if (NULL == pTask)
        return ;

  // Try to find a non-window specific function
    while (bToDoLocked)
        pNSApplication->PumpWaitingMessages() ;

    _bToDoLocked = true ;
    _aToDo->push_back(pTask) ;
    _bToDoLocked = false ;

  std::string sWhatToDo ;
  std::string sParam1 ;
  std::string sParam2 ;

  std::string ps = std::string("addToDo ") + pTask->getWhatToDo() +
                             std::string(" sP1=") + pTask->getParam1() +
                   std::string(" sP2=") + pTask->getParam2() ;
    trace(&ps, 1, trSubDetails);

  // Try to find a non-window specific function
    if (bSend)
        pNSApplication->GetMainWindow()->PostMessage(WM_COMMAND, IDM_TODO) ;
}
*/

void
NSContexte::addToDoBbk(NSToDoTask* pTask, bool bSend)
{
    if (NULL == pTask)
        return ;

    /* Try to find a non-window specific function
    while (bBBKToDoLocked)
        pNSApplication->PumpWaitingMessages() ;
*/

    _bBBKToDoLocked = true ;
    _aBBKToDo.push_back(pTask) ;
    _bBBKToDoLocked = false ;

    /* Try to find a non-window specific function
    if (bSend)
        blackboardInterface->PostMessage(WM_COMMAND, IDM_BBKTODO) ;
*/
}

bool
NSContexte::BbkAskUser(std::string sArchetype, int iTypeArc)
{
    try
    {
        /*
    if ((!pNSApplication) || (!(pNSApplication->GetMainWindow())))
        return false ;

    char szType[10] ;
    itoa(iTypeArc, szType, 10) ;

    NSToDoTask* pTask = new NSToDoTask ;

    pTask->sWhatToDo = "Archetype" ;
    pTask->sParam1   = sArchetype ;
    pTask->sParam2   = string(szType) ;

    addToDo(pTask) ;
*/
        return true ;
    }
    catch (...)
    {
        erreur("Exception Supervisor::BbkAskUser.", standardError) ;
        return false ;
    }
}

bool
NSContexte::bbkToDo(int iTypeArc, std::string ssWhatToDo, std::string ssParam1, std::string ssParam2, void* ppPointer1,bool p1, void* ppPointer2,bool p2)
{
    try
    {
        //	if ((!pNSApplication) || (!(pNSApplication->GetMainWindow())))
        //		return false ;

        // char szType[10] ;
        // itoa(iTypeArc, szType, 10) ;

        NSToDoTask* pTask = new NSToDoTask ;

        pTask->setWhatToDo(ssWhatToDo) ;
        pTask->setParam1(ssParam1) ;
        pTask->setParam2(ssParam2) ;
        pTask->setPointer1(ppPointer1) ;
        pTask->setToDeleteP1(p1) ;
        pTask->setPointer2(ppPointer2) ;
        pTask->setToDeleteP2(p2) ;

        addToDoBbk(pTask) ;

        return true ;
    }
    catch (...)
    {
        erreur("Exception Supervisor::bbkToDo.", standardError) ;
        return false ;
    }
}

bool
NSContexte::BbkShowReferential(std::string sReferential)
{
    try
    {
        NSToDoTask* pTask = new NSToDoTask ;

        pTask->setWhatToDo(string("Referential")) ;
        pTask->setParam1(sReferential) ;

        pTask->sendMe(_pSuper) ;

        return true ;
    }
    catch (...)
    {
        erreur("Exception Supervisor::BbkShowReferential.", standardError) ;
        return false ;
    }
}



//
// *************  NSUserPatRelation  *************
//

/*
NSUserPatRelation::NSUserPatRelation()
{
    reinit() ;
}

bool
NSUserPatRelation::isAdministred(int iAngle, int iDistance)
{
    for (int j = 0; j <= iDistance; j++)
    if (1 == adminPosit[iAngle][j])
        return true ;

    return false ;
}

bool
NSUserPatRelation::isPresent(int iAngle, int iDistance)
{
    return (1 == existPosit[iAngle][iDistance]) ;
}

void
NSUserPatRelation::reinit()
{
  bPatientIsTheUser        = false ;
  bUserActiveInHealthTeam  = false ;
  bUserPresentInHealthTeam = false ;
  bUserIsAdministrator     = false ;
  bUserIsRootAdministrator = false ;

  for (int i = 0 ; i < 24 ; i++)
  {
    for (int j = 0; j < 3; j++)
    {
      adminPosit[i][j] = 0 ;
      existPosit[i][j] = 0 ;
    }
  }
}
*/

// void
// NSUserPatRelation::init(NSContexte* /*pContexte*/)
// {
/*
    reinit() ;

    string sUserId 	= pContexte->getUtilisateurID() ;
    NSPatientChoisi*	pPatChoisi = pContexte->getPatient() ;
    string sPatId	= pPatChoisi->pGraphPerson->getPersonID() ;

    bPatientIsTheUser = (sUserId == sPatId) ;


    NSHealthTeam*		pHealthTeam = pPatChoisi->pHealthTeam ;
    NSHealthTeamMember*	pMember = pHealthTeam->getUserAsMember(pContexte) ;
    if (!pMember)
        return ;

    bUserPresentInHealthTeam = true ;

    NSHTMMandateArray aMandates ;
    pMember->getActiveMandates(&aMandates) ;
    if (!(aMandates.empty()))
    {
        bUserActiveInHealthTeam = true ;

        NSHTMMandateIter iter = aMandates.begin() ;
        for (; iter != aMandates.end() ; )
        {
        int						iAngle	= (*iter)->getAngle() ;
      DistanceType	iDist		= (*iter)->getDistance() ;

            NSHealthTeamMandate::mandateType iType	= (*iter)->getType() ;

            int iRayon = -1 ;
            switch(iDist)
            {
                case patDist 	: iRayon = 0 ; break ;
                case nearDist	: iRayon = 1 ; break ;
                case farDist 	: iRayon = 2 ; break ;
            }

            if (iRayon >= 0)
            {
                existPosit[iAngle][iRayon] = 1 ;
                if (iType == NSHealthTeamMandate::root)
                {
                    adminPosit[iAngle][iRayon] = 1 ;
                    bUserIsAdministrator = true ;
                    if (iRayon == 0)
                        bUserIsRootAdministrator = true ;
                }
            }
      aMandates.erase(iter) ;
        }
    }
*/
// }

// --------------------------------------------------------------------------
//                               NSContexte
// --------------------------------------------------------------------------

NSContexte::NSContexte() : _pPerson(NULL), _pUser(NULL), _pSuper(NULL)
{
    _sTransaction = std::string("") ;
    _sToken       = std::string("") ;
}

NSContexte::NSContexte(NSSuper* pSup) : _pPerson(NULL), _pUser(NULL), _pSuper(pSup)
{
    _sTransaction = std::string("") ;
    _sToken       = std::string("") ;
}

NSContexte::NSContexte(NSSuper* pSup, std::string sPersonId) : _pSuper(pSup)
{
    _pPerson = new Person(this) ;
    _pUser   = new Person(this) ;

    _pPerson->setId(sPersonId) ;
    _pUser->setId(sPersonId) ;

    _sTransaction = std::string("") ;
    _sToken       = std::string("") ;
}

NSContexte::NSContexte(NSContexte& rv) : //_pPerson(rv._pPerson),
    //_pUser(rv._pUser),
    _pSuper(rv._pSuper)
{
    _sTransaction = rv._sTransaction ;
    _sToken       = rv._sToken ;
}

bool
NSContexte::Ob1AskUser(BB1BBInterfaceForKs* pKsDesc)
{
    try
    {
        NSToDoTask* pTask = new NSToDoTask ;

        pTask->setWhatToDo(string("KS_Archetype")) ;
        pTask->setPointer1(pKsDesc) ;
        pTask->setToDeleteP1(false) ;

        pTask->sendMe(_pSuper) ;

        return true ;
    }
    catch (...)
    {
        erreur("Exception NSContexte::Ob1AskUser.", standardError) ;
        return false ;
    }
}

NSContexte::PERSONCLASSES
NSContexte::getPersonClass(std::string sPersonID)
{
    if (sPersonID == "")
        return isUnknown ;

    switch(sPersonID[0])
    {
    case '#' : return isMemory ;
    case '_' : return isLocal ;		// Historical value
    case '-' : return isLocal ;
    }

    return isGlobal ;
}

std::string
NSContexte::getPersonID()
{
    if ((Person*) NULL != _pPerson)
        return _pPerson->getId() ;

    return std::string("") ;
}

void
NSContexte::setPersonID(std::string sId)
{
    if ((Person*) NULL != _pPerson)
        _pPerson->setId(sId) ;
}

std::string
NSContexte::getUserID()
{
    if ((Person*) NULL != _pUser)
        return _pUser->getId() ;
    return std::string("") ;
}

void
NSContexte::setUserID(std::string sId)
{
    if ((Person*) NULL != _pUser)
        _pUser->setId(sId) ;
}

string
NSContexte::getUserLanguage()
{
    if (NULL == _pUser)
        return string("") ;
    return _pUser->donneLang() ;
}

bool
NSContexte::userHasPrivilege(RIGHTSTO /*doThat*/, int /*i1*/, int /*i2*/, std::string /*s1*/, std::string /*s2*/, void* /*p1*/, void* /*p2*/)
{
    return true ;

    /*

    string sPatId	= pPatient->pGraphPerson->getPersonID() ;
  PERSONCLASSES iPatClass = getPersonClass(sPatId) ;

  if (iPatClass == isLocal)
  {
    switch(doThat)
    {
        case modifyDocument :
      case suppresDocument :
        if (s1 == "$JustAccessToInterface$")
          return true ;
        return (s1 == getUtilisateurID()) ;
    }
    return true ;
  }
  else
  {
    switch(doThat)
    {
      case openPatient :
          return relationship.bUserPresentInHealthTeam ;
      //
      // Document opening : s1 = document author's ID
      //
      // - in general, must be present
      // - for a given document, if not active, must be the author
      //
      case openDocument :
        if (s1 == "$JustAccessToInterface$")
          return relationship.bUserPresentInHealthTeam ;
        return ((s1 == getUtilisateurID()) || relationship.bUserActiveInHealthTeam) ;
      //
      // Document modification : s1 = document author's ID
      //
      // - in general, must be active
      // - for a given document, must be the author
      //
      case modifyDocument :
      case suppresDocument :
        if (s1 == "$JustAccessToInterface$")
          return relationship.bUserActiveInHealthTeam ;
        return (s1 == getUtilisateurID()) ;
      case createDocument :
      case modifyLdV :
      case viewConcerns :
      case createConcern :
      case modifyConcern :
      case viewDrug :
      case createDrug :
      case modifyDrug :
      case viewGoal :
      case createGoal :
      case modifyGoal :
      case viewProcess :
      case createProcess :
      case modifyProcess :
      case viewSynthesis :
      case modifySynthesis :
      case viewAdminData :
        return relationship.bUserActiveInHealthTeam ;
      case modifyAdminData :
        return relationship.bPatientIsTheUser ;
      case viewProData :
        return true ;
      case modifyProData :
        return relationship.bPatientIsTheUser ;
      case viewHealthTeam :
        return relationship.bUserActiveInHealthTeam ;
      case createHTMember :
      case createMandate :
      case modifyMandate :
        return relationship.bUserIsAdministrator ;
      case createRosace :
      case modifyRosace :
        return true ;
      case createUser :
        return false ;
    }
    return relationship.bUserActiveInHealthTeam ;
  }

*/
}

//***************************************************************************
//                              NSContextArray
//***************************************************************************

NSContextArray::NSContextArray(const NSContextArray& rv)
{
    if (false == rv.empty())
        for (CtxConstIter i = rv.begin() ; rv.end() != i ; i++)
            push_back(new NSContexte(*(*i))) ;
}

NSContextArray::~NSContextArray()
{
    vider() ;
}

NSContexte*
NSContextArray::getContextFromToken(std::string sToken)
{
    if (empty())
        return (NSContexte *) 0 ;

    for (CtxIter i = begin() ; end() != i ; i++)
        if ((*i)->getToken() == sToken)
            return *i ;

    return (NSContexte *) 0 ;
}

NSContexte*
NSContextArray::getContextFromUser(std::string sUserId)
{
    if (empty())
        return (NSContexte *) 0 ;

    for (CtxIter i = begin() ; end() != i ; i++)
        if ((*i)->getUserID() == sUserId)
            return *i ;

    return (NSContexte *) 0 ;
}

NSContexte*
NSContextArray::getContextFromPatient(std::string sPatId)
{
    if (empty())
        return (NSContexte *) 0 ;

    for (CtxIter i = begin() ; end() != i ; i++)
        if ((*i)->getPersonID() == sPatId)
            return *i ;

    return (NSContexte *) 0 ;
}

void
NSContextArray::vider()
{
    if (empty())
        return ;

    for (CtxIter i = begin() ; end() != i ; )
    {
        delete *i ;
        erase(i) ;
    }
}

