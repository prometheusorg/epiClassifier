#ifdef _ENTERPRISE_DLL
  #include "enterpriseLus/nsdivfctForCgi.h"
  #include "enterpriseLus/superLus.h"
  #include "mysql/mysql.h"
#else
  #include "partage/nsdivfct.h"
  #include "nautilus/nssuper.h"
#endif

#include "nssavoir/nspatho.h"
#include "nssavoir/nsfilgd.h"
#include "dcodeur/nsphrase.h"
#include "dcodeur/nsgen.h"
#include "dcodeur/nsdkd.h"

// -------------------------------------------------------------------------
// ---------------------- METHODES DE phraseur -----------------------------
// -------------------------------------------------------------------------
NSPhraseur::NSPhraseur(NSContexte* pCtx)
           :NSRoot(pCtx), LiaisonLieu(pCtx), PrepositionLieu(pCtx),
                          LiaisonTemps(pCtx), PrepositionTemps(pCtx),
                          LiaisonManiere(pCtx), PrepositionManiere(pCtx),
                          LiaisonMoyen(pCtx), PrepositionMoyen(pCtx),
                          LiaisonCause(pCtx), PrepositionCause(pCtx),
                          LiaisonBut(pCtx), PrepositionBut(pCtx),
                          LiaisonType(pCtx), PrepositionType(pCtx),
                          LiaisonHypoth(pCtx), PrepositionHypoth(pCtx),
                          LiaisonChiffre(pCtx), PrepositionChiffre(pCtx),
                          adjNumeralCardinal(pCtx), adjNumeralOrdinal(pCtx)
{
  initialise() ;
}

NSPhraseur::NSPhraseur(NSPhraseur& rv)
           :NSRoot(rv.pContexte),
            LiaisonLieu(rv.pContexte),
            PrepositionLieu(rv.pContexte),
            LiaisonTemps(rv.pContexte),
            PrepositionTemps(rv.pContexte),
            LiaisonManiere(rv.pContexte),
            PrepositionManiere(rv.pContexte),
            LiaisonMoyen(rv.pContexte),
            PrepositionMoyen(rv.pContexte),
            LiaisonCause(rv.pContexte),
            PrepositionCause(rv.pContexte),
            LiaisonBut(rv.pContexte),
            PrepositionBut(rv.pContexte),
            LiaisonType(rv.pContexte),
            PrepositionType(rv.pContexte),
            LiaisonHypoth(rv.pContexte),
            PrepositionHypoth(rv.pContexte),
            LiaisonChiffre(rv.pContexte),
            PrepositionChiffre(rv.pContexte),
            adjNumeralCardinal(rv.pContexte),
            adjNumeralOrdinal(rv.pContexte)
{
try
{
  copieTout(&rv) ;

  pParentheses = (NSPhraseur*) 0 ;
  pDeuxPoints  = (NSPhraseur*) 0 ;

  if (rv.pParentheses)
    pParentheses = new NSPhraseur(*(rv.pParentheses)) ;

  if (rv.pDeuxPoints)
    pDeuxPoints = new NSPhraseur(*(rv.pDeuxPoints)) ;
}
catch (...)
{
	erreur("Exception NSPhraseur copy ctor.", standardError) ;
}
}

//--------------------------------------------------------------------------
// DESTRUCTEUR
// -------------------------------------------------------------------------

NSPhraseur::~NSPhraseur()
{
  if (pParentheses)
    delete pParentheses ;

  if (pDeuxPoints)
    delete pDeuxPoints ;
}


NSPhraseur&
NSPhraseur::operator=(NSPhraseur src)
{
try
{
	if (this == &src)
		return *this ;

	if (pParentheses)
  {
  	delete pParentheses ;
    pParentheses = (NSPhraseur*) 0 ;
  }

  if (pDeuxPoints)
  {
  	delete pDeuxPoints ;
    pDeuxPoints = (NSPhraseur*) 0 ;
  }

  initialise() ;
  copieTout(&src) ;

	if (src.pParentheses)
  	pParentheses = new NSPhraseur(*(src.pParentheses)) ;

	if (src.pDeuxPoints)
  	pDeuxPoints = new NSPhraseur(*(src.pDeuxPoints)) ;

	return *this ;
}
catch (...)
{
	erreur("Exception NSPhraseur = operator.", standardError) ;
	return *this ;
}
}

void
NSPhraseur::initialise()
{
  iForme      = formeActive ;
  iPhraseType = phrasePrincipale ;

  Sujet.vider() ;

  Verbe.vider() ;
  iVbType     = vbTypeNotInit ;
  iVbEtat     = vbEtatNotInit ;
  iVbTemps    = tempsPresent ;
  iVbAspect   = aspectPonctuel ;
  iVbMode     = modeIndicatif ;
  iVbPersonne = pers3S ;
  iVbNegation = negationNon ;

  iTypeSujet  = sujetNormal ;

  adjEpithete.vider() ;
  adjEpitheteAv.vider() ;
  adjEpitheteAp.vider() ;

  compNom.vider() ;
  adverbe.vider() ;

  COD.vider() ;
  COI.vider() ;
  COS.vider() ;
  AttSujet.vider() ;
  AttCOD.vider() ;

  pParentheses = (NSPhraseur*) 0 ;

  LiaisonLieu.metAZero() ;
  PrepositionLieu.metAZero() ;
  iTypeLocalisation = locUndefined ;
  CCLieu.vider() ;
  LiaisonTemps.metAZero() ;
  PrepositionTemps.metAZero() ;
  CCTemps.vider() ;
  LiaisonManiere.metAZero() ;
  PrepositionManiere.metAZero() ;
  CCManiere.vider() ;
  LiaisonMoyen.metAZero() ;
  PrepositionMoyen.metAZero() ;
  CCMoyen.vider() ;
  LiaisonCause.metAZero() ;
  PrepositionCause.metAZero() ;
  CCCause.vider() ;
  LiaisonBut.metAZero() ;
  PrepositionBut.metAZero() ;
  CCBut.vider() ;
  LiaisonType.metAZero() ;
  PrepositionType.metAZero() ;
  CCType.vider() ;
  LiaisonHypoth.metAZero() ;
  PrepositionHypoth.metAZero() ;
  CCHypoth.vider() ;
  LiaisonChiffre.metAZero() ;
  PrepositionChiffre.metAZero() ;
  CCChiffre.vider() ;
  adjNumeralOrdinal.metAZero() ;
  adjNumeralCardinal.metAZero() ;

  pDeuxPoints = (NSPhraseur*) 0 ;
}

void
NSPhraseur::copieTout(NSPhraseur* pSrc)
{
  iPhraseType         = pSrc->iPhraseType ;
  iForme              = pSrc->iForme ;
  Sujet               = pSrc->Sujet ;
  iTypeSujet          = pSrc->iTypeSujet ;
  //
  Verbe               = pSrc->Verbe ;
  iVbType             = pSrc->iVbType ;
  iVbEtat             = pSrc->iVbEtat ;
  iVbTemps            = pSrc->iVbTemps ;
  iVbAspect           = pSrc->iVbAspect ;
  iVbMode             = pSrc->iVbMode ;
  iVbPersonne         = pSrc->iVbPersonne ;
  iVbNegation         = pSrc->iVbNegation ;
  //
  adjEpithete         = pSrc->adjEpithete ;
  adjEpitheteAv       = pSrc->adjEpitheteAv ;
  adjEpitheteAp       = pSrc->adjEpitheteAp ;
  adjNumeralOrdinal   = pSrc->adjNumeralOrdinal ;
  adjNumeralCardinal  = pSrc->adjNumeralCardinal ;
  // Complement du nom
  compNom             = pSrc->compNom ;

  // Adverbes
  adverbe             = pSrc->adverbe ;

  // Complements essentiels
  COD                 = pSrc->COD ;
  COI                 = pSrc->COI ;
  COS                 = pSrc->COS ;
  AttSujet            = pSrc->AttSujet ;
  AttCOD              = pSrc->AttCOD ;
  // Complements circonstanciels
  LiaisonLieu         = pSrc->LiaisonLieu ;
  PrepositionLieu     = pSrc->PrepositionLieu ;
  CCLieu              = pSrc->CCLieu ;
  LiaisonTemps        = pSrc->LiaisonTemps ;
  PrepositionTemps    = pSrc->PrepositionTemps ;
  CCTemps             = pSrc->CCTemps ;
  LiaisonManiere      = pSrc->LiaisonManiere ;
  PrepositionManiere  = pSrc->PrepositionManiere ;
  CCManiere           = pSrc->CCManiere ;
  LiaisonMoyen        = pSrc->LiaisonMoyen ;
  PrepositionMoyen    = pSrc->PrepositionMoyen ;
  CCMoyen             = pSrc->CCMoyen ;
  LiaisonCause        = pSrc->LiaisonCause ;
  PrepositionCause    = pSrc->PrepositionCause ;
  CCCause             = pSrc->CCCause ;
  LiaisonBut          = pSrc->LiaisonBut ;
  PrepositionBut      = pSrc->PrepositionBut ;
  CCBut               = pSrc->CCBut ;
  LiaisonType         = pSrc->LiaisonType ;
  PrepositionType     = pSrc->PrepositionType ;
  CCType              = pSrc->CCType ;
  LiaisonHypoth       = pSrc->LiaisonHypoth ;
  PrepositionHypoth   = pSrc->PrepositionHypoth ;
  CCHypoth            = pSrc->CCHypoth ;

  LiaisonChiffre      = pSrc->LiaisonChiffre ;
  PrepositionChiffre  = pSrc->PrepositionChiffre ;
  CCChiffre           = pSrc->CCChiffre ;
}

void
NSPhraseur::initParentheses()
{
try
{
	if (pParentheses)
  	pParentheses->initialise() ;
	else
  	pParentheses = new NSPhraseur(pContexte) ;

	pParentheses->iPhraseType = phraseComplement ;
}
catch (...)
{
	erreur("Exception NSPhraseur::initParentheses.", standardError) ;
}
}

void
NSPhraseur::initDeuxPoints()
{
try
{
	if (pDeuxPoints)
  	pDeuxPoints->initialise() ;
	else
  	pDeuxPoints = new NSPhraseur(pContexte) ;
}
catch (...)
{
	erreur("Exception NSPhraseur::initDeuxPoints.", standardError) ;
}
}

//--------------------------------------------------------------------------
// Le nettoyeur de prepositions
//--------------------------------------------------------------------------

void
NSPhraseur::nettoye()
{
  adjEpithete.initPreposition() ;
  adjEpitheteAv.initPreposition() ;
  adjEpitheteAp.initPreposition() ;
  adjNumeralOrdinal.metAZero() ;
  adjNumeralCardinal.metAZero() ;
  compNom.initPreposition() ;
  adverbe.initPreposition() ;
  COD.initPreposition() ;
  COI.initPreposition() ;
  COS.initPreposition() ;
  AttSujet.initPreposition() ;
  AttCOD.initPreposition() ;
  CCLieu.initPreposition() ;
  CCTemps.initPreposition() ;
  CCManiere.initPreposition() ;
  CCMoyen.initPreposition() ;
  CCCause.initPreposition() ;
  CCBut.initPreposition() ;
  CCType.initPreposition() ;
  CCHypoth.initPreposition() ;
  CCChiffre.initPreposition() ;

  if (pDeuxPoints)
    pDeuxPoints->nettoye() ;
  if (pParentheses)
    pParentheses->nettoye() ;
}

void
NSPhraseur::classeAdjectif(NSGenerateur* pGener)
{
try
{
  if (NULL == pGener)
    return ;

  adjEpitheteAv.vider() ;
  adjEpitheteAp.vider() ;

  NSSuper* pSuper = pContexte->getSuperviseur() ;

  if (adjEpithete.empty())
    return ;

  for (iterPhraseMot iterMots = adjEpithete.begin() ; adjEpithete.end() != iterMots ; iterMots++)
  {
    string sLexique = (*iterMots)->getLexique() ;

    if (false == (*iterMots)->estTexteLibre())
    {
      NSPathologData Data ;
      bool bFound = pContexte->getDico()->trouvePathologData(pGener->getLang(), &sLexique, &Data) ;
      if (false == bFound)
      {
        string sErrorText = string("NSPhraseur::classeAdjectif discarding ") + sLexique + string(" which cannot be found in Lexique for langage ") + pGener->getLang() ;
        pSuper->trace(&sErrorText, 1, NSSuper::trWarning) ;
      }
      if (false == Data.estAdjectif())
      {
        string sErrorText = string("NSPhraseur::classeAdjectif discarding ") + sLexique + string(" because an epithete must be an adjective.") ;
        pSuper->trace(&sErrorText, 1, NSSuper::trWarning) ;
        bFound = false ;
      }

      if (bFound)
      {
        string sPos = Data.chercheGrammaire(ADJ_POSITION) ;

        if      ((*iterMots)->getPosition() == NSPhraseMot::posAvant)
          adjEpitheteAv.push_back(new NSPhraseMot(*(*iterMots))) ;
        else if ((*iterMots)->getPosition() == NSPhraseMot::posApres)
          adjEpitheteAp.push_back(new NSPhraseMot(*(*iterMots))) ;
        else
        {
          if (AV_NOM == sPos)
            adjEpitheteAv.push_back(new NSPhraseMot(*(*iterMots))) ;
          else if ((AP_NOM == sPos) || (string("") == sPos))
            adjEpitheteAp.push_back(new NSPhraseMot(*(*iterMots))) ;
        }
      }
    }
    //
    // On met les adjectifs libres apres le nom.
    //
    else
      adjEpitheteAp.push_back(new NSPhraseMot(*(*iterMots))) ;
  }
}
catch (...)
{
  erreur("Exception NSPhraseur::classeAdjectif.", standardError) ;
}
}

//***************************************************************************
// Implementation des methodes NSPhraseMot
//***************************************************************************

//---------------------------------------------------------------------------
//  Constructeur par defaut
//---------------------------------------------------------------------------
NSPhraseMot::NSPhraseMot(NSContexte* pCtx)
            :NSRoot(pCtx)
{
	metAZero() ;
	_pComplement = (NSPhraseur*) 0 ;
}

//---------------------------------------------------------------------------
//  Met les champs de donnees a zero
//---------------------------------------------------------------------------
void
NSPhraseMot::metAZero()
{
  _sPreposition = string("") ;
  _sTexteLibre  = string("") ;
  _avecArticle  = articleSans ;
  _posForcee    = posStandard ;
  _numForme     = numSimple ;

  _sLexique     = string("") ;
  _sComplement  = string("") ;
  _sCertitude   = string("") ;
  _sPluriel     = string("") ;

  _sFormat      = string("") ;
  _sMethode     = string("") ;
  _sUnite       = string("") ;

  _iPriorite    = 50 ;  // standard priority
}

//---------------------------------------------------------------------------
//  Constructeur copie
//---------------------------------------------------------------------------
NSPhraseMot::NSPhraseMot(NSPhraseMot& rv)
            :NSRoot(rv.pContexte)
{
try
{
	_sPreposition = rv._sPreposition ;
	_sTexteLibre  = rv._sTexteLibre ;
	_avecArticle  = rv._avecArticle ;
	_posForcee    = rv._posForcee ;
	_numForme     = rv._numForme ;

  _sLexique     = rv._sLexique ;
  _sComplement  = rv._sComplement ;
  _sCertitude   = rv._sCertitude ;
  _sPluriel     = rv._sPluriel ;

  _sFormat      = rv._sFormat ;
  _sMethode     = rv._sMethode ;
  _sUnite       = rv._sUnite ;

  _iPriorite    = rv._iPriorite ;

	if (rv._pComplement)
  	_pComplement = new NSPhraseur(*(rv._pComplement)) ;
  else
  	_pComplement = (NSPhraseur*) 0 ;
}
catch (...)
{
	erreur("Exception NSPhraseMot copy ctor.", standardError) ;
}
}

/*
//---------------------------------------------------------------------------
//  Constructeur a partir d'un pointeur de PathoBase
//---------------------------------------------------------------------------
 NSPhraseMot::NSPhraseMot(NSPathoBaseData* pPathoBaseElement, NSContexte* pCtx)
            :NSRoot(pCtx)
{
	metAZero();

    strcpy(lexique,    pPathoBaseElement->lexique);
	strcpy(complement, pPathoBaseElement->complement);
	strcpy(certitude,  pPathoBaseElement->certitude);
	strcpy(pluriel,    pPathoBaseElement->pluriel);

    sTexteLibre = pPathoBaseElement->sTexteLibre;

    pComplement = 0;
}
*/

//---------------------------------------------------------------------------
//  Constructeur a partir d'un pointeur de PatPatho
//---------------------------------------------------------------------------

NSPhraseMot::NSPhraseMot(NSPatPathoData* pPatPathoElement, NSContexte* pCtx)
            :NSRoot(pCtx)
{
	metAZero() ;

  _sLexique    = pPatPathoElement->getLexique() ;
	_sComplement = pPatPathoElement->getComplement() ;
	_sCertitude  = pPatPathoElement->getCertitude() ;
  _sPluriel    = pPatPathoElement->getPluriel() ;
  _sUnite      = pPatPathoElement->getUnit() ;

  _sTexteLibre = pPatPathoElement->getTexteLibre() ;

  _pComplement = (NSPhraseur*) 0 ;
}


//---------------------------------------------------------------------------
//  Destructeur
//---------------------------------------------------------------------------
NSPhraseMot::~NSPhraseMot()
{
  if (_pComplement)
    delete _pComplement ;
}

//---------------------------------------------------------------------------
//  Operateur =
//---------------------------------------------------------------------------
NSPhraseMot&
NSPhraseMot::operator=(NSPhraseMot src)
{
try
{
	if (this == &src)
		return *this ;

	metAZero() ;

  _sPreposition = src._sPreposition ;
  _sTexteLibre  = src._sTexteLibre ;
  _avecArticle  = src._avecArticle ;
  _posForcee    = src._posForcee ;
  _numForme     = src._numForme ;

  _sLexique     = src._sLexique ;
  _sComplement  = src._sComplement ;
  _sCertitude   = src._sCertitude ;
  _sPluriel     = src._sPluriel ;

  _sFormat      = src._sFormat ;
  _sMethode     = src._sMethode ;
  _sUnite       = src._sUnite ;

  _iPriorite    = src._iPriorite ;

  if (NULL == _pComplement)
  {
  	delete _pComplement ;
    _pComplement = (NSPhraseur*) 0 ;
  }

  if (src._pComplement)
      _pComplement = new NSPhraseur(*(src._pComplement));

	return *this ;
}
catch (...)
{
	erreur("Exception NSPhraseMot = operator.", standardError) ;
	return *this ;
}
}

//---------------------------------------------------------------------------
//  Operateur de comparaison
//---------------------------------------------------------------------------
int
NSPhraseMot::operator==(const NSPhraseMot& o)
{
	if ((_sLexique    == o._sLexique)    &&
      (_sComplement == o._sComplement) &&
      (_sCertitude  == o._sCertitude)  &&
      (_sPluriel    == o._sPluriel)    &&
      (_sFormat     == o._sFormat)     &&
      (_sMethode    == o._sMethode)    &&
      (_sUnite      == o._sUnite)      &&
      (_sTexteLibre == o._sTexteLibre))
		return 1 ;
	else
		return 0 ;
}

void
NSPhraseMot::initFromNum(gereNum* pNum)
{
  if (NULL == pNum)
    return ;

  setComplement(pNum->getNum()) ;
  setUnite(pNum->getUnite()) ;
  setFormat(pNum->getFormat()) ;
}

void
NSPhraseMot::initComplement()
{
try
{
	if (_pComplement)
  	_pComplement->initialise() ;
  else
  	_pComplement = new NSPhraseur(pContexte) ;

	_pComplement->iPhraseType = NSPhraseur::phraseComplement ;
}
catch (...)
{
	erreur("Exception NSPhraseMot::initComplement.", standardError) ;
}
}

void
NSPhraseMot::setComplementPhr(NSPhraseur* pCompl)
{
  _pComplement = pCompl ;
  
  if (NULL == _pComplement)
    return ;

  _pComplement->iPhraseType = NSPhraseur::phraseComplement ;
}

bool
NSPhraseMot::estTexteLibre()
{
  return (string("£?????") == _sLexique) ;
}

string
NSPhraseMot::forceNombre(NSGenerateur* pGener)
{
  if (NULL == pGener)
    return string("") ;

  iterPhraseMot  iterMots ;
  NSPathologData Data ;
  NSSuper* pSuper = pContexte->getSuperviseur() ;

  // On regarde la certitude. Dans le cas d'un nom, une certitude WCE00 force le
  // singulier.

  string sCertitude  = _sCertitude ;

  string sLexiqueNom = _sLexique ;
  bool bNounFound = pContexte->getDico()->trouvePathologData(pGener->getLang(), &sLexiqueNom, &Data) ;
  if (false == bNounFound)
  {
    string sErrorText = string("NSPhraseMot::forceNombre Fail bacause ") + sLexiqueNom + string(" cannot be found in Lexique for langage ") + pGener->getLang() ;
    pSuper->trace(&sErrorText, 1, NSSuper::trWarning) ;
    return string("") ;
  }

  if (Data.estNom())
  {
    if (string("") != sCertitude)
    {
      if (string(sCertitude, 0, 5) == "WCE00")
        return FORCE_SING ;
    }
  }

  // On regarde les complements

  if (_pComplement)
  {
    if (_pComplement->adjNumeralCardinal.getComplement() != string(""))
    {
      gereNum Nnombre(pContexte->getSuperviseur(), pGener->getLang()) ;

      string sComplement = _pComplement->adjNumeralCardinal.getComplement() ;
      string sUnite      = _pComplement->adjNumeralCardinal.getUnite() ;
      string sFormat     = _pComplement->adjNumeralCardinal.getFormat() ;

      Nnombre.instancier(&sComplement, &sUnite, &sFormat) ;

      if (Nnombre.estExact())
      {
        double dRetour = Nnombre.getValeur() ;
        if (dRetour < 2)
          return FORCE_SING ;
        return FORCE_PLUR ;
      }
    }

    // On verifie si les adjectifs forcent le nombre

    if (false == (_pComplement->adjEpithete).empty())
    {
      for (iterMots = (_pComplement->adjEpithete).begin() ;
                 iterMots != (_pComplement->adjEpithete).end() ;
                 iterMots++)
      {
        string sLexique = (*iterMots)->getLexique() ;
        bool trouve = pContexte->getDico()->trouvePathologData(pGener->getLang(), &sLexique, &Data) ;

        if (trouve)
        {
          string sRetour = Data.chercheGrammaire(FORCE_NBR) ;
          if (string("") != sRetour)
            return sRetour ;
        }
      }
    }
    // On regarde les CCChiffre

    if (false == (_pComplement->CCChiffre).empty())
    {
      for (iterMots = (_pComplement->CCChiffre).begin() ;
                 iterMots != (_pComplement->CCChiffre).end() ;
                 iterMots++)
      {
        string sLexique = (*iterMots)->getLexique() ;
        bool trouve = pContexte->getDico()->trouvePathologData(pGener->getLang(), &sLexique, &Data) ;

        if (trouve)
        {
          string sRetour = Data.chercheGrammaire(FORCE_NBR);
          if      (FORCE_PLUR == sRetour)
            return FORCE_PLUR ;
          else if (FORCE_SING == sRetour)
            return FORCE_SING ;
          else if (FORCE_X    == sRetour)
          {
            gereNum Nnombre(pContexte->getSuperviseur(), pGener->getLang()) ;

            string sComplement = _pComplement->adjNumeralCardinal.getComplement() ;
            string sUnite      = _pComplement->adjNumeralCardinal.getUnite() ;
            string sFormat     = _pComplement->adjNumeralCardinal.getFormat() ;

            Nnombre.instancier(&sComplement, &sUnite, &sFormat);
            if (Nnombre.estExact())
            {
              double dRetour = Nnombre.getValeur() ;
              if (dRetour < 2)
                return FORCE_SING ;
              return FORCE_PLUR ;
            }

            if (Nnombre.estInf())
            {
              double dRetour = Nnombre.getValeurInf() ;
              if (dRetour >= 2)
                return FORCE_PLUR ;
            }

            if (Nnombre.estSup())
            {
              double dRetour = Nnombre.getValeurSup() ;
              if (dRetour >= 2)
                return FORCE_PLUR ;
              else if (dRetour < 2)
                return FORCE_SING ;
            }
          }
        }
      }
    }
  }
  return string("") ;
}

//***************************************************************************
// Implementation des methodes NSPhraseMotTime
//***************************************************************************

//---------------------------------------------------------------------------
//  Constructeur par defaut
//---------------------------------------------------------------------------
NSPhraseMotTime::NSPhraseMotTime(NSContexte* pCtx)
                :NSPhraseMot(pCtx)
{
	metAZero() ;
  _pComplement = (NSPhraseur*) 0 ;
}

//---------------------------------------------------------------------------
//  Met les champs de donnees a zero
//---------------------------------------------------------------------------
void
NSPhraseMotTime::metAZero()
{
  NSPhraseMot::metAZero() ;

  memset(valeurMin,   0, BASE_COMPLEMENT_LEN + 1) ;
  memset(uniteMin,    0, BASE_LEXIQUE_LEN + 1) ;
  memset(formatMin,   0, BASE_LEXIQUE_LEN + 1) ;
  memset(valeurMax,   0, BASE_COMPLEMENT_LEN + 1) ;
  memset(uniteMax,    0, BASE_LEXIQUE_LEN + 1) ;
  memset(formatMax,   0, BASE_LEXIQUE_LEN + 1) ;
  memset(valeurDuree, 0, BASE_COMPLEMENT_LEN + 1) ;
  memset(uniteDuree,  0, BASE_LEXIQUE_LEN + 1) ;
  memset(formatDuree, 0, BASE_LEXIQUE_LEN + 1) ;

  bMinNow = false ;
  bMaxNow = false ;
}

//---------------------------------------------------------------------------
//  Constructeur copie
//---------------------------------------------------------------------------
NSPhraseMotTime::NSPhraseMotTime(NSPhraseMotTime& rv)
                :NSPhraseMot((NSPhraseMot&)rv)
{
  strcpy(valeurMin,   rv.valeurMin) ;
	strcpy(uniteMin,    rv.uniteMin) ;
  strcpy(formatMin,   rv.formatMin) ;
	strcpy(valeurMax,   rv.valeurMax) ;
	strcpy(uniteMax,    rv.uniteMax) ;
  strcpy(formatMax,   rv.formatMax) ;
  strcpy(valeurDuree, rv.valeurDuree) ;
  strcpy(uniteDuree,  rv.uniteDuree) ;
  strcpy(formatDuree, rv.formatDuree) ;

  bMinNow = rv.bMinNow ;
  bMaxNow = rv.bMaxNow ;
}

//---------------------------------------------------------------------------
//  Constructeur a partir d'un pointeur de PatPatho
//---------------------------------------------------------------------------

NSPhraseMotTime::NSPhraseMotTime(NSPatPathoData* pPatPathoElement, NSContexte* pCtx)
                :NSPhraseMot(pPatPathoElement, pCtx)
{
  memset(valeurMin,   0, BASE_COMPLEMENT_LEN + 1) ;
  memset(uniteMin,    0, BASE_LEXIQUE_LEN + 1) ;
  memset(formatMin,   0, BASE_LEXIQUE_LEN + 1) ;
  memset(valeurMax,   0, BASE_COMPLEMENT_LEN + 1) ;
  memset(uniteMax,    0, BASE_LEXIQUE_LEN + 1) ;
  memset(formatMax,   0, BASE_LEXIQUE_LEN + 1) ;
  memset(valeurDuree, 0, BASE_COMPLEMENT_LEN + 1) ;
  memset(uniteDuree,  0, BASE_LEXIQUE_LEN + 1) ;
  memset(formatDuree, 0, BASE_LEXIQUE_LEN + 1) ;

  bMinNow = false ;
  bMaxNow = false ;
}

//---------------------------------------------------------------------------
//  Destructeur
//---------------------------------------------------------------------------
NSPhraseMotTime::~NSPhraseMotTime()
{
}

//---------------------------------------------------------------------------
//  Operateur =
//---------------------------------------------------------------------------
NSPhraseMotTime&
NSPhraseMotTime::operator=(NSPhraseMotTime src)
{
  if (this == &src)
		return *this ;

  NSPhraseMot* pMot = (NSPhraseMot*) this ;
  *pMot = (NSPhraseMot&) src ;

  strcpy(valeurMin,   src.valeurMin) ;
	strcpy(uniteMin,    src.uniteMin) ;
  strcpy(formatMin,   src.formatMin) ;
	strcpy(valeurMax,   src.valeurMax) ;
	strcpy(uniteMax,    src.uniteMax) ;
  strcpy(formatMax,   src.formatMax) ;
  strcpy(valeurDuree, src.valeurDuree) ;
  strcpy(uniteDuree,  src.uniteDuree) ;
  strcpy(formatDuree, src.formatDuree) ;

  bMinNow = src.bMinNow ;
  bMaxNow = src.bMaxNow ;

	return *this ;
}

//---------------------------------------------------------------------------
//  Operateur de comparaison
//---------------------------------------------------------------------------
int
NSPhraseMotTime::operator==(const NSPhraseMotTime& o)
{
  NSPhraseMot* pMot = (NSPhraseMot*) this ;
  if (!(*pMot == (NSPhraseMot&) o))
    return 0 ;

	if ((strcmp(valeurMin,  o.valeurMin)    == 0) &&
        (strcmp(uniteMin,   o.uniteMin)     == 0) &&
        (strcmp(formatMin,  o.formatMin)    == 0) &&
        (strcmp(valeurMax,  o.valeurMax)    == 0) &&
        (strcmp(uniteMax,   o.uniteMax)     == 0) &&
        (strcmp(formatMax,  o.formatMax)    == 0) &&
        (strcmp(valeurDuree, o.valeurDuree) == 0) &&
        (strcmp(uniteDuree,  o.uniteDuree)  == 0) &&
        (strcmp(formatDuree, o.formatDuree) == 0) &&
        (bMinNow == o.bMinNow) &&
        (bMaxNow == o.bMaxNow))
		return 1 ;
	else
		return 0 ;
}

void
NSPhraseMotTime::setSharpDate()
{
  strcpy(valeurMax, valeurMin) ;
  strcpy(uniteMax,  uniteMin) ;
  strcpy(formatMax, formatMin) ;
}

bool
NSPhraseMotTime::minEgalMax()
{
  if ((strcmp(valeurMax, valeurMin) == 0) &&
      (strcmp(uniteMax,  uniteMin)  == 0) &&
      (strcmp(formatMax, formatMin) == 0))
    return true ;

  return false ;
}

bool
NSPhraseMotTime::estVide()
{
  if ((valeurMin[0] == '\0') && (valeurMax[0] == '\0') && (valeurDuree[0] == '\0'))
    return true ;
  return false ;
}

//***************************************************************************
// Implementation des methodes NSPhraseMotTimeCycle
//***************************************************************************

//---------------------------------------------------------------------------
//  Constructeur par defaut
//---------------------------------------------------------------------------
NSPhraseMotTimeCycle::NSPhraseMotTimeCycle(NSContexte* pCtx)
                     :NSPhraseMot(pCtx)
{
	metAZero() ;
  _pComplement = (NSPhraseur*) 0 ;
}

//---------------------------------------------------------------------------
//  Met les champs de donnees a zero
//---------------------------------------------------------------------------
void
NSPhraseMotTimeCycle::metAZero()
{
  NSPhraseMot::metAZero() ;

  iCycleType = CycNotInit ;

  memset(cycleDurationValue,   0, BASE_COMPLEMENT_LEN + 1) ;
  memset(cycleDurationUnit,    0, BASE_LEXIQUE_LEN + 1) ;
  memset(cycleDurationFormat,  0, BASE_LEXIQUE_LEN + 1) ;

  memset(actionDurationValue,  0, BASE_COMPLEMENT_LEN + 1) ;
  memset(actionDurationUnit,   0, BASE_LEXIQUE_LEN + 1) ;
  memset(actionDurationFormat, 0, BASE_LEXIQUE_LEN + 1) ;

  memset(numberOfAction,       0, BASE_COMPLEMENT_LEN + 1) ;
}

//---------------------------------------------------------------------------
//  Constructeur copie
//---------------------------------------------------------------------------
NSPhraseMotTimeCycle::NSPhraseMotTimeCycle(NSPhraseMotTimeCycle& rv)
                     :NSPhraseMot((NSPhraseMot&)rv)
{
  iCycleType = rv.iCycleType ;

	strcpy(cycleDurationValue,      rv.cycleDurationValue) ;
	strcpy(cycleDurationUnit,       rv.cycleDurationUnit) ;
  strcpy(cycleDurationFormat,     rv.cycleDurationFormat) ;

	strcpy(actionDurationValue,     rv.actionDurationValue) ;
	strcpy(actionDurationUnit,      rv.actionDurationUnit) ;
  strcpy(actionDurationFormat,    rv.actionDurationFormat) ;

  strcpy(numberOfAction,          rv.numberOfAction) ;
}

//---------------------------------------------------------------------------
//  Constructeur a partir d'un pointeur de PatPatho
//---------------------------------------------------------------------------

NSPhraseMotTimeCycle::NSPhraseMotTimeCycle(NSPatPathoData* pPatPathoElement, NSContexte* pCtx)
                     :NSPhraseMot(pPatPathoElement, pCtx)
{
	metAZero() ;
}


//---------------------------------------------------------------------------
//  Destructeur
//---------------------------------------------------------------------------
NSPhraseMotTimeCycle::~NSPhraseMotTimeCycle()
{
}

//---------------------------------------------------------------------------
//  Operateur =
//---------------------------------------------------------------------------
NSPhraseMotTimeCycle&
NSPhraseMotTimeCycle::operator=(NSPhraseMotTimeCycle src)
{
  if (this == &src)
		return *this ;

  NSPhraseMot* pMot = (NSPhraseMot*) this ;
  *pMot = (NSPhraseMot&) src ;

  iCycleType = src.iCycleType ;

	strcpy(cycleDurationValue,   src.cycleDurationValue) ;
	strcpy(cycleDurationUnit,    src.cycleDurationUnit) ;
  strcpy(cycleDurationFormat,  src.cycleDurationFormat) ;

	strcpy(actionDurationValue,  src.actionDurationValue) ;
	strcpy(actionDurationUnit,   src.actionDurationUnit) ;
  strcpy(actionDurationFormat, src.actionDurationFormat) ;

  strcpy(numberOfAction,       src.numberOfAction) ;

	return *this;
}

//---------------------------------------------------------------------------
//  Operateur de comparaison
//---------------------------------------------------------------------------
int
NSPhraseMotTimeCycle::operator==(const NSPhraseMotTimeCycle& o)
{
  NSPhraseMot* pMot = (NSPhraseMot*) this ;
  if (!(*pMot == (NSPhraseMot&) o))
    return 0 ;

	if ((strcmp(cycleDurationValue,     o.cycleDurationValue)   == 0) &&
        (strcmp(cycleDurationUnit,      o.cycleDurationUnit)    == 0) &&
        (strcmp(cycleDurationFormat,    o.cycleDurationFormat)  == 0) &&
        (strcmp(actionDurationValue,    o.actionDurationValue)  == 0) &&
        (strcmp(actionDurationUnit,     o.actionDurationUnit)   == 0) &&
        (strcmp(actionDurationFormat,   o.actionDurationFormat) == 0) &&
        (strcmp(numberOfAction,         o.numberOfAction)       == 0) &&
        (iCycleType == o.iCycleType))
		return 1 ;
	else
		return 0 ;
}

// *************************************************************************
// -------------------- METHODES DE NSPhraseMotArray ------------------------
// **************************************************************************

//---------------------------------------------------------------------------
//  Constructeur copie
//---------------------------------------------------------------------------
NSPhraseMotArray::NSPhraseMotArray(NSPhraseMotArray& rv)
                 :NSPhraMotArray()
{
try
{
  if (rv.empty())
    return ;

  for (iterPhraseMot i = rv.begin(); i != rv.end(); i++)
  {
    NSPhraseMotTime* pDate = dynamic_cast<NSPhraseMotTime*>(*i) ;
    if ( pDate )
    {
      NSPhraseMotTime* pNewDate = new NSPhraseMotTime(*pDate) ;
      push_back(pNewDate) ;
    }
    else
    {
      NSPhraseMotTimeCycle* pCycle = dynamic_cast<NSPhraseMotTimeCycle*>(*i) ;
      if ( pCycle )
      {
        NSPhraseMotTimeCycle *pNewCycle = new NSPhraseMotTimeCycle(*pCycle) ;
        push_back(pNewCycle) ;
      }
      else
        push_back(new NSPhraseMot(*(*i))) ;
    }
  }
}
catch (...)
{
	erreur("Exception NSPhraseMotArray copy ctor.", standardError) ;
}
}

//---------------------------------------------------------------------------
//  Destructeur
//---------------------------------------------------------------------------
void
NSPhraseMotArray::vider()
{
  if (empty())
    return ;

  for (iterPhraseMot i = begin(); i != end(); )
  {
    delete *i ;
    erase(i) ;
  }
}

NSPhraseMotArray::~NSPhraseMotArray()
{
  vider() ;
}

//---------------------------------------------------------------------------
//  Operateur d'affectation
//---------------------------------------------------------------------------
NSPhraseMotArray&
NSPhraseMotArray::operator=(NSPhraseMotArray src)
{
try
{
	if (this == &src)
		return *this ;

	//
  // Effacement des elements dejà contenus dans le vecteur destination
  //
  vider() ;
	//
  // Copie et insertion des elements de la source
  //
  if (false == src.empty())
  {
  	for (iterPhraseMot i = src.begin(); i != src.end(); i++)
    {
    	NSPhraseMotTime* pDate = dynamic_cast<NSPhraseMotTime*>(*i) ;
      if ( pDate )
      {
        NSPhraseMotTime* pNewDate = new NSPhraseMotTime(*pDate) ;
      	push_back(pNewDate) ;
      }
      else
      {
      	NSPhraseMotTimeCycle* pCycle = dynamic_cast<NSPhraseMotTimeCycle*>(*i) ;
        if ( pCycle )
        {
          NSPhraseMotTimeCycle* pNewCycle = new NSPhraseMotTimeCycle(*pCycle) ;
        	push_back(pNewCycle) ;
        }
        else
        	push_back(new NSPhraseMot(*(*i))) ;
      }
    }
  }

	return *this ;
}
catch (...)
{
	erreur("Exception NSPhraseMotArray = operator.", standardError) ;
	return *this ;
}
}

// Pour vider toutes les prepositions

void
NSPhraseMotArray::initPreposition()
{
  if (empty())
    return ;

  iterPhraseMot iterMots ;

  for (iterMots = begin(); iterMots != end(); iterMots++)
  {
    (*iterMots)->setPreposition(string("")) ;
    if (NULL != (*iterMots)->getComplementPhr())
      (*iterMots)->getComplementPhr()->nettoye() ;
  }
}


// *************************************************************************
// --------------------- METHODES DE NsProposition -------------------------
// *************************************************************************

//---------------------------------------------------------------------------
//  Constructeur copie
//---------------------------------------------------------------------------
NsProposition::NsProposition(NSContexte* pCtx)
              :NSRoot(pCtx)
{
  iPropositionType = notSetType ;
  iConjonctionType = notSetConjonct ;
  iObjectType      = notSetObjType ;
  pProposition     = 0 ;
  bDeleteAtEnd     = true ;

  sPhrase          = "" ;
  sLang            = "" ;

  pPhraDelAtEnd    = 0 ;
}

NsProposition::NsProposition(NSContexte* pCtx, NSPhraseur** pPhrase, PROPOSITIONTYPE iPropType, CONJONCTIONTYPE iConjonct, bool bDelAtEnd)
              :NSRoot(pCtx)
{
  iPropositionType = iPropType ;
  iConjonctionType = iConjonct ;

  iObjectType      = isPhraseur ;
  pProposition     = *pPhrase ;
  bDeleteAtEnd     = bDelAtEnd ;

  sPhrase          = "" ;
  sLang            = "" ;

  if (bDeleteAtEnd)
    pPhraDelAtEnd = pPhrase ;
  else
    pPhraDelAtEnd = 0 ;
}

NsProposition::NsProposition(NSContexte* pCtx, NSPhraseur* pPhrase, PROPOSITIONTYPE iPropType, CONJONCTIONTYPE iConjonct)
              :NSRoot(pCtx)
{
  iPropositionType = iPropType ;
  iConjonctionType = iConjonct ;

  iObjectType      = isPhraseur ;
  pProposition     = pPhrase ;

  sPhrase          = "" ;
  sLang            = "" ;

  bDeleteAtEnd     = false ;
  pPhraDelAtEnd    = 0 ;
}

NsProposition::NsProposition(NsProposition& rv)
              :NSRoot(rv.pContexte)
{
try
{
  iPropositionType = rv.iPropositionType ;
  iConjonctionType = rv.iConjonctionType ;

  sPhrase          = rv.sPhrase ;
  sLang            = rv.sLang ;

  pPhraDelAtEnd    = 0 ;

  if (rv.pProposition)
  {
    if (rv.bDeleteAtEnd)
    {
      if      (rv.iObjectType == isPhraseur)
      {
        NSPhraseur* pPhraseur = (static_cast<NSPhraseur*>(rv.pProposition)) ;
        pProposition = new NSPhraseur(*pPhraseur) ;
        iObjectType  = isPhraseur ;
      }
      else if (rv.iObjectType == isPropositionArray)
      {
        NSPropositionArray* pPropArray = (static_cast<NSPropositionArray*>(rv.pProposition)) ;
        pProposition = new NSPropositionArray(*pPropArray) ;
        iObjectType  = isPropositionArray ;
      }
      bDeleteAtEnd = true ;
    }
    else
    {
      pProposition = rv.pProposition ;
      iObjectType  = rv.iObjectType ;
      bDeleteAtEnd = false ;
    }
  }
  else
  {
    iObjectType  = notSetObjType ;
    pProposition = 0 ;
    bDeleteAtEnd = true ;
  }
}
catch (...)
{
	erreur("Exception NsProposition copy ctor.", standardError) ;
}
}

NsProposition::~NsProposition()
{
	if (pProposition && bDeleteAtEnd)
  {
  	if      (iObjectType == isPhraseur)
    {
    	NSPhraseur* pPhraseur = (static_cast<NSPhraseur*>(pProposition)) ;
      if (NULL != pPhraseur)
        delete pPhraseur ;

      if (pPhraDelAtEnd)
      	*pPhraDelAtEnd = NULL ;
    }
    else if (iObjectType == isPropositionArray)
    {
    	NSPropositionArray* pPropArray = (static_cast<NSPropositionArray*>(pProposition)) ;
      if (NULL != pPropArray)
        delete pPropArray ;
      pPropArray = 0 ;
    }
    pProposition = 0 ;
  }
}

NsProposition&
NsProposition::operator=(NsProposition src)
{
try
{
	if (this == &src)
		return *this ;

	iPropositionType    = src.iPropositionType ;
  iConjonctionType    = src.iConjonctionType ;

  sPhrase             = src.sPhrase ;
  sLang               = src.sLang ;

  pPhraDelAtEnd       = 0 ;

  if (src.pProposition)
  {
  	if (src.bDeleteAtEnd)
    {
    	if      (src.iObjectType == isPhraseur)
      {
      	NSPhraseur* pPhraseur = (static_cast<NSPhraseur*>(src.pProposition)) ;
        pProposition = new NSPhraseur(*pPhraseur) ;
        iObjectType  = isPhraseur ;
      }
      else if (src.iObjectType == isPropositionArray)
      {
      	NSPropositionArray* pPropArray = (static_cast<NSPropositionArray*>(src.pProposition)) ;
        pProposition = new NSPropositionArray(*pPropArray) ;
        iObjectType  = isPropositionArray ;
      }
      bDeleteAtEnd = true ;
    }
    else
    {
    	pProposition = src.pProposition ;
      iObjectType  = src.iObjectType ;
      bDeleteAtEnd = false ;
    }
  }
  else
  {
  	iObjectType  = notSetObjType ;
    pProposition = 0 ;
  }

	return *this ;
}
catch (...)
{
	erreur("Exception NsProposition = operator.", standardError) ;
	return *this ;
}
}

// *************************************************************************
// -------------------- METHODES DE NSPropositionArray ------------------------
// *************************************************************************

//---------------------------------------------------------------------------
//  Constructeur copie
//---------------------------------------------------------------------------
/*
NSPropositionArray::NSPropositionArray(NSPropositionArray& rv)
                   :NSPropositionVector()
{
try
{
    if (rv.empty())
        return;
    for (iterProposition i = rv.begin(); i != rv.end();i++)
   	    push_back(new NsProposition(*(*i)));
}
catch (...)
{
    erreur("Exception NSPropositionArray copy ctor.", 0, 0);
}
}

//---------------------------------------------------------------------------
//  Destructeur
//---------------------------------------------------------------------------
void
NSPropositionArray::vider()
{
    if (empty())
        return;
    for (iterProposition i = begin(); i != end(); )
    {
   	    delete *i;
        erase(i);
    }
}

NSPropositionArray::~NSPropositionArray()
{
    vider();
}

//---------------------------------------------------------------------------
//  Operateur d'affectation
//---------------------------------------------------------------------------
NSPropositionArray&
NSPropositionArray::operator=(NSPropositionArray src)
{
try
{
    //
    // Effacement des elements deja contenus dans le vecteur destination
    //
    vider();
	//
    // Copie et insertion des elements de la source
    //
    if (!(src.empty()))
  	    for (iterProposition i = src.begin(); i != src.end(); i++)
   	        push_back(new NsProposition(*(*i)));
    return *this;
}
catch (...)
{
    erreur("Exception NSPropositionArray = operator.", 0, 0);
    return *this ;
}
} */

