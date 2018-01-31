// -----------------------------------------------------------------------------
// nsdivfct.cpp
// -----------------------------------------------------------------------------
// Pot-pourri de fonctions
// -----------------------------------------------------------------------------
// $Revision: 1.2 $
// $Author: pameline $
// $Date: 2009/07/23 08:57:59 $
// -----------------------------------------------------------------------------
// PA  - 30 juin 1988
// PA  - 28 juillet 1994
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
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <math.h>

#include <string>
#include <fstream>
#include <regex>

#include "enterpriseLus/nsdivfctForCgi.h"

using std::ofstream;
using std::ifstream;
using namespace std;

int StringToInt(string sValeur)
{
    return (string("") != sValeur) ? atoi(sValeur.c_str()) : 0;
}

// -----------------------------------------------------------------------------
// Function : Transforme un int en string
// -----------------------------------------------------------------------------
string IntToString(int val)
{
    char temp[33];// was itoa(val, temp, 10) but itoa is non-standard; better use sprintf
    sprintf(temp, "%d", val);
    string result(temp);
    return result;
}

// -----------------------------------------------------------------------------
// void strip(string& s, int n)
// Fonction : enleve les blancs d'une string
// Retour   : rien
// -----------------------------------------------------------------------------
void strip(string& s, STRIPTYPE n, char c)
{
    if (string("") == s)
        return;
    size_t pos1 = s.find_first_not_of(c);
    if (string::npos == pos1)
    {
        s = "";
        return;
    }
    size_t pos2;
    switch (n)
    {
    case stripBoth:
        if (pos1 > 0)
            s = string(s, pos1, strlen(s.c_str()) - pos1);
        pos2 = s.find_last_not_of(c);
        if (pos2 != string::npos)
            s = string(s, 0, pos2 + 1);
        break;
    case stripLeft :
        if (pos1 > 0)
            s = string(s, pos1, strlen(s.c_str()) - pos1);
        break;
    case stripRight:
        pos2 = s.find_last_not_of(c);
        if (pos2 != string::npos)
            s = string(s, 0, pos2 + 1);
        break;
    }
}

void strip(string& s, string sM, STRIPTYPE n)
{
    if ((string("") == s) || (string("") == sM))
        return;
    size_t pos1 = s.find_first_not_of(sM);
    if (string::npos == pos1)
    {
        s = string("");
        return;
    }
    size_t pos2;
    switch (n)
    {
    case stripBoth:
        if (pos1 > 0)
            s = string(s, pos1, strlen(s.c_str()) - pos1);
        pos2 = s.find_last_not_of(sM);
        if (string::npos != pos2)
            s = string(s, 0, pos2 + 1);
        break;
    case stripLeft:
        if (pos1 > 0)
            s = string(s, pos1, strlen(s.c_str()) - pos1);
        break;
    case stripRight:
        pos2 = s.find_last_not_of(sM);
        if (string::npos != pos2)
            s = string(s, 0, pos2 + 1);
        break;
    }
}
// -----------------------------------------------------------------------------
// Function   : unsigned int pseumaj(lettre)
// Decription : returns corresponding uppercase without accents/diacritics
// Return     : char
// Created on 13/10/1988 - last update on 28/07/1994
// -----------------------------------------------------------------------------
char pseumaj(char lettre)
{
    if ((lettre >= 'a') && (lettre <= 'z'))
        return char(lettre - 32) ;
    switch (lettre)
    {
    case (unsigned) 'À' :
    case (unsigned) 'Â' :
    case (unsigned) 'Ä' :
    case (unsigned) 'à' : //226
    case (unsigned) 'â' :
    case (unsigned) 'ã' :
    case (unsigned) 'ä' :
    case (unsigned) 'å' :
    case (unsigned) 'æ' :
    case (unsigned) 'á' : return('A') ;
    case (unsigned) 'É' :
    case (unsigned) 'È' :
    case (unsigned) 'Ê' :
    case (unsigned) 'Ë' :
    case (unsigned) 'é' :
    case (unsigned) 'è' :
    case (unsigned) 'ê' :
    case (unsigned) 'ë' : return('E') ;
    case (unsigned) 'Î' :
    case (unsigned) 'Ï' :
    case (unsigned) 'î' :
    case (unsigned) 'í' :
    case (unsigned) 'ï' : return('I') ;
    case (unsigned) 'Ö' :
    case (unsigned) 'Ô' :
    case (unsigned) 'ò' :
    case (unsigned) 'ó' :
    case (unsigned) 'õ' :
    case (unsigned) 'ö' :
    case (unsigned) 'ô' : return('O') ;
    case (unsigned) 'Ù' :
    case (unsigned) 'Ü' :
    case (unsigned) 'Û' :
    case (unsigned) 'ù' :
    case (unsigned) 'ú' :
    case (unsigned) 'ü' :
    case (unsigned) 'û' : return('U') ;
    case (unsigned) 'Ç' :
    case (unsigned) 'ç' : return('C') ;
    }
    return (lettre) ;
}

// -----------------------------------------------------------------------------
// Function    : erreur(Pchar texte, int gravite, Retcode ErrPx)
// Arguments   :	texte   -> texte affiché en cas d'erreur.
//            		gravité -> niveau de gravité.
// Description : Affiche les messages d'erreur et sort si le niveau de gravité est 1.
// Returns     : Retourne 0 si le problème rencontré est surmontable.
// -----------------------------------------------------------------------------
int erreur(const char *texte, ERRORTYPE iSeverity)
{
    // NOTE : Si parent == NULL (cas par défaut)
    // La MessageBox ne sera pas modale !!!
    // Comme on ne peut accéder au contexte, il faut passer un GetHandle()
    // pour ce paramètre à l'appel de la fonction.
    // Attention GetMainWindow()->GetHandle() ne vaut pas NULL...

    if ('\0' == texte[0])
        return(0);

    fileError(texte);

    if (fatalError == iSeverity)
        exit(1);

    return 0;
}

void fileError(string sError)
{
    ofstream outFile;

    // On écrit le nouveau fichier reqerror.log
    outFile.open("ferror.log", ios::app);
    if (!outFile)
        return;

    outFile << sError;
    outFile << string("\n");

    outFile.close();
}

void get_current_date(string* sDate)
{
    if (NULL == sDate)
        return;

    *sDate = donne_date_duJour();
}

int get_age(string *datex, string *dateNaiss)
{
    if ((NULL == datex) || (NULL == dateNaiss))
        return -1;
    if ((strlen(datex->c_str()) != 8) || (strlen(dateNaiss->c_str()) != 8))
        return -1;

    return donne_age(*datex, *dateNaiss);
}

void donne_date_duJour(char* date)
{
    if (NULL == date)
        return;

    int	i;
    struct tm *tp;
    long t;
    char mois[12][4];
    //= { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };

    strcpy(mois[0], "Jan");
    strcpy(mois[1], "Feb");
    strcpy(mois[2], "Mar");
    strcpy(mois[3], "Apr");
    strcpy(mois[4], "May");
    strcpy(mois[5], "Jun");
    strcpy(mois[6], "Jul");
    strcpy(mois[7], "Aug");
    strcpy(mois[8], "Sep");
    strcpy(mois[9], "Oct");
    strcpy(mois[10],"Nov");
    strcpy(mois[11],"Dec");

    char temp[26], month[4], numero[3] = "  ";

    // Initialisation de date
    strcpy(date, "");

    // Prise de temp
    time(&t);
    tp = localtime(&t);
    strcpy(temp, asctime(tp));

    // asctime renvoit la date au format : Sun Sep 16 01:03:52 1973\n\0
    // Récupération de l'année
    for(i = 20; i < 24; i++)
        date[i-20] = temp[i];
    date[4] ='\0';

    // Récupération du mois
    for (i = 4; i < 7; i++)
        month[i-4] = temp[i];
    month[3] = '\0';
    for (i = 0; (i < 12) && (strcmp(month, mois[i]) != 0); i++)
       ;
    i++;
    if (i < 10)
    {
        numero[0] = '0';
        numero[1] = char (i + (int) '0');
    }
    else if (i < 13)
    {
        numero[0] = '1';
        numero[1] = char ((i - 10) + (int) '0');
    }
    strcat(date, numero);

    // Récupération du jour
    for (i = 8; i < 10; i++)
        numero[i - 8] = temp[i];
    numero[2] = '\0';
    strcat(date, numero);
    return;
}

string donne_date_duJour()
{
    string		sMois[12];

    sMois[0]  = string("Jan");
    sMois[1]  = string("Feb");
    sMois[2]  = string("Mar");
    sMois[3]  = string("Apr");
    sMois[4]  = string("May");
    sMois[5]  = string("Jun");
    sMois[6]  = string("Jul");
    sMois[7]  = string("Aug");
    sMois[8]  = string("Sep");
    sMois[9]  = string("Oct");
    sMois[10] = string("Nov");
    sMois[11] = string("Dec");

    // Getting local time
    //
    long t;
    time(&t);
    struct tm *tp = localtime(&t);

    // asctime renvoit la date au format : Sun Sep 16 01:03:52 1973\n\0
    //                                     012345678901234567890123
    //                                               1         2
    string sAscTime = string(asctime(tp));

    // Getting month
    //
    string sMonth = string(sAscTime, 4, 3);

    int i = 0;
    for (; (i < 12) && (sMonth != sMois[i]); i++)
       ;
    i++;

    string sNumMonth = string("00");
    if (i < 10)
        sNumMonth[1] = char (i + (int) '0');
    else if (i < 13)
    {
        sNumMonth[0] = '1';
        sNumMonth[1] = char ((i - 10) + (int) '0');
    }

    return string(sAscTime, 20, 4) + sNumMonth + string(sAscTime, 8, 2);
}

int donne_age(char *datex, char *dateNaiss)
{
    if ((NULL == datex) || (NULL == dateNaiss))
        return -1;

    // On ne calcule l'âge que si on a la date de naissance
    if ((strcmp(dateNaiss, "00000000") == 0) || (strcmp(dateNaiss, "19000000") == 0))
        return -1;

    // Année de l'examen
    unsigned int i = 10 * donneDigit(datex[0]) + donneDigit(datex[1]);
    i = (i - 18) * 100;
    i += 10 * donneDigit(datex[2]) + donneDigit(datex[3]);

    // Année de naissance du patient
    unsigned int j = 10 * donneDigit(dateNaiss[0]) + donneDigit(dateNaiss[1]);
    j = (j - 18) * 100;
    j += 10 * donneDigit(dateNaiss[2]) + donneDigit(dateNaiss[3]);

    // Age qu'aura le patient dans l'année
    signed int age = i - j;

    // Correctifs en fonction de mois et jour
    unsigned int mois_nais = 10 * donneDigit(dateNaiss[4]) + donneDigit(dateNaiss[5]);
    unsigned int jour_nais = 10 * donneDigit(dateNaiss[6]) + donneDigit(dateNaiss[7]);
    unsigned int mois_jour = 10 * donneDigit(datex[4]) + donneDigit(datex[5]);
    unsigned int jour_jour = 10 * donneDigit(datex[6]) + donneDigit(datex[7]);

    if ((mois_jour < mois_nais) || ((mois_jour == mois_nais) && (jour_jour < jour_nais)))
        age--;
    if ((age < 0) || (age > 150))
        return -1;
    return age;
}

int donne_age(string sDatex, string sDateNaiss)
{
    if ((strlen(sDateNaiss.c_str()) < 8) || (strlen(sDatex.c_str()) < 8))
        return -1;

    // On ne calcule l'âge que si on a la date de naissance
    if ((sDateNaiss == string("00000000")) || (sDateNaiss == string("19000000")))
        return -1;

    // Année de l'examen
    unsigned int i = 10 * donneDigit(sDatex[0]) + donneDigit(sDatex[1]);
    i = (i - 18) * 100;
    i += 10 * donneDigit(sDatex[2]) + donneDigit(sDatex[3]);

    // Année de naissance du patient
    unsigned int j = 10 * donneDigit(sDateNaiss[0]) + donneDigit(sDateNaiss[1]);
    j = (j - 18) * 100;
    j += 10 * donneDigit(sDateNaiss[2]) + donneDigit(sDateNaiss[3]);

    // Age qu'aura le patient dans l'année
    signed int age = i - j;

    // Correctifs en fonction de mois et jour
    unsigned int mois_nais = 10 * donneDigit(sDateNaiss[4]) + donneDigit(sDateNaiss[5]);
    unsigned int jour_nais = 10 * donneDigit(sDateNaiss[6]) + donneDigit(sDateNaiss[7]);
    unsigned int mois_jour = 10 * donneDigit(sDatex[4]) + donneDigit(sDatex[5]);
    unsigned int jour_jour = 10 * donneDigit(sDatex[6]) + donneDigit(sDatex[7]);

    if ((mois_jour < mois_nais) || ((mois_jour == mois_nais) && (jour_jour < jour_nais)))
        age--;
    if ((age < 0) || (age > 150))
        return -1;

    return age;
}

// -----------------------------------------------------------------------------
// Function    : void donne_heure(char* pHeure)// Description : Donne l'heure au format hhmmss
// Return      : Rien
// -----------------------------------------------------------------------------
// Created on 26/05/1998
// last update on 26/05/1998
// -----------------------------------------------------------------------------
void donne_heure(char* heure)
{
    if (NULL == heure)
        return;

    strcpy(heure, "hhmmss");

    // Prise de temp
    struct tm *tp;
    long	 t;
    char 	 temp[26];
    time(&t);
    tp = localtime(&t);
    strcpy(temp, asctime(tp));

    // asctime renvoit la date au format : Sun Sep 16 01:03:52 1973\n\0
    int i, j;
    for (i = 0, j = 0; i < 8; i++, j++)
    {
        heure[j] = temp[i + 11];
        if ((i == 1) || (i == 4))
            i++;
    }
}

string donne_heure()
{
    string sHeure = string("hhmmss");

    // Prise de temp
    long t;
    time(&t);
    struct tm *tp = localtime(&t);

    string sAscTime = string(asctime(tp));

    // asctime renvoit la date au format : Sun Sep 16 01:03:52 1973\n\0
    int i, j;
    for (i = 0, j = 0; i < 8; i++, j++)
    {
        sHeure[j] = sAscTime[i + 11];
        if ((i == 1) || (i == 4))
            i++;
    }

    return sHeure;
}

int donneDigit(char nbre)
{
    if (!isdigit(nbre))
        return -1;
    return int(nbre) - int('0');
}

// -----------------------------------------------------------------------------
//
// Implémentation des mèthodes VecteurString
//
// -----------------------------------------------------------------------------

VectString::~VectString()
{
    vider();
}

VectString::VectString()
{
}

bool VectString::contains(string sModele)
{
    if (empty())
        return false;

    for (IterString i = begin(); end() != i; i++)
        if (sModele == **i)
            return true;

    return false;
}

void VectString::vider()
{
    if (empty())
        return;

    IterString i = begin();
    while (end() != i)
    {
        delete (*i);
        erase(i);
    }
}

VectString& VectString::operator=(VectString src)
{
    if (this == &src)
        return (*this);

    vider();
    try
    {
        if (false == src.empty())
            for (IterString i = src.begin(); src.end() != i; i++)
                push_back(new string(*(*i)));
    } // try
    catch (...)
    {
        erreur("Exception VectString::operator=", standardError);}
    return (*this);
}

VectString::VectString(VectString& src)
{
    try
    {
        if (false == src.empty())
            for (IterString i = src.begin(); src.end() != i; i++)
                push_back(new string(*(*i)));
    } // try
    catch (...)
    {
        erreur("Exception VectString copy ctor.", standardError);
    }
}

void pseumaj(string* psChaine)
{
    for (size_t i = 0; i < strlen(psChaine->c_str()); i++)
        (*psChaine)[i] = pseumaj((*psChaine)[i]);
}

string pseumaj(string sChaine)
{
    string sOutputString = sChaine;
    pseumaj(&sOutputString);

    return sOutputString;
}

// -----------------------------------------------------------------------------
// Function    : void pseumaj(char *chaine)// Description : Convertit la chaine en pseudo-majuscules
// Return	     : Rien
// Created on 6/10/1999
// -----------------------------------------------------------------------------
void pseumaj(char *chaine)
{
    for (size_t i = 0; i < strlen(chaine); i++)
        chaine[i] = pseumaj(chaine[i]);
}

// -----------------------------------------------------------------------------
//
// Implémentation des mèthodes ClasseStringVector (vecteur de string)
//
// -----------------------------------------------------------------------------

ClasseStringVector::~ClasseStringVector()
{
    vider();
}

ClasseStringVector::ClasseStringVector()
    :vectclassString()
{
}

void ClasseStringVector::vider()
{
    if (empty())
        return;

    iterClassString i = begin();
    while (end() != i)
    {
        delete (*i);
        erase(i);
    }
}

ClasseStringVector& ClasseStringVector::operator=(ClasseStringVector src)
{
    if (this == &src)
        return (*this);

    vider();
    try
    {
        if (false == src.empty())
            for (iterClassString i = src.begin(); src.end() != i; i++)
                push_back(new classString(*(*i)));
    } // try
    catch (...)
    {
        erreur("Exception ClasseStringVector = operator.", standardError);
    }
    return (*this);
}

ClasseStringVector::ClasseStringVector(ClasseStringVector& src)
{
    try
    {
        if (false == src.empty())
            for (iterClassString i = src.begin(); src.end() != i; i++)
                push_back(new classString(*(*i)));
    } // try
    catch (...)
    {
        erreur("Exception ClasseStringVector copy ctor.", standardError);
    }
}
// -----------------------------------------------------------------------------
//
// classe classString
//
// -----------------------------------------------------------------------------
classString::classString(const classString& src)
{
    sItem      = src.sItem;
    colonne    = src.colonne;
    coche      = src.coche;
    sQualifier = src.sQualifier;
}

// -----------------------------------------------------------------------------
// Opérateur d'affectation
// -----------------------------------------------------------------------------
classString& classString::operator=(classString src)
{
    if (this == &src)
        return (*this);

    sItem      = src.sItem;
    colonne    = src.colonne;
    coche      = src.coche;
    sQualifier = src.sQualifier;

    return (*this);
}

classString::classString(string sChaine, int Colonne, bool Coche, string sQualif)
{
    sItem 	   = sChaine;
    colonne    = Colonne;
    coche 	   = Coche;
    sQualifier = sQualif;
}

// -----------------------------------------------------------------------------
// Split sModel into a left part and a right part on each side of first separator
// -----------------------------------------------------------------------------
void splitString(string sModel, string *psLeft, string *psRight, char separator)
{
    if ((NULL == psLeft) || (NULL == psRight))
        return;

    *psLeft  = string("");
    *psRight = string("");

    size_t iPosit = sModel.find(separator);
    size_t iLen   = strlen(sModel.c_str());

    // No separator or at the end of model
    //
    if (string::npos == iPosit)
    {
        *psLeft = sModel;
        return;
    }

    // Model starts with separator
    //
    if (0 == iPosit)
    {
        *psRight = string(sModel, 1, iLen - 1);
        return;
    }

    *psLeft = string(sModel, 0, iPosit);

    // Separator at the end of model
    //
    if (iPosit == iLen - 1)
        return;

    *psRight = string(sModel, iPosit + 1, iLen - iPosit - 1);
}

// -----------------------------------------------------------------------------
// Parse sModel as starting with qualifiers
// -----------------------------------------------------------------------------
void getQualifiers(string sModel, string &sResult, string &sQualifier, string sQualifiersList)
{
    sResult    = string("");
    sQualifier = string("");

    if (string("") == sModel)
        return;

    if (string("") == sQualifiersList)
    {
        sResult = sModel;
        return;
    }

    int i = 0;
    bool bKeepGoing = true;

    size_t iModelLen = strlen(sModel.c_str());

    // go forward while Model starting chars belongs to Qualifiers List
    //
    while ((i < iModelLen) && bKeepGoing)
    {
        char cAtI = sModel[i];
        if (string::npos == sQualifiersList.find(string(1, cAtI)))
            bKeepGoing = false;
        else
            i++;
    }

    if (0 == i)
    {
        sResult = sModel;
        return;
    }

    sQualifier = string(sModel, 0, i);

    if (i == iModelLen)
        return;

    sResult = string(sModel, i, iModelLen - i);
}

// -----------------------------------------------------------------------------
// Décomposer pChaine et mettre ses items dans le vecteur pVect Séparateur = "|" par défaur
// -----------------------------------------------------------------------------
void DecomposeChaine(string* pChaine, ClasseStringVector* pVect, string separateur, string sQualifiers)
{
    if ((NULL == pChaine) || (NULL == pVect) || (string("") == *pChaine))
        return;

    size_t debut     = 0;
    size_t posit     = pChaine->find(separateur);
    size_t iSeparLen = strlen(separateur.c_str());

    try
    {
        if (string::npos == posit)
        {
            pVect->push_back(new classString(*pChaine));
            return;
        }

        while (string::npos != posit)
        {
            if (debut == posit)
                pVect->push_back(new classString(string("")));
            else
            {
                string sModel  = string(*pChaine, debut, posit - debut);
                string sResult = string("");
                string sQualif = string("");
                getQualifiers(sModel, sResult, sQualif, sQualifiers);

                pVect->push_back(new classString(sResult, -1, false, sQualif));
            }

            debut = posit + iSeparLen;
            if ((debut + iSeparLen <= strlen(pChaine->c_str())) && (separateur == string(*pChaine, debut, iSeparLen)))
                posit = debut;
            else
                posit = pChaine->find(separateur, debut);
        }

        if (strlen(pChaine->c_str()) - debut > 0)
        {
            string sModel  = string(*pChaine, debut, strlen(pChaine->c_str()) - debut);
            string sResult = string("");
            string sQualif = string("");
            getQualifiers(sModel, sResult, sQualif, sQualifiers);

            pVect->push_back(new classString(sResult, -1, false, sQualif));
        }
        else
            pVect->push_back(new classString(string("")));

    } // try
    catch (...)
    {
        erreur("Exception DecomposeChaine.", standardError);
    }
}

// -----------------------------------------------------------------------------
// convertit  sValeur en un double
// -----------------------------------------------------------------------------
double StringToDouble(string sValeur)
{
    // Extraction de la partie entière et de la partie décimale
    string  sEntier = "";
    string  sDecima = "";
    size_t  i       = sValeur.find(".");
    if (i == string::npos)
        i = sValeur.find(",");

    if ((i == string::npos) || ( i > strlen(sValeur.c_str())))
        sEntier = sValeur;
    else
    {
        if (i > 0)
            sEntier = string(sValeur, 0, i);
        sDecima = string(sValeur, i + 1, strlen(sValeur.c_str()) - i - 1);
    }

    // Calcul de la valeur
    double dValeur = 0;

    // Partie entière
    for (i = 0; i < strlen(sEntier.c_str()); i++)
    {
        if (!isdigit(sEntier[i]))
            return 0;
        dValeur = (dValeur * double(10)) + double(donneDigit(sEntier[i]));
    }

    // Partie décimale
    double multi = double(1);
    for (size_t i = 0; i < strlen(sDecima.c_str()); i++)
    {
        if (!isdigit(sDecima[i]))
            return 0;
        multi = multi * 0.1;
        dValeur += double(donneDigit(sDecima[i])) * multi;
    }

    return dValeur;
}


//-------------------------------------------------------------------
// convertit un double en une string
//-------------------------------------------------------------------
string DoubleToString(double* pDouble, int iEntier, int iDecimal)
{
    if (NULL == pDouble)
        return string("");

    double dVal = *pDouble;
    if (dVal == 0)
        return string("0");

    bool bNegatif = false; //double négatif
    if (dVal < 0)
    {
        bNegatif = true;
        dVal = -dVal;
    }
    double dLog = log10(dVal);
    int	   iLog = int(dLog);
    double dMult = pow(double(10), double(iLog));
    double dResu;

    string sResult = "";

    // Assemblage de la partie entière
    while (iLog >= 0)
    {
        dResu = floor(dVal / dMult);
        sResult += string(1, donneChar(int(dResu)));

        dVal = dVal - (dResu * dMult);
        dMult = dMult / 10;
        iLog--;
    }
    if ((iDecimal == 0) || ((iDecimal == -1) && (dVal == 0)))
    {
        if (bNegatif)
            sResult = "-" + sResult;
        return sResult;
    }

    // Assemblage de la partie décimale
    if (sResult == "")
        sResult = "0";
    sResult += ".";
    iLog = 0;
    bool tourner = true;
    while (tourner)
    {
        dResu = floor(10 * dVal);
        // Si on en est à inscrire le dernier chiffre significatif, il faut
        // arrondir
        if ((iDecimal > 0) && (iLog == iDecimal-1) && (10 * dVal - dResu > double(0.5)))
        {
            if (dResu < 9)
                sResult += string(1, donneChar(int(dResu) + 1));
            else
            {
                int i = strlen(sResult.c_str()) - 1;
                bool bIncrementer = true;
                while ((i >= 0) && bIncrementer)
                {
                    if (isdigit(sResult[i]))
                    {
                        int j = donneDigit(sResult[i]);
                        if (j < 9)
                        {
                            j++;
                            sResult[i] = donneChar(j);
                            bIncrementer = false;
                        }
                        else
                        {
                            sResult[i] = '0';
                            if (i == 0)
                                sResult = string("1") + sResult;
                        }
                    }
                    i--;
                }
                sResult += string("0");
            }
        }
        else
            sResult += string(1, donneChar(int(dResu)));

        dVal = (10 * dVal) - dResu;
        iLog++;
        if ((iDecimal > 0) && (iLog >= iDecimal))
            tourner = false;
        if ((iDecimal == -1) && ((iLog > 10) || (dVal == 0)))
            tourner = false;
    }

    if (bNegatif)
        sResult = "-" + sResult;
    return sResult;
}

char donneChar(int digit)
{
    if ((digit >= 0) && (digit < 10))
        return char(int('0') + digit);
    else
        return '*';
}

string texteHtml(string texte)
{
    string  sHtml("");

    for (size_t i = 0; i < strlen(texte.c_str()); i++)
    {
        switch (texte[i])
        {
        case '!'  :	sHtml = sHtml + "&#33;"; 		break;
        case '"'  : sHtml = sHtml + "&quot;";		break;
        case '#'  :	sHtml = sHtml + "&#35;"; 		break;
        case '$'  : sHtml = sHtml + "&#36;";			break;
        case '%'  : sHtml = sHtml + "&#37;";			break;
        case '&'  : sHtml = sHtml + "&amp;";		  break;
        case '\'' : sHtml = sHtml + "&#39;";			break;
        case '<'  : sHtml = sHtml + "&lt;";			break;
        case '>'  : sHtml = sHtml + "&gt;";			break;
        case '?'  : sHtml = sHtml + "&#63;";			break;
        case '@'  : sHtml = sHtml + "&#64;";			break;
        case '\\' : sHtml = sHtml + "&#92;";			break;
        case '|'  : sHtml = sHtml + "&#124;";		break;
        case '£'  : sHtml = sHtml + "&#163;";		break;
        case ''  : sHtml = sHtml + "&euro;";		break;
        case '²'  : sHtml = sHtml + "&sup2;";		break;
        case 'â'  :	sHtml = sHtml + "&acirc;";		break;
        case 'à'  : sHtml = sHtml + "&agrave;";	break;
        case 'æ'  : sHtml = sHtml + "&aelig;";		break;
        case 'ç'  : sHtml = sHtml + "&ccedil;";	break;
        case 'é'  : sHtml = sHtml + "&eacute;";	break;
        case 'ê'  : sHtml = sHtml + "&ecirc;";		break;
        case 'è'  : sHtml = sHtml + "&egrave;";	break;
        case 'ë'  : sHtml = sHtml + "&euml;";		break;
        case 'î'  : sHtml = sHtml + "&icirc;";		break;
        case 'ï'  : sHtml = sHtml + "&iuml;";		break;
        case 'ô'  : sHtml = sHtml + "&ocirc;";		break;
        case 'ö'  : sHtml = sHtml + "&ouml;";		break;
        case ''  : sHtml = sHtml + "&#339;";		break;
        case 'ú'  : sHtml = sHtml + "&uacute;";	break;
        case 'ù'  : sHtml = sHtml + "&ugrave;";	break;
        case 'û'  : sHtml = sHtml + "&ucirc;";		break;
        case 'ü'  : sHtml = sHtml + "&uuml;";		break;
        case 'Â'  : sHtml = sHtml + "&Acirc;";		break;
        case 'À'  : sHtml = sHtml + "&Agrave;";	break;
        case 'Æ'  : sHtml = sHtml + "&Aelig;";		break;
        case 'Ç'  : sHtml = sHtml + "&Ccedil;";	break;
        case 'É'  : sHtml = sHtml + "&Eacute;";	break;
        case 'Ê'  : sHtml = sHtml + "&Ecirc;";		break;
        case 'È'  : sHtml = sHtml + "&Egrave;";	break;
        case 'Ë'  : sHtml = sHtml + "&Euml;";		break;
        case 'Î'  : sHtml = sHtml + "&Icirc;";		break;
        case 'Ï'  : sHtml = sHtml + "&Iuml;";		break;
        case 'Ò'  : sHtml = sHtml + "&Ograve;";	break;
        case 'Ó'  : sHtml = sHtml + "&Oacute;";	break;
        case 'Ô'  : sHtml = sHtml + "&Ocirc;";		break;
        case 'Õ'  : sHtml = sHtml + "&Otilde;";	break;
        case 'Ö'  : sHtml = sHtml + "&Ouml;";		break;
        case ''  : sHtml = sHtml + "&#338;";		break;
        case 'Ú'  : sHtml = sHtml + "&Uacute;";	break;
        case 'Ù'  : sHtml = sHtml + "&Ugrave;";	break;
        case 'Û'  : sHtml = sHtml + "&Ucirc;";		break;
        case 'Ü'  : sHtml = sHtml + "&Uuml;";	    break;
        case 'Ý'  : sHtml = sHtml + "&Yacute;";	break;
        case 'Þ'  : sHtml = sHtml + "&THORN;";	  break;
        case 'ß'	: sHtml = sHtml + "&szlig;";	  break;
        default   : sHtml = sHtml + string(1, texte[i]);
        }
    }
    return sHtml;}

string texteCourant(string textHtml)
{
    string textOut = textHtml;
    size_t pos     = textOut.find("&");

    while (string::npos != pos)
    {
        size_t len = strlen(textOut.c_str()) - pos;
        if 		  ((len >= 7) && (string(textOut, pos, 7) == "&acirc;"))
            textOut.replace(pos, 7, "â");
        else if ((len >= 8) && (string(textOut, pos, 8) == "&agrave;"))
            textOut.replace(pos, 8, "à");
        else if ((len >= 8) && (string(textOut, pos, 7) == "&aelig;"))
            textOut.replace(pos, 7, "æ");
        else if ((len >= 8) && (string(textOut, pos, 8) == "&ccedil;"))
            textOut.replace(pos, 8, "ç");
        else if ((len >= 8) && (string(textOut, pos, 8) == "&eacute;"))
            textOut.replace(pos, 8, "é");
        else if ((len >= 7) && (string(textOut, pos, 7) == "&ecirc;"))
            textOut.replace(pos, 7, "ê");
        else if ((len >= 8) && (string(textOut, pos, 8) == "&egrave;"))
            textOut.replace(pos, 8, "è");
        else if ((len >= 6) && (string(textOut, pos, 6) == "&euml;"))
            textOut.replace(pos, 6, "ë");
        else if ((len >= 7) && (string(textOut, pos, 7) == "&icirc;"))
            textOut.replace(pos, 7, "î");
        else if ((len >= 6) && (string(textOut, pos, 6) == "&iuml;"))
            textOut.replace(pos, 6, "ï");
        else if ((len >= 7) && (string(textOut, pos, 7) == "&ocirc;"))
            textOut.replace(pos, 7, "ô");
        else if ((len >= 6) && (string(textOut, pos, 6) == "&ouml;"))
            textOut.replace(pos, 6, "ö");
        else if ((len >= 8) && (string(textOut, pos, 8) == "&uacute;"))
            textOut.replace(pos, 8, "ú");
        else if ((len >= 8) && (string(textOut, pos, 8) == "&ugrave;"))
            textOut.replace(pos, 8, "ù");
        else if ((len >= 7) && (string(textOut, pos, 7) == "&ucirc;"))
            textOut.replace(pos, 7, "û");
        else if ((len >= 6) && (string(textOut, pos, 6) == "&uuml;"))
            textOut.replace(pos, 6, "ü");
        else if ((len >= 4) && (string(textOut, pos, 4) == "&lt;"))
            textOut.replace(pos, 4, "<");
        else if ((len >= 4) && (string(textOut, pos, 4) == "&gt;"))
            textOut.replace(pos, 4, ">");
        else if ((len >= 6) && (string(textOut, pos, 6) == "&amp;"))
            textOut.replace(pos, 6, "&");
        else if ((len >= 6) && (string(textOut, pos, 6) == "&quot;"))
            textOut.replace(pos, 6, "\"");
        else if ((len >= 6) && (string(textOut, pos, 6) == "&sup2;"))
            textOut.replace(pos, 6, "²");

        // gestion des majuscules accentuées
        else if ((len >= 7) && (string(textOut, pos, 7) == "&Acirc;"))
            textOut.replace(pos, 7, "Â");
        else if ((len >= 8) && (string(textOut, pos, 8) == "&Agrave;"))
            textOut.replace(pos, 8, "À");
        else if ((len >= 8) && (string(textOut, pos, 7) == "&Aelig;"))
            textOut.replace(pos, 7, "Æ");
        else if ((len >= 8) && (string(textOut, pos, 8) == "&Ccedil;"))
            textOut.replace(pos, 8, "Ç");
        else if ((len >= 8) && (string(textOut, pos, 8) == "&Eacute;"))
            textOut.replace(pos, 8, "É");
        else if ((len >= 7) && (string(textOut, pos, 7) == "&Ecirc;"))
            textOut.replace(pos, 7, "Ê");
        else if ((len >= 8) && (string(textOut, pos, 8) == "&Egrave;"))
            textOut.replace(pos, 8, "È");
        else if ((len >= 6) && (string(textOut, pos, 6) == "&Euml;"))
            textOut.replace(pos, 6, "Ë");
        else if ((len >= 7) && (string(textOut, pos, 7) == "&Icirc;"))
            textOut.replace(pos, 7, "Î");
        else if ((len >= 6) && (string(textOut, pos, 6) == "&Iuml;"))
            textOut.replace(pos, 6, "Ï");
        else if ((len >= 8) && (string(textOut, pos, 8) == "&Ograve;"))
            textOut.replace(pos, 8, "Ò");
        else if ((len >= 8) && (string(textOut, pos, 8) == "&Oacute;"))
            textOut.replace(pos, 8, "Ó");
        else if ((len >= 7) && (string(textOut, pos, 7) == "&Ocirc;"))
            textOut.replace(pos, 7, "Ô");
        else if ((len >= 8) && (string(textOut, pos, 8) == "&Otilde;"))
            textOut.replace(pos, 8, "Õ");
        else if ((len >= 6) && (string(textOut, pos, 6) == "&Ouml;"))
            textOut.replace(pos, 6, "Ö");
        else if ((len >= 8) && (string(textOut, pos, 8) == "&Uacute;"))
            textOut.replace(pos, 8, "Ú");
        else if ((len >= 8) && (string(textOut, pos, 8) == "&Ugrave;"))
            textOut.replace(pos, 8, "Ù");
        else if ((len >= 7) && (string(textOut, pos, 7) == "&Ucirc;"))
            textOut.replace(pos, 7, "Û");
        else if ((len >= 6) && (string(textOut, pos, 6) == "&Uuml;"))
            textOut.replace(pos, 6, "Ü");
        else if ((len >= 8) && (string(textOut, pos, 8) == "&Yacute;"))
            textOut.replace(pos, 8, "Ý");
        else if ((len >= 7) && (string(textOut, pos, 7) == "&THORN;"))
            textOut.replace(pos, 7, "Þ");
        else if ((len >= 7) && (string(textOut, pos, 7) == "&szlig;"))
            textOut.replace(pos, 7, "ß");

        else if ((len >= 6) && (string(textOut, pos, 6) == "&euro;"))
            textOut.replace(pos, 6, "");

        // gestion des "&#i;" with 0 <= i <= 255
        else if ((len >= 4) && (string(textOut, pos, 2) == "&#"))
        {
            int iDigit  = 0;
            int iUnit   = 0;
            int iPos    = pos + 2;
            while (isdigit(textOut[iPos]) && (iDigit <= 3))
            {
                iUnit = iUnit * 10 + textOut[iPos] - '0';
                iDigit++;
                iPos++;
            }
            if (iUnit != 0)
            {
                char  pcReplace[5];
                sprintf(pcReplace, "%c", iUnit);
                textOut.replace(pos, 2 + iDigit + 1, pcReplace);
            }
        }
        /*
      textOut.replace(pos, 6, "|");
    else if ((len >= 6) && (string(textOut, pos, 6) == "&#163;"))      textOut.replace(pos, 6, "£");    else if ((len >= 5) && (string(textOut, pos, 5) == "&#36;"))      textOut.replace(pos, 5, "$");*/
        pos++;
        pos = textOut.find("&",pos);
    }
    /*
    bool isdigit (charT c, const locale& loc) const;
  isdigit*/
    return textOut;}

string texteWebLink(string texte)
{
    string  sHtml("");

    for (size_t i = 0; i < strlen(texte.c_str()); i++)
    {
        if (((texte[i] >= '0') && (texte[i] <= '9')) ||
                ((texte[i] >= 'A') && (texte[i] <= 'Z')) ||
                ((texte[i] >= 'a') && (texte[i] <= 'z')))
            sHtml += texte[i];
        else
        {
            unsigned char theChar = texte[i];
            char szCarHex[3];
            char_to_hex(theChar, szCarHex);
            sHtml += string("%") + string(szCarHex);
        }
    }

    return sHtml;
}

string texteCourantFromWebLink(string textWL)
{
    string  sTexte("");

    for (size_t i = 0; i < strlen(textWL.c_str()); i++)
    {
        if ((textWL[i] != '%') || (i > strlen(textWL.c_str()) - 2))
            sTexte += textWL[i];
        else
        {
            unsigned char theChar;
            char szCarHex[3];
            szCarHex[0] = textWL[i+1];
            szCarHex[1] = textWL[i+2];
            hex_to_char(szCarHex, &theChar);
            sTexte += string(1, theChar);
            i += 2;
        }
    }

    return sTexte;
}

void hex_to_char(char car_hex[], unsigned char *car)
{
    if (NULL == car)
        return;

    signed   int  i;
    //  unsigned int  chiffre;
    //  unsigned long multi;
    *car = 0;
    if (strlen(car_hex) != 2)
        return;
    for (i = 0; i < 2; i++)
    {
        *car = *car * 16;
        if ((car_hex[i] >= '0') && (car_hex[i] <= '9'))
            *car += (unsigned long) car_hex[i] - 48;
        if ((car_hex[i] >= 'A') && (car_hex[i] <= 'F'))
            *car += (unsigned long) car_hex[i] - 55;
    }
    return;
}

void char_to_hex(unsigned char car, char* car_hex)
{
    if (NULL == car_hex)
        return;

    unsigned char result, mask = 0xF;
    unsigned short i;
    char hex_car[16] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
    car_hex[2] = '\0';
    for (i = 0; i < 2; i++)
    {
        result = car & mask;
        mask   = mask << 4;
        result = result >> (i << 2);
        car_hex[1-i] = hex_car[result];
    }
    return;
}

string getTreeIDFromID(string sAnyID)
{
    if (strlen(sAnyID.c_str()) < 13)
        return string("");
    return string(sAnyID, 7, 6);
}

string getNodeIDFromID(string sAnyID)
{
    if (strlen(sAnyID.c_str()) < 18)
        return string("");
    return string(sAnyID, 13, 5);
}

string getRegularPath(string sMonoSeparatorPath, char cPath, char cIntranode)
{
    if (string("") == sMonoSeparatorPath)
        return string("");
    string result = sMonoSeparatorPath;
    for (register unsigned int i = 0; i < result.size(); i++)
    {
        if (cPath == result[i])
        {
            bool change = false;
            if (result.size() > i + 1)
            {
                if ('$' == result[i+1])
                    change = true;
                else if (result.size() > i + 2)
                {
                    std::string temp = result.substr(i+1, 2);
                    if ((temp == "£N") || (temp == "£D") || (temp == "£T"))
                        change = true;
                    else if (result.size() > i + 3)
                    {
                        std::string temp = result.substr(i+1, 3);
                        if (temp == "WCE")
                            change = true;
                        else if (result.size() > i + 4)
                        {
                            std::string temp = result.substr(i+1, 4);
                            if (temp == "WPLU")
                                change = true;
                        }
                    }
                }
            }
            if (true == change)
                result[i] = cIntranode;
        }
    }
    return (result);
}

/**
 * Is this element a collective, group, local or in memory object
 * This works with a patient ID, a tree ID or a node ID
 */
GRAPHELEMTYPE getGraphElementTypeFromID(string sElementID)
{
    if (sElementID == "")
        return isIncorrectID;
    char cFirstChar = sElementID[0];
    if (cFirstChar == '#')
        return isMemoryID;
    if (cFirstChar == '-')
        return isLocalID;
    if (cFirstChar == '~')
        return isGroupID;
    return isCollectiveID;
}

string getFormatedTime(string sRawDate, string sLang, string sTimeFormat)
{
    if (strlen(sRawDate.c_str()) < 8)
        return string("");

    if (strlen(sRawDate.c_str()) < 14)
        sRawDate += string(14 - strlen(sRawDate.c_str()), '0');

    if (string("") == sTimeFormat)
    {
        if (string("en-US") == sLang)
            sTimeFormat = string("MM/DD/AAAA hh:mm:ss");
        else
            sTimeFormat = string("DD/MM/AAAA hh:mm:ss");
    }

    string sYear  = string(sRawDate, 0, 4);
    string sMonth = string(sRawDate, 4, 2);
    string sDays  = string(sRawDate, 6, 2);

    string sResult = sTimeFormat;

    size_t iYearPos = sResult.find("AAAA");
    if (string::npos != iYearPos)
        sResult.replace(iYearPos, 4, sYear);

    size_t iMonthPos = sResult.find("MM");
    if (string::npos != iMonthPos)
        sResult.replace(iMonthPos, 2, sMonth);

    size_t iDayPos = sResult.find("DD");
    if (string::npos != iDayPos)
        sResult.replace(iDayPos, 2, sDays);

    string sHour   = string(sRawDate,  8, 2);
    string sMinute = string(sRawDate, 10, 2);
    string sSecond = string(sRawDate, 12, 2);

    size_t iHourPos = sResult.find("hh");
    if (string::npos != iHourPos)
        sResult.replace(iHourPos, 2, sHour);

    size_t iMinutePos = sResult.find("mm");
    if (string::npos != iMinutePos)
        sResult.replace(iMinutePos, 2, sMinute);

    size_t iSecondPos = sResult.find("ss");
    if (string::npos != iSecondPos)
        sResult.replace(iSecondPos, 2, sSecond);

    return sResult;
}
string getFormatedTime(string sRawTime, string sLang, string sDateFormat, string sHourFormat)
{
    size_t iRawTimeSize = strlen(sRawTime.c_str());
    if (iRawTimeSize < 8)
        return string("");
    string sFormatedHour = string("");
    string sRawDate = string(sRawTime, 0, 8);
    string sFormatedDate = getFormatedDate(sRawDate, sLang, sDateFormat);
    if (14 == iRawTimeSize){
        string sRawHour = string(sRawTime, 8, 6);
        if (string("") == sHourFormat)    {
            sFormatedHour = getFormatedHour(sRawHour, sLang, sFormatedDate);
            if (string("") == sFormatedHour)
                return sFormatedDate;
            else
                return sFormatedHour;
        } else {
            sFormatedHour = getFormatedHour(sRawHour, sLang, sHourFormat);
            sFormatedDate += string(" ") + sFormatedHour;
        }
    }
    return sFormatedDate;
}

string getFormatedDate(string sRawDate, string sLang, string sDateFormat)
{
    if (strlen(sRawDate.c_str()) != 8)
        return string("");

    if (string("") == sDateFormat)
    {
        if (string("en-US") == sLang)
            sDateFormat = string("MM/DD/AAAA");
        else
            sDateFormat = string("DD/MM/AAAA");
    }

    string sYear  = string(sRawDate, 0, 4);
    string sMonth = string(sRawDate, 4, 2);
    string sDays  = string(sRawDate, 6, 2);

    string sResult = sDateFormat;

    size_t iYearPos = sResult.find("AAAA");
    if (string::npos != iYearPos)
        sResult.replace(iYearPos, 4, sYear);

    size_t iMonthPos = sResult.find("MM");
    if (string::npos != iMonthPos)
        sResult.replace(iMonthPos, 2, sMonth);

    size_t iDayPos = sResult.find("DD");
    if (string::npos != iDayPos)
        sResult.replace(iDayPos, 2, sDays);

    return sResult;
}

string getFormatedHour(string sRawHour, string sLang, string sHourFormat)
{
    if (strlen(sRawHour.c_str()) != 6)
        return string("");

    if (string("") == sHourFormat)
        sHourFormat = string("hh:mm:ss");

    string sHour   = string(sRawHour, 0, 2);
    string sMinute = string(sRawHour, 2, 2);
    string sSecond = string(sRawHour, 4, 2);

    string sResult = sHourFormat;

    size_t iHourPos = sResult.find("hh");
    if (string::npos != iHourPos)
        sResult.replace(iHourPos, 2, sHour);

    size_t iMinutePos = sResult.find("mm");
    if (string::npos != iMinutePos)
        sResult.replace(iMinutePos, 2, sMinute);

    size_t iSecondPos = sResult.find("ss");
    if (string::npos != iSecondPos)
        sResult.replace(iSecondPos, 2, sSecond);

    return sResult;
}

// -----------------------------------------------------------------------------
// void numacar(mot, nbre, taille)
// Fonction : Transforme un NOMBRE en CARACTERE
// Retour	  : Rien
// Créé le 02/08/1989 -- Dernière mise à jour le 28/07/1994
// -----------------------------------------------------------------------------
void numacar(char *mot, unsigned int nbre, unsigned int taille)
{
    unsigned int i, multi;

    if (taille > 5)
        taille = 5;

    multi = 1;
    for (i = 1; i < taille; i++)
        multi = multi * 10;
    for (i = taille; i > 0; i--, multi = multi / 10)
    {
        if (nbre < multi)
            mot[taille-i] = '0';
        else
        {
            mot[taille-i] = (char)(char(nbre / multi) + '0');
            nbre -= multi * (nbre / multi);
        }
    }
    mot[taille] = '\0';

    return;
}

// -----------------------------------------------------------------------------
// Description : Indique si l'année donnée est bissextile
// -----------------------------------------------------------------------------
// Created on 28/06/99
// -----------------------------------------------------------------------------
bool bissextile(int year)
{
    // Règle : Si l'année est divisible par 4 ET non divisible par 100
    //				OU divisible par 400 ==> elle est bissextile.
    if (((year % 4 == 0) && (!(year % 100 == 0))) || (year % 400 == 0))
        return true;
    else
        return false;
}

// -----------------------------------------------------------------------------
// Function    : int ote_blancs(mot)
// Description : Ote les blancs à droite et à gauche de mot[]
// Return      : Rien
// Created on 13/10/1988 - last update on 11/06/1990
// -----------------------------------------------------------------------------
int ote_blancs(char* mot)
{
    unsigned int i;

    // on ôte les blancs finaux
    for (i = strlen(mot); (mot[i-1] == ' ') && (i > 0); i--)
       ;

    if (i == 0)  {    mot[0] = '\0';    return (0);  }  else
        mot[i] = '\0';

    // On ôte les blancs initiaux
    while (mot[0] == ' ')
        for (i = 0; mot[i] != '\0'; i++)
            mot[i] = mot[i + 1];
    return (0);
}

// -----------------------------------------------------------------------------
// Function    : donne_date_claire(date, message, pays)
// Description : Transforme une date AAAAMMJJ en un message "1er janvier 1994"
// Return      : Rien
// -----------------------------------------------------------------------------
// Created on 30/08/1994
// Last update on 30/08/1994
// -----------------------------------------------------------------------------
//
//  See http://en.wikipedia.org/wiki/Date_and_time_notation_by_country
//
void donne_date_claire(char* date, string *message, string sLang)
{
    if (NULL == message)
        return;

    unsigned int i;
    *message = "";
    if ((NULL == date) || (strlen(date) < 8))
        return;

    // Day / Jour
    //
    string sDayLib = string("??");
    if (isdigit(date[6]) && isdigit(date[7]))
    {
        if (date[6] != '0')
            sDayLib = string(1, date[6]);
        else
            sDayLib = string("");

        sDayLib += string(1, date[7]);
    }

    // Month / Mois
    //
    string sMonthLib = string("??");
    if (isdigit(date[4]) && isdigit(date[5]))
    {
        i = 10 * ((int) date[4] - (int) '0') + (int) date[5] - (int) '0';
        sMonthLib = donne_mois(i, sLang);
    }

    // Year / Année
    //
    string sYearLib = string("");
    for (i = 0; i < 4; i++)
    {
        if (isdigit(date[i]))
            sYearLib += string(1, date[i]);
        else
            sYearLib += '?';
    }

    //
    // Make the "nice modifications"
    //
    if ((sLang == string("fr")) || (sLang == string("fre")) || (sLang == string("fra")))
    {
        if (sDayLib == string("1"))
            sDayLib = string("1er");
    }

    //
    // Put all this together
    //
    if (sLang == string("en-US"))
        *message = sMonthLib + string(" ") + sDayLib + string(", ") + sYearLib;
    else
        *message = sDayLib + string(" ") + sMonthLib + string(" ") + sYearLib;

    return;
}

// -----------------------------------------------------------------------------
// Function    : void donne_date_claire(date, message, pays)
// Description : Transforme une date AAAAMMJJ en un message "1er janvier 1994"
// Return      : Rien
// -----------------------------------------------------------------------------
// Created on 30/08/1994
// Last update on 30/08/1994
// -----------------------------------------------------------------------------
void donne_date_claire(char* date, char* message, string sLang)
{
    if (NULL == message)
        return;

    if (NULL == date)
    {
        strcpy(message, "");
        return;
    }

    string sMessage = string("");

    donne_date_claire(date, &sMessage, sLang);

    strcpy(message, sMessage.c_str());

    return;
}

// -----------------------------------------------------------------------------
// Description : Donne le libellé du mois à partir de "MM"
// -----------------------------------------------------------------------------
// Created on 14/11/1997
// Last update on 14/11/1997
// -----------------------------------------------------------------------------
string donne_mois(string mois, string sLang)
{
    if (strlen(mois.c_str()) < 2)
        return "";
    if ((!(isdigit(mois[0]))) || (!(isdigit(mois[1]))))
        return "";
    int i = 10 * donneDigit(mois[0]) + donneDigit(mois[1]);

    return donne_mois(i, sLang);
}

// -----------------------------------------------------------------------------
// Description : Donne le libellé du mois à partir d'un int             ¦
// -----------------------------------------------------------------------------
// Created on 14/11/1997
// Last modified on 14/11/1997
// -----------------------------------------------------------------------------
string donne_mois(int iMois, string sLang)
{
    if ((iMois < 1) || (iMois > 12))
        return "";

    if (sLang == string("fr"))
    {
        switch (iMois)
        {
        case  1 : return "janvier" ;
        case  2 : return "février" ;
        case  3 : return "mars"    ;
        case  4 : return "avril"   ;
        case  5 : return "mai"     ;
        case  6 : return "juin"    ;
        case  7 : return "juillet" ;
        case  8 : return "août"    ;
        case  9 : return "septembre";
        case 10 : return "octobre" ;
        case 11 : return "novembre";
        case 12 : return "décembre";
        }
    }
    else
    {
        switch (iMois)
        {
        case  1 : return "january" ;
        case  2 : return "february";
        case  3 : return "march"   ;
        case  4 : return "april"   ;
        case  5 : return "may"     ;
        case  6 : return "june"    ;
        case  7 : return "july"    ;
        case  8 : return "august"  ;
        case  9 : return "september";
        case 10 : return "october" ;
        case 11 : return "november";
        case 12 : return "december";
        }
    }
    return "";
}

// -----------------------------------------------------------------------------
// Function    : void donne_date(date, message, pays)// Description : Transforme une date AAAAMMJJ en un message JJ/MM/AAAA
// Return      : Rien
// -----------------------------------------------------------------------------
// Created on 12/04/1991
// Last update on 12/04/1991
// -----------------------------------------------------------------------------
void donne_date(char *date, char *message, string sLang)
{
    if (NULL == message)
        return;

    if ((NULL == date) || (strlen(date) < 8))
    {
        strcpy(message, "");
        return;
    }

    strcpy(message, "  /  /    ");

    // American english notation : MM/JJ/AAAA
    //
    if (sLang == "en-US")
    {
        message[0] = date[4]; message[1] = date[5];
        message[3] = date[6]; message[4] = date[7];
        message[6] = date[0]; message[7] = date[1];
        message[8] = date[2]; message[9] = date[3];
        return;
    }

    // Standard notation : JJ/MM/AAAA
    //
    message[0] = date[6]; message[1] = date[7];
    message[3] = date[4]; message[4] = date[5];
    message[6] = date[0]; message[7] = date[1];
    message[8] = date[2]; message[9] = date[3];
}

string donne_date(string sDate, string sLang)
{
    if (strlen(sDate.c_str()) < 8)
        return string("");

    string sMessage = string("  /  /    ");

    // American english notation : MM/JJ/AAAA
    //
    if (sLang == "en-US")
    {
        sMessage[0] = sMessage[4]; sMessage[1] = sDate[5];
        sMessage[3] = sMessage[6]; sMessage[4] = sDate[7];
        sMessage[6] = sMessage[0]; sMessage[7] = sDate[1];
        sMessage[8] = sMessage[2]; sMessage[9] = sDate[3];
        return sMessage;
    }

    // Standard notation : JJ/MM/AAAA
    //
    sMessage[0] = sDate[6]; sMessage[1] = sDate[7];
    sMessage[3] = sDate[4]; sMessage[4] = sDate[5];
    sMessage[6] = sDate[0]; sMessage[7] = sDate[1];
    sMessage[8] = sDate[2]; sMessage[9] = sDate[3];

    return sMessage;
}

// -----------------------------------------------------------------------------
// Description : Donne la date au format "1er janvier 1994"            ¦
// -----------------------------------------------------------------------------
// Created on 14/11/1997
// Last update on 14/11/1997
// -----------------------------------------------------------------------------
void donne_date_claire(string sDate, string* pMessage, string* pIntro, string sLang)
{
    if ((NULL == pMessage) || (NULL == pIntro))
        return;

    *pMessage	= "";
    *pIntro		= "";

    // Date au format AAAAMMJJ
    if (strlen(sDate.c_str()) < 8)
        return;

    // Date vide
    if (sDate == string("00000000"))
        return;

    if ((string(sDate, 4, 4) == "0000") || (string(sDate, 4, 4) == "0000"))
    {
        donne_date_breve(sDate, pMessage, pIntro, sLang);
        return;
    }

    if      ((sLang == string("fr")) || (sLang == string("fre")) || (sLang == string("fra")))
        *pIntro = "le ";
    else if ((sLang == string("en")) || (sLang == string("eng")) || (sLang == string("en-US")))
        *pIntro = "on the ";

    char szDate[9];
    strcpy(szDate, sDate.c_str());
    donne_date_claire(szDate, pMessage, sLang);

    return;
}

// -----------------------------------------------------------------------------
// Description : Donne la date au format JJ/MM/AAAA                ¦
// -----------------------------------------------------------------------------
// Created on 14/11/1997
// Last update on 14/11/1997
// -----------------------------------------------------------------------------
void donne_date_breve(string sDate, string* pMessage, string* pIntro, string sLang)
{
    if ((NULL == pMessage) || (NULL == pIntro))
        return;

    *pMessage = "";
    *pIntro   = "";

    // Date au format AAAAMMJJ
    if (strlen(sDate.c_str()) < 8)
        return;

    // Date vide
    if (sDate == string("00000000"))
        return;

    // Juste AAAA
    if (string(sDate, 4, 4) == "0000")
    {
        *pMessage = string(sDate,0, 4);

        if ((sLang == string("fr")) || (sLang == string("fre")) || (sLang == string("fra")))
            *pIntro = "en ";
        if ((sLang == string("en")) || (sLang == string("eng")) || (sLang == string("en-US")))
            *pIntro = "in ";

        return;
    }

    // MM/AAAA
    if (string(sDate, 6, 2) == "00")
    {
        *pMessage = donne_mois(string(sDate, 4, 2), sLang) + " " + string(sDate, 0, 4);

        if ((sLang == string("fr")) || (sLang == string("fre")) || (sLang == string("fra")))
            *pIntro = "en ";
        if ((sLang == string("en")) || (sLang == string("eng")) || (sLang == string("en-US")))
            *pIntro = "in ";
    }

    // JJ/MM/AAAA
    else
    {
        char szMessage[11];
        char szDate[9];
        strcpy(szDate, sDate.c_str());
        donne_date(szDate, szMessage, sLang);
        *pMessage = string(szMessage);

        if      ((sLang == string("fr")) || (sLang == string("fre")) || (sLang == string("fra")))
            *pIntro = "le ";
        else if ((sLang == string("en")) || (sLang == string("eng")) || (sLang == string("en-US")))
            *pIntro = "on the ";
    }
    return;
}

// -----------------------------------------------------------------------------
// Calcul de l'âge des bébés en mois
// -----------------------------------------------------------------------------
// Created on 17/07/1995
// Last update on 17/07/1995
// -----------------------------------------------------------------------------
int donne_age_mois(char *datex, char *dateNaiss)
{
    if ((NULL == datex) || (NULL == dateNaiss))
        return -1;

    // On ne calcule l'âge que si on a la date de naissance
    if ((strcmp(dateNaiss, "00000000") == 0) || (strcmp(dateNaiss, "19000000") == 0))
        return -1;

    // Année de l'examen
    unsigned int i = 10 * donneDigit(datex[0]) + donneDigit(datex[1]);
    i = (i - 18) * 100;
    i += 10 * donneDigit(datex[2]) + donneDigit(datex[3]);

    // Année de naissance du patient
    unsigned int j = 10 * donneDigit(dateNaiss[0]) + donneDigit(dateNaiss[1]);
    j = (j - 18) * 100;
    j += 10 * donneDigit(dateNaiss[2]) + donneDigit(dateNaiss[3]);

    // Age qu'aura le patient dans l'année (en mois)
    signed int age = (i - j) * 12;

    // Correctifs en fonction de mois et jour
    unsigned int mois_nais = 10 * donneDigit(dateNaiss[4]) + donneDigit(dateNaiss[5]);
    unsigned int jour_nais = 10 * donneDigit(dateNaiss[6]) + donneDigit(dateNaiss[7]);
    unsigned int mois_jour = 10 * donneDigit(datex[4]) + donneDigit(datex[5]);
    unsigned int jour_jour = 10 * donneDigit(datex[6]) + donneDigit(datex[7]);

    age = age + mois_jour - mois_nais;
    if (jour_jour < jour_nais)
        age--;

    return age;
}

int donne_age_mois(string sDatex, string sDateNaiss)
{
    if ((strlen(sDateNaiss.c_str()) < 8) || (strlen(sDatex.c_str()) < 8))
        return -1;
    // On ne calcule l'âge que si on a la date de naissance
    if ((sDateNaiss == string("00000000")) || (sDateNaiss == string("19000000")))
        return -1;

    // Année de l'examen
    unsigned int i = 10 * donneDigit(sDatex[0]) + donneDigit(sDatex[1]);
    i = (i - 18) * 100;
    i += 10 * donneDigit(sDatex[2]) + donneDigit(sDatex[3]);

    // Année de naissance du patient
    unsigned int j = 10 * donneDigit(sDateNaiss[0]) + donneDigit(sDateNaiss[1]);
    j = (j - 18) * 100;
    j += 10 * donneDigit(sDateNaiss[2]) + donneDigit(sDateNaiss[3]);

    // Age qu'aura le patient dans l'année (en mois)
    signed int age = (i - j) * 12;

    // Correctifs en fonction de mois et jour
    unsigned int mois_nais = 10 * donneDigit(sDateNaiss[4]) + donneDigit(sDateNaiss[5]);
    unsigned int jour_nais = 10 * donneDigit(sDateNaiss[6]) + donneDigit(sDateNaiss[7]);
    unsigned int mois_jour = 10 * donneDigit(sDatex[4]) + donneDigit(sDatex[5]);
    unsigned int jour_jour = 10 * donneDigit(sDatex[6]) + donneDigit(sDatex[7]);

    age = age + mois_jour - mois_nais;
    if (jour_jour < jour_nais)
        age--;

    return age;
}

// Valid operators are '!' for negation and '|' and '&' for OR and AND
//
// Pure logical means "just a combination of boolean vars" in which case
// "a b | c" is invalid. On the contrary, when the formula can contains
// calculation and comparison operators, "a - b > c | d" is valid
//
bool isLogicalFormulaValid(string sFormula, string *psErrorMsg, bool bPureLogical)
{
    if (string("") == sFormula)
        return true;

    string sOperators = string("!()|&");

    size_t cc = 0;
    char oper = ' ';

    size_t iSize = strlen(sFormula.c_str());

    while (cc < iSize)
    {
        // Skip blanks and tabs
        //
        if ((' ' == sFormula[cc]) || ('\t' == sFormula[cc]))
        {
            while ((cc < iSize) && ((' ' == sFormula[cc]) || ('\t' == sFormula[cc])))        cc++;

            // If pure logical, check that 2 'non operators' don't appear in a row
            //
            if (bPureLogical && (sOperators.find(sFormula[cc]) == string::npos) &&
                    (sOperators.find(oper) == string::npos))
            {
                if (psErrorMsg)
                    *psErrorMsg = string("missingOperatorBetweenTwoVars");
                return false;
            }
        }

        // Checking current operand    //    if (cc < iSize)
        {
            // Is current operand compatible with the previous one?
            //
            if (false == areConsecutiveLogicalCharCompatible(oper, sFormula[cc]))
            {
                if (psErrorMsg)
                    *psErrorMsg = string("invalidOperatorsSequence");
                return false;
            }

            // ')' is always invalid
            //
            if (')' == sFormula[cc])
            {
                if (psErrorMsg)
                    *psErrorMsg = string("excessOfClosingParenthesis");
                return false;
            }

            // '(' opens a block and leads to a recursive call
            //
            if ('(' == sFormula[cc])
            {
                size_t iStart = cc + 1;
                size_t iParenthesisLevel = 1;

                cc++;

                // Looking for the closing parenthesis
                //
                while ((cc < iSize) && (iParenthesisLevel > 0))
                {
                    switch (sFormula[cc])
                    {
                    case '(' : iParenthesisLevel++; break;
                    case ')' : iParenthesisLevel--; break;
                    }
                    cc++;
                }

                if (iParenthesisLevel > 0)
                {
                    if (psErrorMsg)
                        *psErrorMsg = string("missingClosingParenthesis");
                    return false;
                }

                if (cc <= iStart + 1)
                {
                    if (psErrorMsg)
                        *psErrorMsg = string("emptyLogicalBlock");
                    return false;
                }

                string sInsideBlokSentence = string(sFormula, iStart, cc - iStart -1);

                if (false == isLogicalFormulaValid(sInsideBlokSentence, psErrorMsg, bPureLogical))
                    return false;

                oper = ')';
            }
            else
            {
                oper = sFormula[cc];
                cc++;
            }
        }
    }
    return true;
}

// Is current operand compatible with the previous one?
//
bool areConsecutiveLogicalCharCompatible(char cPrevious, char cNext)
{
    // after '!' operators !)|& are not allowed
    if (('!' == cPrevious) &&
            (('!' == cNext) ||
             (')' == cNext) ||
             ('|' == cNext) ||
             ('&' == cNext)))
        return false;
    else
        return true;

    // after '(' operators )|& are not allowed
    if (('(' == cPrevious) &&
            ((')' == cNext) ||
             ('|' == cNext) ||
             ('&' == cNext)))
        return false;
    else
        return true;

    if ((')' == cPrevious) &&
            (('!' == cNext) ||
             ('(' == cNext)))
        return false;
    else
        return true;

    if ((('|' == cPrevious) || ('&' == cPrevious)) &&
            ((')' == cNext) ||
             ('&' == cNext) ||
             ('|' == cNext)))
        return false;
    else
        return true;

    return true;
}

void parseLogicalBlocks(string &sFormula, map<string, string> *paBlocks)
{
    if ((string("") == sFormula) || (NULL == paBlocks))
        return;

    size_t iBlockStart = 0;
    size_t cc = 0;
    char oper = ' ';

    size_t iSize = strlen(sFormula.c_str());

    while (cc < iSize)
    {
        // Skip blanks and tabs
        //
        while ((cc < iSize) && ((' ' == sFormula[cc]) || ('\t' == sFormula[cc])))      cc++;

        // Checking current operand    //    if (cc < iSize)
        {
            // ')' is always invalid
            //
            if (')' == sFormula[cc])
                return;

            // '(' opens a block and leads to a recursive call
            //
            if ('(' == sFormula[cc])
            {
                size_t iStart = cc + 1;
                size_t iParenthesisLevel = 1;

                cc++;

                // Looking for the closing parenthesis
                //
                while ((cc < iSize) && (iParenthesisLevel > 0))
                {
                    switch (sFormula[cc])
                    {
                    case '(' : iParenthesisLevel++; break;
                    case ')' : iParenthesisLevel--; break;
                    }
                    cc++;
                }

                if (iParenthesisLevel > 0)
                    return;

                if (cc <= iStart + 1)
                    return;

                string sInsideBlokSentence = string(sFormula, iStart, cc - iStart -1);

                strip(sInsideBlokSentence, stripBoth);

                // if block contains logical operators, then recurse
                //
                if (isLogicalBlock(sInsideBlokSentence))
                    parseLogicalBlocks(sInsideBlokSentence, paBlocks);

                oper = ')';
            }
            // '|' or '&' separate blocks
            //
            else if (('|' == sFormula[cc]) || ('&' == sFormula[cc]))
            {
                string sInsideBlokSentence = string(sFormula, iBlockStart, cc - iBlockStart);

                if (string("") != sInsideBlokSentence)
                {
                    string sAlias = replaceVarByAlias(&sFormula, paBlocks, iBlockStart, sInsideBlokSentence, string("block"));

                    cc = iBlockStart + strlen(sAlias.c_str());
                    iSize = strlen(sFormula.c_str());
                }

                iBlockStart = cc + 1;

                cc++;
            }
            // '!' can start a block
            //
            else if (('!' == sFormula[cc]) &&
                     ((0 == cc) || (cc > 0) && ((' ' == sFormula[cc-1]) ||
                                                (')' == sFormula[cc-1]))))
            {
                iBlockStart = cc + 1;

                cc++;
            }
            else
            {
                oper = sFormula[cc];
                cc++;
            }
        }
    }

    string sInsideBlokSentence = string(sFormula, iBlockStart, iSize - iBlockStart);

    if (string("") != sInsideBlokSentence)
        replaceVarByAlias(&sFormula, paBlocks, iBlockStart, sInsideBlokSentence, string("block"));

    return;

    /*
  size_t iBlockStart  = 0;
  size_t iCurseur     = 0;
  size_t iFormulaSize = strlen(sFormula.c_str());
  while (iCurseur < iFormulaSize)
  {
    // This char is a reserved char
    //
    if (sLogicalOperators.find(sFormula[iCurseur]) != string::npos)
    {
      string sBlockContent = string(sFormula, iBlockStart, iCurseur - iBlockStart);
      strip(sBlockContent, stripBoth);
      if ((string("") != sBlockContent) && (false == paBlocks->contains(sBlockContent)))
        paBlocks->push_back(new string(sBlockContent));

      iCurseur++;

      // Looking for the starting char of next block (not a separator)
      //
      while ((iCurseur < strlen(sFormula.c_str())) &&
             (sLogicalOperators.find(sFormula[iCurseur]) != string::npos))
        iCurseur++;

      iBlockStart = iCurseur;
    }
    else
      iCurseur++;
  }

  string sBlockContent = string(sFormula, iBlockStart, iCurseur - iBlockStart);
  strip(sBlockContent, stripBoth);
  if ((string("") != sBlockContent) && (false == paBlocks->contains(sBlockContent)))
    paBlocks->push_back(new string(sBlockContent));
*/
}

// Parse formula into "LValue Operator RValue"
//
// if bSingleBlockAccepted is true, it means that a contiguous block is
// accepted (assuming that the implicit operator is "exists")
//
bool parseComparisonBlock(string sFormula,
                          string &sLValue,
                          string &sOperator,
                          string &sRValue,
                          string *psErrorMsg,
                          bool bSingleBlockAccepted)
{
    // Blocks initialization
    //
    sLValue   = string("");
    sOperator = string("");
    sRValue   = string("");

    // Is sentence empty?
    //
    strip(sFormula, string(" \t"), stripBoth);

    if (string("") == sFormula)
    {
        if (psErrorMsg)
            *psErrorMsg = string("emptyComparisonBlock");

        return false;
    }

    size_t cc   = 0;

    // looking for a comparison operator that separates LValue and RValue
    //
    size_t iSize = strlen(sFormula.c_str());

    if (cc == iSize)
    {    if (psErrorMsg)      *psErrorMsg = string("emptyComparisonBlock");    return false;  }
    bool bExistsBlanck = false;

    string sOper = string("");
    size_t iOpNb = 0;

    while (cc < iSize)
    {
        bool bAfterBlank = false;

        // Skip blanks and tabs
        //
        while ((cc < iSize) && ((' ' == sFormula[cc]) || ('\t' == sFormula[cc])))    {      cc++;
            bAfterBlank   = true;
            bExistsBlanck = true;
        }

        // Checking current operand    //    if (cc < iSize)
        {
            if (bAfterBlank && ('*' == sFormula[cc]))
            {
                size_t iOperatorStart = cc;

                sOper = string("");
                while ((cc < iSize) && (' ' != sFormula[cc]) && ('\t' != sFormula[cc]))
                {
                    sOper += string(1, sFormula[cc]);
                    cc++;
                }
                if (isComparisonOperator(sOper))
                {
                    iOpNb++;

                    if (iOpNb > 1)
                    {
                        if (psErrorMsg)
                            *psErrorMsg = string("emptyComparisonBlock");
                        return false;
                    }

                    if (cc >= iSize)
                    {
                        if (psErrorMsg)
                            *psErrorMsg = string("noRvalueInComparisonBlock");
                        return false;
                    }

                    sLValue   = string(sFormula, 0, iOperatorStart);
                    strip(sLValue, string(" \t"), stripBoth);

                    if (string("") == sLValue)
                    {
                        if (psErrorMsg)
                            *psErrorMsg = string("noLvalueInComparisonBlock");
                        return false;
                    }

                    sOperator = sOper;

                    sRValue   = string(sFormula, cc, iSize - cc);
                    strip(sRValue, string(" \t"), stripBoth);

                    if (string("") == sRValue)
                    {
                        if (psErrorMsg)
                            *psErrorMsg = string("noRvalueInComparisonBlock");
                        return false;
                    }
                }
            }
            else
                cc++;
        }
    }

    if (string("") == sOperator)
    {
        if ((false == bExistsBlanck) && bSingleBlockAccepted)
        {
            sLValue = sFormula;
            return true;
        }

        if (psErrorMsg)
            *psErrorMsg = string("noComparisonOperatorFound");
        return false;
    }

    return true;
}

int evaluate(map<string, bool> *pDatas, string sFormula, size_t& cc)
{
    if (string("") == sFormula)
        return true;

    if ((NULL == pDatas) || pDatas->empty())
        return false;

    string sOperators = string("!&|");
    string sBlanks    = string(" \t\r\n");

    int result = 1;
    int result1;
    char oper = ' ';
    bool bNegation;

    while (cc < strlen(sFormula.c_str()))  {
        result1 = 1;
        bNegation = false;

        // Skip blanks
        //
        while ((cc < strlen(sFormula.c_str())) && (string::npos != sBlanks.find(sFormula[cc])))      cc++;

        // on évalue l'opérande en cours
        if (cc < strlen(sFormula.c_str()))
        {
            // on évalue d'abord la négation
            if ('!' == sFormula[cc])
            {
                cc++;
                bNegation = true;
            }

            if (cc == strlen(sFormula.c_str()))
                return -1;

            // on doit avoir ici une '(' ou un nom de contrainte
            //
            if ('(' == sFormula[cc])
            {
                cc++;
                result1 = evaluate(pDatas, sFormula, cc);
            }
            else
            {
                string sNomContrainte = "";

                // Getting the constraint while no blank and no operator
                //
                while ((cc < strlen(sFormula.c_str())) && (string::npos == sOperators.find(sFormula[cc]))
                       && (string::npos == sBlanks.find(sFormula[cc]))
                       && (')' != sFormula[cc]))
                {
                    sNomContrainte += string(1, sFormula[cc]);
                    cc++;
                }
                if (string("") != sNomContrainte)
                    result1 = (*pDatas)[sNomContrainte];
                else
                    return -1;
            }
        }

        // cas erreur à l'évaluation de l'opérande
        if (-1 == result1)
            return -1;
        if (bNegation)
            result1 = !result1;

        // on calcule le résultat selon l'opérateur en cours
        if      (' ' == oper)
            result = result1;
        else if ('|' == oper)
            result = result || result1;
        else if ('&' == oper)
            result = result && result1;

        // on avance à nouveau
        while ((cc < strlen(sFormula.c_str())) && (' ' == sFormula[cc]))
            cc++;

        // on évalue l'opérateur
        if (cc < strlen(sFormula.c_str()))
        {
            // on doit avoir ici une ')' ou un opérateur
            if (('|' == sFormula[cc]) || ('&' == sFormula[cc]))
            {
                oper = sFormula[cc];
                cc++;
            }
            else if (')' == sFormula[cc])
            {
                cc++;
                return result;
            }
            else
                return -1;
        }
    }
    return result;
}

bool compare(double dLValue, string sOperator, double dRValue)
{
    if (string("*!=") == sOperator)
        return (dLValue != dRValue);
    if (string("*<")  == sOperator)
        return (dLValue < dRValue);
    if (string("*<=") == sOperator)
        return (dLValue <= dRValue);
    if (string("*==") == sOperator)
        return (dLValue == dRValue);
    if (string("*>")  == sOperator)
        return (dLValue > dRValue);
    if (string("*>=") == sOperator)
        return (dLValue >= dRValue);

    return false;
} 

double calculate(map<string, double> *pDatas, string sFormula)
{
    if (string("") == sFormula)
        return 0;

    // 100 niveaux de piles
    double val[100];
    string ope[100];
    int	   nbNiv = 100;

    for (int j = 0; j < nbNiv; j++)
    {
        val[j] = 0;
        ope[j] = string("");
    }

    string sCurrentVar = string("");

    size_t i = 0;
    int    pileur = 0;
    bool   tourner = true;
    while (tourner)
    {
        switch(sFormula[i])
        {
        case '(' : // Parenthèse ouvrante : on empile
        {
            for (int j = nbNiv - 1; j > pileur; j--)
            {
                val[j] = val[j-1];
                ope[j] = ope[j-1];
            }
            val[pileur] = 0;
            ope[pileur] = "";
            i++;

            break;
        }
        case '+' : // Opérateurs + - * / ^x, etc
        case '-' :
        case '*' :
        case '/' :
        case '^' : // power  example 10^var
        case '£' : // log    example 10£var -> log10(var)

            // Processing sCurrentVar
            //
            if (string("") != sCurrentVar)
            {
                if ((NULL == pDatas) || (pDatas->empty()))
                    return 0;
                double dValue = (*pDatas)[sCurrentVar];
                sCurrentVar = string("");

                if (ope[pileur] != "")
                    val[pileur] = operation(val[pileur], dValue, ope[pileur]);
                else
                    val[pileur] = dValue;
            }

            ope[pileur] = string(1, sFormula[i]);
            i++;
            break;

        case ')' : // Parenthèse fermante : on dépile
        {
            // Processing sCurrentVar
            //
            if (string("") != sCurrentVar)
            {
                if ((NULL == pDatas) || (pDatas->empty()))
                    return 0;
                double dValue = (*pDatas)[sCurrentVar];
                sCurrentVar = string("");

                if (ope[pileur] != "")
                    val[pileur] = operation(val[pileur], dValue, ope[pileur]);
                else
                    val[pileur] = dValue;
            }

            while (ope[pileur+1] != "")
            {
                val[pileur] = operation(val[pileur+1], val[pileur], ope[pileur+1]);
                for (int j = pileur+1; j < nbNiv-1; j++)
                {
                    val[j] = val[j+1];
                    ope[j] = ope[j+1];
                }
                val[nbNiv-1] = 0;
                ope[nbNiv-1] = "";
            }
            i++;

            break;
        }

        case '0' : // Digits; if not part of a var name, get the whole number
        case '1' :
        case '2' :
        case '3' :
        case '4' :
        case '5' :
        case '6' :
        case '7' :
        case '8' :
        case '9' :
        {
            if (string("") != sCurrentVar)
            {
                sCurrentVar += string(1, sFormula[i]);
                i++;
            }
            else
            {
                string sNbre = string(1, sFormula[i]);
                i++;
                while ((i < strlen(sFormula.c_str())) &&
                       (((sFormula[i] >= '0') && (sFormula[i] <= '9')) ||
                        (sFormula[i] == '.')))
                {
                    sNbre += string(1, sFormula[i]);
                    i++;
                }
                double dNbre = StringToDouble(sNbre);
                if (ope[pileur] != "")
                    val[pileur] = operation(val[pileur], dNbre, ope[pileur]);
                else
                    val[pileur] = dNbre;
            }

            break;
        }

        case ' ' :
        {
            if (string("") != sCurrentVar)
            {
                if ((NULL == pDatas) || (pDatas->empty()))
                    return 0;
                double dValue = (*pDatas)[sCurrentVar];
                sCurrentVar = string("");

                if (ope[pileur] != "")
                    val[pileur] = operation(val[pileur], dValue, ope[pileur]);
                else
                    val[pileur] = dValue;
            }
            i++;

            break;
        }

        default :

            sCurrentVar += string(1, sFormula[i]);
            i++;
        }

        if (i >= strlen(sFormula.c_str()))
            tourner = false;
    }

    if (string("") != sCurrentVar)
    {
        if ((NULL == pDatas) || (pDatas->empty()))
            return 0;
        double dValue = (*pDatas)[sCurrentVar];
        sCurrentVar = string("");

        if (ope[pileur] != "")
            val[pileur] = operation(val[pileur], dValue, ope[pileur]);
        else
            val[pileur] = dValue;
    }

    return val[pileur];
}

double operation(double dOperande, double dOperateur, string sOperation)
{
    if (string("+") == sOperation)
        return dOperande + dOperateur;
    if (string("-") == sOperation)
        return dOperande - dOperateur;
    if (string("*") == sOperation)
        return dOperande * dOperateur;
    if ((string("/") == sOperation) && (0 != dOperateur))
        return dOperande / dOperateur;
    if (string("^") == sOperation)
        return pow(dOperande, dOperateur);
    if (string("£") == sOperation)
    {
        if (double(10) == dOperande)
            return log10(dOperateur);
    }
    return 0;
}

// Does this string constitute a logical sentence?
//
bool isLogicalBlock(string sBlock)
{
    if (string("") == sBlock)
        return false;

    // Chars & and | are dedicated logical operators
    //
    if (sBlock.find('|') != string::npos)
        return true;
    if (sBlock.find('&') != string::npos)
        return true;

    // Char ! is a logical operator when first char
    //
    size_t iNegPos = sBlock.find('!');
    if (string::npos == iNegPos)
        return false;

    if (0 == iNegPos)
        return true;

    while (string::npos != iNegPos)
    {
        if ((' ' == sBlock[iNegPos - 1]) || ('(' == sBlock[iNegPos - 1]))
            return true;

        iNegPos = sBlock.find('!', iNegPos);
    }

    return false;
}

bool isComparisonOperator(string sCandidateOperator)
{
    if ((string("*!=") == sCandidateOperator) ||
            (string("*<")  == sCandidateOperator) ||
            (string("*<=") == sCandidateOperator) ||
            (string("*==") == sCandidateOperator) ||
            (string("*>")  == sCandidateOperator) ||
            (string("*>=") == sCandidateOperator))
        return true;

    return false;
}

string getComparisonUnit(string sLValue, string sRValue)
{
    // First, check if RValue is of the kind 35[2KGM2] (only value[unit])
    //
    strip(sRValue, string(" \t"), stripBoth);

    // Is it a monolithic block
    //
    if ((sRValue.find(' ') == string::npos) && (sRValue.find('\t') == string::npos))
        return getPathUnit(sRValue);

    return string("");
} 

string getPathUnit(string sPath)
{
    if (string("") == sPath)
        return string("");

    // First case, desired unit is explicitly mentionned inside []
    //
    size_t posStartUnit = sPath.find("[");
    if (string::npos != posStartUnit)  {    size_t posEndUnit = sPath.find("]", posStartUnit + 1);    if (string::npos != posEndUnit)
            return string(sPath, posStartUnit + 1, posEndUnit - posStartUnit - 1);
    }

    return string("");
}

string replaceVarByAlias(string *psFormula,
                         map<string, string> *paAliases,
                         size_t iVarStartPos,
                         string sVar,
                         string sNameInit)
{
    if ((NULL == psFormula) || (string("") == *psFormula) || (string("") == sVar))
        return string("");

    string sAlias = string("");

    // Elaborate a "cleaned var"
    //
    string sCleanedVar = sVar;

    // Remove trailing blanks
    strip(sCleanedVar, stripBoth);

    // remove useless parenthesis
    while (('(' == sCleanedVar[0]) && (')' == sCleanedVar[strlen(sCleanedVar.c_str())-1]))
    {
        sCleanedVar = string(sCleanedVar, 1, strlen(sCleanedVar.c_str())-2);
        strip(sCleanedVar, stripBoth);
    }

    if (paAliases)
    {
        // Is this var already known?
        //
        map<string, string>::iterator cur = paAliases->find(sCleanedVar);
        if (paAliases->end() != cur)
            sAlias = (*cur).second;
    }

    // unknown, create a new alias
    //
    if (string("") == sAlias)
    {
        size_t iBlockNb;
        if (paAliases)
        {
            iBlockNb = paAliases->size();
            // check if this alias doesn't already exist
            bool bKeepChecking = true;
            while (bKeepChecking)
            {
                sAlias = sNameInit + std::to_string(iBlockNb);
                map<string, string>::iterator cur = paAliases->begin();
                while ((paAliases->end() != cur) && ((*cur).second != sAlias))
                    cur++;

                if (paAliases->end() != cur)
                    iBlockNb++;
                else
                    bKeepChecking = false;
            }
            (*paAliases)[sCleanedVar] = sAlias;
        }
        else
            sAlias = sNameInit;
    }

    psFormula->replace(iVarStartPos, strlen(sVar.c_str()), sAlias);

    return sAlias;
}

// Convert an unsigned int to a string for a base (up to 62 (10 + 26 + 26))
//
void numacarbase(string* pResult, unsigned int nbre, unsigned int size, unsigned int base)
{
    if (NULL == pResult)
        return;

    if ((0 == size) || (0 == base) || (base > 62))
    {
        *pResult = string("");
        return;
    }

    // Initialisation: size * '0'
    //
    *pResult = string(size, '0');

    if (0 == nbre)
        return;

    unsigned int remainingNbre = nbre;

    for (unsigned int i = 0; i < size; i++)
    {
        unsigned int iLocalDivide = (unsigned int) pow(base, size - i - 1);
        unsigned int iLocalValue  = remainingNbre / iLocalDivide;

        if      (iLocalValue < 10)
            (*pResult)[i] = iLocalValue + '0';
        else if (iLocalValue < 36)
            (*pResult)[i] = iLocalValue - 10 + 'A';
        else if (iLocalValue < 62)
            (*pResult)[i] = iLocalValue - 36 + 'a';

        remainingNbre -= iLocalDivide * iLocalValue;

        if (0 == remainingNbre)
            return;
    }
}

/*	Function	:	ScanFor()
**	Parameters	:	string *psBuff
**				char cTarget
**				size_t &x
**	Returns		:	char
*/
char URL_ScanFor(string *psBuff, char cTarget, size_t &x)
{
    if ((NULL == psBuff) || (string("") == *psBuff))
        return '\0';

    size_t iBuffLen = strlen(psBuff->c_str());

    int Qchar	= 0	;

    do
    {
        if (x >= iBuffLen)
            break;

        char C = (*psBuff)[x];
        if (C == cTarget)
            break;

        if ('+' == C)
            (*psBuff)[x] = ' ';
        if (127 == C)
        {
            (*psBuff)[x] = '&';
            x--;
            continue;
        }
        if (('\"' == C) || ('\'' == C))
        {
            Qchar = C;
            continue;
        }

        if (Qchar	&& (C != Qchar))
            continue;

        Qchar = 0;
    } while ((*psBuff)[++x] != cTarget);

    return (*psBuff)[x];
}

int ParseUrlQuery(string *psBuff, map<string, string> *pQueryParts)
{
    std::cout << "ParseUrlQuery() psBuff: " << *psBuff << std::endl;
    if (!pQueryParts || !psBuff) {
        std::cout << "null pointer" << std::endl;
        return 0;
    }

    size_t iQueryLength = strlen(psBuff->c_str());

    if (iQueryLength < 1)
        return 0;


    /*
    **	MAIN QUERY PARSING LOOP
    **
    */

    string sFieldName  = string("");
    string sFieldValue = string("");

    size_t zpoint = 0;

    //***	While x < than the length of obQUERY_STRING as stored in iQueryLength
    for (size_t x = 0; x <= iQueryLength; x++)
    {
        //***	Locate the assignment character '='
        char cScanResult = URL_ScanFor(psBuff, '=', x);
        if ('=' == cScanResult)
        {
            //***	Collect a Field name
            sFieldName = string(*psBuff, zpoint, x - zpoint);
            DecodeURL(sFieldName);
            zpoint = ++x;

            //***	Locate the field separation character '&'
            URL_ScanFor(psBuff, '&', x);
            sFieldValue = string(*psBuff, zpoint, x - zpoint);
            DecodeURL(sFieldValue);
            zpoint = ++x;

            bool bIsImagemap = false;
            if ((string::npos != sFieldName.find(".y")) &&
                    (string::npos != sFieldName.find(".x")))
                bIsImagemap = true;

            // If this is not an imagemap button and it starts with an "@"
            // Split the NAME at the first "." and move the right half into VALUE

            if (('@' == sFieldName[0]) && (string::npos != sFieldName.find(".")))
            {
                size_t iNameLen = strlen(sFieldName.c_str());

                sFieldName = string(sFieldName, 1, iNameLen - 1);
                iNameLen--;

                size_t iDotPos = sFieldName.find(".");
                sFieldValue = string(sFieldName, iDotPos + 1, iNameLen - iDotPos - 1);
                strip(sFieldValue, string("\"'\r\n"));
                sFieldName  = string(sFieldName, 0, iDotPos);

                (*pQueryParts)[sFieldName] = sFieldValue;

                continue;
            }

            (*pQueryParts)[sFieldName] = sFieldValue;
        }
        else
            break;
    }  /***** End for ***/

    return 1;
}

void DecodeURL(string& sBuffer)
{
    if (string("") == sBuffer)
        return;

    for (size_t x = 0; x < strlen(sBuffer.c_str()); x++)
    {
        if ('%' != sBuffer[x])
            continue;

        // double '%': just keep one "as is"
        //
        if ((x + 1 < strlen(sBuffer.c_str())) && ('%' == sBuffer[x+1]))
        {
            sBuffer = string(sBuffer, 0, x) + string(sBuffer, x+1, strlen(sBuffer.c_str()) - x - 2);
            x++;
            continue;
        }

        // replace the % and the 2 next chars by the char they represent in hexadecimal notation
        //
        if (x + 2 < strlen(sBuffer.c_str()))
        {
            string sHexaVal = string(sBuffer, x + 1, 2);
            sBuffer[x] = (unsigned char) (int)strtol(sHexaVal.c_str(), NULL, 16);
            if (x + 3 < strlen(sBuffer.c_str()))
                sBuffer = string(sBuffer, 0, x+1) + string(sBuffer, x+3, strlen(sBuffer.c_str()) - x - 3);
            else
                sBuffer = string(sBuffer, 0, x+1);
        }
    }
}

