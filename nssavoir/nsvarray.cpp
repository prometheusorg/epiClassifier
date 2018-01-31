#include <string>
#include "nssavoir/nsguide.h"

// pour NPOS
#ifdef _ENTERPRISE_DLL
#include "enterpriseLus/nsglobalLus.h"
#else
#include "partage/nsglobal.h"
#endif

#include "nssavoir/nsvarray.h"

//***************************************************************************
// 							Implémentation des mèthodes VecteurString
//***************************************************************************

VecteurString::VecteurString()
{}

VecteurString::VecteurString(char* iniTab[])
{
    int size = sizeof(iniTab);
    for (int i = 0; i < size; i++)
        push_back(new std::string(iniTab[i]));
}

VecteurString::~VecteurString()
{
    vider();
}

void
VecteurString::vider()
{
    if (empty())
        return;

    EquiItemIter i = begin();
    while (i != end())
    {
        delete *i;
        erase(i);
    }
}

VecteurString&
VecteurString::operator=(const VecteurString& src)
{
    if (&src == this)
        return *this;

    vider();

    if (false == src.empty())
        for (ConstEquiItemIter i = src.begin(); i != src.end(); i++)
            push_back(new string(*(*i)));

    return *this;
}

VecteurString::VecteurString(VecteurString& src)
{
    if (!(src.empty()))
        for (EquiItemIter i = src.begin(); i != src.end(); i++)
            push_back(new string(*(*i)));
}

//---------------------------------------------------------------------------
// VERIFIER SI UN ITEM APPARTIENT A UN VECTEUR
// true si sItem se trouve dans pVect, false sinon
//---------------------------------------------------------------------------
bool
VecteurString::ItemDansUnVecteur(string sItem)
{
    if (empty())
        return false;

    for (EquiItemIter i = begin(); i != end(); i++)
        if ((*(*i)) == (sItem))
            return true;

    return false;
}

//---------------------------------------------------------------------------
// return l'élément strictement supérieur à sItem
//---------------------------------------------------------------------------
string
VecteurString::ItemStrictementSuperieur(string sItem)
{
    if (empty())
        return string("");

    for (EquiItemIter i = begin(); i != end(); i++)
        if (sItem < (*(*i)))
            return (*(*i));

    return string("");
}

//---------------------------------------------------------------------------
// return l'élément strictement inférieur à sItem
//---------------------------------------------------------------------------
string
VecteurString::ItemStrictementInferieur(string sItem)
{
    if (empty())
        return string("");

    string sup = string("");
    for (EquiItemIter i = begin(); i != end(); i++)
    {
        if ((*(*i)) < sItem )
            sup = (*(*i));
        else
            return sup;
    }
    return (*back());
}

//---------------------------------------------------------------------------
// donner le nombre d'items communs (au sens exact) à deux chaines
// pVectItemSource : vecteur contenant les items de la première chaine
// exemple  A/B/C/D/E/I  et A/B/C/k/E/I donne 3
//---------------------------------------------------------------------------
int
VecteurString::NbElementCommun(VecteurString* pVectItemCible)
{
    if ((empty()) || (pVectItemCible->empty()))
        return 0;

    int NbElementCommun = 0;

    EquiItemIter k = pVectItemCible->begin();
    EquiItemIter i = begin();

    //comparer item par item dans les deux vecteurs
    for (k = pVectItemCible->begin(), i = begin(); (k != pVectItemCible->end()) &&
         (i != end()); i++ , k++)
    {
        if ((*(*k)) == (*(*i)))
            NbElementCommun++;
        else
            break;
    }

    return NbElementCommun;
}

//***************************************************************************
// 							Implémentation des mèthodes VecteurDecode
//***************************************************************************

VecteurDecode::~VecteurDecode()
{
    vider();
}

VecteurDecode::VecteurDecode()
{}

void
VecteurDecode::vider()
{
    if (empty())
        return;

    EquiDecodeIter i = begin();
    while (i != end())
    {
        delete *i;
        erase(i);
    }
}

VecteurDecode::VecteurDecode(VecteurDecode& src)
{
    if (!(src.empty()))
        for (EquiDecodeIter i = src.begin(); i != src.end(); i++)
            push_back(new BBDecodeData(*(*i)));
}

VecteurDecode&
VecteurDecode::operator=(VecteurDecode src)
{
    if (&src == this)
        return *this;

    vider();

    if (!(src.empty())) {
        for (EquiDecodeIter i = src.begin(); i != src.end(); i++)
            push_back(new BBDecodeData(*(*i)));
    }

    return *this;
}

//***************************************************************************
// 							Implémentation des mèthodes VecteurData
//***************************************************************************
/*
VecteurData::~VecteurData()
{
    vider();
}

VecteurData::VecteurData()
{}

void
VecteurData::vider()
{
    if (empty())
        return;

    EquiDataIter i = begin();
    while(i != end())
    {
        delete *i;
        erase(i);
    }
}

VecteurData::VecteurData(VecteurData& src)
{
    if (!(src.empty()))
        for (EquiDataIter i = src.begin(); i != src.end(); i++)
            push_back(new BBItemData(*(*i)));
}

VecteurData&
VecteurData::operator=(VecteurData src)
{
    vider();

    if (!(src.empty()))
        for (EquiDataIter i = src.begin(); i != src.end(); i++)
            push_back(new BBItemData(*(*i)));

    return *this;
}
             */
//-------------------------------------------------------------------------
//----------------------- METHODES DE BBFicheArray ------------------------
//-------------------------------------------------------------------------

// constructeur copie
BBFicheArray::BBFicheArray(BBFicheArray& rv)
    : BBFicheVector()
{
    if (!(rv.empty()))
        for (BBFicheIter i = rv.begin(); i != rv.end(); i++)
            push_back(new BBItemInfo(*(*i)));
}

// fonction vider()
void
BBFicheArray::vider()
{
    if (empty())
        return;

    for (BBFicheIter i = begin(); i != end(); )
    {
        delete *i;
        erase(i);
    }
}

// destructeur
BBFicheArray::~BBFicheArray()
{
    vider();
}

//---------------------------------------------------------------------------
//  Fonction	 : BBFicheArray::operator=(BBFicheArray src)
//
//  Description : Opérateur d'affectation
//---------------------------------------------------------------------------
BBFicheArray&
BBFicheArray::operator=(BBFicheArray src)
{
    if (&src == this)
        return *this;

    vider();

    if (!(src.empty())) {
        for (BBFicheIter i = src.begin(); i != src.end(); i++)
            push_back(new BBItemInfo(*(*i)));
    }

    return *this;
}

//***************************************************************************
// 							Implémentation des mèthodes VecteurItem
//***************************************************************************

VecteurItem::~VecteurItem()
{
    vider();
}

VecteurItem::VecteurItem()
{}

void
VecteurItem::vider()
{
    if (empty())
        return;

    EquiItemVectorIter i = begin();
    while (i != end())
    {
        delete *i;
        erase(i);
    }
}

VecteurItem::VecteurItem(VecteurItem& src)
{
    if (!(src.empty()))
        for (EquiItemVectorIter i = src.begin(); i != src.end(); i++)
            push_back(new VecteurString(*(*i)));
}

VecteurItem&
VecteurItem::operator=(VecteurItem src)
{
    if (&src == this)
        return *this;

    vider();

    if (!(src.empty())) {
        for (EquiItemVectorIter i = src.begin(); i != src.end(); i++)
            push_back(new VecteurString(*(*i)));
    }

    return *this;
}

//-----------------------------------------------------------------------------
// donner le nombre d'items communs ( au sens sémantique) entre  deux chaines
//	sCible : chaine cible
//-----------------------------------------------------------------------------
size_t
VecteurItem::NbElementSemantiqueCommun(VecteurString* pVectItemCible)
{
    if ((!pVectItemCible) || (pVectItemCible->empty()) || (empty()))
        return 0;

    size_t  Nb   = 0;

    //chaine cible
    EquiItemIter IterCible = pVectItemCible->begin();

    //Traiter le père
    EquiItemVectorIter iterPere = begin();
    for(; (IterCible != pVectItemCible->end()) && (iterPere != end()); IterCible++, iterPere++)
    {
        string sItem = *(*IterCible);
        //verifier si sItem appartient au vecteur (en cours) des équivalents
        if ((*iterPere)->ItemDansUnVecteur(sItem))
            Nb++;
        else
            return Nb;
    }
    return Nb;
}

//************************************************************************
// classe RechercheMere
//************************************************************************
RechercheMere::RechercheMere(const RechercheMere& src)
{
    sEtiquette = src.sEtiquette;
    trouve     = src.trouve;

    if (src.pEquivalentTrie)
        pEquivalentTrie = new VecteurItem(*(src.pEquivalentTrie));
    else
        pEquivalentTrie = 0;

    if (src.pEquivalentBrut)
        pEquivalentBrut = new VecteurItem(*(src.pEquivalentBrut));
    else
        pEquivalentBrut = 0;
}

//---------------------------------------------------------------------------
//  Opérateur d'affectation
//---------------------------------------------------------------------------
RechercheMere&
RechercheMere::operator= (RechercheMere src)
{
    if (&src == this)
        return *this;

    sEtiquette 	= src.sEtiquette;
    trouve     	= src.trouve;

    if (pEquivalentTrie)
        delete pEquivalentTrie;

    if (src.pEquivalentTrie)
        pEquivalentTrie = new VecteurItem(*(src.pEquivalentTrie));
    else
        pEquivalentTrie = 0;

    if (pEquivalentBrut)
        delete pEquivalentBrut;

    if (src.pEquivalentBrut)
        pEquivalentBrut = new VecteurItem(*(src.pEquivalentBrut));
    else
        pEquivalentBrut = 0;

    return *this;
}

RechercheMere::RechercheMere(string sEtiqu, bool Trouve)
{
    sEtiquette 			= sEtiqu;
    trouve     			= Trouve;
    pEquivalentTrie = new VecteurItem;
    pEquivalentBrut = new VecteurItem;
}

RechercheMere::~RechercheMere()
{
    if (pEquivalentTrie)
    {
        delete pEquivalentTrie;
        pEquivalentTrie = 0;
    }

    if (pEquivalentBrut)
    {
        delete pEquivalentBrut;
        pEquivalentBrut = 0;
    }
}

//------------------------------------------------------------------------
// pVectItemPere : dernier item du père
// pVectItemCible vecteur contenant la partie de sCible(initiale) au delà du père
//------------------------------------------------------------------------
string
RechercheMere::ChaineStrictementSuperieur(VecteurString* pVectItemCible, string sDernierItemPere)
{
    if ((!pVectItemCible) || (pVectItemCible->empty()))
        return "";

    if ((!pEquivalentTrie) || (pEquivalentTrie->empty()))
        return "";

    EquiItemIter IterCible = pVectItemCible->begin();
    size_t NombreElementSemantiqueEnCommun = pEquivalentTrie->NbElementSemantiqueCommun(pVectItemCible);
    EquiItemVectorIter vect = pEquivalentTrie->begin();

    //calculer le nombre d'items  dans l'étiquette du fils
    size_t NBItem   = 0;
    size_t debut    = 0;
    size_t posit    = sEtiquette.find(string(1,cheminSeparationMARK));

    if(posit == NPOS)
        NBItem = 1;
    else
    {
        while (posit != NPOS)
        {
            debut = posit + 1;
            NBItem++;
            posit = sEtiquette.find(string(1,cheminSeparationMARK), debut+1);
        }
    }

    //
    // se positionner sur l'Item de sCible numéro "NombreElementSemantiqueEnCommun + 1"
    //
    size_t Rang;
    for (Rang = 1; (IterCible != pVectItemCible->end()) && (Rang <= NombreElementSemantiqueEnCommun)
         && (vect != pEquivalentTrie->end()); IterCible++, Rang++, vect++);

    bool CherchePivot = true;
    string sPivot = "";
    bool dernierEstUnTilde = false;
    if ((Rang != pVectItemCible->size()) && (Rang != pEquivalentTrie->size()))
    {
        Rang++; //sPivot est l'item qui suit celui dont la position est Rang
        if (vect != pEquivalentTrie->end())
            vect++;
        if (IterCible != pVectItemCible->end())
            IterCible++;
    }

    while((CherchePivot) && (Rang))
    {
        //
        // ATTENTION : si l'élément ne suit pas un ~~** on cherche
        // l'élément strictement supérieur pour le rang en cours.
        //
        // Si l'élément suit un ~~** (qui peut remplacer plusieurs items),
        // on cherche le plus petit des éléments strictement supérieurs
        // pour tous les éléments situés après
        //
        if (IterCible == pVectItemCible->begin() ) //premier item du fils
        {
            if (sDernierItemPere.find(string("*")) != NPOS)
                dernierEstUnTilde = true;
        }
        else //on n'est pas sur le premier item du père
        {
            if (IterCible != pVectItemCible->end())
            {
                string sPrePivot;
                if (IterCible != pVectItemCible->begin())
                {
                    IterCible--;
                    sPrePivot = *(*IterCible);
                    IterCible++;
                }
                if (sPrePivot.find(string("*")) != NPOS)
                    dernierEstUnTilde = true;
            }
        }

        if (!dernierEstUnTilde)  //l'élément avant le pivot n'est pas un "tilde"
        {
            if ((vect != pEquivalentTrie->end()) && (IterCible != pVectItemCible->end()))
                sPivot = (*vect)->ItemStrictementSuperieur(*(*IterCible));
            if (sPivot != "")
                CherchePivot = false;
        }
        /*******************************************************************/
        else
        {
            //l'item avant sPivot est un "~~***"
            EquiItemVectorIter Tempvect = pEquivalentTrie->begin();
            size_t rangSup  = 0;
            size_t rang     = 1;
            string sSup     = "~~~~~~~~~~~~~";
            string sPivotSupLocal;
            while (rang <= NBItem)
            {
                // Si on est au-delà des équivalents sémantiques de sCible
                if (rang >= Rang)
                {
                    sPivotSupLocal = (*Tempvect)->ItemStrictementSuperieur(*(*IterCible));
                    if ((sPivotSupLocal != "") && (sPivotSupLocal < sSup))
                    {
                        char chFirst = sPivotSupLocal[0];
                        char chLast  = sPivotSupLocal[strlen(sPivotSupLocal.c_str())-1];

                        if ((chFirst != '~') ||
                                ((chLast >= '0') && (chLast <= '9')) ||
                                ((chLast >= 'A') && (chLast <= 'Z')))
                        {
                            sSup = sPivotSupLocal;
                            rangSup = rang;
                        }
                    }
                }
                if (rang < pEquivalentTrie->size())
                    Tempvect++;
                rang++;
            }
            if (sSup != "~~~~~~~~~~~~~")
            {
                if (sSup < sPivotSupLocal)
                    sPivot = sSup;
                else
                    sPivot = sPivotSupLocal;

                CherchePivot = false;
                rang = rangSup;
                Rang = rang;
            }
            else
            {
                if(Rang <= 1)
                    return "";
            }
            //
            //else
            //		return "";
            //Rang = rang;
        }
        /*******************************************************************/
        if (sPivot != "")
            CherchePivot = false;
        else
        {
            Rang--;
            if (vect != pEquivalentTrie->begin())
                vect--;
            if (IterCible != pVectItemCible->begin())
                IterCible--;
        }
    }

    if (Rang == 0)
        return "";

    //traiter le père
    string sNouvelleCible = "";
    EquiItemIter iter = pVectItemCible->begin();
    for (size_t cont = 1; (iter != pVectItemCible->end()) && (cont < Rang); iter++, cont++)
        sNouvelleCible += *(*iter) + string(1,cheminSeparationMARK);

    sNouvelleCible += sPivot;
    while (Rang < pEquivalentTrie->size())
    {
        if(vect != pEquivalentTrie->end())
            vect++;
        Rang++;
        if(vect != pEquivalentTrie->end())
            sNouvelleCible += string(1,cheminSeparationMARK) + *(*(*vect)->begin());
        if(IterCible != pVectItemCible->end())
            IterCible++;
    }
    return sNouvelleCible;
}

//************************************************************************
// classe RechercheSelonCritereData
//************************************************************************
RechercheSelonCritereData::RechercheSelonCritereData(const RechercheSelonCritereData& src)
{
    sEtiquette = src.sEtiquette;
    trouve     = src.trouve;
    if (src.pEquivalentTrie)
        pEquivalentTrie = new VecteurItem(*(src.pEquivalentTrie));
    else
        pEquivalentTrie = 0;
    if (src.pEquivalentBrut)
        pEquivalentBrut = new VecteurItem(*(src.pEquivalentBrut));
    else
        pEquivalentBrut = 0;

    if (src.pVecteurData)
        pVecteurData = new VecteurData(*(src.pVecteurData));
    else
        pVecteurData = 0;

    if (src.pDonnees)
        pDonnees = new BBItemData(*(src.pDonnees));
    else
        pDonnees = 0;
}

//---------------------------------------------------------------------------
//  Opérateur d'affectation
//---------------------------------------------------------------------------
RechercheSelonCritereData&
RechercheSelonCritereData::operator=(RechercheSelonCritereData src)
{
    if (&src == this)
        return *this;

    sEtiquette 	= src.sEtiquette;
    trouve     	= src.trouve;

    if (pEquivalentTrie)
        delete pEquivalentTrie;
    if (src.pEquivalentTrie)
        pEquivalentTrie = new VecteurItem(*(src.pEquivalentTrie));
    else
        pEquivalentTrie = 0;

    if (pEquivalentBrut)
        delete pEquivalentBrut;
    if (src.pEquivalentBrut)
        pEquivalentBrut = new VecteurItem(*(src.pEquivalentBrut));
    else
        pEquivalentBrut = 0;

    if (pVecteurData)
        delete pVecteurData;
    if (src.pVecteurData)
        pVecteurData 	= new VecteurData(*(src.pVecteurData));
    else
        pVecteurData    = 0;

    if (pDonnees)
        delete pDonnees;

    if (src.pDonnees) {
        pDonnees = new BBItemData(*(src.pDonnees));
    } else {
        pDonnees = 0;
    }

    return *this;
}

RechercheSelonCritereData::RechercheSelonCritereData( string sEtiqu, bool Trouve)
    :RechercheMere(sEtiqu, Trouve)
{
    pVecteurData = new VecteurData;
    pDonnees	   = new BBItemData;
    pDonnees->metAZero();
}

RechercheSelonCritereData::~RechercheSelonCritereData()
{
    if (pVecteurData)
        delete pVecteurData;
    if (pDonnees)
        delete pDonnees;
}

//************************************************************************
// classe RechercheSelonCritereDecode
//************************************************************************

RechercheSelonCritereDecode::RechercheSelonCritereDecode(string sEtiqu, bool Trouve)
    :RechercheMere(sEtiqu, Trouve)
{
    pVecteurData = new VecteurDecode;
    pDonnees	   = new BBDecodeData;
    pDonnees->metAZero();
}

RechercheSelonCritereDecode::~RechercheSelonCritereDecode()
{
    if (pVecteurData)
        delete pVecteurData;
    if (pDonnees)
        delete pDonnees;
}

RechercheSelonCritereDecode::RechercheSelonCritereDecode(const RechercheSelonCritereDecode& src)
{
    sEtiquette = src.sEtiquette;
    trouve     = src.trouve;

    if (src.pEquivalentTrie)
        pEquivalentTrie = new VecteurItem(*(src.pEquivalentTrie));
    else
        pEquivalentTrie = 0;

    if (src.pEquivalentBrut)
        pEquivalentBrut = new VecteurItem(*(src.pEquivalentBrut));
    else
        pEquivalentBrut = 0;

    if (src.pVecteurData)
        pVecteurData 	= new VecteurDecode(*(src.pVecteurData));
    else
        pVecteurData    = 0;

    if (src.pDonnees)
        pDonnees        = new BBDecodeData(*(src.pDonnees));
    else
        pDonnees        = 0;
}

//---------------------------------------------------------------------------
//  Opérateur d'affectation
//---------------------------------------------------------------------------
RechercheSelonCritereDecode&
RechercheSelonCritereDecode::operator= (RechercheSelonCritereDecode src)
{
    if (&src == this)
        return *this;

    sEtiquette 	= src.sEtiquette;
    trouve     	= src.trouve;

    if (pEquivalentTrie)
        delete pEquivalentTrie;
    if (src.pEquivalentTrie)
        pEquivalentTrie = new VecteurItem(*(src.pEquivalentTrie));
    else
        pEquivalentTrie = 0;

    if (pEquivalentBrut)
        delete pEquivalentBrut;
    if (src.pEquivalentBrut)
        pEquivalentBrut = new VecteurItem(*(src.pEquivalentBrut));
    else
        pEquivalentBrut = 0;

    if (pVecteurData)
        delete pVecteurData;
    if (src.pVecteurData)
        pVecteurData 	= new VecteurDecode(*(src.pVecteurData));
    else
        pVecteurData    = 0;

    if (pDonnees)
        delete pDonnees;
    if (src.pDonnees)
        pDonnees		= new BBDecodeData(*(src.pDonnees));
    else
        pDonnees        = 0;

    return *this;
}

//***************************************************************************
// Implémentation des mèthodes VecteurRechercheSelonCritere
//***************************************************************************
VecteurRechercheSelonCritere::~VecteurRechercheSelonCritere()
{
    vider();
}

VecteurRechercheSelonCritere::VecteurRechercheSelonCritere(string sTypeBD)
    :VecRechSelonCritere()
{
    sTypeBase = sTypeBD;
}

void
VecteurRechercheSelonCritere::vider()
{
    if (empty())
        return;

    IterCritere i = begin();
    while (i != end())
    {
        delete *i;
        erase(i);
    }
}

VecteurRechercheSelonCritere&
VecteurRechercheSelonCritere::operator=(VecteurRechercheSelonCritere src)
{
    if (&src == this)
        return *this;

    vider();

    if (!(src.empty()))
    {
        if (sTypeBase == GUIDE)
        {
            for (IterCritere i = src.begin(); i != src.end(); i++)
            {
                RechercheSelonCritereData* pRSC = static_cast<RechercheSelonCritereData*>(*i);
                if (pRSC)
                    push_back(new RechercheSelonCritereData(*pRSC));
            }
        }
        else if (sTypeBase == DECODE)
        {
            for (IterCritere i = src.begin(); i != src.end(); i++)
            {
                RechercheSelonCritereDecode* pRSD = static_cast<RechercheSelonCritereDecode*>(*i);
                if (pRSD)
                    push_back(new RechercheSelonCritereDecode(*pRSD));
            }
        }
    }
    return *this;
}

VecteurRechercheSelonCritere::VecteurRechercheSelonCritere(VecteurRechercheSelonCritere& src)
{
    if (src.empty())
        return;

    if (sTypeBase == GUIDE)
    {
        for (IterCritere i = src.begin(); i != src.end(); i++)
        {
            RechercheSelonCritereData* pRSC = static_cast<RechercheSelonCritereData*>(*i);
            if (pRSC)
                push_back(new RechercheSelonCritereData(*pRSC));
        }
        return;
    }

    if (sTypeBase == DECODE)
    {
        for (IterCritere i = src.begin(); i != src.end(); i++)
        {
            RechercheSelonCritereDecode* pRSD = static_cast<RechercheSelonCritereDecode*>(*i);
            if (pRSD)
                push_back(new RechercheSelonCritereDecode(*pRSD));
        }
    }
}

//----------------------------------------------------------------------------
// savoir s'il reste des fils à traiter
//----------------------------------------------------------------------------
bool
VecteurRechercheSelonCritere::FilsAtraiter()
{
    if (empty())
        return false;

    for (IterCritere i = begin(); i != end(); i++)
        if ((*i)->trouve == false)
            return true;

    return false;
}

//---------------------------------------------------------------------------
// VERIFIER SI sItem APPARTIENT A UN VECTEUR
// rang de la recherche
//---------------------------------------------------------------------------
bool
VecteurRechercheSelonCritere::ItemDansUnVecteur(int rang, string sItem)
{
    if (empty())
        return false;

    int Rang;
    for (IterCritere i = begin(); i != end(); i++)
    {
        if (!(*i)->trouve)
        {
            if (!((*i)->pEquivalentTrie->empty()))
            {
                EquiItemVectorIter iterVect = (*i)->pEquivalentTrie->begin();
                for (Rang = 1; (iterVect != (*i)->pEquivalentTrie->end()) && (Rang < rang); iterVect++, Rang++);
                if (iterVect != (*i)->pEquivalentTrie->end())
                    if ((*iterVect)->ItemDansUnVecteur(sItem))
                        return true;
            }
        }
    }

    return false;
}

//----------------------------------------------------------------------------
// Borne Inférieure des vecteurs situés au RANG Rang
//----------------------------------------------------------------------------
string VecteurRechercheSelonCritere::BorneInf(int Rang)
{
    if (empty())
        return "";

    int rang;
    string sInf = string("~~~~~~~~~~~");
    for (IterCritere i = begin(); i != end(); i++)
    {
        if (!(*i)->trouve)
        {
            if (!((*i)->pEquivalentTrie->empty()))
            {
                EquiItemVectorIter iterVect = (*i)->pEquivalentTrie->begin();
                for (rang = 1; (iterVect  != (*i)->pEquivalentTrie->end()) && (rang < Rang); iterVect++, rang++);
                if (iterVect != (*i)->pEquivalentTrie->end())
                    if ((*(*((*iterVect)->begin()))) < sInf)
                        sInf = (*(*((*iterVect)->begin())));
            }
        }
    }
    if (sInf == string("~~~~~~~~~~~"))
        sInf = string("");

    return sInf;
}

//----------------------------------------------------------------------------
// retourner la chaine du fils ayant la chaine sémantique (formée par des équivalents)
// la plus minimale
//----------------------------------------------------------------------------
string
VecteurRechercheSelonCritere::BorneInf()
{
    if (empty())
        return "";

    string sBorneInf = "~~~~~~~~~~~~~~";
    string sInf = "";
    string sEtiquette = ""; //étiquette du fils à retourner

    for (IterCritere i = begin(); i != end(); i++)
    {
        if (!(*i)->trouve)
        {
            if (!((*i)->pEquivalentTrie->empty()))
            {
                sInf = "";
                EquiItemVectorIter iterVect = (*i)->pEquivalentTrie->begin();
                if (iterVect != (*i)->pEquivalentTrie->end())
                {
                    if ((*iterVect) && (!((*iterVect)->empty())))
                        sInf = (*(*((*iterVect)->begin())));
                    iterVect++;
                }
                for (; iterVect  != (*i)->pEquivalentTrie->end(); iterVect++)
                    if ((*iterVect) && (!((*iterVect)->empty())))
                        sInf += string(1,cheminSeparationMARK) + (*(*((*iterVect)->begin())));
                if (sInf < sBorneInf)
                    sBorneInf = sInf;
            }
        }
    }

    if (sBorneInf == "~~~~~~~~~~~~~~")
        sBorneInf = "";
    return sBorneInf;
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
string
VecteurRechercheSelonCritere::ChaineStrictementSuperieur(VecteurString* pVectItemCible, string sDernierItemPere)
{
    if ((!pVectItemCible) || (pVectItemCible->empty()) || (empty()))
        return "";

    string sStrictementSuperieur = string("~~~~~~~~~");
    string sResult      = "";
    string sEtqiuette   = ""; //étiquette du fils à retourner

    for (IterCritere fils = begin(); fils != end(); fils++)
    {
        if (!(*fils)->trouve)
        {
            string sNouvelleCible = (*fils)->ChaineStrictementSuperieur(pVectItemCible, sDernierItemPere);
            if ((sNouvelleCible != "" ) && (sNouvelleCible < sStrictementSuperieur))
                sStrictementSuperieur = sNouvelleCible;
        }
    }
    if (sStrictementSuperieur == string("~~~~~~~~~"))
        sStrictementSuperieur = "";

    return sStrictementSuperieur;
}

//----------------------------------------------------------------------------
//item strictement situé au rang Rang et supérieur à sItem
//----------------------------------------------------------------------------
string
VecteurRechercheSelonCritere::ItemStrictementSuperieur(int Rang, string sItem)
{
    if (empty())
        return "";

    int rang;
    string sStrictementSuperieur = string("~~~~~~~~~~");
    string sResult;
    for (IterCritere i = begin(); i != end(); i++)
    {
        if (!(*i)->trouve)
        {
            if (!((*i)->pEquivalentTrie->empty()))
            {
                EquiItemVectorIter iterVect = (*i)->pEquivalentTrie->begin();
                for (rang = 1; (iterVect != (*i)->pEquivalentTrie->end()) && (rang < Rang); iterVect++, rang++);
                if (iterVect != (*i)->pEquivalentTrie->end())
                {
                    sResult = (*iterVect)->ItemStrictementSuperieur(sItem);

                    if ((sResult != "") && (sResult < sStrictementSuperieur))
                        sStrictementSuperieur = sResult;
                }
            }
        }
    }

    if (sStrictementSuperieur == string("~~~~~~~~~~"))
        sStrictementSuperieur = "";
    return sStrictementSuperieur;
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
string
VecteurRechercheSelonCritere::ItemStrictementInferieur(int Rang, string sItem)
{
    if (empty())
        return "";

    int rang;
    string sStrictementSuperieur = string("");
    string sResult;
    for (IterCritere i = begin(); i != end(); i++)
    {
        if (!(*i)->trouve)
        {
            if (!((*i)->pEquivalentTrie->empty()))
            {
                EquiItemVectorIter iterVect = (*i)->pEquivalentTrie->begin();
                for (rang = 1; (iterVect != (*i)->pEquivalentTrie->end()) && (rang < Rang); iterVect++, rang++);
                if (iterVect != (*i)->pEquivalentTrie->end())
                {
                    sResult = (*iterVect)->ItemStrictementInferieur(sItem);
                    if ((sResult != "") && (sStrictementSuperieur < sResult))
                        sStrictementSuperieur = sResult;
                }
            }
        }
    }
    return sStrictementSuperieur;
}

//---------------------------------------------------------------------------
//ajouter l'étiquette sEtiquette au vecteur
//---------------------------------------------------------------------------
void
VecteurRechercheSelonCritere::AjouteEtiquette(string sEtiquette)
{
    if (sTypeBase == GUIDE)
    {
        RechercheSelonCritereData* pVect = new RechercheSelonCritereData;
        pVect->sEtiquette = sEtiquette;
        //
        //"#C£££" cas particulier des textes libres
        //
        if (sEtiquette.find(string("£")) != NPOS)
        {
            pVect->trouve = true;
            pVect->pDonnees->metAZero();
        }
        else
            pVect->trouve = false;

        push_back(pVect);

        return;
    }

    if (sTypeBase == DECODE)
    {
        RechercheSelonCritereDecode* pVect = new RechercheSelonCritereDecode;
        pVect->sEtiquette = sEtiquette;
        //
        //"#C£££" cas particulier des textes libres
        //
        if (sEtiquette.find(string("£")) != NPOS)
        {
            pVect->trouve = true;
            pVect->pDonnees->metAZero();
        }
        else
            pVect->trouve = false;

        push_back(pVect);

        return;
    }
}

//---------------------------------------------------------------------------
// A partir de l'étiquette sEtiquette du fils on instancie trouve et pDonnees
// relatifs à ce fils
//---------------------------------------------------------------------------
void
VecteurRechercheSelonCritere::SetData(string sEtiquette, bool* trouve, BBChemData* pDonnees)
{
    if (NULL == trouve)
        return;

    *trouve = false;

    if (empty())
        return;

    if (sTypeBase == DECODE)
    {
        for (IterCritere i = begin(); i != end(); i++)
        {
            if ((*i)->sEtiquette == sEtiquette )
            {
                if (trouve)
                    *trouve = (*i)->trouve;
                RechercheSelonCritereDecode* pRSC = static_cast<RechercheSelonCritereDecode*>(*i);
                if (pDonnees)
                {
                    BBDecodeData* pDo = static_cast<BBDecodeData*>(pDonnees);
                    if (pDo)
                        *pDo = *(pRSC->pDonnees);
                }
                return;
            }
        }
        return;
    }

    if (sTypeBase == GUIDE)
    {
        for (IterCritere i = begin(); end() != i; i++)
        {
            if ((*i)->sEtiquette == sEtiquette )
            {
                if (trouve)
                    *trouve = (*i)->trouve;
                RechercheSelonCritereData* pRSC = static_cast<RechercheSelonCritereData*>(*i);
                if (pDonnees)
                {
                    BBItemData* pDo = static_cast<BBItemData*>(pDonnees);
                    if (pDo)
                        *pDo = *(pRSC->pDonnees);
                }
                return;
            }
        }
    }
}

