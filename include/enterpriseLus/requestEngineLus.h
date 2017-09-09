// -----------------------------------------------------------------------------
// requestEngineLus.h
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
// Request Engine model
// -----------------------------------------------------------------------------
// PA  - 10/2011
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Copyright Philippe Ameline, 2011
// -----------------------------------------------------------------------------
// Ce logiciel est un programme informatique servant a [rappeler les
// caractéristiques techniques de votre logiciel].
//
// Ce logiciel est regi par la licence CeCILL soumise au droit francais et
// respectant les principes de diffusion des logiciels libres. Vous pouvez
// utiliser, modifier et/ou redistribuer ce programme sous les conditions de la
// licence CeCILL telle que diffusee par le CEA, le CNRS et l'INRIA sur le site
// "http://www.cecill.info".
//
// En contrepartie de l'accessibilite au code source et des droits de copie, de
// modification et de redistribution accordes par cette licence, il n'est offert
// aux utilisateurs qu'une garantie limitee. Pour les memes raisons, seule une
// responsabilite restreinte pese sur l'auteur du programme, le titulaire des
// droits patrimoniaux et les concedants successifs.
//
// A cet egard  l'attention de l'utilisateur est attiree sur les risques
// associes au chargement, a l'utilisation, a la modification et/ou au
// developpement et a la reproduction du logiciel par l'utilisateur etant donne
// sa specificite de logiciel libre, qui peut le rendre complexe a manipuler et
// qui le reserve donc a des developpeurs et des professionnels avertis
// possedant des connaissances informatiques approfondies. Les utilisateurs sont
// donc invites a charger et tester l'adequation du logiciel a leurs besoins
// dans des conditions permettant d'assurer la securite de leurs systemes et ou
// de leurs donnees et, plus generalement, a l'utiliser et l'exploiter dans les
// memes conditions de securite.
//
// Le fait que vous puissiez acceder a cet en-tete signifie que vous avez pris
// connaissance de la licence CeCILL, et que vous en avez accepte les termes.
// -----------------------------------------------------------------------------
// This software is a computer program whose purpose is to [describe
// functionalities and technical features of your software].
//
// This software is governed by the CeCILL  license under French law and abiding
// by the rules of distribution of free software. You can use, modify and/ or
// redistribute the software under the terms of the CeCILL license as circulated
// by CEA, CNRS and INRIA at the following URL "http://www.cecill.info".
//
// As a counterpart to the access to the source code and  rights to copy, modify
// and redistribute granted by the license, users are provided only with a
// limited warranty and the software's author, the holder of the economic
// rights, and the successive licensors have only limited liability.
//
// In this respect, the user's attention is drawn to the risks associated with
// loading, using, modifying and/or developing or reproducing the software by
// the user in light of its specific status of free software, that may mean that
// it is complicated to manipulate, and that also therefore means that it is
// reserved for developers and experienced professionals having in-depth
// computer knowledge. Users are therefore encouraged to load and test the
// software's suitability as regards their requirements in conditions enabling
// the security of their systems and/or data to be ensured and, more generally,
// to use and operate it in the same conditions as regards security.
//
// The fact that you are presently reading this means that you have had
// knowledge of the CeCILL license and that you accept its terms.
// -----------------------------------------------------------------------------

#ifndef __REQUESTENGINELUS_H
#define __REQUESTENGINELUS_H

# if defined(_DANSENGINEDLL)
#  define _CLASSEENGINE __export
# else
#  define _CLASSEENGINE __import
# endif

# include "partage/nsvector.h"
# include "nautilus/nsanxary.h"

// -----------------------------------------------------------------------------
//
//                                Person
//
// -----------------------------------------------------------------------------
// A person...
// -----------------------------------------------------------------------------

class RequestEngine
{
  public:

    enum TRACETYPE { trNone = -1, trError = 0, trWarning, trSteps, trSubSteps, trDetails, trSubDetails } ;

  protected:

	  // Information storage strings
    //
    TRACETYPE   _iTrace ;

    std::string _sEnterprisePort ;
    int         _iEnterprisePort ;

    dti::sockets::tcp_listener* _pSocketListener ;

  public:

	  RequestEngine() ;
    ~RequestEngine() ;

    void   mainLoop() ;
    std::string processCommand(std::string sInput) ;
    std::string processURL(std::string sQuery) ;

    virtual std::string processCommands(std::map<std::string, std::string> *pCommands) = 0 ;

    void   Init(std::string sInitFileName) ;
    void   LoadInitParams(std::string sInitFileName) ;
    void   initSocketManager() ;

    void   trace(std::string pTracePtr[], int nbString, TRACETYPE iTraceLevel) ;

    TRACETYPE getTraceLevel() { return _iTrace ; }
} ;

#endif // __REQUESTENGINELUS_H

