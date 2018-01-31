//***************************************************************************
//  NOM du FICHIER  : NSBBTRAN.CPP
//
//  Implémentation de toutes les classes qui font l'interface entre
//  BBItem et NSdlg
//
//  NSTransferInfo
//  NSTransferArray
//  Kaddachi Hafedh
// Date de création : PA Mai 96                mise à jour : PA 15/02/1995
//**************************************************************************

#ifndef _ENTERPRISE_DLL
  #include "partage/nsdivfct.h"
#else
  #include "enterpriseLus/nsdivfctForCgi.h"
#endif

#include "nsbb/nsbbtran.h"
#include "nsbb/nspatpat.h"

#ifndef _ENTERPRISE_DLL

  #include "nautilus\nssuper.h"
  #include "nssavoir\nsguide.h"

  #include "nsbb\nsbb_msg.h"
  #include "nsbb\nsbbitem.h"
  #include "nsbb\nsbb_dlg.h"

  long NSTransferInfo::lObjectCount = 0;

#else

  #include "enterpriseLus/superLus.h"

#endif

long Message::lObjectCount = 0;

//---------------------------------------------------------------------------
//  Constructeur
//---------------------------------------------------------------------------
Message::Message(string noeud, string lexique, string complement, string type,
                 string Interet, string Certitude, string Pluriel,
                 string Visible, string TL, string sArc, string sUnite, string rig)
        :sNoeud(noeud), sLexique(lexique), sComplement(complement), sInteret(Interet),
         sCertitude(Certitude), sPluriel(Pluriel),
         sVisible(Visible), sType(type), sTexteLibre(TL), sArchetype(sArc), sUnit(sUnite), sRights(rig)
{
	pTemporaryLinks = 0;
}

//---------------------------------------------------------------------------
//  Constructeur par défaut
//---------------------------------------------------------------------------
Message::Message()
{
	pTemporaryLinks = 0;

  Reset();
}

//---------------------------------------------------------------------------
//  Constructeur copie
//---------------------------------------------------------------------------
Message::Message(const Message& src)
{
	sTreeID     = src.sTreeID;
	sNoeud      = src.sNoeud;
	sLexique    = src.sLexique;
	sComplement = src.sComplement;
	sType       = src.sType;
	sInteret    = src.sInteret;
	sCertitude  = src.sCertitude;
	sPluriel    = src.sPluriel;
	sVisible    = src.sVisible;
	sTexteLibre = src.sTexteLibre;
	sArchetype  = src.sArchetype;
	sUnit       = src.sUnit;
	sRights     = src.sRights;
  
	pTemporaryLinks = 0;
	if (src.pTemporaryLinks)
  	pTemporaryLinks = new NSLinkedNodeArray(*(src.pTemporaryLinks));
}

//---------------------------------------------------------------------------
//  Surcharge de l'opérateur d'affectation
//---------------------------------------------------------------------------
Message&
Message::operator=(Message src)
{
	if (this == &src)
		return *this;

	sTreeID     = src.sTreeID;
	sNoeud      = src.sNoeud;
  sLexique    = src.sLexique;
	sComplement = src.sComplement;
  sType       = src.sType;
  sInteret    = src.sInteret;
	sCertitude  = src.sCertitude;
  sPluriel    = src.sPluriel;
  sVisible    = src.sVisible;
  sTexteLibre = src.sTexteLibre;
  sArchetype  = src.sArchetype;
  sUnit       = src.sUnit;
  sRights     = src.sRights;
  SetTemporaryLinks(src.pTemporaryLinks);

	return *this;
}

Message::~Message()
{
	if (pTemporaryLinks)
  	delete pTemporaryLinks;
}

void
Message::Reset()
{
	sTreeID     = string("");
	sNoeud      = string("");
	sLexique    = string("");
	sComplement = string("");
	sType       = string("");
	sInteret    = string("A");
	sCertitude  = string("");
	sPluriel    = string("");
	sVisible    = string("1");
	sTexteLibre = string("");
	sArchetype  = string("");
	sUnit       = string("");
	sRights     = string("");

	if (pTemporaryLinks)
  	delete pTemporaryLinks;

  pTemporaryLinks = 0;
}

void
Message::SetTemporaryLinks(NSLinkedNodeArray* pArray)
{
	if (pTemporaryLinks)
	{
  	delete pTemporaryLinks;
    pTemporaryLinks = 0;
	}
	if (pArray && (false == pArray->empty()))
  	pTemporaryLinks = new NSLinkedNodeArray(*pArray);
}

//-----------------------------------------------------------------------
// exemple si sEtiquette : PFONC1.WCE751
//									---> patpatho : certitude : WCE751
//-----------------------------------------------------------------------
void
Message::MettreAJourCertitudePluriel(string sEtiquette)
{
  size_t posWPL 	= sEtiquette.find(string("WPL"));
  size_t posWCE  	= sEtiquette.find(string("WCE"));
  size_t posWCEA0 = sEtiquette.find(string("WCEA0"));
  size_t posCompl = sEtiquette.find(string("$"));

  // si pluriel
  if ((posWPL != NPOS))
    SetPluriel(string(sEtiquette, posWPL, BASE_PLURIEL_LEN));

  // si certitude = présence totale, ne rien mettre dans la case certitude de
  // la patpatho (c'est la valeur par défaut)
  if (posWCEA0 != NPOS)
    SetCertitude(string(""));
  else
    if ((posWCE != NPOS))
      SetCertitude(string(sEtiquette, posWCE, BASE_CERTITUDE_LEN));

  // on considère que le complement est toujours à la fin de sEtiquette
  if (posCompl != NPOS)
  {
    string sCompl = string(sEtiquette, posCompl+1, strlen(sEtiquette.c_str())-posCompl-1);
    if (strlen(sCompl.c_str()) > BASE_COMPLEMENT_LEN)
      sCompl = string(sCompl, 0, BASE_COMPLEMENT_LEN);
    SetComplement(sCompl);
  }
}

void
Message::InitFromEtiquette(string sEtiquette)
{
  char separator = intranodeSeparationMARK;

  // L'étiquette peut être du type Lexique/Certitude/Pluriel ou Lexique.Certitude.Pluriel
  //
  size_t posit = sEtiquette.find(string(1, separator));
  if (NPOS == posit)
  {
    separator = cheminSeparationMARK;
    posit = sEtiquette.find(string(1, separator));
  }
  //
  // Si c'est le cas, la méthode la plus propre consiste
  // à passer par un Message
  //
  if (posit == NPOS)
  {
    sLexique = sEtiquette;
    return;
  }

  int iTaille = strlen(sEtiquette.c_str());
  sLexique   = string(sEtiquette, 0, posit);
  sEtiquette = string(sEtiquette, posit + 1, iTaille - posit - 1);

  if (sLexique[0] == '2')
  {
    sUnit      = sLexique;

    posit = sEtiquette.find(string(1, separator));
    if (posit == NPOS)
    {
      sLexique = sEtiquette;
      return;
    }

    sLexique   = string(sEtiquette, 0, posit);
    sEtiquette = string(sEtiquette, posit + 1, iTaille - posit - 1);
  }

  MettreAJourCertitudePluriel(sEtiquette);
}

#ifndef _ENTERPRISE_DLL

//***************************************************************************
// Implémentation des méthodes NSTransferInfo
//***************************************************************************

//---------------------------------------------------------------------------
//  Constructeurs
//---------------------------------------------------------------------------
NSTransferInfo::NSTransferInfo(BBFilsItem* pFilsItem, int actif,
                               string noeud, string complement, string type,
                               string interet, string certitude, string pluriel,
                               string visible, string sTL, string sArc, string sUnite)
               :_pBBFilsItem(pFilsItem), _iActif(actif),
                _TransfertMessage(noeud, complement, type, interet, certitude,
                                  pluriel, visible, sTL, sArc, sUnite)
{
try
{
	_pControle 	          = (NSControle*) 0;
	_pTmpTransfertMessage = &_TransfertMessage;

  lObjectCount++;
}
catch (...)
{
	error("Exception (new NSTransferInfo 1).", standardError, 0);
}
}

NSTransferInfo::NSTransferInfo(BBFilsItem* pFilsItem, const NSTransferInfo& src)
               :_pBBFilsItem(pFilsItem), _TransfertMessage(src._TransfertMessage),
                _TransPatpatho(src._TransPatpatho),
                _TempoPatpatho(src._TempoPatpatho)
{
try
{
	_iActif               = src._iActif;
	_pControle            = src._pControle;

	_pTmpTransfertMessage = &_TransfertMessage ;

  lObjectCount++;
}
catch (...)
{
	error("Exception (new NSTransferInfo 2).", standardError, 0);
}
}

//---------------------------------------------------------------------------
//  Destructeur
//---------------------------------------------------------------------------
NSTransferInfo::~NSTransferInfo()
{
  lObjectCount--;

  // Disconnect from Control
  //
	if (_pControle)
		_pControle->setTransfert((NSTransferInfo*) 0);
}

//---------------------------------------------------------------------------
//  Demande à un contrôle de se mettre dans l'état voulu
//---------------------------------------------------------------------------
void
NSTransferInfo::activeControle(int activation, Message* pMessage)
{
	if (_pControle)
		_pControle->activeControle(activation, pMessage);
}

//---------------------------------------------------------------------------
//  Transmet à BBItem l'avis d'activation d'un contrôle
//
//  etatInitial	 : état du contrôle avant activation
//  etatSouhaité : état souhaité après activation
//  message		 : renseignements complémentaires : Donnees & Type
//---------------------------------------------------------------------------
void
NSTransferInfo::ctrlNotification(int etatInitial, int etatSouhaite, Message* pMessage, int indexFils)
{
	if (_pBBFilsItem && _pBBFilsItem->getItemFather())
		_pBBFilsItem->getItemFather()->ctrlNotification(_pBBFilsItem, &_pControle->getIdentite(),
                                         etatInitial, etatSouhaite, pMessage, indexFils);
}

//---------------------------------------------------------------------------
//  Transfère les données du contrôle vers iActif et sDonnees
//						et vice-versa suivant la direction
//
//  direction : tdGetData ou tdSetData
//
//  Retourne  :	1 si le transfert a fonctionné
//				0 sinon
//---------------------------------------------------------------------------
uint
NSTransferInfo::Transfer(TTransferDirection direction)
{
	if (_pControle)
		return _pControle->Transfer(direction);
	return 1;
}

uint
NSTransferInfo::TransferFinal(TTransferDirection direction)
{
	if (_pControle)
		return _pControle->TransferFinal(direction);
	return 1;
}

uint
NSTransferInfo::TempTransfer()
{
	if (_pControle)
		return _pControle->TempTransfer();
	return 1;
}

void
NSTransferInfo::Desactive()
{
	_iActif = 0;
}

void
NSTransferInfo::Active()
{
	_iActif = 1;
}

//---------------------------------------------------------------------------
//  Enregistre le référencement,
//  lance éventuellement la fonction d'initialisation
//
//  pCtrl : pointeur sur le contrôle Windows qui se déclare
//---------------------------------------------------------------------------
void
NSTransferInfo::referenceControle(NSControle* pCtrl)
{
	_pControle = pCtrl;
}

//---------------------------------------------------------------------------
//  Détache la partie Windows de la partie BigBrother
//---------------------------------------------------------------------------
void
NSTransferInfo::detacheControle()
{
	if (_pControle)
	{
		_pControle->setTransfert((NSTransferInfo*) 0);
		_pControle = (NSControle*) 0;
	}
}

//---------------------------------------------------------------------------
//  Constructeur copie
//---------------------------------------------------------------------------
NSTransferInfo::NSTransferInfo(const NSTransferInfo& src)
               :_TransPatpatho(src._TransPatpatho),
                _TempoPatpatho(src._TempoPatpatho),
                _TransfertMessage(src._TransfertMessage)

{
try
{
	_pBBFilsItem = src._pBBFilsItem;
	_pControle   = src._pControle;

	_pTmpTransfertMessage = &_TransfertMessage;

	_iActif	     = src._iActif;

  lObjectCount++;
}
catch (...)
{
	error("Exception (new NSTransferInfo copie).", standardError, 0);
}
}

//---------------------------------------------------------------------------
//  Surcharge de l'opérateur d'affectation
//---------------------------------------------------------------------------
NSTransferInfo&
NSTransferInfo::operator=(NSTransferInfo src)
{
	if (this == &src)
		return *this;

	_pBBFilsItem = src._pBBFilsItem;
	_pControle   = src._pControle;

	_TransfertMessage   = src._TransfertMessage;
	_pTmpTransfertMessage = &_TransfertMessage;

	_TransPatpatho      = src._TransPatpatho;
	_TempoPatpatho      = src._TempoPatpatho;

	_iActif	   = src._iActif;

	return *this;
}

//---------------------------------------------------------------------------
//  Opérateur de comparaison
//---------------------------------------------------------------------------
int
NSTransferInfo::operator==(const NSTransferInfo& x)
{
	return ((_pControle == x._pControle) && (_pBBFilsItem == x._pBBFilsItem));
}

//***************************************************************************
// Implémentation des méthodes NSTransferArray
//***************************************************************************

//---------------------------------------------------------------------------
//  Constructeur copie
//---------------------------------------------------------------------------
NSTransferArray::NSTransferArray(NSTransferArray& rv)
                :TransferArray()
{
try
{
	if (false == rv.empty())
		for (iterTransferArray i = rv.begin(); rv.end() != i; i++)
			push_back(new NSTransferInfo(*(*i)));
}
catch (...)
{
	error("Exception (new NSTransferArray copie).", standardError, 0);
}
}

//---------------------------------------------------------------------------
//  Destructeur
//---------------------------------------------------------------------------
NSTransferArray::~NSTransferArray()
{
	if (empty())
		return;
	for (iterTransferArray i = begin(); end() != i; )
	{
  	delete *i;
    erase(i);
	}
}

#endif // ifndef _ENTERPRISE_DLL

