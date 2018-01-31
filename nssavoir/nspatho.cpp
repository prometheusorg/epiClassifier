//---------------------------------------------------------------------------
//    NSPERSON.CPP
//    KRZISCH PH.   janvier 92
//  Implémentation de objets NAUTILUS
//---------------------------------------------------------------------------

#include <string>
#include <cstring>

//using namespace std;

#ifdef _ENTERPRISE_DLL
  #include "enterpriseLus/nsdivfctForCgi.h"
  #include "enterpriseLus/superLus.h"
  #include "nsdatabaseLus/nsSQL.h"
  #include "mysql/mysql.h"
#else
  #include "nautilus\nssuper.h"
  #include "partage\nsdivfct.h"
  #include "nautilus\nsrechdl.h"
#endif

#include "nssavoir/nspatho.h"

// -----------------------------------------------------------------------------
//
// Implémentation des méthodes NSPatholog
//
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Description: Constructeur.
// -----------------------------------------------------------------------------
NSPathologData::NSPathologData()
{
#ifndef _ENTERPRISE_DLL
	// Met les champs de données à zéro
	memset(libelle,   0, PATHO_LIBELLE_LEN + 1);
	memset(code, 	    0, PATHO_CODE_LEN + 1);
	memset(grammaire, 0, PATHO_GRAMMAIRE_LEN + 1);
	memset(freq, 	    0, PATHO_FREQ_LEN + 1);
#else
  sLabel     = string("");
  sCode      = string("");
  sGrammar   = string("");
  sFrequency = string("");
#endif
}


// -----------------------------------------------------------------------------
// Description:	Constructeur copie
// -----------------------------------------------------------------------------
NSPathologData::NSPathologData(NSPathologData& rv)
{
#ifndef _ENTERPRISE_DLL
	strcpy(libelle,   rv.libelle);
	strcpy(code, 	    rv.code);
	strcpy(grammaire, rv.grammaire);
	strcpy(freq,   	  rv.freq);
#else
  sLabel     = rv.sLabel;
  sCode      = rv.sCode;
  sGrammar   = rv.sGrammar;
  sFrequency = rv.sFrequency;
#endif
}

// -----------------------------------------------------------------------------
// Description:	Destructeur
// -----------------------------------------------------------------------------
NSPathologData::~NSPathologData()
{
}

// -----------------------------------------------------------------------------
// Description:	Opérateur =
// -----------------------------------------------------------------------------
NSPathologData&
NSPathologData::operator=(NSPathologData src)
{
	if (this == &src)
		return *this;

#ifndef _ENTERPRISE_DLL
	strcpy(libelle,   src.libelle);
	strcpy(code, 	    src.code);
	strcpy(grammaire, src.grammaire);
	strcpy(freq,   	  src.freq);
#else
  sLabel     = src.sLabel;
  sCode      = src.sCode;
  sGrammar   = src.sGrammar;
  sFrequency = src.sFrequency;
#endif

	return *this;
}

// -----------------------------------------------------------------------------
// Description:	Opérateur de comparaison
// -----------------------------------------------------------------------------
int
NSPathologData::operator==(const NSPathologData& o)
{
#ifndef _ENTERPRISE_DLL
	if ((strcmp(libelle, o.libelle) == 0) && (strcmp(code, o.code) == 0))
#else
  if ((sLabel == o.sLabel) && (sCode == o.sCode))
#endif
		return 1;
	else
		return 0;
}

// -----------------------------------------------------------------------------
// Description: Met à blanc les variables de la fiche
// -----------------------------------------------------------------------------
#ifndef _ENTERPRISE_DLL
void
NSPathologData::metABlanc()
{
	// Met les champs de données à blanc
	memset(libelle, 	' ', PATHO_LIBELLE_LEN);
	memset(code, 		  ' ', PATHO_CODE_LEN);
	memset(grammaire, ' ', PATHO_GRAMMAIRE_LEN);
	memset(freq, 	    ' ', PATHO_FREQ_LEN);
}
#endif

// -----------------------------------------------------------------------------
// Function     : NSPathologData::fabriqueNomLong()
// Description  : Initialise nom_long
// Returns      : Rien
// -----------------------------------------------------------------------------
void
NSPathologData::fabriqueLibelleLong()
{
  //	libelleLong = string(libelle);
}

// -----------------------------------------------------------------------------
//  Function    : NSPathologData::donneLibelleChoix(string* pLibelle)
// Description  : Donne le libellé affiché dans ChoixPathoDialogue
// -----------------------------------------------------------------------------
void
NSPathologData::donneLibelleChoix(string* pLibelle)
{
  if ((string *) NULL == pLibelle)
    return;

  // le libellé est du type 	Crohn [maladie][de]{détails}
  // qui donne 					Crohn (maladie){détails}
  //
  // Les indications d'orthographe au pluriel sont entre ||,
  // il faut les enlever
  string sLibelle = string("");

  bool dansPluriel = false;
#ifndef _ENTERPRISE_DLL
  for (int k = 0; (k < PATHO_LIBELLE_LEN) && (libelle[k] != '\0'); k++)
	{
    if      (libelle[k] == '|')
    {
      if (dansPluriel)
        dansPluriel = false;
      else
        dansPluriel = true;
    }
    else if (!dansPluriel)
      sLibelle += libelle[k];
  }
#else
  for (size_t k = 0; (k < strlen(sLabel.c_str())); k++)
	{
    if ('|' == sLabel[k])
    {
      if (dansPluriel)
        dansPluriel = false;
      else
        dansPluriel = true;
    }
    else if (false == dansPluriel)
      sLibelle += sLabel[k];
  }
#endif

  int  niveau_crochet = -1;

  string qualifiant[2];
  string mot_debut  = "";
  string mot_fin	  = "";
  qualifiant[0]     = "";
  qualifiant[1]     = "";

  // On sépare le libellé sous la forme :
  // mot_debut 	  = "Crohn"
  // mot_fin   	  = "{détails}"
  // qualifiant[0] = "maladie"
  // qualifiant[1] = "de"
  int iLibLen = strlen(sLibelle.c_str());
  for (int k = 0; k < iLibLen; k++)
  {
    if (sLibelle[k] == '[')
    {
      niveau_crochet++;
      k++;

      while ((sLibelle[k] != ']') && (k < iLibLen))
      {
        qualifiant[niveau_crochet] += sLibelle[k];
        k++;
      }
    }
    else
    {
      if (niveau_crochet == -1)
        mot_debut += sLibelle[k];
      else
        mot_fin += sLibelle[k];
    }
  }

  // On assemble le nouveau libelle
  *pLibelle = mot_debut;
  if (qualifiant[0] != "")
    *pLibelle += "(" + qualifiant[0] + ")";
  *pLibelle += mot_fin;

  // On s'assure qu'il fait encore PATHO_LIBELLE_LEN caractères
  if 	  (strlen(pLibelle->c_str()) < PATHO_LIBELLE_LEN)
    *pLibelle += string(PATHO_LIBELLE_LEN - strlen(pLibelle->c_str()), ' ');
  else if (strlen(pLibelle->c_str()) > PATHO_LIBELLE_LEN)
    (*pLibelle)[PATHO_LIBELLE_LEN-1] = '\0';


  // On supprime les blancs terminaux
  strip(*pLibelle, stripRight);

  // On vérifie qu'il n'existe pas plusieurs blancs contigus
  size_t positBlanc = pLibelle->find(" ");
  size_t debut, fin, taille;
  while (positBlanc != NPOS)
  {
    debut  = positBlanc;

    taille = strlen(pLibelle->c_str());
    if ((debut < taille - 1) && ((*pLibelle)[debut + 1] == ' '))
    {
      fin = debut + 1;
      while ((fin < taille - 1) && ((*pLibelle)[fin + 1] == ' '))
        fin++;

      *pLibelle = string(*pLibelle, 0, debut) + " " + string(*pLibelle, fin + 1, taille - fin - 1);
    }
    else
      fin = debut;

    debut++;
    positBlanc = pLibelle->find(" ", debut);
  }
}

// -----------------------------------------------------------------------------
//  Function    : NSPathologData::donneLibelleAffiche(string* pLibelle)
//  Description : Donne le "beau" libellé
//                La variable GenreMaitre sert à donner un genre aux adjectifs
// -----------------------------------------------------------------------------
void
NSPathologData::donneLibelleAffiche(string* pLibelle, int iDeclinaison)
{
  if ((string *) NULL == pLibelle)
    return;

  *pLibelle = "";

  // le libellé est du type 	"Crohn [maladie][de]{détails}"
  // qui donne 					"maladie de Crohn"
  //
  // Les indications de déclinaison sont entre ||, séparés par des /
  // ex : "disjoint|t/te/ts/tes|"
  //
  // Trois cas sont possibles :
  // 1) Pas de || : mot invariable
  // 2) Un seul élément entre || (ex test|s|) qui donne la première
  //    déclinaison lorsque la racine est constituée du mot entier
  // 3) Plusieurs éléments entre || (ex animal|al/aux|), le premier
  //    élément (0) permet de définir la racine, les autres donnent les
  //    déclinaisons (1, 2...)

  int niveau_crochet  = -1;
  string mot_debut 	  = "";
  string mot_fin	  	= "";
  string orthographe  = "";
  string qualifiant[2];
  qualifiant[0]       = "";
  qualifiant[1]       = "";

  // On sépare le libellé sous la forme :
  // mot_debut 	  = "Crohn"
  // mot_fin   	  = "{détails}"
  // qualifiant[0] = "maladie"
  // qualifiant[1] = "de"
#ifndef _ENTERPRISE_DLL
  for (int k = 0; (k < PATHO_LIBELLE_LEN) && (libelle[k] != '\0'); k++)
  {
    if (libelle[k] == '[')
    {
      niveau_crochet++;
      k++;
      while ((libelle[k] != ']') && (k < PATHO_LIBELLE_LEN) && (libelle[k] != '\0'))
      {
        if (libelle[k] == '|')
        {
          orthographe = "";
          k++;
          while ((libelle[k] != '|') && (k < PATHO_LIBELLE_LEN) && (libelle[k] != '\0'))
          {
            orthographe += libelle[k];
            k++;
          }
          if ((k < PATHO_LIBELLE_LEN) && (libelle[k] == '|'))
            traiteOrthographe(&qualifiant[niveau_crochet], &orthographe, iDeclinaison);
          else
            return;
        }
        else
          qualifiant[niveau_crochet] += libelle[k];
        k++;
      }
    }
    else
    {
      if      ((libelle[k] == '{') && (k < PATHO_LIBELLE_LEN + 1) && (libelle[k+1] == '|'))
      {
        while ((libelle[k] != '}') && (k < PATHO_LIBELLE_LEN) && (libelle[k] != '\0'))
          k++;
      }
      else if (libelle[k] == '|')
      {
        k++;
        orthographe = "";
        while ((libelle[k] != '|') && (k < PATHO_LIBELLE_LEN) && (libelle[k] != '\0'))
        {
          orthographe += libelle[k];
          k++;
        }
        if ((k < PATHO_LIBELLE_LEN) && (libelle[k] == '|'))
        {
          if (niveau_crochet == -1)
            traiteOrthographe(&mot_debut, &orthographe, iDeclinaison);
          else
            traiteOrthographe(&mot_fin, &orthographe, iDeclinaison);
        }
        else
          return;
      }
      else
      {
        if (niveau_crochet == -1)
          mot_debut += libelle[k];
        else
          mot_fin   += libelle[k];
      }
    }
  }
#else
  for (size_t k = 0; (k < strlen(sLabel.c_str())); k++)
  {
    if ('[' == sLabel[k])
    {
      niveau_crochet++;
      k++;
      while ((k < strlen(sLabel.c_str())) && (']' != sLabel[k]))
      {
        if ('|' == sLabel[k])
        {
          orthographe = "";
          k++;
          while ((k < strlen(sLabel.c_str())) && ('|' != sLabel[k]))
          {
            orthographe += sLabel[k];
            k++;
          }
          if ((k < strlen(sLabel.c_str())) && ('|' == sLabel[k]))
            traiteOrthographe(&qualifiant[niveau_crochet], &orthographe, iDeclinaison);
          else
            return;
        }
        else
          qualifiant[niveau_crochet] += sLabel[k];
        k++;
      }
    }
    else
    {
      if      ((k < strlen(sLabel.c_str()) - 1) && ('{' == sLabel[k]) && ('|' == sLabel[k+1]))
      {
        while ((k < strlen(sLabel.c_str())) && ('}' != sLabel[k]))
          k++;
      }
      else if ('|' == sLabel[k])
      {
        k++;
        orthographe = "";
        while ((k < strlen(sLabel.c_str())) && ('|' != sLabel[k]))
        {
          orthographe += sLabel[k];
          k++;
        }
        if ((k < strlen(sLabel.c_str())) && ('|' == sLabel[k]))
        {
          if (-1 == niveau_crochet)
            traiteOrthographe(&mot_debut, &orthographe, iDeclinaison);
          else
            traiteOrthographe(&mot_fin, &orthographe, iDeclinaison);
        }
        else
          return;
      }
      else
      {
        if (-1 == niveau_crochet)
          mot_debut += sLabel[k];
        else
          mot_fin   += sLabel[k];
      }
    }
  }
#endif

  // On assemble le nouveau libelle
  char dernCar = '\0';
  char premCar = '\0';

  if (!(mot_debut == ""))
    strip(mot_debut, stripBoth);

  if (!(mot_fin == ""))
    strip(mot_fin, stripBoth);

  *pLibelle = qualifiant[0];
  if (qualifiant[0] != "")
    dernCar = qualifiant[0][strlen(qualifiant[0].c_str())-1];

  if (qualifiant[1] != "")
  {
    premCar = qualifiant[1][0];

    if ((premCar != ' ') && (dernCar != '-') && (dernCar != '\'') && (dernCar != ' '))
      *pLibelle += " ";

    *pLibelle += qualifiant[1];

    dernCar = qualifiant[1][strlen(qualifiant[1].c_str()) - 1];
  }

  if (mot_debut != "")
  {
    premCar = mot_debut[0];

    if (*pLibelle != "" )
      if ((premCar != ' ') && (dernCar != '-') && (dernCar != '\'') && (dernCar != ' '))
        *pLibelle += " ";

    *pLibelle += mot_debut;
  }

  // On supprime les blancs terminaux
  strip(*pLibelle, stripRight);

  size_t positionAccolades = pLibelle->find("{");
  size_t positionVideAccolades = pLibelle->find(" {");
  if      (positionVideAccolades != NPOS)
    *pLibelle = string(*pLibelle, 0, positionVideAccolades);
  else if (positionAccolades != NPOS)
    *pLibelle = string(*pLibelle, 0, positionAccolades);

  // On vérifie qu'il n'existe pas plusieurs blancs contigus
  size_t positBlanc = pLibelle->find(" ");
  size_t debut, fin, taille;
  while (positBlanc != NPOS)
  {
    debut  = positBlanc;

    taille = strlen(pLibelle->c_str());
    if ((debut < taille - 1) && ((*pLibelle)[debut + 1] == ' '))
    {
      fin = debut + 1;
      while ((fin < taille - 1) && ((*pLibelle)[fin + 1] == ' '))
        fin++;

      *pLibelle = string(*pLibelle, 0, debut) + " " + string(*pLibelle, fin + 1, taille - fin - 1);
    }
    else
      fin = debut;

    debut++;
    positBlanc = pLibelle->find(" ", debut);
  }

  // On supprime les blancs terminaux
  strip(*pLibelle, stripRight);
}

// -----------------------------------------------------------------------------
// NSPathologData::traiteOrthographe(string* pMot, string* pOrthographe, int GenreMaitre)
//
// Description  : Donne le "beau" libellé
//                La variable GenreMaitre sert à donner un genre aux adjectifs
// -----------------------------------------------------------------------------
void
NSPathologData::traiteOrthographe(string* pMot, string* pOrthographe, int iDeclinaison)
{
  if (NULL == pMot)
    return;

  // simple précaution
  strip(*pMot, stripRight, ' ');

  // Si on demande la déclinaison 0, il n'y a rien à faire
  if (iDeclinaison == 0)
    return;

  if (NULL == pOrthographe)
    return;

  int iLen = strlen(pOrthographe->c_str());

  // Cas simple, pas de déclinaisons, on ne fait rien
  if (0 == iLen)
    return;

  // distribution des déclinaisons dans les différentes cases d'un vecteur
  vector<string>            vOrthographe;
  vector<string>::iterator  ivOrtho;

  int    iNbElmnt = 0;
  string sOrtho   = string("");
  for (int i = 0; i < iLen; i++)
  {
    if ('/' == (*pOrthographe)[i])
    {
      vOrthographe.push_back(string(sOrtho, 0, strlen(sOrtho.c_str())));
      sOrtho = string("");
      iNbElmnt++;
    }
    else
      sOrtho += (*pOrthographe)[i];
  }

  vOrthographe.push_back(string(sOrtho, 0, strlen(sOrtho.c_str())));
  iNbElmnt++;

  // Cas où il n'existe qu'une seule déclinaison :
  //
  //      si iDeclinaison == 0, rien à faire
  //      si iDeclinaison == 1, on ajoute la déclinaison au mot
  if (1 == iNbElmnt)
  {
    if (1 == iDeclinaison)
      *pMot = *pMot + *(vOrthographe.begin());
    return;
  }

  // Cas où il existe plusieurs déclinaisons :
  //      on détermine la racine à partir de la déclinaison 0
  //      puis on ajoute la déclinaison demandée

  // Recherche de la iDeclinaisonième déclinaison

  int i = 0;
  for (ivOrtho = vOrthographe.begin(); (i < iDeclinaison) && (ivOrtho != vOrthographe.end()); i++, ivOrtho++)
   ;

  // On vérifie quand même qu'elle existe
  if ((i < iDeclinaison) || (vOrthographe.end() == ivOrtho))
    return;

  int iRaciLen = strlen(pMot->c_str()) - strlen(vOrthographe.begin()->c_str());
  if (iRaciLen <= 0)
  {
    *pMot = *ivOrtho;
    return;
  }

  *pMot = string(*pMot, 0, iRaciLen) + *ivOrtho;
}

// -----------------------------------------------------------------------------
//  Function:    NSPathologData::donneGenre(int *pGenre)
//
//  Description: Donne le "beau" libellé
// -----------------------------------------------------------------------------
void
NSPathologData::donneGenre(GENRE *pGenre)
{
  if ((GENRE *) NULL == pGenre)
    return;

#ifndef _ENTERPRISE_DLL
  string sGenre = string(grammaire);
#else
  string sGenre = sGrammar;
#endif

  strip(sGenre);

  if 	    (string("FS") == sGenre)
    *pGenre = genreFS;
  else if (string("MS") == sGenre)
    *pGenre = genreMS;
  else if (string("NS") == sGenre)
    *pGenre = genreNS;
  else if (string("FP") == sGenre)
    *pGenre = genreFP;
  else if (string("MP") == sGenre)
    *pGenre = genreMP;
  else if (string("NP") == sGenre)
    *pGenre = genreNP;
  else
    *pGenre = genreNull;
}

void
NSPathologData::donneGenrePluriel(GENRE *pGenre)
{
  if ((GENRE *) NULL == pGenre)
    return;

  if      (genreFS == *pGenre)
    *pGenre = genreFP;
  else if (genreMS == *pGenre)
    *pGenre = genreMP;
  else if (genreNS == *pGenre)
    *pGenre = genreNP;
  else
    *pGenre = genreNull;
}

void
NSPathologData::donneGenreSingulier(GENRE *pGenre)
{
  if ((GENRE *) NULL == pGenre)
    return;

  if      (genreFP == *pGenre)
    *pGenre = genreFS;
  else if (genreMP == *pGenre)
    *pGenre = genreMS;
  else if (genreNP == *pGenre)
    *pGenre = genreNS;
  else
    *pGenre = genreNull;
}

// -----------------------------------------------------------------------------
// Function : NSPathologData::chercheGrammaire(string sAChercher)
//
// Cherche dans le libelle le commentaire grammatical ( {|__} ) commencant par
// sAChercher, et le renvoie.
// -----------------------------------------------------------------------------
string
NSPathologData::chercheGrammaire(string sAChercher)
{
#ifndef _ENTERPRISE_DLL
  string sLibelleBrut = string(libelle);
#else
  string sLibelleBrut = sLabel;
#endif

  size_t positionAccolades = sLibelleBrut.find("{|");
  if (positionAccolades == string::npos)
    return string("");
  size_t finAccolades = sLibelleBrut.find("}", positionAccolades + 1);
  if ((finAccolades == string::npos) || (finAccolades-positionAccolades < 4))
    return "";

  // On recupere dans sGrammaire le contenu entre {| et }
  // On analysera par la suite sGrammaire
  string sGrammaire = string(sLibelleBrut, positionAccolades + 2, finAccolades-positionAccolades - 2);
  size_t pos = sGrammaire.find(sAChercher);
  // size_t len = strlen(sAChercher.c_str());

  if (string::npos == pos)
    return("");

  // On recherche la fin de la string cherchee
  // Puis on renvoie la string cherchee
  if (pos == strlen(sGrammaire.c_str()))
    return string(sGrammaire, pos, 1);

  size_t posfin = sGrammaire.find("/", pos + 1);
  if (string::npos == posfin)
    return string(sGrammaire, pos, strlen(sGrammaire.c_str()) - pos);

  return string(sGrammaire, pos, posfin - pos);
}

bool
NSPathologData::estReel()
{
  return true;
}

// -----------------------------------------------------------------------------
// Constructeur
// -----------------------------------------------------------------------------
NSPatholog::NSPatholog(NSSuper* pSuper, string sLang)
#ifndef _ENTERPRISE_DLL
           :NSFiche(pSuper)
#else
           :NSSuperRoot(pSuper)
#endif
{
  _sLangue = sLang;

#ifdef _ENTERPRISE_DLL
  if      (string("") == _sLangue)
    _sTableName = "lexiq_fr";
  else
    _sTableName = string("lexiq_") + _sLangue;
#else
  if      (_sLangue == "")
    sNomFichier = "LEXIQUE.DB";
  else if (_sLangue == "fr")
    sNomFichier = "LEXIQUE.DB";
  else
    sNomFichier = string("LEXIQ_") + _sLangue + string(".DB");
#endif
}

// -----------------------------------------------------------------------------
// Constructeur copie
// -----------------------------------------------------------------------------
NSPatholog::NSPatholog(NSPatholog& rv)
#ifndef _ENTERPRISE_DLL
           :NSFiche(rv._pSuper)
#else
           :NSSuperRoot(rv._pSuper)
#endif
{
	// Copie les valeurs du NSPathologInfo d'origine
	_Donnees    = rv._Donnees;
  _sLangue    = rv._sLangue;

#ifdef _ENTERPRISE_DLL
  _sTableName = rv._sTableName;
#endif
}

// -----------------------------------------------------------------------------
// Destructeur
// -----------------------------------------------------------------------------
NSPatholog::~NSPatholog()
{
}

// -----------------------------------------------------------------------------
// Fonction     : NSPatholog::alimenteFiche()
// Description  : Transfère le contenu de pRecBuff dans les variables de la fiche
// -----------------------------------------------------------------------------
void
NSPatholog::alimenteFiche()
{
#ifndef _ENTERPRISE_DLL
  alimenteChamp(_Donnees.libelle,    PATHO_LIBELLE_FIELD,   PATHO_LIBELLE_LEN);
  alimenteChamp(_Donnees.code, 	    PATHO_CODE_FIELD, 	   PATHO_CODE_LEN);
	alimenteChamp(_Donnees.grammaire,  PATHO_GRAMMAIRE_FIELD, PATHO_GRAMMAIRE_LEN);
  alimenteChamp(_Donnees.freq, 	    PATHO_FREQ_FIELD, 	   PATHO_FREQ_LEN);

	_Donnees.fabriqueLibelleLong();
#endif
}

#ifdef _ENTERPRISE_DLL
void
NSPatholog::alimenteFiche(MYSQL_ROW *pRow)
{
  _Donnees.setLabel((*pRow)[PATHO_LIBELLE_FIELD-1]);
  _Donnees.setCode((*pRow)[PATHO_CODE_FIELD-1]);
	_Donnees.setGrammar((*pRow)[PATHO_GRAMMAIRE_FIELD-1]);
  _Donnees.setFrequency((*pRow)[PATHO_FREQ_FIELD-1]);

	_Donnees.fabriqueLibelleLong();
}
#endif

// -----------------------------------------------------------------------------
//  Fonction    : NSPatholog::videFiche()
// Description  : Transfère le contenu des valeurs de la fiche dans pRecBuff
// -----------------------------------------------------------------------------
void
NSPatholog::videFiche()
{
#ifndef _ENTERPRISE_DLL
	videChamp(_Donnees.libelle,   PATHO_LIBELLE_FIELD,   PATHO_LIBELLE_LEN);
  videChamp(_Donnees.code, 	   PATHO_CODE_FIELD, 	    PATHO_CODE_LEN);
	videChamp(_Donnees.grammaire, PATHO_GRAMMAIRE_FIELD, PATHO_GRAMMAIRE_LEN);
	videChamp(_Donnees.freq, 	   PATHO_FREQ_FIELD, 	    PATHO_FREQ_LEN);
#endif
}

// -----------------------------------------------------------------------------
// Fonction     : NSPatholog::getRecord()
// Description  : Prend l'enregistrement en cours et assigne la valeur des
//             	  champs aux variables membres de la classe.
// Returns      : PXERR_, s'il y a lieu
// -----------------------------------------------------------------------------
#ifndef _ENTERPRISE_DLL
DBIResult
NSPatholog::getPatRecord()
{
	// La table est-elle ouverte ?
  if (!isOpen)
    return(lastError = ERROR_TABLENOTOPEN);

	// Appel de la classe de base pour récupérer l'enregistrement.
	lastError = getDbiRecord(dbiWRITELOCK);
  return (lastError);
}
#endif

#ifdef _ENTERPRISE_DLL
string
NSPatholog::getTableName(string sLanguage)
{
  string sTableName = string("lexiq_fr");

  if (string("") != _sLangue)
    sTableName = string("lexiq_") + _sLangue;

  return sTableName;
}

bool
NSPatholog::executeQuery(string sQuery, bool bVerbose)
{
  if (string("") == sQuery)
    return false;

  if (NULL == _pSuper->getDatabaseManager())
    return false;

  MYSQL_RES *pResult = _pSuper->getDatabaseManager()->executeQuery(sQuery, bVerbose);

  if ((MYSQL_RES *) NULL == pResult)
    return false;

  if (mysql_num_rows(pResult) == 0)
    return false;

  bool bSucces = true;

  MYSQL_ROW Row = mysql_fetch_row(pResult);
  if (Row)
    alimenteFiche(&Row);
  else if (bVerbose)
  {
    string sErreurMsg = string("Error when fetching row for query [") + sQuery + string("] -> ") + mysql_error(_pSuper->getDatabaseManager()->getConnector());
    error(sErreurMsg.c_str(), standardError);
    bSucces = false;
  }

  mysql_free_result(pResult);

  return bSucces;
}
#endif

// -----------------------------------------------------------------------------
// Renvoie la fiche NSPathologData correspondant à ce code
// -----------------------------------------------------------------------------
bool
#ifndef _ENTERPRISE_DLL

NSPatholog::trouvePathologData(string* pCode, NSPathologData* pData, bool afficheErreur)
{
  if (((string *) NULL == pCode) || (string("") == *pCode) || ((NSPathologData *) NULL == pData))
    return false;

  lastError = chercheClef(pCode, "CODE_INDEX", keySEARCHEQ, dbiWRITELOCK);

  if (lastError != DBIERR_NONE)
	{
    if (afficheErreur)
    {
    	string sErreurMsg = string("Erreur à la recherche du code [") + *pCode + string("]");
            error(sErreurMsg.c_str(), standardError, lastError);
    }
		return false;
	}

	lastError = getRecord();
	if (lastError != DBIERR_NONE)
	{
        error("Erreur à la lecture du lexique.", standardError, lastError);
		return false;
	}

  *pData = _Donnees;

#else

NSPatholog::trouvePathologData(string sLang, string* pCode, NSPathologData* pData, bool afficheErreur)
{
  if (((string *) NULL == pCode) || (string("") == *pCode) || ((NSPathologData *) NULL == pData))
    return false;

  string sQuery = string("SELECT * FROM ") + getTableName(sLang) + string(" WHERE code = \"") + *pCode + string("\";");

  if (false == executeQuery(sQuery, afficheErreur))
    return false;

  *pData = _Donnees;

#endif
	return true;
}


// -----------------------------------------------------------------------------
// Description  : Trouve la première fiche dont le code convient
//             	  Transfère la fiche dans pDonnees
//
// Returns      : PXERR_, s'il y a lieu
// -----------------------------------------------------------------------------
#ifndef _ENTERPRISE_DLL
DBIResult
NSPatholog::trouveCode(string* pCode, DBISearchCond searchMode, DBILockType Blocage, bool bErrMessage)
{
  lastError = chercheClef(pCode, "CODE_INDEX", searchMode, Blocage);
  if (lastError != DBIERR_NONE)
	{
    char msg[255];
    string sCode;

    if ((*pCode) == "")
      sCode = "vide";
    else
      sCode = *pCode;

    if (bErrMessage)
    {
      sprintf(msg, "Erreur à la recherche du code : [%s]", sCode.c_str());
          error(msg, standardError, lastError);
    }
		return lastError;
	}
	lastError = getRecord();
	if (lastError != DBIERR_NONE)
	{
    if (bErrMessage)
          error("Erreur à la lecture du lexique.", standardError, lastError);
		return lastError;
	}
  return (lastError);
}
#else
bool
NSPatholog::trouveCode(string sLang, string* pCode, bool bVerbose)
{
  if ((string *) NULL == pCode)
    return false;

  string sQuery = string("SELECT * FROM ") + getTableName(sLang) + string(" WHERE code = \"") + *pCode + string("\";");

  if (false == executeQuery(sQuery, bVerbose))
    return false;

  return true;
}
#endif

// -----------------------------------------------------------------------------
// Description  : Trouve la première fiche dont le libellé convient
//             	  Transfère la fiche dans pDonnees
// Returns      : PXERR_, s'il y a lieu
// -----------------------------------------------------------------------------
#ifndef _ENTERPRISE_DLL
DBIResult
NSPatholog::trouveLibelle(string* pLibelle, DBISearchCond searchMode, DBILockType Blocage)
{
  lastError = chercheClef((unsigned char *)pLibelle->c_str(), "", 0, searchMode, Blocage);
  if (lastError != DBIERR_NONE)
	{
        error("Erreur à la recherche de l'ammorce.", standardError, lastError);
		return lastError;
	}

	lastError = getRecord();
	if (lastError != DBIERR_NONE)
	{
        error("Erreur à la lecture du lexique.", standardError, lastError);
		return lastError;
	}
  return (lastError);
}
#else
bool
NSPatholog::trouveLibelle(string sLang, string* pLibelle)
{
  if ((string *) NULL == pLibelle)
    return false;

  string sQuery = string("SELECT * FROM ") + getTableName(sLang) + string(" WHERE libelle = \"") + *pLibelle + string("\";");

  if (false == executeQuery(sQuery, true))
    return false;

  return true;
}

bool
NSPatholog::trouveLibelleApproche(string sLang, string* pLibelle)
{
  if ((string *) NULL == pLibelle)
    return false;

  string sQuery = string("SELECT * FROM ") + getTableName(sLang) + string(" WHERE libelle LIKE \"") + *pLibelle + string("%\";");

  if (false == executeQuery(sQuery, true))
    return false;

  return true;
}
#endif

// -----------------------------------------------------------------------------
// Fonction     : NSPatient::open()
// Description  : Ouvre la table primaire et les index secondaires
// Returns      : PXERR_, s'il y a lieu
// -----------------------------------------------------------------------------
#ifndef _ENTERPRISE_DLL
DBIResult
NSPatholog::open()
{
	//char tableName[] = "LEXIQUE.DB";

	// Appelle la fonction open() de la classe de base pour ouvrir
	// l'index primaire
	lastError = NSFiche::open(/*tableName*/sNomFichier.c_str(), NSF_GUIDES);
	return (lastError);
}
#endif

// -----------------------------------------------------------------------------
// Function     : NSPatholog::Create()
// -----------------------------------------------------------------------------
bool
NSPatholog::Create()
{
	return true;
}


// -----------------------------------------------------------------------------
// Function     : NSPatholog::Modify()
// -----------------------------------------------------------------------------
bool
NSPatholog::Modify()
{
	return true;
}

// -----------------------------------------------------------------------------
// Opérateur d'affectation
// -----------------------------------------------------------------------------
NSPatholog&
NSPatholog::operator=(NSPatholog src)
{
  if (this == &src)
		return *this;

	_Donnees = src._Donnees;
  _sLangue = src._sLangue;

	return *this;
}

// -----------------------------------------------------------------------------
// Opérateur de comparaison
// -----------------------------------------------------------------------------
int
NSPatholog::operator==(const NSPatholog& o)
{
  return (_Donnees == o._Donnees);
}

// -----------------------------------------------------------------------------
// Fonction     : NSPathologInfo::NSPathologInfo()
// Description  : Constructeur par défaut
// -----------------------------------------------------------------------------
NSPathologInfo::NSPathologInfo()
{
}

// -----------------------------------------------------------------------------
// Fonction     : NSPathologInfo::NSPathologInfo(NSPatholog *)
// Description  : Constructeur à partir d'un NSPatient
// -----------------------------------------------------------------------------
NSPathologInfo::NSPathologInfo(NSPatholog* pPatho)
{
  if (NULL == pPatho)
    return;

	// Copie les valeurs du NSDocument
	_Donnees = pPatho->_Donnees;
}

// -----------------------------------------------------------------------------
// Constructeur copie
// -----------------------------------------------------------------------------
NSPathologInfo::NSPathologInfo(NSPathologInfo& rv)
{
	// Copie les valeurs du NSPathologInfo d'origine
	_Donnees = rv._Donnees;
}

// -----------------------------------------------------------------------------
// Destructeur
// -----------------------------------------------------------------------------
NSPathologInfo::~NSPathologInfo()
{
}

// -----------------------------------------------------------------------------
// Opérateur d'affectation
// -----------------------------------------------------------------------------
NSPathologInfo&
NSPathologInfo::operator=(NSPathologInfo src)
{
  if (this == &src)
		return *this;

	_Donnees = src._Donnees;

	return *this;
}

// -----------------------------------------------------------------------------
// Opérateur de comparaison
// -----------------------------------------------------------------------------
int
NSPathologInfo::operator==(const NSPathologInfo& o)
{
  return (_Donnees == o._Donnees);
}

// -----------------------------------------------------------------------------
//
// Implémentation des méthodes NSSavoir
//
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Function     : NSSavoirData::NSSavoirData()
// Description  : Constructeur.
// -----------------------------------------------------------------------------
NSSavoirData::NSSavoirData()
{
#ifndef _ENTERPRISE_DLL
	// Met les champs de données à zéro
	memset(code, 		    0, SAVOIR_CODE_LEN + 1);
  memset(qualifie, 	  0, SAVOIR_QUALIFIE_LEN + 1);
  memset(lien, 		    0, SAVOIR_LIEN_LEN + 1);
  memset(qualifiant,  0, SAVOIR_QUALIFIANT_LEN + 1);
  memset(classe, 	    0, SAVOIR_CLASSE_LEN + 1);
  memset(degre, 	 	  0, SAVOIR_DEGRE_LEN + 1);
  memset(scenario, 	  0, SAVOIR_SCENARIO_LEN + 1);
#else
  _sCode      = string("");
	_sQualified = string("");
	_sLink      = string("");
	_sQualifier = string("");
	_sLevel     = string("");
	_sClass     = string("");
	_sScenario  = string("");
#endif
}

// -----------------------------------------------------------------------------
// Fonction     : NSPathologData::NSSavoirData(NSSavoirData& rv)
// Description  : Constructeur copie
// Retour       : Rien
// -----------------------------------------------------------------------------
NSSavoirData::NSSavoirData(NSSavoirData& rv)
{
#ifndef _ENTERPRISE_DLL
	strcpy(code, 		    rv.code);
  strcpy(qualifie, 	  rv.qualifie);
  strcpy(lien, 		    rv.lien);
  strcpy(qualifiant,  rv.qualifiant);
  strcpy(degre, 	 	  rv.degre);
  strcpy(classe, 	    rv.classe);
  strcpy(scenario, 	  rv.scenario);
#else
  _sCode      = rv._sCode;
	_sQualified = rv._sQualified;
	_sLink      = rv._sLink;
	_sQualifier = rv._sQualifier;
	_sLevel     = rv._sLevel;
	_sClass     = rv._sClass;
	_sScenario  = rv._sScenario;
#endif
}

// -----------------------------------------------------------------------------
// Fonction     :	NSSavoirData::~NSSavoirData()
// Description  : Destructeur
// -----------------------------------------------------------------------------
NSSavoirData::~NSSavoirData()
{
}

// -----------------------------------------------------------------------------
// Fonction     : NSSavoirData::operator=(NSSavoirData src)
// Description  : Opérateur =
// Retour       : Référence sur l'objet cible
// -----------------------------------------------------------------------------
NSSavoirData&
NSSavoirData::operator=(NSSavoirData rv)
{
  if (&rv == this)
		return *this;

#ifndef _ENTERPRISE_DLL
	strcpy(code, 		    rv.code);
  strcpy(qualifie, 	  rv.qualifie);
  strcpy(lien, 		    rv.lien);
  strcpy(qualifiant,  rv.qualifiant);
  strcpy(degre, 	 	  rv.degre);
  strcpy(classe, 	    rv.classe);
  strcpy(scenario, 	  rv.scenario);
#else
  _sCode      = rv._sCode;
	_sQualified = rv._sQualified;
	_sLink      = rv._sLink;
	_sQualifier = rv._sQualifier;
	_sLevel     = rv._sLevel;
	_sClass     = rv._sClass;
	_sScenario  = rv._sScenario;
#endif

	return *this;
}

// -----------------------------------------------------------------------------
// Fonction     :	NSSavoirData::operator=(NSSavoirData src)
// Description  : Opérateur de comparaison
// Retour       : 0 ou 1
// -----------------------------------------------------------------------------
int
NSSavoirData::operator==(const NSSavoirData& o)
{
#ifndef _ENTERPRISE_DLL
	if ((strcmp(code, o.code) == 0))
#else
  if (_sCode == o._sCode)
#endif
		return 1;
	else
		return 0;
}

// -----------------------------------------------------------------------------
// Function     : NSSavoir::NSSavoir(NSSuper* pSuper)
// Description  : Constructeur.
// -----------------------------------------------------------------------------
NSSavoir::NSSavoir(NSSuper* pSuper)
#ifndef _ENTERPRISE_DLL
         :NSFiche(pSuper)
#else
         :NSSuperRoot(pSuper)
#endif
{
	// Crée un objet de données
	pDonnees = new NSSavoirData();
}

// -----------------------------------------------------------------------------
// Constructeur copie
// -----------------------------------------------------------------------------
NSSavoir::NSSavoir(NSSavoir& rv)
#ifndef _ENTERPRISE_DLL
         :NSFiche(rv._pSuper)
#else
         :NSSuperRoot(rv._pSuper)
#endif
{
	// Crée l'objet de données
	pDonnees = new NSSavoirData();

	// Copie les valeurs du NSSavoirInfo d'origine
	*pDonnees = *(rv.pDonnees);
}

// -----------------------------------------------------------------------------
// Function     : NSSavoir::~NSSavoir()
// Description  : Destructeur.
// -----------------------------------------------------------------------------
NSSavoir::~NSSavoir()
{
	delete pDonnees;
}

// -----------------------------------------------------------------------------
// Fonction     : NSSavoir::alimenteFiche()
// Description  : Transfère le contenu de pRecBuff dans les variables de la fiche
// -----------------------------------------------------------------------------
void
NSSavoir::alimenteFiche()
{
#ifndef _ENTERPRISE_DLL
	alimenteChamp(pDonnees->code, 		  SAVOIR_CODE_FIELD, 	  	 SAVOIR_CODE_LEN);
  alimenteChamp(pDonnees->qualifie, 	SAVOIR_QUALIFIE_FIELD, 	 SAVOIR_QUALIFIE_LEN);
  alimenteChamp(pDonnees->lien, 		  SAVOIR_LIEN_FIELD, 	  	 SAVOIR_LIEN_LEN);
  alimenteChamp(pDonnees->qualifiant, SAVOIR_QUALIFIANT_FIELD, SAVOIR_QUALIFIANT_LEN);
  alimenteChamp(pDonnees->degre, 	  	SAVOIR_DEGRE_FIELD,   	 SAVOIR_DEGRE_LEN);
  alimenteChamp(pDonnees->classe, 	  SAVOIR_CLASSE_FIELD,   	 SAVOIR_CLASSE_LEN);
  alimenteChamp(pDonnees->scenario, 	SAVOIR_SCENARIO_FIELD,   SAVOIR_SCENARIO_LEN);
#endif
}

#ifdef _ENTERPRISE_DLL
void
NSSavoir::alimenteFiche(MYSQL_ROW *pRow)
{
  if ((MYSQL_ROW *) NULL == pRow)
    return;

  if ((*pRow)[SAVOIR_CODE_FIELD-1])
    pDonnees->setCode((*pRow)[SAVOIR_CODE_FIELD-1]);
  if ((*pRow)[SAVOIR_QUALIFIE_FIELD-1])
    pDonnees->setQualified((*pRow)[SAVOIR_QUALIFIE_FIELD-1]);
  if ((*pRow)[SAVOIR_LIEN_FIELD-1])
	  pDonnees->setLink((*pRow)[SAVOIR_LIEN_FIELD-1]);
  if ((*pRow)[SAVOIR_QUALIFIANT_FIELD-1])
    pDonnees->setQualifier((*pRow)[SAVOIR_QUALIFIANT_FIELD-1]);
  if ((*pRow)[SAVOIR_DEGRE_FIELD-1])
    pDonnees->setLevel((*pRow)[SAVOIR_DEGRE_FIELD-1]);
  if ((*pRow)[SAVOIR_CLASSE_FIELD-1])
	  pDonnees->setClass((*pRow)[SAVOIR_CLASSE_FIELD-1]);
  if ((*pRow)[SAVOIR_SCENARIO_FIELD-1])
    pDonnees->setScenario((*pRow)[SAVOIR_SCENARIO_FIELD-1]);
}
#endif

// -----------------------------------------------------------------------------
// Fonction     : NSSavoir::videFiche()
// Description  : Transfère le contenu des valeurs de la fiche dans pRecBuff
// -----------------------------------------------------------------------------
void
NSSavoir::videFiche()
{
#ifndef _ENTERPRISE_DLL
	videChamp(pDonnees->code, 		  SAVOIR_CODE_FIELD, 	  	  SAVOIR_CODE_LEN);
  videChamp(pDonnees->qualifie,   SAVOIR_QUALIFIE_FIELD, 	  SAVOIR_QUALIFIE_LEN);
  videChamp(pDonnees->lien, 		  SAVOIR_LIEN_FIELD, 	  	  SAVOIR_LIEN_LEN);
  videChamp(pDonnees->qualifiant, SAVOIR_QUALIFIANT_FIELD,  SAVOIR_QUALIFIANT_LEN);
  videChamp(pDonnees->degre, 	    SAVOIR_DEGRE_FIELD,   	  SAVOIR_DEGRE_LEN);
  videChamp(pDonnees->classe, 	  SAVOIR_CLASSE_FIELD,   	  SAVOIR_CLASSE_LEN);
  videChamp(pDonnees->scenario,   SAVOIR_SCENARIO_FIELD,    SAVOIR_SCENARIO_LEN);
#endif
}

#ifdef _ENTERPRISE_DLL
bool
NSSavoir::executeQuery(string sQuery, bool bVerbose)
{
  if (string("") == sQuery)
    return false;

  if (NULL == _pSuper->getDatabaseManager())
    return false;

  MYSQL_RES *pResult = _pSuper->getDatabaseManager()->executeQuery(sQuery, bVerbose);

  if ((MYSQL_RES *) NULL == pResult)
    return false;

  if (mysql_num_rows(pResult) == 0)
    return false;

  bool bSucces = true;

  MYSQL_ROW Row = mysql_fetch_row(pResult);
  if (Row)
    alimenteFiche(&Row);
  else if (bVerbose)
  {
    string sErreurMsg = string("Error when fetching row for query [") + sQuery + string("] -> ") + mysql_error(_pSuper->getDatabaseManager()->getConnector());
    error(sErreurMsg.c_str(), standardError);
    bSucces = false;
  }

  mysql_free_result(pResult);

  return bSucces;
}
#endif

// -----------------------------------------------------------------------------
// Fonction     : NSSavoir::getRecord()
// Description  : Prend l'enregistrement en cours et assigne la valeur des
//             	  champs aux variables membres de la classe.
// Returns      : PXERR_, s'il y a lieu
// -----------------------------------------------------------------------------
#ifndef _ENTERPRISE_DLL
DBIResult
NSSavoir::getPatRecord()
{
	// La table est-elle ouverte ?
	if (!isOpen)
	  return (lastError = ERROR_TABLENOTOPEN);

	// Appel de la classe de base pour récupérer l'enregistrement.
	lastError = getDbiRecord(dbiWRITELOCK);

	return (lastError);
}
#endif

// -----------------------------------------------------------------------------
// Description  : Ouvre la table primaire et les index secondaires
// Returns      : PXERR_, s'il y a lieu
// -----------------------------------------------------------------------------
#ifndef _ENTERPRISE_DLL
DBIResult
NSSavoir::open()
{
	char tableName[] = "SAVOIR.DB";

	// Appelle la fonction open() de la classe de base pour ouvrir
	// l'index primaire
	lastError = NSFiche::open(tableName, NSF_GUIDES);
	return (lastError);
}
#endif

// -----------------------------------------------------------------------------
// Function     : NSSavoir::Create()
// -----------------------------------------------------------------------------
bool
NSSavoir::Create()
{
	return true;
}


// -----------------------------------------------------------------------------
// Function     : NSSavoir::Modify()
// -----------------------------------------------------------------------------
bool
NSSavoir::Modify()
{
	return true;
}


// -----------------------------------------------------------------------------
// Opérateur d'affectation
// -----------------------------------------------------------------------------
NSSavoir&
NSSavoir::operator=(NSSavoir src)
{
  if (this == &src)
		return *this;

	*pDonnees = *(src.pDonnees);
  
	return *this;
}


// -----------------------------------------------------------------------------
// Opérateur de comparaison
// -----------------------------------------------------------------------------
int
NSSavoir::operator==(const NSSavoir& o)
{
  return (*pDonnees == *(o.pDonnees));
}


// -----------------------------------------------------------------------------
// Constructeur par défaut
// -----------------------------------------------------------------------------
NSSavoirInfo::NSSavoirInfo()
{
	// Crée l'objet de données
	pDonnees = new NSSavoirData();
}


// -----------------------------------------------------------------------------
// Constructeur à partir d'un NSPatient
// -----------------------------------------------------------------------------
NSSavoirInfo::NSSavoirInfo(NSSavoir* pPatho)
{
	// Crée l'objet de données
	pDonnees = new NSSavoirData();

	// Copie les valeurs du NSDocument
	*pDonnees = *(pPatho->pDonnees);
}


// -----------------------------------------------------------------------------
// Constructeur copie
// -----------------------------------------------------------------------------
NSSavoirInfo::NSSavoirInfo(NSSavoirInfo& rv)
{
	// Crée l'objet de données
	pDonnees = new NSSavoirData();

	// Copie les valeurs du NSSavoirInfo d'origine
	*pDonnees = *(rv.pDonnees);
}


// -----------------------------------------------------------------------------
// Destructeur.
// -----------------------------------------------------------------------------
NSSavoirInfo::~NSSavoirInfo()
{
  delete pDonnees;
}


// -----------------------------------------------------------------------------
// Opérateur d'affectation
// -----------------------------------------------------------------------------
NSSavoirInfo&
NSSavoirInfo::operator=(NSSavoirInfo src)
{
  if (this == &src)
		return *this;

	*pDonnees = *(src.pDonnees);
  
	return *this;
}


// -----------------------------------------------------------------------------
// Opérateur de comparaison
// -----------------------------------------------------------------------------
int
NSSavoirInfo::operator==(const NSSavoirInfo& o)
{
  return (*pDonnees == *(o.pDonnees));
}


// -----------------------------------------------------------------------------
//
// BASE DES MEDICAMENTS
//
// -----------------------------------------------------------------------------


// -----------------------------------------------------------------------------
// Constructeur
// -----------------------------------------------------------------------------
NSLexiMed::NSLexiMed(NSSuper* pSuper, string sLangue)
          :NSPatholog(pSuper, sLangue)
{
}


// -----------------------------------------------------------------------------
// Constructeur copie
// -----------------------------------------------------------------------------
NSLexiMed::NSLexiMed(NSLexiMed& rv)
          :NSPatholog(rv._pSuper, rv._sLangue)
{
}


// -----------------------------------------------------------------------------
// Destructeur
// -----------------------------------------------------------------------------
NSLexiMed::~NSLexiMed()
{
}

#ifndef _ENTERPRISE_DLL
DBIResult
NSLexiMed::open()
{
	char tableName[] = "LEXI_MED.DB";

	// Appelle la fonction open() de la classe de base pour ouvrir l'index primaire
	lastError = NSFiche::open(tableName, NSF_GUIDES);

	return (lastError);
}
#endif

