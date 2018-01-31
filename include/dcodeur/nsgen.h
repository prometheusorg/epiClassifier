#ifndef __NSGEN_H
#define __NSGEN_H

class NSGenereCompArray;
class NSPathologData;

class gereNum;

#include "dcodeur/nsphrase.h"
#include "nssavoir/nspatho.h"

#define STR_SUJET     "Sujt"
#define STR_COD       "CCOD"
#define STR_COI       "CCOI"
#define STR_COS       "CCOS"
#define STR_CCLIEU    "CCLi"
#define STR_CCTEMPS   "CCTp"
#define STR_CCMANIERE "CCMa"
#define STR_CCMOYEN   "CCMo"
#define STR_CCCAUSE   "CCCa"
#define STR_CCBUT     "CCBu"
#define STR_CCTYPE    "CCTy"
#define STR_CCCHIFFRE "CCCh"
#define STR_CCHYPOTH  "CCHy"

//
// Enumeration des modes de decodage
//
enum DCODETYPE { dcPhrase, dcTiret };

class NSGenerateur : public NSRoot
{
	public :

    enum GERECERTITUDE { sansCertitude = 0, avecCertitude };
    enum WORDTYPE { isAny = 1, isNoun, isAdjective, isAdverb, isVerb, isInvar };

    NSGenerateur(NSContexte* pCtx, NsProposition* pPropos, string sLangue);
    NSGenerateur(NSGenerateur& rv);
    ~NSGenerateur();

    void initialise();
    void reinitialise();
    void copieTout(NSGenerateur* rv);

    bool ajouteMot(NSPhraseMotArray** ppMotArray, NSPhraseMot* pMot);
    bool ajouteCopieMot(NSPhraseMotArray** ppMotArray, NSPhraseMot* pMot);

    bool CommenceParVoyelle(string* pLib);
    bool terminePhrase();
    bool termineProposition(NsProposition* pPropos = (NsProposition*) 0);
    void etDuMilieu(string *type, string *type1, string *type2);

    // Pour la gestion de la certitude
    //
    NSPhraseMotArray*  pAdjEpitheteAvPos;
    NSPhraseMotArray*  pAdjEpitheteAvNeg;

    NSPhraseMotArray*  pAdjEpitheteApPos;
    NSPhraseMotArray*  pAdjEpitheteApNeg;

    NSPhraseMotArray*  pCompNomPos;
    NSPhraseMotArray*  pCompNomNeg;

    NSPhraseMotArray*  pAdverbePos;
    NSPhraseMotArray*  pAdverbeNeg;

    NSPhraseMotArray*  pCODPos;
    NSPhraseMotArray*  pCODNeg;

    NSPhraseMotArray*  pAttSujetPos;
    NSPhraseMotArray*  pAttSujetNeg;

    NSPhraseMotArray*  pAttCODPos;
    NSPhraseMotArray*  pAttCODNeg;

    NSGenereCompArray* pCCArray;

    //
    // Generateur de phrase principale
    //
    bool           genereProposition(DCODETYPE iStyle, NsProposition* pPropos = (NsProposition*) 0);
		virtual bool   assembleProposition(DCODETYPE iStyle, NsProposition* pPropos) = 0;
    //
    // Generateur de relative
    //
    virtual void   donneLibelleAffiche(string* pLibelle, NSPathologData* pData, GENRE iGenre = genreNull) = 0;

    virtual string donnePhraseComplement(NSPhraseMot* pLiaison,
                                         NSPhraseMot* pPreposition,
                                         NSPhraseMotArray* pCC,
                                         bool* bSucces) = 0;
    virtual string donnePhraseChiffree(NSPhraseMot* pLiaison,
                                       NSPhraseMot* pPreposition,
                                       NSPhraseMotArray* pCC,
                                       bool* bSucces) = 0;

    virtual bool   donnePremierAdj(NSPhraseur* pCompAdj, NSPhraseMot* pMotAdj = 0) = 0;

    virtual string donneArticleIndefini(NSPhraseMot* pMot, bool bPluriel, GERECERTITUDE iGereCertitude = avecCertitude) = 0;
    virtual string donneArticleDefini(NSPhraseMot* pMot, bool bPluriel, GERECERTITUDE iGereCertitude = avecCertitude) = 0;
    virtual string donneArticlePartitif(NSPhraseMot* pMot, bool bPluriel, GERECERTITUDE iGereCertitude = avecCertitude) = 0;
    virtual string donnePronomPersonnel(GENRE iGenre, NSPhraseur::VBPERSO iVbPersonne, string sFonction) = 0;

    virtual void   etFinal(string *type, string *type1) = 0;

    virtual string donneParticipePresent(NSPathologData* pPathoData) = 0;
    virtual string donneParticipePasse(NSPhraseMot* pPhraseMot, GENRE iGenre) = 0;
    virtual void   donneVerbe(string* principal, string* auxilliaire) = 0;

    virtual string decodeNum(gereNum* pNum, bool* bSucces) = 0;

    virtual string postTraitement(string *sEntree) = 0;

    void           contracting(string *pSentence, string sFrom, string sTo);
    void           dispatchArray(NSPhraseMotArray* pMots, NSPhraseMotArray** ppYesArray, NSPhraseMotArray** ppNoArray, WORDTYPE iWordType = isAny);
    void           dispatchYesNo(NSPhraseMot* pMot, NSPhraseMotArray** ppYesArray, NSPhraseMotArray** ppNoArray);

    // void            setPhrase(string sPhra) { sPhrase = sPhra; }
    string         getTempoPhrase()        { return sPhrase; }
    string         getPropositionPhrase()  { return pProposition->sPhrase; }

    void           setCurrentPhraseur(NSPhraseur* pPhr) { pPh = pPhr; }
    NSPhraseur*    getCurrentPhraseur()                 { return pPh; }

    void           setProposition(NsProposition* pPhr) { pProposition = pPhr; }
    NsProposition* getProposition()                    { return pProposition; }

    void           setLang(string sLng)  { sLang = sLng; }
    string         getLang()             { return sLang; }

	protected:

    string         sPhrase, sLang;
    NSPhraseur*    pPh;
    NsProposition* pProposition;

    virtual bool   generePhrase(DCODETYPE iStyle) = 0;
};

class NSGenComplement : public NSRoot
{
	public:

    // Type de complement
    //
    string            sType;

    // Arrays de decomposition en positif et negatif
    //
    NSPhraseMotArray* pMotsPos;
    NSPhraseMotArray* pMotsNeg;

    // Phrases issues du decodage
    //
    string            sPositif;
    string            sNegatif;

    // Priorite de ce complement dans la phrase
    //
    int               iPriorite;

    NSGenerateur*     pGenerateur;

    NSGenComplement(NSContexte* pCtx, NSGenerateur* pGener);
    NSGenComplement(NSGenComplement& rv);
    ~NSGenComplement();

    void donnePhrElements(NSPhraseMot** ppLiaison, NSPhraseMot** ppPreposition, NSPhraseMotArray** ppMots);
    bool donnePhrase();
    void initialise();
    void classe();

    NSGenComplement& operator =(const NSGenComplement&);
};

typedef vector<NSGenComplement*> NSGenCompArray;
typedef NSGenCompArray::iterator GenCompIter;

class NSGenereCompArray : public NSGenCompArray
{
	public :

		NSContexte*   pContexte;
		NSGenerateur* pGenerateur;

    // Constructeurs
    NSGenereCompArray(NSContexte* pCtx, NSGenerateur* pGenerateur);
    NSGenereCompArray(NSGenereCompArray& rv);
    // Destructeur
    ~NSGenereCompArray();

    bool donnePhrase();
    void initialise();
    void classeTout();

    NSGenComplement* donneComplement(string sType);

    void vider();
    NSGenereCompArray& operator=(NSGenereCompArray src);
};

#endif  // __NSGEN_H
