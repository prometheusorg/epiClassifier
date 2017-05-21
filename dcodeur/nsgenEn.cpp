#ifdef _ENTERPRISE_DLL
  #include "enterpriseLus/nsdivfctForCgi.h"
  #include "enterpriseLus/superLus.h"
  #include "mysql/mysql.h"
#else
  #include "partage\nsdivfct.h"
  #include "nautilus\nssuper.h"
#endif

#include "dcodeur/nsgen.h"
#include "dcodeur/nsgenlan.h"
#include "dcodeur/nsphrase.h"
#include "dcodeur/nsdkd.h"
#include "nsbb/nsbbtran.h"
#include "nssavoir/nspatho.h"
#include "nssavoir/nsfilgd.h"

// -------------------------------------------------------------------------
// ---------------------- METHODES DE generateurEn -------------------------
// -------------------------------------------------------------------------

NSGenerateurEn::NSGenerateurEn(NSContexte* pCtx, NsProposition* pPropos, string sLangue)
               :NSGenerateur(pCtx, pPropos, sLangue)
{
}

NSGenerateurEn::NSGenerateurEn(NSGenerateurEn& rv)
               :NSGenerateur(rv)
{
}

NSGenerateurEn::~NSGenerateurEn()
{
}

NSGenerateurEn&
NSGenerateurEn::operator=(NSGenerateurEn rv)
{
	if (&rv == this)
		return *this ;

	reinitialise() ;
  copieTout(&rv) ;

	return *this ;
}

void
NSGenerateurEn::classeTout()
{
try
{
	iterPhraseMot  iterMots;
	NSPathologData Data;
	NSSuper* pSuper = pContexte->getSuperviseur();

	reinitialise();

    // Les adjectifs ayant une certitude sont toujours places apres le nom.

    if (!(pPh->adjEpitheteAv.empty()))
    {
        for (iterMots = pPh->adjEpitheteAv.begin(); iterMots != pPh->adjEpitheteAv.end(); iterMots++)
        {
            // Les adjectifs ont deja ete tries en avant et apres, donc on ne reverifie pas
            // qu'il s'agit bien d'adjectifs du lexique.

            string sCertitude = (*iterMots)->getCertitude() ;

            if (sCertitude != "")
            {
                if ((string(sCertitude, 0, 3) == "WCE") && (string(sCertitude, 0, 5) < "WCE50"))
                    ajouteCopieMot(&pAdjEpitheteApNeg, *iterMots);
                else if ((string(sCertitude, 0, 3) == "WCE") && (string(sCertitude, 0, 5) >= "WCE50") && (string(sCertitude, 0, 5) != "WCEA0"))
                    ajouteCopieMot(&pAdjEpitheteApPos, *iterMots);
                else if ((string(sCertitude, 0, 3) == "WCE") && (string(sCertitude, 0, 5) == "WCEA0"))
                    ajouteCopieMot(&pAdjEpitheteAvPos, *iterMots);
            }
            else
                ajouteCopieMot(&pAdjEpitheteAvPos, *iterMots);
        }
    }

    if (!(pPh->adjEpitheteAp.empty()))
    {
        for (iterMots = pPh->adjEpitheteAp.begin(); iterMots != pPh->adjEpitheteAp.end(); iterMots++)
        {
            // Les adjectifs ont deja ete tries en avant et apres, donc on ne reverifie pas
            // qu'il s'agit bien d'adjectifs du lexique.

            string sCertitude = (*iterMots)->getCertitude() ;

            if (sCertitude != "")
            {
                if ((string(sCertitude, 0, 3) == "WCE") && (string(sCertitude, 0, 5) < "WCE50"))
                    ajouteCopieMot(&pAdjEpitheteApNeg, *iterMots);
                else if ((string(sCertitude, 0, 3) == "WCE") && (string(sCertitude, 0, 5) >= "WCE50"))
                    ajouteCopieMot(&pAdjEpitheteApPos, *iterMots);
            }
            else
                ajouteCopieMot(&pAdjEpitheteApPos, *iterMots);
        }
    }

    if (!(pPh->compNom.empty()))
    {
        for (iterMots = pPh->compNom.begin(); iterMots != pPh->compNom.end(); iterMots++)
        {
            string sLexique = (*iterMots)->getLexique() ;

            if (false == (*iterMots)->estTexteLibre())
            {
                bool trouve = pContexte->getDico()->trouvePathologData(sLang, &sLexique, &Data);
                if (!trouve)
                    return;
                if (!Data.estNom())
                    return;
            }
            string sCertitude = (*iterMots)->getCertitude() ;

            if (sCertitude != "")
            {
                if ((string(sCertitude, 0, 3) == "WCE") && (string(sCertitude, 0, 5) < "WCE50"))
                    ajouteCopieMot(&pCompNomNeg, *iterMots);
                else if ((string(sCertitude, 0, 3) == "WCE") && (string(sCertitude, 0, 5) >= "WCE50"))
                    ajouteCopieMot(&pCompNomPos, *iterMots);
            }
            else
                ajouteCopieMot(&pCompNomPos, *iterMots);
        }
    }

    if (!(pPh->adverbe.empty()))
    {
        for (iterMots = pPh->adverbe.begin(); iterMots != pPh->adverbe.end(); iterMots++)
        {
            string sLexique = (*iterMots)->getLexique() ;
            if (false == (*iterMots)->estTexteLibre())
            {
                bool trouve = pContexte->getDico()->trouvePathologData(sLang, &sLexique, &Data);
                if (!trouve)
                    return;
                if (!Data.estAdverbe())
                    return;
            }

            string sCertitude = (*iterMots)->getCertitude() ;

            if (sCertitude != "")
            {
                if ((string(sCertitude, 0, 3) == "WCE") && (string(sCertitude, 0, 5) < "WCE50"))
                    ajouteCopieMot(&pAdverbeNeg, *iterMots);
                else if ((string(sCertitude, 0, 3) == "WCE") && (string(sCertitude, 0, 5) >= "WCE50"))
                    ajouteCopieMot(&pAdverbePos, *iterMots);
            }
            else
                ajouteCopieMot(&pAdverbePos, *iterMots);
        }
    }

    if (!(pPh->COD.empty()))
    {
        for (iterMots = pPh->COD.begin(); iterMots != pPh->COD.end(); iterMots++)
        {
            string sLexique = (*iterMots)->getCertitude() ;

            if (false == (*iterMots)->estTexteLibre())
            {
                bool trouve = pContexte->getDico()->trouvePathologData(sLang, &sLexique, &Data);
                if (!trouve)
                    return;
                if (!Data.estNom())
                    return;
            }

            string sCertitude = (*iterMots)->getCertitude() ;

            if (sCertitude != "")
            {
                if ((string(sCertitude, 0, 3) == "WCE") && (string(sCertitude, 0, 5) < "WCE50"))
                    ajouteCopieMot(&pCODNeg, *iterMots);
                else if ((string(sCertitude, 0, 3) == "WCE") && (string(sCertitude, 0, 5) >= "WCE50"))
                    ajouteCopieMot(&pCODPos, *iterMots);
            }
            else
                ajouteCopieMot(&pCODPos, *iterMots);
        }
    }

    if (!(pPh->AttSujet.empty()))
    {
        for (iterMots = pPh->AttSujet.begin(); iterMots != pPh->AttSujet.end(); iterMots++)
        {
            string sLexique = (*iterMots)->getLexique() ;

            if (!((*iterMots)->estTexteLibre()))
            {
                bool trouve = pContexte->getDico()->trouvePathologData(sLang, &sLexique, &Data);
                if (!trouve)
                    return;
            }

            string sCertitude = (*iterMots)->getCertitude() ;

            if (sCertitude != "")
            {
                if ((string(sCertitude, 0, 3) == "WCE") && (string(sCertitude, 0, 5) < "WCE50"))
                    ajouteCopieMot(&pAttSujetNeg, *iterMots);
                else if ((string(sCertitude, 0, 3) == "WCE") && (string(sCertitude, 0, 5) >= "WCE50"))
                    ajouteCopieMot(&pAttSujetPos, *iterMots);
            }
            else
                ajouteCopieMot(&pAttSujetPos, *iterMots);
        }
    }

    if (!(pPh->AttCOD.empty()))
    {
        for (iterMots = pPh->AttCOD.begin(); iterMots != pPh->AttCOD.end(); iterMots++)
        {
            string sLexique = (*iterMots)->getLexique() ;

            if (!((*iterMots)->estTexteLibre()))
            {
                bool trouve = pContexte->getDico()->trouvePathologData(sLang, &sLexique, &Data);
                if (!trouve)
                    return;
            }

            string sCertitude = (*iterMots)->getCertitude() ;

            if (sCertitude != "")
            {
                if ((string(sCertitude, 0, 3) == "WCE") && (string(sCertitude, 0, 5) < "WCE50"))
                    ajouteCopieMot(&pAttCODNeg, *iterMots);
                else if ((string(sCertitude, 0, 3) == "WCE") && (string(sCertitude, 0, 5) >= "WCE50"))
                    ajouteCopieMot(&pAttCODNeg, *iterMots);
            }
            else
                ajouteCopieMot(&pAttCODNeg, *iterMots);
        }
    }

    pCCArray->classeTout();
}
catch (...)
{
	erreur("Exception NSGenerateurEn::classeTout", standardError) ;
}
}

void
NSGenerateurEn::donneLibelleAffiche(string* pLibelle, NSPathologData* pData, GENREVERBE iGenre)
{
  if (NULL == pLibelle)
    return ;

  // En anglais, cette methode sert pour les noms et les verbes.
  // Les noms peuvent etre singuliers ou pluriel.
  // Les verbes peuvent etre au participe present, au preterite ou au participe
  // passe.

  int iDeclinaison = 0 ;
  *pLibelle = string("") ;

  if (NULL == pData)
    return ;

  if (pData->estVerbe())
  {
    switch (iGenre)
    {
      case declinaisonBase        : iDeclinaison = declinaisonBase ;        break ;
      case declinaison3Personne   : iDeclinaison = declinaison3Personne ;   break ;
      case declinaisonPartPresent : iDeclinaison = declinaisonPartPresent ; break ;
      case declinaisonPreterite   : iDeclinaison = declinaisonPreterite ;   break ;
      case declinaisonPartPasse   : iDeclinaison = declinaisonPartPasse ;   break ;
      default                     : iDeclinaison = 0 ;
    }

    pData->donneLibelleAffiche(pLibelle, iDeclinaison) ;
  }
  else
    pData->donneLibelleAffiche(pLibelle) ;
}

void
NSGenerateurEn::donneLibelleAffiche(string* pLibelle, NSPathologData* pData, GENRE iGenre)
{
  if (NULL == pLibelle)
    return ;

  // En anglais, cette methode sert pour les noms et les verbes.
  // Les noms peuvent etre singuliers ou pluriel.
  // Les verbes peuvent etre au participe present, au preterite ou au participe
  // passe.

  int iDeclinaison = 0 ;
  *pLibelle = string("") ;

  if (NULL == pData)
    return ;

  if (pData->estNom())
  {
    if ((genreMP == iGenre) || (genreFP == iGenre) || (genreNP == iGenre))
      pData->donneLibelleAffiche(pLibelle, declinaisonPluriel) ;
    else
      pData->donneLibelleAffiche(pLibelle, declinaisonSingulier) ;
  }
  else
    pData->donneLibelleAffiche(pLibelle) ;
}

bool
NSGenerateurEn::generePhrase(DCODETYPE iStyle)
{
    reinitialise();
    sPhrase = "";

    bool     trouve;
    NSSuper* pSuper = pContexte->getSuperviseur();
    string   sLexique;
    string   sPluriel;
    string   sCertitude;
    string   sInter1 = "";
    string   sInter2 = "";
    bool     bSucces;

    // La phrase est-elle affirmative ou negative ?
    // true pour phrase affirmative, false pour phrase negative.

    bool    bPhraseAffirmative = true;
    string  sAdverbeModificateur = " ";
    string  sAdverbeModificateurAtt = " ";

    iterPhraseMot  iterMots;
    NSPathologData Data;

    string sVerbe = "";

    // Pour les adverbes du verbe

    string sPhraseAdverbe = "";

    string sPhrasePos = "";
    string sPhraseNeg = "";
    string sInterPos1 = "";
    string sInterPos2 = "";
    string sInterNeg1 = "";
    string sInterNeg2 = "";

    // Avant de classer les complements, on leur met la preposition lorsqu'il y
    // en a une d'imposee par le mot complete.
    // Le verbe peut imposer une preposition au COI ainsi qu'au COS.
    // Il faut traiter les prepositions des complements circonstanciels.

    if (!pPh->Verbe.empty())
        traitePostposition(*(pPh->Verbe.begin()));

    traitePrepositionCC(&pPh->CCLieu,    &pPh->PrepositionLieu,    NSPhraseur::prepLieu);
    traitePrepositionCC(&pPh->CCTemps,   &pPh->PrepositionTemps,   NSPhraseur::prepTemps);
    traitePrepositionCC(&pPh->CCManiere, &pPh->PrepositionManiere, NSPhraseur::prepManiere);
    traitePrepositionCC(&pPh->CCMoyen,   &pPh->PrepositionMoyen,   NSPhraseur::prepMoyen);
    traitePrepositionCC(&pPh->CCCause,   &pPh->PrepositionCause,   NSPhraseur::prepCause);
    traitePrepositionCC(&pPh->CCBut,     &pPh->PrepositionBut,     NSPhraseur::prepBut);
    traitePrepositionCC(&pPh->CCType,    &pPh->PrepositionType,    NSPhraseur::prepType);
    traitePrepositionCC(&pPh->CCChiffre, &pPh->PrepositionChiffre, NSPhraseur::prepChiffre);


    // Ensuite on classe.
    classeTout();


    // Adverbes du verbe. (stockes dans le complement du verbe.)

    if (!((pPh->Verbe).empty()))
    {
        if ((*(pPh->Verbe.begin()))->getComplementPhr())
        {
            NSPhraseur* pCompVerbe = (*(pPh->Verbe.begin()))->getComplementPhr() ;

            if (!((pCompVerbe->adverbe).empty()))
            {
                NsProposition* pPropos = new NsProposition(pContexte, &pCompVerbe, NsProposition::notSetType, NsProposition::notSetConjonct) ;
                NSGenerateurEn* pGeneAdverbe = new NSGenerateurEn(pContexte, pPropos, sLang);
                pGeneAdverbe->classeTout();

                if (pGeneAdverbe->pAdverbePos)
                {
                    if (!((pGeneAdverbe->pAdverbePos)->empty()))
                    {
                        for (iterMots  = (pGeneAdverbe->pAdverbePos)->begin();
                             iterMots != (pGeneAdverbe->pAdverbePos)->end();
                             iterMots++)
                        {
                            if (!((*iterMots)->estTexteLibre()))
                                sInterPos2 = donneAdverbeComplet(*iterMots);

                            else
                                sInterPos2 = (*iterMots)->getTexteLibre() ;

                            etDuMilieu(&sPhrasePos, &sInterPos1, &sInterPos2);
                        }
                        etFinal(&sPhrasePos, &sInterPos1);

                        sPhraseAdverbe += sPhrasePos;
                    }
                }

                if (pGeneAdverbe->pAdverbeNeg)
                {
                    if (!((pGeneAdverbe->pAdverbeNeg)->empty()))
                    {
                        // s'il existe des adverbes a sens positif.
                        if (sPhrasePos != "")
                        {
                            for (iterMots  = (pGeneAdverbe->pAdverbeNeg)->begin();
                                 iterMots != (pGeneAdverbe->pAdverbeNeg)->end();
                                 iterMots++)
                            {
                                if (!((*iterMots)->estTexteLibre()))
                                    sInterNeg2 = donneAdverbeComplet(*iterMots);

                                else
                                    sInterNeg2 = (*iterMots)->getTexteLibre() ;

                                etDuMilieu(&sPhraseNeg, &sInterNeg1, &sInterNeg2);
                            }
                            etFinal(&sPhraseNeg, &sInterNeg1);

                            sPhraseAdverbe += string(" mais ") + sPhraseNeg;
                        }

                        // s'il n'y a pas d'adverbe a sens positif, la phrase devient
                        // negative.

                        else
                        {
                            bPhraseAffirmative = false;

                            // On prend l'adverbe modificateur du premier adverbe du COD negatif

                            bool bPremier = true;
                            sInter1 = "";
                            sInter2 = "";
                            for (iterMots  = (pGeneAdverbe->pAdverbeNeg)->begin();
                                 iterMots != (pGeneAdverbe->pAdverbeNeg)->end();
                                 iterMots++)
                            {
                                sInterNeg2 = donneNomComplet(*iterMots, sansCertitude);

                                sCertitude = string((*iterMots)->getCertitude(), 0, 5) ;

                                if (sCertitude != "")
                                {
                                    if (bPremier)
                                    {
                                        bPremier = false;
                                        if (sCertitude != "")
                                        {
                                            if (string(sCertitude, 0 ,5) == "WCE00")
                                                sAdverbeModificateur = " ";
                                            else if ((string(sCertitude, 0 ,5) > "WCE00") && (string(sCertitude, 0 ,5) < "WCE50"))
                                                sAdverbeModificateur = " probably ";
                                        }
                                    }
                                    else
                                    {
                                        if (sCertitude != "")
                                        {
                                            if (string(sCertitude, 0 ,5) == "WCE00")
                                                sInterNeg2 = string("not ") + sInter2;
                                            else if ((string(sCertitude, 0 ,5) > "WCE00") && (string(sCertitude, 0 ,5) < "WCE50"))
                                                sInterNeg2 = string("probably not ") + sInter2;
                                        }
                                    }
                                }
                                etDuMilieu(&sPhraseNeg, &sInterNeg1, &sInterNeg2);
                            }
                            etFinal(&sPhraseNeg, &sInterNeg1);

                            sPhraseAdverbe = sPhraseNeg;
                        }
                    }
                }
                delete pGeneAdverbe ;
                delete pPropos ;
            }
        }
    }

    //
    // PREPARATION DES COMPOSANTS
    //

    //
    // Sujet
    //
    string sSujet      = string("") ;
    GENRE  iGenreSujet = genreNull ;
    string sAttSujet   = string("") ;

    if (!(prepareSujet(&sSujet, &iGenreSujet, &sAttSujet, &sAdverbeModificateur, &bPhraseAffirmative, iStyle)))
        return false;

    // On traite le COD.
    // On met d'abord les complements positifs, puis les negatifs, avec "but not de" entre.
    // S'il n'y a que des negatifs, la phrase devient negative.
    //

    string sCOD = "";
    GENRE  iGenreCOD = genreNull ;
    string sAttCOD = "";

    if (!(prepareCOD(&sCOD, &iGenreCOD, &sAttCOD, &sAdverbeModificateur, &bPhraseAffirmative, iStyle)))
        return false;

    //
    // Complements : quand, ou, comment, avec quoi, pourquoi, dans quel but
    //

    pCCArray->donnePhrase();

    //
    // Cas de la phrase principale
    //
    if (pPh->iPhraseType == NSPhraseur::phrasePrincipale)
    {
        if (dcPhrase == iStyle)
        {
            //
            // Cas classique : FORME ACTIVE
            //
            if (pPh->iForme != NSPhraseur::formePassive)
            {

                //
                //     P
                //   /  \
                //  GN  GV
                //
                //
                if (sSujet != "")
                {
                    if (pPh->iTypeSujet == NSPhraseur::sujetNoRepeat)
                    {
                        NSPhraseur::VBPERSO iPersonVb ;
                        if ((genreFP == iGenreSujet) || (genreMP == iGenreSujet) || (genreNP == iGenreSujet))
                            iPersonVb = NSPhraseur::pers3P ;
                        else
                            iPersonVb = NSPhraseur::pers3S ;
                        sPhrase = donnePronomPersonnel(iGenreSujet, iPersonVb, STR_SUJET) ;
                    }
                    else
                        sPhrase = sSujet;

                    // Cas d'une vraie phrase (iStyle == dcPhrase)
                    //
                    //
                    //                P
                    //              /  \         \
                    //             GN  GV        GN prep
                    //                / \
                    //               V  GN (COD)
                    //   ex : My daughter has driven the car on the highway.
                    //
                    // Il n'est pas possible d'avoir à la fois un COD et un attribut
                    // du sujet (qui appartiennent tous deux au GV)

                    if ((sCOD != "") && (sAttSujet != ""))
                        return false;

                    // L'existence d'un COD impose celle d'un verbe
                    if ((sCOD != "") && (pPh->Verbe.empty()))
                        return false;

                    // S'il existe un attribut et pas de verbe, on impose
                    // le verbe être
                    if ((sAttSujet != "") && (pPh->Verbe.empty()))
                    {
                      NSPatPathoData PathoData ;
                      PathoData.setLexique("4ETRE1") ;
                      pPh->Verbe.push_back(new NSPhraseMot(&PathoData, pContexte)) ;
                    }

                    if ((iGenreSujet == genreMP) || (iGenreSujet == genreFP) || (iGenreSujet == genreNP))
                      pPh->iVbPersonne = NSPhraseur::pers3P ;
                    else
                      pPh->iVbPersonne = NSPhraseur::pers3S ;

                    // On traite au passage le cas ou la phrase est negative

                    string sPrincipal, sAuxilliaire;
                    donneVerbe(&sPrincipal, &sAuxilliaire);
                    if (sAuxilliaire != "")
                    {
                        if (!bPhraseAffirmative)
                        {
                            sVerbe += sAuxilliaire + sAdverbeModificateur + string("not");

                            if (sPhraseAdverbe != "")
                                sVerbe += string(" ") + sPhraseAdverbe;

                            if (sPrincipal != "")
                                sVerbe += string(" ") + sPrincipal;
                        }

                        else
                        {
                            sVerbe += sAuxilliaire;
                            if (sPrincipal != "")
                                sVerbe += string(" ") + sPrincipal;

                            if (sPhraseAdverbe != "")
                                sVerbe += string(" ") + sPhraseAdverbe;
                        }
                    }

                    else
                    {
                        // La phrase negative en anglais comporte necessairement
                        // un auxilliaire.

                        if (!bPhraseAffirmative)
                            return false;
                        else
                            sVerbe += sPrincipal;

                        if (sPhraseAdverbe != "")
                            sVerbe += string(" ") + sPhraseAdverbe;
                    }

                    sPhrase += string(" ") + sVerbe;

                    if (sCOD != "")
                        sPhrase += string(" ") + sCOD;
                    if (sAttCOD != "")
                        sPhrase += string(" ") + sAttCOD;
                    if (sAttSujet != "")
                        sPhrase += string(" ") + sAttSujet;
                }

                else // Pas de sujet.
                {
                    //
                    // On s'occupe du verbe
                    //
                    if (!(pPh->Verbe.empty()))
                    {
                        // Creation d'un sujet virtuel "there" pour les verbes d'etat.
                        // Pour les verbes d'action, on passe a la voix passive.
                        //
                        // On mange des pommes.
                        // -> Apples are eaten.


                        string sTransitivite;
                        string sActEtat;

                        if (!donneVerbeClasse(*(pPh->Verbe.begin()), &sTransitivite, &sActEtat))
                            return false;

                        switch (pPh->iVbType)
                        {
                            case NSPhraseur::vbTypeAction :
                            {
                                NsProposition* pPropos = new NsProposition(pContexte, &pPh, NsProposition::notSetType, NsProposition::notSetConjonct) ;
                                NSGenerateurEn* pGenePassif = new NSGenerateurEn(pContexte, pPropos, sLang) ;
                                (pGenePassif->pPh->iForme) = NSPhraseur::formePassive ;
                                bool result = pGenePassif->genereProposition(dcPhrase) ;
                                sPhrase = pGenePassif->getPropositionPhrase() ;
                                delete pGenePassif ;
                                delete pPropos ;
                                return result ;
                            }
                            case NSPhraseur::vbTypeEtat :
                            {
                                sPhrase += string("there");

                                pPh->iVbPersonne = NSPhraseur::pers3S;

                                string sPrincipal, sAuxilliaire;
                                donneVerbe(&sPrincipal, &sAuxilliaire);

                                // Phrase affirmative

                                if (bPhraseAffirmative)
                                {
                                    if (sAuxilliaire != "")
                                        sVerbe = string(" ") + sAuxilliaire;
                                    if (sPrincipal != "")
                                        sVerbe += string(" ") + sPrincipal;

                                    if (sPhraseAdverbe != "")
                                        sVerbe += string(" ") + sPhraseAdverbe;
                                }

                                // Phrase negative

                                else
                                {
                                    if (sAuxilliaire != "")
                                    {
                                        sVerbe += string(" ") + sAuxilliaire + sAdverbeModificateur + string("not");

                                        if (sPhraseAdverbe != "")
                                            sVerbe += string(" ") + sPhraseAdverbe;

                                        if (sPrincipal != "")
                                            sVerbe += string(" ") + sPrincipal;
                                    }

                                    // La phrase anglaise negative comporte toujours un
                                    // auxilliaire.
                                    else
                                        return false;
                                }
                                sPhrase += sVerbe;
                                break;
                            }
                        }
                    }
                    //
                    // Ni verbe, ni sujet -> phrase sur le mode "There is"
                    //
                    else if (string("") != sCOD)
                    {
                      NSPatPathoData PathoData ;
                      PathoData.setLexique("4ETRE1") ;
                      pPh->Verbe.push_back(new NSPhraseMot(&PathoData, pContexte)) ;

                      sPhrase += string("there") ;
                      pPh->iVbPersonne = NSPhraseur::pers3S ;

                      string sPrincipal, sAuxilliaire ;
                      donneVerbe(&sPrincipal, &sAuxilliaire) ;
                      if (bPhraseAffirmative)
                      {
                        if (sAuxilliaire != "")
                          sVerbe = string(" ") + sAuxilliaire ;
                        if (sPrincipal != "")
                          sVerbe += string(" ") + sPrincipal ;
                      }

                      // Phrase negative

                      else
                      {
                            if (sAuxilliaire != "")
                            {
                                sVerbe += string(" ") +  sAuxilliaire + sAdverbeModificateur + string("not");
                                if (sPrincipal != "")
                                    sVerbe += string(" ") + sPrincipal;
                            }
                            else
                                return false;
                        }
                        sPhrase += sVerbe;
                        if (sCOD != "")
                            sPhrase += string(" ") + sCOD;
                        if (sAttCOD != "")
                            sPhrase += string(" ") + sAttCOD;
                    }
                }
            }
            //
            // FORME PASSIVE
            //
            // Le sujet et le COD inversent leur rôle ; le temps est donne par
            // le verbe etre, le verbe est au participe passe
            //
            // Ex The cat eats the mouse. -> The mouse is eaten by the cat.
            //
            else
            {
                // Il faut forcement un COD, sinon on se retrouve sans sujet
                if (sCOD == "")
                    return false;
                // Il faut forcement un verbe
                if (pPh->Verbe.empty())
                    return false;

                if (pPh->iTypeSujet == NSPhraseur::sujetNoRepeat)
                {
                    NSPhraseur::VBPERSO iPersonVb ;
                    if ((genreFP == iGenreCOD) || (genreMP == iGenreCOD) || (genreNP == iGenreCOD))
                        iPersonVb = NSPhraseur::pers3P;
                    else
                        iPersonVb = NSPhraseur::pers3S;
                    sPhrase = donnePronomPersonnel(iGenreSujet, iPersonVb, STR_SUJET) ;
                }
                else
                    sPhrase = sCOD;

                //
                // On prend le verbe au participe passe, c'est le verbe etre
                // qui est conjugue
                //
                string sVb = donneParticipePasse(*(pPh->Verbe.begin()), iGenreCOD);

                // On ne peut pas faire de phrase passive avec des verbes qui
                // n'ont pas de participe passe comme "to can".

                if (string("") == sVb)
                  return false ;

                pPh->Verbe.vider() ;
                NSPatPathoData PathoData ;
                PathoData.setLexique("4ETRE1") ;
                pPh->Verbe.push_back(new NSPhraseMot(&PathoData, pContexte)) ;

                if ((iGenreSujet == genreMP) || (iGenreSujet == genreFP) || (iGenreSujet == genreNP))
                  pPh->iVbPersonne = NSPhraseur::pers3P ;
                else
                  pPh->iVbPersonne = NSPhraseur::pers3S ;

                string sPrincipal, sAuxilliaire ;
                donneVerbe(&sPrincipal, &sAuxilliaire) ;

                // Phrase affirmative

                if (bPhraseAffirmative)
                {
                    if (sAuxilliaire != "")
                        sVerbe = string(" ") + sAuxilliaire;
                    if (sPrincipal != "")
                        sVerbe += string(" ") + sPrincipal;
                }

                // Phrase negative

                else
                {
                    if (sAuxilliaire != "")
                    {
                        sVerbe += string(" ") + sAuxilliaire + sAdverbeModificateur + string("not");
                        if (sPrincipal != "")
                            sVerbe += string(" ") + sPrincipal;
                    }
                    else
                    {
                        return false;
                    }
                }

                if (bPhraseAffirmative)
                {
                    sPhrase += sVerbe + string(" ") + sVb;

                    if (sPhraseAdverbe != "")
                        sPhrase += string(" ") + sPhraseAdverbe;
                }
                else
                {
                    sPhrase += sVerbe;

                    if (sPhraseAdverbe != "")
                        sPhrase += string(" ") + sPhraseAdverbe;

                    sPhrase += string(" ") + sVb;
                }

                // I've found your propositions interesting.
                // -> Your propositions have been found interesting.


                if (sAttCOD != "")
                    sPhrase += string(" ") + sAttCOD;

                //
                // S'il y a un sujet, on ajoute "by" devant.
                // Il peut ne pas y avoir de sujet, par exemple
                // "The car has been stolen by someone."
                //
                if (sSujet != "")
                    sPhrase += string(" by ") + sSujet;
            }
        }

        else if (dcTiret == iStyle)
        {
            // C'est comme la forme passive, sauf qu'on enleve le verbe etre, et
            // qu'on enleve les articles du COD.

            // Deux cas de figure :
            // - "vraie phrase" et il faut un COD
            // - enumeration de valeurs chiffrees

            // Il faut forcement un COD, ou des valeurs chiffrees
            if ((sCOD == "") && (pPh->CCChiffre.empty()))
                return false;

            // S'il y a un COD mais pas de verbe, c'est juste une enumeration.
            if (sCOD != "")
                sPhrase = sCOD;

            if (!(pPh->Verbe.empty()))
            {
                // Il faut forcement un COD, sinon on se retrouve sans sujet
                if (sCOD == "")
                    return false;

                //
                // On prend le verbe au participe passe
                //
                string sVb = donneParticipePasse(*(pPh->Verbe.begin()), iGenreCOD);

                if (sVb == "")
                    return false;

                // Si la phrase est negative, on ajoute "not", ou "absolutely not" etc...
                // juste avant le participe passe.

                if (!bPhraseAffirmative)
                {
                    sPhrase += string(" ") + sAdverbeModificateur + string("not ");

                    if (sPhraseAdverbe != "")
                        sPhrase += sPhraseAdverbe += string(" ");

                    sPhrase += sVb;
                }
                else
                {
                    sPhrase += string(" ") + sVb ;

                    if (sPhraseAdverbe != "")
                        sPhrase += string(" ") + sPhraseAdverbe;
                }

                // Propostions found interesting by Jean.

                if (sAttCOD != "")
                    sPhrase += string(" ") + sAttCOD;

                //
                // S'il y a un sujet, on ajoute "par" devant.
                // Il peut ne pas y avoir de sujet, par exemple
                // "La vesicule est etudiee en decubitus dorsal"
                //
                if (sSujet != "")
                    sPhrase += string(" by ") + sSujet;
            }
        }
    }
    //
    // Complements
    //
    // Penser a pouvoir forcer l'ordre (importance) des Complements

    if (!(pCCArray->empty()))
    {
        for (GenCompIter itCompl = pCCArray->begin(); itCompl != pCCArray->end(); itCompl++)
        {
            if ((*itCompl)->sPositif != "")
            {
                sPhrase += " " + (*itCompl)->sPositif;
                if ((*itCompl)->sNegatif != "")
                    sPhrase += string(" but not ") + (*itCompl)->sNegatif;
            }
            else if ((*itCompl)->sNegatif != "")
                sPhrase += string(" not ") + (*itCompl)->sNegatif;
        }
    }

    //
    // Cas de la subordonnee relative.
    //
    if (pPh->iPhraseType == NSPhraseur::phraseRelative)
    {
        if (dcPhrase == iStyle)
        {
            //
            // Cas classique : FORME ACTIVE
            //
            if (pPh->iForme != NSPhraseur::formePassive)
            {

                //
                //     P
                //   /  \
                //  GN  GV
                //
                //
                if (sSujet != "")
                {
                    // Le pronom qui introduit la subordonnee relative depend du genre
                    // du sujet. Si celui-ci est un homme (ou une femme), la proposition
                    // est introduite par "who". Si c'est un objet (genre neutre), elle
                    // est introduite par "which".

                    if ((iGenreSujet == genreMS) || (iGenreSujet == genreMP) ||
                    		(iGenreSujet == genreFS) || (iGenreSujet == genreFP))

                    		sPhrase = string(", who");

                    if ((iGenreSujet == genreNS) || (iGenreSujet == genreNP))

                    		sPhrase = string(", which");

                    // Cas d'une vraie phrase (iStyle == dcPhrase)
                    //
                    //
                    //                P
                    //              /  \         \
                    //             GN  GV        GN prep
                    //                / \
                    //               V  GN (COD)
                    //   ex : My daughter who has driven the car on the highway.
                    //
                    // Il n'est pas possible d'avoir à la fois un COD et un attribut
                    // du sujet (qui appartiennent tous deux au GV)

                    if ((sCOD != "") && (sAttSujet != ""))
                        return false;

                    // L'existence d'un COD impose celle d'un verbe
                    if ((sCOD != "") && (pPh->Verbe.empty()))
                        return false;

                    // S'il existe un attribut et pas de verbe, on impose
                    // le verbe être
                    if ((sAttSujet != "") && (pPh->Verbe.empty()))
                    {
                      NSPatPathoData PathoData ;
                      PathoData.setLexique("4ETRE1") ;
                      pPh->Verbe.push_back(new NSPhraseMot(&PathoData, pContexte)) ;
                    }

                    if ((iGenreSujet == genreMP) || (iGenreSujet == genreFP)
                    		|| (iGenreSujet == genreNP))

                        pPh->iVbPersonne = NSPhraseur::pers3P;
                    else
                        pPh->iVbPersonne = NSPhraseur::pers3S;

                    // On traite au passage le cas ou la phrase est negative

                    string sPrincipal, sAuxilliaire;
                    donneVerbe(&sPrincipal, &sAuxilliaire);
                    if (sAuxilliaire != "")
                    {
                        if (!bPhraseAffirmative)
                        {
                            sVerbe += sAuxilliaire + sAdverbeModificateur + string("not");

                            if (sPhraseAdverbe != "")
                                sVerbe += string(" ") + sPhraseAdverbe;

                            if (sPrincipal != "")
                                sVerbe += string(" ") + sPrincipal;
                        }

                        else
                        {
                            sVerbe += sAuxilliaire;
                            if (sPrincipal != "")
                                sVerbe += string(" ") + sPrincipal;

                            if (sPhraseAdverbe != "")
                                sVerbe += string(" ") + sPhraseAdverbe;

                        }
                    }

                    else
                    {
                        // La phrase negative en anglais comporte necessairement
                        // un auxilliaire.

                        if (!bPhraseAffirmative)
                            return false;
                        else
                            sVerbe += sPrincipal;

                        if (sPhraseAdverbe != "")
                            sVerbe += string(" ") + sPhraseAdverbe;
                    }

                    sPhrase += string(" ") + sVerbe;

                    if (sCOD != "")
                        sPhrase += string(" ") + sCOD;
                    if (sAttCOD != "")
                        sPhrase += string(" ") + sAttCOD;
                    if (sAttSujet != "")
                        sPhrase += string(" ") + sAttSujet;
                }

                else // Pas de sujet.
                {
                	  // Impossible dans le cadre d'une subordonnee relative.
                    return false;
                }
            }
            //
            // FORME PASSIVE
            //
            // On ne la traite pas encore
            else
            {
                return false;
            }
        }
        else if (dcTiret == iStyle)
        {
            return false;
        }
    }


    // A la fin, on rajoute la phrase qui se place apres les " : ".

    if (pPh->pDeuxPoints)
    {
        NsProposition* pPropos = new NsProposition(pContexte, &(pPh->pDeuxPoints), NsProposition::notSetType, NsProposition::notSetConjonct) ;
        NSGenerateurEn* pGeneDeuxPoints = new NSGenerateurEn(pContexte, pPropos, sLang);
        pGeneDeuxPoints->genereProposition(iStyle);
        sPhrase += string(": ") + pGeneDeuxPoints->getPropositionPhrase();
        delete pGeneDeuxPoints ;
        delete pPropos ;
    }


    // Elimination des eventuels blancs initiaux
    strip(sPhrase, stripLeft);
    //while ((strlen(sPhrase.c_str()) > 0) && (sPhrase[0] == ' '))
    //	sPhrase = string(sPhrase, 1, strlen(sPhrase.c_str())-1);

    sPhrase = postTraitement(&sPhrase);

    return true ;
}

bool
NSGenerateurEn::assembleProposition(DCODETYPE iStyle, NsProposition* pPropos)
{
    NsProposition* pCurrentProp ;
    if (pPropos)
        pCurrentProp = pPropos ;
    else
        pCurrentProp = pProposition ;

    NSPropositionArray* pPropArray = (static_cast<NSPropositionArray*>(pCurrentProp->pProposition)) ;
    if (pPropArray->empty())
        return true ;

    iterProposition i = pPropArray->begin() ;
    pCurrentProp->sPhrase = (*i)->sPhrase ;

    while (i != pPropArray->end())
    {
        NsProposition* pItProp = *i ;
        i++ ;

        if ((*i)->sPhrase != "")
        {
            switch ((*i)->iPropositionType)
            {
                case NsProposition::principale :
                    if (i != pPropArray->end())
                        pCurrentProp->sPhrase += string(", ") + (*i)->sPhrase ;
                    else
                        pCurrentProp->sPhrase += string(" and ") + (*i)->sPhrase ;
                    break ;
                    //
                    // Completives
                    //
                case NsProposition::completiveQue :                          // SCQ : Je veux que tu reviennes
                    pCurrentProp->sPhrase += string(" that ") + (*i)->sPhrase ;
                    break ;
                case NsProposition::completiveInfinitive :                   // SCI : J'entends les chats miauler
                    pCurrentProp->sPhrase += (*i)->sPhrase ;
                    break ;
                case NsProposition::completiveInterrogative :                // SCN : Je voudrais savoir pourquoi tu ne veux pas
                    pCurrentProp->sPhrase += string(" why ") + (*i)->sPhrase ;
                    break ;
                //
                // Circonstancielles
                //
                // - Temporelle
                case NsProposition::circonstancielleTemporelleAnterieure :   // SCTA : Je le verrai avant qu'il chante
                    switch ((*i)->iConjonctionType)
                    {
                        case NsProposition::SCTA_AvantQue :        // ---X---|
                            pCurrentProp->sPhrase += string(" before ") + (*i)->sPhrase ;
                            break ;
                        case NsProposition::SCTA_JusteAvantQue :   // ------X|
                            pCurrentProp->sPhrase += string(" just before ") + (*i)->sPhrase ;
                            break ;
                        case NsProposition::SCTA_JusquAceQue :     // --XXXXX|
                            pCurrentProp->sPhrase += string(" until ") + (*i)->sPhrase ;
                            break ;
                        default :
                            pCurrentProp->sPhrase += string(" before ") + (*i)->sPhrase ;
                    }
                    break ;
                case NsProposition::circonstancielleTemporelleSimultanee :   // SCTS : Je chantais au moment où il arriva
                    pCurrentProp->sPhrase += string(" at the moment when ") + (*i)->sPhrase ;
                    break ;
                case NsProposition::circonstancielleTemporellePosterieure :  // SCTP : Je chanterai après que vous serez parti
                    switch ((*i)->iConjonctionType)
                    {
                        case NsProposition::SCTP_ApresQue :          // ---|---X
                            pCurrentProp->sPhrase += string(" after ") + (*i)->sPhrase ;
                            break ;
                        case NsProposition::SCTP_DesQue :            // ---|X---
                            pCurrentProp->sPhrase += string(" as soons as ") + (*i)->sPhrase ;
                            break ;
                        case NsProposition::SCTP_DepuisQue :         // ---|XXXX
                            pCurrentProp->sPhrase += string(" since ") + (*i)->sPhrase ;
                            break ;
                        default :
                            pCurrentProp->sPhrase += string(" before ") + (*i)->sPhrase ;
                    }
                    break ;
                // - Consecutive -> resultat de l'action
                case NsProposition::circonstancielleConsecutive :            // SCR : Il fait trop froid pour que je chante
                    pCurrentProp->sPhrase += string(" for ") + (*i)->sPhrase ;
                    break ;
                // - Causale -> cause de l'action
                case NsProposition::circonstancielleCausale :                // SCC : Il a reussi parce qu'il chantait bien
                    pCurrentProp->sPhrase += string(" because ") + (*i)->sPhrase ;
                    break ;
                // - Concessive -> relation inattendue
                case NsProposition::circonstancielleConcessive :             // SCS : Il a echoue bien qu'il chanta bien
                    pCurrentProp->sPhrase += string(" even if ") + (*i)->sPhrase ;
                    break ;
                // - Finale -> but de l'action
                case NsProposition::circonstancielleFinale :                 // SCF : Il chante fort pour qu'on l'entende bien
                    pCurrentProp->sPhrase += string(" in order to ") + (*i)->sPhrase ;
                    break ;
                // - Comparative -> comparaison
                case NsProposition::circonstancielleComparativeProportion :  // SCCP : Vous chanterez d'autant plus fort que vous serez bien echauffe
                    switch ((*i)->iConjonctionType)
                    {
                        case NsProposition::SCCP_AutantQuantite :    // suivant, selon, à mesure
                            pCurrentProp->sPhrase += string(" as much as ") + (*i)->sPhrase ;
                            break ;
                        case NsProposition::SCCP_Synchrone :         // au fur et à mesure
                            pCurrentProp->sPhrase += string(" at the same time ") + (*i)->sPhrase ;
                            break ;
                        case NsProposition::SCCP_PlusQuantite :      // d'autant plus
                            pCurrentProp->sPhrase += string(" as much as ") + (*i)->sPhrase ;
                            break ;
                        case NsProposition::SCCP_PlusVite :          // d'autant plus vite
                            pCurrentProp->sPhrase += string(" the faster as ") + (*i)->sPhrase ;
                            break ;
                        case NsProposition::SCCP_MoinsQuantite :      // d'autant moins
                            pCurrentProp->sPhrase += string(" as less as ") + (*i)->sPhrase ;
                            break ;
                        case NsProposition::SCCP_MoinsVite :          // d'autant moins vite
                            pCurrentProp->sPhrase += string(" the slower as ") + (*i)->sPhrase ;
                            break ;
                    }
                    break ;
                case NsProposition::circonstancielleComparativeEgalite :     // SCCE : Vous chantez aussi fort que moi
                    switch ((*i)->iConjonctionType)
                    {
                        case NsProposition::SCCE_Aspect :            // tel que, de même que, si ... que
                            pCurrentProp->sPhrase += string(" as ") + (*i)->sPhrase ;
                            break ;
                        case NsProposition::SCCE_Quantite :          // autant que, aussi que
                            pCurrentProp->sPhrase += string(" as much as ") + (*i)->sPhrase ;
                            break ;
                        case NsProposition::SCCE_Temps :             // tant que
                            pCurrentProp->sPhrase += string(" until ") + (*i)->sPhrase ;
                            break ;
                    }
                    break ;
                case NsProposition::circonstancielleComparativeRessemblance : // SCCR : Tu me regardes comme me regardait ta mère
                    pCurrentProp->sPhrase += string(" in the same way as ") + (*i)->sPhrase ;
                    break ;
                // - Conditionnelle -> condition
                case NsProposition::circonstancielleConditionnelleHypothese : // SCHH : il mange s'il a faim, il mangera s'il a faim
                    pCurrentProp->sPhrase += string(" if ") + (*i)->sPhrase ;
                    break ;
                case NsProposition::circonstancielleConditionnelleImaginee :  // SCHH : il mangerait s'il avait faim
                    pCurrentProp->sPhrase += string(" if ") + (*i)->sPhrase ;
                    break ;
                default :
                    pCurrentProp->sPhrase += string(", ") + (*i)->sPhrase ;
            }
        }
    }
    return true ;
}

bool
NSGenerateurEn::prepareSujet(string* psSujet, GENRE* piGenreSujet, string* psAttSujet, string* psAdverbeModificateur, bool* pbPhraseAffirmative, DCODETYPE iStyle)
{
    bool     trouve;
    NSSuper* pSuper = pContexte->getSuperviseur();
    string   sLexique;
    string   sPluriel;
    string   sCertitude;
    string   sInter1 = "";
    string   sInter2 = "";
    GENRE    iGenre;

    string   sAdverbeModificateurAtt = " ";

    iterPhraseMot  iterMots;
    NSPathologData Data;

    if (!(pPh->Sujet.empty()))
    {
        sInter1 = "";
        sInter2 = "";
        for (iterMots = pPh->Sujet.begin(); iterMots != pPh->Sujet.end(); iterMots++)
        {
            sLexique = (*iterMots)->getLexique() ;

            if (false == (*iterMots)->estTexteLibre())
            {
                trouve = pContexte->getDico()->trouvePathologData(sLang, &sLexique, &Data);
                sPluriel = (*iterMots)->getPluriel() ;
                if (!trouve)
                    return false;
                if (!(Data.estNom()))
                    return false;
                Data.donneGenre(&iGenre);

                // Recuperation du genre (composition du genre recupere avec
                // le genre dejà existant)

                if (sPluriel != "")
                    Data.donneGenrePluriel(&iGenre);

                string sForceNbr = (*iterMots)->forceNombre(this);

                if (sForceNbr == FORCE_PLUR)
                    Data.donneGenrePluriel(&iGenre);
                else if (sForceNbr == FORCE_SING)
                    Data.donneGenreSingulier(&iGenre);
            }
            else
                iGenre = genreMS;

            if (genreNull == *piGenreSujet)
                *piGenreSujet = iGenre;
            else
            {
                // En anglais, le genre n'a aucune importance au pluriel.
                // On met donc par defaut le genre MP au pluriel.
                // En fait, on compose quand meme les genres, de maniere a ce que
                // l'on puisse choisir le bon pronom pour introduire les subordonnees
                // relatives.

                if (((*piGenreSujet == genreNS) || (*piGenreSujet == genreNP)) &&
                     (Data.estNeutre()))
                    *piGenreSujet = genreNP;

                else if (((*piGenreSujet == genreFS) || (*piGenreSujet == genreFP) ||
                	   	  (*piGenreSujet == genreNS) || (*piGenreSujet == genreNP))
                     	 && (Data.estFeminin()))
                    *piGenreSujet = genreFP;
                else
                    *piGenreSujet = genreMP;
            }

            // L'article depend du statut d'objet reel ou abstrait

            if (((sPluriel == "") && (Data.estReel())) || (string(sPluriel, 0, 5) == "WPLUR"))
                sInter2 = donneNomComplet(*iterMots, NSPhraseMot::articleDefini);
            else if (((sPluriel == "") && (!Data.estReel())) || (string(sPluriel, 0, 5) == "WPLUS"))
                sInter2 = donneNomComplet(*iterMots, NSPhraseMot::articleIndefini);

            etDuMilieu(psSujet, &sInter1, &sInter2);
        }
        etFinal(psSujet, &sInter1);
    }
    // L'attribut du sujet est un adjectif qualificatif qui s'accorde avec le sujet
    //
    // ex : La valve est dilatee
    //
    // L'attribut doit être un adjectif
    //
    // On met d'abord les attributs positifs, puis les negatifs, avec "but not" entre.
    // S'il n'y a que des negatifs, la phrase devient negative.
    //

    string sAttSujet = "";

    string sAttSujetPos = "";
    if (pAttSujetPos)
    {
        if (!(pAttSujetPos->empty()))
        {
            if (pPh->Sujet.empty())
                return false;
            sInter1 = "";
            sInter2 = "";
            for (iterMots = pAttSujetPos->begin(); iterMots != pAttSujetPos->end(); iterMots++)
            {
                if (!((*iterMots)->estTexteLibre()))
                {
                    // On impose le genre du sujet
                    sInter2 = donneAdjectifComplet(*iterMots, *piGenreSujet);
                }
                else
                    sInter2 = (*iterMots)->getTexteLibre() ;

                etDuMilieu(&sAttSujetPos, &sInter1, &sInter2);
            }
            etFinal(&sAttSujetPos, &sInter1);
        }
    }

    string sAttSujetNeg = "";
    if (pAttSujetNeg)
    {
        if (!(pAttSujetNeg->empty()))
        {
            if (pPh->Sujet.empty())
                return false;
            sInter1 = "";
            sInter2 = "";
            bool bPremier = true;

            for (iterMots = pAttSujetNeg->begin(); iterMots != pAttSujetNeg->end(); iterMots++)
            {
                // On impose le genre du sujet
                sInter2 = donneAdjectifComplet(*iterMots, genreNull, sansCertitude);

                sCertitude = string((*iterMots)->getCertitude(), 0, 5) ;

                if (bPremier)
                {
                    bPremier = false;
                    if (sCertitude != "")
                    {
                        if (sCertitude == "WCE00")
                            sAdverbeModificateurAtt = " ";
                        else if ((sCertitude > "WCE00") && (sCertitude < "WCE50"))
                            sAdverbeModificateurAtt = " probably ";
                    }
                }
                else
                {
                    if (sCertitude != "")
                    {
                        if (sCertitude == "WCE00")
                            sInter2 = string("not ") + sInter2;
                        else if ((sCertitude > "WCE00") && (sCertitude < "WCE50"))
                            sInter2 = string("probably not ") + sInter2;
                    }
                }

                etDuMilieu(&sAttSujetNeg, &sInter1, &sInter2);
            }
            etFinal(&sAttSujetNeg, &sInter1);
        }
    }

    if (sAttSujetPos == "")
    {
        if (sAttSujetNeg != "")
        {
            if (*pbPhraseAffirmative)
            {
                *pbPhraseAffirmative = false ;
                *psAttSujet = sAttSujetNeg;
                *psAdverbeModificateur = sAdverbeModificateurAtt;
            }
            else
            {
                // il y a deja un adverbe du verbe qui rend la phrase negative.
                *psAttSujet = sAdverbeModificateurAtt + string("but ") + sAttSujetNeg;
                // on enlève les blancs initaux.
                strip(*psAttSujet, stripLeft);
            }
        }
    }
    else
    {
        *psAttSujet = sAttSujetPos;
        if (sAttSujetNeg != "")
            *psAttSujet += string(" but") + sAdverbeModificateurAtt + string("not ") + sAttSujetNeg ;
    }
    return true;
}

bool
NSGenerateurEn::prepareCOD(string* psCOD, GENRE* piGenreCOD, string* psAttCOD, string* psAdverbeModificateur, bool* pbPhraseAffirmative, DCODETYPE iStyle)
{
  bool     trouve ;
  NSSuper* pSuper = pContexte->getSuperviseur() ;
  string   sLexique ;
  string   sPluriel ;
  string   sCertitude ;
  string   sInter1 = "" ;
  string   sInter2 = "" ;
  GENRE    iGenre ;

  iterPhraseMot  iterMots ;
  NSPathologData Data ;

  *psCOD      = "" ;
  *psAttCOD   = "" ;
  *piGenreCOD = genreNull ;

    //
    // On met d'abord les complements positifs, puis les negatifs, avec "but not de" entre.
    // S'il n'y a que des negatifs, la phrase devient negative.
    //

    // D'abord on va recuperer le genre du COD

    if (!(pPh->COD.empty()))
    {
        for (iterMots = pPh->COD.begin(); iterMots != pPh->COD.end(); iterMots++)
        {
            sLexique = (*iterMots)->getLexique() ;

            if (false == (*iterMots)->estTexteLibre())
            {
                sPluriel = (*iterMots)->getPluriel() ;
                trouve = pContexte->getDico()->trouvePathologData(sLang, &sLexique, &Data);
                if (!trouve)
                    return false ;
                if (false == Data.estNom())
                    return false;
                // Recuperation du genre (composition du genre recupere avec
                // le genre deja existant)
                Data.donneGenre(&iGenre);
                if (sPluriel != "")
                    Data.donneGenrePluriel(&iGenre);

                string sForceNbr = (*iterMots)->forceNombre(this);
                if (sForceNbr == FORCE_PLUR)
                    Data.donneGenrePluriel(&iGenre);
                else if (sForceNbr == FORCE_SING)
                    Data.donneGenreSingulier(&iGenre);
            }
            else
                iGenre = genreMS;

            if (genreNull == *piGenreCOD)
                *piGenreCOD = iGenre;
            else
            {
                if (((*piGenreCOD == genreNS) || (*piGenreCOD == genreNP)) &&
                     (Data.estNeutre()))
                    *piGenreCOD = genreNP;

                else if (((*piGenreCOD == genreFS) || (*piGenreCOD == genreFP) ||
                	   	  (*piGenreCOD == genreNS) || (*piGenreCOD == genreNP))
                     	 && (Data.estFeminin()))
                    *piGenreCOD = genreFP;
                else
                    *piGenreCOD = genreMP;
            }
        }
    }

    // On traite ensuite successivement les non du COD a certitude positive, puis ceux
    // a certitude negative.
    // Pour une dcTiret, il ne faut pas mettre d'article dans le COD.

    string sCODPos = "";

    if (pCODPos)
    {
        if (!(pCODPos->empty()))
        {
            sInter1 = "";
            sInter2 = "";
            for (iterMots = pCODPos->begin(); iterMots != pCODPos->end(); iterMots++)
            {
                if (dcPhrase == iStyle)
                {
                    if (!((*iterMots)->estTexteLibre()))
                    {
                        if (sPluriel == "")
                            sInter2 = donneNomComplet(*iterMots, (*iterMots)->getArticle());

                        else if (string(sPluriel, 0, 5) == "WPLUR")
                            sInter2 = donneNomComplet(*iterMots, NSPhraseMot::articleDefini);
                        else if (string(sPluriel, 0, 5) == "WPLUS")
                            sInter2 = donneNomComplet(*iterMots, NSPhraseMot::articleIndefini);
                    }
                    else
                        sInter2 = (*iterMots)->getTexteLibre() ;
                }

                else if (dcTiret == iStyle)
                {
                    if (!((*iterMots)->estTexteLibre()))
                    {
                        sInter2 = donneNomComplet(*iterMots, NSPhraseMot::articleSans);
                    }
                    else
                        sInter2 = (*iterMots)->getTexteLibre() ;
                }

                etDuMilieu(&sCODPos, &sInter1, &sInter2);
            }
            etFinal(&sCODPos, &sInter1);
            *psCOD += sCODPos ;
        }
    }

    string sCODNeg = "";

    if (pCODNeg)
    {
        if (!(pCODNeg->empty()))
        {
            if ((sCODPos != "") || (!(*pbPhraseAffirmative)))
            {
                sInter1 = "";
                sInter2 = "";
                for (iterMots = pCODNeg->begin(); iterMots != pCODNeg->end(); iterMots++)
                {
                    if (dcPhrase == iStyle)
                    {
                        if (sPluriel == "")
                            sInter2 = donneNomComplet(*iterMots, (*iterMots)->getArticle());

                        else if (string(sPluriel, 0, 5) == "WPLUR")
                            sInter2 = donneNomComplet(*iterMots, NSPhraseMot::articleDefini);
                        else if (string(sPluriel, 0, 5) == "WPLUS")
                            sInter2 = donneNomComplet(*iterMots, NSPhraseMot::articleIndefini);
                    }

                    else if (dcTiret == iStyle)
                        sInter2 = donneNomComplet(*iterMots, NSPhraseMot::articleSans);

                    etDuMilieu(&sCODNeg, &sInter1, &sInter2);
                }
                etFinal(&sCODNeg, &sInter1);
                *psCOD += string(" but ") + sCODNeg;
            }

            // Pas de COD a sens affirmatif
            else
            {
                *pbPhraseAffirmative = false;

                // On prend l'adverbe modificateur du premier nom du COD negatif

                bool bPremier = true;
                sInter1 = "";
                sInter2 = "";
                for (iterMots = pCODNeg->begin(); iterMots != pCODNeg->end(); iterMots++)
                {
                    if (dcPhrase == iStyle)
                    {
                        if (sPluriel == "")
                            sInter2 = donneNomComplet(*iterMots, (*iterMots)->getArticle(), sansCertitude);

                        else if (string(sPluriel, 0, 5) == "WPLUR")
                            sInter2 = donneNomComplet(*iterMots, NSPhraseMot::articleDefini, sansCertitude);
                        else if (string(sPluriel, 0, 5) == "WPLUS")
                            sInter2 = donneNomComplet(*iterMots, NSPhraseMot::articleIndefini, sansCertitude);

                        sCertitude = string((*iterMots)->getCertitude(), 0, 5);

                        if (sCertitude != "")
                        {
                            if (bPremier)
                            {
                                bPremier = false;
                                if (sCertitude != "")
                                {
                                    if (string(sCertitude, 0 ,5) == "WCE00")
                                        *psAdverbeModificateur = " ";
                                    else if ((string(sCertitude, 0 ,5) > "WCE00") && (string(sCertitude, 0 ,5) < "WCE50"))
                                        *psAdverbeModificateur = " probably ";
                                }
                            }
                            else
                            {
                                if (sCertitude != "")
                                {
                                    if (string(sCertitude, 0 ,5) == "WCE00")
                                        sInter2 = string("not ") + sInter2;
                                    else if ((string(sCertitude, 0 ,5) > "WCE00") && (string(sCertitude, 0 ,5) < "WCE50"))
                                        sInter2 = string("probably not ") + sInter2;
                                }
                            }
                        }
                    }

                    etDuMilieu(&sCODNeg, &sInter1, &sInter2);
                }
                etFinal(&sCODNeg, &sInter1);
                *psCOD = sCODNeg;
            }
        }
    }

    // Attribut du COD (se place apres les adjectifs qualificatifs qualifiant le COD).

    string sAttCODPos = "";

    if (pAttCODPos)
    {
        if (!(pAttCODPos->empty()))
        {
            if (pPh->COD.empty())
                return false;
            sInter1 = "";
            sInter2 = "";
            for (iterMots = pAttCODPos->begin(); iterMots != pAttCODPos->end(); iterMots++)
            {
                 if (!((*iterMots)->estTexteLibre()))
                {
                    // On impose le genre du COD
                    sInter2 = donneAdjectifComplet(*iterMots, *piGenreCOD);
                }
                else
                    sInter2 = (*iterMots)->getTexteLibre() ;

                etDuMilieu(&sAttCODPos, &sInter1, &sInter2);
            }
            etFinal(&sAttCODPos, &sInter1);
        }
    }

    string sAttCODNeg = "";
    if (pAttCODNeg)
    {
        if (!(pAttCODNeg->empty()))
        {
            if (pPh->COD.empty())
                return false;
            sInter1 = "";
            sInter2 = "";
            for (iterMots = pAttCODNeg->begin(); iterMots != pAttCODNeg->end(); iterMots++)
            {
                // On impose le genre du COD
                sInter2 = donneAdjectifComplet(*iterMots);

                etDuMilieu(&sAttCODNeg, &sInter1, &sInter2);
            }
            etFinal(&sAttCODNeg, &sInter1);
        }
    }

    if (sAttCODPos == "")
    {
        if (sAttCODNeg != "")
        {
            *psAttCOD = sAttCODNeg;
        }
    }
    else
    {
        *psAttCOD = sAttCODPos;
        if (sAttCODNeg != "")
            *psAttCOD += string(" but ") + sAttCODNeg ;
    }
    return true;
}

string
NSGenerateurEn::donneNomComplet(NSPhraseMot* pMot, int iArticle, GERECERTITUDE iGereCertitude)
{
  string sLibel = "" ;

  NSPathologData Data ;

  iterPhraseMot iterMots ;

  string sCertitude;

  string sLexique = pMot->getLexique() ;
  string sPluriel = pMot->getPluriel() ;

  bool bPluriel = false ;
  string sPrep = "" ;
  NSSuper* pSuper = pContexte->getSuperviseur() ;

  bool trouve = pContexte->getDico()->trouvePathologData(sLang, &sLexique, &Data) ;
  if (!trouve)
      return "";

  // Recuperation du genre (composition du genre recupere avec
  // le genre deja existant)
  //
  GENRE iGenre ;
  Data.donneGenre(&iGenre) ;
  if (string("") != sPluriel)
    Data.donneGenrePluriel(&iGenre) ;

  // On regarde si le nombre est force.

  string sForceNbr = pMot->forceNombre(this);

  if (sForceNbr == FORCE_PLUR)
      Data.donneGenrePluriel(&iGenre);
  else if (sForceNbr == FORCE_SING)
      Data.donneGenreSingulier(&iGenre);

  //
  donneLibelleAffiche(&sLibel, &Data, iGenre);

  if (pMot->getComplementPhr())
  {
      NsProposition* pPropos = new NsProposition(pContexte, pMot->getComplementPhr(), NsProposition::notSetType, NsProposition::notSetConjonct) ;
      NSGenerateurEn* pGeneCpl = new NSGenerateurEn(pContexte, pPropos, sLang) ;
      traitePostposition(pMot) ;

      // Le complement du nom est necessairement introduit par une preposition.
      // Donc si on n'a pas de preposition, on en met une par defaut : "of".
      if (false == pMot->getComplementPhr()->compNom.empty())
      {
          for (iterMots = pMot->getComplementPhr()->compNom.begin();
               iterMots != pMot->getComplementPhr()->compNom.end();
               iterMots++)
          {
              if (false == (*iterMots)->estTexteLibre())
              {
                  if ((*iterMots)->getPreposition() == "")
                      (*iterMots)->setPreposition("of") ;
              }
              else
              {
                  if ((*iterMots)->getPreposition() == "")
                      (*iterMots)->setTexteLibre(string("of ") + (*iterMots)->getTexteLibre()) ;
              }
          }
      }

      if (pGeneCpl->genereNomComplet(pMot, iArticle, iGereCertitude))
          sLibel = pGeneCpl->getTempoPhrase() ;
      delete pGeneCpl ;
      delete pPropos ;
  }

  else
  {
      if (iGereCertitude == avecCertitude)
      {
          sCertitude = string(pMot->getCertitude(), 0, 5) ;
          string sCertitudeEcrit = donneCertitude(sCertitude);

          if (sCertitudeEcrit != "")
              sLibel = sCertitudeEcrit + string(" ") + sLibel;
      }

      if ((iGenre == genreMP) || (iGenre == genreFP) || (iGenre == genreNP))
          bPluriel = true;

      if      (iArticle == NSPhraseMot::articleDefini)
          sLibel = donneArticleDefini(pMot, bPluriel) + sLibel;
      else if (iArticle == NSPhraseMot::articleIndefini)
          sLibel = donneArticleIndefini(pMot, bPluriel) + sLibel;
      else if (iArticle == NSPhraseMot::articlePartitif)
          sLibel = donneArticlePartitif(pMot, bPluriel) + sLibel;


      // la preposition
      sPrep = Data.chercheGrammaire(PREPOSITION);

    if (sPrep != "")
    {
      if (pMot->getPreposition() != "")
        pMot->setPreposition(pMot->getPreposition() + string(" ")) ;
      pMot->setPreposition(pMot->getPreposition() + string(sPrep, 1, strlen(sPrep.c_str()) - 1)) ;
    }
    else if (pMot->getPreposition() != "")
      sLibel = pMot->getPreposition() + string(" ") + sLibel ;
  }

  return sLibel ;
}

bool
NSGenerateurEn::genereNomComplet(NSPhraseMot* pMot, int iArticle, GERECERTITUDE iGereCertitude)
{
    sPhrase = "";

    reinitialise();

    bool     trouve;
    NSSuper* pSuper = pContexte->getSuperviseur() ;
    NSPhraseur* pComp = pMot->getComplementPhr() ;
    string   sLexique = "";
    string   sPluriel = "";
    string   sInter1 = "";
    string   sInter2 = "";
    GENRE    iGenreNom;
    string   sCertitude = "";
    string   sCertitudeNom = "";

    NSPathologData Data;
    NSPathologData* pPremierAdjData = 0;

    if (! (pComp->adjEpithete).empty() )
        pComp->classeAdjectif(this);

    traitePrepositionCC(&pPh->CCLieu,    &pPh->PrepositionLieu,    NSPhraseur::prepLieu);
    traitePrepositionCC(&pPh->CCTemps,   &pPh->PrepositionTemps,   NSPhraseur::prepTemps);
    traitePrepositionCC(&pPh->CCManiere, &pPh->PrepositionManiere, NSPhraseur::prepManiere);
    traitePrepositionCC(&pPh->CCMoyen,   &pPh->PrepositionMoyen,   NSPhraseur::prepMoyen);
    traitePrepositionCC(&pPh->CCCause,   &pPh->PrepositionCause,   NSPhraseur::prepCause);
    traitePrepositionCC(&pPh->CCBut,     &pPh->PrepositionBut,     NSPhraseur::prepBut);
    traitePrepositionCC(&pPh->CCType,    &pPh->PrepositionType,    NSPhraseur::prepType);
    traitePrepositionCC(&pPh->CCChiffre, &pPh->PrepositionChiffre, NSPhraseur::prepChiffre);


    classeTout();

    //
    // Prise des informations sur le nom à completer
    //
    sLexique = pMot->getLexique() ;
    sPluriel = pMot->getPluriel() ;
    trouve = pContexte->getDico()->trouvePathologData(sLang, &sLexique, &Data) ;
    if (!trouve)
        return false;
    if (!(Data.estNom()))
        return false;
    //
    // Recuperation du genre (composition du genre recupere avec
    // le genre deja existant)
    Data.donneGenre(&iGenreNom);
    if (sPluriel != "")
        Data.donneGenrePluriel(&iGenreNom);

    // On regarde si le nombre est force.

    string sForceNbr = pMot->forceNombre(this);

    if (sForceNbr == FORCE_PLUR)
        Data.donneGenrePluriel(&iGenreNom);
    else if (sForceNbr == FORCE_SING)
        Data.donneGenreSingulier(&iGenreNom);

    string sNom = "";

    donneLibelleAffiche(&sNom, &Data, iGenreNom);

    //
    // Complements : quand, ou, comment, avec quoi, pourquoi, dans quel but
    //

    pCCArray->donnePhrase();

    // Expansions du groupe nominal
    //
    // Cas 1 Adjectif epithete     le gros garcon
    //       le plus pres du nom, dans adjEpithete
    //
    // Cas 2 Complement du nom     le garcon aux yeux bleus
    //       dans les complements circonstanciels
    //
    // Cas 3 Subordonnee relative  le garcon qui a pris le ballon
    //       suppose qu'il y ait un verbe
    //

    // On traite d'abord les adjectifs epithètes
    //

    iterPhraseMot  iterMotsAdj;
    NSPathologData DataAdj;

    string sInterAvPos1 = "";
    string sInterAvPos2 = "";
    string sPhraseAvPos = "";

    string sInterApPos1 = "";
    string sInterApPos2 = "";
    string sPhraseApPos = "";

    string sInterApNeg1 = "";
    string sInterApNeg2 = "";
    string sPhraseApNeg = "";

    string sPhraseAv = "";
    string sPhraseAp = "";

    // On traite les epithetes qui se placent avant le nom.
    // Ils ont tous une certitude "WCEA0", ou bien pas de certitude.
    // En fait, *pAdjEpitheteAvNeg est toujours vide.

    if (pAdjEpitheteAvPos)
    {
        if (!(pAdjEpitheteAvPos->empty()))
        {
            for (iterMotsAdj = pAdjEpitheteAvPos->begin();
                 iterMotsAdj != pAdjEpitheteAvPos->end();
                 iterMotsAdj++)
            {
                // L'epithete s'accorde avec le nom qu'il qualifie.
                if (!((*iterMotsAdj)->estTexteLibre()))
                    sInterAvPos2 = donneAdjectifComplet(*iterMotsAdj, iGenreNom);

                else
                    sInterAvPos2 = (*iterMotsAdj)->getTexteLibre() ;

                etDuMilieu(&sPhraseAvPos, &sInterAvPos1, &sInterAvPos2);
            }
            etFinal(&sPhraseAvPos, &sInterAvPos1);
        }
    }

    // On traite les adjectifs qui se placent apres le nom.
    // D'abord les affirmatifs, puis les negatifs.
    // Si on a des positifs et des negatifs, on met "but not" entre les deux.
    // Si on n'a que des negatifs, on met juste "pas".

    if (pAdjEpitheteApPos)
    {
        if (!(pAdjEpitheteApPos->empty()))
        {
            for (iterMotsAdj = pAdjEpitheteApPos->begin();
                 iterMotsAdj != pAdjEpitheteApPos->end();
                 iterMotsAdj++)
            {
                // L'epithete s'accorde avec le nom qu'il qualifie.
                if (!((*iterMotsAdj)->estTexteLibre()))
                    sInterApPos2 = donneAdjectifComplet(*iterMotsAdj, iGenreNom);

                else
                    sInterApPos2 = (*iterMotsAdj)->getTexteLibre() ;

                etDuMilieu(&sPhraseApPos, &sInterApPos1, &sInterApPos2);
            }
            etFinal(&sPhraseApPos, &sInterApPos1);
        }
    }

    if (pAdjEpitheteApNeg)
    {
        if (!(pAdjEpitheteApNeg->empty()))
        {
            for (iterMotsAdj = pAdjEpitheteApNeg->begin();
                 iterMotsAdj != pAdjEpitheteApNeg->end();
                 iterMotsAdj++)
            {
                // L'epithete s'accorde avec le nom qu'il qualifie.

                sInterApNeg2 = donneAdjectifComplet(*iterMotsAdj);

                etDuMilieu(&sPhraseApNeg, &sInterApNeg1, &sInterApNeg2);
            }
            etFinal(&sPhraseApNeg, &sInterApNeg1);
        }
    }

    sPhraseAv = sPhraseAvPos;
    sPhraseAp = sPhraseApPos;

    if (sPhraseApNeg != "")
    {
        if (sPhraseApPos != "")
            sPhraseAp += string(" but ") + sPhraseApNeg;
        else
            sPhraseAp = sPhraseApNeg;
    }

    // On traite ensuite les complements du nom.
    // Considerons pour simplifier qu'il ne s'agit que de noms.
    // "Il y a suspicion d'endocartite."
    // Le complement du nom est le plus souvent introduit pâr la preposition "de".

    iterPhraseMot  iterMotsCompNom;
    NSPathologData DataCompNom;

    string sInterCompNomPos1 = "";
    string sInterCompNomPos2 = "";
    string sCompNomPos = "";

    string sInterCompNomNeg1 = "";
    string sInterCompNomNeg2 = "";
    string sCompNomNeg = "";

    string sCompNom = "";

    if (pCompNomPos)
    {
        if (!(pCompNomPos->empty()))
        {
            for (iterMotsCompNom = pCompNomPos->begin();
                 iterMotsCompNom != pCompNomPos->end();
                 iterMotsCompNom++)
            {
                sLexique = (*iterMotsCompNom)->getLexique() ;
                if (false == (*iterMotsCompNom)->estTexteLibre())
                {
                    trouve = pContexte->getDico()->trouvePathologData(sLang, &sLexique, &DataCompNom);
                    if (!trouve)
                        return false;

                    sCertitude = string((*iterMotsCompNom)->getCertitude(), 0, 5) ;

                    sInterCompNomPos2 = donneCertitude(sCertitude) + donneNomComplet(*iterMotsCompNom, (*iterMotsCompNom)->getArticle(), sansCertitude);
                }
                else
                    sInterCompNomPos2 = (*iterMotsCompNom)->getTexteLibre() ;

                etDuMilieu(&sCompNomPos, &sInterCompNomPos1, &sInterCompNomPos2);
            }
            etFinal(&sCompNomPos, &sInterCompNomPos1);
        }
    }

    if (pCompNomNeg)
    {
        if (!(pCompNomNeg->empty()))
        {
            for (iterMotsCompNom = pCompNomNeg->begin();
                 iterMotsCompNom != pCompNomNeg->end();
                 iterMotsCompNom++)
            {
                sLexique = (*iterMotsCompNom)->getLexique() ;
                trouve = pContexte->getDico()->trouvePathologData(sLang, &sLexique, &DataCompNom) ;
                if (!trouve)
                    return false;

                sCertitude = string((*iterMotsCompNom)->getCertitude(), 0, 5);
                sInterCompNomNeg2 = donneCertitude(sCertitude) + donneNomComplet(*iterMotsCompNom, (*iterMotsCompNom)->getArticle(), sansCertitude);

                etDuMilieu(&sCompNomNeg, &sInterCompNomNeg1, &sInterCompNomNeg2);
            }
            etFinal(&sCompNomNeg, &sInterCompNomNeg1);
        }
    }

    sCompNom = sCompNomPos;

    if (sCompNomNeg != "")
    {
        if (sCompNomPos != "")
            sCompNom += string(" but ") + sCompNomNeg;
        else
            sCompNom = sCompNomNeg;
    }

    string sCertitudeEcrit = donneCertitude(sCertitudeNom);

    if (sCertitudeEcrit != "")
    {
        sPhrase = sCertitudeEcrit;

        if (sPhraseAv != "")
            sPhrase += string(" ") + sPhraseAv;
        if (sNom != "")
            sPhrase += string(" ") + sNom;
        if (sCompNom != "")
            sPhrase += string(" ") + sCompNom;
        if (sPhraseAp != "")
            sPhrase += string(" ") + sPhraseAp;
    }

    else
    {
        sPhrase = sPhraseAv;

        if (sPhraseAv != "")
            sPhrase += string(" ");

        sPhrase += sNom;

        if (sCompNom != "")
            sPhrase += string(" ") + sCompNom;

        if (sPhraseAp != "")
            sPhrase += string(" ") + sPhraseAp;
    }


    // Traitons les subordonnees circonstancielles.

    if (!(pCCArray->empty()))
    {
        for (GenCompIter itCompl = pCCArray->begin(); itCompl != pCCArray->end(); itCompl++)
        {
            if ((*itCompl)->sPositif != "")
            {
                sPhrase += " " + (*itCompl)->sPositif;
                if ((*itCompl)->sNegatif != "")
                    sPhrase += string(" but not ") + (*itCompl)->sNegatif;
            }
            else if ((*itCompl)->sNegatif != "")
                sPhrase += string(" not ") + (*itCompl)->sNegatif;
        }
    }

    // On traite d'abord la preposition
    string sPrep = Data.chercheGrammaire(PREPOSITION);

    if (sPrep != "")
    {
      if (pMot->getPreposition() != "")
        pMot->setPreposition(pMot->getPreposition() + string(" ")) ;
      pMot->setPreposition(pMot->getPreposition() + string(sPrep, 1, strlen(sPrep.c_str()) - 1)) ;
    }

    // Traitement de l'article

    bool bPluriel = false;

    if ((iGenreNom == genreMP) || (iGenreNom == genreFP) || (iGenreNom == genreNP))
        bPluriel = true;

    if      (iArticle == NSPhraseMot::articleDefini)
        sPhrase = donneArticleDefini(pMot, bPluriel, sansCertitude) + sPhrase;
    else if (iArticle == NSPhraseMot::articleIndefini)
        sPhrase = donneArticleIndefini(pMot, bPluriel, sansCertitude) + sPhrase;
    else if (iArticle == NSPhraseMot::articlePartitif)
        sPhrase = donneArticlePartitif(pMot, bPluriel, sansCertitude) + sPhrase;

    if (pMot->getPreposition() != "")
        sPhrase = pMot->getPreposition() + string(" ") + sPhrase;


    if (iGereCertitude == avecCertitude)
    {
        sCertitude = string(pMot->getCertitude(), 0, 5) ;
        string sCertitudeEcrit = donneCertitude(sCertitude);

        if (sCertitudeEcrit != "")
            sPhrase = sCertitudeEcrit + string(" ") + sPhrase;
    }

    if (pComp->pParentheses)
    {
        NsProposition* pPropos = new NsProposition(pContexte, &(pComp->pParentheses), NsProposition::notSetType, NsProposition::notSetConjonct) ;
        NSGenerateurEn* pGeneParentheses = new NSGenerateurEn(pContexte, pPropos, sLang) ;
        pGeneParentheses->genereProposition(dcPhrase);
        sPhrase += string(" (") + pGeneParentheses->getPropositionPhrase() + string(")");
        delete pGeneParentheses ;
        delete pPropos ;
    }

    // On indique que le nom est complete par une subordonnee relative en donnant
    // à iPhraseType la valeur phraseRelative.
    // Dans Nautilus, on considerera que l'antecedent a toujours valeur de sujet
    // relativement a la subordonnee relative. Celle-ci est donc toujours introduite
    // par le mot de liaison "who" dans le cas d'un antecedent humain, et "which" dans le
    // cas d'un antecedent inanime.
    // On met l'antecedent dans l'array Sujet du complement, pour permettre a generePhrase
    // de mettre le verbe a la bonne personne.

    if (pComp->iPhraseType == NSPhraseur::phraseRelative)
    {
        pComp->Sujet.vider() ;
        pComp->Sujet.push_back(new NSPhraseMot(*pMot)) ;
        NsProposition* pPropos = new NsProposition(pContexte, &pComp, NsProposition::notSetType, NsProposition::notSetConjonct) ;
        NSGenerateurEn* pGeneRelative = new NSGenerateurEn(pContexte, pPropos, sLang) ;
        pGeneRelative->genereProposition(dcPhrase) ;
        sPhrase += pGeneRelative->getPropositionPhrase() ;
        delete pGeneRelative ;
        delete pPropos ;
    }

    return true;
}


string
NSGenerateurEn::donneAdjectifComplet(NSPhraseMot* pMot, GENRE iGenre, GERECERTITUDE iGereCertitude)
{
  string          sCertitude ;
  NSPathologData  DataAdj ;
  NSSuper*        pSuper = pContexte->getSuperviseur() ;

  string sLexique = pMot->getLexique() ;
  bool trouve = pContexte->getDico()->trouvePathologData(sLang, &sLexique, &DataAdj) ;
  if (false == trouve)
    return "" ;

  if (false == DataAdj.estAdjectif())
    return "" ;

  string sLibelle = "" ;
  donneLibelleAffiche(&sLibelle, &DataAdj) ;

  // L'adjectif peut etre precede d'adverbes.

  if (pMot->getComplementPhr())
  {
    NSPhraseur* pComp = pMot->getComplementPhr() ;
    NsProposition* pPropos = new NsProposition(pContexte, &pComp, NsProposition::notSetType, NsProposition::notSetConjonct) ;
    NSGenerateurEn* pGeneCpl = new NSGenerateurEn(pContexte, pPropos, sLang) ;
    if (pGeneCpl->genereAdjectifComplet(pMot, iGenre, iGereCertitude))
      sLibelle = pGeneCpl->getTempoPhrase() ;
    delete pGeneCpl ;
    delete pPropos ;
  }

  if (avecCertitude == iGereCertitude)
  {
    string(pMot->getCertitude(), 0, 5) ;
    sLibelle = donneCertitude(sCertitude) + sLibelle;
  }

  return sLibelle ;
}

bool
NSGenerateurEn::genereAdjectifComplet(NSPhraseMot* pMot, GENRE iGenre, GERECERTITUDE iGereCertitude)
{
  sPhrase = string("") ;

  reinitialise() ;

  NSSuper* pSuper = pContexte->getSuperviseur() ;
  NSPhraseur* pComp = pMot->getComplementPhr() ;
  string   sLexique = "" ;

  NSPathologData Data;

  classeTout();

  //
  // Prise des informations sur l'adverbe à completer
  //
  sLexique = pMot->getLexique() ;
  bool trouve = pContexte->getDico()->trouvePathologData(sLang, &sLexique, &Data);
    if (!trouve)
        return false;
    if (!(Data.estAdjectif()))
        return false;

    string sAdjectif = "";

    donneLibelleAffiche(&sAdjectif, &Data);
    if (sAdjectif == "")
        return false;

    // L'adjectif peut etre complete par des adverbes.

    iterPhraseMot  iterMotsComp;
    NSPathologData DataComp;

    string sInterCompPos1 = "";
    string sInterCompPos2 = "";
    string sCompPos = "";

    string sInterCompNeg1 = "";
    string sInterCompNeg2 = "";
    string sCompNeg = "";

    string sComp = "";

    if (pAdverbePos)
    {
        if (!(pAdverbePos->empty()))
        {
            for (iterMotsComp  = pAdverbePos->begin();
                 iterMotsComp != pAdverbePos->end();
                 iterMotsComp++)
            {
                sLexique = (*iterMotsComp)->getLexique() ;

                if (false == (*iterMotsComp)->estTexteLibre())
                {
                    trouve = pContexte->getDico()->trouvePathologData(sLang, &sLexique, &DataComp);
                    if (!trouve)
                        return false;

                    sInterCompPos2 = donneAdverbeComplet(*iterMotsComp);
                }
                else
                    sInterCompPos2 = (*iterMotsComp)->getTexteLibre() ;


                etDuMilieu(&sCompPos, &sInterCompPos1, &sInterCompPos2);
            }
            etFinal(&sCompPos, &sInterCompPos1);
        }
    }

    if (pAdverbeNeg)
    {
        if (!(pAdverbeNeg->empty()))
        {
            for (iterMotsComp  = pAdverbeNeg->begin();
                 iterMotsComp != pAdverbeNeg->end();
                 iterMotsComp++)
            {
                sLexique = (*iterMotsComp)->getLexique() ;
                trouve = pContexte->getDico()->trouvePathologData(sLang, &sLexique, &DataComp);
                if (!trouve)
                    return false;

                sInterCompNeg2 = donneAdverbeComplet(*iterMotsComp);

                etDuMilieu(&sCompNeg, &sInterCompNeg1, &sInterCompNeg2);
            }
            etFinal(&sCompNeg, &sInterCompNeg1);
        }
    }

    sComp = sCompPos;

    if (sCompNeg != "")
    {
        if (sCompPos != "")
            sComp += string(" but ") + sCompNeg;
        else
            sComp = sCompNeg;
    }

    sPhrase = sComp;

    if (sComp != "")
        sPhrase += string(" ");
    sPhrase += sAdjectif;

    // Tout a la fin, on met le complement entre parentheses.

    if (pComp->pParentheses)
    {
        NsProposition* pPropos = new NsProposition(pContexte, &(pComp->pParentheses), NsProposition::notSetType, NsProposition::notSetConjonct) ;
        NSGenerateurEn* pGeneParentheses = new NSGenerateurEn(pContexte, pPropos, sLang) ;
        pGeneParentheses->genereProposition(dcPhrase) ;
        sPhrase += string(" (") + pGeneParentheses->getPropositionPhrase() + string(")") ;
        delete pGeneParentheses ;
        delete pPropos ;
    }

    if (iGereCertitude == avecCertitude)
    {
        string sCertitude = string(pMot->getCertitude(), 0, 5);
        string sCertitudeEcrit = donneCertitude(sCertitude);

        if (sCertitudeEcrit != "")
            sPhrase = sCertitudeEcrit + string(" ") + sPhrase;
    }
    return true;
}

string
NSGenerateurEn::donneAdverbeComplet(NSPhraseMot* pMot, GERECERTITUDE iGereCertitude)
{
    string sLibel = "";
    NSPathologData Data;

    string   sCertitude;

    string sLexique = pMot->getLexique() ;
    NSSuper* pSuper = pContexte->getSuperviseur();

    bool trouve = pContexte->getDico()->trouvePathologData(sLang, &sLexique, &Data);
    if (!trouve)
        return "";
    if (!(Data.estAdverbe()))
        return "";

    donneLibelleAffiche(&sLibel, &Data);

    if (pMot->getComplementPhr())
    {
        NsProposition* pPropos = new NsProposition(pContexte, pMot->getComplementPhr(), NsProposition::notSetType, NsProposition::notSetConjonct) ;
        NSGenerateurEn* pGeneCpl = new NSGenerateurEn(pContexte, pPropos, sLang) ;
        if (pGeneCpl->genereAdverbeComplet(pMot))
            sLibel = pGeneCpl->getTempoPhrase() ;
        delete pGeneCpl ;
        delete pPropos ;
    }

    if (iGereCertitude == avecCertitude)
    {
        sCertitude = string(pMot->getCertitude(), 0, 5) ;
        string sCertitudeEcrit = donneCertitude(sCertitude) ;

        if (sCertitudeEcrit != "")
            sLibel = sCertitudeEcrit + string(" ") + sLibel ;
    }

    return sLibel;
}


bool
NSGenerateurEn::genereAdverbeComplet(NSPhraseMot* pMot, GERECERTITUDE iGereCertitude)
{
    sPhrase = "";

    reinitialise();

    bool     trouve ;
    NSSuper* pSuper = pContexte->getSuperviseur() ;
    NSPhraseur* pComp = pMot->getComplementPhr() ;
    string   sLexique = "" ;

    NSPathologData Data;

    classeTout();

    //
    // Prise des informations sur l'adverbe à completer
    //
    sLexique = pMot->getLexique() ;
    trouve = pContexte->getDico()->trouvePathologData(sLang, &sLexique, &Data) ;
    if (!trouve)
        return false;
    if (!(Data.estAdverbe()))
        return false;

    string sAdverbe = "";

    donneLibelleAffiche(&sAdverbe, &Data);
    if (sAdverbe == "")
        return false;

    // L'adverbe peut etre complete par des adverbes.

    iterPhraseMot  iterMotsComp;
    NSPathologData DataComp;

    string sInterCompPos1 = "";
    string sInterCompPos2 = "";
    string sCompPos = "";

    string sInterCompNeg1 = "";
    string sInterCompNeg2 = "";
    string sCompNeg = "";

    string sComp = "";

    if (pAdverbePos)
    {
        if (!(pAdverbePos->empty()))
        {
            for (iterMotsComp  = pAdverbePos->begin();
                 iterMotsComp != pAdverbePos->end();
                 iterMotsComp++)
            {
                sLexique = (*iterMotsComp)->getLexique() ;

                if (false == (*iterMotsComp)->estTexteLibre())
                {
                    trouve = pContexte->getDico()->trouvePathologData(sLang, &sLexique, &DataComp);
                    if (!trouve)
                        return false;

                    sInterCompPos2 = donneAdverbeComplet(*iterMotsComp);
                }
                else
                    sInterCompPos2 = (*iterMotsComp)->getTexteLibre() ;
                etDuMilieu(&sCompPos, &sInterCompPos1, &sInterCompPos2) ;
            }
            etFinal(&sCompPos, &sInterCompPos1);
        }
    }

    if (pAdverbeNeg)
    {
        if (!(pAdverbeNeg->empty()))
        {
            for (iterMotsComp  = pAdverbeNeg->begin();
                 iterMotsComp != pAdverbeNeg->end();
                 iterMotsComp++)
            {
                sLexique = (*iterMotsComp)->getLexique() ;
                trouve = pContexte->getDico()->trouvePathologData(sLang, &sLexique, &DataComp);
                if (!trouve)
                    return false;

                sInterCompNeg2 = donneAdverbeComplet(*iterMotsComp);

                etDuMilieu(&sCompNeg, &sInterCompNeg1, &sInterCompNeg2);
            }
            etFinal(&sCompNeg, &sInterCompNeg1);
        }
    }

    sComp = sCompPos;

    if (sCompNeg != "")
    {
        if (sCompPos != "")
            sComp += string(" but ") + sCompNeg;
        else
            sComp = sCompNeg;
    }

    sPhrase = sComp;

    if (sComp != "")
        sPhrase += string(" ");
    sPhrase += sAdverbe;

    // Tout a la fin, on met le complement entre parentheses.

    if (pComp->pParentheses)
    {
        NsProposition* pPropos = new NsProposition(pContexte, &(pComp->pParentheses), NsProposition::notSetType, NsProposition::notSetConjonct) ;
        NSGenerateurEn* pGeneParentheses = new NSGenerateurEn(pContexte, pPropos, sLang) ;
        pGeneParentheses->genereProposition(dcPhrase) ;
        sPhrase += string(" (") + pGeneParentheses->getPropositionPhrase() + string(")") ;
        delete pGeneParentheses ;
        delete pPropos ;
    }
    return true;
}


string
NSGenerateurEn::donneCertitude(string sCertitude, bool* pCommenceParVoyelleOuH)
{
    // pCommenceParVoyelleOuH sert au cas ou la certitude peut imposer une
    // apostrophe. On ne le modifie pas au cas ou la ceritude

    if (sCertitude == "")
        return "";

    if (sCertitude == "WCE00")
    {
        if (pCommenceParVoyelleOuH)
            *pCommenceParVoyelleOuH = false;
        return string("not");
    }
    else if ((sCertitude > "WCE00") && (sCertitude < "WCE50"))
    {
        if (pCommenceParVoyelleOuH)
            *pCommenceParVoyelleOuH = false;
        return string("probably not");
    }
    else if (string(sCertitude, 0 ,5) == "WCE50")
    {
        if (pCommenceParVoyelleOuH)
            *pCommenceParVoyelleOuH = false;
        return string("perhaps");
    }
    else if ((string(sCertitude, 0 ,5) > "WCE50") && (string(sCertitude, 0 ,5) < "WCE90"))
    {
        if (pCommenceParVoyelleOuH)
            *pCommenceParVoyelleOuH = false;
        return string("probably");
    }
    else if ((string(sCertitude, 0 ,5) >= "WCE90") && (string(sCertitude, 0 ,5) < "WCEA0"))
    {
        if (pCommenceParVoyelleOuH)
            *pCommenceParVoyelleOuH = false;
        return string("certainly");
    }
    else
        return "";
}

string
NSGenerateurEn::donnePhraseComplement(NSPhraseMot* pLiaison,
                                      NSPhraseMot* pPreposition,
                                      NSPhraseMotArray* pCC,
                                      bool* bSucces)
{
  *bSucces = false ;

  string sInter1 = "" ;
  string sInter2 = "" ;
  string sPhr = "" ;

  bool     trouve ;
  NSSuper* pSuper = pContexte->getSuperviseur() ;
  string   sLexique ;
  string   sPluriel ;
  GENRE    iGenre ;

  iterPhraseMot  iterMots ;
  NSPathologData Data ;

  if (false == pCC->empty())
  {
    for (iterMots = pCC->begin() ; pCC->end() != iterMots ; iterMots++)
    {
      // Est-ce une donnee chiffree ?
      string sFormat = (*iterMots)->getFormat() ;

            // Est-ce une structure de type NSPhraseMotTime ?
            // Is it a NSPhraseMotTime object
            NSPhraseMotTime* pDate = dynamic_cast<NSPhraseMotTime*>(*iterMots) ;
            if ( pDate )
            {
                string sComplement;
                string sUnite;
                if (pDate->valeurMin[0] != '\0')
                {
                    sComplement = string(pDate->valeurMin);
                    sUnite      = string(pDate->uniteMin);
                    sFormat     = string(pDate->formatMin);

                    gereDateFr DateMin(pContexte->getSuperviseur(), sLang);
                    DateMin.setDate(&sComplement);
                    DateMin.setFormat(&sFormat);

                    string sUniteSens = "";
                    pContexte->getDico()->donneCodeSens(&sUnite, &sUniteSens);
                    DateMin.setUnite(&sUniteSens);

                    string sLibelMin, sIntroMin;

                    if (pDate->valeurMax[0] != '\0')
                    {
                        DateMin.donne_date_claire(&sLibelMin, &sIntroMin, gereDateFr::DateDeb);

                        sComplement = string(pDate->valeurMax);
                        sUnite      = string(pDate->uniteMax);
                        sFormat     = string(pDate->formatMax);

                        gereDateFr DateMax(pContexte->getSuperviseur(), sLang);
                        DateMax.setDate(&sComplement);
                        DateMax.setFormat(&sFormat);

                        string sUniteSens = "";
                        pContexte->getDico()->donneCodeSens(&sUnite, &sUniteSens);
                        DateMax.setUnite(&sUniteSens);

                        string sLibelMax, sIntroMax;

                        DateMax.donne_date_claire(&sLibelMax, &sIntroMax, gereDateEn::DateFin);

                        sInter2 = sIntroMin + sLibelMin + string(" ") + sIntroMax + sLibelMax;
                    }
                    else if (pDate->bMaxNow)
                    {
                        DateMin.donne_date_claire(&sLibelMin, &sIntroMin, gereDateEn::DateDeb);
                        sInter2 = " since" + sIntroMin + sLibelMin;
                    }
                    else
                    {
                        DateMin.donne_date_claire(&sLibelMin, &sIntroMin);
                        sInter2 = string("starting ") + sIntroMin + sLibelMin;
                    }
                }
                else if (pDate->valeurMax[0] != '\0')
                {
                    sComplement = string(pDate->valeurMax);
                    sUnite      = string(pDate->uniteMax);
                    sFormat     = string(pDate->formatMax);

                    gereDateFr DateMax(pContexte->getSuperviseur(), sLang);
                    DateMax.setDate(&sComplement);
                    DateMax.setFormat(&sFormat);

                    string sUniteSens = "";
                    pContexte->getDico()->donneCodeSens(&sUnite, &sUniteSens);
                    DateMax.setUnite(&sUniteSens);

                    string sLibelMax, sIntroMax;

                    if (pDate->bMinNow)
                    {
                        DateMax.donne_date_claire(&sLibelMax, &sIntroMax, gereDateEn::DateFin);
                        sInter2 = " until" + sIntroMax + sLibelMax;
                    }
                    else
                    {
                        DateMax.donne_date_claire(&sLibelMax, &sIntroMax);
                        sInter2 = sIntroMax + sLibelMax;
                    }
                }
            }
            else if (sFormat == "")
            {
                sLexique = (*iterMots)->getLexique() ;
                sPluriel = (*iterMots)->getPluriel() ;
                if (false == (*iterMots)->estTexteLibre())
                {
                    trouve = pContexte->getDico()->trouvePathologData(sLang, &sLexique, &Data);
                    if (!trouve)
                        return "";
                    if (!(Data.estNom()))
                        return "";

                    // L'article du statut d'objet reel ou abstrait


                    if ((*iterMots)->getArticle() != NSPhraseMot::articleSans)
                    {
                        if (sPluriel == "")
                            sInter2 = donneNomComplet(*iterMots, (*iterMots)->getArticle(), sansCertitude);
                        else if (string(sPluriel, 0, 5) == "WPLUR")
                            sInter2 = donneNomComplet(*iterMots, NSPhraseMot::articleDefini, sansCertitude);
                        else if (string(sPluriel, 0, 5) == "WPLUS")
                            sInter2 = donneNomComplet(*iterMots, NSPhraseMot::articleIndefini, sansCertitude);
                    }

                    else
                        sInter2 = donneNomComplet(*iterMots, NSPhraseMot::articleSans, sansCertitude);

                    string sCertitude = string((*iterMots)->getCertitude(), 0, 5) ;
                    string sCertitudeEcrit = donneCertitude(sCertitude) ;

                    if (sCertitudeEcrit != "")
                        sInter2 = sCertitudeEcrit + string(" ") + sInter2 ;
                }
                else
                    sInter2 = (*iterMots)->getTexteLibre() ;
            }
            else
            {
                //
                // Date
                //
                if      ((sFormat[0] == '£') &&
                         ((sFormat[1] == dateMARK) || (sFormat[1] == dateHeureMARK)))
                {
                    string sComplement = (*iterMots)->getComplement() ;
                    string sUnite      = (*iterMots)->getUnite() ;

                    gereDateEn Date(pContexte->getSuperviseur(), sLang);
                    Date.setDate(&sComplement);
                    Date.setFormat(&sFormat);

                    string sUniteSens = "";
                    pContexte->getDico()->donneCodeSens(&sUnite, &sUniteSens);
                    Date.setUnite(&sUniteSens);

                    string sLibel, sIntro;

                    Date.donne_date_claire(&sLibel, &sIntro);
                    sInter2 = sIntro + sLibel;
                }
            }
            etDuMilieu(&sPhr, &sInter1, &sInter2);
        }
        etFinal(&sPhr, &sInter1);

        // Traitement de la preposition globale
            sLexique = pPreposition->getLexique() ;
            if (sLexique != "")
            {
                sPluriel = pPreposition->getPluriel() ;
                trouve = pContexte->getDico()->trouvePathologData(sLang, &sLexique, &Data);
                if (!trouve)
                    return "";
                Data.donneGenre(&iGenre);
                if (sPluriel != "")
                    Data.donneGenrePluriel(&iGenre);
                donneLibelleAffiche(&sInter2, &Data, iGenre);
                sPhr = sInter2 + " " + sPhr;
            }
        // Traitement de l'element de liaison
        sLexique = pLiaison->getLexique() ;
        if (sLexique != "")
        {
            sPluriel = pLiaison->getPluriel() ;
            trouve = pContexte->getDico()->trouvePathologData(sLang, &sLexique, &Data);
            if (!trouve)
                return "";
            Data.donneGenre(&iGenre);
            if (sPluriel != "")
                Data.donneGenrePluriel(&iGenre);
            donneLibelleAffiche(&sInter2, &Data, iGenre);
            sPhr = sInter2 + " " + sPhr;
        }
    }
    *bSucces = true;
    return sPhr;
}

string
NSGenerateurEn::donnePhraseChiffree(NSPhraseMot* pLiaison,
                                    NSPhraseMot* pPreposition,
                                    NSPhraseMotArray* pCC,
                                    bool* bSucces)
{
  string sInter1 = "";
  string sInter2 = "";

  string sPhr = "";

  bool     trouve ;
  NSSuper* pSuper = pContexte->getSuperviseur() ;
  string   sLexique ;
  string   sPluriel ;
  GENRE    iGenre ;

  iterPhraseMot  iterMots;
  NSPathologData Data;

  if (false == pCC->empty())
  {
    for (iterMots = pCC->begin(); iterMots != pCC->end(); iterMots++)
    {
            // Traitement du complement, en fonction de son format
            //
            string sComplement = (*iterMots)->getComplement() ;

            string sUnite  = (*iterMots)->getUnite() ;
            string sFormat = (*iterMots)->getFormat() ;

            string sValeur = "";
            //
            // Valeur numerique   ((sFormat[1] == dateMARK) || (sFormat[1] == dateHeureMARK)))
            //
            if (sFormat[0] == '£')
            {
                if (sFormat[1] == nbMARK)
                {
                    gereNum Num(pContexte->getSuperviseur(), sLang);
                    Num.instancier(&sComplement, &sUnite, &sFormat);
                    bool bSucces;
                    sValeur = decodeNum(&Num, &bSucces);

                    // On traite le cas ou la valeur chiffree est du type :
                    // "dans 3 jours", "il y a 3 jours", "pendant 3 jours",
                    // "il y a 2 ans pendant 3 jours"...

                    if (sLexique == "KDATE1")
                    {
                        // Cas ou il s'agit d'un evenement passe ponctuel :
                        // "3 days ago".
                        sValeur += string(" ago");
                    }
                    else if (sLexique == "KDATD1")
                    {
                        // Cas ou il s'agit d'un evenement futur ponctuel :
                        // "in 3 days".
                        sValeur = string("in ") + sValeur;
                    }
                }
                //
                // Date
                //
                else if ((sFormat[1] == dateMARK) || (sFormat[1] == dateHeureMARK))
                {
                    string sMessage = "" ;
                    string sIntro = "" ;

                    gereDateEn Date(pContexte->getSuperviseur(), sLang);
                    Date.setUnite(&sUnite);
                    Date.setFormat(&sFormat);
                    Date.setDate(&sComplement);
                    Date.donne_date_claire(&sMessage, &sIntro);
                    sValeur = sIntro + sMessage;
                }
                //
                // Heure
                //
                else if (sFormat[1] == heureMARK)
                {
                    string sMessage = "";

                    gereHeure Heure(pContexte->getSuperviseur(), sLang);
                    Heure.setUnite(&sUnite);
                    Heure.setFormat(&sFormat);
                    Heure.setHeure(&sComplement);
                    Heure.donne_heure(&sMessage);
                    sValeur = sMessage;
                }
            }

            sLexique = (*iterMots)->getLexique() ;
            if (sLexique != "")
            {
                trouve = pContexte->getDico()->trouvePathologData(sLang, &sLexique, &Data);
                if (!trouve)
                {
                    *bSucces = false;
                    return "";
                }
                // Recuperation du genre (composition du genre recupere avec
                // le genre deja existant)
                Data.donneGenre(&iGenre);
                if (sPluriel != "")
                    Data.donneGenrePluriel(&iGenre);
                //
                donneLibelleAffiche(&sInter2, &Data, iGenre);
            }

            string sMethode = (*iterMots)->getMethode() ;
            string sLibelMethode = "";

            if (sMethode != "")
            {
                trouve = pContexte->getDico()->trouvePathologData(sLang, &sMethode, &Data);
                if (!trouve)
                {
                    *bSucces = false;
                    return "";
                }
                // Recuperation du genre (composition du genre recupere avec
                // le genre deja existant)
                Data.donneGenre(&iGenre);
                if (sPluriel != "")
                    Data.donneGenrePluriel(&iGenre);
                //
                donneLibelleAffiche(&sLibelMethode, &Data, iGenre);
            }

            if (sInter2 != "")
                sInter2 += " ";
            sInter2 += sValeur;
            if (sLibelMethode != "")
                sInter2 += " (" + sLibelMethode + ")";

            etDuMilieu(&sPhr, &sInter1, &sInter2);
        }
        etFinal(&sPhr, &sInter1);

        // Traitement de la preposition
        sLexique = pPreposition->getLexique() ;
        if (sLexique != "")
        {
            sPluriel = pPreposition->getPluriel() ;
            trouve = pContexte->getDico()->trouvePathologData(sLang, &sLexique, &Data);
            if (!trouve)
                return "";
            Data.donneGenre(&iGenre);
            if (sPluriel != "")
                Data.donneGenrePluriel(&iGenre);
            donneLibelleAffiche(&sInter2, &Data, iGenre);
            sPhr = sInter2 + " " + sPhr;
        }
        // Traitement de l'element de liaison
        sLexique = pLiaison->getLexique() ;
        if (sLexique != "")
        {
            sPluriel = pLiaison->getPluriel() ;
            trouve = pContexte->getDico()->trouvePathologData(sLang, &sLexique, &Data);
            if (!trouve)
                return "";
            if (Data.estVerbe())
            {
                sInter2 = donneParticipePresent(&Data);
                sPhr = sInter2 + " " + sPhr;
            }
            else
            {
                Data.donneGenre(&iGenre);
                if (sPluriel != "")
                    Data.donneGenrePluriel(&iGenre);
                donneLibelleAffiche(&sInter2, &Data, iGenre);
                sPhr = sInter2 + " " + sPhr;
            }
        }
    }
    return sPhr;
}


bool
NSGenerateurEn::donnePremierAdj(NSPhraseur* pCompAdj, NSPhraseMot* pMotAdj)
{
    NSSuper* pSuper = pContexte->getSuperviseur();
    NSPathologData PremierAdjData;
    NSPhraseMot PremierAdj(pContexte);
    bool trouve;
    string sLexiqueAdj = "";

    if (pCompAdj)
    {
        NsProposition* pPropos = new NsProposition(pContexte, &pCompAdj, NsProposition::notSetType, NsProposition::notSetConjonct) ;
        NSGenerateurEn* pGeneCompl = new NSGenerateurEn(pContexte, pPropos, sLang) ;
        pCompAdj->classeAdjectif(this) ;
        pGeneCompl->classeTout() ;

        if (pGeneCompl->pAdjEpitheteAvPos)
        {
            if (!((pGeneCompl->pAdjEpitheteAvPos)->empty()))
            {
                PremierAdj = *(*((pGeneCompl->pAdjEpitheteAvPos)->begin())) ;
                sLexiqueAdj = PremierAdj.getLexique() ;

                // Si jamais le premier adjectif est un texte libre, on l'ignore,
                // vu qu'on est incapable de le traiter grammaticalement.
                if (PremierAdj.estTexteLibre())
                    return false ;

                trouve = pContexte->getDico()->trouvePathologData(sLang, &sLexiqueAdj, &PremierAdjData) ;
                if (!trouve)
                    return false ;
                if (pMotAdj)
                    *pMotAdj = PremierAdj ;
                return true ;
            }
        }
        else if (pGeneCompl->pAdjEpitheteAvNeg)
            if (!((pGeneCompl->pAdjEpitheteAvNeg)->empty()))
            {
                PremierAdj = *(*((pGeneCompl->pAdjEpitheteAvNeg)->begin())) ;
                sLexiqueAdj = PremierAdj.getLexique() ;

                if (PremierAdj.estTexteLibre())
                    return false ;

                trouve = pContexte->getDico()->trouvePathologData(sLang, &sLexiqueAdj, &PremierAdjData) ;
                if (!trouve)
                    return false ;
                if (pMotAdj)
                    *pMotAdj = PremierAdj ;
                return true;
            }
        delete pGeneCompl ;
        delete pPropos ;
    }
    return false;
}

bool
NSGenerateurEn::donnePremierAdv(NSPhraseur* pComp, NSPhraseMot* pMotAdv)
{
    NSSuper* pSuper = pContexte->getSuperviseur();
    NSPathologData PremierAdvData;
    NSPhraseMot PremierAdv(pContexte);
    NSPhraseMot PremierAdvRec(pContexte);
    bool trouve;
    string sLexiqueAdv = "";

    if (pComp)
    {
        NsProposition* pPropos = new NsProposition(pContexte, &pComp, NsProposition::notSetType, NsProposition::notSetConjonct) ;
        NSGenerateurEn* pGeneCompl = new NSGenerateurEn(pContexte, pPropos, sLang) ;
        pComp->classeAdjectif(this) ;
        pGeneCompl->classeTout() ;

        if (pGeneCompl->pAdverbePos)
        {
            if (!((pGeneCompl->pAdverbePos)->empty()))
            {
                PremierAdv = *(*((pGeneCompl->pAdverbePos)->begin()));
                sLexiqueAdv = PremierAdv.getLexique() ;
                trouve = pContexte->getDico()->trouvePathologData(sLang, &sLexiqueAdv, &PremierAdvData);
                if (!trouve)
                    return false;
                if (PremierAdv.getComplementPhr())
                {
                    if (donnePremierAdv(PremierAdv.getComplementPhr(), &PremierAdvRec))
                    {
                    *pMotAdv = PremierAdvRec;
                    return true;
                    }
                }
                if (pMotAdv)
                    *pMotAdv = PremierAdv;
                return true;
            }
        }
        else if (pGeneCompl->pAdverbeNeg)
            if (!((pGeneCompl->pAdverbeNeg)->empty()))
            {
                PremierAdv = *(*((pGeneCompl->pAdverbeNeg)->begin()));
                sLexiqueAdv = PremierAdv.getLexique() ;
                trouve = pContexte->getDico()->trouvePathologData(sLang, &sLexiqueAdv, &PremierAdvData);
                if (!trouve)
                    return false;
                if (PremierAdv.getComplementPhr())
                {
                    if (donnePremierAdv(PremierAdv.getComplementPhr(), &PremierAdvRec))
                    {
                        *pMotAdv = PremierAdvRec;
                        return true;
                    }
                }
                if (pMotAdv)
                    *pMotAdv = PremierAdv;
                return true;
            }
        delete pGeneCompl ;
        delete pPropos ;
    }
    return false;
}

string
NSGenerateurEn::decodeNum(gereNum* pNum, bool* bSucces)
{
    string sLibelle = "";
    *bSucces = true;

    if ((pNum->estExact()) || (pNum->estInf()) || (pNum->estSup()))
    {
        bool bPluriel = false;

        if (pNum->estExact())
        {
            sLibelle = pNum->getNum("");
            if (pNum->getValeur() > 1)
                bPluriel = true;
        }
        else
        {
            if ((pNum->estInf()) && (pNum->estSup()))
            {
                sLibelle = "between " + pNum->getNumInf() + " and " + pNum->getNumSup();
            }
            else if (pNum->estInf())
            {
                sLibelle = "more than " + pNum->getNumInf();
            }

            else if (pNum->estSup())
            {
                sLibelle = "less than " + pNum->getNumSup();
            }

            if ((pNum->getValeurInf() > 1) || (pNum->getValeurSup() > 1))
                bPluriel = true;
        }
        //
        // Libelle de l'unite de l'unite
        //
        if (sLibelle != "")
        {
            string sUniteLibel = pNum->donneLibelleUnite(pContexte) ;

            if (sUniteLibel != "")
                sLibelle += string(" ") + sUniteLibel;
        }
    }
    return sLibelle;
}

string
NSGenerateurEn::donneArticleIndefini(NSPhraseMot* pMot, bool bPluriel, GERECERTITUDE iGereCertitude)
{
    bool bModifieArticle = false;

    NSPhraseMot    MotAdj(pContexte);
    NSPhraseMot    MotAdv(pContexte);
    NSPathologData DataAdj;
    NSPathologData DataAdv;
    NSPathologData Data;

    string sLexiqueAdj = "";
    string sLexiqueAdv = "";

    string sCertitudeNom = "";
    string sCertitudeAdj = "";
    string sCertitudeAdv = "";

    string sLexique = pMot->getLexique() ;
    NSSuper* pSuper = pContexte->getSuperviseur();

    bool trouve = pContexte->getDico()->trouvePathologData(sLang, &sLexique, &Data);
    if (!trouve)
        return "";

    if (!(Data.estNom()))
        return "";

    // Il n'y a pas d'article indefini au pluriel en anglais

    if ((Data.estNom()) && bPluriel)
        return string(" ");

    GENRE  iGenreNom;
    string sLibelle = "";

    Data.donneGenre(&iGenreNom);
    donneLibelleAffiche(&sLibelle, &Data, iGenreNom);

    if (CommenceParVoyelle(&sLibelle))
        bModifieArticle = true;

    // On traite le cas des noms commencant par h.
    // Si le h est muet, l'article est an.
    // Sinon, l'article est a.
    // Cette information est stockee dans le libelle, en commentaire grammatical
    // {| / / }.
    // h : le h est muet.
    // H : le h est prononce.


    if ((sLibelle[0] == 'h') || (sLibelle[0] == 'H'))
    {
        if (Data.chercheGrammaire(VALEUR_DE_H) == H_MUET)
            bModifieArticle = true;
    }

    if (pMot->getComplementPhr())
    {
        bool bPremierAdjExiste = donnePremierAdj(pMot->getComplementPhr(), &MotAdj);
        sLexiqueAdj = MotAdj.getLexique() ;

        if (bPremierAdjExiste)
        {
            sCertitudeAdj = string(MotAdj.getCertitude(), 0, 5) ;

            if (sCertitudeAdj != "")
            {
                // Le premier adjectif est precede d'une certitude, donc on ne
                // s'occupe pas de la premiere lettre de l'adjectif.

                donneCertitude(sCertitudeAdj, &bModifieArticle);
            }

            else if (MotAdj.getComplementPhr())
            {
                bool bPremierAdvExiste = donnePremierAdv(MotAdj.getComplementPhr(), &MotAdv);
                sLexiqueAdv = MotAdv.getLexique() ;

                if (bPremierAdvExiste)
                {
                    sCertitudeAdv = string(MotAdv.getCertitude() , 0, 5);

                    if (sCertitudeAdv != "")
                    {
                        // Le premier adverbe est precede d'une certitude, donc on ne
                        // s'occupe pas de la premiere lettre de l'adverbe.

                        donneCertitude(sCertitudeAdv, &bModifieArticle);
                    }
                    else
                    {
                        // On regarde si l'adverbe commence par une voyelle ou par un
                        // h muet.
                        string sLibelleAdv = "";
                        donneLibelleAffiche(&sLibelleAdv, &DataAdv);

                        if (CommenceParVoyelle(&sLibelleAdv))
                            bModifieArticle = true;

                        else if ((sLibelleAdv[0] == 'h') || (sLibelleAdv[0] == 'H'))
                        {
                            if (DataAdv.chercheGrammaire(VALEUR_DE_H) == H_MUET)
                            bModifieArticle = true;
                        }
                    }
                }
                else
                {
                    // On regarde si l'adjectif commence par une voyelle ou par un
                    // h muet.
                    string sLibelleAdj = "";
                    donneLibelleAffiche(&sLibelleAdj, &DataAdj);

                    if (CommenceParVoyelle(&sLibelleAdj))
                        bModifieArticle = true;

                    else if ((sLibelleAdj[0] == 'h') || (sLibelleAdj[0] == 'H'))
                    {
                        if (DataAdj.chercheGrammaire(VALEUR_DE_H) == H_MUET)
                            bModifieArticle = true;
                    }
                }
            }
        }
    }

    // Si on gere la certitude du nom, celle-ci prend le dessus sur le reste.

    if (iGereCertitude == avecCertitude)
        donneCertitude(sCertitudeNom, &bModifieArticle);

    if (bModifieArticle)
        return string("an ");
    else
        return string("a ");
}

string
NSGenerateurEn::donneArticleDefini(NSPhraseMot* pMot, bool /* bPluriel */, GERECERTITUDE /* iGereCertitude */)
{
  NSPathologData Data ;

  string sLexique = pMot->getLexique() ;
  NSSuper* pSuper = pContexte->getSuperviseur() ;

  bool trouve = pContexte->getDico()->trouvePathologData(sLang, &sLexique, &Data) ;
  if (!trouve)
    return "" ;

  if (!(Data.estNom()))
    return "" ;

  return string("the ") ;
}

string
NSGenerateurEn::donneArticlePartitif(NSPhraseMot* /* pMot */, bool /* bPluriel */, GERECERTITUDE /* iGereCertitude */)
{
  return string("") ;
}

string
NSGenerateurEn::donnePronomPersonnel(GENRE iGenre, NSPhraseur::VBPERSO /* iVbPersonne */, string /* sFonction */)
{
  switch (iGenre)
  {
    case genreMS : return string("he") ;
    case genreFS : return string("she") ;
    case genreNS : return string("it") ;
    case genreMP : return string("they") ;
    case genreFP : return string("they") ;
    case genreNP : return string("they") ;
    default      : return "" ;
  }
}

void
NSGenerateurEn::etFinal(string *type, string *type1)
{
  if ((NULL == type) || (NULL == type1))
    return ;

	if (string("") == *type1)
		return ;

	if (string("") != *type)
    *type += string(" and ") ;

	*type += *type1 ;

	return ;
}

string
NSGenerateurEn::donneParticipePresent(NSPathologData* pPathoData)
{
  string sPartPresent = "" ;
  donneLibelleAffiche(&sPartPresent, pPathoData, declinaisonPartPresent) ;
  return sPartPresent ;
}

string
NSGenerateurEn::donneParticipePasse(NSPhraseMot* pPhraseMot, GENRE /* iGenre */)
{
    bool     trouve;
    NSSuper* pSuper = pContexte->getSuperviseur();
    string   sLexique;
    NSPathologData Data;
    string   sPartPasse = "";

    sLexique = pPhraseMot->getLexique() ;
    trouve = pContexte->getDico()->trouvePathologData(sLang, &sLexique, &Data);
    if (!trouve)
        return "";
    if (!(Data.estVerbe()))
        return "";

    donneLibelleAffiche(&sPartPasse, &Data, declinaisonPartPasse);
    return sPartPasse;
}

string
NSGenerateurEn::donnePreterite(NSPhraseMot* pPhraseMot)
{
  if (NULL == pPhraseMot)
    return string("") ;

  NSSuper* pSuper = pContexte->getSuperviseur() ;

  string sLexique = pPhraseMot->getLexique() ;

  NSPathologData Data ;
  bool trouve = pContexte->getDico()->trouvePathologData(sLang, &sLexique, &Data);
  if (false == trouve)
    return string("") ;
  if (false == Data.estVerbe())
    return string("") ;

  string sPreterite = string("") ;
  donneLibelleAffiche(&sPreterite, &Data, declinaisonPreterite) ;

  return sPreterite ;
}

void
NSGenerateurEn::donneVerbe(string* principal, string* auxilliaire)
{
  if ((NULL == principal) || (NULL == auxilliaire))
    return ;

  *principal   = string("") ;
  *auxilliaire = string("") ;

  if (pPh->Verbe.empty())
    return ;

    NSSuper* pSuper = pContexte->getSuperviseur();
    string   sLexique;
    NSPathologData Data;

  NSPhraseMot* pVerbe = *(pPh->Verbe.begin()) ;
  string sVb = (*(pPh->Verbe.begin()))->getLexique() ;

  string sVerbe ;
  pContexte->getDico()->donneCodeSens(&sVb, &sVerbe) ;

  bool trouve = pContexte->getDico()->trouvePathologData(sLang, &sVb, &Data) ;

  switch (pPh->iVbAspect)
  {
    case NSPhraseur::aspectPonctuel :
    {
      switch (pPh->iVbTemps)
      {
        case NSPhraseur::tempsPasseRevolu :
        case NSPhraseur::tempsPasseActuel : // preterite
        {
          if (sVb == "4ETRE1")
          {
            switch (pPh->iVbPersonne)
            {
              case NSPhraseur::pers3S : *auxilliaire = "was" ;  return ;
              case NSPhraseur::pers1S : *auxilliaire = "was" ;  return ;
              default                 : *auxilliaire = "were" ; return ;
            }
          }
          else if (sVb == "4AVOI1")
          {
            switch (pPh->iVbPersonne)
            {
              case NSPhraseur::pers3S : *auxilliaire = "had" ; return ;
              case NSPhraseur::pers1S : *auxilliaire = "had" ; return ;
              default                 : *auxilliaire = "had" ; return ;
            }
          }
          else
            *principal = donnePreterite(pVerbe) ;
        }
        case NSPhraseur::tempsPresent : // present simple
        {
          if (sVb == "4ETRE1")
          {
            switch (pPh->iVbPersonne)
            {
              case NSPhraseur::pers1S : *auxilliaire = "am" ;  return ;
              case NSPhraseur::pers3S : *auxilliaire = "is" ;  return ;
              default                 : *auxilliaire = "are" ; return ;
            }
          }
          else if (sVb == "4AVOI1")
          {
            switch (pPh->iVbPersonne)
            {
              case NSPhraseur::pers1S : *auxilliaire = "have" ; return ;
              case NSPhraseur::pers3S : *auxilliaire = "has" ;  return ;
              default                 : *auxilliaire = "have" ; return ;
            }
          }
          else
          {
            switch (pPh->iVbPersonne)
            {
              case NSPhraseur::pers3S :
              {
                donneLibelleAffiche(principal, &Data, declinaison3Personne) ;
                break ;
              }
              default :
                donneLibelleAffiche(principal, &Data, declinaisonBase) ;
            }
          }
        }
        case NSPhraseur::tempsFuturLointain :
        case NSPhraseur::tempsFuturProche   : // futur
        {
          donneLibelleAffiche(principal, &Data, declinaisonBase) ;
          switch (pPh->iVbPersonne)
          {
            case NSPhraseur::pers1S :
            case NSPhraseur::pers3S :
            {
              *auxilliaire = "shall" ;
              return ;
            }
            default :
            {
              *auxilliaire = "will" ;
              return ;
            }
          }
        }
        default : return ;
      }
    }
    case NSPhraseur::aspectIntervalle :
    {
      switch (pPh->iVbTemps)
      {
        case NSPhraseur::tempsPasseRevolu : // present perfect
        {
          *principal = donneParticipePasse(pVerbe);
          switch (pPh->iVbPersonne)
          {
            case NSPhraseur::pers3S : *auxilliaire = "has" ;  return ;
            default                 : *auxilliaire = "have" ; return ;
          }
        }
        case NSPhraseur::tempsPasseActuel : // present perfect progressif
        {
          *principal = string("been ") + donneParticipePresent(&Data) ;
          switch (pPh->iVbPersonne)
          {
            case NSPhraseur::pers3S : *auxilliaire = "has" ;  return ;
            default                 : *auxilliaire = "have" ; return ;
          }
        }
        case NSPhraseur::tempsPresent : // present progressif
        {
          *principal = donneParticipePresent(&Data) ;
          switch (pPh->iVbPersonne)
          {
            case NSPhraseur::pers1S : *auxilliaire = "am" ;  return ;
            case NSPhraseur::pers3S : *auxilliaire = "is" ;  return ;
            default                 : *auxilliaire = "are" ; return ;
          }
        }
        case NSPhraseur::tempsFuturProche :
        case NSPhraseur::tempsFuturLointain : // futur progressif
        {
          *principal   = string("be ") + donneParticipePresent(&Data) ;
          *auxilliaire = "will" ;
        }
        default : return;
      }
    }
    default : return;
  }
}

bool
NSGenerateurEn::donneVerbeClasse(NSPhraseMot* pPhraseMot, string* psTransitivite, string* psActEtat)
{
    // En anglais, le verbe est du type : etudier {|T/A}
    // T   pour Transitif, I intransitif
    // A   pour verbe d'Action,
    // E verbe d'etat (ER reel "to be", EA apparent "to look", ED dure "to stay"
    //                 EC change "to become")


    NSPathologData Data;

    *psTransitivite = "";
    *psActEtat      = "";

    string sTransitif = "";
    string sIntransitif = "";
    string sEtat = "";
    string sAction = "";

	string sLexique = (*(pPh->Verbe.begin()))->getLexique() ;
    NSSuper* pSuper = pContexte->getSuperviseur();

    bool trouve = pContexte->getDico()->trouvePathologData(sLang, &sLexique, &Data);
    if (!trouve)
        return false;

    if (!(Data.estVerbe()))
        return false;

    sTransitif     = Data.chercheGrammaire("T");
    sIntransitif   = Data.chercheGrammaire("I");
    sEtat          = Data.chercheGrammaire("E");
    sAction        = Data.chercheGrammaire("A");


    // Un verbe ne peut pas etre a la fois d'action et d'etat, ni a la fois
    // transitif et intransitif(ou au moins dans le lexique).
    // Dans les deux cas, un verbe est soit l'un soit l'autre.

    if ((sTransitif != "") && (sIntransitif != ""))
        return false;

    if ((sAction != "") && (sEtat != ""))
        return false;

    if ((sTransitif == "") && (sIntransitif == ""))
        return false;

    if ((sAction == "") && (sEtat == ""))
        return false;

    if (sTransitif != "")
        *psTransitivite = sTransitif;

    if (sIntransitif != "")
        *psTransitivite = sIntransitif;

    if (sAction != "")
        *psActEtat = sAction;

    if (sEtat != "")
        *psActEtat = sEtat;

    if (*psActEtat != "")
    {
        if      (((*psActEtat)[0] == 'A') || ((*psActEtat)[0] == 'a'))
            pPh->iVbType = NSPhraseur::vbTypeAction;
        else if (((*psActEtat)[0] == 'E') || ((*psActEtat)[0] == 'e'))
            pPh->iVbType = NSPhraseur::vbTypeEtat;
    }
    return true;
}


bool
NSGenerateurEn::traitePostposition(NSPhraseMot* pMot, NSPhraseMotArray* pSpecifique)
{
    NSSuper* pSuper = pContexte->getSuperviseur();
    iterPhraseMot  iterMots;
    NSPathologData Data;
    bool trouve;
    string sLexique = pMot->getLexique() ;
    string sPostposition = "";

    trouve = pContexte->getDico()->trouvePathologData(sLang, &sLexique, &Data);
    if (!trouve)
        return false;

    string sPostpositionBrute = Data.chercheGrammaire(POSTPOSITION);
    if (sPostpositionBrute == "")
        return false;

    // La postposition est la string sPostpositionBrute privee du "O" initial.

    sPostposition = string(sPostpositionBrute, 1, strlen(sPostposition.c_str()) - 1);

    // Si pMot est un nom, il peut imposer une preposition a son complement du nom.

    if (pMot->getComplementPhr())
    {
        NSPhraseur* pComp = pMot->getComplementPhr() ;

        if (!(pComp->compNom.empty()))
        {
            for (iterMots = pComp->compNom.begin(); iterMots != pComp->compNom.end(); iterMots++)
            {
                if (!((*iterMots)->estTexteLibre()))
                    (*iterMots)->setPreposition(sPostposition);
                else
                {
                    // On rajoute la preposition dans le libelle du texte libre.
                    (*iterMots)->setPreposition(sPostposition);
                    (*iterMots)->setTexteLibre(sPostposition + string(" ")
                                                + (*iterMots)->getTexteLibre()) ;
                }
            }
        }
    }

    // Si pMot est un verbe, il peut imposer une preposition au COI ou au COS.

    if (Data.estVerbe())
    {
        if (!(pPh->COI.empty()))
        {
            for (iterMots = pPh->COI.begin(); iterMots != pPh->COI.end(); iterMots++)
            {
                if (!((*iterMots)->estTexteLibre()))
                    (*iterMots)->setPreposition(sPostposition);

                else
                {
                    // On rajoute la preposition dans le libelle du texte libre.
                    (*iterMots)->setPreposition(sPostposition);
                    (*iterMots)->setTexteLibre(sPostposition + string(" ")
                                                + (*iterMots)->getTexteLibre()) ;
                }
            }
        }

        if (!(pPh->COS.empty()))
        {
            for (iterMots = pPh->COS.begin(); iterMots != pPh->COS.end(); iterMots++)
            {
                if (false == (*iterMots)->estTexteLibre())
                    (*iterMots)->setPreposition(sPostposition);

                else
                {
                    // On rajoute la preposition dans le libelle du texte libre.
                    (*iterMots)->setPreposition(sPostposition);
                    (*iterMots)->setTexteLibre(sPostposition + string(" ")
                                                + (*iterMots)->getTexteLibre()) ;
                }
            }
        }
    }

    if (pSpecifique)
    {
        if (!(pSpecifique->empty()))
        {
            for (iterMots = pSpecifique->begin(); iterMots != pSpecifique->end(); iterMots++)
            {
                if (!((*iterMots)->estTexteLibre()))
                    (*iterMots)->setPreposition(sPostposition);

                else
                {
                    // On rajoute la preposition dans le libelle du texte libre.
                    (*iterMots)->setPreposition(sPostposition);
                    (*iterMots)->setTexteLibre(sPostposition + string(" ")
                                                + (*iterMots)->getTexteLibre()) ;
                }
            }
        }
    }

    return true;
}


bool
NSGenerateurEn::traitePrepositionCC(NSPhraseMotArray* pCompC, NSPhraseMot* pPreposition, NSPhraseur::VBPREPOCTX iContexte)
{
  if (pPreposition)
  {
    if (pPreposition->getLexique() != "")
    {
      traitePostposition(pPreposition, pCompC) ;
      return true ;
    }
  }

  // Si on n'a pas specifie de preposition, il faut au moins avoir specifie un contexte.

  if (iContexte == 0)
    return false ;

  // Si on a specifie un contexte, on met une preposition par defaut en fonction du
  // contexte.

  // Pour le lieu, on met "au niveau de".
  if (iContexte == NSPhraseur::prepLieu)
  {
    (pPh->PrepositionLieu).setLexique("1AUNI1") ;
    return traitePrepositionCC(pCompC, &(pPh->PrepositionLieu)) ;
  }

    // Pour le temps, on met..., on met quoi au fait?

    // Pour la maniere, on met "avec"
    else if (iContexte == NSPhraseur::prepManiere)
    {
        (pPh->PrepositionManiere).setLexique("0AVEC1");
        return traitePrepositionCC(pCompC, &(pPh->PrepositionManiere));
    }

    // Pour la cause, mettre "a cause de".
    // Pour le but, mettre "pour".

    // pour le type, on met "a type de"

    else if (iContexte == NSPhraseur::prepType)
    {
        (pPh->PrepositionType).setLexique("ATYPEDE");
        return traitePrepositionCC(pCompC, &(pPh->PrepositionManiere));
    }

    return false;
}


string
NSGenerateurEn::postTraitement(string *sEntree)
{
    if (*sEntree == "")
        return "";

    string sSortie = *sEntree;

    size_t posit1;

    // Contraction de la forme "can not" en "cannot"
    posit1 = sSortie.find("can not ");
    while (posit1 != NPOS)
    {
        if (posit1 == 0)
            sSortie.replace(0, 8, "cannot ");
        else if (sSortie[posit1-1] == ' ')
            sSortie.replace(posit1, 8, "cannot ");

        posit1 = sSortie.find("can not ", posit1+1);
    }

    return sSortie ;
}

//
// ////////////////  OBJET  gereDateEn   /////////////////
//

//
//  constructeur copie
//
gereDateEn::gereDateEn(const gereDateEn& src)
           :gereDate(src._pSuper, src.sLang)
{
  gereDate(*this) = (gereDate) src ;
}

//
//  operateur d'affectation
//
gereDateEn&
gereDateEn::operator=(const gereDateEn& src)
{
  if (this == &src)
		return *this ;

  gereDate(*this) = (gereDate) src ;

  return *this ;
}

//  +-----------------------------------------------------------------+
//  ¦               Donne la date au format JJ/MM/AAAA                ¦
//  +-----------------------------------------------------------------+
//
void
gereDateEn::donne_date_breve(string* pMessage, string* pIntro, int iType)
{
    *pMessage = "";
    *pIntro 	 = "";
  	//
  	// Date au format AAAAMMJJ
	//
  	if (sUnite == "2DA01")
  	{
        // Date vide
        if (sDate == "00000000")
      	    return;
        // Juste AAAA -> "en 2001"
        else if (string(sDate, 4, 4) == "0000")
        {
      	    *pMessage = string(sDate, 0, 4);
            switch (iType)
            {
                case DateNormal :   *pIntro = "in ";
                                    break;
                case DateDeb :      *pIntro = "from ";
                                    break;
                case DateFin :      *pIntro = "to ";
                                    break;
            }
        }
        // MM/AAAA -> "en juillet 2001"
        else if (string(sDate, 6, 2) == "00")
        {
      	    *pMessage = donne_mois(string(sDate, 4, 2)) + " " + string(sDate, 0, 4);
            switch (iType)
            {
                case DateNormal :   *pIntro = "in ";
                                    break;
                case DateDeb :      *pIntro = "from ";
                                    break;
                case DateFin :      *pIntro = "to ";
                                    break;
            }
        }
        // JJ/MM/AAAA -> "le 25/07/2001"
        else
        {
      	    *pMessage = string(sDate, 6, 2) + "/" + string(sDate, 4, 2) + "/" + string(sDate, 0, 4);
            switch (iType)
            {
                case DateNormal :   *pIntro = "on the ";
                                    break;
                case DateDeb :      *pIntro = "from the ";
                                    break;
                case DateFin :      *pIntro = "to the ";
                                    break;
            }
        }
    }
    //
  	// Date au format AAAAMMJJhhmmss
	//
  	if (sUnite == "2DA02")
  	{
        // Date vide
        if (sDate == "00000000")
      	    return;
        // Juste AAAA -> "en 2001"
        else if (string(sDate, 4, 4) == "0000")
        {
      	    *pMessage = string(sDate, 0, 4);
            switch (iType)
            {
                case DateNormal :   *pIntro = "in ";
                                    break;
                case DateDeb :      *pIntro = "from ";
                                    break;
                case DateFin :      *pIntro = "to ";
                                    break;
            }
        }
        // MM/AAAA -> "en juillet 2001"
        else if (string(sDate, 6, 2) == "00")
        {
      	    *pMessage = donne_mois(string(sDate, 4, 2)) + " " + string(sDate, 0, 4);
            switch (iType)
            {
                case DateNormal :   *pIntro = "in ";
                                    break;
                case DateDeb :      *pIntro = "from ";
                                    break;
                case DateFin :      *pIntro = "to ";
                                    break;
            }
        }
        // JJ/MM/AAAA -> "le 25/07/2001"
        else
        {
      	    *pMessage = string(sDate, 6, 2) + "/" + string(sDate, 4, 2) + "/" + string(sDate, 0, 4);
            switch (iType)
            {
                case DateNormal :   *pIntro = "on the ";
                                    break;
                case DateDeb :      *pIntro = "from the ";
                                    break;
                case DateFin :      *pIntro = "to the ";
                                    break;
            }
            string sHeure = "" ;
            // gestion de l'heure
            if      (strlen(sDate.c_str()) == 10)
                sHeure = string(sDate, 8, 2) + string("h") ;
            else if (strlen(sDate.c_str()) == 12)
                sHeure = string(sDate, 8, 2) + string(":") + string(sDate, 10, 2) ;
            else if (strlen(sDate.c_str()) == 14)
                sHeure = string(sDate, 8, 2) + string(":") + string(sDate, 10, 2) + string(":") + string(sDate, 12, 2) ;

            if (sHeure != "")
                *pMessage += string(" at ") + sHeure ;
        }
    }
    //
  	// Heure au format HHmm ou HH
	//
    else if ((sUnite == "2HE01") || (sUnite == "2HE00"))
    {
        if (sUnite == "2HE01")
            *pMessage = string(sDate, 0, 2) + string(":") + string(sDate, 2, 2) ;
        if (sUnite == "2HE00")
            *pMessage = sDate + string(":") ;

        if ((*pMessage)[0] == '0')
            *pMessage = string(*pMessage, 1, strlen(pMessage->c_str()) - 1) ;

        switch (iType)
        {
            case DateNormal :
                *pIntro = "at " ;
                break;
            case DateDeb :
                *pIntro = "from " ;
                break;
            case DateFin :
                *pIntro = "to " ;
                break;
        }
    }
  	return;
}

//  +-----------------------------------------------------------------+
//  ¦           Donne la date au format "July 12th 1994"            ¦
//  +-----------------------------------------------------------------+
//
void
gereDateEn::donne_date_claire(string* pMessage, string* pIntro, int iType)
{
    unsigned int i;

    *pMessage = "";
    *pIntro 	 = "";
  	//
  	// Date au format AAAAMMJJ
	//
  	if (sUnite == "2DA01")
  	{
        if (strlen(sDate.c_str()) < 8)
            return;

        switch (iType)
        {
            case DateNormal :   *pIntro = "the ";
                                break;
            case DateDeb :      *pIntro = "from the ";
                                break;
            case DateFin :      *pIntro = "to the ";
                                break;
        }
		//
		// Jour
		//
		if (isdigit(sDate[6]) && isdigit(sDate[7]))
		{
			if (sDate[6] == '0')
			{
                if (sDate[7] != 0)
                {
				    if (sDate[7] == '1')
					    *pMessage = "1st ";
				    else if (sDate[7] == '2')
				    {
					    *pMessage = "2nd ";
				    }
                    else if (sDate[7] == '3')
				    {
					    *pMessage = "3rd ";
				    }
                    else
                        *pMessage = sDate[7];
                        *pMessage += "th ";
                }
                else
                {
                    switch (iType)
                    {
                        case DateNormal :   *pIntro = "in ";
                                            break;
                        case DateDeb :      *pIntro = "from ";
                                            break;
                        case DateFin :      *pIntro = "to ";
                                            break;
                    }
                }
			}
			else
			{
				*pMessage = sDate[6];
				*pMessage += sDate[7];
				*pMessage += "th ";
			}
		}
		else
			*pMessage = "?? ";
		//
		// Mois
		//
        string sMois = donne_mois(string(sDate, 4, 2));
        if (sMois != "")
      	    *pMessage += sMois + " " + *pMessage + " ";
        else
      	    *pMessage = "?? ";
        //
    // Annee
		//
		for (i = 0; i < 4; i++)
		{
			if (isdigit(sDate[i]))
				*pMessage += sDate[i];
			else
				*pMessage += '?';
		}
    }
    //
  	// Date au format AAAAMMJJhhmmss
	//
  	else if (sUnite == "2DA02")
  	{
        if (strlen(sDate.c_str()) < 8)
            return;

        string sHeure = "" ;

        switch (iType)
        {
            case DateNormal :   *pIntro = "the ";
                                break;
            case DateDeb :      *pIntro = "from the ";
                                break;
            case DateFin :      *pIntro = "to the ";
                                break;
        }
		//
		// Jour
		//
		if (isdigit(sDate[6]) && isdigit(sDate[7]))
		{
			if (sDate[6] == '0')
			{
                if (sDate[7] != 0)
                {
				    if (sDate[7] == '1')
					    *pMessage = "1st ";
				    else if (sDate[7] == '2')
				    {
					    *pMessage = "2nd ";
				    }
                    else if (sDate[7] == '3')
				    {
					    *pMessage = "3rd ";
				    }
                    else
                        *pMessage = sDate[7];
                        *pMessage += "th ";
                }
                else
                {
                    switch (iType)
                    {
                        case DateNormal :   *pIntro = "in ";
                                            break;
                        case DateDeb :      *pIntro = "from ";
                                            break;
                        case DateFin :      *pIntro = "to ";
                                            break;
                    }
                }
			}
			else
			{
				*pMessage = sDate[6];
				*pMessage += sDate[7];
				*pMessage += "th ";
			}
            // gestion de l'heure
            if      (strlen(sDate.c_str()) == 10)
                sHeure = string(sDate, 8, 2) ;
            else if (strlen(sDate.c_str()) == 12)
                sHeure = string(sDate, 8, 2) + string(":") + string(sDate, 10, 2) ;
            else if (strlen(sDate.c_str()) == 14)
                sHeure = string(sDate, 8, 2) + string(":") + string(sDate, 10, 2) + string(":") + string(sDate, 12, 2) ;
		}
		else
			*pMessage = "?? ";
		//
		// Mois
		//
        string sMois = donne_mois(string(sDate, 4, 2));
        if (sMois != "")
      	    *pMessage += sMois + " " + *pMessage + " ";
        else
      	    *pMessage = "?? ";
        //
    // Annee
		//
		for (i = 0; i < 4; i++)
		{
			if (isdigit(sDate[i]))
				*pMessage += sDate[i];
			else
				*pMessage += '?';
		}

        if (sHeure != "")
            *pMessage += string(" at ") + sHeure ;
    }
    //
  	// Heure au format HHmm ou HH
	//
    else if ((sUnite == "2HE01") || (sUnite == "2HE00"))
    {
        if (sUnite == "2HE01")
            *pMessage = string(sDate, 0, 2) + string("h") + string(sDate, 2, 2) ;
        if (sUnite == "2HE00")
            *pMessage = sDate + string("h") ;

        if ((*pMessage)[0] == '0')
            *pMessage = string(*pMessage, 1, strlen(pMessage->c_str()) - 1) ;

        switch (iType)
        {
            case DateNormal :
                *pIntro = "at " ;
                break;
            case DateDeb :
                *pIntro = "from " ;
                break;
            case DateFin :
                *pIntro = "to " ;
                break;
        }
    }
	return;
}

//  +-----------------------------------------------------------------+
//  ¦          Donne le libelle du mois à partir de "MM"              ¦
//  +-----------------------------------------------------------------+
//  Cree le 14/11/1997 Dernière mise à jour 14/11/1997
string
gereDateEn::donne_mois(string mois)
{
    if (strlen(mois.c_str()) < 2)
   	    return "";
    if ((!(isdigit(mois[0]))) || (!(isdigit(mois[1]))))
   	    return "";
    int i = 10 * donneDigit(mois[0]) + donneDigit(mois[1]);

    return donne_mois(i);
}

//  +-----------------------------------------------------------------+
//  ¦          Donne le libelle du mois à partir d'un int             ¦
//  +-----------------------------------------------------------------+
//  Cree le 14/11/1997 Dernière mise à jour 14/11/1997
string
gereDateEn::donne_mois(int iMois)
{
    switch (iMois)
    {
   	    case  1 : return "January";
		case  2 : return "February";
		case  3 : return "March";
		case  4 : return "April";
		case  5 : return "May";
		case  6 : return "June";
		case  7 : return "July";
		case  8 : return "August";
		case  9 : return "September";
		case 10 : return "Octobre";
		case 11 : return "Novembre";
		case 12 : return "December";
    }
    return "";
}











