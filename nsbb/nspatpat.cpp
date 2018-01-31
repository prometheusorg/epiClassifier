//---------------------------------------------------------------------------
//    NSPATPAT.CPP
//    PA   juillet 94
//    Kaddachi Hafedh
//    Implementation des objets de type patpatho
//---------------------------------------------------------------------------

#ifdef _ENTERPRISE_DLL
  #include "enterpriseLus/nsdivfctForCgi.h"
  #include "enterpriseLus/superLus.h"
  #include "enterpriseLus/personLus.h"
#else
  #include "nautilus\nssuper.h"
  #include "nssavoir\nspatho.h"
  #include "partage\nsdivfct.h"
#endif

#include "nsbb/nsbb_glo.h"
#include "nsbb/nspatpat.h"
#include "partage/stringar.h"
#include "nsbb/nsbbtran.h"
#include "nsbb/nstlibre.h"
#include "nsbb/ns_objs.h"
#include "nssavoir/nsfilgd.h"
#include "nssavoir/nsguide.h"
#include "nssavoir/nsconver.h"

long NSPatPathoData::lObjectCount = 0;
long NSPatPathoInfo::lObjectCount = 0;
long NSPatPathoArray::lObjectCount = 0;
long NSVectPatPathoArray::lObjectCount = 0;
long NSFatheredPatPathoArray::lObjectCount = 0;
long NSVectFatheredPatPathoArray::lObjectCount = 0;

// -----------------------------------------------------------------------------
// ComposeNoeudsChemin
// A partir d'un chemin "fil guide" ('/' comme seul séparateur), compose un chemin classique
// -----------------------------------------------------------------------------
void ComposeNoeudsChemin(string sCheminBrut, string& sCheminPatho)
{
  if (!(&sCheminPatho))
    return;
  sCheminPatho = string("");

  if (string("") == sCheminBrut)
    return;

  bool   bCont = true;
  size_t pos1 = 0;
  string sCode;

  size_t pos2 = sCheminBrut.find("/");

  while (bCont)
  {
    if (NPOS == pos2)
        sCode = string(sCheminBrut, pos1, strlen(sCheminBrut.c_str()) - pos1);
    else
        sCode = string(sCheminBrut, pos1, pos2-pos1);

    if ((string(sCode, 0, 3) == "WCE") ||
        (string(sCode, 0, 4) == "WPLU") ||
        ('£' == sCode[0]) || ('$' == sCode[0]))
    {
    	if ((string("") != sCheminPatho) && (string("") != sCode))
        sCheminPatho += string(1, intranodeSeparationMARK);
    }
    else
    {
    	if ((string("") != sCheminPatho) && (string("") != sCode))
        sCheminPatho += string(1, cheminSeparationMARK);
    }

    sCheminPatho += sCode;

    if (NPOS == pos2)
        break;

    pos1 = pos2 + 1;
    pos2 = sCheminBrut.find("/", pos1);
  }
}

bool
infLoc(NSPatPathoInfo* s, NSPatPathoInfo* b)
{
  if ((NULL == s) || (NULL == b))
    return false;

  return (s->getLigne() < b->getLigne());
}

// -------------------------------------------------------------------------
// -------------------- METHODES DE NSVectPatPathoArray ------------------------
// -------------------------------------------------------------------------
NSVectPatPathoArray::NSVectPatPathoArray()
                    :NSVectPatPathoInfoArray()
{
    lObjectCount++;
}

//---------------------------------------------------------------------------
//  Constructeur copie
//---------------------------------------------------------------------------
NSVectPatPathoArray::NSVectPatPathoArray(const NSVectPatPathoArray& rv)
                    :NSVectPatPathoInfoArray()
{
try
{
	if (false == rv.empty())
        for (ConstPatPathoIterVect i = rv.begin(); rv.end() != i; i++)
            push_back(new NSPatPathoArray(*(*i)));

  lObjectCount++;
}
catch (...)
{
    erreur("Exception (NSVectPatPathoArray copy ctor).", standardError);
}
}

//---------------------------------------------------------------------------
//  Destructeur
//---------------------------------------------------------------------------
void
NSVectPatPathoArray::vider()
{
	if (empty())
        return;
    for (PatPathoIterVect i = begin(); end() != i; )
	{
        delete *i;
        erase(i);
	}
}

NSVectPatPathoArray::~NSVectPatPathoArray()
{
    vider();
  
  lObjectCount--;
}

//---------------------------------------------------------------------------
//  Opérateur d'affectation
//---------------------------------------------------------------------------
NSVectPatPathoArray&
NSVectPatPathoArray::operator=(const NSVectPatPathoArray& src)
{
try
{
	if (&src == this)
        return *this;

	//
	// Effacement des éléments déjà contenus dans le vecteur destination
	//
    vider();
	//
	// Copie et insertion des éléments de la source
	//
	if (false == src.empty())
        for (ConstPatPathoIterVect i = src.begin(); src.end() != i; i++)
            push_back(new NSPatPathoArray(*(*i)));

    return *this;
}
catch (...)
{
    erreur("Exception (op = NSVectPatPathoArray).", standardError);
    return *this;
}
}

//---------------------------------------------------------------------------
//   NSVectPatPathoArray vide ou non dans le cas non multidialogue
//---------------------------------------------------------------------------
bool NSVectPatPathoArray::estVide() {
  if (empty())
    return true;

  PatPathoIterVect iter;
  for (iter = begin(); (end() != iter) && (*iter)->empty(); iter++);
	if (end() == iter)
      return true;

  return false;
}

//---------------------------------------------------------------------------
//   NSVectPatPathoArray vide ou non dans le cas multidialogue
//---------------------------------------------------------------------------
bool
NSVectPatPathoArray::MultiestVide()
{
	if (empty())
        return true;

	PatPathoIterVect iter;
    for (iter = begin(); (end() != iter) && (*iter)->empty(); iter++);
	if (end() == iter)
        return true;

    return false;
}

// *************************************************************************
// -------------------- METHODES DE NSPatPathoArray ------------------------
// **************************************************************************
NSPatPathoArray::NSPatPathoArray(NSSuper* pSuper, int iType)
                :NSPatPathoInfoArray(), NSSuperRoot(pSuper)
{
    iArrayType = iType;
    // reserve(50);
    lObjectCount++;
}

//---------------------------------------------------------------------------
//  Constructeur copie
//---------------------------------------------------------------------------
NSPatPathoArray::NSPatPathoArray(const NSPatPathoArray& rv)
                :NSPatPathoInfoArray(), NSSuperRoot(rv._pSuper)
{
try
{
  iArrayType = rv.iArrayType;

    // reserve(50);
	if (false == rv.empty())
        for (PatPathoConstIter i = rv.begin(); rv.end() != i; i++)
            push_back(new NSPatPathoInfo(*(*i)));

    lObjectCount++;
}
catch (...)
{
    erreur("Exception (NSPatPathoArray copy ctor).", standardError);
}
}

//---------------------------------------------------------------------------
//  Destructeur
//---------------------------------------------------------------------------
void
NSPatPathoArray::vider()
{
	if (empty())
        return;

    for (PatPathoIter i = begin(); end() != i; )
	{
        delete *i;
        erase(i);
	}
}

void
NSPatPathoArray::clearAllIDs()
{
	if (empty())
        return;

    for (PatPathoIter i = begin(); end() != i; i++)
	{
    (*i)->setTreeID(string(""));
        (*i)->setNodeID(string(""));
    (*i)->setPerson(string(""));
    (*i)->setDocum(string(""));
	}
}

NSPatPathoArray::~NSPatPathoArray()
{
    vider();

    lObjectCount--;
}

//---------------------------------------------------------------------------
//  Opérateur d'affectation
//---------------------------------------------------------------------------
NSPatPathoArray&
NSPatPathoArray::operator=(const NSPatPathoArray& src)
{
try
{
	if (&src == this)
        return *this;

  iArrayType = src.iArrayType;

	//
	// Effacement des éléments déjà contenus dans le vecteur destination
	//
    vider();
	//
	// Copie et insertion des éléments de la source
	//

	if (false == src.empty())
        for (PatPathoConstIter i = src.begin(); src.end() != i; i++)
            push_back(new NSPatPathoInfo(*(*i)));

    return *this;
}
catch (...)
{
  erreur("Exception (op = NSPatPathoArray).", standardError);
  return *this;
}
}

//---------------------------------------------------------------------------
//  Opérateur d'égalité
// Attention : ne compile pas si parametre const
//---------------------------------------------------------------------------
int
NSPatPathoArray::operator==(NSPatPathoArray& o)
{
try
{
  PatPathoIter i, j;
  int egal = 1;

  if (empty() || o.empty())
  {
    if (o.empty() && empty())
      return 1;
    else
      return 0;
  }

  i = begin();
  j = o.begin();

  while ((end() != i) && (o.end() != j))
  {
    if (!((*(*i)) == (*(*j))))
    {
      egal = 0;
      break;
    }

    i++;
    j++;
  }

  if (egal && ((end() != i) || (o.end() != j)))
    egal = 0;

  return egal;
}
catch (...)
{
  erreur("Exception (op == NSPatPathoArray).", standardError);
  return 0;
}
}

//---------------------------------------------------------------------------
//  Ajout d'un élément à l'array
//---------------------------------------------------------------------------
bool
NSPatPathoArray::ajouteElement(NSPatPathoInfo* pPatPatho)
{
try
{
	if (NULL == pPatPatho)
        return false;

    push_back(pPatPatho);
  
    return true;
}
catch (...)
{
    erreur("Exception NSPatPathoArray::ajouteElement.", standardError);
    return false;
}
}

//---------------------------------------------------------------------------
//  Destruction d'un élément de l'array
//---------------------------------------------------------------------------
bool
NSPatPathoArray::detruitElement(NSPatPathoInfo* pPatPatho)
{
	if ((empty()) || (NULL == pPatPatho))
        return false;

    PatPathoIter i;
	//
	// On cherche l'élément de même ID
	//
    for (i = begin(); (end() != i) && ((*i)->_ID != pPatPatho->_ID); i++);

	//
	// On supprime l'élément trouvé
	//
	if (end() != i)
	{
    delete *i;
    erase(i);
    return true;
	}
    return false;
}

//---------------------------------------------------------------------------
// Ajout du patpatho du BBFilsItem
//---------------------------------------------------------------------------
void
NSPatPathoArray::ajouteVecteur(NSPatPathoArray* pPath, int decalageColonne)
{
  if (((NSPatPathoArray *) NULL == pPath) || (pPath->empty()))
    return;

  int ligne;

  if (false == empty())
    ligne = (back())->getLigne() + 1;
  else
    ligne = ORIGINE_PATH_PATHO;

  int LigneReference = (*(pPath->begin()))->getLigne();

try
{
    for (PatPathoIter i = pPath->begin(); pPath->end() != i; )
  {
    NSPatPathoInfo* _pPatPatho = new NSPatPathoInfo(*(*i));
    string sScalaire = ((*i))->getComplement();
    _pPatPatho->setLigne(ligne);
    _pPatPatho->setColonne(_pPatPatho->getColonne() + decalageColonne);

    push_back(_pPatPatho);

    i++;
    if ((pPath->end() != i) && ((*i)->getLigne() > LigneReference))
    {
      ligne++;
      LigneReference = (*i)->getLigne();
    }
  }
} // try
catch (...)
{
    erreur("Exception (NSPatPathoArray ajouteVecteur).", standardError);
}
}

//---------------------------------------------------------------------------
// Ajout d'un élément à la patpatho en cours
// pMessage contient les données patpatho sauf le champ lexique qui est contenu
// dans sEtiquette
//---------------------------------------------------------------------------
void
NSPatPathoArray::ajoutePatho(string sEtiquette, Message* pMessage, int colonne)
{
try
{
	if (((Message *) NULL == pMessage) ||
                 (string("") == sEtiquette) || (colonne < 0))
        return;

	// En mode MUE, chaque ligne n'est occupée que par un seul noeud
    int decalageLigne = 1;

    NSPatPathoInfo* _pPatPatho = new NSPatPathoInfo();

  _pPatPatho->initFromMessage(pMessage);
    _pPatPatho->setLexique(sEtiquette);

  _pPatPatho->setColonne(colonne);

  int ligne;
	if (false == empty())
    ligne = (back())->getLigne() + decalageLigne;
  else
    ligne = ORIGINE_PATH_PATHO;
  _pPatPatho->setLigne(ligne);

  push_back(_pPatPatho);

} // try
catch (...)
{
    erreur("Exception (NSPatPathoArray ajoutePatho 1).", standardError);
}
}

//---------------------------------------------------------------------------
//ajout d'un élément à la patpatho
//---------------------------------------------------------------------------
void
NSPatPathoArray::ajoutePatho(string sEtiquette, int colonne)
{
try
{
	if ((string("") == sEtiquette) || (colonne < 0))
        return;

	// En mode MUE, chaque ligne n'est occupée que par un seul noeud
  // There is only one node per line
    int decalageLigne = 1;

  // Input can be of the kind: Node1/Node2...
  // L'étiquette peut être du type Noeud1/Noeud2...
	//
    size_t posit = sEtiquette.find(string(1, cheminSeparationMARK));
  if (NPOS != posit)
	{
    int iTaille = strlen(sEtiquette.c_str());
    string sLexique = string(sEtiquette, 0, posit);
    string sSuite   = string(sEtiquette, posit+1, iTaille - posit - 1);

    if (string("") != sLexique)
      ajoutePatho(sLexique, colonne);

    if (string("") != sSuite)
      ajoutePatho(sSuite, colonne + 1);

    return;
  }

  // Input can be of the kind: Lexique.certainty.plural
	// L'étiquette peut être du type Lexique.Certitude.Pluriel
	//
    posit = sEtiquette.find(string(1, intranodeSeparationMARK));
	//
	// Si c'est le cas, la méthode la plus propre consiste
	// à passer par un Message
	//
	if (NPOS != posit)
	{
    int iTaille = strlen(sEtiquette.c_str());
    string sLexique = string(sEtiquette, 0, posit);
    string sSuite   = string(sEtiquette, posit+1, iTaille - posit - 1);

    Message BufMessage("", sLexique);
    BufMessage.MettreAJourCertitudePluriel(sSuite);

    ajoutePatho(sLexique, &BufMessage, colonne);
    return;
  }
  //
  // Plain vanilla Lexique concept
  // Schéma simple
  //
  NSPatPathoInfo* pPatPatho = new NSPatPathoInfo();
  pPatPatho->setLexique(sEtiquette);

  pPatPatho->setComplement   ("");
  pPatPatho->setUnit         ("");
  pPatPatho->setCertitude    ("");
  pPatPatho->setInteret      ("A");
  pPatPatho->setPluriel      ("");
  if ('V' == sEtiquette[0])
    pPatPatho->setVisible    ("1");

  pPatPatho->setTexteLibre("");
  pPatPatho->setArchetype("");
  pPatPatho->setColonne(colonne);

  int ligne;
	if (false == empty())
    ligne = (back())->getLigne() + decalageLigne;
  else
    ligne = ORIGINE_PATH_PATHO;
  pPatPatho->setLigne(ligne);

  push_back(pPatPatho);

} // try
catch (...)
{
    erreur("Exception NSPatPathoArray ajoutePatho 2", standardError);
}
}

//-------------------------------------------------------------------------
// A partir de la patpatho globale (this) extraire la patpatho de l'élément
// iterPere et la mettre dans pPatpathoPere
//-------------------------------------------------------------------------
void
NSPatPathoArray::ExtrairePatPatho(PatPathoConstIter iterPere, NSPatPathoArray* pPatpathoPere) const
{
  if ((NSPatPathoArray*) NULL == pPatpathoPere)
        return;

  if (false == pPatpathoPere->empty())
    pPatpathoPere->vider();

  if (empty() || (end() == iterPere) || ((PatPathoIter) NULL == iterPere))
        return;

    PatPathoConstIter itFils    = begin();
    PatPathoConstIter precedent = begin();

    itFils = iterPere;
    int ligneElement   = (*iterPere)->getLigne();
    int colonneElement = (*iterPere)->getColonne();
	if (end() != itFils)
        itFils++;
	else
        return;
    precedent = itFils;

try
{
    int Ligne = 0;
    bool continuer = true;
	while ((end() != itFils) && (continuer))
	{
    int ligneFils   = (*itFils)->getLigne();
    int colonneFils = (*itFils)->getColonne();

    if ((colonneFils > colonneElement) && (ligneFils > ligneElement))
    {
      NSPatPathoInfo* pPatPatho = new NSPatPathoInfo();
      *(pPatPatho->getDataTank()) = *((*itFils)->getDataTank());

      int colonne = colonneFils - (colonneElement + 1);
      pPatPatho->setColonne(colonne);
      if ((end() != precedent)  &&
                   ((*precedent)->getLigne() != (*itFils)->getLigne()))
        Ligne++;
      pPatPatho->setLigne(Ligne);

      pPatpathoPere->push_back(pPatPatho);

      precedent = itFils;
      if (end() != itFils)
        itFils++;
    }
    else
        continuer = false;
	}
} // try
catch (...)
{
    erreur("Exception NSPatPathoArray ExtrairePatPatho 1.", standardError);
}
}

//-------------------------------------------------------------------------
//A partir de la patpatho globale (this) extraire la patpatho de l'élément
//(ligneElement, colonneElement) et la mettre dans pPatpathoPere
//-------------------------------------------------------------------------
void
NSPatPathoArray::ExtrairePatPatho(int ligneElement, int colonneElement, NSPatPathoArray* pPatpathoPere) const
{
  if ((NSPatPathoArray*) NULL == pPatpathoPere)
    return;

  if (false == pPatpathoPere->empty())
    pPatpathoPere->vider();

  if (empty())
    return;

  PatPathoConstIter iterPere = ChercherItem(ligneElement, colonneElement);//chercher l'élément
   																		//de coordonnées (ligneElement, colonneElement)
  if (((PatPathoIter) NULL == iterPere) || (end() == iterPere))
    return;

    ExtrairePatPatho(iterPere, pPatpathoPere);
}

//-------------------------------------------------------------------------
// From (this) extract element sub-patpatho with element as root
//-------------------------------------------------------------------------
void
NSPatPathoArray::ExtrairePatPathoWithRoot(PatPathoConstIter iterPere, NSPatPathoArray* pSubPatpatho) const
{
  if ((NSPatPathoArray*) NULL == pSubPatpatho)
        return;

  if (false == pSubPatpatho->empty())
    pSubPatpatho->vider();

  if (empty() || (end() == iterPere) || ((PatPathoIter) NULL == iterPere))
        return;

  // Getting sub-patpatho without root
  //
  NSPatPathoArray tempPatpatho(*pSubPatpatho);
  ExtrairePatPatho(iterPere, &tempPatpatho);

  // Add iterPere as root
  //
  pSubPatpatho->ajoutePatho(iterPere, 0, 0);
  pSubPatpatho->InserePatPathoFille(pSubPatpatho->begin(), &tempPatpatho);
}

void
NSPatPathoArray::ExtrairePatPathoWithRoot(int ligneElement, int colonneElement, NSPatPathoArray* pPatpathoPere) const
{
  if ((NSPatPathoArray*) NULL == pPatpathoPere)
    return;

  if (false == pPatpathoPere->empty())
    pPatpathoPere->vider();

  if (empty())
    return;

  // Get element at ligneElement, colonneElement
  //
  PatPathoConstIter iterPere = ChercherItem(ligneElement, colonneElement); 
  if (((PatPathoConstIter) NULL == iterPere) || (end() == iterPere))
    return;

    ExtrairePatPathoWithRoot(iterPere, pPatpathoPere);
}

//-------------------------------------------------------------------------
// Create a NSPatPathoArray that contains all brother nodes of same kind
// than iterRef
//-------------------------------------------------------------------------
void
NSPatPathoArray::ExtrairePatPathoFreres(PatPathoIter iterRef, NSPatPathoArray* pPatpathoDest) const
{
  if ((empty()) || ((NSPatPathoArray*) NULL == pPatpathoDest))
    return;
  if (((PatPathoIter) NULL == iterRef) || (end() == iterRef))
    return;

try
{
  int colonneElement = (*iterRef)->getColonne();

    PatPathoConstIter itFils = begin();
  itFils = iterRef;

  int Ligne = 0;
  bool bBonneBranche = true;
  while (end() != itFils)
  {
    int colonneFils = (*itFils)->getColonne();
    //
    // Trop loin
    //
    if (colonneFils < colonneElement)
      break;
    //
    // Même niveau que iterRef, on vérifie que c'est un noeud de même type
    //
    if (colonneFils == colonneElement)
    {
      if (((*itFils)->getLexique()    == (*iterRef)->getLexique())    &&
          ((*itFils)->getComplement() == (*iterRef)->getComplement()) &&
          ((*itFils)->getCertitude()  == (*iterRef)->getCertitude())  &&
          ((*itFils)->getPluriel()    == (*iterRef)->getPluriel()))
        bBonneBranche = true;
      else
        bBonneBranche = false;
    }
    //
    // Ajout du noeud
    //
    if (bBonneBranche)
    {
      string sLexique = (*itFils)->getLexique();

      int colonne = colonneFils - colonneElement;

      NSPatPathoInfo* pPatPatho = new NSPatPathoInfo();
      *(pPatPatho->getDataTank()) = *((*itFils)->getDataTank());

      pPatPatho->setColonne(colonne);
      pPatPatho->setLigne(Ligne);
      Ligne++;

      pPatpathoDest->push_back(pPatPatho);
    }
    itFils++;
  }
} // try
catch (...)
{
    erreur("Exception NSPatPathoArray ExtrairePatPathoFreres.", standardError);
}
}

//-------------------------------------------------------------------------
// A partir de la patpatho globale (this) extraire le vecteur des freres
// de même colonne à partir de la colonne de référence (iterRef)
//-------------------------------------------------------------------------
void
NSPatPathoArray::ExtraireVecteurPatPathoFreres(PatPathoIter iterRef, NSVectPatPathoArray* pVect) const
{
	if ((empty()) || (!pVect))
        return;
	if (iterRef == end())
        return;

    pVect->vider();

try
{
    NSPatPathoArray PatPathoFrere(_pSuper);
    int colonneElement = (*iterRef)->getColonne();

    PatPathoIter itFils = iterRef;

    int Ligne;
    bool bDebut = true;

	while (itFils != end())
	{
        int colonneFils = (*itFils)->getColonne();
		//
		// Trop loin
		//
		if (colonneFils < colonneElement)
            break;
		//
		// Même niveau que iterRef, on vérifie que c'est un noeud de même type
		//
		if (colonneFils == colonneElement)
		{
			if (false == bDebut)
                pVect->push_back(new NSPatPathoArray(PatPathoFrere));

            PatPathoFrere.vider();
            Ligne  = 0;
            bDebut = false;
		}
		//
		// Ajout du noeud
		//
		if (false == bDebut)
		{
            string sLexique = (*itFils)->getLexique();

            int colonne = colonneFils - colonneElement;

            NSPatPathoInfo* pPatPatho = new NSPatPathoInfo();
            *(pPatPatho->getDataTank()) = *((*itFils)->getDataTank());

            pPatPatho->setColonne(colonne);
            pPatPatho->setLigne(Ligne);
            Ligne++;

            PatPathoFrere.push_back(pPatPatho);
		}

        itFils++;
	}

	// on push_back le dernier frere
	if (false == bDebut)
        pVect->push_back(new NSPatPathoArray(PatPathoFrere));

} // try
catch (...)
{
    erreur("Exception NSPatPathoArray ExtraireVecteurPatPathoFreres.", standardError);
}
}

//-------------------------------------------------------------------------
// Insérer dans la patpatho globale (this) la patpatho source juste
// avant l' élément iterAvant
//
// ATTENTION : la colonne des éléments ajoutés est calculée en ajoutant
//             decalageColonne à la COLONNE QU'IL A DANS pPatpathoSource
//             et non pas à la colonne de iterAvant
//
// WARNING : the added element's column is set by adding decalageColonne to
//           the COLUMN HE HAD IN pPatpathoSource (and not to iterAvant's col)
//
//-------------------------------------------------------------------------
void
NSPatPathoArray::InserePatPatho(PatPathoIter iterAvant,
      										 NSPatPathoArray* pPatpathoSource,
                                             int decalageColonne,
                                             bool bWithTreeID)
{
try
{
  if (((NSPatPathoArray*) NULL == pPatpathoSource) || (pPatpathoSource->empty()))
        return;

	// On ne pourra plus tester de façon valide si iterAvant == end() dès
	// qu'on aura inséré un nouveau noeud
	//
  NSPatPathoInfo* pPathAvant = (NSPatPathoInfo*) 0;
    bool bInsertAtEnd;
  if ((end() == iterAvant) || ((PatPathoIter) NULL == iterAvant))
        bInsertAtEnd = true;
	else
        bInsertAtEnd = false;

  // http://www.sgi.com/tech/stl/Vector.html
  // A vector's iterators are invalidated when its memory is reallocated.
  // Additionally, inserting or deleting an element in the middle of a vector
  // invalidates all iterators that point to elements following the insertion
  // or deletion point.
  // It follows that you can prevent a vector's iterators from being
  // invalidated if you use reserve() to preallocate as much memory as the
  // vector will ever use, and if all insertions and deletions are at the
  // vector's end.
  //
	if (false == bInsertAtEnd)
	{
    pPathAvant = *iterAvant;

    size_type iSourceSize  = pPatpathoSource->size();
    size_type iCurrentSize = size();
    size_type iCurrentCapa = capacity();
    if (iCurrentCapa < iSourceSize + iCurrentSize)
    {
        reserve(iSourceSize + iCurrentSize);

      iterAvant = begin();
      for (; (end() != iterAvant) && (*iterAvant != pPathAvant); iterAvant++);
      if (iterAvant == end())
      {
        erreur("Erreur lors de l'insertion d'un arbre : le noeud d'insertion n'est plus présent.", standardError);
        return;
      }
    }
  }

  // Attention : on commence par décaler les lignes d'iterAvant et des
  // éléments suivant de la "taille" de patpathoSource, comme la ligne
  // d'iterAvant reste la référence des éléments insérés, celà oblige ensuite
  // à insérer avec un décalage de ligne - pPatpathoSource->Taille())
  //
  // Ce qu'Hafedh n'avait pas prévu, c'est le cas iterAvant == end()
  // dans ce cas, ce décalage négatif n'a pas lieu d'être, il faut le
  // remplacer par un décalage positif (ou nul) par rapport au dernier
  // élément de l'Array
  // L'autre problème, c'est qu'il n'existe, dans ce même cas, aucun moyen de
  // savoir à quelle colonne insérer patpathoSource, on insère donc en
  // colonne 0 + decalageColonne
	//
	if (false == empty())
	{
    PatPathoIter jter = begin();
    jter = iterAvant;
    for (; end() != jter; jter++)
        (*jter)->setLigne((*jter)->getLigne() + pPatpathoSource->Taille());
	}

    PatPathoIter j = pPatpathoSource->begin();
    int ligne = 0;
    int decalFin = 0;
    PatPathoIter precedent = pPatpathoSource->begin(); // utilisé pour ne pas
                                                       // incrémenter ligne
                                                       // si on est sur des
                                                       // étiquettes composées
	while (pPatpathoSource->end() != j)
	{
    Message Msg;

    if (bWithTreeID)
        Msg.SetTreeID((*j)->getDoc());
    Msg.SetNoeud((*j)->getNodeID());
    Msg.SetUnit((*j)->getUnit());
    Msg.SetComplement((*j)->getComplement());
    Msg.SetCertitude((*j)->getCertitude());
    Msg.SetInteret((*j)->getInteret());
    Msg.SetPluriel((*j)->getPluriel());
    Msg.SetVisible((*j)->getVisible());
    Msg.SetType((*j)->getType());
    Msg.SetTexteLibre((*j)->getTexteLibre());
    Msg.SetArchetype((*j)->getArchetype());
    Msg.SetNodeRight((*j)->getNodeRight());
    Msg.SetTemporaryLinks((*j)->getTemporaryLinks());

    if ((*precedent)->getLigne() != (*j)->getLigne())
    {
        ligne++;
      decalFin = 1;
    }
    else
    {
    	if (pPatpathoSource->begin() == j)
        decalFin = 1;
      else
        decalFin = 0;
    }

    if (false == bInsertAtEnd)
    	ajoutePatho(iterAvant, (*j)->getLexique(), &Msg,
                        (*j)->getColonne() + decalageColonne,
                        ligne - pPatpathoSource->Taille());
    else
    	ajoutePatho(end(), (*j)->getLexique(), &Msg,
                        (*j)->getColonne() + decalageColonne, decalFin);
    precedent = j;
    j++;
    if (false == bInsertAtEnd)
    {
    	// Pour garantir qu'on est bien toujours sur le même et que
      // l'insertion n'a pas décalé iterAvant
      if (*iterAvant != pPathAvant)
      {
        iterAvant = begin();
        for (; (end() != iterAvant) && (*iterAvant != pPathAvant); iterAvant++);
        if (end() == iterAvant)
        {
            erreur("Erreur lors de l'insertion d'un arbre : le noeud d'insertion n'est plus présent.", standardError);
          break;
        }
      }
    }
  }
} // try
catch (...)
{
    erreur("Exception NSPatPathoArray InserePathPatho.", standardError);
}
}

//-------------------------------------------------------------------------
// Insérer dans la patpatho globale (this) la patpatho source en tant que
// fille de l'élément iterPere
//-------------------------------------------------------------------------
void
NSPatPathoArray::InserePatPathoFille(PatPathoIter iterPere,
      										 NSPatPathoArray* pPatpathoSource)
{
	if ((NULL == pPatpathoSource) || (pPatpathoSource->empty()))
        return;
  if (((PatPathoIter) NULL == iterPere) || (end() == iterPere))
        return;

    int iColPere = (*iterPere)->getColonne();

	//
	// On se positionne sur l'élément qui suit le dernier fils
	//
    PatPathoIter iterFrere = iterPere;
    iterFrere++;
  while ((end() != iterFrere) && ((*iterFrere)->getColonne() > iColPere))
        iterFrere++;

	// iDeltaCol sera ajouté à la colonne qu'à l'élément dans pPatpathoSource
    // pour fournir sa colonne définitive; puisqu'on veut que les éléments
	// soit fils de iterPere, il faut "normaliser" les choses
	//
	// iDeltaCol will be added to the column of the node inside pPatpathoSource
    // in order to get the node column; since we want the nodes to be sons
	// of iterPere, we have to "normalise"
	//
    int iDeltaCol = iColPere + 1 - ((*(pPatpathoSource->begin()))->getColonne());

    InserePatPatho(iterFrere, pPatpathoSource, iDeltaCol);
}

// Get the date of the document... if the tree is a document tree
//
string
NSPatPathoArray::GetDocDate()
{
  if (empty())
    return string("");

  // First, check if first element is a KCHIR
  //
  PatPathoIter iterPpt = begin();
  iterPpt++;
  if (end() == iterPpt)
    return string("");

  string sDocDate = string("");

  if ((*iterPpt)->getLexiqueSens(_pSuper) == "KCHIR")
    CheminDansPatpatho((NSSmallBrother *) 0, "KCHIR", &sDocDate);

  if (string("") == sDocDate)
    CheminDansPatpatho((NSSmallBrother *) 0, "LADMI/KCHIR", &sDocDate);

  return sDocDate;
}

//--------------------------------------------------------------------
//comparaison de 2 patpathos : this et pPatho
// Teste l'égalité du point de vue des données (ne tient pas compte du numéro de noeud)
//--------------------------------------------------------------------
bool
NSPatPathoArray::estEgal(NSPatPathoArray* pPatho)
{
	if (NULL == pPatho)
        return false;

	if (empty())
	{
		if (pPatho->empty())
            return true;
		else
            return false;
	}

    PatPathoIter i = begin();
    PatPathoIter j = pPatho->begin();

	while ((end() != i) && (pPatho->end() != j))
	{
		if (false == (*i)->estEgal(*j))
          return false;

    i++;
    j++;
	}

  if ((end() != i) || (pPatho->end() != j))
    return false;

    return true;
}

//--------------------------------------------------------------------
// Comparaison de 2 patpathos : this et pPatho
// Teste l'égalité du point de vue des données (ne tient compte
// ni du numéro de noeud, ni de la visibilité)
//--------------------------------------------------------------------
bool
NSPatPathoArray::hasSameContent(NSPatPathoArray* pPatho)
{
	if (NULL == pPatho)
        return false;

	if (empty())
	{
		if (pPatho->empty())
            return true;
		else
            return false;
	}

    PatPathoIter i = begin();
    PatPathoIter j = pPatho->begin();

	while ((end() != i) && (pPatho->end() != j))
	{
		if (false == (*i)->sameContent(*j))
          return false;

    i++;
    j++;
	}

  if ((end() != i) || (pPatho->end() != j))
    return false;

    return true;
}

//----------------------------------------------------------------------
// savoir si dans pPatpath les éléments de sChaine sont en relation de
// 							descendance entre eux.
//
// 				exemple sChaine : "A/B/C/D/E/F/G/H"
//si on trouve l'enchaînement suivant A/B/C/D alors retourner l'itérateur
//											sur D
//							pPostionItem est la position de D (4)
//----------------------------------------------------------------------
PatPathoIter
NSPatPathoArray::ChaineDansPatpatho(string sChaine, string* pLocalisation, string separateur, PatPathoIter* pIterFrom)
{
try
{
  if ((empty()) || (NULL == pLocalisation))
    return (PatPathoIter) NULL;

  // Qualifiers are '/' for 'at tree root' and '.' for 'direct son of previous element'
  //
    ClasseStringVector Vect;
    DecomposeChaine(&sChaine, &Vect, separateur, string("/."));

  if (Vect.empty())
    return (PatPathoIter) NULL;

    iterClassString iterpVect     = Vect.begin();
    iterClassString iterpVectTemp = Vect.begin();
  *pLocalisation = string("");
  PatPathoIter  iterRetour  = begin();
  PatPathoIter  iterPatPath = begin();

    int colonneEncours;
    int colonne;
    string sItem;

    bool bCanReturn = true;
  if (pIterFrom && (iterPatPath != *pIterFrom))
        bCanReturn = false;

	//
  // pour chaque élément de pVect, scanner toute la patpatho, et mettre à jour les champs de
  // l'élément en question :
  //									colonne = sa colonne;
  //   								coche   = élément détecté ou non;
  //   								sItem   = son lablel;
  //			(selon sa position par rapport à l élément de la patpatho)
  //

  for (iterPatPath = begin(); end() != iterPatPath; iterPatPath++)
  {
    colonneEncours = (*iterPatPath)->getColonne();
    for (iterpVect = Vect.begin(); iterpVect != Vect.end(); iterpVect++)
    {
        colonne = (*iterpVect)->getColumn();
      // Décocher les éléments dont la colonne est >= colonneEncours
      if (colonne >= colonneEncours)
        (*iterpVect)->setChecked(false);
    }

    // Trouver le premier élément de pVect non coché
    //
    for (iterpVectTemp = Vect.begin(); (Vect.end() != iterpVectTemp) &&
                    ((*iterpVectTemp)->getChecked()); iterpVectTemp++);
    if (Vect.end() != iterpVectTemp)
    {
        sItem = (*iterpVectTemp)->getItem();

      string sCodeSens = (*iterPatPath)->getLexique();
      NSDico::donneCodeSens(sCodeSens);

      string ssItemCodeSens = sItem;
      NSDico::donneCodeSens(ssItemCodeSens);

      // Node is ok...
      //
      if (sCodeSens == ssItemCodeSens)
      {
        bool bIsOk = true;

        // Is it ok for qualifiers
        //
        if ((NPOS != (*iterpVectTemp)->getQualifier().find("/")) && (0 != colonneEncours))
          bIsOk = false;
        if (NPOS != (*iterpVectTemp)->getQualifier().find("."))
        {
          if (Vect.begin() != iterpVectTemp)
          {
            iterClassString iterFather = iterpVectTemp - 1;
            if ((*iterFather)->getColumn() != colonneEncours - 1)
              bIsOk = false;
          }
        }

        if (bIsOk)
        {
      	  if (*pLocalisation == "")
              *pLocalisation += (*iterpVectTemp)->getItem();
          else
              *pLocalisation += separateur + (*iterpVectTemp)->getItem();
          (*iterpVectTemp)->setColumn((*iterPatPath)->getColonne());
          (*iterpVectTemp)->setChecked(true);
          iterRetour = iterPatPath;
          // si c'est le dernier on sort
          if (bCanReturn && ((*iterpVectTemp) == Vect.back()))
              return iterRetour;
        }
      }
    }
    if (pIterFrom && (*pIterFrom == iterPatPath))
        bCanReturn = true;
  }

  if ((string("") != *pLocalisation) && (NULL == pIterFrom))
    return iterRetour;
	else
        return end();

} // try
catch (...)
{
    erreur("Exception NSPatPathoArray ChaineDansPatpatho.", standardError);
  return end();
}
}

/**
* Ajoute un chemin à une patPatho
* Insert a path inside a patPatho
*/
void
NSPatPathoArray::AjouterChemin(string sChaine, string sValeur, bool bEcraser, string separateur)
{
try
{
	if (string("") == sChaine)
        return;

    int Colonne = 0;
    int ColonnePere;

  PatPathoIter suivant = (PatPathoIter) NULL;

    // bool bSuivant = false;
    // bool bChaineDansPatpatho = false;

	string  sChaineInsertion; //chaîne à insérer

	if (string("") != sValeur)
        sChaineInsertion = sChaine + separateur + sValeur;
	else
		sChaineInsertion = sChaine;

	if (false == empty())
	{
    suivant = end();
		//
		// Chercher l'itérateur dans pPatPath correspondant au dernier item
		// de la plus grande sous chaîne de sChaine.
		//
        string sousChaine;  // plus grande sous chaine de sChaine représentant
    										 // un chemin dans pPatPath
        PatPathoIter iter = ChaineDansPatpatho(sChaine, &sousChaine, separateur);

		//
		//exemple : sChaine = "AVGCA1/QCINE1/QCOVG1"
		//          sValeur = "1DIMI1"
    //
		//          iter -> QCINE1
    //
		//          sousChaine       = "AVGCA1/QCINE1"
		//          sChaineInsertion = "QCOVG1/1DIMI1"
    //
		if (sousChaine == sChaine)
            sChaineInsertion = sValeur;
		else
		{
            bEcraser = false;
			if (sousChaine != "")
			{
                int i = strlen(sousChaine.c_str());
                sChaineInsertion = string(sChaine, i + 1, strlen(sChaine.c_str()) - (i + 1));
        if (string("") != sValeur)
          sChaineInsertion += separateur + sValeur;
			}
		}
    if (((PatPathoIter) NULL != iter) && (end() != iter))
		{
            ColonnePere = (*iter)->getColonne();
            Colonne     = ColonnePere + 1;
            // bChaineDansPatpatho = true;
			if (bEcraser)//écraser ce qui vient après iter
			{
                PatPathoIter suppr = begin();
                suppr = iter;
				if (end() != suppr)
                    suppr++;
				if (end() != suppr)
				{
					if ((*suppr)->getColonne() == (*iter)->getColonne() + 1)
          {
            if (sChaineInsertion == sValeur)
            {
              // before removing element, check if it is not already the term
              // to be inserted
              //
              PatPathoIter itFirstSon =	ChercherPremierFils(suppr);
              if (((PatPathoIter) NULL == itFirstSon) || (end() == itFirstSon))
              {
                Message insertMsg;
                insertMsg.InitFromEtiquette(sValeur);
                NSPatPathoInfo insertPptInfo;
                insertPptInfo.initFromMessage(&insertMsg);
                if (insertPptInfo.sameContent(*suppr))
                  return;
              }
            }

            SupprimerItem(suppr);
          }
				}
                suivant = begin();
                suivant = iter;
				if (suivant != end())
                    suivant++;
			}
			else
                suivant = ChercherItem(iter); //le premier frère de iter
		}

		// if ((suivant != end()) && (suivant != NULL))
        // 	bSuivant = true;
	}

    ClasseStringVector Vect;
    DecomposeChaine(&sChaineInsertion, &Vect, separateur);

	if (Vect.empty())
    return;

  NSPatPathoArray pptNewBlock(_pSuper);
  int iNewBlockCol = 0;

  iterClassString iterpVect = Vect.begin();
  while (Vect.end() != iterpVect)
  {
    pptNewBlock.ajoutePathoBlock((*iterpVect)->getItem(), iNewBlockCol++);
    iterpVect++;
  }

  InserePatPatho(suivant, &pptNewBlock, Colonne);

/*
  iterString iterpVect = Vect.begin();
  while (Vect.end() != iterpVect)
  {
			//traiter le cas des étiquettes multiples
            ClasseStringVector* pVectEtiqMulti = new ClasseStringVector;
            DecomposeChaine(&(*iterpVect)->sItem, pVectEtiqMulti, "/");
            iterString  iterpVectMulti  = pVectEtiqMulti->begin();
            int TailleEtiquette = pVectEtiqMulti->size();
			if (bChaineDansPatpatho)
			{
				if (bSuivant)
				{
                    PatPathoIter jter = begin();
					jter = suivant;
                    for (; jter != end(); jter++)
                            (*jter)->setLigne((*jter)->getLigne() + TailleEtiquette);
					//insérer les éléments d'une étiquette multiple dans le même noeud
					if (iterpVectMulti != pVectEtiqMulti->end())
					{
                        string sEtiquette   = (*iterpVectMulti)->sItem;
                        string sCertitude   = "";
                        string sPluriel     = "";
                        string sComplement  = "";
                        string sUnit        = "";

                        iterpVectMulti++;

						if ((sEtiquette[0] == '2') && (iterpVectMulti != pVectEtiqMulti->end()))
						{
                            sUnit       = sEtiquette;
                            sEtiquette  = (*iterpVectMulti)->sItem;
                            iterpVectMulti++;
						}

						if (iterpVectMulti != pVectEtiqMulti->end())
						{
                            bool bTourner = true;

							while (bTourner)
							{
                                string sSuite = (*iterpVectMulti)->sItem;
								if (sSuite != "")
								{
									if      (string(sSuite, 0, 1) == string("$"))
                                        sComplement = string(sSuite, 1, strlen(sSuite.c_str())-1);
									else if (string(sSuite, 0, 3) == string("WCE"))
                                        sCertitude = sSuite;
									else if (string(sSuite, 0, 3) == string("WPL"))
										sPluriel = sSuite;
									else
                                        bTourner = false;
								}
								else
                                    bTourner = false;

								if (bTourner)
								{
                                    iterpVectMulti++;
									if (iterpVectMulti == pVectEtiqMulti->end())
                                        bTourner = false;
								}
							}
						}

                        Message CodeMsg;
                        CodeMsg.SetLexique(sEtiquette);
                        CodeMsg.SetPluriel(sPluriel);
                        CodeMsg.SetCertitude(sCertitude);
                        CodeMsg.SetComplement(sComplement);
                        CodeMsg.SetUnit(sUnit);

                        ajoutePatho(suivant, sEtiquette, &CodeMsg, Colonne , -1);

            Colonne++;

						if (suivant != end())
                            suivant++;
						if (suivant != end())
                            bSuivant = true;
						else
                            bSuivant = false;
					}
				}
				else
				{
                    int ligne = 1;
          //insérer les éléments d'une étiquette multiple dans le même noeud
          if (iterpVectMulti  != pVectEtiqMulti->end())
          {
            string sEtiquette   = (*iterpVectMulti)->sItem;
            string sCertitude   = "";
            string sPluriel     = "";
            string sComplement  = "";
            string sUnit        = "";

            iterpVectMulti++;

            if ((sEtiquette[0] == '2') && (iterpVectMulti != pVectEtiqMulti->end()))
            {
              sUnit      = sEtiquette;
              sEtiquette = (*iterpVectMulti)->sItem;
              iterpVectMulti++;
            }

            if (iterpVectMulti != pVectEtiqMulti->end())
            {
              bool bTourner = true;

              while (bTourner)
              {
                string sSuite = (*iterpVectMulti)->sItem;
                if (sSuite != "")
                {
                  if      (string(sSuite, 0, 1) == string("$"))
                    sComplement = string(sSuite, 1, strlen(sSuite.c_str())-1);
                  else if (string(sSuite, 0, 3) == string("WCE"))
                    sCertitude = sSuite;
                  else if (string(sSuite, 0, 3) == string("WPL"))
                    sPluriel = sSuite;
                  else
                    bTourner = false;
                }
                else
                  bTourner = false;

                if (bTourner)
                {
                  iterpVectMulti++;
                  if (iterpVectMulti == pVectEtiqMulti->end())
                    bTourner = false;
                }
              }
            }

            Message CodeMsg;
            CodeMsg.SetLexique(sEtiquette);
            CodeMsg.SetPluriel(sPluriel);
            CodeMsg.SetCertitude(sCertitude);
            CodeMsg.SetComplement(sComplement);
            CodeMsg.SetUnit(sUnit);

            ajoutePatho(sEtiquette, &CodeMsg, Colonne, ligne);

            ligne = 0;
            Colonne++;
          }
        }
      }
      else
      {
        int ligne = 1;
        // Insérer les éléments d'une étiquette multiple dans le même noeud
        if (iterpVectMulti != pVectEtiqMulti->end())
        {
          string sEtiquette  = (*iterpVectMulti)->sItem;
          string sCertitude  = "";
          string sPluriel    = "";
          string sComplement = "";
          string sUnit       = "";

          iterpVectMulti++;

          if ((sEtiquette[0] == '2') && (iterpVectMulti != pVectEtiqMulti->end()))
          {
            sUnit      = sEtiquette;
            sEtiquette = (*iterpVectMulti)->sItem;
            iterpVectMulti++;
          }

          if (iterpVectMulti != pVectEtiqMulti->end())
          {
            bool bTourner = true;

            while (bTourner)
            {
              string sSuite = (*iterpVectMulti)->sItem;
              if (sSuite != "")
              {
                if      (string(sSuite, 0, 1) == string("$"))
                  sComplement = string(sSuite, 1, strlen(sSuite.c_str())-1);
                else if (string(sSuite, 0, 3) == string("WCE"))
                  sCertitude = sSuite;
                else if (string(sSuite, 0, 3) == string("WPL"))
                  sPluriel = sSuite;
                else
                  bTourner = false;
              }
              else
                bTourner = false;

              if (bTourner)
              {
                iterpVectMulti++;
                if (iterpVectMulti == pVectEtiqMulti->end())
                  bTourner = false;
              }
            }
          }

          Message CodeMsg;
          CodeMsg.SetLexique(sEtiquette);
          CodeMsg.SetPluriel(sPluriel);
          CodeMsg.SetCertitude(sCertitude);
          CodeMsg.SetComplement(sComplement);
          CodeMsg.SetUnit(sUnit);

          ajoutePatho(sEtiquette, &CodeMsg, Colonne, ligne);

          ligne = 0;
          Colonne++;
        }
      }
      iterpVect++;
      delete pVectEtiqMulti;
    }
*/
} // try
catch (...)
{
    erreur("Exception NSPatPathoArray AjouterChemin 1.", standardError);
}
}

//-------------------------------------------------------------------
// ajoute un chemin à une patpatho
//-------------------------------------------------------------------
void
NSPatPathoArray::AjouterChemin(string sChaine, Message* pMessage, bool bEcraser, string separateur)
{
try
{
	if (NULL == pMessage)
        return;

    int Colonne = 0;
    int ColonnePere;
    PatPathoIter suivant = end();
    bool sSuivant = false;

    string sValeur = pMessage->GetLexique();

	//
	//chercher l'itérateur dans pPatPath correspondant au dernier item
	//de la plus grande sous chaîne de sChaine.
	//
	//
    string sousChaine; //plus grande sous chaine de sChaine représntant un chemin
   						        //dans pPatPath
    PatPathoIter iter = ChaineDansPatpatho(sChaine, &sousChaine, separateur);
    bool bChaineDansPatpatho = false;
    string sChaineInsertion = ""; //chaîne à insérer

	//
	//exemple : sChaine = "AVGCA1|QCINE1|QCOVG1"
	//				sValeur = "1DIMI1"
	//          iter -> QCINE1
	//          sousChaine = "AVGCA1|QCINE1"
	//				sChaineInsertion = "QCOVG1|1DIMI1"

	if (sousChaine == sChaine)
        sChaineInsertion = "";
	else
	{
        bEcraser = false;
		if (sousChaine != "")
		{
        int i = strlen(sousChaine.c_str());
      sChaineInsertion = string(sChaine, i + 1, strlen(sChaine.c_str()) - (i + 1));
  	}
    else
        sChaineInsertion = sChaine;
	}

	// La chaîne mère existe déjà dans la patpatho
	//
  if ((end() != iter) && ((PatPathoIter) NULL != iter))
	{
    ColonnePere = (*iter)->getColonne();
    Colonne = ColonnePere + 1;
    bChaineDansPatpatho = true;
    if (bEcraser) //écraser ce qui vient après iter
    {
        PatPathoIter suppr = begin();
      suppr = iter;
      if (suppr != end())
        suppr++;
      if (suppr != end())
      {
      	if ((*suppr)->getColonne() > ((*iter)->getColonne()))
            SupprimerItem(suppr);
      }
      suivant = begin();
      suivant = iter;
      if (suivant != end())
        suivant++;
    }
    else
        suivant = ChercherItem(iter); //le premier frère de iter
	}

  if ((end() != suivant) && ((PatPathoIter) NULL != suivant))
        sSuivant = true;

	// Décomposition de la chaine mère en fils
	//
	if (sChaineInsertion != "")
	{
    ClasseStringVector Vect;
    DecomposeChaine(&sChaineInsertion, &Vect, separateur);
    iterClassString iterpVect = Vect.begin();

    // Ajout de ces fils à la patpatho
    //
		while (Vect.end() != iterpVect)
		{
    	//traiter le cas des étiquettes multiples
      ClasseStringVector VectEtiqMulti;
      string sMultiLabel = (*iterpVect)->getItem();
      DecomposeChaine(&sMultiLabel, &VectEtiqMulti, "/");
      iterClassString iterpVectMulti = VectEtiqMulti.begin();
      int TailleEtiquette = VectEtiqMulti.size();

      // La chaîne mère existait déjà dans la patpatho
      //
      if (bChaineDansPatpatho)
      {
      	// Si on insère dans une patpatho
        //
        if (sSuivant)
        {
            PatPathoIter jter = begin();
          jter = suivant;
          for (; jter != end(); jter++)
            (*jter)->setLigne((*jter)->getLigne() + TailleEtiquette);

          //insérer les éléments d'une étiquette multiple sur la même ligne
          for (; iterpVectMulti != VectEtiqMulti.end(); iterpVectMulti++)
          {
            ajoutePatho(suivant, (*iterpVectMulti)->getItem(), Colonne, -1);
            Colonne++;
            if (suivant != end())
            	suivant++;
            if (suivant != end())
                sSuivant = true;
            else
                sSuivant = false;
          }
        }
        //
        // Si on ajoute à la fin
        //
        else
        {
          //insérer les éléments d'une étiquette multiple sur la même ligne
          for (; iterpVectMulti != VectEtiqMulti.end(); iterpVectMulti++)
          {
            ajoutePatho((*iterpVectMulti)->getItem(), Colonne);
            Colonne++;
          }
        }
      }
      //
      // Sinon on ajoute à la fin
      //
      else
      {
        //insérer les éléments d'une étiquette multiple sur la même ligne
        for (; iterpVectMulti != VectEtiqMulti.end(); iterpVectMulti++)
        {
            ajoutePatho((*iterpVectMulti)->getItem(), Colonne);
          Colonne++;
        }
      }
      iterpVect++;
    }
	}
	//
  // Ajout de notre message
  //
  // La chaîne mère existait déjà dans la patpatho
  //
  if (bChaineDansPatpatho)
	{
  	// Si on insère dans une patpatho
    //
    if (sSuivant)
    {
        PatPathoIter jter = begin();
      jter = suivant;
      for (; jter != end(); jter++)
        (*jter)->setLigne((*jter)->getLigne() + 1);

      ajoutePatho(suivant, sValeur, pMessage, Colonne , -1);
    }
    //
    // Si on ajoute à la fin
    //
    else
      ajoutePatho(sValeur, pMessage, Colonne);
  }
  //
  // Sinon on ajoute à la fin
  //
  else
  {
    ajoutePatho(sValeur, pMessage, Colonne);
    Colonne++;
	}
} // try
catch (...)
{
    erreur("Exception NSPatPathoArray AjouterChemin 2.", standardError);
}
}

//-------------------------------------------------------------------------
// pPatPatho est une sous patpatho de this formée par des pathos dont l'intêret
// est >= sImportance. Exemple si sImportance coorrespond à 3+ ,
// mettre dans pPatPatho tous les éléments ayant pour intêret "D" et ""
//-------------------------------------------------------------------------
void
NSPatPathoArray::donnePatpatho(NSPatPathoArray* pPatNew, string sImportance)
{
try
{
  if ((empty()) || (NULL == pPatNew))
    return;
	//
	// Prendre l'intêret >= sImportance
	// trouver un élément dont l'intêret est sImportance
	//

  //recopier this sans son premier élément dans pPatPathoArray
  PatPathoIter iter = begin();
  NSPatPathoArray PatPathoArray(_pSuper);
  ExtrairePatPatho(iter, &PatPathoArray);

  PatPathoIter element = PatPathoArray.begin(); //patho à insérer
  int NewligneEnCours = 0;

	while (PatPathoArray.end() != element)
  {
    int ligne = (*element)->getLigne();
    //
    // Si le noeud ou un de ses fils a au moins l'importance voulue
    //
    if (PatPathoArray.InteretFils(element, sImportance))
    {
      pPatNew->ajoutePatho(element, NewligneEnCours, (*element)->getColonne());
      element++;
      //
      // On ajoute tous les éléments de la même ligne
      //
      while ((PatPathoArray.end() != element) && ((*element)->getLigne() == ligne))
      {
        pPatNew->ajoutePatho(element, NewligneEnCours, (*element)->getColonne());
        element++;
      }
      NewligneEnCours++;
    }
    else
      element++;
  }
} // try
catch (...)
{
    erreur("Exception NSPatPathoArray donnePatpatho.", standardError);
}
}

//--------------------------------------------------------------------
// Retourner true si l'intêret de iterPere ou l'un de ses descendants
// est >= sImportance
//
// ATTENTION : si sImportance est vide (""), on retourne toujours false
//
//--------------------------------------------------------------------
bool
NSPatPathoArray::InteretFils(PatPathoIter iterPere, string sImportance)
{
  if ((empty()) || ((PatPathoIter) NULL == iterPere) || (end() == iterPere))
    return false;

  if (string("") == sImportance)
    return false;

  string sInteretPere = (*iterPere)->getInteret();
  if (sInteretPere >= sImportance)
    return true;

    PatPathoIter iter = begin();
  int ligne   = (*iterPere)->getLigne();
  int colonne = (*iterPere)->getColonne();
  iter = iterPere;

  if (end() != iter)
    iter++;

  bool continuer = true;
  while ((end() != iter) && continuer)
  {
    string sInteret = (*iter)->getInteret();
    int ligneFils   = (*iter)->getLigne();
    int colonneFils = (*iter)->getColonne();
    if ((ligneFils >= ligne) && (colonneFils > colonne))
    {
      if (sInteret >= sImportance)
        return true;
      iter++;
    }
    else
      continuer = false; //on n'est plus sur les fils
  }
  return false;
}

//-------------------------------------------------------------------
// pPatNew est une sous patpatho de this
//-------------------------------------------------------------------
void
NSPatPathoArray::SousPatpatho(NSPatPathoArray* pPatNew, string sChaine,
                              string sValeur, string separateur)
{
try
{
	if ((NULL == pPatNew) || (pPatNew->empty()))
        return;

  int Colonne = 0;
  int ColonnePere;
  string sEtiquette;
  PatPathoIter suivant = pPatNew->end();
  bool sSuivant = false;
  //
  // Chercher l'itérateur dans pPatPath correspondant au dernier item
  // de la plus grande sous chaîne de sChaine.
  //
  string sousChaine;  // Plus grande sous chaine de sChaine représentant
                       // un chemin dans pPatPath
  PatPathoIter iter = pPatNew->ChaineDansPatpatho(sChaine, &sousChaine, separateur);
  bool bChaineDansPatpatho = false;
  string sChaineInsertion = ""; //chaîne à insérer
  //
  //exemple : sChaine = "AVGCA1/QCINE1/QCOVG1"
  //				sValeur = "1DIMI1"
  //          iter -> QCINE1
  //          sousChaine = "AVGCA1/QCINE1"
  //				sChaineInsertion = "QCOVG1/1DIMI1"
  if (sousChaine == sChaine)
    sChaineInsertion = sValeur;
  else
  {
    if (sousChaine != "")
    {
      int i = strlen(sousChaine.c_str());
      sChaineInsertion = string(sChaine, i + 1, strlen(sChaine.c_str()) - (i + 1))
      							+ separateur + sValeur;
    }
    else
    {
      if (sValeur != "")
        sChaineInsertion = sChaine + separateur + sValeur;
      else
        sChaineInsertion = sChaine;
    }
  }
  if ((pPatNew->end() != iter) && ((PatPathoIter) NULL != iter))
  {
    ColonnePere = (*iter)->getColonne();
    // LignePere   = (*iter)->getLigne();
    sEtiquette  = (*iter)->getLexique();
    Colonne = ColonnePere + 1;
    bChaineDansPatpatho = true;
    suivant = pPatNew->ChercherItem(iter); //le premier frère de iter
  }

  if ((pPatNew->end() != suivant) && ((PatPathoIter) NULL != suivant))
    sSuivant = true;

  ClasseStringVector* pVect = new ClasseStringVector;
  DecomposeChaine(&sChaineInsertion, pVect, separateur);

  if (false == pVect->empty())
  {
    iterClassString iterpVect = pVect->begin();
    Message* pMessage = new Message;

    while (pVect->end() != iterpVect)
    {
      //traiter le cas des étiquettes multiples
      ClasseStringVector* pVectEtiqMulti = new ClasseStringVector;
      string sMultiLabel = (*iterpVect)->getItem();
      DecomposeChaine(&sMultiLabel, pVectEtiqMulti, "/");
      iterClassString iterpVectMulti  = pVectEtiqMulti->begin();
      int TailleEtiquette = pVectEtiqMulti->size();
      if (bChaineDansPatpatho)
      {
        if (sSuivant)
        {
          PatPathoIter jter = pPatNew->begin();
          jter = suivant;
          for (; pPatNew->end() != jter; jter++)
            (*jter)->setLigne((*jter)->getLigne() + TailleEtiquette);
          //insérer les éléments d'une étiquette multiple sur la même ligne
          if (false == pVectEtiqMulti->empty())
          {
            iterpVectMulti  = pVectEtiqMulti->begin();
            string sItem = (*iterpVectMulti)->getItem();
            PatPathoIter fils = ChercherItem(sChaine + "/" + sItem, "/");
            if ((end() != fils) && ((PatPathoIter) NULL != fils))
            {
              pMessage->SetComplement((*fils)->getComplement());
              pMessage->SetCertitude((*fils)->getCertitude());
              pMessage->SetInteret((*fils)->getInteret());
              pMessage->SetPluriel((*fils)->getPluriel());
              pMessage->SetUnit((*fils)->getUnit());
              pMessage->SetVisible((*fils)->getVisible());
              pMessage->SetTexteLibre((*fils)->getTexteLibre());
              pMessage->SetNodeRight((*fils)->getNodeRight());
            }
            pPatNew->ajoutePatho(suivant, (*iterpVectMulti)->getItem(),
                                             pMessage, Colonne , -1);

            if ((pPatNew->end() != suivant) && ((PatPathoIter) NULL != suivant))
              suivant++;
            if (pPatNew->end() != suivant)
              sSuivant = true;
            else
              sSuivant = false;
          }
        }
        else
        {
          //insérer les éléments d'une étiquette multiple sur la même ligne
          if (false == pVectEtiqMulti->empty())
          {
            iterpVectMulti  = pVectEtiqMulti->begin();
            string sItem = (*iterpVectMulti)->getItem();
            PatPathoIter fils = ChercherItem(sChaine + "/" + sItem, "/");
            if ((end() != fils) && ((PatPathoIter) NULL != fils))
            {
              pMessage->SetUnit((*fils)->getUnit());
              pMessage->SetComplement((*fils)->getComplement());
              pMessage->SetCertitude((*fils)->getCertitude());
              pMessage->SetInteret((*fils)->getInteret());
              pMessage->SetPluriel((*fils)->getPluriel());
              pMessage->SetVisible((*fils)->getVisible());
              pMessage->SetTexteLibre((*fils)->getTexteLibre());
              pMessage->SetNodeRight((*fils)->getNodeRight());
            }
            pPatNew->ajoutePatho((*iterpVectMulti)->getItem(), pMessage,
                                             Colonne);
          }
        }
      }
      else
      {
        string sItem;
        //insérer les éléments d'une étiquette multiple sur la même ligne
        if (false == pVectEtiqMulti->empty())
        {
          iterpVectMulti  = pVectEtiqMulti->begin();
          sItem = (*iterpVectMulti)->getItem();
          PatPathoIter fils = ChercherItem(sChaine + "/" + sItem, "/");
          if ((end() != fils) && ((PatPathoIter) NULL != fils))
          {
            pMessage->SetUnit((*fils)->getUnit());
            pMessage->SetComplement((*fils)->getComplement());
            pMessage->SetCertitude((*fils)->getCertitude());
            pMessage->SetInteret((*fils)->getInteret());
            pMessage->SetPluriel((*fils)->getPluriel());
            pMessage->SetVisible((*fils)->getVisible());
            pMessage->SetTexteLibre((*fils)->getTexteLibre());
            pMessage->SetNodeRight((*fils)->getNodeRight());
          }
          pPatNew->ajoutePatho((*iterpVectMulti)->getItem(), pMessage, Colonne);
          Colonne++;
        }
      }
      iterpVect++;
      delete pVectEtiqMulti;
    }
    delete pMessage;
  }
  delete pVect;
} // try
catch (...)
{
    erreur("Exception NSPatPathoArray SousPatpatho.", standardError);
}
}

string
NSPatPathoArray::donneCheminItem(PatPathoConstIter iter, char cSubSeparator)
{
    string sChemin = string("");

	if (empty())
        return sChemin;
  if (((PatPathoIter) NULL == iter) || (end() == iter) || (begin() == iter))
        return sChemin;

    int iCurCol = (*iter)->getColonne();
    int iCol    = 0;

    iter--;

	while (begin() != iter)
	{
        iCol = (*iter)->getColonne();
		if (iCol < iCurCol)
    {
        string sNode = (*iter)->getLexique();
      if (string("") != (*iter)->getPluriel())
          sNode += string(1, cSubSeparator) + (*iter)->getPluriel();
      if (string("") != (*iter)->getCertitude())
        sNode += string(1, cSubSeparator) + (*iter)->getCertitude();

      if (string("") == sChemin)
        sChemin = sNode;
      else
        sChemin = sNode + string(1, cheminSeparationMARK) + sChemin;

      iCurCol = iCol;
    }
    iter--;
	}

    iCol = (*iter)->getColonne();
	if (iCol < iCurCol)
	{
    string sNode = (*iter)->getLexique();
    if (string("") != (*iter)->getPluriel())
        sNode += string(1, cSubSeparator) + (*iter)->getPluriel();
    if (string("") != (*iter)->getCertitude())
        sNode += string(1, cSubSeparator) + (*iter)->getCertitude();

    if (string("") == sChemin)
        sChemin = sNode;
    else
        sChemin = sNode + string(1, cheminSeparationMARK) + sChemin;
	}

    return sChemin;
}

// This algorithm makes no arbitration; if needed, it must be done separately
//
void
NSPatPathoArray::mergePatpatho(NSPatPathoArray* pMergingPpt, PatPathoIter* pLocalFatherIter)
{
	if ((NULL == pMergingPpt) || (pMergingPpt->empty()))
        return;

    PatPathoIter itThis;

  // Global merging
  //
  if ((PatPathoIter*) NULL == pLocalFatherIter)
  {
  	if (empty())
    {
        *this = *pMergingPpt;
      return;
    }
        itThis = begin();
  }
  // Merging for a subpatpatho
  //
	else
  {
        itThis = ChercherPremierFils(*pLocalFatherIter);
    if (((PatPathoIter) NULL == itThis) || (end() == itThis))
    {
        InserePatPathoFille(*pLocalFatherIter, pMergingPpt);
      return;
    }
	}

  // Iterating through incoming ppt
  //
    PatPathoIter itInput = pMergingPpt->begin();

  // We will iterate first level branchs after root
  //
  itInput = pMergingPpt->ChercherPremierFils(itInput);

  while (pMergingPpt->end() != itInput)
	{
    // Is it a leaf?
    //
    PatPathoIter itInputFirstChild = pMergingPpt->ChercherPremierFils(itInput);

    // Not a leaf for input patho
    //
    if (((PatPathoIter) NULL != itInputFirstChild) && (pMergingPpt->end() != itInputFirstChild))
    {
        bool bLookingForSameBranch = true;
      PatPathoIter itLocal = itThis;

    	while ((end() != itLocal) && bLookingForSameBranch)
      {
    		if ((*itInput)->sameContent(*itLocal))
        {
            NSPatPathoArray subPPT(_pSuper);
          pMergingPpt->ExtrairePatPathoWithRoot(itInput, &subPPT);

          // recursive call
          //
          if (false == subPPT.empty())
            mergePatpatho(&subPPT, &itLocal);

            bLookingForSameBranch = false;
        }
        else
            itLocal = ChercherFrere(itLocal);
      }

      // not found: just add
      //
      if (bLookingForSameBranch)
      {
        NSPatPathoArray toInsertPPT(_pSuper);
        pMergingPpt->ExtrairePatPathoWithRoot(itInput, &toInsertPPT);

        InserePatPathoFille(*pLocalFatherIter, &toInsertPPT);
      }
    }
    // We are on a leaf for input patho
    //
    else
    {
        bool bLookingForSameBranch = true;
      PatPathoIter itLocal = itThis;
    	while ((end() != itLocal) && bLookingForSameBranch)
      {
        PatPathoIter itLocalFirstChild = ChercherPremierFils(itLocal);

        // First case: local node is also a leaf
        //
        if ((end() == itLocalFirstChild) || ((PatPathoIter) NULL == itLocalFirstChild))
        {
      		// If not equal, either add or replace value
        	//
    			if (false == (*itInput)->sameContent(*itLocal))
      		{
        		// The only case where we replace the node is when we find the same
          	// concept, the same unit, the same certitude and plural
      			if (((*itInput)->getLexiqueSens(_pSuper)   == (*itLocal)->getLexiqueSens(_pSuper))   &&
            	  ((*itInput)->getCertitudeSens(_pSuper) == (*itLocal)->getCertitudeSens(_pSuper)) &&
            	  ((*itInput)->getPlurielSens(_pSuper)   == (*itLocal)->getPlurielSens(_pSuper))   &&
            	  ((*itInput)->getTexteLibre() == string("")))
            {
            	// Same unit, we replace
              //
            	if ((*itInput)->getUnitSens(_pSuper) == (*itLocal)->getUnitSens(_pSuper))
                (*itLocal)->getOtherNodeContent(*itInput);
              //
              // Different unit, we add
              //
              else
              {
                NSPatPathoArray toInsertPPT(_pSuper);
                        toInsertPPT.ajoutePatho(itInput, 0, 0);
                        InserePatPathoFille(*pLocalFatherIter, &toInsertPPT);
              }
              bLookingForSameBranch = false;
            }
      		}
        	// If equal, do nothing
        	//
        	else
                bLookingForSameBranch = false;
        }
        // Second case: local node is not a leaf (means you can't modify it)
        //
        else
        {
        	if ((*itInput)->sameContent(*itLocal))
            bLookingForSameBranch = false;
        }

        if (bLookingForSameBranch)
            itLocal = ChercherFrere(itLocal);
      }

      // not found: just add
      //
      if (bLookingForSameBranch)
      {
        NSPatPathoArray toInsertPPT(_pSuper);
        toInsertPPT.ajoutePatho(itInput, 0, 0);
        InserePatPathoFille(*pLocalFatherIter, &toInsertPPT);
      }
    }

    // Switching to next node on the same column
    //
    itInput = pMergingPpt->ChercherFrere(itInput);
  }
}

/*
void
NSPatPathoArray::mergePatpatho(NSPatPathoArray* pMergingPpt, PatPathoIter* pLocalFatherIter)
{
	if ((NULL == pMergingPpt) || (pMergingPpt->empty()))
        return;

    PatPathoIter itThis;

  // Global merging
  //
  if ((PatPathoIter*) NULL == pLocalFatherIter)
  {
  	if (empty())
    {
        *this = *pMergingPpt;
      return;
    }
        itThis = begin();
  }
  // Merging for a subpatpatho
  //
	else
  {
        itThis = ChercherPremierFils(*pLocalFatherIter);
    if (((PatPathoIter) NULL == itThis) || (end() == itThis))
    {
        InserePatPathoFille(*pLocalFatherIter, pMergingPpt);
      return;
    }
	}

  // Iterating through incoming ppt
  //
    PatPathoIter itInput = pMergingPpt->begin();
  while (pMergingPpt->end() != itInput)
	{
    //
    //
    PatPathoIter itInputFirstChild = pMergingPpt->ChercherPremierFils(itInput);

    // Not a leaf for input patho
    //
    if (((PatPathoIter) NULL != itInputFirstChild) && (pMergingPpt->end() != itInputFirstChild))
    {
        bool bLookingForSameBranch = true;
      PatPathoIter itLocal = itThis;

    	while ((end() != itLocal) && bLookingForSameBranch)
      {
    		if ((*itInput)->sameContent(*itLocal))
        {
            NSPatPathoArray subPPT(_pSuper);
            pMergingPpt->ExtrairePatPatho(itInput, &subPPT);

          // recursive call
          //
          if (false == subPPT.empty())
            mergePatpatho(&subPPT, &itLocal);

            bLookingForSameBranch = false;
        }
        else
            itLocal = ChercherFrere(itLocal);
      }

      // not found: just add
      //
      if (bLookingForSameBranch)
      {
        NSPatPathoArray subPPT(_pSuper);
        pMergingPpt->ExtrairePatPatho(itInput, &subPPT);

        NSPatPathoArray toInsertPPT(_pSuper);
        toInsertPPT.ajoutePatho(itInput, 0, 0);
        toInsertPPT.InserePatPathoFille(toInsertPPT.begin(), &subPPT);

        InserePatPathoFille(*pLocalFatherIter, &toInsertPPT);
      }
    }
    // We are on a leaf for input patho
    //
    else
    {
        bool bLookingForSameBranch = true;
      PatPathoIter itLocal = itThis;
    	while ((end() != itLocal) && bLookingForSameBranch)
      {
        PatPathoIter itLocalFirstChild = ChercherPremierFils(itLocal);

        // First case: local node is also a leaf
        //
        if ((end() == itLocalFirstChild) || ((PatPathoIter) NULL == itLocalFirstChild))
        {
      		// If not equal, either add or replace value
        	//
    			if (!((*itInput)->sameContent(*itLocal)))
      		{
        		// The only case where we replace the node is when we find the same
          	// concept, the same unit, the same certitude and plural
      			if (((*itInput)->getLexiqueSens(_pSuper)   == (*itLocal)->getLexiqueSens(_pSuper))   &&
            	  ((*itInput)->getCertitudeSens(_pSuper) == (*itLocal)->getCertitudeSens(_pSuper)) &&
            	  ((*itInput)->getPlurielSens(_pSuper)   == (*itLocal)->getPlurielSens(_pSuper))   &&
            	  ((*itInput)->getTexteLibre() == string("")))
            {
            	// Same unit, we replace
              //
            	if ((*itInput)->getUnitSens(_pSuper) == (*itLocal)->getUnitSens(_pSuper))
                (*itLocal)->getOtherNodeContent(*itInput);
              //
              // Different unit, we add
              //
              else
              {
                NSPatPathoArray toInsertPPT(_pSuper);
                        toInsertPPT.ajoutePatho(itInput, 0, 0);
                        InserePatPathoFille(*pLocalFatherIter, &toInsertPPT);
              }
              bLookingForSameBranch = false;
            }
      		}
        	// If equal, do nothing
        	//
        	else
                bLookingForSameBranch = false;
        }
        // Second case: local node is not a leaf (means you can't modify it)
        //
        else
        {
        	if ((*itInput)->sameContent(*itLocal))
            bLookingForSameBranch = false;
        }

        if (bLookingForSameBranch)
            itLocal = ChercherFrere(itLocal);
      }

      // not found: just add
      //
      if (bLookingForSameBranch)
      {
        NSPatPathoArray toInsertPPT(_pSuper);
        toInsertPPT.ajoutePatho(itInput, 0, 0);
        InserePatPathoFille(*pLocalFatherIter, &toInsertPPT);
      }
    }

    // Switching to next node on the same column
    //
    itInput = pMergingPpt->ChercherFrere(itInput);
  }
}
*/


// Paths from aSubPaths that are in current ppt and not in merging one must be deleted
//
void
NSPatPathoArray::mergePatpathoDirected(NSPatPathoArray* pMergingPpt, PatPathoIter* pLocalFatherIter, vector<string*> *pSubPaths)
{
  if ((NULL == pMergingPpt) || (pMergingPpt->empty()))
        return;

  // First, merge
  //
  mergePatpatho(pMergingPpt, pLocalFatherIter);

  if (empty() || (NULL == pSubPaths) || pSubPaths->empty())
    return;

  string sRootPath = donneCheminItem(*pLocalFatherIter);

  // Next, remove all paths from this that are in the list and don't appear in Merging
  //
  vector<string*>::iterator it = pSubPaths->begin();
  for (; pSubPaths->end() != it; it++)
  {
    string sIt = **it;

    if (CheminDansPatpatho(sRootPath + string(1, cheminSeparationMARK) + sIt) &&
        (false == pMergingPpt->CheminDansPatpatho(sIt)))
      SupprimerChemin(sRootPath + string(1, cheminSeparationMARK) + sIt);
  }
}

//---------------------------------------------------------------------------
//taille d'une patpatho : les éléments d'une même ligne comptent pour un seul élément
//---------------------------------------------------------------------------
int
NSPatPathoArray::Taille()
{
   	if (empty())
        return 0;

    int taille = 1;

   	if (size() == 1)
        return 1;
   	else
   	{
   		//taille de la patpatho
        PatPathoIter itFils    = begin();
        PatPathoIter suivant   = begin();
        if (suivant != end())
            suivant++;
   		while(suivant != end())
   		{
   			if((*suivant)->getLigne() != (*itFils)->getLigne())
                taille++;
            itFils = suivant;
            suivant++;
   		}
	}
    return taille;
}

//---------------------------------------------------------------------------
//ajout d'un élément à la patpatho juste avant l'élément iterAvant
//---------------------------------------------------------------------------
void
NSPatPathoArray::ajoutePatho(PatPathoConstIter iterAvant, string sEtiquette,
			                 int colonne, int decalageLigne)
{
try
{
  if ((string("") == sEtiquette) || (colonne < 0))
    return;

  NSPatPathoInfo* pPatPatho = new NSPatPathoInfo();
  pPatPatho->setLexique(sEtiquette);
  pPatPatho->setNodeID(string(""));
  pPatPatho->setComplement(string(""));
  pPatPatho->setCertitude(string(""));
  pPatPatho->setInteret(string("A"));
  pPatPatho->setPluriel(string(""));
  pPatPatho->setVisible(string(""));
  pPatPatho->setUnit(string(""));
  pPatPatho->setTexteLibre(string(""));
  pPatPatho->setArchetype(string(""));
  pPatPatho->setNodeRight(string(""));
  pPatPatho->setColonne(colonne);

  int ligne;

  if (empty())
  {
    ligne = decalageLigne;
    push_back(pPatPatho);
  }
  else if ((end() == iterAvant) || ((PatPathoIter) NULL == iterAvant))
  {
    PatPathoIter iter = end();
    iter--;
    ligne = (*iter)->getLigne() + decalageLigne;
    push_back(pPatPatho);
  }
  else
  {
    ligne = (*iterAvant)->getLigne() + decalageLigne;
    pPatPatho->setLigne(ligne);

    // Put iter where iterAvant is.
    // We cannot write "iter = iterAvant" because iterAvant is a const_iter
    // See http://www.drdobbs.com/cpp/three-guidelines-for-effective-iterator/184401406?pgno=2
    //
    PatPathoIter iter = begin();
    advance(iter, distance<PatPathoConstIter>(iter, iterAvant));

    insert(iter, pPatPatho);

    // Décalage des lignes suivantes
    if (decalageLigne >= 0)
    {
      PatPathoConstIter jter = iter;
      jter++;
      for (; end() != jter; iter++)
        (*jter)->setLigne((*jter)->getLigne() + decalageLigne + 1);
    }
  }
} // try
catch (...)
{
    erreur("Exception NSPatPathoArray ajoutePatho 1.", standardError);
}
}

//---------------------------------------------------------------------------
//ajout d'un élément à la patpatho en changeant ses coordonnées
//---------------------------------------------------------------------------
void
NSPatPathoArray::ajoutePatho(PatPathoConstIter iter, int ligne, int colonne)
{
try
{
  if (((PatPathoIter) NULL == iter) || (end() == iter))
        return;

    NSPatPathoInfo* pPatPatho = new NSPatPathoInfo();
    *(pPatPatho->getDataTank()) = *((*iter)->getDataTank());

    pPatPatho->setLigne(ligne);
    pPatPatho->setColonne(colonne);
  
    push_back(pPatPatho);
} // try
catch (...)
{
    erreur("Exception NSPatPathoArray ajoutePatho 2.", standardError);
}
}

//---------------------------------------------------------------------------
// Ajout d'un élément à la patpatho en cours avant iterAvant
// pMessage contient les données patpatho sauf le champ lexique qui, lui,
// est contenu dans sEtiquette
//---------------------------------------------------------------------------
void
NSPatPathoArray::ajoutePatho(PatPathoConstIter iterAvant, string sEtiquette,
                   Message* pMessage, int colonne, int decalageLigne,
                   bool bPushBlock)
{
try
{
  NSPatPathoInfo* pPatPatho = new NSPatPathoInfo();

  pPatPatho->initFromMessage(pMessage);
  pPatPatho->setLexique(sEtiquette);

  pPatPatho->setColonne(colonne);

  // Si on insère avant un véritable élément
  //
  if (end() != iterAvant)
  {
    pPatPatho->setLigne((*iterAvant)->getLigne() + decalageLigne);

    PatPathoIter iter = begin();

    // Put iter where iterAvant is.
    // We cannot write "iter = iterAvant" because iterAvant is a const_iter
    // See http://www.drdobbs.com/cpp/three-guidelines-for-effective-iterator/184401406?pgno=2
    //
    advance(iter, distance<PatPathoConstIter>(iter, iterAvant));

    if (bPushBlock)
      for (PatPathoIter jter = iter; end() != jter; jter++)
        (*jter)->setLigne((*jter)->getLigne() + 1);

    insert(iter, pPatPatho);
  }
  // Si on insère avant la fin (alors on insère à la fin)
  //
  else
  {
    int ligne = -1;
    if (false == empty())
    {
      // On se place sur le dernier noeud
      iterAvant--;
      ligne = (*iterAvant)->getLigne();
    }
    pPatPatho->setLigne(ligne + decalageLigne);
    push_back(pPatPatho);
    // On se replace sur end()
    iterAvant = end();
  }
} // try
catch (...)
{
    erreur("Exception NSPatPathoArray ajoutePatho 3.", standardError);
}
}

void
NSPatPathoArray::parseBlock(string sNodeBlock, Message* pMessage)
{
  if (NULL == pMessage)
    return;

  pMessage->Reset();

  if (string("") == sNodeBlock)
    return;

  ClasseStringVector VectEtiqMulti;
  DecomposeChaine(&sNodeBlock, &VectEtiqMulti, string(1, intranodeSeparationMARK));
  if (VectEtiqMulti.empty())
    return;

  iterClassString iterpVectMulti = VectEtiqMulti.begin();

  string sEtiquette   = (*iterpVectMulti)->getItem();
  string sCertitude   = string("");
  string sPluriel     = string("");
  string sComplement  = string("");
  string sUnit        = string("");

  iterpVectMulti++;

  if ((sEtiquette[0] == '2') && (VectEtiqMulti.end() != iterpVectMulti))
  {
    sUnit      = sEtiquette;
    sEtiquette = (*iterpVectMulti)->getItem();
    iterpVectMulti++;
  }

  if (VectEtiqMulti.end() != iterpVectMulti)
  {
    bool bTourner = true;

    while (bTourner)
    {
      string sSuite = (*iterpVectMulti)->getItem();
      if (string("") != sSuite)
      {
        if      (string(sSuite, 0, 1) == string("$"))
          sComplement = string(sSuite, 1, strlen(sSuite.c_str())-1);
        else if (string(sSuite, 0, 3) == string("WCE"))
          sCertitude = sSuite;
        else if (string(sSuite, 0, 3) == string("WPL"))
          sPluriel = sSuite;
        else
          bTourner = false;
      }
      else
        bTourner = false;

      if (bTourner)
      {
        iterpVectMulti++;
        if (VectEtiqMulti.end() == iterpVectMulti)
          bTourner = false;
      }
    }
  }

  // Make certain codes are true codes and not semantic codes
  //
  NSDico::donneCodeComplet(sEtiquette);
  NSDico::donneCodeComplet(sUnit);
  NSDico::donneCodeComplet(sCertitude);
  NSDico::donneCodeComplet(sPluriel);

  pMessage->SetLexique(sEtiquette);
  pMessage->SetUnit(sUnit);
  pMessage->SetPluriel(sPluriel);
  pMessage->SetCertitude(sCertitude);
  pMessage->SetComplement(sComplement);
}

void
NSPatPathoArray::ajoutePathoBlock(string sNodeBlock, int colonne, PatPathoIter iterAvant)
{
  if (string("") == sNodeBlock)
    return;

  Message CodeMsg;
  parseBlock(sNodeBlock, &CodeMsg);

  string sEtiquette = CodeMsg.GetLexique();
  if (string("") == sEtiquette)
    return;

  if ((PatPathoIter) NULL != iterAvant)
    ajoutePatho(iterAvant, sEtiquette, &CodeMsg, colonne, 1);
  else
    ajoutePatho(sEtiquette, &CodeMsg, colonne);
}

//---------------------------------------------------------------------------
// Ajout du titre du compte rendu
//---------------------------------------------------------------------------
void
NSPatPathoArray::ajouteTitre(string sTitre, string sNoeud)
{
try
{
  if (string("") == sTitre)
    return;

  NSPatPathoInfo* pPatPatho = new NSPatPathoInfo();
  pPatPatho->setLexique(sTitre);

  // Précaution pas toujours inutile : si le noeud est composé, ne pas faire
  // déborder pDonnees->noeud
  if (strlen(sNoeud.c_str()) == PPD_NOEUD_LEN)
    pPatPatho->setNodeID(sNoeud);

  pPatPatho->setInteret("A");
  pPatPatho->setColonne(ORIGINE_PATH_PATHO);
  pPatPatho->setLigne(ORIGINE_PATH_PATHO);

  if (false == empty())
    insert(begin(), pPatPatho);
  else
    push_back(pPatPatho);
} // try
catch (...)
{
    erreur("Exception NSPatPathoArray ajouteTitre.", standardError);
}
}

//-------------------------------------------------------------------------
//chercher un élément dans la patpatho en cours selon son étiquette
//-------------------------------------------------------------------------
PatPathoIter
NSPatPathoArray::ChercherItem(string sItem, bool bPrepareNext, PatPathoIter iterFrom)
{
  if ((empty()) || (string("") == sItem))
    return (PatPathoIter) NULL;

	// Recherche sur le code sens ?
	//
    bool bCodeSens = false;
	if (strlen(sItem.c_str()) < BASE_LEXI_LEN)
        bCodeSens = true;

    PatPathoIter iterFound = end();
    PatPathoIter iter;

  if ((PatPathoIter) NULL == iterFrom)
        iter = begin();
	else
        iter = iterFrom;

  if ((bPrepareNext) && (end() != iter))
        iter++;

  while (end() != iter)
	{
        string sLexique = (*iter)->getLexique();
		if (bCodeSens)
        NSDico::donneCodeSens(sLexique);

    if (sLexique == sItem)
    {
        iterFound = iter;
      break;
    }
    else
        iter++;
	}

    return iterFound;
}

//------------------------------------------------------------------------
//chercher un élément dans la patpatho en cours selon son chemin
//-------------------------------------------------------------------------
PatPathoIter
NSPatPathoArray::ChercherItem(string sChemin, string separateur)
{
  string sousChaine;
  PatPathoIter iter = ChaineDansPatpatho(sChemin, &sousChaine, separateur);
  return iter;
}

//-------------------------------------------------------------------------
//chercher un élément dans la patpatho en cours selon sa localisation
//-------------------------------------------------------------------------
PatPathoIter
NSPatPathoArray::ChercherItem(int ligne, int colonne)
{
	if (empty())
    return (PatPathoIter) NULL;

  PatPathoIter iter = begin();
  while (end() != iter)
  {
    if (((*iter)->getLigne() == ligne) && ((*iter)->getColonne()  == colonne))
      return iter;
    else
      iter++;
  }
  return end();
}

PatPathoConstIter
NSPatPathoArray::ChercherItem(int ligne, int colonne) const
{
	if (empty())
    return (PatPathoConstIter) NULL;

  PatPathoConstIter iter = begin();
  while (end() != iter)
  {
    if (((*iter)->getLigne() == ligne) && ((*iter)->getColonne()  == colonne))
      return iter;
    else
      iter++;
  }
  return end();
}

//-------------------------------------------------------------------------
//chercher le premier frère de iterFrere (après ce dernier) ou le premier
//élément après iterFrere et dont la colonne est < à celle de iterFrere
//exemple : * cas 1 (iterFrere = A)
/*            M
						A
							B
                     	C
                     D
						E
              S
              dans ce cas retourner E

             * cas 2 (iterFrere = A)
             M
             		A
                  	B
                     	C
                     D
             E
             		S

             dans ce cas retourner E
*/
//
//-------------------------------------------------------------------------
PatPathoIter
NSPatPathoArray::ChercherItem(PatPathoConstIter iterFrere)
{
  if ((empty()) || ((PatPathoIter) NULL == iterFrere))
    return (PatPathoIter) NULL;
  if (end() == iterFrere)
    return end();

  int ligne   = (*iterFrere)->getLigne();
  int colonne = (*iterFrere)->getColonne();

  // Put iter where iterFrere is.
  // We cannot write "iter = iterFrere" because iterFrere is a const_iter
  // See http://www.drdobbs.com/cpp/three-guidelines-for-effective-iterator/184401406?pgno=2
  //
  PatPathoIter iter = begin();
  advance(iter, distance<PatPathoConstIter>(iter, iterFrere));

    iter++;
  while (end() != iter)
  {
    if (((*iter)->getLigne()  > ligne) &&
            ((*iter)->getColonne() <= colonne))
      return iter;
    else
      iter++;
  }
  return end();
}

//-------------------------------------------------------------------------
//chercher un élément dans la patpatho en cours, "fils" de iterPere
//-------------------------------------------------------------------------
PatPathoIter
NSPatPathoArray::ChercherItem(string sItem, PatPathoIter iterPere)
{
  if ((empty()) || ((PatPathoIter) NULL == iterPere))
    return (PatPathoIter) NULL;
  if (end() == iterPere)
        return end();

	// Recherche sur le code sens ?
	//
    bool bCodeSens = false;
	if (strlen(sItem.c_str()) < BASE_LEXI_LEN)
        bCodeSens = true;

    PatPathoIter iter = begin();
    int ligne   = (*iterPere)->getLigne();
    int colonne = (*iterPere)->getColonne();
    iter = iterPere;
    iter++;
	while ((end() != iter) && ((*iter)->getColonne() > colonne))
	{
        string sLexique = (*iter)->getLexique();
		if (bCodeSens)
        NSDico::donneCodeSens(sLexique);

		if (((*iter)->getLigne() > ligne  ) && (sLexique == sItem))
            return iter;
		else
            iter++;
	}
    return end();
}

PatPathoIter
NSPatPathoArray::ChercherPere(PatPathoConstIter iterFils)
{
  if (empty() || (begin() == iterFils) || ((PatPathoIter) NULL == iterFils))
    return (PatPathoIter) NULL;
  if (iterFils == begin())
    return (PatPathoIter) NULL;

  int colonne = (*iterFils)->getColonne();

  if (0 == colonne)
    return (PatPathoIter) NULL;

  // Put iter where iterFils is.
  // We cannot write "iter = iterFils" because iterFils is a const_iter
  // See http://www.drdobbs.com/cpp/three-guidelines-for-effective-iterator/184401406?pgno=2
  //
  PatPathoIter iter = begin();
  advance(iter, distance<PatPathoConstIter>(iter, iterFils));

  while (begin() != iter)
  {
    iter--;

    if ((*iter)->getColonne() == colonne - 1)
      return iter;
  }

  if ((*iter)->getColonne() == colonne - 1)
    return iter;

  return (PatPathoIter) NULL;
}


PatPathoIter
NSPatPathoArray::ChercherFrere(PatPathoConstIter iterFrere)
{
  if (empty() || ((PatPathoIter) NULL == iterFrere))
    return (PatPathoIter) NULL;

/*
//  a priori mauvais copier/coller le frere d'un patpathoiter peut-être avant ou apres
    if (iterFrere == end())
   	    return end();
*/

    int ligne   = (*iterFrere)->getLigne();
    int colonne = (*iterFrere)->getColonne();

  // Put iter where iterFrere is.
  // We cannot write "iter = iterFrere" because iterFrere is a const_iter
  // See http://www.drdobbs.com/cpp/three-guidelines-for-effective-iterator/184401406?pgno=2
  //
  PatPathoIter iter = begin();
  advance(iter, distance<PatPathoConstIter>(iter, iterFrere));

    iter++;
	while ((end() != iter) && ((*iter)->getColonne() >= colonne))
	{
		if (((*iter)->getLigne() > ligne) && ((*iter)->getColonne() == colonne))
            return iter;
		else
        iter++;
	}

    return end();
}

PatPathoIter
NSPatPathoArray::ChercherPremierFils(PatPathoConstIter iterPere)
{
  if (empty() || ((PatPathoIter) NULL == iterPere))
    return (PatPathoIter) NULL;
  if (end() == iterPere)
        return end();

  int colonne = (*iterPere)->getColonne();

  // Put iter where iterPere is.
  // We cannot write "iter = iterPere" because iterPere is a const_iter
  // See http://www.drdobbs.com/cpp/three-guidelines-for-effective-iterator/184401406?pgno=2
  //
  PatPathoIter iter = begin();
  advance(iter, distance<PatPathoConstIter>(iter, iterPere));

    iter++;

  if ((end() == iter) || ((*iter)->getColonne() != colonne + 1))
        return end();

    return iter;
}

PatPathoIter
NSPatPathoArray::ChercherNoeud(string sNodeId)
{
  if ((empty()) || (string("") == sNodeId))
    return (PatPathoIter) NULL;

    PatPathoIter iter = begin();
  for (; (end() != iter) && ((*iter)->getNode() != sNodeId); iter++);

  return (PatPathoIter) iter;
}

//-------------------------------------------------------------------
//			supprimer l'item Supprimer et tous ses descendants
// 		 		pNbSupprime : nombre d'éléments supprimés
//-------------------------------------------------------------------
void
NSPatPathoArray::SupprimerItem(PatPathoIter Supprimer)
{
	if (empty())
        return;

  if (((PatPathoIter) NULL == Supprimer) || (end() == Supprimer))
        return;

    int NbSupprime = 1;
    int ligneSupprimer   = (*Supprimer)->getLigne();
    int colonneSupprimer = (*Supprimer)->getColonne();

	//
	// Enlever l'élément Supprimer et tous ses descendants
	//
    PatPathoIter j = begin();
    j = Supprimer;
	if (j != end())
        j++;

  // Deleting the sons
  //
    int lastLigne = 0; //dernière ligne rencontrée
    bool bContinuer = true;
	while ((bContinuer) && (end() != j))
	{
    int ligneFils = (*j)->getLigne();
    if (((*j)->getColonne() > colonneSupprimer) && (ligneFils >= ligneSupprimer))
    {
			if (ligneFils > lastLigne) //2 éléments peuvent être sur la même ligne
        NbSupprime++;
      lastLigne = (*j)->getLigne();
      delete *j;
      erase(j);
    }
    else
        bContinuer = false;
	}

	//
	// Enlever Supprimer
  //
    delete *Supprimer;
    erase(Supprimer);

	if (empty())
        return;

    PatPathoIter element = begin();
	while (element != end())
	{
  	if ((*element)->getLigne() > ligneSupprimer)
        (*element)->setLigne((*element)->getLigne() - NbSupprime );
    element++;
	}
}

void
NSPatPathoArray::SupprimerFils(PatPathoIter iPptPere)
{
	if (empty())
    return;
  if (((PatPathoIter) NULL == iPptPere) || (end() == iPptPere))
    return;

  int iColPere = (*iPptPere)->getColonne();

  PatPathoIter iter = iPptPere;
  iter++;
  if (end() == iter)
    return;

  int iCol = (*iter)->getColonne();
  while ((end() != iter) && (iCol > iColPere))
  {
    SupprimerItem(iter);

    iter = iPptPere;
    iter++;
    if (end() == iter)
      return;

    iCol = (*iter)->getColonne();
  }
}

//------------------------------------------------------------------
//suppression d'un chemin dans une patpatho
//------------------------------------------------------------------
void
NSPatPathoArray::SupprimerChemin(string sChaine, string sValeur, string separateur )
{}

void
NSPatPathoArray::SupprimerChemin(string sChaine, string separateur)
{
  if (string("") == sChaine)
        return;

  string sousChaine = string("");
	//itérateur correspondant au dernier élément de sChaine
    PatPathoIter Supprimer = ChaineDansPatpatho(sChaine, &sousChaine);
  if (((PatPathoIter) NULL == Supprimer) || (end() == Supprimer))
        return;

    int ligneSupprimer   = (*Supprimer)->getLigne();
    int colonneSupprimer = (*Supprimer)->getColonne();
	//
	// enlever l'élément Supprimer et tous ses descendants
	//
    SupprimerItem(Supprimer);
	//supprimer éventuellement le père de Supprimer
    SupprimerPere(ligneSupprimer, colonneSupprimer);
}

//-----------------------------------------------------------------------
//				former la localisation fil guide d'un élément
//				patpatho de coordonnées(ligneElement, colonneElement)
//			bCodeSens : mettre les codes sens ou les codes lexique
//-----------------------------------------------------------------------
void
NSPatPathoArray::FormeLocalisation(PatPathoIter Element,
                              string* pLocalisation, int ligneElement,
                              int colonneElement, bool bCodeSens, string separateur)
{
  if (empty() || ((PatPathoIter) NULL == Element) || (end() == Element) || (NULL == pLocalisation))
    return;

  string sCodeSens = string("");
  PatPathoIter Temp = begin();
  PatPathoIter Root = begin();
  Root = Element;
  bool continuer = true;
  if ((*Element)->getColonne() == ORIGINE_PATH_PATHO)
  {
    string sEltLex = (*Element)->getLexique();
    if (bCodeSens)
      NSDico::donneCodeSens(&sEltLex, &sCodeSens);
    else
      sCodeSens = sEltLex;
    *pLocalisation = sCodeSens; //exemple GECHY
    return;
  }

  if (continuer)
  {
    Temp = Element;
    if (Temp == begin())
    {
      string sEltLex = (*(begin()))->getLexique();
      if (bCodeSens)
        NSDico::donneCodeSens(&sEltLex, &sCodeSens);
      else
        sCodeSens = sEltLex;
      *pLocalisation = sCodeSens; //exemple GECHY
      return;
    }
    Temp--;
    if (Temp == begin())
    {
      string sEltLex = (*(begin()))->getLexique();
      if (bCodeSens)
        NSDico::donneCodeSens(&sEltLex, &sCodeSens);
      else
        sCodeSens = sEltLex;
      *pLocalisation = sCodeSens; //exemple GECHY
      return;
    }
  }
  int i = 1;
  bool bPlusieursPeres = false; //chemin formé d'au moins 2 éléments : exemple GECHY/0CONT

  while ((Temp != begin()) && continuer)
  {
    if (((*Temp)->getColonne()  == (colonneElement - 1)) &&
        ((*Temp)->getLigne()    < ligneElement))
    {
      bPlusieursPeres = true;
      ligneElement   = (*Temp)->getLigne();
      colonneElement = (*Temp)->getColonne();

      string sEltLex = (*Temp)->getLexique();
      if (bCodeSens)
        NSDico::donneCodeSens(&sEltLex, &sCodeSens);
      else
        sCodeSens = sEltLex;
      if (i == 1)
        *pLocalisation = sCodeSens;
      else
        *pLocalisation = sCodeSens + separateur + *pLocalisation;

      i++;
    }
    if ((*Temp)->getColonne() == ORIGINE_PATH_PATHO)
      continuer = false;
    else
    {
      Temp--;
      Root = Temp; //sortie
    }
  }
  if (Root == begin())
  {
    string sEltLex = (*Root)->getLexique();
    if (bCodeSens)
      NSDico::donneCodeSens(&sEltLex, &sCodeSens);
    else
      sCodeSens = sEltLex;
    if (bPlusieursPeres)  //exemple GECHY/0CONT/0CONX
      *pLocalisation = sCodeSens + separateur + *pLocalisation;
    else
      *pLocalisation = sCodeSens; //exemple GECHY
  }
}

//--------------------------------------------------------------------
// supprimer le père de l'élément de coordonnées(ligneSupprimer, colonneSupprimer)
//						traiter le cas oas où le père est actfVide ou non
//--------------------------------------------------------------------
void
NSPatPathoArray::SupprimerPere(int ligneSupprimer, int colonneSupprimer)
{
try
{
  if (empty())
    return;

  // PatPathoIter ancetre = begin(); //gechy
  PatPathoIter PereSupprimer = begin();

  //
  // commencer par chercher le père
  //

  bool trouve = false;
  //
  // find first candidate: first node with column = colonneSupprimer - 1
  // trouver le premier candidat
  //
  PatPathoIter iter = begin();

    for (iter = begin(); (end() != iter) && (false == trouve); iter++)
 	{
 		if (((*iter)->getLigne() < ligneSupprimer) &&
        ((*iter)->getColonne() == (colonneSupprimer - 1)))
    {
      trouve = true;
      PereSupprimer = iter;
    }
  }

  if (false == trouve)
    return;

  // find the closest candidate
  // chercher éventuellement un autre père dont la ligne est < à celle du premier père trouvé
  //
  PatPathoIter jter = PereSupprimer;
  jter++;
  while (end() != jter)
  {
    if (((*jter)->getLigne()   < ligneSupprimer) &&
        ((*jter)->getColonne() == (colonneSupprimer - 1)))
      PereSupprimer = jter;

    jter++;
  }

	if ((end() == PereSupprimer) || (begin() == PereSupprimer))
    return;

  // Father node found
  // on a trouvé le père
  //
  int lignePere   = (*PereSupprimer)->getLigne();
  int colonnePere = (*PereSupprimer)->getColonne();

  string sLocalisation = string("");

  FormeLocalisation(PereSupprimer, &sLocalisation, lignePere, colonnePere, false, "/");

  bool bContinuer = true;
  //
  // chercher d'autres fils
  //
  PatPathoIter autreFils = PereSupprimer;
  autreFils++;
  while ((bContinuer) && (end() != autreFils))
  {
    if (((*autreFils)->getColonne() > colonnePere) &&
        ((*autreFils)->getLigne()   > lignePere  ))
      bContinuer = false; //on a trouvé au moins un autre fils : on sort alors
    else
      autreFils = end();
  }

  if (end() == autreFils)
  {
    //
    // Si le père de Supprimer n'a que celui_ci comme fils alors
    // faire une recherche sémantique pour savoir si le père de Supprimer est ActifVide
    // ou non. sinon supprimer ce père. Cette étape est récursive
    //
    VecteurRechercheSelonCritere vecteurSelonCritere(GUIDE);
    string sCodeSens;
    string sCodeLexique = (*PereSupprimer)->getLexique();
    NSDico::donneCodeSens(&sCodeLexique, &sCodeSens);

    vecteurSelonCritere.AjouteEtiquette(sCodeSens);
    _pSuper->getFilGuide()->chercheChemin(&sLocalisation,
                             &vecteurSelonCritere, NSFilGuide::compReseau);

    //
    // trouver la fiche du père
    //
    bool trouve;
    BBItemData Donnees;
    sCodeLexique = (*PereSupprimer)->getLexique();
    NSDico::donneCodeSens(&sCodeLexique, &sCodeSens);
    vecteurSelonCritere.SetData(sCodeSens, &trouve, &Donnees);
    bool ActifVide = true;
    if (trouve)
      if (false == Donnees.actifVide())
        ActifVide = false;

    //!trouve : pas de fiche, exemple en cas de conclusion automatique
    if (!trouve || !ActifVide)
    {
      delete *PereSupprimer;
      erase(PereSupprimer);
      //
      // MAJ ligbes et colonnes et des autres éléments patpatho
      //
      PatPathoIter element = begin();
      while (end() != element)
      {
        if ((*element)->getLigne() > lignePere)
	      				(*element)->setLigne((*element)->getLigne() - 1 );
          element++;
      }
      SupprimerPere(lignePere, colonnePere);
    }
  }
} // try
catch (...)
{
    erreur("Exception NSPatPathoArray SupprimerPere.", standardError);
}
}

//----------------------------------------------------------------------
// savoir si dans pPatpath les éléments de sChaine sont en relation de
// 							descendance entre eux.
//
// 				exemple sChaine : "0PREA1/A0PRM1/ABIVC1"
//----------------------------------------------------------------------
bool
NSPatPathoArray::CheminDansPatpatho(string sChaine, string separateur, PatPathoIter* pIter, PatPathoConstIter* pstartIter)
{
try
{
	if (pIter)
    *pIter = (PatPathoIter) NULL;

  if ((string("") == sChaine) || (empty()))
        return false;

  bool bStartIterWasMet = false;
  // if ((NULL != pstartIter) && (begin() != *pstartIter))
    //	return false;

  // If there is no start iter, we start searching from start
  //
  if ((NULL == pstartIter) || ((NULL != pstartIter) && (begin() == *pstartIter)))
    bStartIterWasMet = true;

  // Qualifiers are '/' for 'at tree root' and '.' for 'direct son of previous element'
  //
    ClasseStringVector Vect;
    DecomposeChaine(&sChaine, &Vect, separateur, string("/."));

	if (Vect.empty())
        return false;

    iterClassString iterpVect     = Vect.begin();
    iterClassString iterpVectTemp = Vect.begin();

    PatPathoIter iterPatPath = begin();

	//
	// pour chaque élément de pVect, scanner toute la patpatho, et mettre à jour les champs de
	// l'élément en question :
	//									colonne = sa colonne
	//   								coche   = élément détecté ou non;
	//   								sItem   = son lablel;
	//			(selon sa position par rapport à l élément de la patpatho)
	//
    for (; end() != iterPatPath; iterPatPath++ )
	{
    // Have we reached the point we can start searching from?
    //
    if ((false == bStartIterWasMet) && (NULL != pstartIter))
    {
      const NSPatPathoInfo* pStartPptInfo = **pstartIter;
      const NSPatPathoInfo* pCurrPptInfo  = *iterPatPath;
      if (pCurrPptInfo == pStartPptInfo)
        bStartIterWasMet = true;
    }

    // Uncheck elements whose column is greater than current node
    //
        int colonneEncours = (*iterPatPath)->getColonne();
        for (iterpVect = Vect.begin(); Vect.end() != iterpVect; iterpVect++)
		{
        int colonne = (*iterpVect)->getColumn();
      //décocher les éléments dont la colonne est >= colonneEncours
      if (colonne >= colonneEncours)
        (*iterpVect)->setChecked(false);
    }

    //trouver le premier élément de pVect non coché
    for (iterpVectTemp = Vect.begin(); (Vect.end() != iterpVectTemp) &&
             ((*iterpVectTemp)->getChecked()); iterpVectTemp++);

		if (Vect.end() != iterpVectTemp)
		{
      bool bQualifiersOk = true;

      // Is it ok for qualifiers
      //
      if ((NPOS != (*iterpVectTemp)->getQualifier().find("/")) && (0 != colonneEncours))
        bQualifiersOk = false;
      if (NPOS != (*iterpVectTemp)->getQualifier().find("."))
      {
        if (Vect.begin() != iterpVectTemp)
        {
          iterClassString iterFather = iterpVectTemp - 1;
          if ((*iterFather)->getColumn() != colonneEncours - 1)
            bQualifiersOk = false;
        }
      }

      if (bQualifiersOk)
      {
          string sItem = (*iterpVectTemp)->getItem();

        // on compose le noeud *iterPatPath avec ses attributs (pluriel et certitude)
        //
        string sEtiquette = (*iterPatPath)->getNodeLabel();

        //
        // Si le code "Oui" a été précisé dans la requête, on l'ajoute
        //
        string sCertitude = (*iterPatPath)->getCertitude();
        if (string("") == sCertitude)
        {
          size_t isYes = sItem.find("WCEA0");
          if (NPOS != isYes)
            sEtiquette += string(1, intranodeSeparationMARK) + string("WCEA01");
        }

        string sCodeSens;
        NSDico::donneCodeSens(&sEtiquette, &sCodeSens);
        string ssItemCodeSens;
        NSDico::donneCodeSens(&sItem, &ssItemCodeSens);

        if ((sCodeSens == ssItemCodeSens) || (string("£C;") == ssItemCodeSens) ||
                                             (string("£CX") == ssItemCodeSens))
        {
          (*iterpVectTemp)->setColumn((*iterPatPath)->getColonne());
          (*iterpVectTemp)->setChecked(true);

          // si c'est le dernier on sort
          if (bStartIterWasMet && (Vect.back() == *iterpVectTemp))
          {
            // on regarde si on avait un itérateur en parametre
            if (pIter)
              *pIter = iterPatPath;

            return true;
          }
        }
        // Check numerical
        //
        else if ((strlen(ssItemCodeSens.c_str()) > strlen(sCodeSens.c_str())) &&
                 (string(ssItemCodeSens, 0, strlen(sCodeSens.c_str())) == sCodeSens))
        {
          size_t iItemLength = strlen(ssItemCodeSens.c_str());
          size_t iCodeLength = strlen(sCodeSens.c_str());
          string sExcesString = string(ssItemCodeSens, iCodeLength, iItemLength - iCodeLength);
          if ((strlen(sExcesString.c_str()) > 2) && (string(sExcesString, 0, 2) == string(".$")))
          {
            string sValue = string(sExcesString, 2, strlen(sExcesString.c_str()) - 2);
            if (sValue == (*iterPatPath)->getComplement())
            {
              (*iterpVectTemp)->setColumn((*iterPatPath)->getColonne());
              (*iterpVectTemp)->setChecked(true);

              //si c'est le dernier on sort
              if (bStartIterWasMet && (Vect.back() == *iterpVectTemp))
              {
                // on regarde si on avait un itérateur en parametre
                if (pIter)
                  *pIter = iterPatPath;

                return true;
              }
            }
          }
        }
      }
    }
  }

	if (pIter)
        *pIter = iterPatPath;

    return false;
} // try
catch (...)
{
    erreur("Exception NSPatPathoArray CheminDansPatpatho 1.", standardError);
    return false;
}
}

//----------------------------------------------------------------------
// savoir si dans pPatpath les éléments de sChaine sont en relation de
// 							descendance entre eux.
//                     pBigBoss peut être null
// 				exemple sChaine : "0PREA1/A0PRM1/ABIVC1"
//			  retourner éventuellement dans sValeur la valeur chiffrée :
//						  complément de l'élément patpatho
//			"correspondant à un champ édit par exemple (fils du dernier élément
//      									de sChaine)"
//                  	  ou le champ lexique de cet élément
//----------------------------------------------------------------------
bool
NSPatPathoArray::CheminDansPatpatho(NSSmallBrother * /*pBigBoss */,
                                    string sChaine, string* pValeur,
                                    string* pUnite, string separateur) const
{
try
{
	if ((string("") == sChaine) || (empty()))
        return false;

  // Qualifiers are '/' for 'at tree root' and '.' for 'direct son of previous element'
  //
    ClasseStringVector Vect;
    DecomposeChaine(&sChaine, &Vect, separateur, string("/."));

	if (Vect.empty())
        return false;

    iterClassString iterpVect     = Vect.begin();
    iterClassString iterpVectTemp = Vect.begin();

    PatPathoConstIter iterValeur = begin();
    string sPluriel, sCertitude;
	//
	// Pour chaque élément de pVect, scanner toute la patpatho, et mettre à jour
	// les champs de l'élément en question :
	//				                    colonne = sa colonne;
	//   								coche   = élément détecté ou non;
	//   								sItem   = son lablel;
	//			(selon sa position par rapport à l élément de la patpatho)
	//
  PatPathoConstIter iterPatPath = begin();

    for (; end() != iterPatPath; iterPatPath++)
	{
        int colonneEncours = (*iterPatPath)->getColonne();
        for (iterpVect = Vect.begin(); Vect.end() != iterpVect; iterpVect++)
		{
            int colonne  = (*iterpVect)->getColumn();
			// Décocher les éléments dont la colonne est >= colonneEncours
			if (colonne >= colonneEncours)
                (*iterpVect)->setChecked(false);
		}

		// Trouver le premier élément de pVect non coché

        for (iterpVectTemp = Vect.begin(); (Vect.end() != iterpVectTemp) &&
                                    ((*iterpVectTemp)->getChecked()); iterpVectTemp++);
		if (Vect.end() != iterpVectTemp)
		{
      bool bQualifiersOk = true;

      // Is it ok for qualifiers
      //
      if ((NPOS != (*iterpVectTemp)->getQualifier().find("/")) && (0 != colonneEncours))
        bQualifiersOk = false;
      if (NPOS != (*iterpVectTemp)->getQualifier().find("."))
      {
        if (Vect.begin() != iterpVectTemp)
        {
          iterClassString iterFather = iterpVectTemp - 1;
          if ((*iterFather)->getColumn() != colonneEncours - 1)
            bQualifiersOk = false;
        }
      }

      if (bQualifiersOk)
      {
              string sItem = (*iterpVectTemp)->getItem();
              string sCodeSens;
              string ssItemCodeSens;

			  // on compose le noeud *iterPatPath avec ses attributs (pluriel et certitude)
              string sEtiquette = (*iterPatPath)->getNodeLabel();

/*
			if (pBigBoss)
			{
                _pSuper->getDico()->donneCodeSens(&sEtiquette, &sCodeSens);
                _pSuper->getDico()->donneCodeSens(&sItem, &ssItemCodeSens);
			}
			else if (pContexte)
			{
                pContexte->getDico()->donneCodeSens(&sEtiquette, &sCodeSens);
                pContexte->getDico()->donneCodeSens(&sItem, &ssItemCodeSens);
			}
			else
			{
                NSDico Dico(NULL);
                Dico.donneCodeSens(&sEtiquette, &sCodeSens);
                Dico.donneCodeSens(&sItem,      &ssItemCodeSens);
			}
*/

        NSDico::donneCodeSens(&sEtiquette, &sCodeSens);
        NSDico::donneCodeSens(&sItem, &ssItemCodeSens);

			  // s'il y a égalité des deux éléments
			  if (sCodeSens == ssItemCodeSens)
			  {
                  (*iterpVectTemp)->setColumn((*iterPatPath)->getColonne());
                  (*iterpVectTemp)->setChecked(true);
          //
				  // Si c'est le dernier on a trouvé
          //
				  if (Vect.back() == *iterpVectTemp)
				  {
        	  if (NULL == pValeur)
              return true;

					  // Si c'est un code, on regarde tout de suite s'il a un complement
					  // if (pContexte && (pContexte->getFilGuide()->Prend()) && (pContexte->getFilGuide()->VraiOuFaux(sCodeSens, "ES", "0CODE")))
#ifndef _ENTERPRISE_DLL
            if ((_pSuper->getFilGuide()->Prend()) && (_pSuper->getFilGuide()->VraiOuFaux(sCodeSens, string("ES"), string("0CODE"))))
#else
            if (_pSuper->getFilGuide()->VraiOuFaux(sCodeSens, string("ES"), string("0CODE")))
#endif
					  {
                          *pValeur = (*iterPatPath)->getComplement();
              if (string("") != *pValeur)
                return true;
					  }
					  //
					  // Chercher éventuellement le fils de iterpVectTemp ayant
					  // une valeur chiffrée
					  //
                      iterValeur = iterPatPath;
					  if (end() != iterValeur)
            {
              string sLexique = (*iterValeur)->getLexique();
              if ('£' != sLexique[0])
                            iterValeur++;
            }

					  if (end() != iterValeur)
					  {
						  //
              // Si valeur non chiffrée prendre iterValeur lui même
              // sinon le complément contenu dans £N0;0..
              //
              string sLexique = (*iterValeur)->getLexique();

						  if ('£' == sLexique[0])
						  {
            	  if (string("£CL") == string(sLexique, 0, 3))
                  *pValeur = (*iterValeur)->getTexteLibre();
                else
                {
                                  *pValeur = (*iterValeur)->getComplement();
								  if (pUnite)
                                      *pUnite = (*iterValeur)->getUnit();
                }
              }
              else
              {
                  *pValeur = sLexique;
                if (pUnite)
                  *pUnite = "";
              }
					  }
            return true;
				  }
        }
			}
		}
	}
	if (pValeur)
        *pValeur = string("");

    return false;
} // try
catch (...)
{
    erreur("Exception NSPatPathoArray CheminDansPatpatho 2.", standardError);
	return false;
}
}

//----------------------------------------------------------------------
// Savoir si dans pPatpath les éléments du VecteurItem sont en relation de
// descendance entre eux, sans que la chaine ne comporte d'interdit.
//----------------------------------------------------------------------
bool
NSPatPathoArray::CheminSemantiqueDansPatpatho(PatPathoIter* pIter, VecteurItem* pVecEquivalents, VectString* /* pInterdits */)
{
try
{
  *pIter = (PatPathoIter) NULL;

	if ((NULL == pVecEquivalents) || (pVecEquivalents->empty()) || (empty()))
        return false;

  //
  // Fabrication du vecteur qui nous permet de suivre où on en est
  //
  ClasseStringVector Vect;
  EquiItemVectorIter itEquivalent = pVecEquivalents->begin();
  for (; itEquivalent != pVecEquivalents->end(); itEquivalent++)
  {
  	if ((*itEquivalent)->empty())
        Vect.push_back(new classString(""));
    else
        Vect.push_back(new classString(*(*((*itEquivalent)->begin()))));
  }

  iterClassString iterpVect     = Vect.begin();
  iterClassString iterpVectTemp = Vect.begin();

  itEquivalent = pVecEquivalents->begin();

  PatPathoIter iterPatPath = begin();

  int colonneEncours;
  int colonne;

  string sItem;
  string sCertitude;
  string sPluriel;

	//
	// pour chaque élément de pVect, scanner toute la patpatho, et mettre à jour les champs de
	// l'élément en question :
	//									colonne = sa colonne
	//   								coche   = élément détecté ou non;
	//   								sItem   = son lablel;
	//			(selon sa position par rapport à l élément de la patpatho)
	//
    for (iterPatPath = begin(); end() != iterPatPath; iterPatPath++ )
	{
        colonneEncours = (*iterPatPath)->getColonne();

		// décocher les éléments dont la colonne est >= colonneEncours
		// uncheck all elements whose column is >= colonneEncours
        for (iterpVect = Vect.begin(); Vect.end() != iterpVect; iterpVect++)
		{
        colonne  = (*iterpVect)->getColumn();
      if (colonne >= colonneEncours)
        (*iterpVect)->setChecked(false);
    }

		//trouver le premier élément de pVect non coché
        for (iterpVectTemp = Vect.begin(); (Vect.end() != iterpVectTemp) &&
                    ((*iterpVectTemp)->getChecked()); iterpVectTemp++);

		if (Vect.end() != iterpVectTemp)
		{
        sItem = (*iterpVectTemp)->getItem();
      string sCodeSens;
      string ssItemCodeSens;

      // on compose le noeud *iterPatPath avec ses attributs (pluriel et certitude)
      string sEtiquette = (*iterPatPath)->getNodeLabel();

/*
      if (pContexte)
      {
        pContexte->getDico()->donneCodeSens(&sEtiquette, &sCodeSens);
        pContexte->getDico()->donneCodeSens(&sItem, &ssItemCodeSens);
      }
      else
      {
        ClasseStringVector Vect1;
        DecomposeChaine(&sEtiquette, &Vect1, string(1, intranodeSeparationMARK));
        iterString iterpVect = Vect1.begin();
        sCodeSens = string((*iterpVect)->sItem, 0, BASE_LEXIQUE_LEN - 1);

        while (iterpVect != Vect1.end())
        {
            sCodeSens += string(1, intranodeSeparationMARK) + string((*iterpVect)->sItem, 0, BASE_LEXIQUE_LEN - 1);
          iterpVect++;
        }

        Vect1.vider();
        DecomposeChaine(&sItem, &Vect1, string(1, intranodeSeparationMARK));

        iterpVect = Vect1.begin();
        ssItemCodeSens = string((*iterpVect)->sItem, 0, BASE_LEXIQUE_LEN - 1);

        while(iterpVect != Vect1.end())
        {
            ssItemCodeSens += string(1, intranodeSeparationMARK) + string((*iterpVect)->sItem, 0, BASE_LEXIQUE_LEN - 1);
          iterpVect++;
        }
      }
*/

      NSDico::donneCodeSens(&sEtiquette, &sCodeSens);
      NSDico::donneCodeSens(&sItem, &ssItemCodeSens);

      if (sCodeSens == ssItemCodeSens)
      {
        (*iterpVectTemp)->setColumn((*iterPatPath)->getColonne());
        (*iterpVectTemp)->setChecked(true);

      	//si c'est le dernier on sort
        if ((*iterpVectTemp) == Vect.back())
        {
        	// on regarde si on avait un itérateur en parametre
          if (pIter)
            *pIter = iterPatPath;

          return true;
        }
      }
    }
  }

	if (pIter)
        *pIter = iterPatPath;

    return false;
} // try
catch (...)
{
    erreur("Exception NSPatPathoArray CheminDansPatpatho 1.", standardError);
	return false;
}
}

#ifndef _ENTERPRISE_DLL
void
NSPatPathoArray::genereChaine(NSContexte *pContexte, string* pChaine)
{
	if (NULL == pChaine)
        return;

    *pChaine = string("");

	if (empty())
        return;

try
{
    for (PatPathoIter i = begin(); end() != i; i++)
	{
        *pChaine += string("{Fiche}");
        *pChaine += texteHtml((*i)->getLocalisation()) + string("|");
    if ((*i)->getType() != "")
        *pChaine += texteHtml((*i)->getType());
    *pChaine += string("|");

    string sLexique = (*i)->getLexique();
    *pChaine += texteHtml(sLexique) + string("|");

    string sLexiqueSens;
    NSDico::donneCodeSens(&sLexique, &sLexiqueSens);

    // Free text - getting text
    //
    if (string("£?????") == sLexique)
    {
        string sTexteLibre = (*i)->getTexteLibre();
      *pChaine += texteHtml(sTexteLibre) + string("|");
    }
    //
    // Material - getting serial number
    //
    else if (string("£SGMA") == sLexiqueSens)
    {
      NSMaterielInfo MatInfo;
      if (MatInfo.initialiseDepuisObjet(_pSuper, (*i)->getComplement()))
      {
        string sLabel = MatInfo.getLabelAndSN();

        string sCplt  = string("");
        if (string("") != MatInfo.getSN())
          sCplt = string("S/N:") + MatInfo.getSN() + string(":Label:") + sLabel;
        else
          sCplt = string("Label:") + sLabel;

        *pChaine += texteHtml(sCplt) + string("|");
      }
      else
        *pChaine += texteHtml((*i)->getComplement()) + string("|");
    }
    //
    // Doctor - getting professionnal identifier
    //
    else if (string("£SGDR") == sLexiqueSens)
    {
      string sPersonId = (*i)->getComplement();
      if (string("") != sPersonId)
		  {
        NSPersonInfo* pPersonInfoPtr = _pSuper->getPersonArray()->getPerson(pContexte, sPersonId, pidsCorresp);
        if (NULL == pPersonInfoPtr)
          *pChaine += texteHtml((*i)->getComplement()) + string("|");
        else
        {
          string sLabel = pPersonInfoPtr->getCivilite();

          string sCplt  = string("");
          if      (string("") != pPersonInfoPtr->getRpps())
            sCplt = string("RPPS:") + pPersonInfoPtr->getRpps() + string(":Label:") + sLabel;
          else if (string("") != pPersonInfoPtr->getAdeli())
            sCplt = string("ADELI:") + pPersonInfoPtr->getAdeli() + string(":Label:") + sLabel;
          else
            sCplt = string("Label:") + sLabel;

          *pChaine += texteHtml(sCplt) + string("|");
        }
      }
      else
        *pChaine += texteHtml((*i)->getComplement()) + string("|");
    }
    else
        *pChaine += texteHtml((*i)->getComplement()) + string("|");

		if ((*i)->getCertitude() != "")
        *pChaine += texteHtml((*i)->getCertitude());
    *pChaine += string("|");
    if ((*i)->getInteret() != "")
        *pChaine += texteHtml((*i)->getInteret());
    *pChaine += string("|");
    if ((*i)->getPluriel() != "")
        *pChaine += texteHtml((*i)->getPluriel());
    *pChaine += string("|");
    if ((*i)->getUnit() != "")
        *pChaine += texteHtml((*i)->getUnit());
		*pChaine += string("{/Fiche}");
	}
} // try
catch (...)
{
    erreur("Exception NSPatPathoArray genereChaine.", standardError);
}
}
#endif

#define MAXNODEELTS 50

void
NSPatPathoArray::initialiseDepuisChaine(NSContexte *pContexte, string* pChaine, string sPersonID)
{
  vider();

	if ((NULL == pChaine) || (string("") == *pChaine) || (strlen(pChaine->c_str()) < 15))
        return;

	//
	// La chaine doit commencer par "{PatPatho}" et se terminer par "{/PatPatho}"
	//
	if (string("{PatPatho") != string(*pChaine, 0, 9))
        return;

    bool bOldStringFormat = false;
    string sStringType = string(*pChaine, 10, 5);
  if ('3' == sStringType[3])
    bOldStringFormat = true;

    string sNomTag;
    string sFiche[MAXNODEELTS];
    int    index;

    int    iLastLineNumber = -1;

    size_t n = 10;
	while ((n < strlen(pChaine->c_str())) && (string("/PatPatho") != sNomTag))
	{
		// on lit le prochain tag
    sNomTag = string("");
    while ((n < strlen(pChaine->c_str())) && ('{' != (*pChaine)[n]))
        n++;
    n++;

    while ((n < strlen(pChaine->c_str())) && ('}' != (*pChaine)[n]))
        sNomTag += (*pChaine)[n++];
    n++;

    // New node
    //
  	if (sNomTag == string("Fiche"))
    {
      for (int i = 0; i < MAXNODEELTS; i++)
        sFiche[i] = string("");

        index = 0;

      // on lit chaque enregistrement jusqu'au tag de fin
      while ((n < strlen(pChaine->c_str())) && ('{' != (*pChaine)[n]))
      {
      	while ((n < strlen(pChaine->c_str())) &&
                			('{' != (*pChaine)[n]) && ('|' != (*pChaine)[n]))
            sFiche[index] += (*pChaine)[n++];

        if ('|' == (*pChaine)[n]) // tag champ suivant
        {
          n++;

            index++;
          if (index >= MAXNODEELTS)
          {
            erreur("Le fichier à importer contient une PatPatho erronée (un noeud a trop d'éléments).", standardError);
            return;
          }
        }
      }

      n++;

      if (n >= strlen(pChaine->c_str()))
      {
        // erreur("Le fichier à importer contient une PatPatho erronée.", standardError, 0, _pSuper->donneMainWindow()->GetHandle());
        erreur("Le fichier à importer contient une PatPatho erronée (pas de balise finale).", standardError);
        return;
      }
try
{
            NSPatPathoInfo* pInfo = new NSPatPathoInfo();

      pInfo->setPerson(sPersonID);
      pInfo->setDocum("");		// ce code est mis à jour après référencement
      pInfo->setLocalisation(texteCourant(sFiche[0]));
      pInfo->setType(texteCourant(sFiche[1]));
      pInfo->setLexique(texteCourant(sFiche[2]));

      string sLexiqueSens = pInfo->getLexiqueSens(_pSuper);

      // cas des textes libres : on récupère le texte situé dans le complement
      if (pInfo->getLexique() == string("£?????"))
        pInfo->setTexteLibre(texteCourant(sFiche[3]));
      //
      // Material - getting materiel ID from serial number
      //
      else if (string("£SGMA") == sLexiqueSens)
      {
        importNodeForMaterial(pInfo, texteCourant(sFiche[3]));
      }
      //
      // Doctor - getting professionnal identifier
      //
      else if (string("£SGDR") == sLexiqueSens)
      {
        importNodeForCorresp(pContexte, pInfo, texteCourant(sFiche[3]));
      }
      else
        pInfo->setComplement(texteCourant(sFiche[3]));

      pInfo->setCertitude(texteCourant(sFiche[4]));
            pInfo->setInteret(texteCourant(sFiche[5]));
      pInfo->setPluriel(texteCourant(sFiche[6]));
      if (false == bOldStringFormat)
			{
        pInfo->setUnit(texteCourant(sFiche[7]));
        push_back(pInfo);
			}
			else
			{
                int iNewLineNumber = pInfo->getLigne();
				if (iNewLineNumber == iLastLineNumber)
        {
        	// Example {Fiche}0k080000|1|2MM001|||A|{/Fiche}{Fiche}0k090000|1|£N0;03|41||A|{/Fiche}
            NSPatPathoInfo* pPreviousInfo = back();
          if (NULL != pPreviousInfo)
          {
            pPreviousInfo->setUnit(pPreviousInfo->getLexique());
            pPreviousInfo->setLexique(texteCourant(sFiche[2]));
            pPreviousInfo->setComplement(texteCourant(sFiche[3]));
            delete pInfo;
          }
        }
        else
            push_back(pInfo);

        iLastLineNumber = iNewLineNumber;
			}
} // try
catch (...)
{
    erreur("Exception NSPatPathoArray::initialiseDepuisChaine.", standardError);
    return;
}
		}
	}
}

void
NSPatPathoArray::importNodeForMaterial(NSPatPathoInfo* pInfo, string sCplt)
{
  if ((NULL == pInfo) || (string("") == sCplt))
    return;

#ifndef _ENTERPRISE_DLL

  // Parse serial number and label
  //
  string sSerialNumber = string("");
  string sLabel        = string("");
  getCodeAndLabel(sCplt, string("S/N"), sSerialNumber, sLabel);

  if (string("") != sSerialNumber)
  {
    NSMaterielInfo MatInfo;
    if (MatInfo.initialiseDepuisSN(_pSuper, sSerialNumber))
    {
      pInfo->setComplement(MatInfo.getObjectID());
      return;
    }
  }

  if (string("") != sLabel)
  {
    pInfo->setLexique(string("£?????"));
    pInfo->setComplement(string(""));
    pInfo->setTexteLibre(sLabel);
    return;
  }

  pInfo->setComplement(sCplt);
#endif
}

void
NSPatPathoArray::importNodeForCorresp(NSContexte *pContexte, NSPatPathoInfo* pInfo, string sCplt)
{
  if ((NULL == pInfo) || (string("") == sCplt))
    return;

#ifndef _ENTERPRISE_DLL

  string sCode  = string("");
  string sLabel = string("");

  // Parse RPPS and label
  //
  getCodeAndLabel(sCplt, string("RPPS"), sCode, sLabel);
  if (string("") != sCode)
  {
    NSPersonInfo* pPersonInfoPtr = _pSuper->getPersonArray()->getPersonFromRpps(pContexte, sCode);
    if (pPersonInfoPtr)
    {
      pInfo->setComplement(pPersonInfoPtr->getNss());
      return;
    }
    if (string("") != sLabel)
    {
      pInfo->setLexique(string("£?????"));
      pInfo->setComplement(string(""));
      pInfo->setTexteLibre(sLabel);
      return;
    }
  }

  // Parse Adeli and label
  //
  getCodeAndLabel(sCplt, string("ADELI"), sCode, sLabel);
  if (string("") != sCode)
  {
    NSPersonInfo* pPersonInfoPtr = _pSuper->getPersonArray()->getPersonFromAdeli(pContexte, sCode);
    if (pPersonInfoPtr)
    {
      pInfo->setComplement(pPersonInfoPtr->getNss());
      return;
    }
    if (string("") != sLabel)
    {
      pInfo->setLexique(string("£?????"));
      pInfo->setComplement(string(""));
      pInfo->setTexteLibre(sLabel);
      return;
    }
  }

  pInfo->setComplement(sCplt);

#endif
}

void
NSPatPathoArray::getCodeAndLabel(string sCplt, string sCode, string &sCodeTxt, string &sLabel)
{
  sCodeTxt = string("");
  sLabel   = string("");

  if ((string("") == sCplt) || (string("") == sCode))
    return;

  string sCodeSepar = sCode + string(":");

  // Does-it start with sCodeSepar?
  //
  size_t iCodeSeparLen = strlen(sCodeSepar.c_str());

  if ((strlen(sCplt.c_str()) > iCodeSeparLen) &&
            (string(sCplt, 0, iCodeSeparLen) == sCodeSepar))
  {
    sCodeTxt = string(sCplt, iCodeSeparLen, strlen(sCplt.c_str()) - iCodeSeparLen);
    size_t iLabelPos = sCodeTxt.find(":Label:");
    if (NPOS != iLabelPos)
    {
      sLabel   = string(sCodeTxt, iLabelPos + 7, strlen(sCodeTxt.c_str()) - iLabelPos - 7);
      sCodeTxt = string(sCodeTxt, 0, iLabelPos);
    }

    return;
  }

  if ((strlen(sCplt.c_str()) > 6) &&
            (string(sCplt, 0, 6) == string("Label:")))
    sLabel = string(sCplt, 6, strlen(sCplt.c_str()) - 6);
}

string
NSPatPathoArray::genereXML()
{
    string sXMLstring = "";

	if (empty())
        return sXMLstring;

    int iCurCol = -1;
    int iLevel  = 0;

	for (PatPathoIter it = begin(); it != end(); it++)
	{
        int colonneEncours = (*it)->getColonne();

    // Closing previous nodes
    //
		while (colonneEncours <= iCurCol)
		{
    	if (iLevel > 0)
        sXMLstring += string(2*iLevel, ' ');
      sXMLstring += string("</node>\r\n");
      iCurCol--;
      iLevel--;
    }
    iCurCol = colonneEncours;

    if (iLevel > 0)
        sXMLstring += string(2*iLevel, ' ');
    sXMLstring += string("<node");

    if (graphPerson == iArrayType)
    {
    	if ((*it)->getPerson() != "")
        sXMLstring += string(" patId=\"") + texteHtml((*it)->getPerson()) + string("\"");
      if ((*it)->getDocum() != "")
        sXMLstring += string(" treeId=\"") + texteHtml((*it)->getDocum()) + string("\"");
    }
    else
    {
    	if ((*it)->getDoc() != "")
        sXMLstring += string(" objectId=\"") + texteHtml((*it)->getDoc()) + string("\"");
    }

    if ((*it)->getNodeID() != "")
        sXMLstring += string(" nodeId=\"") + texteHtml((*it)->getNodeID()) + string("\"");


    sXMLstring += string(" lexique=\"") + texteHtml((*it)->getLexique()) + string("\"");

    if ((*it)->getUnit() != "")
        sXMLstring += string(" unite=\"") + texteHtml((*it)->getUnit()) + string("\"");

    if ((*it)->getComplement() != "")
        sXMLstring += string(" complement=\"") + texteHtml((*it)->getComplement()) + string("\"");
    if ((*it)->getCertitude() != "")
        sXMLstring += string(" certitude=\"") + texteHtml((*it)->getCertitude()) + string("\"");
    if ((*it)->getInteret() != "")
        sXMLstring += string(" interet=\"") + texteHtml((*it)->getInteret()) + string("\"");
    if ((*it)->getPluriel() != "")
        sXMLstring += string(" pluriel=\"") + texteHtml((*it)->getPluriel()) + string("\"");
    if ((*it)->getVisible() != "")
        sXMLstring += string(" visible=\"") + texteHtml((*it)->getVisible()) + string("\"");
    if ((*it)->getTexteLibre() != "")
        sXMLstring += string(" freeText=\"") + texteHtml((*it)->getTexteLibre()) + string("\"");
    if ((*it)->getArchetype() != "")
        sXMLstring += string(" archetype=\"") + texteHtml((*it)->getArchetype()) + string("\"");

    sXMLstring += string(">\r\n");
    iLevel++;
	}

	while (iLevel > 0)
	{
        iLevel--;
    if (iLevel > 0)
        sXMLstring += string(2*iLevel, ' ');
    sXMLstring += string("</node>\r\n");
  }

    return sXMLstring;
}

bool
NSPatPathoArray::initialiseDepuisXML(string* pXMLstring)
{
    vider();

	if (*pXMLstring == string(""))
        return true;

    size_t iNodePos = pXMLstring->find("<node");
  if (iNodePos == NPOS)
        return false;

    int iCol = 0;

	while (iNodePos != NPOS)
	{
    size_t iEndPos = pXMLstring->find(">", iNodePos);
    if (iEndPos == NPOS)
        return false;

    string sNodeContent = string(*pXMLstring, iNodePos + 5, iEndPos - iNodePos - 5);

    if (!addElementFromXMLnode(&sNodeContent, iCol))
        return false;

    iNodePos = pXMLstring->find("<node", iEndPos);

    size_t iClosePos = pXMLstring->find("</node>", iEndPos);
    if (iClosePos > iNodePos)
        iCol++;
    else
    {
    	while (iClosePos < iNodePos)
      {
        iCol--;
        iClosePos = pXMLstring->find("</node>", iClosePos + 1);
      }
    }
	}

    return false;
}

bool
NSPatPathoArray::addElementFromXMLnode(string *pXMLnodeString, int iCol)
{
    strip(*pXMLnodeString, stripBoth);

	if (*pXMLnodeString == string(""))
        return false;

    size_t iStartPos = 0;
  size_t iNodeLenght = strlen(pXMLnodeString->c_str());

  NSPatPathoInfo pptInfo;

  string sPatId = string("");

  while (iStartPos < iNodeLenght)
  {
		// Get tag
  	//
    size_t iEqualChar = pXMLnodeString->find("=", iStartPos);
    if (iEqualChar == NPOS)
        return false;
    size_t iStartQuote = pXMLnodeString->find('"', iEqualChar);
    if (iStartQuote == NPOS)
        return false;
    size_t iEndQuote = pXMLnodeString->find('"', iStartQuote + 1);
    if (iEndQuote == NPOS)
        return false;

    string sElement = texteCourant(string(*pXMLnodeString, iStartPos, iEqualChar - iStartPos));
    string sValue   = texteCourant(string(*pXMLnodeString, iStartQuote + 1, iEqualChar - iStartQuote - 1));

    strip(sElement, stripBoth);

    if      (sElement == "patId")
        pptInfo.setPerson(sValue);
    else if (sElement == "treeId")
        pptInfo.setTreeID(sValue);
    else if (sElement == "objectId")
        pptInfo.setDocum(sValue);
    else if (sElement == "nodeId")
        pptInfo.setNodeID(sValue);
    else if (sElement == "lexique")
        pptInfo.setLexique(sValue);
    else if (sElement == "unite")
        pptInfo.setUnit(sValue);
    else if (sElement == "complement")
        pptInfo.setComplement(sValue);
    else if (sElement == "certitude")
        pptInfo.setCertitude(sValue);
    else if (sElement == "interet")
        pptInfo.setInteret(sValue);
    else if (sElement == "pluriel")
        pptInfo.setPluriel(sValue);
    else if (sElement == "visible")
        pptInfo.setVisible(sValue);
    else if (sElement == "freeText")
        pptInfo.setTexteLibre(sValue);
    else if (sElement == "archetype")
        pptInfo.setArchetype(sValue);

    iStartPos = iEndQuote + 1;
	}

    pptInfo.setColonne(iCol);

    int ligne = 0;
	if (!empty())
        ligne = (back())->getLigne() + 1;
    pptInfo.setLigne(ligne);

    push_back(new NSPatPathoInfo(pptInfo));

  return true;
}

string
NSPatPathoArray::getNodeDate(PatPathoConstIter iNode) const
{
  if (((PatPathoConstIter) NULL == iNode) || (end() == iNode))
    return string("");

    string sLexique = (*iNode)->getLexique();
	//
	// Donnée chiffrée : on regarde si le noeud n'a pas un frère
	//
	if (string(sLexique, 0, 2) == string("£N"))
	{
    int iCol = (*iNode)->getColonne();
    PatPathoConstIter iNodeNext = iNode++;
    while ((iNodeNext != end()) && ((*iNodeNext)->getColonne() == iCol))
  	{
            string sLexNext = (*iNodeNext)->getLexique();
      NSDico::donneCodeSens(sLexNext);
      if (sLexNext == "KDARE")
      {
        iNodeNext++;
        if ((end() != iNodeNext) && ((*iNodeNext)->getColonne() == iCol+1))
          return ((*iNodeNext)->getComplement());
      }
      iNodeNext++;
		}
  }
  else
  {
    string sKchirDate = getNodeSpecificDate(iNode, string("KCHIR"));
    if (string("") != sKchirDate)
      return sKchirDate;

    string sFermDate = getNodeSpecificDate(iNode, string("KFERM"));
    if (string("") != sFermDate)
      return sFermDate;
  }

  return string("");
}

string
NSPatPathoArray::getNodeSpecificDate(PatPathoConstIter iNode, string sDateTypeSens) const
{
  if (((PatPathoConstIter) NULL == iNode) || (end() == iNode))
    return string("");

  int iCol = (*iNode)->getColonne();

  PatPathoConstIter iNodeNext = iNode++;
  while ((end() != iNodeNext) && ((*iNodeNext)->getColonne() == iCol + 1))
  {
    string sLexNext = (*iNodeNext)->getLexiqueSens(_pSuper);
    if (sLexNext == sDateTypeSens)
    {
      iNodeNext++;
      if ((end() != iNodeNext) && ((*iNodeNext)->getColonne() == iCol + 2))
        return ((*iNodeNext)->getComplement());
    }
    iNodeNext++;
  }

  return string("");
}

string
NSPatPathoArray::getDirectArchetypeId(PatPathoConstIter iNode)
{
  if ((PatPathoConstIter) NULL == iNode)
    return string("");

  string sArchetype = (*iNode)->getArchetype();
  if (string("") != sArchetype)
    return sArchetype;

  PatPathoIter iterPere = ChercherPere(iNode);
  while ((PatPathoIter) NULL != iterPere)
  {
    sArchetype = (*iterPere)->getArchetype();
    if (string("") != sArchetype)
      return sArchetype;

    iterPere = ChercherPere(iterPere);
  }

  return string("");
}

string
NSPatPathoArray::isPatientData(PatPathoIter iNode)
{
  return "";
}

bool
NSPatPathoArray::isExpressionTrue(string sExpression, bool *pbExpressionIsValid)
{
  strip(sExpression, stripBoth);

  // Before operation has been analyzed, we suppose it is not valid
  //
  if (NULL != pbExpressionIsValid)
    *pbExpressionIsValid = false;

  //
  // As a convention, an empty expression is always true, but not valid
  //
  if (string("") == sExpression)
    return true;

  // If the expression is not empty but the pathpatho is empty, then return false
  //
  if (empty())
  {
    if (NULL != pbExpressionIsValid)
      *pbExpressionIsValid = true;
    return false;
  }

  // Separating operator and operand: operator + blanks + operand
  //
  size_t posEndOperator = sExpression.find(" ");
  if (string::npos == posEndOperator)
    return false;

  string sOperator = string(sExpression, 0, posEndOperator);

  if ('*' != sOperator[0])
    return false;

  size_t posStartOperand = sExpression.find_first_not_of(' ', posEndOperator);
  if (string::npos == posStartOperand)
    return false;

  string sOperand = string(sExpression, posStartOperand, strlen(sExpression.c_str()) - posStartOperand);

  // Operand must be in the form: value[unit] (with no space in between)
  //
  size_t posStartUnit = sOperand.find("[");
  if (string::npos == posStartUnit)
    return isNonNumericExpressionTrue(sOperator, sOperand, pbExpressionIsValid);

  string sValue = string(sOperand, 0, posStartUnit);

  size_t posEndUnit = sOperand.find("]", posStartUnit + 1);
  if (string::npos == posEndUnit)
    return false;

  string sUnit = string(sOperand, posStartUnit + 1, posEndUnit - posStartUnit - 1);

  return isNumericExpressionTrue(sOperator, sValue, sUnit, pbExpressionIsValid);
}

bool
NSPatPathoArray::isNumericExpressionTrue(string sOperator, string sValue, string sUnit, bool *pbExpressionIsValid)
{
  if (NULL != pbExpressionIsValid)
    *pbExpressionIsValid = false;

  if ((string("") == sOperator) || ('*' != sOperator[0]) || (string(1, '*') == sOperator) ||
      (string("") == sValue)    || (string("") == sUnit))
    return false;

  sUnit = NSDico::donneCodeSens(&sUnit);
  if (strlen(sUnit.c_str()) != BASE_SENS_LEN)
    return false;

  // We can now suppose that is valid
  //
  if (NULL != pbExpressionIsValid)
    *pbExpressionIsValid = true;

  PatPathoIter iter = getFirstNumericalValue();
  if (((PatPathoIter) NULL == iter) || (end() == iter))
    return false;

  string sFormat  = (*iter)->getLexiqueSens(_pSuper);
  string sValeur1 = (*iter)->getComplement();
  string sUnite1  = (*iter)->getUnitSens(_pSuper);

  if ((string("") == sFormat) || (sUnite1 != sUnit))
    return false;

  char cValueType = sFormat[1];

  if ('N' == cValueType)
  {
    double dValue1 = StringToDouble(sValeur1);
    double dValue2 = StringToDouble(sValue);

    if (isComparisonOperator(sOperator))
      return compare(dValue1, sOperator, dValue2);

    // string sErrorText = pContexte->getSuperviseur()->getText("referentialErrors", "invalidOperatorForExpressions");
    // sErrorText += string(" -> ") + sOper + string(" in ") + sExp;
    // pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError);
    // erreur(sErrorText.c_str(), standardError, 0);

    if (NULL != pbExpressionIsValid)
      *pbExpressionIsValid = false;
    return false;
  }

  return false;
}

bool
NSPatPathoArray::isNonNumericExpressionTrue(string sOperator, string sOperand, bool *pbExpressionIsValid)
{
  return false;
}

PatPathoIter
NSPatPathoArray::getFirstNumericalValue(char cValueType, PatPathoIter iterFrom, int iBaseCol)
{
  if (empty())
    return (PatPathoIter) 0;

  PatPathoIter iter = begin();

  if ((PatPathoIter) NULL != iterFrom)
    iter = iterFrom;

  while (end() != iter)
  {
    if ((*iter)->getColonne() >= iBaseCol)
    {
      string sLexique = (*iter)->getLexique();

      // Num data... we treat it
      //
      if ('£' == sLexique[0])
      {
        char cCurrentType = sLexique[1];

        if ((cCurrentType == cValueType) || (' ' == cValueType))
          return iter;

        iter++;
      }

      // A different concept as a son... we must skip it
      //
      // (for example it might be "max normal value" or "min normal value")
      //
      else if ('V' == sLexique[0])
      {
        int iCol = (*iter)->getColonne();
        iter++;

        while ((end() != iter) && ((*iter)->getColonne() > iCol))
          iter++;
      }
      else
        iter++;
    }
    else
      iter++;
  }

  return iter;
}

/*bool
NSPatPathoArray::getNumericalValueForUnit(string sUnit, double *pdValue, string sConcept)
{
  if ((string("") == sUnit) || (NULL == pdValue) || empty())
    return false;

  string sSemanticUnit = NSDico::donneCodeSens(&sUnit);
  if (strlen(sSemanticUnit.c_str()) != BASE_SENS_LEN)
  {
    string sErrorText = string("getNumericalValueForUnit: invalid unit ") + sUnit;
    _pSuper->trace(&sErrorText, 1, NSSuper::trError);
    return false;
  }

  PatPathoIter iter = begin();

  int iBaseCol = (*iter)->getColonne();

  string sSemConcept = NSDico::donneCodeSens(&sConcept);
  if ((*iter)->getLexiqueSens(_pSuper) == sSemConcept)
    iBaseCol++;

  std::map<string, string> aValues;

  // First strategy, find a value with the proper unit
  //
  iter = getFirstNumericalValue(' ', (PatPathoIter) 0, iBaseCol);

  if (((PatPathoIter) NULL == iter) || (end() == iter))
  {
    string sErrorText = string("getNumericalValueForUnit: no numerical value found");
    _pSuper->trace(&sErrorText, 1, NSSuper::trError);
    return false;
  }

  while (((PatPathoIter) NULL != iter) && (end() != iter))
  {
    string sExistingUnit = (*iter)->getUnitSens(_pSuper);

    if (sExistingUnit == sSemanticUnit)
    {
      *pdValue = StringToDouble((*iter)->getComplement());
      return true;
    }

    aValues[sExistingUnit] = (*iter)->getComplement();

    iter++;

    if (end() != iter)
      iter = getFirstNumericalValue(' ', iter, iBaseCol);
  }

  // Next strategy, try to convert existing values to match needed one
  //

  // Special cases: "weird conversions" like date to months
  //

  // Days, Weeks, Months, Years
  //
  if ((string("2DAT0") == sSemanticUnit) ||
      (string("2DAT1") == sSemanticUnit) ||
      (string("2DAT2") == sSemanticUnit) ||
      (string("2DAT3") == sSemanticUnit))
  {
    std::map<string, string>::iterator itValues = aValues.begin();
    while (aValues.end() != itValues)
    {
      // Date AAAAMMJJ or AAAAMMJJHHmmss
      //
      if ((string("2DA01") == (*itValues).first) ||
          (string("2DA02") == (*itValues).first))
      {
        NVLdVTemps tpsValue;
        if (tpsValue.initFromDate((*itValues).second))
        {
          NVLdVTemps tpsNow;
          tpsNow.takeTime();
          if      (string("2DAT0") == sSemanticUnit)
            *pdValue = (double) tpsNow.daysBetween(tpsValue);
          else if (string("2DAT1") == sSemanticUnit)
            *pdValue = (double) tpsNow.daysBetween(tpsValue) / 7;
          else if (string("2DAT2") == sSemanticUnit)
            *pdValue = (double) tpsNow.monthsBetween(tpsValue);
          else if (string("2DAT3") == sSemanticUnit)
            *pdValue = (double) tpsNow.yearsBetween(tpsValue);
          return true;
        }
      }
      itValues++;
    }
  }

  // Usual stuff: use Convert
  //
  NSCV NsCv(_pSuper);

#ifndef _ENTERPRISE_DLL
  DBIResult Resultat = NsCv.open();
  if (DBIERR_NONE != Resultat)
  {
    erreur("Erreur à l'ouverture de la base convert.", standardError, Resultat);
    return false;
  }
#endif

  std::map<string, string>::iterator itValues = aValues.begin();

  while (aValues.end() != itValues)
  {
    double dVal = StringToDouble((*itValues).second);
    if (NsCv.ConvertirUnite(&dVal, sUnit, (*itValues).first, sConcept))
    {
      *pdValue = dVal;
#ifndef _ENTERPRISE_DLL
      NsCv.close();
#endif
      return true;
    }

    itValues++;
  }

#ifndef _ENTERPRISE_DLL
  NsCv.close();
#endif

  return false;
}*/

//
//

NSFatheredPatPathoArray::NSFatheredPatPathoArray(NSSuper* pSuper, NSPatPathoInfo* pFatherNode, NSPatPathoArray* pPatPatho)
                        :NSSuperRoot(pSuper)
{
  _pPatPatho   = pPatPatho;
  _pFatherNode = pFatherNode;

  lObjectCount++;
}

NSFatheredPatPathoArray::NSFatheredPatPathoArray(const NSFatheredPatPathoArray& rv)
                        :NSSuperRoot(rv._pSuper)
{
  if (NULL == rv._pPatPatho)
    _pPatPatho = (NSPatPathoArray*) 0;
  else
    _pPatPatho = new NSPatPathoArray(*(rv._pPatPatho));

  if (NULL == rv._pFatherNode)
    _pFatherNode = (NSPatPathoInfo*) 0;
  else
    _pFatherNode = new NSPatPathoInfo(*(rv._pFatherNode));

  lObjectCount++;
}

NSFatheredPatPathoArray::~NSFatheredPatPathoArray()
{
  if (_pPatPatho)
    delete _pPatPatho;
  if (_pFatherNode)
    delete _pFatherNode;

  lObjectCount--;
}

void
NSFatheredPatPathoArray::referenceFatherNode(NSPatPathoInfo* pModel)
{
  if (NULL == pModel)
  {
    if (_pFatherNode)
    {
      delete _pFatherNode;
      _pFatherNode = (NSPatPathoInfo*) 0;
    }
  }

  if (_pFatherNode)
    *_pFatherNode = *pModel;
  else
    _pFatherNode = new NSPatPathoInfo(*pModel);
}

NSFatheredPatPathoArray&
NSFatheredPatPathoArray::operator=(const NSFatheredPatPathoArray& src)
{
  if (&src == this)
        return *this;

  if (_pPatPatho)
  {
    delete _pPatPatho;
    _pPatPatho = (NSPatPathoArray*) 0;
  }
  if (_pFatherNode)
  {
    delete _pFatherNode;
    _pFatherNode = (NSPatPathoInfo*) 0;
  }

  if (src._pPatPatho)
    _pPatPatho = new NSPatPathoArray(*(src._pPatPatho));

  if (src._pFatherNode)
    _pFatherNode = new NSPatPathoInfo(*(src._pFatherNode));

  return *this;
}

// -------------------------------------------------------------------------
// ---------------- METHODES DE NSVectFatheredPatPathoArray ----------------
// -------------------------------------------------------------------------
NSVectFatheredPatPathoArray::NSVectFatheredPatPathoArray()
                            :NSVectFatheredPatPathoInfoArray()
{
    lObjectCount++;
}

//---------------------------------------------------------------------------
//  Constructeur copie
//---------------------------------------------------------------------------
NSVectFatheredPatPathoArray::NSVectFatheredPatPathoArray(const NSVectFatheredPatPathoArray& rv)
                            :NSVectFatheredPatPathoInfoArray()
{
try
{
	if (false == rv.empty())
        for (ConstFatheredPatPathoIterVect i = rv.begin(); rv.end() != i; i++)
            push_back(new NSFatheredPatPathoArray(*(*i)));

  lObjectCount++;
}
catch (...)
{
    erreur("Exception (NSVectFatheredPatPathoArray copy ctor).", standardError);
}
}

//---------------------------------------------------------------------------
//  Destructeur
//---------------------------------------------------------------------------
void
NSVectFatheredPatPathoArray::vider()
{
	if (empty())
        return;
    for (FatheredPatPathoIterVect i = begin(); end() != i; )
	{
        delete *i;
        erase(i);
	}
}

NSVectFatheredPatPathoArray::~NSVectFatheredPatPathoArray()
{
    vider();

  lObjectCount--;
}

//---------------------------------------------------------------------------
//  Opérateur d'affectation
//---------------------------------------------------------------------------
NSVectFatheredPatPathoArray&
NSVectFatheredPatPathoArray::operator=(const NSVectFatheredPatPathoArray& src)
{
try
{
	if (&src == this)
        return *this;

	//
	// Effacement des éléments déjà contenus dans le vecteur destination
	//
    vider();
	//
	// Copie et insertion des éléments de la source
	//
	if (false == src.empty())
        for (ConstFatheredPatPathoIterVect i = src.begin(); src.end() != i; i++)
            push_back(new NSFatheredPatPathoArray(*(*i)));

    return *this;
}
catch (...)
{
    erreur("Exception (op = NSVectFatheredPatPathoArray).", standardError);
    return *this;
}
}

//---------------------------------------------------------------------------
//   NSVectPatPathoArray vide ou non dans le cas non multidialogue
//---------------------------------------------------------------------------
bool
NSVectFatheredPatPathoArray::estVide()
{
	if (empty())
        return true;

    FatheredPatPathoIterVect iter;
    for (iter = begin(); (end() != iter) && ((*iter)->getPatPatho()->empty()); iter++);
	if (end() == iter)
        return true;

    return false;
}

//---------------------------------------------------------------------------
//   NSVectPatPathoArray vide ou non dans le cas multidialogue
//---------------------------------------------------------------------------
bool
NSVectFatheredPatPathoArray::MultiEstVide()
{
	if (empty())
        return true;

	FatheredPatPathoIterVect iter;
    for (iter = begin(); (end() != iter) && ((*iter)->getPatPatho()->empty()); iter++);
	if (end() == iter)
        return true;

    return false;
}

//***************************************************************************
// Implémentation des mèthodes NSPatPathoData
//***************************************************************************

//---------------------------------------------------------------------------
//  Constructeur
//---------------------------------------------------------------------------
NSPatPathoData::NSPatPathoData() : NSPathoBaseData()
{
    metAZero();

  lObjectCount++;
}

//---------------------------------------------------------------------------
//  Destructeur
//---------------------------------------------------------------------------
NSPatPathoData::~NSPatPathoData()
{
  lObjectCount--;
}

//---------------------------------------------------------------------------
//  Constructeur copie
//---------------------------------------------------------------------------
NSPatPathoData::NSPatPathoData(const NSPatPathoData& rv)
               :NSPathoBaseData(rv)
{
	// metAZero(); Don't since the NSPathoBaseData part is already set

  // memset(treeID, 0, PAT_NSS_LEN + DOC_CODE_DOCUM_LEN + 1);
  // memset(noeud,  0, PPD_NOEUD_LEN + 1);

  _sTreeId     = string("");
  _sNode       = string("");

  _sPatientId  = string("");
  _sDocumentId = string("");

  _sTexteLibre = string("");
  _sArchetype  = string("");
  _sNodeRight  = string("");

	//strcpy(codePatient,  rv.codePatient);
	//strcpy(codeDocument, rv.codeDocument);
	// strcpy(treeID,  rv.treeID);
	// strcpy(noeud,   rv.noeud);

  _sTreeId = rv._sTreeId;
  _sNode   = rv._sNode;

  parseTreeId();

  if (string("") != rv._sTexteLibre)
    _sTexteLibre = rv._sTexteLibre;

  if (string("") != rv._sArchetype)
    _sArchetype = rv._sArchetype;

  if (string("") != rv._sNodeRight)
    _sNodeRight = rv._sNodeRight;

/*
	if (&(rv.sArchetype) != NULL)
        sArchetype = rv.sArchetype;
	else
        sArchetype = string("");
    _NodeRight  = rv._NodeRight;
*/

  lObjectCount++;
}

//---------------------------------------------------------------------------
//  Opérateur =
//---------------------------------------------------------------------------
NSPatPathoData&
NSPatPathoData::operator=(const NSPatPathoData& src)
{
	if (&src == this)
        return *this;

    metAZero();

    NSPathoBaseData* pbdata1 = this;
    const NSPathoBaseData* pbdata2 = &src;

    *pbdata1 = *pbdata2;

	//strcpy(codePatient,  src.codePatient);
	//strcpy(codeDocument, src.codeDocument);
    // strcpy(treeID, src.treeID);
    // strcpy(noeud,  src.noeud);

  _sPatientId  = string("");
  _sDocumentId = string("");

  _sTreeId = src._sTreeId;
  _sNode   = src._sNode;

  parseTreeId();

  if (string("") != src._sTexteLibre)
    _sTexteLibre = src._sTexteLibre;
  if (string("") != src._sArchetype)
    _sArchetype  = src._sArchetype;
  if (string("") != src._sNodeRight)
    _sNodeRight  = src._sNodeRight;

    return *this;
}

//---------------------------------------------------------------------------
//  Opérateur de comparaison
//---------------------------------------------------------------------------
int
NSPatPathoData::operator == (const NSPatPathoData& o )
{
  NSPathoBaseData* pbdata1 = this;
  const NSPathoBaseData* pbdata2 = &o;

  if ((*pbdata1 == *pbdata2) &&
    	//(strcmp(codePatient, 	  o.codePatient) 	  == 0) &&
        //(strcmp(codeDocument,   o.codeDocument) 	  == 0) &&
        (_sTreeId     == o._sTreeId) &&
        (_sNode       == o._sNode) &&
        (_sTexteLibre == o._sTexteLibre) &&
        (_sArchetype  == o._sArchetype))
    return 1;
  else
    return 0;
}

//---------------------------------------------------------------------------
//  Met à 0 les variables.
//---------------------------------------------------------------------------
void
NSPatPathoData::metAZero()
{
	//
	// Met les champs de données à zéro
	//
  NSPathoBaseData::metAZero();

	//memset(codePatient,	 0, PAT_NSS_LEN + 1);
  //memset(codeDocument, 0, DOC_CODE_DOCUM_LEN + 1);
  // memset(treeID, 		 0, PAT_NSS_LEN + DOC_CODE_DOCUM_LEN + 1);
  // memset(noeud, 		 0, PPD_NOEUD_LEN + 1);

  _sTreeId     = string("");
  _sNode       = string("");

  _sPatientId  = string("");
  _sDocumentId = string("");

  _sTexteLibre = string("");
  _sArchetype  = string("");
  _sNodeRight  = string("");
}

void
NSPatPathoData::parseTreeId()
{
  if (string("") == _sTreeId)
    return;

  size_t iTreeIdLen = strlen(_sTreeId.c_str());

  if (iTreeIdLen < PAT_NSS_LEN)
    return;

  _sPatientId = string(_sTreeId, 0, PAT_NSS_LEN);

  if (iTreeIdLen < PAT_NSS_LEN + DOC_CODE_DOCUM_LEN)
    return;

  _sDocumentId = string(_sTreeId, PAT_NSS_LEN, DOC_CODE_DOCUM_LEN);
}

void
NSPatPathoData::buildTreeId()
{
  if ((string("") == _sPatientId) || (string("") == _sDocumentId))
  {
    _sTreeId = string("");
    return;
  }

  _sTreeId = _sPatientId + _sDocumentId;
}

//---------------------------------------------------------------------------
//  Teste l'égalité du point de vue des données
//---------------------------------------------------------------------------

bool
NSPatPathoData::sameContent(NSPatPathoData* pPathoData)
{
	if (NULL == pPathoData)
        return false;

    NSPathoBaseData* pbdata1 = this;
    NSPathoBaseData* pbdata2 = pPathoData;

	// on ne tient pas compte du numéro de noeud
  //
	if ((pbdata1->sameContent(*pbdata2)) &&
      (_sTexteLibre == pPathoData->_sTexteLibre) &&
      (_sArchetype  == pPathoData->_sArchetype))
        return true;
	else
        return false;
}

bool
NSPatPathoData::estEgal(NSPatPathoData* pPathoData)
{
	if (NULL == pPathoData)
        return false;

    NSPathoBaseData* pbdata1 = this;
    NSPathoBaseData* pbdata2 = pPathoData;

	// on ne tient pas compte du numéro de noeud
  //
	if ((*pbdata1 == *pbdata2) &&
    	//(strcmp(codePatient, 	  pPathoData->codePatient) 	  == 0) &&
      //(strcmp(codeDocument, 	  pPathoData->codeDocument) 	  == 0) &&
      //(strcmp(treeID, pPathoData->treeID) == 0) &&
      (_sTexteLibre == pPathoData->_sTexteLibre) &&
      (_sArchetype  == pPathoData->_sArchetype))
        return true;
	else
        return false;
}

bool
NSPatPathoData::estMemeNode(NSPatPathoData* pPathoData)
{
  NSPathoBaseData* pbdata1 = this;
  NSPathoBaseData* pbdata2 = pPathoData;

  // on tient compte ici du numéro de noeud
  // mais pas des données archetype et droits
  if (/* (*pbdata1 == *pbdata2) */ (pbdata1->sameContent(*pbdata2)) &&
        (_sTreeId     == pPathoData->_sTreeId) &&
        (_sNode       == pPathoData->_sNode) &&
        (_sTexteLibre == pPathoData->_sTexteLibre))
    return true;
  else
        return false;
}

void
NSPatPathoData::getOtherNodeContent(NSPatPathoData* pPathoData)
{
	if ((NULL == pPathoData) || (pPathoData == this))
        return;

	// Don't overwrite the localisation
  // char initialCodeLocalisation[BASE_LOCALISATION_LEN + 1];
  // strcpy(initialCodeLocalisation, codeLocalisation);

  string sInitialCodeLoc = _sCodeLocalisation;

    NSPathoBaseData* pbdata1 = this;
    NSPathoBaseData* pbdata2 = pPathoData;

    *pbdata1 = *pbdata2;

  // strcpy(codeLocalisation, initialCodeLocalisation);

  _sCodeLocalisation = sInitialCodeLoc;

    _sTexteLibre = pPathoData->_sTexteLibre;
    _sArchetype  = pPathoData->_sArchetype;
    _sNodeRight  = pPathoData->_sNodeRight;
}

//---------------------------------------------------------------------------
//  Va chercher le libellé dans le fichier Patholog.
//---------------------------------------------------------------------------
/*
DBIResult
NSPatPathoData::donneLibelle(string* pLibel, NSPatholog* pPathor)
{
  DBIResult ErrDBI = pPathor->chercheClef((unsigned char*)pLibel,
                                            "CODE_INDEX",
                                            NODEFAULTINDEX,
                                            keySEARCHEQ,
                                            dbiWRITELOCK);
  if (ErrDBI == DBIERR_NONE)
    *pLibel = pPathor->pDonnees->libelleLong;
  else
    *pLibel = "Erreur";

  return ErrDBI;
}
*/

//---------------------------------------------------------------------------
//  Retourne le type de l'élément : pptLex, pptNum, pptLibre
//---------------------------------------------------------------------------
int
NSPatPathoData::getType()
{
  if (string("") == _sType)
    return pptIndef;

  switch (_sType[0])
  {
    case 'C' : return pptLex;
        case 'N' : return pptNum;
    case 'L' : return pptLibre;
  }
  
  return pptIndef;
}

//---------------------------------------------------------------------------
//  Impose le type de l'élément : pptLex, pptNum, pptLibre
//---------------------------------------------------------------------------
void
NSPatPathoData::setType(int newType)
{
  switch (newType)
  {
    case pptLex   : _sType = string("C"); break;
        case pptNum   : _sType = string("N"); break;
    case pptLibre : _sType = string("L"); break;
  }
}

void
NSPatPathoData::setPerson(string sPat)
{
  if (string("") == sPat)
  {
    _sTreeId    = string("");
    _sPatientId = string("");
    return;
  }

  if (strlen(sPat.c_str()) != PAT_NSS_LEN)
    return;

  _sPatientId = sPat;

  buildTreeId();
}

void
NSPatPathoData::setDocum(string sDocum)
{
  if (string("") == sDocum)
  {
    _sTreeId     = string("");
    _sDocumentId = string("");
    return;
  }
  if (strlen(sDocum.c_str()) != DOC_CODE_DOCUM_LEN)
    return;

  _sDocumentId = sDocum;

  buildTreeId();
}

void
NSPatPathoData::setTreeID(string sTreeID)
{
  if (string("") == sTreeID)
  {
    _sTreeId = string("");
    return;
  }

  if (strlen(sTreeID.c_str()) < OBJECT_ID_LEN)
    return;

  _sTreeId = sTreeID;
}

void
NSPatPathoData::setNodeID(string sNodeID)
{
  if (string("") == sNodeID)
  {
    _sNode = string("");
    return;
  }

  if (strlen(sNodeID.c_str()) < PPD_NOEUD_LEN)
    return;

  _sNode = sNodeID;
}

//***************************************************************************
//          Implémentation des méthodes NSPatPathoInfo
//***************************************************************************

//---------------------------------------------------------------------------
//  Constructeur par défaut
//---------------------------------------------------------------------------
NSPatPathoInfo::NSPatPathoInfo()
{
try
{
    _ID              = -1;
    _pTemporaryLinks = (NSLinkedNodeArray*) 0;

  setLocalisation(string(BASE_LOCALISATION_LEN, '0'));

  lObjectCount++;
} // try
catch (...)
{
    erreur("Exception NSPatPathoInfo ctor.", standardError);
}
}

//---------------------------------------------------------------------------
//  Constructeur à partir d'un NSSavFiche
//---------------------------------------------------------------------------
#ifndef __linux__
NSPatPathoInfo::NSPatPathoInfo(NSSavFiche* pSavFiche)
{
try
{
    _ID              = -1;
    _pTemporaryLinks = (NSLinkedNodeArray*) 0;

	//
	// Copie les valeurs du NSDocument
	//
  if (pSavFiche)
  {
      setLocalisation(pSavFiche->getLocalisation());
      setType(pSavFiche->getType());
      setLexique(pSavFiche->getLexique());
      setComplement(pSavFiche->getComplement());
      setUnit(pSavFiche->getUnit());
      setCertitude(pSavFiche->getCertitude());
      setInteret(pSavFiche->getInteret());
      setPluriel(pSavFiche->getPluriel());
      setVisible(pSavFiche->getVisible());
  }

  lObjectCount++;
} // try
catch (...)
{
    erreur("Exception NSPatPathoInfo ctor(NSSavFiche*).", standardError);
}
}
#endif // #ifndef __linux__

//---------------------------------------------------------------------------
//  Constructeur copie
//---------------------------------------------------------------------------
NSPatPathoInfo::NSPatPathoInfo(const NSPatPathoInfo& rv)
               :_Donnees(rv._Donnees)
{
try
{
  // Make certain that this is initialized in case the exception is thrown
  //
  _pTemporaryLinks = (NSLinkedNodeArray*) 0;

	//
	// Copie les valeurs du NSPatPathoInfo d'origine
	//
    _ID = rv._ID;
	if (rv._pTemporaryLinks)
        _pTemporaryLinks = new NSLinkedNodeArray(*(rv._pTemporaryLinks));

    lObjectCount++;
} // try
catch (...)
{
    erreur("Exception NSPatPathoInfo copy ctor.", standardError);
}
}

//---------------------------------------------------------------------------
//  Opérateur d'affectation
//---------------------------------------------------------------------------
NSPatPathoInfo&
NSPatPathoInfo::operator=(const NSPatPathoInfo& src)
{
	if (&src == this)
        return *this;

  _Donnees = src._Donnees;

    _ID = src._ID;

  if (_pTemporaryLinks)
    delete _pTemporaryLinks;

	if (src._pTemporaryLinks)
    _pTemporaryLinks = new NSLinkedNodeArray(*(src._pTemporaryLinks));
	else
    _pTemporaryLinks = (NSLinkedNodeArray*) 0;

    return *this;
}

void
NSPatPathoInfo::initFromMessage(Message *pMsg)
{
  if (NULL == pMsg)
    return;

  string sTreeId = pMsg->GetTreeID();
  if (strlen(sTreeId.c_str()) == OBJECT_ID_LEN)
    setTreeID(sTreeId);

  string sNodeId = pMsg->GetNoeud();
  if (strlen(sNodeId.c_str()) == PPD_NOEUD_LEN)
    setNodeID(sNodeId);

  setLexique   (pMsg->GetLexique());
  setComplement(pMsg->GetComplement());
  setUnit      (pMsg->GetUnit());
  setCertitude (pMsg->GetCertitude());
  setInteret   (pMsg->GetInteret());
  setPluriel   (pMsg->GetPluriel());
  setVisible   (pMsg->GetVisible());
  setVisible   (pMsg->GetVisible());
  setNodeRight (pMsg->GetNodeRight());

  if (pMsg->GetTemporaryLinks())
  {
    if (_pTemporaryLinks)
      *_pTemporaryLinks = *(pMsg->GetTemporaryLinks());
    else
      _pTemporaryLinks = new NSLinkedNodeArray(*(pMsg->GetTemporaryLinks()));
  }

  setTexteLibre(pMsg->GetTexteLibre());
  setArchetype (pMsg->GetArchetype());
}

//---------------------------------------------------------------------------
//  Opérateur de comparaison
//---------------------------------------------------------------------------
int
NSPatPathoInfo::operator == ( const NSPatPathoInfo& o )
{
	// ID est devenu obsolete
	// return ((*pDonnees == *(o.pDonnees)) && (ID == o.ID));

    return (_Donnees == o._Donnees);
}

string
NSPatPathoInfo::getNodeLabel()
{
  string sEtiquette = getLexique();
  string sUnit      = getUnit();
  string sPluriel   = getPluriel();
  string sCertitude = getCertitude();

  if (string("") != sUnit)
    sEtiquette = sUnit + string(1, intranodeSeparationMARK) + sEtiquette;

  if (string("") != sPluriel)
    sEtiquette += string(1, intranodeSeparationMARK) + sPluriel;
  if (string("") != sCertitude)
    sEtiquette += string(1, intranodeSeparationMARK) + sCertitude;

  return sEtiquette;
}

//---------------------------------------------------------------------------
//  Opérateur de comparaison du point de vue des données
//---------------------------------------------------------------------------
bool
NSPatPathoInfo::sameContent(NSPatPathoInfo* pPathoInfo)
{
  if (NULL == pPathoInfo)
    return false;

    return _Donnees.sameContent(&(pPathoInfo->_Donnees));
}

bool
NSPatPathoInfo::estEgal(NSPatPathoInfo* pPathoInfo)
{
  if (NULL == pPathoInfo)
    return false;

    return _Donnees.estEgal(&(pPathoInfo->_Donnees));
}

bool
NSPatPathoInfo::estMemeNode(NSPatPathoInfo* pPathoInfo)
{
  if (NULL == pPathoInfo)
    return false;

    return _Donnees.estMemeNode(&(pPathoInfo->_Donnees));
}

void
NSPatPathoInfo::getOtherNodeContent(NSPatPathoInfo* pPathoInfo)
{
	if ((NULL == pPathoInfo) || (this == pPathoInfo))
        return;

    _Donnees.getOtherNodeContent(&(pPathoInfo->_Donnees));
}

// -----------------------------------------------------------------------------
// destructeur
// -----------------------------------------------------------------------------
NSPatPathoInfo::~NSPatPathoInfo()
{
  if (_pTemporaryLinks)
    delete _pTemporaryLinks;

    lObjectCount--;
}

string
NSPatPathoInfo::getLexiqueSens(NSSuper* /* pSuper */)
{
    string sCode = getLexique();
  return NSDico::donneCodeSens(&sCode);
}

string
NSPatPathoInfo::getUnitSens(NSSuper* /* pSuper */) const
{
    string sCode = getUnit();
  return NSDico::donneCodeSens(&sCode);
}

string
NSPatPathoInfo::getCertitudeSens(NSSuper* /* pSuper */) const
{
    string sCode = getCertitude();
  return NSDico::donneCodeSens(&sCode);
}

string
NSPatPathoInfo::getInteretSens(NSSuper* /* pSuper */) const
{
    string sCode = getInteret();
  return NSDico::donneCodeSens(&sCode);
}

string
NSPatPathoInfo::getPlurielSens(NSSuper* /* pSuper */) const
{
    string sCode = getPluriel();
  return NSDico::donneCodeSens(&sCode);
}

void
NSPatPathoInfo::addTemporaryLink(string sNode, NSRootLink::NODELINKTYPES iLkType,
                                 NODELINKDIRECTION iLkDirection,
                                 bool bLkWithDoc)
{
try
{
	if (NULL == _pTemporaryLinks)
        _pTemporaryLinks = new NSLinkedNodeArray();

    _pTemporaryLinks->push_back(new NSLinkedNode(sNode, iLkType, iLkDirection, bLkWithDoc));
}
catch (...)
{
    erreur("Exception NSPatPathoInfo::addTemporaryLink", standardError);
}
}

void
NSPatPathoInfo::addTemporaryLink(NSPatPathoInfo* pOtherTemporaryNode,
                                 NSRootLink::NODELINKTYPES iLkType,
                                 NODELINKDIRECTION iLkDirection,
                                 bool bLkWithDoc)
{
try
{
	if (NULL == _pTemporaryLinks)
        _pTemporaryLinks = new NSLinkedNodeArray();

    _pTemporaryLinks->push_back(new NSLinkedNode(pOtherTemporaryNode, iLkType, iLkDirection, bLkWithDoc));
}
catch (...)
{
    erreur("Exception NSPatPathoInfo::addTemporaryLink", standardError);
}
}

bool
NSPatPathoInfo::numberTemporaryNodes(NSLinkManager* pLinkManager)
{
	if (NULL == _pTemporaryLinks)
        return false;

	if (_pTemporaryLinks->empty())
        return true;

	if (getNodeID() == string(""))
	{
    erreur("Un noeud à relier ne possède pas d'identifiant.", standardError);
    return false;
	}

    for (NSLinkedNodeIter i = _pTemporaryLinks->begin(); _pTemporaryLinks->end() != i; i++)
	{
    string sNodeSource;
  	if ((*i)->bLinkWithDoc)
        sNodeSource = getDoc();
    else
        sNodeSource = getNode();

    // dans le cas où sOtherNode est instancié, on établit un lien si
    // on a un pLinkManager en paramètre != NULL
    if (string("") == (*i)->sOtherNode)
    {
    	if (NULL == (*i)->pOtherTemporaryNode)
        erreur("Un lien en attente ne possède pas d'adresse.", standardError);
      else
      {
        NSPatPathoInfo* pOtherInfo = (*i)->pOtherTemporaryNode;
        bool bNotPresent = true;

        if (pOtherInfo->_pTemporaryLinks && (false == pOtherInfo->_pTemporaryLinks->empty()))
        {
            for (NSLinkedNodeIter j = pOtherInfo->_pTemporaryLinks->begin();
                                 pOtherInfo->_pTemporaryLinks->end() != j; j++)
          {
          	if ((*j)->pOtherTemporaryNode == this)
            {
                (*j)->sOtherNode = sNodeSource;
              (*j)->pOtherTemporaryNode = NULL;
              bNotPresent = false;
            }
          }
        }
        else
        {
        	if ((*i)->iLinkDirection == dirFleche)
            pOtherInfo->addTemporaryLink(sNodeSource, (*i)->iLinkType, dirEnvers);
          else
            pOtherInfo->addTemporaryLink(sNodeSource, (*i)->iLinkType, dirFleche);

          bNotPresent = false;
        }

        if (bNotPresent)
            erreur("Un noeud à relier n'est pas présent dans le noeud destination.", standardError);
      }
    }
#ifndef __linux__
    else if (pLinkManager)
    {
        bool bLinkResult;
      if ((*i)->iLinkDirection == dirFleche)
        bLinkResult = pLinkManager->etablirLien(sNodeSource, (*i)->iLinkType, (*i)->sOtherNode);
      else
        bLinkResult = pLinkManager->etablirLien((*i)->sOtherNode, (*i)->iLinkType, sNodeSource);

      if (false == bLinkResult)
        erreur("Impossible d'initialiser le lien en attente.", standardError);
    }
#endif
  }

    return true;
}

//
// -------------------- METHODES DE NSCutPastTLArray ----------------------
//
 /*
NSCutPastTLArray::NSCutPastTLArray()
				 :NSCutPastTLIBArray()
{}

NSCutPastTLArray::NSCutPastTLArray(NSCutPastTLArray& rv)
				 :NSCutPastTLIBArray()
{
try
{
	if (!(rv.empty()))
		for (CutPastTLIter i = rv.begin(); i != rv.end(); i++)
   	    	push_back(new NSCutPastTL(*(*i)));
} // try
catch (...)
{
    erreur("Exception NSCutPastTLArray copy ctor.", standardError, 0);
}
}

void
NSCutPastTLArray::vider()
{
	if (empty())
    	return;

	for (CutPastTLIter i = begin(); i != end(); )
    {
   	    delete *i;
        erase(i);
    }
}

NSCutPastTLArray::~NSCutPastTLArray()
{
    vider();
}

NSCutPastTLArray&
NSCutPastTLArray::operator=(NSCutPastTLArray src)
{
    // Effacement des éléments déjà contenus dans le vecteur destination
	vider();
	//
    // Copie et insertion des éléments de la source
    //
try
{
	if (!(src.empty()))
  		for (CutPastTLIter i = src.begin(); i != src.end(); i++)
    		push_back(new NSCutPastTL(*(*i)));
} // try
catch (...)
{
    erreur("Exception NSCutPastTLArray = op.", standardError, 0);
}
    return *this;
}
 */
//
// -------------------- METHODES DE NSCutPaste ----------------------
//

NSCutPaste::NSCutPaste(NSSuper* pSuper)
           :NSSuperRoot(pSuper), _PatPatho(pSuper)
{
}

NSCutPaste::NSCutPaste(const NSCutPaste& rv)
           :NSSuperRoot(rv._pSuper), _PatPatho(rv._PatPatho)
{
try
{
    _TextesLibres = rv._TextesLibres;
    _aConcerns    = rv._aConcerns;
} // try
catch (...)
{
    erreur("Exception NSCutPaste copy ctor.", standardError);
}
}

NSCutPaste::~NSCutPaste()
{
}

void
NSCutPaste::vider()
{
  _PatPatho.vider();
  _TextesLibres.vider();
  _aConcerns.vider();
}

NSCutPaste&
NSCutPaste::operator=(const NSCutPaste& src)
{
	if (&src == this)
        return *this;

    _PatPatho.vider();
    _TextesLibres.vider();

    _PatPatho     = src._PatPatho;
    _TextesLibres = src._TextesLibres;
    _aConcerns    = src._aConcerns;

    return *this;
}

void
NSCutPaste::setPatPatho(const NSPatPathoArray* pPpt)
{
  if (NULL == pPpt)
    return;

  _PatPatho = *pPpt;
}

void
NSCutPaste::initFromPatPatho(NSPatPathoArray* pPpt) const
{
  if (NULL == pPpt)
    return;

  *pPpt = _PatPatho;
}

//
// -------------------- METHODES DE NSLinkedNode ----------------------
//

NSLinkedNode::NSLinkedNode(string sNode, NSRootLink::NODELINKTYPES iLkType,
                           NODELINKDIRECTION iLkDirection, bool bLkWithDoc)
{
  sOtherNode          = sNode;
  pOtherTemporaryNode = 0;
  iLinkType           = iLkType;
  iLinkDirection      = iLkDirection;
  bLinkWithDoc        = bLkWithDoc;
}

NSLinkedNode::NSLinkedNode(NSPatPathoInfo* pTemporaryNode,
                           NSRootLink::NODELINKTYPES iLkType,
                           NODELINKDIRECTION iLkDirection, bool bLkWithDoc)
{
  sOtherNode          = "";
  pOtherTemporaryNode = pTemporaryNode;
  iLinkType           = iLkType;
  iLinkDirection      = iLkDirection;
  bLinkWithDoc        = bLkWithDoc;
}

NSLinkedNode::~NSLinkedNode()
{
}

NSLinkedNode::NSLinkedNode(NSLinkedNode& rv)
{
  sOtherNode          = rv.sOtherNode;
  pOtherTemporaryNode = rv.pOtherTemporaryNode;
  iLinkType           = rv.iLinkType;
  iLinkDirection      = rv.iLinkDirection;
  bLinkWithDoc        = rv.bLinkWithDoc;
}

NSLinkedNode&
NSLinkedNode::operator=(const NSLinkedNode& src)
{
	if (&src == this)
        return *this;

    sOtherNode          = src.sOtherNode;
    pOtherTemporaryNode = src.pOtherTemporaryNode;
    iLinkType           = src.iLinkType;
    iLinkDirection      = src.iLinkDirection;
    bLinkWithDoc        = src.bLinkWithDoc;

    return *this;
}

//
// -------------------- METHODES DE NSLinkedNodeArray ----------------------
//

NSLinkedNodeArray::NSLinkedNodeArray(NSLinkedNodeArray& rv)
                  :NSLinkedNodeVector()
{
try
{
	if (!(rv.empty()))
		for (NSLinkedNodeIter i = rv.begin(); i != rv.end(); i++)
   	    	push_back(new NSLinkedNode(*(*i)));
} // try
catch (...)
{
    erreur("Exception NSLinkedNodeArray copy ctor.", standardError);
}
}

void
NSLinkedNodeArray::vider()
{
	if (empty())
        return;

	for (NSLinkedNodeIter i = begin(); i != end(); )
	{
    delete *i;
    erase(i);
	}
}

NSLinkedNodeArray::~NSLinkedNodeArray()
{
    vider();
}

NSLinkedNodeArray&
NSLinkedNodeArray::operator=(const NSLinkedNodeArray& src)
{
	if (&src == this)
        return *this;

	// Effacement des éléments déjà contenus dans le vecteur destination
    vider();
	//
	// Copie et insertion des éléments de la source
	//
try
{
	if (false == src.empty())
        for (ConstNSLinkedNodeIter i = src.begin(); src.end() != i; i++)
            push_back(new NSLinkedNode(*(*i)));
} // try
catch (...)
{
    erreur("Exception NSLinkedNodeArray = op.", standardError);
}
    return *this;
}


