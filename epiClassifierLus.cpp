#include <mysql/mysql.h>

// #include <string>

// using std::string ;

// using namespace std ;

#include "enterpriseLus/superLus.h"
#include "enterpriseLus/epiClassifierLus.h"
#include "nsepisod/nsclasser.h"
#include "nsepisod/eptables.h"

#include <iostream>

int main(int argc, char* argv[])
{
    std::cout << "main\n";

    NSSuper NSSuperviseur(3);
    NSSuperviseur.setTraceLevel(NSSuper::trSubDetails);

    string sTr = string("Starting epiClassifier version ") + NSSuperviseur._sNumVersion ;
    NSSuperviseur.trace(&sTr, 1, NSSuper::trSteps) ;

    NSSuperviseur.initInstanceCounters() ;
    NSSuperviseur.InitPointers() ;

    if (false == NSSuperviseur.InitDatabase())
        return -1;

    ClassifierEngine engine(&NSSuperviseur) ;

    engine.Init(string("enterpriseLus.ini")) ;
    /*
    std::string sArg;
    if (argc > 1) {
        sArg = argv[1];
        std::cout << sArg;
    } else {
        sArg = "";
    }

    if (!(sArg.empty())) {
        std::map<string, string> *pCommands = new std::map<string, string>;
        (*pCommands)["askfor"] = string("getCodesFromText");
        (*pCommands)["text"] = sArg;
        std::cout << engine.processCommands(pCommands);
    }
*/
    engine.mainLoop() ;

    string sTrace = string("Closing epiClassifier") ;
    engine.trace(&sTrace, 1, ClassifierEngine::trSteps) ;

    return 0 ;
}

ClassifierEngine::ClassifierEngine(NSSuper* pSuper)
    :RequestEngine()
{
    _pSuper = pSuper;
}

ClassifierEngine::~ClassifierEngine()
{
}

string ClassifierEngine::processCommands(map<string, string> *pCommands)
{
    std::cout << "inside function "
                 "string ClassifierEngine::processCommands(map<string, string> *pCommands)";

    string sResult = string("<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\r\n") ;

    if (NULL == pCommands)
        return sResult + string("<error code=\"EPI_ERR_BAD_INPUT_POINTER\" />") ;

    string sWhatToDo = (*pCommands)[string("askfor")] ;
    if (string("") == sWhatToDo)
        return sResult + string("<error code=\"EPI_ERR_MISSING_COMMAND\" />") ;

    std::cout << "Connecting database\n";
    ontologyBaseManager* pDbManager = _pSuper->getDatabaseManager() ;
    if ((ontologyBaseManager*) NULL == pDbManager)
    {
        string sTrace = string("No database manager, cannot process command") ;
        trace(&sTrace, 1, trError) ;
        return string("") ;
    }
    if (false == pDbManager->openBase())
    {
        std::cout << "Cannot open database, cannot process command\n";
        if (pDbManager) {
            if (!pDbManager->getMysqlError().empty()) {
                std::cout << pDbManager->getMysqlError();
                string sTrace = string("Cannot open database, cannot process command (") + pDbManager->getMysqlError() +string(")") ;
                trace(&sTrace, 1, trError) ;
                return string("");
            }
        }
    }

    if (string("getCodesFromText") == sWhatToDo)
    {
        std::cout << "(\"getCodesFromText\") == sWhatToD \n";

        string sText = (*pCommands)[string("text")] ;
        if (string("") == sText)
        {
            pDbManager->closeBase() ;
            return sResult + string("<error code=\"EPI_ERR_EMPTY_INPUT\" />") ;
        }

        string sLanguage  = string("") ;
        string sClassif   = string("") ;
        string sPatInfos  = string("") ;
        string sSoapSlot  = string("") ;

        sLanguage  = (*pCommands)[string("lang")] ;
        sClassif   = (*pCommands)[string("classif")] ;
        sPatInfos  = (*pCommands)[string("patinfo")] ;
        sSoapSlot  = (*pCommands)[string("soapslot")] ;

        string sTrace = string("Entering getCodesFromText") ;
        trace(&sTrace, 1, trSteps) ;

        /*
  long lStElemSetObjectCount          = ElemSet::getNbInstance() ;
  long lStIElemSetArrayObjectCount    = ElemSetArray::getNbInstance() ;
  long lStTermsParserObjectCount      = TermsParser::getNbInstance() ;
  long lStParseElemObjectCount        = ParseElem::getNbInstance() ;
  long lStParseElemArrayObjectCount   = ParseElemArray::getNbInstance() ;
  long lStClassifElemObjectCount      = ClassifElem::getNbInstance() ;
  long lStClassifElemArrayObjectCount = ClassifElemArray::getNbInstance() ;
*/

        // Traces for inputs
        //
        trace(&sLanguage,  1, trSteps) ;
        trace(&sText,      1, trSteps) ;
        trace(&sClassif,   1, trSteps) ;
        trace(&sPatInfos,  1, trSteps) ;
        trace(&sSoapSlot,  1, trSteps) ;

        if (string("") == sLanguage)
            sLanguage = string("fr") ;

        if (string("") == sClassif)
            sClassif = string("6CISP") ;

        string sResO = string("") ;
        string sResP = string("") ;
        string sResI = string("") ;
        string sRes3 = string("") ;

        sTrace = string("Creating the parser") ;
        trace(&sTrace, 1, trSubSteps) ;

        NSContexte localContext(_pSuper) ;
        classifExpert ClassifExpert(&localContext) ;
        _pSuper->setClassifExpert(&ClassifExpert) ;

        ParseSOAP SOAPParser(&localContext, &sClassif) ;

        sTrace = string("Computing") ;
        trace(&sTrace, 1, trSubSteps) ;

        string sChemin = string("") ;
        SOAPParser.computeParsing(&sText, &sChemin, &sResO, &sResP, &sResI, &sRes3) ;

        string sParsingResult = sResP ;

        sTrace = string("Parsing result: ") + sParsingResult ;
        trace(&sTrace, 1, trSubSteps) ;

        // Limit output to valid codes for context
        //
        if (string("") != sParsingResult)
        {
            classifExpert cExpert(&localContext) ;

            string sDomain = sParsingResult ;
            ParseCategory Parser(cExpert.donneCodeSize(sClassif), sClassif, cExpert.donnePattern(sClassif)) ;
            ElemSetArray* pElemDomain = Parser.DefDomain(sDomain) ;
            if (pElemDomain)
            {
                //
                // Finding the codes that belong to the domain
                // (Fr On trouve les codes qui correspondent au domaine)
                //
                NSEpiClassifInfoArray ArrayClassif ;
                string sCase = string("") ;
                switch(sSoapSlot[0])
                {
                case 'S' : sCase = string("0SOA01") ; break ;
                case 'O' : sCase = string("0SOA21") ; break ;
                case 'A' : sCase = string("0SOA41") ; break ;
                case 'P' : sCase = string("0SOA34") ; break ;
                }

                cExpert.fillList(sClassif, pElemDomain, &ArrayClassif, sCase) ;
                delete pElemDomain ;

                if (false == ArrayClassif.empty())
                {
                    sResult += string("<result>Ok</result>") ;

                    sResult += string("<output>") ;
                    sResult += string("<codes>") ;

                    for (NSEpiClassifInfoIter it = ArrayClassif.begin() ; ArrayClassif.end() != it ; it++)
                        sResult += string("<code>") + (*it)->getCode() + string("</code>") ;

                    sResult += string("</codes>") ;
                    sResult += string("</output>") ;

                    sTrace = string("Writing final result: ") + sResult ;
                    trace(&sTrace, 1, trSubSteps) ;
                }
            }
        }

        sTrace = string("Leaving getCodesFromText (no error)") ;
        trace(&sTrace, 1, trSteps) ;

        /*
  long lFiElemSetObjectCount          = ElemSet::getNbInstance() ;
  long lFiElemSetArrayObjectCount     = ElemSetArray::getNbInstance() ;
  long lFiTermsParserObjectCount      = TermsParser::getNbInstance() ;
  long lFiParseElemObjectCount        = ParseElem::getNbInstance() ;
  long lFiParseElemArrayObjectCount   = ParseElemArray::getNbInstance() ;
  long lFiClassifElemObjectCount      = ClassifElem::getNbInstance() ;
  long lFiClassifElemArrayObjectCount = ClassifElemArray::getNbInstance() ;
*/

        pDbManager->closeBase() ;

        return sResult ;
    }
    /*
  else if (string("getSessionId") == sWhatToDo)
  {
    string sResult = getTokenFromTraits(pSuper, pCommands) ;
    if (string("") != sResult)
    {
      sResult = string("<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\r\n") + sResult ;
      return sResult ;
    }
  }
  else if (string("getPseudo") == sWhatToDo)
  {
    string sResult = getPseudo(pSuper, pCommands) ;
    if (string("") != sResult)
    {
      sResult = string("<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\r\n") + sResult ;
      return sResult ;
    }
  }
  else if (string("getDemographics") == sWhatToDo)
  {
    string sToken = (*pCommands)[string("token")] ;
    if (string("") == sToken)
      return string("") ;

    string sResult = getDemographics(pSuper, pCommands, sToken) ;
    if (string("") != sResult)
    {
      sResult = string("<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\r\n") + sResult ;
      return sResult ;
    }
  }
*/
    /*
  else if (string("createNewPerson") == sWhatToDo)
  {
    string sToken = (*pCommands)[string("token")] ;
    if (string("") == sToken)
      return string("") ;

    string sResult = createPerson(pSuper, pCommands, sToken) ;
    if (string("") != sResult)
    {
      sResult = string("<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\r\n") + sResult ;
      return sResult ;
    }
  }
*/

    pDbManager->closeBase() ;

    return string("") ;
}

