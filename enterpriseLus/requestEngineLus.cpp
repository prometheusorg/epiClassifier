//---------------------------------------------------------------------------
//    requestEngineLus.cpp
//    PA  octobre 2011
//    Request Engine model
//---------------------------------------------------------------------------

#include <iostream>
#include <stdio.h>
#include <fstream>
#include <string>
#include <sstream>
#include <cstring>
#include <nspr/prtime.h>

// #include "enterpriseLus/nsglobalLus.h"
#include "nssavoir/nslocalise.h"

#include "enterpriseLus/nsdivfctForCgi.h"

#include "enterpriseLus/superLus.h"
#include "enterpriseLus/requestEngineLus.h"

using namespace std ;

//***************************************************************************
//                           RequestEngine
//***************************************************************************

//---------------------------------------------------------------------------
// Constructor
//---------------------------------------------------------------------------
RequestEngine::RequestEngine()
{
    _iTrace = trDetails ;

    _sEnterprisePort = std::string("") ;
    _iEnterprisePort = 0 ;

    _pSocketListener = (dti::sockets::tcp_listener*) 0 ;
}

RequestEngine::~RequestEngine()
{
}

void RequestEngine::mainLoop()
{
    std::string sMsg = std::string("Entering main loop") ;
    trace(&sMsg, 1, trSubSteps) ;

    if ((dti::sockets::tcp_listener*) NULL == _pSocketListener)
    {
        std::string sError = std::string("Main loop: no socket listener, leaving") ;
        trace(&sError, 1, trError) ;
        return ;
    }

    std::string sInput = std::string("") ;

    bool bListen = true ;
    while (bListen)
    {
        try
        {
            std::string sReceivedInfo = std::string("") ;

            dti::sockets::tcp_socket socket = _pSocketListener->accept() ;
            int iSize = socket.recv(sReceivedInfo) ;
            if (iSize > 0)
            {
                trace(&sReceivedInfo, 1, trSteps) ;
                sInput += sReceivedInfo ;
            }

            std::string sOutput = processCommand(sInput) ;
            sInput = std::string("") ;

            if (std::string("") != sOutput)
                socket.send(sOutput + std::string("\n")) ;

            socket.send(std::string("bye")) ;
        }
        catch (dti::sockets::sock_error_ex& e)
        {
            std::string sError = std::string("Exception tcp_listener::recv(): ") + e.what() ;
            sError += std::string(" Error number: ") + std::to_string(e.whaterr()) ;
            sError += std::string(" Error text: ") + e.whatsystext() ;
            trace(&sError, 1, trError) ;
            return ;
        }
        catch (...)
        {
            std::string sError = std::string("Exception tcp_listener::recv().") ;
            trace(&sError, 1, trError) ;
            return ;
        }
    }

    sMsg = std::string("Leaving main loop") ;
    trace(&sMsg, 1, trSubSteps) ;
}

string
RequestEngine::processCommand(std::string sInput)
{
    if (string("") == sInput)
        return std::string("") ;

    ClasseStringVector headerParts ;
    DecomposeChaine(&sInput, &headerParts, string("\r\n")) ;

    if (headerParts.empty())
        return string("") ;

    iterClassString itS = headerParts.begin() ;
    string sFirstLine = (*itS)->getItem() ;
    size_t iFirstLineSize = strlen(sFirstLine.c_str()) ;

    size_t iBlank = sFirstLine.find(" ") ;
    if (string::npos == iBlank)
        return string("") ;

    string sCommand = string(sFirstLine, 0, iBlank) ;
    if (string("GET") != sCommand)
        return string("") ;

    while ((' ' == sFirstLine[iBlank]) && (iBlank < iFirstLineSize))
        iBlank++ ;

    size_t iSecondBlank = sFirstLine.find(" ", iBlank) ;
    if (string::npos == iSecondBlank)
        return string("") ;

    string sURL = std::string(sFirstLine, iBlank, iSecondBlank - iBlank) ;

    return processURL(sURL) ;
}

std::string RequestEngine::processURL(std::string sQuery)
{
    std::cout << "std::string RequestEngine::processURL(std::string sQuery)" << std::endl;
    std::cout << "sQuery: " << sQuery << std::endl;

    if (std::string("") == sQuery)
        return std::string("") ;

    string toRemove = std::string("/?");
    if (toRemove.compare(0, toRemove.length(), sQuery)) {
        try {
            sQuery = sQuery.substr(2);
        } catch (std::out_of_range&) {
            //oops str is too short!!!
        }
    }

    std::cout << "sQuery after trim: " << sQuery << std::endl;

    map<std::string, std::string> EnvironmentArray ;
    ParseUrlQuery(&sQuery, &EnvironmentArray) ;

    for(auto elem : EnvironmentArray)
    {
        std::cout << elem.first << " " << elem.second << "\n";
    }

    return processCommands(&EnvironmentArray) ;
}

void RequestEngine::Init(std::string sInitFileName)
{
    std::string sMsg = std::string("Entering RequestEngine initialization") ;
    trace(&sMsg, 1, trDetails) ;

    LoadInitParams(sInitFileName) ;
    initSocketManager() ;
}

void RequestEngine::LoadInitParams(std::string sInitFileName)
{
    std::string sMsg = std::string("Entering params loading function") ;
    trace(&sMsg, 1, trSubDetails) ;

    if (std::string("") == sInitFileName)
    {
        std::string sError = std::string("Empty params filename, leaving") ;
        trace(&sError, 1, trError) ;
        return ;
    }

    NSLocalChapterArray localChap ;

    std::string sErrorMsg = std::string("") ;
    localChap.init(sInitFileName, &sErrorMsg) ;

    if (localChap.empty())
    {
        std::string sError = std::string("Error when loading params from file ") + sInitFileName ;
        if (std::string("") != sErrorMsg)
            sError += std::string(" (") + sErrorMsg + std::string(")") ;
        sError += std::string(", leaving") ;
        trace(&sError, 1, trError) ;
        return ;
    }

    // Get socket information
    //
    _sEnterprisePort = localChap.getLocalText("EnterpriseSockets", "port") ;
    _iEnterprisePort = StringToInt(_sEnterprisePort) ;

    sMsg = std::string("Socket port: ") + _sEnterprisePort ;
    trace(&sMsg, 1, trSubDetails) ;

    // Get trace information
    //
    _iTrace = trDetails ;

    std::string sTraceLevel = localChap.getLocalText("Traces", "traceLevel") ;

    sMsg = std::string("Switching to trace level: ") + sTraceLevel ;
    trace(&sMsg, 1, trSubDetails) ;

    if ((std::string("") != sTraceLevel) && (isdigit(sTraceLevel[0])))
    {
        std::string sRealTraceLevel = string(1, sTraceLevel[0]) ;
        int iLevel = StringToInt(sRealTraceLevel) ;
        switch(iLevel)
        {
        case 0  : _iTrace = trError ;      break ;
        case 1  : _iTrace = trWarning ;    break ;
        case 2  : _iTrace = trSteps ;      break ;
        case 3  : _iTrace = trSubSteps ;   break ;
        case 4  : _iTrace = trDetails ;    break ;
        case 5  : _iTrace = trSubDetails ; break ;
        default :
            if (iLevel > 5)
                _iTrace = trSubDetails ;
            else
                _iTrace = trNone ;
        }
    }
}

void RequestEngine::initSocketManager()
{
    std::string sMsg = std::string("Entering socket initialization") ;
    trace(&sMsg, 1, trSubDetails) ;

    if (0 == _iEnterprisePort)
    {
        std::string sError = std::string("Socket port not specified, leaving") ;
        trace(&sError, 1, trError) ;
        return ;
    }

    _pSocketListener = new dti::sockets::tcp_listener(_iEnterprisePort) ;
}

//
// Fonction trace
//----------------------------------------------------------------------------
void RequestEngine::trace(std::string pTracePtr[], int nbString, TRACETYPE iTraceLevel)
{
    try
    {
        if (iTraceLevel > _iTrace)
            return ;

        /*
    struct  date dateSys;
    struct  time heureSys;
*/
        char    msg[255] ;

        if (!pTracePtr || (pTracePtr[0] == ""))
            return ;

        ofstream outFile;
        if (!outFile.is_open()) {
            outFile.open("traceEngine.inf", ios::out | ios::app);
        }

        if (outFile.is_open()) { std::cout << "traceEngine.inf open" << std::endl; }

        // calcul de l'heure systeme - get system time
        PRTime         now = PR_Now() ;
        PRExplodedTime et_tmp ;
        PR_ExplodeTime(now, PR_GMTParameters, &et_tmp) ;
        sprintf(msg, "%02d/%02d/%4d - %02d:%02d:%02d,%02d>", et_tmp.tm_mday,
                et_tmp.tm_month+1,
                et_tmp.tm_year,
                et_tmp.tm_hour,
                et_tmp.tm_min,
                et_tmp.tm_sec,
                et_tmp.tm_usec / 1000) ;

        /*
    getdate(&dateSys) ;
    gettime(&heureSys) ;
    sprintf(msg, "%02d/%02d/%4d - %02d:%02d:%02d,%02d>", dateSys.da_day, dateSys.da_mon,
                dateSys.da_year, heureSys.ti_hour, heureSys.ti_min,
                heureSys.ti_sec, heureSys.ti_hund) ;
*/

        int iNbre = nbString ;
        if (iNbre == -1)
            iNbre = 50 ;

        outFile << std::string(msg) ;

        for (int i = 0; (i < iNbre) && (pTracePtr[i] != ""); i++)
        {
            for (int j = 0; j < iTraceLevel; j++)
                outFile << std::string("\t") ;
            outFile << pTracePtr[i] ;
        }

        outFile << std::string("\n") ;
        outFile.close() ;
    }
    catch (...)
    {
    }
}

//////////////////////// ending requestEngineLus.cpp //////////////////////////

