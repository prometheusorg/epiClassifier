#ifndef __NSPHRASE_H
#define __NSPHRASE_H

class NSPhraseur ;
class NSGenerateur ;

class NSPatPathoData ;

class gereNum ;

#ifdef _ENTERPRISE_DLL
  #include "enterpriseLus/nsglobalLus.h"
#else
  #include "nsglobal.h"
#endif

#include "partage/nsvector.h"
#include "nssavoir/nspatbas.h"
#include "partage/ntarray.h"

class NSPhraseMot : public NSRoot
{
  public :

  enum TYPEARTICLE { articleSans = 1, articleDefini, articleIndefini,
                                                            articlePartitif} ;

  enum FORCEPOSITION { posStandard = 1, posAvant, posApres } ;

  enum NUMFORME { numTiret = 1, numSimple, numRelative } ;

  protected :

    // Les preposition "non traduisibles" comme EN decubitus ou A velo
    // n'ont pas d'interet a etre traitees avec le lexique puisqu'elles
    // sont artificielles
    string _sPreposition ;
    //
    // "dans la rue" mais "en decubitus"
    //

    // On a une string pour les textes libres.
    string _sTexteLibre ;


    TYPEARTICLE _avecArticle ;

    //
    // Pour forcer la position d'un adjectif par rapport au nom.

    FORCEPOSITION _posForcee ;

    //
    // Variables de stockage des valeurs
    //
    // char lexique[BASE_LEXIQUE_LEN + 1] ;
    // char complement[BASE_COMPLEMENT_LEN + 1] ;
    // char certitude[BASE_CERTITUDE_LEN + 1] ;
    // char pluriel[BASE_PLURIEL_LEN + 1] ;

    string _sLexique ;
    string _sComplement ;
    string _sCertitude ;
    string _sPluriel ;

    // Pour les valeurs chiffrees
    // char format[BASE_LEXIQUE_LEN + 1] ;
    // char methode[BASE_LEXIQUE_LEN + 1] ;
    // char unite[BASE_LEXIQUE_LEN + 1] ;

    string _sFormat ;
    string _sMethode ;
    string _sUnite ;

    NUMFORME _numForme ;

    // Complément de nom
    //
    NSPhraseur* _pComplement ;

    // Priorite de cet element dans la phrase
    //
    int _iPriorite ;

  public :

    NSPhraseMot(NSContexte* pCtx) ;
    NSPhraseMot(NSPhraseMot& rv) ;
    // NSPhraseMot(NSPathoBaseData* pPathoBaseElement, NSContexte* pCtx);
    NSPhraseMot(NSPatPathoData* pPatPathoElement, NSContexte* pCtx) ;
    ~NSPhraseMot() ;

    void initFromNum(gereNum* pNum) ;

    void setPreposition(string sPrep)     { _sPreposition = sPrep ; }
    void setArticle(TYPEARTICLE iArticle) { _avecArticle = iArticle ; }
    void setLexique(string sLexi)         { _sLexique = sLexi ; }
    void setComplement(string sCompl)     { _sComplement = sCompl ; }
    void setCertitude(string sCert)       { _sCertitude = sCert ; }
    void setPluriel(string sPlur)         { _sPluriel = sPlur ; }
    void setFormat(string sForm)          { _sFormat = sForm ; }
    void setMethode(string sMetho)        { _sMethode = sMetho ; }
    void setUnite(string sUnit)           { _sUnite = sUnit ; }
    void setPosition(FORCEPOSITION iPos)  { _posForcee = iPos ; }
    void setTexteLibre(string sTexte)     { _sTexteLibre = sTexte ; }
    void setNumForme(NUMFORME iforme)     { _numForme = iforme ; }
    void setPriority(int iPrior)          { _iPriorite = iPrior ; }

    string        getPreposition() { return _sPreposition ; }
    TYPEARTICLE   getArticle()     { return _avecArticle ; }
    string        getLexique()     { return _sLexique ; }
    string        getComplement()  { return _sComplement ; }
    string        getCertitude()   { return _sCertitude ; }
    string        getPluriel()     { return _sPluriel ; }
    string        getFormat()      { return _sFormat ; }
    string        getMethode()     { return _sMethode ; }
    string        getUnite()       { return _sUnite ; }
    FORCEPOSITION getPosition()    { return _posForcee ; }
    string        getTexteLibre()  { return _sTexteLibre ; }
    int           getNumForme()    { return _numForme ; }
    int           getPriority()    { return _iPriorite ; }

    void          setComplementPhr(NSPhraseur* pCompl) ;
    NSPhraseur*   getComplementPhr()                   { return _pComplement ; }

    bool         estTexteLibre() ;

    string       forceNombre(NSGenerateur* pGener) ;

    NSPhraseMot& operator=(NSPhraseMot src) ;
    int          operator==(const NSPhraseMot& o) ;

    virtual void metAZero() ;
    void         initComplement() ;

    bool         estVide() ;
};

enum TIME_EVENT { TpsTypeUndefined = 0, TpsDuration, TpsDate } ;  // Dure 2h ou Dans 2h
enum TIME_FORM  { TpsFormUndefined = 0, TpsInstant, TpsInterval } ; //
enum TIME_FRAME { TpsFrameUndefined = 0, TpsPresent, TpsPast, TpsFutur } ;

class NSPhraseMotTime : public NSPhraseMot
{
  public :

    //
    // Duree ou Date : pendant 2 heures vs dans 2 heures
    // Duration or date : lasting 2 hours vs starting 2 hours from now
    //
    // Pour une date precise, alors mettre valeurMin == valeurMax
    // For a sharp date, then set valeurMin == valeurMax
    //
    TIME_EVENT iTypeTps ;

    //
    // Temps ponctuel ou intervalle de temps
    // Point or Duration
    //
    TIME_FORM iFormeTps ;

    //
    // Now, in the past or in the future
    //
    TIME_FRAME iRepererTps ;

    //
    // Time position related to an event (event = lexique)
    //
    enum TIME_POSITION { TpsPositionNotDefined = 0,
                         EventFarBefore, EventBefore, EventJustBefore,
                         EventDuring,
                         EventJustAfter, EventAfter, EventFarAfter,
                         EventFarFrom } ;
    TIME_POSITION iEventTps ;

    //
    // Min time
    //
    char valeurMin[BASE_COMPLEMENT_LEN + 1] ; // Valeur min - Min Value
    char uniteMin[BASE_LEXIQUE_LEN + 1] ;     // Unite min  - Min value unit
    char formatMin[BASE_LEXIQUE_LEN + 1] ;
    //
    // Le temps min est "maintenant"    : de maintenant a plus tard
    // Min time is "now"                : from now to max time
    //
    bool bMinNow ;

    //
    // Max time
    //
    char valeurMax[BASE_COMPLEMENT_LEN + 1] ; // Valeur max  - Max Value
    char uniteMax[BASE_LEXIQUE_LEN + 1] ;     // Unite max   - Max value unit
    char formatMax[BASE_LEXIQUE_LEN + 1] ;
    //
    // Le temps max est "maintenant"    : d'une date a maintenant
    // Max time is "now"                : from min time to now
    //
    bool bMaxNow ;

    //
    // Duree (3 mois...)
    //
    char valeurDuree[BASE_COMPLEMENT_LEN + 1] ; // Valeur max  - Max Value
    char uniteDuree[BASE_LEXIQUE_LEN + 1] ;     // Unite max   - Max value unit
    char formatDuree[BASE_LEXIQUE_LEN + 1] ;


    NSPhraseMotTime(NSContexte* pCtx) ;
    NSPhraseMotTime(NSPhraseMotTime& rv) ;
    // NSPhraseMot(NSPathoBaseData* pPathoBaseElement, NSContexte* pCtx);
    NSPhraseMotTime(NSPatPathoData* pPatPathoElement, NSContexte* pCtx) ;
    ~NSPhraseMotTime() ;

    NSPhraseMotTime& operator=(NSPhraseMotTime src) ;
    int 		     operator==(const NSPhraseMotTime& o) ;

    void metAZero() ;
    void setSharpDate() ;
    bool minEgalMax() ;
    bool estVide() ;
};

class NSPhraseMotTimeCycle : public NSPhraseMot
{
	public :

		//
    // Cycle regulier ou libre :
    // Regulier : |XXX_____|XXX_____|XXX_____ : duree cycle + duree action
    // Libre    : |XX__XX__|XX_XX___|_XX__XX_ : duree cycle + duree action + nb actions dans un cycle
    //
    // Regular or free cycle :
    // Regular : |XXX_____|XXX_____|XXX_____ : cycle duration + action duration
    // Free    : |XX__XX__|XX_XX___|_XX__XX_ : cycle duration + action duration + nb of actions in a cycle
    //
    enum CYCLETYPE { CycNotInit = 0, CycRegular, CycFree } ;
    CYCLETYPE iCycleType ;

    //
    // Cycle duration time
    //
    char cycleDurationValue[BASE_COMPLEMENT_LEN + 1] ;
    char cycleDurationUnit[BASE_LEXIQUE_LEN + 1] ;
    char cycleDurationFormat[BASE_LEXIQUE_LEN + 1] ;
    //
    // Action duration time
    //
    char actionDurationValue[BASE_COMPLEMENT_LEN + 1] ;
    char actionDurationUnit[BASE_LEXIQUE_LEN + 1] ;
    char actionDurationFormat[BASE_LEXIQUE_LEN + 1] ;
    //
    // Action duration time
    //
    char numberOfAction[BASE_COMPLEMENT_LEN + 1] ;

    NSPhraseMotTimeCycle(NSContexte* pCtx) ;
    NSPhraseMotTimeCycle(NSPhraseMotTimeCycle& rv) ;
    // NSPhraseMot(NSPathoBaseData* pPathoBaseElement, NSContexte* pCtx);
    NSPhraseMotTimeCycle(NSPatPathoData* pPatPathoElement, NSContexte* pCtx) ;
    ~NSPhraseMotTimeCycle() ;

    NSPhraseMotTimeCycle& operator=(NSPhraseMotTimeCycle src) ;
    int                   operator==(const NSPhraseMotTimeCycle& o) ;

    void metAZero() ;
    bool estVide() ;
};

typedef vector<NSPhraseMot*> NSPhraMotArray ;
typedef NSPhraMotArray::iterator iterPhraseMot ;

class NSPhraseMotArray : public NSPhraMotArray
{
  public :

    // Constructeurs
    NSPhraseMotArray() : NSPhraMotArray() {}
    NSPhraseMotArray(NSPhraseMotArray& rv) ;
    // Destructeur
    virtual ~NSPhraseMotArray() ;
    NSPhraseMotArray& operator=(NSPhraseMotArray src) ;
    //
    void vider() ;

    void initPreposition() ;
};

class NSPhraseur : public NSRoot
{
  public :

    enum TYPEARTICLE { articleSans = 1, articleDefini, articleIndefini} ;

    // Qualification de la localisation (CC de lieu)
    // Caractéristique descriptive : la longueur "de la" jambe est de x cm
    // Localisation niveau : il existe un polype "au niveau du" côlon
    // Localisation à : il existe un ulcère "à" la jambe droite
    //
    enum TYPELOCALISATION { locUndefined = 0, locCaractDescr, locNiveau, locA} ;

    // Phrase principale ou complement d'un nom
    enum PHRTYPE { phrasePrincipale = 1, phraseComplement, phraseRelative} ;
    PHRTYPE iPhraseType ;

    // Forme active ou passive
    enum PHRFORME { formeActive = 1, formePassive } ;
    PHRFORME iForme ;

    NSPhraseMotArray Sujet ;

    // Sujet normal ou remplace par un article
    enum SUJTYPE { sujetNormal = 1, sujetNoRepeat } ;
    SUJTYPE iTypeSujet;

    NSPhraseMotArray Verbe ;
    // Caracteristiques du verbe

    enum VBTYPE { vbTypeNotInit = 0, vbTypeAction, vbTypeEtat} ;
    enum VBETAT { vbEtatNotInit = 0, vbEtatReel, vbEtatApparent, vbEtatDure, vbEtatChange} ;
    VBTYPE iVbType ;
    VBETAT iVbEtat ;

    // Emploi du verbe : temps, aspect, mode, personne, negation
    enum VBASPEC { aspectPonctuel = 1, aspectIntervalle} ;
    enum VBTEMPS { tempsPasseRevolu = 1, tempsPasseActuel,
                   tempsPresent, tempsFuturProche, tempsFuturLointain} ;
    enum VBMODE  { modeIndicatif = 1, modeSubjonctif, modeImperatif,
                   modeConditionnel, modeInfinitif, modeParticipe,
                   modeGerondif} ;
    enum VBPERSO { pers1S = 1, pers2S, pers3S, pers1P, pers2P, pers3P} ;
    enum VBNEGAT { negationOui = 1, negationNon} ;
    enum VBPREPOCTX { prepUndefined = 0, prepLieu = 1, prepTemps, prepManiere, prepMoyen,
              prepCause, prepBut, prepType, prepHypoth, prepChiffre} ;
    VBTEMPS iVbTemps ;
    VBASPEC iVbAspect ;
    VBMODE  iVbMode ;
    VBPERSO iVbPersonne ;
    VBNEGAT iVbNegation ;

    // Explications concernant un mot entre parentheses.

    NSPhraseur* pParentheses ;

    // Complements essentiels
    NSPhraseMotArray    COD ;       // Complement d'Objet Direct
    NSPhraseMotArray    COI ;       // Complement d'Objet Indirect
    NSPhraseMotArray    COS ;       // Complement d'Objet Second
    NSPhraseMotArray    AttSujet ;  // Attribut du sujet
                                    // "L'homme paraissait fatigue"
                                    // "Sa demarche etait lourde"
    NSPhraseMotArray    AttCOD ;    // Attribut du COD
                                    // "J'ai trouve vos propositions interessantes"

    // Complements circonstanciels : quand, ou, comment, avec quoi, pourquoi,
    // dans quel but
    TYPELOCALISATION    iTypeLocalisation ;
    NSPhraseMot         LiaisonLieu, PrepositionLieu ;
    NSPhraseMotArray    CCLieu ;    // Complement circonstanciel de lieu

    NSPhraseMot         LiaisonTemps, PrepositionTemps ;
    NSPhraseMotArray    CCTemps ;   // Complement circonstanciel de temps

    NSPhraseMot         LiaisonManiere, PrepositionManiere ;
    NSPhraseMotArray    CCManiere ; // Complement circonstanciel de maniere
                                    // Il mange avec delicatesse

    NSPhraseMot         LiaisonMoyen, PrepositionMoyen ;
    NSPhraseMotArray    CCMoyen ;   // Complement circonstanciel de moyen
                                    // Il mange avec des couverts en argent

    NSPhraseMot         LiaisonCause, PrepositionCause ;
    NSPhraseMotArray    CCCause ;   // Complement circonstanciel de cause
                                    // Il tremble de peur

    NSPhraseMot         LiaisonBut, PrepositionBut ;
    NSPhraseMotArray    CCBut ;     // Complement circonstanciel de but
                                    // Il court pour maigrir

    NSPhraseMot         LiaisonType, PrepositionType ;
    NSPhraseMotArray    CCType ;    // Complement circonstanciel de type
                                    // Douleur thoracique a type de douleur angineuse.

    NSPhraseMot         LiaisonHypoth, PrepositionHypoth ;
    NSPhraseMotArray    CCHypoth ;  // Complement circonstanciel d'hypothese
                                    // A prendre en cas de douleur.

    NSPhraseMot         LiaisonChiffre, PrepositionChiffre ;
    NSPhraseMotArray    CCChiffre ; // Valeur chiffree + valeur normale

    // Complement de la phrase apres ":".
    //
    NSPhraseur*         pDeuxPoints ;

    //
    // ------------- Variables specifiques des complements --------------
    //
    NSPhraseMotArray    adjEpithete ;           // Epithete "Une belle journee"
    NSPhraseMotArray    adjEpitheteAv ;         // Epithete "Une belle journee"
    NSPhraseMotArray    adjEpitheteAp ;         // Epithete "Une maison rouge"
    NSPhraseMot         adjNumeralCardinal ;    // Adjectif numeral cardinal : "3 pommes"
    NSPhraseMot         adjNumeralOrdinal ;     // Adjectif numeral ordinal : "la troisieme pomme"

    // Complement du nom
    NSPhraseMotArray    compNom ;               // "Suspicion d'endocartite"

    // Adverbes qui peuvent completer un adjectif, un verbe ou un adverbe.

    NSPhraseMotArray    adverbe ;               // "un homme tres grand", "ils marchent vite"

    //
    // -------------------------- Methodes --------------------------------
    //

    NSPhraseur(NSContexte* pCtx) ;
    NSPhraseur(NSPhraseur& rv) ;
    ~NSPhraseur() ;

    // Pour mettre a zero toutes les prepositions.
    void   nettoye() ;

    void   initialise() ;
    void   copieTout(NSPhraseur* pSrc) ;

    void   initParentheses() ;
    void   initDeuxPoints() ;

    void   classeAdjectif(NSGenerateur* pGener) ; // Classe les adjectifs en avant et apres

    NSPhraseur& operator=(NSPhraseur src) ;
};

class NsProposition : public NSRoot
{
  public :

    enum PROPOSITIONOBJECTTYPE { notSetObjType = 0, isPhraseur, isPropositionArray } ;

    enum PROPOSITIONTYPE
    {
      notSetType = 0,
      principale,
      //
      // Completives
      //
      completiveQue,                          // SCQ : Je veux que tu reviennes
      completiveInfinitive,                   // SCI : J'entends les chats miauler
      completiveInterrogative,                // SCN : Je voudrais savoir pourquoi tu ne veux pas
      //
      // Circonstancielles
      //
      // - Temporelle
      circonstancielleTemporelleAnterieure,   // SCTA : Je le verrai avant qu'il chante
      circonstancielleTemporelleSimultanee,   // SCTS : Je chantais au moment ou il arriva
      circonstancielleTemporellePosterieure,  // SCTP : Je chanterai apres que vous serez parti
      // - Consecutive -> resultat de l'action
      circonstancielleConsecutive,            // SCR : Il fait trop froid pour que je chante
      // - Causale -> cause de l'action
      circonstancielleCausale,                // SCC : Il a reussi parce qu'il chantait bien
      // - Concessive -> relation inattendue
      circonstancielleConcessive,             // SCS : Il a echoue bien qu'il chanta bien
      // - Finale -> but de l'action
      circonstancielleFinale,                 // SCF : Il chante fort pour qu'on l'entende bien
      // - Comparative -> comparaison
      circonstancielleComparativeProportion,  // SCCP : Vous chanterez d'autant plus fort que vous serez bien echauffe
      circonstancielleComparativeEgalite,     // SCCE : Vous chantez aussi fort que moi
      circonstancielleComparativeRessemblance,// SCCR : Tu me regardes comme me regardait ta mere
      // - Conditionnelle -> condition
      circonstancielleConditionnelleHypothese,// SCHH : il mange s'il a faim, il mangera s'il a faim
      circonstancielleConditionnelleImaginee, // SCHH : il mangerait s'il avait faim
      //
      // Relative : traitee en complement du nom
      // Participiale ? (pas de mot subordonnant : le travail acheve, ils partirent en vacance,
      //                  fonctionne comme une circonstancielle sans subordonnant ; peut introduire
      //                  des nuances de temps, cause, concession et condition)
      //

      //
      // Deux points
      //
      deuxPoints, // tout va bien : la vie est belle
    } ;

    enum CONJONCTIONTYPE
    {
      notSetConjonct = 0,
      SCTA_AvantQue,          // ---X---|
      SCTA_JusteAvantQue,     // ------X|
      SCTA_JusquAceQue,       // --XXXXX|
      SCTP_ApresQue,          // ---|---X
      SCTP_DesQue,            // ---|X---
      SCTP_DepuisQue,         // ---|XXXX
      SCCP_AutantQuantite,    // suivant, selon, a mesure
      SCCP_Synchrone,         // au fur et a mesure
      SCCP_PlusQuantite,      // d'autant plus
      SCCP_PlusVite,          // d'autant plus vite
      SCCP_MoinsQuantite,     // d'autant moins
      SCCP_MoinsVite,         // d'autant moins vite
      SCCE_Aspect,            // tel que, de meme que, si ... que
      SCCE_Quantite,          // autant que, aussi que
      SCCE_Temps,             // tant que
    } ;

    PROPOSITIONTYPE         iPropositionType ;
    CONJONCTIONTYPE         iConjonctionType ;

    PROPOSITIONOBJECTTYPE   iObjectType ;
    void*                   pProposition ;
    bool                    bDeleteAtEnd ;

    string                  sPhrase, sLang ;

    NSPhraseur**            pPhraDelAtEnd ;

    NsProposition(NSContexte* pCtx) ;
    NsProposition(NSContexte* pCtx, NSPhraseur** pPhrase, PROPOSITIONTYPE iPropType, CONJONCTIONTYPE iConjonct, bool bDelAtEnd = true) ;
    NsProposition(NSContexte* pCtx, NSPhraseur* pPhrase, PROPOSITIONTYPE iPropType, CONJONCTIONTYPE iConjonct) ;
    NsProposition(NsProposition& rv) ;
    ~NsProposition() ;

    NsProposition& operator=(NsProposition src) ;
};

typedef vector<NsProposition*> NSPropositionVector ;
typedef NSPropositionVector::iterator iterProposition ;

typedef NTArray<NsProposition> NSPropositionArray;

#endif  // __NSPHRASE_H

