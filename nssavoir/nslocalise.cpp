// -----------------------------------------------------------------------------
// GraphicHealthTeam.cpp
// -----------------------------------------------------------------------------
// Implementation des variables de localisation
// -----------------------------------------------------------------------------
// $Revision: 1.8 $
// $Author: pameline $
// $Date: 2012/08/24 10:05:42 $
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// PA  - janvier 2004
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Copyright Nautilus, 2005
// http://www.nautilus-info.com
// -----------------------------------------------------------------------------
// Ce logiciel est un programme informatique servant à gérer et traiter les
// informations de santé d'une personne.
//
// Ce logiciel est régi par la licence CeCILL soumise au droit français et
// respectant les principes de diffusion des logiciels libres. Vous pouvez
// utiliser, modifier et/ou redistribuer ce programme sous les conditions de la
// licence CeCILL telle que diffusée par le CEA, le CNRS et l'INRIA sur le site
// "http://www.cecill.info".
//
// En contrepartie de l'accessibilité au code source et des droits de copie, de
// modification et de redistribution accordés par cette licence, il n'est offert
// aux utilisateurs qu'une garantie limitée. Pour les mêmes raisons, seule une
// responsabilité restreinte pèse sur l'auteur du programme, le titulaire des
// droits patrimoniaux et les concédants successifs.
//
// A cet égard  l'attention de l'utilisateur est attirée sur les risques
// associés au chargement, à l'utilisation, à la modification et/ou au
// développement et à la reproduction du logiciel par l'utilisateur étant donné
// sa spécificité de logiciel libre, qui peut le rendre complexe à manipuler et
// qui le réserve donc à des développeurs et des professionnels avertis
// possédant des connaissances informatiques approfondies. Les utilisateurs sont
// donc invités à charger et tester l'adéquation du logiciel à leurs besoins
// dans des conditions permettant d'assurer la sécurité de leurs systèmes et ou
// de leurs données et, plus généralement, à l'utiliser et l'exploiter dans les
// mêmes conditions de sécurité.
//
// Le fait que vous puissiez accéder à cet en-tête signifie que vous avez pris
// connaissance de la licence CeCILL, et que vous en avez accepté les termes.
// -----------------------------------------------------------------------------
// This software is a computer program whose purpose is to manage and process
// a person's health data.
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


//---------------------------------------------------------------------------
//    NSLOCALISE.CPP
//    PA   janvier 2004
//  Implémentation des variables de localisation
//---------------------------------------------------------------------------

#include <iostream>
#include <fstream>
#include <cstring>

#ifdef _ENTERPRISE_DLL
  #include "enterpriseLus/nsglobalLus.h"
  #include "enterpriseLus/nsdivfctForCgi.h"
  #include "enterpriseLus/superLus.h"
#else
  #include "partage/nsglobal.h"
  #include "partage/nsdivfct.h"
  #include "nautilus/nssuper.h"
#endif

#include "nssavoir/nslocalise.h"


//***************************************************************************
// Implémentation des méthodes NSLocalChapter
//***************************************************************************

// Constructeurs
NSLocalisedChapter::NSLocalisedChapter(NSLocalisedChapter& rv)
{
  aLocalTexts = rv.aLocalTexts ;
}

// Opérateurs
NSLocalisedChapter&
NSLocalisedChapter::operator=(NSLocalisedChapter& src)
{
  if (&src == this)
    return *this ;

  aLocalTexts.clear() ;
  for (std::map<std::string, std::string >::iterator i = src.aLocalTexts.begin(); i != src.aLocalTexts.end(); i++)
    addText((*i).first, (*i).second ) ;

  return *this ;
}

// Destructeur
NSLocalisedChapter::~NSLocalisedChapter()
{
	aLocalTexts.clear() ;
}

void
NSLocalisedChapter::addText(std::string sCod, std::string sTxt)
{
  aLocalTexts[sCod] = sTxt ;
}

std::string
NSLocalisedChapter::getLocalText(std::string sCod)
{
  if ( aLocalTexts.find(sCod) != aLocalTexts.end() )
    return aLocalTexts[sCod] ;
    
  return "" ;
}

//***************************************************************************
// Implémentation des méthodes NSLocalChapterArray
//***************************************************************************

NSLocalChapterArray::NSLocalChapterArray()
{
}

void
NSLocalChapterArray::vider()
{
  if (_chapters.empty())
    return ;

  for (std::map<std::string, NSLocalisedChapter* >::iterator i = _chapters.begin(); _chapters.end() != i ; i++)
  {
    delete (*i).second ;
    (*i).second = NULL ;
  }
  _chapters.clear() ;
}

void
NSLocalChapterArray::addText(string sChap, string sCod, string sTxt)
{
  if ( _chapters.find(sChap) != _chapters.end() )
  {
    _chapters[sChap]->addText(sCod, sTxt) ;
    return ;
  }
  NSLocalisedChapter* pChapter = new NSLocalisedChapter() ;
  pChapter->addText(sCod, sTxt) ;
  _chapters[sChap] = pChapter ;
}

string
NSLocalChapterArray::getLocalText(string sChap, string sCod, bool bSendTextBackIfNotFound)
{
	if ((string("") == sChap) || (string("") == sCod))
  	return string("") ;

  if (_chapters.empty())
  	return string("") ;

  if (_chapters.find(sChap) != _chapters.end())
  	return _chapters[sChap]->getLocalText(sCod) ;

	if (false == bSendTextBackIfNotFound)
		return string("") ;

  for (size_t i = 0 ; i < strlen(sCod.c_str()) ; i++)
  {
    if (isupper(sCod[i]))
    {
      sCod = string(sCod, 0, i) + string(" ") + string(sCod, i, strlen(sCod.c_str()) - i) ;

      if ((i+2 < strlen(sCod.c_str())) && (false == isupper(sCod[i+1])))
      	sCod[i+1] = tolower(sCod[i+1]) ;

      i++ ;
    }
  }
  sCod[0] = pseumaj(sCod[0]) ;

	return sCod ;
}

NSLocalisedChapter*
NSLocalChapterArray::getChapter(string sChapterTitle)
{
  if ((string("") == sChapterTitle) || _chapters.empty())
    return (NSLocalisedChapter*) 0 ;

  NSLocalChapterArrayIter it = _chapters.find(sChapterTitle) ;
  if (_chapters.end() == it)
    return (NSLocalisedChapter*) 0 ;

  return (*it).second ;
}

NSLocalisedChapter*
NSLocalChapterArray::getChapterWhereText(string sText, string sValue)
{
  if ((string("") == sText) || _chapters.empty())
    return (NSLocalisedChapter*) 0 ;

  NSLocalChapterArrayIter it = _chapters.begin() ;
  for ( ; (_chapters.end() != it) && ((*it).second->getLocalText(sText) != sValue) ; it++) ;

  if (_chapters.end() == it)
    return (NSLocalisedChapter*) 0 ;

  return (*it).second ;
}

void
NSLocalChapterArray::init(ifstream * pInFile, string *psErrorMsg)
{
  if ((ifstream*) NULL == pInFile)
  {
    if (psErrorMsg)
      *psErrorMsg = string("null file pointer") ;
    return ;
  }
  if (pInFile->eof())
  {
    if (psErrorMsg)
      *psErrorMsg = string("empty file") ;
    return ;
  }

  string sCurrentChapter = string("") ;
  if (psErrorMsg)
    *psErrorMsg = string("no chapter found") ;

  string sLine ;
  while (!(pInFile->eof()))
  {
    getline(*pInFile, sLine) ;
    strip(sLine) ;

    size_t iLineLen = strlen(sLine.c_str()) ;

#ifdef __linux__
    while ((iLineLen > 0) && (('\n' == sLine[iLineLen-1]) || ('\r' == sLine[iLineLen-1])))
    {
      sLine = string(sLine, 0, iLineLen - 1) ;
      iLineLen-- ;
    }
#endif

    if (string("") != sLine)
    {
      if (iLineLen > 2)
      {
        if (('/' == sLine[0]) && ('/' == sLine[1]))
        {
          // Commentaire
        }
        else if (('[' == sLine[0]) && (']' == sLine[iLineLen-1]))
        {
          // Chapitre
          sCurrentChapter = string(sLine, 1, iLineLen - 2) ;

          if (psErrorMsg)
            *psErrorMsg = string("") ;
        }
        else if (string("") != sCurrentChapter)
        {
          // Valeur
          size_t iRecherche = sLine.find('=') ;
          size_t iRechText = iRecherche + 1 ;

          // on doit éliminer les tabulations
          while (('\t' == sLine[iRechText]) || (' ' == sLine[iRechText]))
            iRechText += 1 ;

          if ((string::npos != iRecherche) && (iRecherche < iLineLen - 1))
          {
            // On sépare la chaine en Nom et Contenu
            //	Ex : "Font1=Times New Roman,120"
            // -> Nom : "Font1" et Contenu = "Times New Roman,120"
            string sChaineNom     = string(sLine, 0, iRecherche) ;
            string sChaineContenu = string(sLine, iRechText, iLineLen - iRechText) ;
            strip(sChaineNom) ;
            strip(sChaineContenu) ;
            addText(sCurrentChapter, sChaineNom, sChaineContenu) ;
          }
        }
      }
    }
  }
}

void
NSLocalChapterArray::init(string sFileName, string *psErrorMsg)
{
  vider() ;

  if (string("") == sFileName)
  {
    if (psErrorMsg)
      *psErrorMsg = string("empty filename") ;
    return ;
  }

  ifstream file ;
  file.open(sFileName.c_str(), ofstream::in) ;
  if (false == file.is_open())
  {
    if (psErrorMsg)
      *psErrorMsg = string("Cannot open file ") + sFileName ;
    return ;
  }

  init(&file, psErrorMsg) ;

  file.close() ;
}

#ifndef _ENTERPRISE_DLL
bool
NSLocalChapterArray::writeToFile(string sFileName, NSSuper *pSuper)
{
  if ((string("") == sFileName) || (NULL == pSuper))
    return false ;

  if (empty())
    return false ;

try
{
  ofstream outFile ;
	outFile.open(sFileName.c_str(), ios::out) ;
  if (!outFile)
  {
  	string sErrorText = pSuper->getText("fileErrors", "errorOpeningOutputFile") ;
    sErrorText += string(" (") + sFileName + string(")") ;
    pSuper->trace(&sErrorText, 1, NSSuper::trError) ;
    erreur(sErrorText.c_str(), standardError) ;
    return false ;
  }

  std::map<std::string, NSLocalisedChapter*>::iterator chapIter = _chapters.begin() ;
  for ( ; _chapters.end() != chapIter ; chapIter++)
  {
    string              sChapterLabel = (*chapIter).first ;
    NSLocalisedChapter* pChapter      = (*chapIter).second ;

    outFile << (string("[") + sChapterLabel + string("]") + string("\n")) ;

    if (false == pChapter->empty())
      for (std::map<std::string, std::string >::iterator i = pChapter->getLocalTexts()->begin() ; pChapter->getLocalTexts()->end() != i ; i++)
        outFile << ((*i).first + string("= ") + (*i).second + string("\n")) ;

    outFile << string("\n") ;
  }

  outFile.close() ;

	return true ;
}
catch (...)
{
  erreur("Exception NSLocalChapterArray::writeToFile.", standardError) ;
  return false ;
}
}
#endif

NSLocalChapterArray::~NSLocalChapterArray()
{
	vider() ;
}

NSLocalChapterArray&
NSLocalChapterArray::operator=(NSLocalChapterArray& rv)
{
try
{
  if (this == &rv)
    return *this ;

  vider() ;

  for (std::map<std::string,NSLocalisedChapter*>::iterator i = rv._chapters.begin(); i != rv._chapters.end(); i++)
    _chapters[ (*i).first] =  new NSLocalisedChapter( *((*i).second)) ;

  return *this ;
}
catch (...)
{
  return *this ;
}
}

///////////////////////// fin de nsperary.cpp ////////////////////////////////

