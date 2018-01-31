#ifndef __NSGENLAN_H
#define __NSGENLAN_H

#include "dcodeur/nsgen.h"
#include "dcodeur/nsdkd.h"
#include "nssavoir/nspatho.h"

class NSGenerateurFr : public NSGenerateur
{
  public :

    enum GENRENOM { declinaisonSingulier = 0, declinaisonPluriel };
    enum GENREADJ { declinaisonMS = 0, declinaisonFS, declinaisonMP, declinaisonFP };

    enum TEMPSVERBEFR { tempsPasseFr = 1, tempsPresentFr, tempsFuturFr };
    enum ASPECTVERBEFR { aspectMoment = 1, aspectAccompli, aspectProspectif };

    NSGenerateurFr(NSContexte* pCtx, NsProposition* pPropos, string sLangue);
    NSGenerateurFr(NSGenerateurFr& rv);

    ~NSGenerateurFr();

    NSGenerateurFr& operator=(NSGenerateurFr src);

    void    classeTout();

    bool    assembleProposition(DCODETYPE iStyle, NsProposition* pPropos = (NsProposition*) 0);

    void    donneLibelleAffiche(string* pLibelle, NSPathologData* pData,
                                   GENRE iGenre = genreNull);

    bool    prepareVerbe(string* psPhraseAdverbe, string* psAdverbeModificateur, bool* pbPhraseAffirmative, DCODETYPE iStyle);
    bool    prepareSujet(string* psSujet, GENRE* piGenreSujet, string* psAttSujet, string* psAdverbeModificateur, bool* pbPhraseAffirmative, DCODETYPE iStyle);
    bool    prepareCOD(string* psCOD, GENRE* piGenreCOD, string* psAttCOD, string* psAdverbeModificateur, bool* pbPhraseAffirmative, DCODETYPE iStyle);

    string  donneNomComplet(NSPhraseMot* pMot, int iArticle, GERECERTITUDE iGereCertitude = avecCertitude);

    bool    genereNomComplet(NSPhraseMot* pMot, int iArticle, GERECERTITUDE iGereCertitude = avecCertitude);

    string  donneAdjectifComplet(NSPhraseMot* pMot, GENRE iGenre = genreNull, GERECERTITUDE iGereCertitude = avecCertitude);
    bool    genereAdjectifComplet(NSPhraseMot* pMot, GENRE iGenre = genreNull, GERECERTITUDE iGereCertitude = avecCertitude);

    string  donneAdverbeComplet(NSPhraseMot* pMot, GERECERTITUDE iGereCertitude = avecCertitude);
    bool    genereAdverbeComplet(NSPhraseMot* pMot, GERECERTITUDE iGereCertitude = avecCertitude);

    string  donneCertitude(string sCertitude, bool* pCommenceParVoyelleOuH = (bool*) 0);

    string  donnePhraseComplement(NSPhraseMot* pLiaison,
                                 NSPhraseMot* pPreposition,
                                 NSPhraseMotArray* pCC, bool* bSucces);
    string  donnePhraseChiffree(NSPhraseMot* pLiaison,
                               NSPhraseMot* pPreposition,
                               NSPhraseMotArray* pCC, bool* bSucces);

    bool    donnePremierAdj(NSPhraseur* pCompAdj, NSPhraseMot* pMotAdj = (NSPhraseMot*) 0);
    bool    donnePremierAdv(NSPhraseur* pComp, NSPhraseMot* pMotAdv = (NSPhraseMot*) 0);

    string  decodeNum(gereNum* pNum, bool* bSucces);
    string  decodeTime(NSPhraseMotTime* pTime, bool* bSucces);
    string  decodeCycle(NSPhraseMotTimeCycle* pCycle, bool* bSucces);

    string  donneArticleIndefini(NSPhraseMot* pMot, bool bPluriel, GERECERTITUDE iGereCertitude = avecCertitude);
    string  donneArticleDefini(NSPhraseMot* pMot, bool bPluriel, GERECERTITUDE iGereCertitude = avecCertitude);
    string  donneArticlePartitif(NSPhraseMot* pMot, bool bPluriel, GERECERTITUDE iGereCertitude = avecCertitude);
    string  donnePronomPersonnel(GENRE iGenre, NSPhraseur::VBPERSO iVbPersonne, string sFonction);

    void    etFinal(string *type, string *type1);

    string  donneParticipePresent(NSPathologData* pPathoData);
    string  donneParticipePasse(NSPhraseMot* pPhraseMot, GENRE iGenre);

    void    mapTempsVerbe();
    void    donneVerbe(string* principal, string* auxilliaire);
    string  donneVerbeTerminaison(int groupe, string ssgroupe);
    bool    donneVerbeClasse(NSPhraseMot* pPhraseMot, string* psTransitivite,
                          string* psConjugaison, string* psActEtat);
    string  donneAuxilliaire(string* auxilliaire, int iVbTemps, int iVbAspect,
                            int iVbMode, int iVbPersonne, int iVbNegation);

    bool    traitePostposition(NSPhraseMot* pMot, NSPhraseMotArray* pSpecifique = (NSPhraseMotArray*) 0);
    bool    traitePrepositionCC(NSPhraseMotArray* pCompC, NSPhraseMot* pPreposition = (NSPhraseMot*) 0, NSPhraseur::VBPREPOCTX iContexte = NSPhraseur::prepUndefined);
    bool    modifiePreposition(NSPhraseMot* pMot, int iArticle, bool bPluriel);

    string  postTraitement(string *sEntree);

  protected :

    int     iTempsVerbe;
    int     iAspectVerbe;

    bool    generePhrase(DCODETYPE iStyle);
};

class gereDateFr : public gereDate
{
  public :

    gereDateFr(NSSuper* pSuper, string sLangue):gereDate(pSuper, sLangue) {}
    gereDateFr(const gereDateFr&);
    ~gereDateFr() {}

    gereDateFr& operator =(const gereDateFr&);

    enum TYPEDATE { DateNormal = 1, DateDeb, DateFin};

    void   donne_date_breve(string* pMessage, string* pIntro, int iType = DateNormal);
    void   donne_date_claire(string* pMessage, string* pIntro, int iType = DateNormal);
    string donne_mois(string mois);
    string donne_mois(int iMois);
};


class NSGenerateurEn : public NSGenerateur
{
  public :

    enum GENRENOM { declinaisonSingulier = 0, declinaisonPluriel };
    enum GENREVERBE { declinaisonBase = 0, declinaison3Personne,
                      declinaisonPartPresent, declinaisonPreterite,
                      declinaisonPartPasse };

    NSGenerateurEn(NSContexte* pCtx, NsProposition* pPropos, string sLangue);
    NSGenerateurEn(NSGenerateurEn& rv);

    ~NSGenerateurEn();

    NSGenerateurEn& operator=(NSGenerateurEn src);

    void    classeTout();

    bool    assembleProposition(DCODETYPE iStyle, NsProposition* pPropos = (NsProposition*) 0);

    void    donneLibelleAffiche(string* pLibelle, NSPathologData* pData, GENRE iGenre = genreNull);
    void    donneLibelleAffiche(string* pLibelle, NSPathologData* pData, GENREVERBE iGenre);

    bool    prepareVerbe(string* psPhraseAdverbe, string* psAdverbeModificateur, bool* pbPhraseAffirmative, DCODETYPE iStyle);
    bool    prepareSujet(string* psSujet, GENRE* piGenreSujet, string* psAttSujet, string* psAdverbeModificateur, bool* pbPhraseAffirmative, DCODETYPE iStyle);
    bool    prepareCOD(string* psCOD, GENRE* piGenreCOD, string* psAttCOD, string* psAdverbeModificateur, bool* pbPhraseAffirmative, DCODETYPE iStyle);

    string  donneNomComplet(NSPhraseMot* pMot, int iArticle, GERECERTITUDE iGereCertitude = avecCertitude);

    bool    genereNomComplet(NSPhraseMot* pMot, int iArticle, GERECERTITUDE iGereCertitude = avecCertitude);

    string  donneAdjectifComplet(NSPhraseMot* pMot, GENRE iGenre = genreNull, GERECERTITUDE iGereCertitude = avecCertitude);
    bool    genereAdjectifComplet(NSPhraseMot* pMot, GENRE iGenre = genreNull, GERECERTITUDE iGereCertitude = avecCertitude);

    string  donneAdverbeComplet(NSPhraseMot* pMot, GERECERTITUDE iGereCertitude = avecCertitude);
    bool    genereAdverbeComplet(NSPhraseMot* pMot, GERECERTITUDE iGereCertitude = avecCertitude);

    bool    donnePremierAdv(NSPhraseur* pComp, NSPhraseMot* pMotAdv = (NSPhraseMot*) 0);

    string  donneCertitude(string sCertitude, bool* pCommenceParVoyelleOuH = (bool*) 0);

    string  donnePhraseComplement(NSPhraseMot* pLiaison,
                                 NSPhraseMot* pPreposition,
                                 NSPhraseMotArray* pCC, bool* bSucces);
    string  donnePhraseChiffree(NSPhraseMot* pLiaison,
                               NSPhraseMot* pPreposition,
                               NSPhraseMotArray* pCC, bool* bSucces);

    bool    donnePremierAdj(NSPhraseur* pCompAdj, NSPhraseMot* pMotAdj = 0);

    string  decodeNum(gereNum* pNum, bool* bSucces);
    string  decodeTime(NSPhraseMotTime* pTime, bool* bSucces);
    string  decodeCycle(NSPhraseMotTimeCycle* pCycle, bool* bSucces);

    string  donneArticleIndefini(NSPhraseMot* pMot, bool bPluriel, GERECERTITUDE iGereCertitude = avecCertitude);
    string  donneArticleDefini(NSPhraseMot* pMot, bool bPluriel, GERECERTITUDE iGereCertitude = avecCertitude);
    string  donneArticlePartitif(NSPhraseMot* pMot, bool bPluriel, GERECERTITUDE iGereCertitude = avecCertitude);
    string  donnePronomPersonnel(GENRE iGenre, NSPhraseur::VBPERSO iVbPersonne, string sFonction);

    void    etFinal(string *type, string *type1);

    string  donneParticipePresent(NSPathologData* pPathoData);
    string  donneParticipePasse(NSPhraseMot* pPhraseMot, GENRE iGenre = genreNull);
    string  donnePreterite(NSPhraseMot* pPhraseMot);
    void    donneVerbe(string* principal, string* auxilliaire);
    bool    donneVerbeClasse(NSPhraseMot* pPhraseMot, string* psTransitivite, string* psActEtat);

    bool    traitePostposition(NSPhraseMot* pMot, NSPhraseMotArray* pSpecifique = (NSPhraseMotArray*) 0);
    bool    traitePrepositionCC(NSPhraseMotArray* pCompC, NSPhraseMot* pPreposition = (NSPhraseMot*) 0, NSPhraseur::VBPREPOCTX iContexte = NSPhraseur::prepUndefined);

    string  postTraitement(string *sEntree);

  protected :

    bool    generePhrase(DCODETYPE iStyle);
};

class gereDateEn : public gereDate
{
  public :

    gereDateEn(NSSuper* pSuper, string sLangue):gereDate(pSuper, sLangue) {}
    gereDateEn(const gereDateEn&);
    ~gereDateEn() {}

    gereDateEn& operator =(const gereDateEn&);

    enum TYPEDATE { DateNormal = 1, DateDeb, DateFin};

    void    donne_date_breve(string* pMessage, string* pIntro, int iType = DateNormal);
    void    donne_date_claire(string* pMessage, string* pIntro, int iType = DateNormal);
    string  donne_mois(string mois);
    string  donne_mois(int iMois);
};

#endif  // __NSGENLAN_H

