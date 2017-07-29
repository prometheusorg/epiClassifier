#include <algorithm>

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

bool
GenComplementSortByPriorityInf(NSGenComplement *pCompl1, NSGenComplement *pCompl2)
{
	return (pCompl1->iPriorite < pCompl2->iPriorite) ;
}

bool
GenComplementSortByPrioritySup(NSGenComplement *pCompl1, NSGenComplement *pCompl2)
{
	return (pCompl1->iPriorite > pCompl2->iPriorite) ;
}

// -------------------------------------------------------------------------
// ---------------------- METHODES DE generateurFr -------------------------
// -------------------------------------------------------------------------

NSGenerateurFr::NSGenerateurFr(NSContexte* pCtx, NsProposition* pPropos, string sLangue)
               :NSGenerateur(pCtx, pPropos, sLangue)
{
  iTempsVerbe  = tempsPresentFr ;
  iAspectVerbe = aspectMoment ;
}

NSGenerateurFr::NSGenerateurFr(NSGenerateurFr& rv)
               :NSGenerateur(rv)
{
  iTempsVerbe  = rv.iTempsVerbe;
  iAspectVerbe = rv.iAspectVerbe;
}

NSGenerateurFr::~NSGenerateurFr()
{
}

NSGenerateurFr&
NSGenerateurFr::operator=(NSGenerateurFr rv)
{
	if (&rv == this)
		return *this ;

	reinitialise() ;

	copieTout(&rv) ;

	iTempsVerbe  = rv.iTempsVerbe ;
	iAspectVerbe = rv.iAspectVerbe ;

	return *this ;
}

// classeTout() : permet de remplir les arrays du generateur a partir de celles
// du phraseur. Les arrays du generateur separent les elements a sens positif
// (dont la certitude est superieure ou egale a WCE50) et les elements a sens
// negatif (dont la certitude est strictement inferieure a WCE50).

void
NSGenerateurFr::classeTout()
{
try
{
  reinitialise() ;

  // En francais, les adjectifs ayant une certitude sont toujours places apres le nom.
  //
  // Les adjectifs ont deja ete tries en avant et apres, donc on ne reverifie pas
  // qu'il s'agit bien d'adjectifs du lexique.
  //
  dispatchArray(&(pPh->adjEpitheteAv), &pAdjEpitheteAvPos, &pAdjEpitheteAvNeg) ;
  dispatchArray(&(pPh->adjEpitheteAp), &pAdjEpitheteApPos, &pAdjEpitheteApNeg) ;

  dispatchArray(&(pPh->compNom), &pCompNomPos, &pCompNomNeg, isNoun) ;

  dispatchArray(&(pPh->adverbe), &pAdverbePos, &pAdverbeNeg, isAdverb) ;

  dispatchArray(&(pPh->COD), &pCODPos, &pCODNeg, isNoun) ;

  dispatchArray(&(pPh->AttSujet), &pAttSujetPos, &pAttSujetNeg) ;
  dispatchArray(&(pPh->AttCOD), &pAttCODPos, &pAttCODNeg) ;

  pCCArray->classeTout() ;
}
catch (...)
{
	erreur("Exception NSGenerateurFr::classeTout", standardError) ;
}
}

// generePhrase(int iStyle) : methode principale de generation de la phrase.

bool
NSGenerateurFr::generePhrase(DCODETYPE iStyle)
{
try
{
  reinitialise() ;
  sPhrase = string("") ;

  bool     trouve;
  NSSuper* pSuper = pContexte->getSuperviseur();
  string   sLexique;
  string   sPluriel;
  string   sCertitude;
  string   sInter2 = "";

  // La phrase est-elle affirmative ou negative ?
  // true pour phrase affirmative, false pour phrase negative.

  bool    bPhraseAffirmative = true ;
  string  sAdverbeModificateur    = string(" ") ;
  string  sAdverbeModificateurAtt = string(" ") ;

  NSPathologData Data;

  string sVerbe = string("") ;

  // Pour les adverbes du verbe

  string sPhraseAdverbe = string("") ;

  // Avant de classer les compléments, on leur met la preposition lorsqu'il y
  // en a une d'imposée par le mot complété.
  // Le verbe peut imposer une préposition au COI ainsi qu'au COS.
  // Il faut traiter les prépositions des complements circonstanciels.

  if      (false == pPh->Verbe.empty())
    traitePostposition(*(pPh->Verbe.begin())) ;
  else if (false == pPh->Sujet.empty())
  {
    if ((NSPhraseur::phrasePrincipale == pPh->iPhraseType) &&
        (NSPhraseur::formePassive != pPh->iForme) &&
        (false == pPh->CCChiffre.empty()))
    {
      // S'il existe un complément de nombre et pas de verbe, on impose
      // le verbe "être" et on ajoute la préposition "de"
      // (le poids est de 70 kg)
      NSPatPathoData PathoData ;
      PathoData.setLexique("4ETRE1") ;
      pPh->Verbe.push_back(new NSPhraseMot(&PathoData, pContexte)) ;

      pPh->PrepositionChiffre.setPreposition(string("de")) ;

      // for (iterPhraseMot itNb = pPh->CCChiffre.begin() ; pPh->CCChiffre.end() != itNb ; itNb++)
      //  if (string("") == (*itNb)->getPreposition())
      //    (*itNb)->setPreposition(string("de")) ;
    }
  }

  traitePrepositionCC(&(pPh->CCLieu),     &(pPh->PrepositionLieu),    NSPhraseur::prepLieu) ;
  traitePrepositionCC(&(pPh->CCTemps),    &(pPh->PrepositionTemps),   NSPhraseur::prepTemps) ;
  traitePrepositionCC(&(pPh->CCManiere),  &(pPh->PrepositionManiere), NSPhraseur::prepManiere) ;
  traitePrepositionCC(&(pPh->CCMoyen),    &(pPh->PrepositionMoyen),   NSPhraseur::prepMoyen) ;
  traitePrepositionCC(&(pPh->CCCause),    &(pPh->PrepositionCause),   NSPhraseur::prepCause) ;
  traitePrepositionCC(&(pPh->CCBut),      &(pPh->PrepositionBut),     NSPhraseur::prepBut) ;
  traitePrepositionCC(&(pPh->CCType),     &(pPh->PrepositionType),    NSPhraseur::prepType) ;
  traitePrepositionCC(&(pPh->CCHypoth),   &(pPh->PrepositionHypoth),  NSPhraseur::prepHypoth) ;
  traitePrepositionCC(&(pPh->CCChiffre),  &(pPh->PrepositionChiffre), NSPhraseur::prepChiffre) ;

  // Ensuite on classe.
  classeTout() ;

  //
  // PREPARATION DES COMPOSANTS
  //

  // On transforme la gestion du verbe d'internationale a francaise.
  //

  mapTempsVerbe() ;

  // Dans tous les composants, on peut avoir des caracteres libres. Ceux-ci ont
  // le code "£C;020". Leur lexique est ce qu'il faut afficher.

  // Adverbes du verbe. (stockés dans le complément du verbe.)

  if (false == prepareVerbe(&sPhraseAdverbe, &sAdverbeModificateur, &bPhraseAffirmative, iStyle))
    return false ;

  //
  // Sujet
  //
  string sSujet      = "" ;
  GENRE  iGenreSujet = genreNull ;
  string sAttSujet   = "" ;

  if (false == prepareSujet(&sSujet, &iGenreSujet, &sAttSujet, &sAdverbeModificateur, &bPhraseAffirmative, iStyle))
    return false ;

  // On traite le COD.
  // On met d'abord les complements positifs, puis les négatifs, avec "mais pas de" entre.
  // S'il n'y a que des négatifs, la phrase devient négative.
  //

  string sCOD      = string("") ;
  string sAttCOD   = string("") ;
  GENRE  iGenreCOD = genreNull ;

  if (false == prepareCOD(&sCOD, &iGenreCOD, &sAttCOD, &sAdverbeModificateur, &bPhraseAffirmative, iStyle))
    return false ;

  //
  // Compléments : quand, où, comment, avec quoi, pourquoi, dans quel but
  //
  int iComplLevelThreshold = 101 ;

  pCCArray->donnePhrase() ;
  if (false == pCCArray->empty())
    sort(pCCArray->begin(), pCCArray->end(), GenComplementSortByPrioritySup) ;

  //
  // Cas de la phrase principale
  //
  if (NSPhraseur::phrasePrincipale == pPh->iPhraseType)
  {
    // Phrase classique (pas en mode "tiret")
    //
    if (dcPhrase == iStyle)
    {
      //
      // Cas classique : FORME ACTIVE
      //
      if (NSPhraseur::formePassive != pPh->iForme)
      {
        // On commence par inscrire les compléments de priorité >= 90
        // "au niveau du côlon droit, il existe un polype"
        //
        if (false == pCCArray->empty())
        {
          for (GenCompIter itCompl = pCCArray->begin(); itCompl != pCCArray->end(); itCompl++)
          {
            if ((*itCompl)->iPriorite >= 90)
            {
              if ((*itCompl)->sPositif != "")
                sPhrase += (*itCompl)->sPositif + string(", ") ;
              if ((*itCompl)->sNegatif != "")
                sPhrase += string("pas ") + (*itCompl)->sNegatif + string(", ") ;
            }
          }
          iComplLevelThreshold = 90 ;
        }
        //
        //     P
        //   /  \
        //  GN  GV
        //
        //
        if (string("") != sSujet)
        {
          if (NSPhraseur::sujetNoRepeat == pPh->iTypeSujet)
          {
            NSPhraseur::VBPERSO iPersonVb ;
            if ((genreFP == iGenreSujet) || (genreMP == iGenreSujet) || (genreNP == iGenreSujet))
              iPersonVb = NSPhraseur::pers3P ;
            else
              iPersonVb = NSPhraseur::pers3S ;
            sPhrase += donnePronomPersonnel(iGenreSujet, iPersonVb, STR_SUJET) ;
          }
          else
            sPhrase += sSujet ;

          // Cas d'une vraie phrase (iStyle == dcPhrase)
          //
          //
          //                P
          //              /  \         \
          //             GN  GV        GN prep
          //                / \
          //               V  GN (COD)
          //   ex : Ma fille a conduit la voiture sur l'autoroute
          //
          // Il n'est pas possible d'avoir à la fois un COD et un attribut
          // du sujet (qui appartiennent tous deux au GV)
          //                       Cet aspect
          //                          /  \
          //              est atypique   évoque un kyste
          //
          if ((string("") != sCOD) && (string("") != sAttSujet))
            return false ;

          // L'existence d'un COD impose celle d'un verbe
          if ((string("") != sCOD) && (pPh->Verbe.empty()))
            return false ;

          // S'il existe un attribut et pas de verbe, on impose
          // le verbe être
          if ((string("") != sAttSujet) && (pPh->Verbe.empty()))
          {
            NSPatPathoData PathoData ;
            PathoData.setLexique("4ETRE1") ;
            pPh->Verbe.push_back(new NSPhraseMot(&PathoData, pContexte)) ;
          }

          if ((genreFP == iGenreSujet) || (genreMP == iGenreSujet))
            pPh->iVbPersonne = NSPhraseur::pers3P ;
          else
            pPh->iVbPersonne = NSPhraseur::pers3S ;

          // On traite au passage le cas ou la phrase est negative

          string sPrincipal, sAuxilliaire ;
          donneVerbe(&sPrincipal, &sAuxilliaire) ;
          if (string("") != sAuxilliaire)
          {
            if (false == bPhraseAffirmative)
            {
              // L'auxilliaire (être ou avoir) ne commence jamais par un h.
              if (CommenceParVoyelle(&sAuxilliaire))
                sVerbe += string("n'") + sAuxilliaire + sAdverbeModificateur + string("pas") ;
              else
                sVerbe += string("ne ") + sAuxilliaire + sAdverbeModificateur + string("pas") ;

              if (string("") != sPhraseAdverbe)
                sVerbe += string(" ") + sPhraseAdverbe ;

              if (string("") != sPrincipal)
                sVerbe += string(" ") + sPrincipal ;
            }
            else
            {
              sVerbe += sAuxilliaire ;
              if (sPrincipal != "")
                sVerbe += string(" ") + sPrincipal ;

              if (sPhraseAdverbe != "")
                sVerbe += string(" ") + sPhraseAdverbe ;
            }
          }
          else
          {
            if (false == bPhraseAffirmative)
            {
              // On vérifie si le verbe commence par une voyelle ou un h muet.
              if (CommenceParVoyelle(&sPrincipal))
                sVerbe += string("n'") + sPrincipal + sAdverbeModificateur + string("pas");
              else
              {
                string sVb = (*(pPh->Verbe.begin()))->getLexique() ;
                trouve = pContexte->getDico()->trouvePathologData(sLang, &sVb, &Data) ;
                if (false == trouve)
                  return false ;
                if (Data.chercheGrammaire(VALEUR_DE_H) == H_MUET)
                  sVerbe += "n'" + string(" ") + sPrincipal + sAdverbeModificateur + string("pas");
                else
                  sVerbe += string("ne ") + sPrincipal + sAdverbeModificateur + string("pas");
              }
            }
            else
              sVerbe += sPrincipal ;

            if (string("") != sPhraseAdverbe)
              sVerbe += string(" ") + sPhraseAdverbe ;
          }

          sPhrase += string(" ") + sVerbe ;

          if (sCOD != "")
            sPhrase += string(" ") + sCOD ;
          if (sAttCOD != "")
            sPhrase += string(" ") + sAttCOD ;
          if (sAttSujet != "")
            sPhrase += string(" ") + sAttSujet ;
        }
        else // Pas de sujet.
        {
          //
          // On s'occupe du verbe
          //
          if (false == pPh->Verbe.empty())
          {
            // Création d'un sujet virtuel : "on" pour les
            // verbes d'action et "il" pour les verbes d'état

            string sTransitivite ;
            string sConjugaison ;
            string sActEtat ;

            if (false == donneVerbeClasse(*(pPh->Verbe.begin()), &sTransitivite, &sConjugaison, &sActEtat))
              return false ;

            switch (pPh->iVbType)
            {
              case NSPhraseur::vbTypeAction : sPhrase += string("on") ; break ;
              case NSPhraseur::vbTypeEtat   : sPhrase += string("il") ; break ;
            }
            pPh->iVbPersonne = NSPhraseur::pers3S ;

            string sPrincipal, sAuxilliaire ;
            donneVerbe(&sPrincipal, &sAuxilliaire) ;

            // Phrase affirmative
            //
            if (bPhraseAffirmative)
            {
              if (sAuxilliaire != "")
                sVerbe = string(" ") + sAuxilliaire ;
              if (sPrincipal != "")
                sVerbe += string(" ") + sPrincipal ;

              if (sPhraseAdverbe != "")
                sVerbe += string(" ") + sPhraseAdverbe ;
            }
            // Phrase négative
            //
            else
            {
              if (sAuxilliaire != "")
              {
                if (CommenceParVoyelle(&sAuxilliaire))
                  sVerbe += " n'" + sAuxilliaire + sAdverbeModificateur + string("pas");
                else
                  sVerbe += string(" ne ") + sAuxilliaire + sAdverbeModificateur + string("pas");

                if (sPhraseAdverbe != "")
                  sVerbe += string(" ") + sPhraseAdverbe ;

                if (sPrincipal != "")
                  sVerbe += string(" ") + sPrincipal ;
              }
              else
              {
                // On verifie si le verbe commence par une voyelle ou un h muet
                if (CommenceParVoyelle(&sPrincipal))
                  sVerbe += string(" n'") + sPrincipal + sAdverbeModificateur + string("pas") ;
                else
                {
                  string sVb = (*(pPh->Verbe.begin()))->getLexique() ;
                  trouve = pContexte->getDico()->trouvePathologData(sLang, &sVb, &Data) ;
                  if (false == trouve)
                    return false ;
                  if (Data.chercheGrammaire(VALEUR_DE_H) == H_MUET)
                    sVerbe += string(" n'") + sPrincipal + sAdverbeModificateur + string("pas") ;
                  else
                    sVerbe += string(" ne ") + sPrincipal + sAdverbeModificateur + string("pas") ;
                }

                if (sPhraseAdverbe != "")
                  sVerbe += string(" ") + sPhraseAdverbe ;
              }
            }
          }
          //
          // Ni verbe, ni sujet -> phrase sur le mode "il existe" ou "on note"
          //
          else if (string("") != sCOD)
          {
            NSPatPathoData PathoData ;
            PathoData.setLexique("4EXIS1") ;
            pPh->Verbe.push_back(new NSPhraseMot(&PathoData, pContexte)) ;

            sPhrase += string("il") ;
            pPh->iVbPersonne = NSPhraseur::pers3S ;

            string sPrincipal, sAuxilliaire ;
            donneVerbe(&sPrincipal, &sAuxilliaire) ;
            if (bPhraseAffirmative)
            {
              if (string("") != sAuxilliaire)
                sVerbe = string(" ") + sAuxilliaire ;
              if (string("") != sPrincipal)
                sVerbe += string(" ") + sPrincipal ;
            }

            // Phrase négative

            else
            {
              if (sAuxilliaire != "")
              {
                if (CommenceParVoyelle(&sAuxilliaire))
                  sVerbe += string(" n'") + sAuxilliaire + sAdverbeModificateur + string("pas") ;
                else
                  sVerbe += string(" ne ") + sAuxilliaire + sAdverbeModificateur + string("pas") ;

                if (sPrincipal != "")
                  sVerbe += string(" ") + sPrincipal ;
              }
              else
              {
                // On vérifie si le verbe commence par une voyelle ou un h muet
                if (CommenceParVoyelle(&sPrincipal))
                  sVerbe += string(" n'") + sPrincipal + sAdverbeModificateur + string("pas") ;
                else
                {
                  string sVb = (*(pPh->Verbe.begin()))->getLexique() ;
                  trouve = pContexte->getDico()->trouvePathologData(sLang, &sVb, &Data) ;
                  if (false == trouve)
                    return false ;
                  if (Data.chercheGrammaire(VALEUR_DE_H) == H_MUET)
                    sVerbe += string(" n'") + sPrincipal + sAdverbeModificateur + string("pas") ;
                  else
                    sVerbe += string(" ne ") + sPrincipal + sAdverbeModificateur + string("pas") ;
                }
              }
            }
            sPhrase += sVerbe ;
            if (string("") != sCOD)
              sPhrase += string(" ") + sCOD ;
            if (string("") != sAttCOD)
              sPhrase += string(" ") + sAttCOD ;
          }
        }
      }
      //
      // FORME PASSIVE
      //
      // Le sujet et le COD inversent leur rôle ; le temps est donné par
      // le verbe être, le verbe est au participe passé
      //
      // Ex Jean construit la maison -> La maison est construite par Jean
      //
      else
      {
        // Il faut forcément un COD, sinon on se retrouve sans sujet
        if (sCOD == "")
          return false ;
        // Il faut forcément un verbe
        if (pPh->Verbe.empty())
          return false ;

        if (pPh->iTypeSujet == NSPhraseur::sujetNoRepeat)
        {
          NSPhraseur::VBPERSO iPersonVb ;
          if ((iGenreCOD == genreFP) || (iGenreCOD == genreMP) || (iGenreCOD == genreNP))
            iPersonVb = NSPhraseur::pers3P ;
          else
            iPersonVb = NSPhraseur::pers3S ;
          sPhrase = donnePronomPersonnel(iGenreSujet, iPersonVb, STR_SUJET) ;
        }
        else
          sPhrase = sCOD ;

        //
        // On prend le verbe au participe passé, c'est le verbe être
        // qui est conjugué
        //
        // Il faut necessairement un verbe.

        if (pPh->Verbe.empty())
          return false ;

        string sVb = donneParticipePasse(*(pPh->Verbe.begin()), iGenreCOD) ;

        pPh->Verbe.vider() ;
        NSPatPathoData PathoData ;
        PathoData.setLexique("4ETRE1") ;
        pPh->Verbe.push_back(new NSPhraseMot(&PathoData, pContexte)) ;

        if ((genreFP == iGenreCOD) || (genreMP == iGenreCOD))
          pPh->iVbPersonne = NSPhraseur::pers3P ;
        else
          pPh->iVbPersonne = NSPhraseur::pers3S ;

        string sPrincipal, sAuxilliaire ;
        donneVerbe(&sPrincipal, &sAuxilliaire) ;

        // Phrase affirmative


        // On traite les adverbes.
        // Si la phrase est négative, on les met entre le verbe être et
        // l'auxilliaire.

        if (bPhraseAffirmative)
        {
          if (sAuxilliaire != "")
            sVerbe = string(" ") + sAuxilliaire ;
          if (sPrincipal != "")
            sVerbe += string(" ") + sPrincipal ;
        }

        // Phrase négative

        else
        {
          if (sAuxilliaire != "")
          {
            if (CommenceParVoyelle(&sAuxilliaire))
              sVerbe += " n'" + sAuxilliaire + sAdverbeModificateur + "pas" ;
            else
              sVerbe += " ne " + sAuxilliaire + sAdverbeModificateur + "pas" ;

            if (sPrincipal != "")
              sVerbe += string(" ") + sPrincipal ;
          }
          else
          {
            // On verifie si le verbe commence par une voyelle ou un h muet
            if (CommenceParVoyelle(&sPrincipal))
              sVerbe += " n'" + sPrincipal + sAdverbeModificateur + "pas" ;
            else
            {
              string sVb = (*(pPh->Verbe.begin()))->getLexique() ;
              trouve = pContexte->getDico()->trouvePathologData(sLang, &sVb, &Data) ;
              if (false == trouve)
                return false ;
              if (Data.chercheGrammaire(VALEUR_DE_H) == H_MUET)
                sVerbe += " n'" + sPrincipal + sAdverbeModificateur + "pas" ;
              else
                sVerbe += " ne " + sPrincipal + sAdverbeModificateur + "pas" ;
            }
          }
        }

        if (bPhraseAffirmative)
        {
          sPhrase += sVerbe + string(" ") + sVb ;

          if (sPhraseAdverbe != "")
            sPhrase += string(" ") + sPhraseAdverbe ;
        }
        else
        {
          sPhrase += sVerbe ;

          if (sPhraseAdverbe != "")
            sPhrase += string(" ") + sPhraseAdverbe ;

          sPhrase += string(" ") + sVb ;
        }

        // Après le verbe, on met l'attribut du COD :
        //
        // J'ai trouve vos propositions interessantes

        // -> Vos propositions ont ete trouvees interessantes par moi.


        if (string("") != sAttCOD)
          sPhrase += string(" ") + sAttCOD ;

        //
        // S'il y a un sujet, on ajoute "par" devant.
        // Il peut ne pas y avoir de sujet, par exemple
        // "La vésicule est étudiée en decubitus dorsal"
        //
        if (string("") != sSujet)
          sPhrase += string(" par ") + sSujet ;
      }
    }

    else if (dcTiret == iStyle)
    {
      // C'est comme la forme passive, sauf qu'on enlève le verbe être, et
      // qu'on enlève les articles du COD.

      // Deux cas de figure :
      // - "vraie phrase" et il faut un COD
      // - énumération de valeurs chiffrées

      // Il faut forcément un COD, ou des valeurs chiffrées
      // if ((sCOD == "") && (pPh->CCChiffre.empty()))
      //     return false;

      // S'il y a un COD mais pas de verbe, c'est juste une énumeration.
      if (string("") != sCOD)
        sPhrase = sCOD ;

      if (false == pPh->Verbe.empty())
      {
        // Il faut forcément un COD, sinon on se retrouve sans sujet
        if (string("") == sCOD)
          return false ;

        //
        // On prend le verbe au participe passé
        //
        string sVb = donneParticipePasse(*(pPh->Verbe.begin()), iGenreCOD) ;

        // Si la phrase est négative, on ajoute "pas", ou "absolument pas" etc...
        // juste avant le participe passé.

        if (false == bPhraseAffirmative)
        {
          sPhrase += sAdverbeModificateur + string("pas ") ;

          if (string("") != sPhraseAdverbe)
            sPhrase += sPhraseAdverbe += string(" ") ;

          sPhrase += sVb ;
        }
        else
        {
          sPhrase += string(" ") + sVb ;

          if (string("") != sPhraseAdverbe)
            sPhrase += string(" ") + sPhraseAdverbe ;
        }

        // Propositions trouvées intéressantes par Jean.

        if (sAttCOD != "")
          sPhrase += string(" ") + sAttCOD ;
        //
        // S'il y a un sujet, on ajoute "par" devant.
        // Il peut ne pas y avoir de sujet, par exemple
        // "Vésicule étudiée en decubitus dorsal"
        //
        if (sSujet != "")
          sPhrase += string(" par ") + sSujet ;
      }
    }
  }

  //
  // Compléments
  //
  // Penser à pouvoir forcer l'ordre (importance) des Compléments.
  //
  // Ceci peut être réalisé en utilisant l'int iPriorite de NSGenComplement.

  if (false == pCCArray->empty())
  {
    for (GenCompIter itCompl = pCCArray->begin(); itCompl != pCCArray->end(); itCompl++)
    {
      if ((*itCompl)->iPriorite < iComplLevelThreshold)
      {
        if ((*itCompl)->sPositif != "")
          sPhrase += " " + (*itCompl)->sPositif ;
        if ((*itCompl)->sNegatif != "")
          sPhrase += string(" mais pas ") + (*itCompl)->sNegatif ;
      }
    }
  }

  //
  // Cas de la subordonnée relative
  //
  // La subordonnée relative complète un nom ou un groupe nominal appartenant
  // à la principale. Elle fait partie des expansions du nom au même titre que
  // le complément du nom et l'adjectif.

  if (NSPhraseur::phraseRelative == pPh->iPhraseType)
  {
    if (dcPhrase == iStyle)
    {
      //
      // Cas classique : FORME ACTIVE
      //
      if (NSPhraseur::formePassive != pPh->iForme)
      {
        //
        //     P
        //   /  \
        //  GN  GV
        //
        //
        if (string("") != sSujet)
        {
          sPhrase = string("qui") ;
          // Cas d'une vraie phrase (iStyle == dcPhrase)
          //
          //
          //                P
          //              /  \         \
          //             GN  GV        GN prep
          //                / \
          //               V  GN (COD)
          //   ex : Ma fille qui a conduit la voiture sur l'autoroute
          //
          // Il n'est pas possible d'avoir à la fois un COD et un attribut
          // du sujet (qui appartiennent tous deux au GV)
          //                       Cet aspect
          //                          /  \
          //              est atypique   évoque un kyste
          //
          if ((string("") != sCOD) && (string("") != sAttSujet))
            return false ;

          // L'existence d'un COD impose celle d'un verbe
          if ((string("") != sCOD) && (pPh->Verbe.empty()))
            return false ;

          // S'il existe un attribut et pas de verbe, on impose
          // le verbe être
          if ((string("") != sAttSujet) && (pPh->Verbe.empty()))
          {
            NSPatPathoData PathoData ;
            PathoData.setLexique("4ETRE1") ;
            pPh->Verbe.push_back(new NSPhraseMot(&PathoData, pContexte)) ;
          }

          if ((genreFP == iGenreSujet) || (genreMP == iGenreSujet))
            pPh->iVbPersonne = NSPhraseur::pers3P ;
          else
            pPh->iVbPersonne = NSPhraseur::pers3S ;

          // On traite au passage le cas où la phrase est négative

          string sPrincipal, sAuxilliaire ;
          donneVerbe(&sPrincipal, &sAuxilliaire) ;
          if (string("") != sAuxilliaire)
          {
            if (false == bPhraseAffirmative)
            {
              // L'auxilliaire (être ou avoir) ne commence jamais par un h.
              if (CommenceParVoyelle(&sAuxilliaire))
                sVerbe += string("n'") + sAuxilliaire + sAdverbeModificateur + string("pas") ;
              else
                sVerbe += string("ne ") + sAuxilliaire + sAdverbeModificateur + string("pas") ;

              if (string("") != sPhraseAdverbe)
                sVerbe += string(" ") + sPhraseAdverbe ;

              if (string("") != sPrincipal)
                sVerbe += string(" ") + sPrincipal ;
            }
            else
            {
              sVerbe += sAuxilliaire ;
              if (string("") != sPrincipal)
                sVerbe += string(" ") + sPrincipal ;

              if (string("") != sPhraseAdverbe)
                sVerbe += string(" ") + sPhraseAdverbe ;
            }
          }
          else
          {
            if (false == bPhraseAffirmative)
            {
              // On vérifie si le verbe commence par une voyelle ou un h muet.
              if (CommenceParVoyelle(&sPrincipal))
                sVerbe += string("n'") + sPrincipal + sAdverbeModificateur + string("pas") ;
              else
              {
                string sVb = (*(pPh->Verbe.begin()))->getLexique() ;
                trouve = pContexte->getDico()->trouvePathologData(sLang, &sVb, &Data) ;
                if (false == trouve)
                  return false ;
                if (Data.chercheGrammaire(VALEUR_DE_H) == H_MUET)
                  sVerbe += "n'" + string(" ") + sPrincipal + sAdverbeModificateur + string("pas") ;
                else
                  sVerbe += string("ne ") + sPrincipal + sAdverbeModificateur + string("pas") ;
              }
            }
            else
              sVerbe += sPrincipal ;

            if (string("") != sPhraseAdverbe)
              sVerbe += string(" ") + sPhraseAdverbe ;
          }

          sPhrase += string(" ") + sVerbe ;

          if (string("") != sCOD)
            sPhrase += string(" ") + sCOD ;
          if (string("") != sAttCOD)
            sPhrase += string(" ") + sAttCOD ;
          if (string("") != sAttSujet)
            sPhrase += string(" ") + sAttSujet ;
        }
        // Pas de sujet. Impossible, puisqu'on a normalement rempli
        // la case sujet avant d'appeler generePhrase.
        //
        else
          return false ;
      }
      //
      // FORME PASSIVE
      //
      // Ca donne :
      //
      // La personne par qui le drame est arrivé.

      else
      {
        // Il faut forcément un COD, sinon on se retrouve sans sujet.
        if (string("") == sCOD)
          return false ;

        // Il faut forcément un verbe
        if (pPh->Verbe.empty())
          return false ;

        sPhrase = string("par qui ") + sCOD ;

        //
        // On prend le verbe au participe passé, c'est le verbe être
        // qui est conjugué
        //
        // Il faut nécessairement un verbe.

        string sVb = donneParticipePasse(*(pPh->Verbe.begin()), iGenreCOD) ;

        pPh->Verbe.vider() ;
        NSPatPathoData PathoData ;
        PathoData.setLexique("4ETRE1") ;
        pPh->Verbe.push_back(new NSPhraseMot(&PathoData, pContexte)) ;

        if ((genreFP == iGenreCOD) || (genreMP == iGenreCOD))
          pPh->iVbPersonne = NSPhraseur::pers3P ;
        else
          pPh->iVbPersonne = NSPhraseur::pers3S ;

        string sPrincipal, sAuxilliaire ;
        donneVerbe(&sPrincipal, &sAuxilliaire) ;

        // Phrase affirmative
        //
        if (bPhraseAffirmative)
        {
          if (string("") != sAuxilliaire)
            sVerbe = string(" ") + sAuxilliaire ;
          if (string("") != sPrincipal)
            sVerbe += string(" ") + sPrincipal ;
        }
        //
        // Phrase negative
        //
        else
        {
          if (string("") != sAuxilliaire)
          {
            if (CommenceParVoyelle(&sAuxilliaire))
              sVerbe += " n'" + sAuxilliaire + sAdverbeModificateur + "pas" ;
            else
              sVerbe += " ne " + sAuxilliaire + sAdverbeModificateur + "pas" ;

            if (string("") != sPrincipal)
              sVerbe += string(" ") + sPrincipal ;
          }
          else
          {
            // On verifie si le verbe commence par une voyelle ou un h muet
            if (CommenceParVoyelle(&sPrincipal))
              sVerbe += " n'" + sPrincipal + sAdverbeModificateur + "pas" ;
            else
            {
              string sVb = (*(pPh->Verbe.begin()))->getLexique() ;
              trouve = pContexte->getDico()->trouvePathologData(sLang, &sVb, &Data) ;
              if (false == trouve)
                return false ;
              if (Data.chercheGrammaire(VALEUR_DE_H) == H_MUET)
                sVerbe += " n'" + sPrincipal + sAdverbeModificateur + "pas" ;
              else
                sVerbe += " ne " + sPrincipal + sAdverbeModificateur + "pas" ;
            }
          }
        }

        if (bPhraseAffirmative)
        {
          sPhrase += sVerbe + string(" ") + sVb ;

          if (string("") != sPhraseAdverbe)
            sPhrase += string(" ") + sPhraseAdverbe ;
        }
        else
        {
          sPhrase += sVerbe ;

          if (string("") != sPhraseAdverbe)
            sPhrase += string(" ") + sPhraseAdverbe ;

          sPhrase += string(" ") + sVb ;
        }

        // Apres le verbe, on met l'attribut du COD :
        //
        // J'ai trouve vos propositions interessantes

        // -> Vos propositions ont ete trouvees interessantes par moi.
        //
        if (string("") != sAttCOD)
          sPhrase += string(" ") + sAttCOD ;
      }
    }

    else if (dcTiret == iStyle)
    {
      // Impossible : il faut un verbe dans une subordonnée relative.
      return false ;
    }
  }

  // Remarquons qu'on ne traite pas les compléments dans le cas d'une subordonnée
  // relative.
  // Ceux-ci sont traites dans genereNomComplet, car ils se placent devant la
  // subordonnee relative.

  // A la fin, on rajoute la phrase qui se place apres les " : ".

  if (pPh->pDeuxPoints)
  {
    NsProposition  Propos(pContexte, &(pPh->pDeuxPoints), NsProposition::deuxPoints, NsProposition::notSetConjonct, false) ;
    NSGenerateurFr GeneDeuxPoints(pContexte, &Propos, sLang) ;
    GeneDeuxPoints.genereProposition(iStyle) ;
    sPhrase += string(" : ") + GeneDeuxPoints.getPropositionPhrase() ;
  }

  // Elimination des éventuels blancs initiaux
  strip(sPhrase, stripLeft) ;

  sPhrase = postTraitement(&sPhrase) ;

  return true ;
}
catch (...)
{
	erreur("Exception NSGenerateurFr::generePhrase.", standardError) ;
	return false ;
}
}

bool
NSGenerateurFr::assembleProposition(DCODETYPE /* iStyle */, NsProposition* pPropos)
{
  NsProposition* pCurrentProp ;
  if (pPropos)
    pCurrentProp = pPropos ;
  else
    pCurrentProp = pProposition ;

  if (NULL == pCurrentProp)
    return false ;

  if (NsProposition::isPropositionArray != pCurrentProp->iObjectType)
    return true ;

  NSPropositionArray* pPropArray = (static_cast<NSPropositionArray*>(pCurrentProp->pProposition)) ;
  if (pPropArray->empty())
    return true ;

  iterProposition itPr = pPropArray->begin() ;
  pCurrentProp->sPhrase = (*itPr)->sPhrase ;

  itPr++ ;

  while (pPropArray->end() != itPr)
  {
    NsProposition* pItProp = *itPr ;
    itPr++ ;

    if (string("") != pItProp->sPhrase)
    {
      switch (pItProp->iPropositionType)
      {
        case NsProposition::principale :
          if (pPropArray->end() != itPr)
            pCurrentProp->sPhrase += string(", ") + pItProp->sPhrase ;
          else
            pCurrentProp->sPhrase += string(" et ") + pItProp->sPhrase ;
          break ;
        //
        // Completives
        //
        case NsProposition::completiveQue :                          // SCQ : Je veux que tu reviennes
          pCurrentProp->sPhrase += string(" que ") + pItProp->sPhrase ;
          break ;
        case NsProposition::completiveInfinitive :                   // SCI : J'entends les chats miauler
          pCurrentProp->sPhrase += pItProp->sPhrase ;
          break ;
        case NsProposition::completiveInterrogative :                // SCN : Je voudrais savoir pourquoi tu ne veux pas
          pCurrentProp->sPhrase += string(" pourquoi ") + pItProp->sPhrase ;
          break ;
        //
        // Circonstancielles
        //
        // - Temporelle
        case NsProposition::circonstancielleTemporelleAnterieure :   // SCTA : Je le verrai avant qu'il chante
          switch (pItProp->iConjonctionType)
          {
            case NsProposition::SCTA_AvantQue :        // ---X---|
              pCurrentProp->sPhrase += string(" avant que ") + pItProp->sPhrase ;
              break ;
            case NsProposition::SCTA_JusteAvantQue :   // ------X|
              pCurrentProp->sPhrase += string(" juste avant que ") + pItProp->sPhrase ;
              break ;
            case NsProposition::SCTA_JusquAceQue :     // --XXXXX|
              pCurrentProp->sPhrase += string(" jusqu'à ce que ") + pItProp->sPhrase ;
              break ;
            default :
              pCurrentProp->sPhrase += string(" avant que ") + pItProp->sPhrase ;
          }
          break ;
        case NsProposition::circonstancielleTemporelleSimultanee :   // SCTS : Je chantais au moment où il arriva
          pCurrentProp->sPhrase += string(" au moment où ") + pItProp->sPhrase ;
          break ;
        case NsProposition::circonstancielleTemporellePosterieure :  // SCTP : Je chanterai après que vous serez parti
          switch (pItProp->iConjonctionType)
          {
            case NsProposition::SCTP_ApresQue :          // ---|---X
              pCurrentProp->sPhrase += string(" après que ") + pItProp->sPhrase ;
              break ;
            case NsProposition::SCTP_DesQue :            // ---|X---
              pCurrentProp->sPhrase += string(" dès que ") + pItProp->sPhrase ;
              break ;
            case NsProposition::SCTP_DepuisQue :         // ---|XXXX
              pCurrentProp->sPhrase += string(" depuis que ") + pItProp->sPhrase ;
              break ;
            default :
              pCurrentProp->sPhrase += string(" avant que ") + pItProp->sPhrase ;
          }
          break ;
        // - Consécutive -> résultat de l'action
        case NsProposition::circonstancielleConsecutive :            // SCR : Il fait trop froid pour que je chante
          pCurrentProp->sPhrase += string(" pour que ") + pItProp->sPhrase ;
          break ;
        // - Causale -> cause de l'action
        case NsProposition::circonstancielleCausale :                // SCC : Il a réussi parce qu'il chantait bien
          pCurrentProp->sPhrase += string(" parce que ") + pItProp->sPhrase ;
          break ;
        // - Concessive -> relation inattendue
        case NsProposition::circonstancielleConcessive :             // SCS : Il a échoué bien qu'il chanta bien
          pCurrentProp->sPhrase += string(" bien que ") + pItProp->sPhrase ;
          break ;
        // - Finale -> but de l'action
        case NsProposition::circonstancielleFinale :                 // SCF : Il chante fort pour qu'on l'entende bien
          pCurrentProp->sPhrase += string(" afin que ") + pItProp->sPhrase ;
          break ;
        // - Comparative -> comparaison
        case NsProposition::circonstancielleComparativeProportion :  // SCCP : Vous chanterez d'autant plus fort que vous serez bien échauffé
          switch (pItProp->iConjonctionType)
          {
            case NsProposition::SCCP_AutantQuantite :    // suivant, selon, à mesure
                pCurrentProp->sPhrase += string(" à mesure que ") + pItProp->sPhrase ;
                break ;
            case NsProposition::SCCP_Synchrone :         // au fur et à mesure
                pCurrentProp->sPhrase += string(" au fur à mesure que ") + pItProp->sPhrase ;
                break ;
            case NsProposition::SCCP_PlusQuantite :      // d'autant plus
                pCurrentProp->sPhrase += string(" d'autant plus que ") + pItProp->sPhrase ;
                break ;
            case NsProposition::SCCP_PlusVite :          // d'autant plus vite
                pCurrentProp->sPhrase += string(" d'autant plus vite que ") + pItProp->sPhrase ;
                break ;
            case NsProposition::SCCP_MoinsQuantite :      // d'autant moins
                pCurrentProp->sPhrase += string(" d'autant moins que ") + pItProp->sPhrase ;
                break ;
            case NsProposition::SCCP_MoinsVite :          // d'autant moins vite
                pCurrentProp->sPhrase += string(" d'autant moins vite que ") + pItProp->sPhrase ;
                break ;
          }
          break ;
        case NsProposition::circonstancielleComparativeEgalite :     // SCCE : Vous chantez aussi fort que moi
          switch (pItProp->iConjonctionType)
          {
            case NsProposition::SCCE_Aspect :            // tel que, de même que, si ... que
              pCurrentProp->sPhrase += string(" tel que ") + pItProp->sPhrase ;
              break ;
            case NsProposition::SCCE_Quantite :          // autant que, aussi que
              pCurrentProp->sPhrase += string(" autant que ") + pItProp->sPhrase ;
              break ;
            case NsProposition::SCCE_Temps :             // tant que
              pCurrentProp->sPhrase += string(" tant que ") + pItProp->sPhrase ;
              break ;
          }
          break ;
        case NsProposition::circonstancielleComparativeRessemblance : // SCCR : Tu me regardes comme me regardait ta mère
          pCurrentProp->sPhrase += string(" comme ") + pItProp->sPhrase ;
          break ;
        // - Conditionnelle -> condition
        case NsProposition::circonstancielleConditionnelleHypothese : // SCHH : il mange s'il a faim, il mangera s'il a faim
          pCurrentProp->sPhrase += string(" si ") + pItProp->sPhrase ;
          break ;
        case NsProposition::circonstancielleConditionnelleImaginee :  // SCHH : il mangerait s'il avait faim
          pCurrentProp->sPhrase += string(" si ") + pItProp->sPhrase ;
          break ;
        default :
          pCurrentProp->sPhrase += string(", ") + pItProp->sPhrase ;
      }
    }
  }
  return true ;
}

// donneLibelleAffiche : sert à faire les bons appels à donneLibelleAffiche de
// NSPathologData.

void
NSGenerateurFr::donneLibelleAffiche(string* pLibelle, NSPathologData* pData, GENRE iGenre)
{
  // En francais, cette methode ne sert que pour les noms et les adjectifs.
  // Ceux-ci peuvent etre MS, MP, FS ou FP.

  int iDeclinaison = 0 ;
  *pLibelle = string("") ;

  if (NULL == pData)
    return ;

  if (pData->estNom())
  {
    if ((genreMP == iGenre) || (genreFP == iGenre))
      pData->donneLibelleAffiche(pLibelle, declinaisonPluriel) ;
    else
      pData->donneLibelleAffiche(pLibelle, declinaisonSingulier) ;

    return ;
  }
  else if (pData->estAdjectif())
  {
    switch (iGenre)
    {
      case genreMP : iDeclinaison = declinaisonMP ; break ;
      case genreFP : iDeclinaison = declinaisonFP ; break ;
      case genreMS : iDeclinaison = declinaisonMS ; break ;
      case genreFS : iDeclinaison = declinaisonFS ; break ;
      default      : iDeclinaison = 0 ;
    }

    pData->donneLibelleAffiche(pLibelle, iDeclinaison) ;
    return ;
  }
  else
    pData->donneLibelleAffiche(pLibelle) ;
}

bool
NSGenerateurFr::prepareVerbe(string* psPhraseAdverbe, string* psAdverbeModificateur, bool* pbPhraseAffirmative, DCODETYPE /* iStyle */)
{
  if ((pPh->Verbe).empty())
    return true ;

  string          sCertitude;
  iterPhraseMot   iterMots;
  NSPathologData  Data;

  string sPhrasePos = string("") ;
  string sPhraseNeg = string("") ;
  string sInterPos1 = string("") ;
  string sInterPos2 = string("") ;
  string sInterNeg1 = string("") ;
  string sInterNeg2 = string("") ;

  if ((*(pPh->Verbe.begin()))->getComplementPhr())
  {
    NSPhraseur* pCompVerbe = (*(pPh->Verbe.begin()))->getComplementPhr() ;

    if (false == (pCompVerbe->adverbe).empty())
    {
      NsProposition Propos(pContexte, &pCompVerbe, NsProposition::notSetType, NsProposition::notSetConjonct, false) ;
      NSGenerateurFr GeneAdverbe(pContexte, &Propos, sLang) ;
      GeneAdverbe.classeTout() ;

      if (GeneAdverbe.pAdverbePos)
      {
        if (false == (GeneAdverbe.pAdverbePos)->empty())
        {
          for (iterMots  = (GeneAdverbe.pAdverbePos)->begin() ;
               iterMots != (GeneAdverbe.pAdverbePos)->end() ;
               iterMots++)
          {
            if (false == (*iterMots)->estTexteLibre())
              sInterPos2 = donneAdverbeComplet(*iterMots) ;
            else
              sInterPos2 = (*iterMots)->getTexteLibre() ;

            etDuMilieu(&sPhrasePos, &sInterPos1, &sInterPos2) ;
          }
          etFinal(&sPhrasePos, &sInterPos1) ;

          *psPhraseAdverbe += sPhrasePos ;
        }
      }

      if (GeneAdverbe.pAdverbeNeg)
      {
        if (false == (GeneAdverbe.pAdverbeNeg)->empty())
        {
          // s'il existe des adverbes à sens positif.
          //
          if (string("") != sPhrasePos)
          {
            for (iterMots  = (GeneAdverbe.pAdverbeNeg)->begin() ;
                 iterMots != (GeneAdverbe.pAdverbeNeg)->end() ;
                 iterMots++)
            {
              if (false == (*iterMots)->estTexteLibre())
                sInterNeg2 = donneAdverbeComplet(*iterMots) ;
              else
                sInterNeg2 = (*iterMots)->getTexteLibre() ;

              etDuMilieu(&sPhraseNeg, &sInterNeg1, &sInterNeg2) ;
            }
            etFinal(&sPhraseNeg, &sInterNeg1) ;

            *psPhraseAdverbe += string(" mais ") + sPhraseNeg ;
          }

          // s'il n'y a pas d'adverbe a sens positif,
          // la phrase devient négative.

          else
          {
            *pbPhraseAffirmative = false ;

            // On prend l'adverbe modificateur du premier adverbe
            // du COD négatif

            bool bPremier = true;
            for (iterMots  = (GeneAdverbe.pAdverbeNeg)->begin() ;
                 iterMots != (GeneAdverbe.pAdverbeNeg)->end() ;
                 iterMots++)
            {
              sInterNeg2 = donneNomComplet(*iterMots, sansCertitude);
              if (strlen((*iterMots)->getCertitude().c_str()) >= 5)
                sCertitude = string((*iterMots)->getCertitude(), 0, 5) ;
              else
                sCertitude = string("") ;

              if (string("") != sCertitude)
              {
                if (bPremier)
                {
                  bPremier = false ;
                  if (string("") != sCertitude)
                  {
                    if (sCertitude == "WCE00")
                      *psAdverbeModificateur = string(" ") ;
                    else if ((sCertitude > "WCE00") &&
                             (sCertitude < "WCE50"))
                      *psAdverbeModificateur = " probablement " ;
                  }
                }
                else
                {
                  if (sCertitude != "")
                  {
                    if (sCertitude == "WCE00")
                      sInterNeg2 = string("pas ") /* + sInter2*/ ;
                    else if ((sCertitude > "WCE00") &&
                             (sCertitude < "WCE50"))
                      sInterNeg2 = string("probablement pas ") /* + sInter2*/ ;
                  }
                }
              }
              etDuMilieu(&sPhraseNeg, &sInterNeg1, &sInterNeg2) ;
            }
            etFinal(&sPhraseNeg, &sInterNeg1) ;

            *psPhraseAdverbe = sPhraseNeg ;
          }
        }
      }
    }
  }
  return true ;
}

bool
NSGenerateurFr::prepareSujet(string* psSujet, GENRE* piGenreSujet, string* psAttSujet, string* psAdverbeModificateur, bool* pbPhraseAffirmative, DCODETYPE /* iStyle */)
{
  bool     trouve     = false ;
  NSSuper* pSuper     = pContexte->getSuperviseur() ;
  string   sLexique   = string("") ;
  string   sPluriel   = string("") ;
  string   sCertitude = string("") ;
  GENRE    iGenre     = genreNull ;

  string   sAdverbeModificateurAtt = string(" ") ;

  iterPhraseMot  iterMots ;
  NSPathologData Data ;

  //
  // Le sujet doit être un nom - Subject must be a noun
  //
  if (false == pPh->Sujet.empty())
  {
    string sInter1 = string("") ;
    string sInter2 = string("") ;

    // On itère sur tous les sujets
    //
    for (iterMots = pPh->Sujet.begin() ; pPh->Sujet.end() != iterMots ; iterMots++)
    {
      sLexique = (*iterMots)->getLexique() ;

      // A propos de textes libres.
      // On donne le genre MS à tous les textes libres, par défaut.
      // On ne peut pas traiter grammaticalement les textes libres.

      if (false == (*iterMots)->estTexteLibre())
      {
        trouve = pContexte->getDico()->trouvePathologData(sLang, &sLexique, &Data) ;
        sPluriel = (*iterMots)->getPluriel() ;
        if (false == trouve)
          return false ;
        if (false == Data.estNom())
          return false ;
        Data.donneGenre(&iGenre) ;

        // Récupération du genre (composition du genre récupéré avec
        // le genre déjà existant)

        if (string("") != sPluriel)
          Data.donneGenrePluriel(&iGenre) ;

        string sForceNbr = (*iterMots)->forceNombre(this) ;

        if      (FORCE_PLUR == sForceNbr)
          Data.donneGenrePluriel(&iGenre) ;
        else if (FORCE_SING == sForceNbr)
          Data.donneGenreSingulier(&iGenre) ;
      }
      else
        iGenre = genreMS ;

      // Premier genre attribué, prendre celui du sujet en cours
      // Attributing a gender for first time, take subject's gender
      //
      if (genreNull == *piGenreSujet)
        *piGenreSujet = iGenre ;

      // Genre déjà attribué (donc déjà pluriel ou passe au pluriel) ; adapter selon celui du sujet en cours
      // Gender already set (hence already plural or switch to plural) ; adapt it from current subject
      //
      else
      {
        if (((genreFS == *piGenreSujet) || (genreFP == *piGenreSujet)) &&
                                                          (Data.estFeminin()))
          *piGenreSujet = genreFP ;
        else
          *piGenreSujet = genreMP ;
      }

      // L'article dépend du statut d'objet réel ou abstrait
      // Pour un texte libre, on va chercher son libellé dans la table
      // des textes libres.

      if (false == (*iterMots)->estTexteLibre())
      {
        if (((sPluriel == "") && (Data.estReel())) || (string(sPluriel, 0, 5) == "WPLUR"))
          sInter2 = donneNomComplet(*iterMots, NSPhraseMot::articleDefini);
        else if (((sPluriel == "") && (!Data.estReel())) || (string(sPluriel, 0, 5) == "WPLUS"))
          sInter2 = donneNomComplet(*iterMots, NSPhraseMot::articleIndefini);
      }
      else
        sInter2 = (*iterMots)->getTexteLibre() ;

      etDuMilieu(psSujet, &sInter1, &sInter2);
    }
    etFinal(psSujet, &sInter1) ;
  }
  // L'attribut du sujet est un adjectif qualificatif qui s'accorde avec le sujet
  //
  // ex : La valve est dilatée
  //
  // L'attribut doit être un adjectif
  //
  // On met d'abord les attributs positifs, puis les négatifs, avec "mais pas" entre.
  // S'il n'y a que des négatifs, la phrase devient négative.
  //
  // ex : La valve n'est pas dilatée.
  // 		La valve est dilatée, mais pas détruite.

  string sAttSujetPos = string("") ;
  if (pAttSujetPos)
  {
    if (false == pAttSujetPos->empty())
    {
      if (pPh->Sujet.empty())
        return false ;

      string sInter1 = string("") ;
      string sInter2 = string("") ;

      for (iterMots = pAttSujetPos->begin() ; pAttSujetPos->end() != iterMots ; iterMots++)
      {
        if (false == (*iterMots)->estTexteLibre())
          // On impose le genre du sujet
          sInter2 = donneAdjectifComplet(*iterMots, *piGenreSujet) ;
        else
          sInter2 = (*iterMots)->getTexteLibre() ;

        etDuMilieu(&sAttSujetPos, &sInter1, &sInter2) ;
      }
      etFinal(&sAttSujetPos, &sInter1) ;
    }
  }

  string sAttSujetNeg = string("") ;
  if (pAttSujetNeg)
  {
    if (false == pAttSujetNeg->empty())
    {
      if (pPh->Sujet.empty())
        return false ;

      string sInter1 = string("") ;
      string sInter2 = string("") ;
      bool bPremier = true ;

      for (iterMots = pAttSujetNeg->begin() ; pAttSujetNeg->end() != iterMots ; iterMots++)
      {
        // On impose le genre du sujet
        sInter2 = donneAdjectifComplet(*iterMots, *piGenreSujet, sansCertitude) ;

        sCertitude = string((*iterMots)->getCertitude(), 0, 5) ;

        if (bPremier)
        {
          bPremier = false ;
          if (string("") != sCertitude)
          {
            if (sCertitude == "WCE00")
              sAdverbeModificateurAtt = string(" ") ;
            else if ((sCertitude > "WCE00") && (sCertitude < "WCE50"))
              sAdverbeModificateurAtt = string(" probablement ") ;
          }
        }
        else
          sInter2 = donneCertitude(sCertitude) + string(" ") + sInter2 ;

        etDuMilieu(&sAttSujetNeg, &sInter1, &sInter2) ;
      }
      etFinal(&sAttSujetNeg, &sInter1) ;
    }
  }

  if (string("") == sAttSujetPos)
  {
    if (string("") != sAttSujetNeg)
    {
      if (*pbPhraseAffirmative)
      {
        *pbPhraseAffirmative   = false ;
        *psAttSujet            = sAttSujetNeg ;
        *psAdverbeModificateur = sAdverbeModificateurAtt ;
      }
      else
      {
        // il y a déjà un adverbe du verbe qui rend la phrase négative.
        *psAttSujet = sAdverbeModificateurAtt + string("pas ") + sAttSujetNeg ;
        // on enlève les blancs initaux.
        strip(*psAttSujet, stripLeft) ;
      }
    }
  }
  else
  {
    *psAttSujet = sAttSujetPos ;
    if (string("") != sAttSujetNeg)
      *psAttSujet += string(" mais") + sAdverbeModificateurAtt + string ("pas ") + sAttSujetNeg ;
  }
  return true ;
}

bool
NSGenerateurFr::prepareCOD(string* psCOD, GENRE* piGenreCOD, string* psAttCOD, string* psAdverbeModificateur, bool* pbPhraseAffirmative, DCODETYPE iStyle)
{
  if ((NULL == psCOD) || (NULL == piGenreCOD) || (NULL == psAttCOD))
    return false ;

  bool     trouve     = false ;
  string   sPluriel   = string("") ;
  string   sCertitude = string("") ;
  GENRE    iGenre     = genreNull ;

  iterPhraseMot  iterMots;
  NSPathologData Data;

  *psCOD      = string("") ;
  *psAttCOD   = string("") ;
  *piGenreCOD = genreNull ;

  // D'abord on va récuperer le genre du COD

  if (false == pPh->COD.empty())
  {
    for (iterMots = pPh->COD.begin() ; pPh->COD.end() != iterMots ; iterMots++)
    {
      string sLexique = (*iterMots)->getLexique() ;

      if (false == (*iterMots)->estTexteLibre())
      {
        sPluriel = (*iterMots)->getPluriel() ;
        trouve = pContexte->getDico()->trouvePathologData(sLang, &sLexique, &Data) ;
        if (false == trouve)
          return false ;
        if (false == Data.estNom())
          return false ;

        // Récupération du genre (composition du genre récupéré avec
        // le genre déjà existant)
        Data.donneGenre(&iGenre) ;
        if (string("") != sPluriel)
          Data.donneGenrePluriel(&iGenre) ;

        string sForceNbr = (*iterMots)->forceNombre(this) ;
        if      (FORCE_PLUR == sForceNbr)
          Data.donneGenrePluriel(&iGenre) ;
        else if (FORCE_SING == sForceNbr)
          Data.donneGenreSingulier(&iGenre) ;
      }
      else
        iGenre = genreMS ;

      if (genreNull == *piGenreCOD)
        *piGenreCOD = iGenre ;
      else
      {
        if (((genreFS == *piGenreCOD) || (genreFP == *piGenreCOD)) &&
                     (Data.estFeminin()))
          *piGenreCOD = genreFP ;
        else
          *piGenreCOD = genreMP ;
      }
    }
  }

  // On traite ensuite successivement les noms du COD à certitude positive, puis ceux
  // à certitude negative.
  // Pour une dcTiret, il ne faut pas mettre d'article dans le COD.

  string sCODPos = string("") ;

  if (pCODPos)
  {
    if (false == pCODPos->empty())
    {
      string sInter1 = string("") ;
      string sInter2 = string("") ;

      for (iterMots = pCODPos->begin() ; pCODPos->end() != iterMots ; iterMots++)
      {
        if (dcPhrase == iStyle)
        {
          if (false == (*iterMots)->estTexteLibre())
          {
            if (sPluriel == "")
              sInter2 = donneNomComplet(*iterMots, (*iterMots)->getArticle()) ;

            else if (string(sPluriel, 0, 5) == "WPLUS")
              sInter2 = donneNomComplet(*iterMots, NSPhraseMot::articleDefini);
            else if (string(sPluriel, 0, 5) == "WPLUR")
              sInter2 = donneNomComplet(*iterMots, NSPhraseMot::articleIndefini);
          }
          else
            sInter2 = (*iterMots)->getTexteLibre() ;
        }

        else if (dcTiret == iStyle)
        {
          if (false == (*iterMots)->estTexteLibre())
            sInter2 = donneNomComplet(*iterMots, NSPhraseMot::articleSans) ;
          else
            sInter2 = (*iterMots)->getTexteLibre() ;
        }

        etDuMilieu(&sCODPos, &sInter1, &sInter2) ;
      }
      etFinal(&sCODPos, &sInter1) ;
      *psCOD += sCODPos ;
    }
  }

  string sCODNeg = string("") ;

  if (pCODNeg)
  {
    if (false == pCODNeg->empty())
    {
      if ((string("") != sCODPos) || (!(*pbPhraseAffirmative)))
      {
        string sInter1 = string("") ;
        string sInter2 = string("") ;

        for (iterMots = pCODNeg->begin(); iterMots != pCODNeg->end(); iterMots++)
        {
          if (dcPhrase == iStyle)
          {
            if (string("") == sPluriel)
              sInter2 = donneNomComplet(*iterMots, (*iterMots)->getArticle()) ;
            else if (string(sPluriel, 0, 5) == "WPLUS")
              sInter2 = donneNomComplet(*iterMots, NSPhraseMot::articleDefini) ;
            else if (string(sPluriel, 0, 5) == "WPLUR")
              sInter2 = donneNomComplet(*iterMots, NSPhraseMot::articleIndefini) ;
          }
          else if (dcTiret == iStyle)
            sInter2 = donneNomComplet(*iterMots, NSPhraseMot::articleSans) ;

          etDuMilieu(&sCODNeg, &sInter1, &sInter2) ;
        }
        etFinal(&sCODNeg, &sInter1) ;
        *psCOD += string(" mais ") + sCODNeg ;
      }
      // Pas de COD à sens affirmatif
      else
      {
        *pbPhraseAffirmative = false ;

        // On prend l'adverbe modificateur du premier nom du COD négatif

        bool bPremier = true;
        string sInter1 = string("") ;
        string sInter2 = string("") ;
        for (iterMots = pCODNeg->begin(); iterMots != pCODNeg->end(); iterMots++)
        {
          if (dcPhrase == iStyle)
          {
            if (string("") == sPluriel)
              sInter2 = donneNomComplet(*iterMots, (*iterMots)->getArticle(), sansCertitude);
            else if (string(sPluriel, 0, 5) == "WPLUS")
              sInter2 = donneNomComplet(*iterMots, NSPhraseMot::articleDefini, sansCertitude);
            else if (string(sPluriel, 0, 5) == "WPLUR")
              sInter2 = donneNomComplet(*iterMots, NSPhraseMot::articleIndefini, sansCertitude);

            sCertitude = string((*iterMots)->getCertitude(), 0, 5);

            if (string("") != sCertitude)
            {
              if (bPremier)
              {
                bPremier = false;
                if (string("") != sCertitude)
                {
                  if (string(sCertitude, 0 ,5) == "WCE00")
                    *psAdverbeModificateur = string(" ") ;
                  else if ((string(sCertitude, 0 ,5) > "WCE00") && (string(sCertitude, 0 ,5) < "WCE50"))
                    *psAdverbeModificateur = string(" probablement ") ;
                }
              }
              else
              {
                if (string("") != sCertitude)
                {
                  if (string(sCertitude, 0 ,5) == "WCE00")
                    sInter2 = string("pas ") + sInter2 ;
                  else if ((string(sCertitude, 0 ,5) > "WCE00") && (string(sCertitude, 0 ,5) < "WCE50"))
                    sInter2 = string("probablement pas ") + sInter2 ;
                }
              }
            }
          }

          else if (dcTiret == iStyle)
            sInter2 = donneNomComplet(*iterMots, NSPhraseMot::articleSans) ;

          etDuMilieu(&sCODNeg, &sInter1, &sInter2) ;
        }
        etFinal(&sCODNeg, &sInter1) ;
        *psCOD = sCODNeg ;
      }
    }
  }

  // Attribut du COD (se place après les adjectifs qualificatifs qualifiant le COD).

  string sAttCODPos = string("") ;

  if (pAttCODPos)
  {
    if (false == pAttCODPos->empty())
    {
      if (pPh->COD.empty())
        return false ;

      string sInter1 = string("") ;
      string sInter2 = string("") ;

      for (iterMots = pAttCODPos->begin() ; pAttCODPos->end() != iterMots ; iterMots++)
      {
        if (false == (*iterMots)->estTexteLibre())
          // On impose le genre du COD
          sInter2 = donneAdjectifComplet(*iterMots, *piGenreCOD);
        else
          sInter2 = (*iterMots)->getTexteLibre() ;

        etDuMilieu(&sAttCODPos, &sInter1, &sInter2);
      }
      etFinal(&sAttCODPos, &sInter1);
    }
  }

  string sAttCODNeg = string("") ;
  if (pAttCODNeg)
  {
    if (false == pAttCODNeg->empty())
    {
      if (pPh->COD.empty())
        return false ;

      string sInter1 = string("") ;
      string sInter2 = string("") ;

      for (iterMots = pAttCODNeg->begin() ; pAttCODNeg->end() != iterMots ; iterMots++)
      {
        // On impose le genre du COD
        sInter2 = donneAdjectifComplet(*iterMots, *piGenreCOD) ;

        etDuMilieu(&sAttCODNeg, &sInter1, &sInter2) ;
      }
      etFinal(&sAttCODNeg, &sInter1);
    }
  }

  if (string("") == sAttCODPos)
  {
    if (string("") != sAttCODNeg)
      *psAttCOD = sAttCODNeg ;
  }
  else
  {
    *psAttCOD = sAttCODPos ;
    if (string("") != sAttCODNeg)
      *psAttCOD += string(" mais ") + sAttCODNeg ;
  }

  return true ;
}

// donneNomComplet : méthode qui sert à fabriquer un nom, accompagné de tous ses
// compléments. On peut lui demander de ne pas gérer la certitude du nom, ce qui
// est utile notamment dans les subordonnées circonstancielles :
//
// ex : Il existe un polype au niveau de la bouche mais pas de la langue.
//
// Dans ce cas, la certitude se place avant la préposition, donc ne doit pas
// être gérée par donneNomComplet.

string
NSGenerateurFr::donneNomComplet(NSPhraseMot* pMot, int iArticle, GERECERTITUDE iGereCertitude)
{
  string sLibel = string("") ;
  NSPathologData Data ;

  iterPhraseMot iterMots ;

  string sCertitude = pMot->getCertitude() ;
  if ((string("") != sCertitude) && (strlen(sCertitude.c_str()) > 5))
    sCertitude = string(sCertitude, 0, 5) ;

  string sLexique = pMot->getLexique() ;
  string sPluriel = pMot->getPluriel() ;
  GENRE iGenre ;
  bool bPluriel = false ;
  string sPrep = string("") ;

  bool trouve = pContexte->getDico()->trouvePathologData(sLang, &sLexique, &Data) ;
  if (false == trouve)
    return string("") ;
  if (false == Data.estNom())
    return string("") ;
  // Récupération du genre (composition du genre récupéré avec
  // le genre déjà existant)
  Data.donneGenre(&iGenre) ;
  if (string("") != sPluriel)
    Data.donneGenrePluriel(&iGenre) ;

  // On regarde si le nombre est forcé.

  string sForceNbr = pMot->forceNombre(this) ;

  if      (FORCE_PLUR == sForceNbr)
    Data.donneGenrePluriel(&iGenre) ;
  else if (FORCE_SING == sForceNbr)
    Data.donneGenreSingulier(&iGenre) ;

  //
  donneLibelleAffiche(&sLibel, &Data, iGenre) ;

  if (pMot->getComplementPhr())
  {
    NsProposition Propos(pContexte, pMot->getComplementPhr(), NsProposition::notSetType, NsProposition::notSetConjonct) ;
    NSGenerateurFr GeneCpl(pContexte, &Propos, sLang) ;
    traitePostposition(pMot) ;

    // Le complément du nom est nécessairement introduit par une préposition.
    // Donc si on n'a pas de préposition, on en met une par défaut : "de".
    if (false == pMot->getComplementPhr()->compNom.empty())
    {
      for (iterMots = pMot->getComplementPhr()->compNom.begin() ;
                 pMot->getComplementPhr()->compNom.end() != iterMots ;
                 iterMots++)
      {
        if (false == (*iterMots)->estTexteLibre())
        {
          if ((*iterMots)->getPreposition() == string(""))
            (*iterMots)->setPreposition("de") ;
        }
        else
        {
          if ((*iterMots)->getPreposition() == string(""))
            (*iterMots)->setTexteLibre(string("de ") + (*iterMots)->getTexteLibre()) ;
        }
      }
    }

    if (GeneCpl.genereNomComplet(pMot, iArticle, iGereCertitude))
      sLibel = GeneCpl.getTempoPhrase() ;
  }
  //
  // Pas de complément
  //
  else
  {
    if (iGereCertitude == avecCertitude)
    {
      string sCertitudeEcrit = donneCertitude(sCertitude) ;

      if (string("") != sCertitudeEcrit)
        sLibel = sCertitudeEcrit + string(" ") + sLibel ;
    }

    // On traite d'abord la préposition
    sPrep = Data.chercheGrammaire(PREPOSITION);

    if (string("") != sPrep)
    {
      if (pMot->getPreposition() != string(""))
        pMot->setPreposition(pMot->getPreposition() + string(" ")) ;
      pMot->setPreposition(pMot->getPreposition() + string(sPrep, 1, strlen(sPrep.c_str()) - 1)) ;
    }

    if ((genreMP == iGenre) || (genreFP == iGenre) || (genreNP == iGenre))
      bPluriel = true ;

    bool modifie = modifiePreposition(pMot, iArticle, bPluriel) ;

    if      (NSPhraseMot::articleDefini == iArticle)
      sLibel = donneArticleDefini(pMot, bPluriel) + sLibel ;
    else if (NSPhraseMot::articleIndefini == iArticle)
    {
      // A la forme négative, l'article indéfini est remplacé par un
      // article partitif
      // ex : il existe un polype -> il n'existe pas de polype
//            if ((iGereCertitude == avecCertitude) && (sCertitude < "WCE50"))
      if ((string("") != sCertitude) && (sCertitude < "WCE50"))
        sLibel = donneArticlePartitif(pMot, bPluriel) + sLibel ;
      else
        sLibel = donneArticleIndefini(pMot, bPluriel) + sLibel ;
    }
    else if (NSPhraseMot::articlePartitif == iArticle)
      sLibel = donneArticlePartitif(pMot, bPluriel) + sLibel ;

    if (modifie)
      sLibel = pMot->getPreposition() + sLibel ;
    else if (pMot->getPreposition() != string(""))
      sLibel = pMot->getPreposition() + string(" ") + sLibel ;
  }

  return sLibel ;
}


// genereNomComplet : méthode qui sert à générerun nom lorsque celui-ci a des compléments.

bool
NSGenerateurFr::genereNomComplet(NSPhraseMot* pMot, int iArticle, GERECERTITUDE iGereCertitude)
{
  sPhrase = string("") ;

  reinitialise() ;

  NSSuper* pSuper = pContexte->getSuperviseur() ;

  bool     bPluriel = false;
  string   sInter1 = "";
  string   sInter2 = "";
  GENRE    iGenreNom;
  string   sCertitude = "";
  string   sCertitudeNom = "";

  NSPathologData Data ;

  NSPhraseur* pComp = pMot->getComplementPhr() ;

  if (false == (pComp->adjEpithete).empty())
    pComp->classeAdjectif(this) ;

  traitePrepositionCC(&pPh->CCLieu,    &pPh->PrepositionLieu,    NSPhraseur::prepLieu) ;
  traitePrepositionCC(&pPh->CCTemps,   &pPh->PrepositionTemps,   NSPhraseur::prepTemps) ;
  traitePrepositionCC(&pPh->CCManiere, &pPh->PrepositionManiere, NSPhraseur::prepManiere) ;
  traitePrepositionCC(&pPh->CCMoyen,   &pPh->PrepositionMoyen,   NSPhraseur::prepMoyen) ;
  traitePrepositionCC(&pPh->CCCause,   &pPh->PrepositionCause,   NSPhraseur::prepCause) ;
  traitePrepositionCC(&pPh->CCBut,     &pPh->PrepositionBut,     NSPhraseur::prepBut) ;
  traitePrepositionCC(&pPh->CCType,    &pPh->PrepositionType,    NSPhraseur::prepType) ;
  traitePrepositionCC(&pPh->CCHypoth,  &pPh->PrepositionHypoth,  NSPhraseur::prepHypoth) ;
  traitePrepositionCC(&pPh->CCChiffre, &pPh->PrepositionChiffre, NSPhraseur::prepChiffre) ;

  classeTout() ;

  //
  // Prise des informations sur le nom à compléter
  //
  string sLexique = pMot->getLexique() ;
  string sPluriel = pMot->getPluriel() ;
  bool trouve = pContexte->getDico()->trouvePathologData(sLang, &sLexique, &Data) ;
  if (false == trouve)
    return false ;
  if (false == Data.estNom())
    return false ;
  //
  // Récupération du genre (composition du genre récupéré avec
  // le genre déjà existant)
  Data.donneGenre(&iGenreNom) ;
  if (sPluriel != "")
    Data.donneGenrePluriel(&iGenreNom) ;

  // On regarde si le nombre est force.

  string sForceNbr = pMot->forceNombre(this) ;

  if      (sForceNbr == FORCE_PLUR)
    Data.donneGenrePluriel(&iGenreNom) ;
  else if (sForceNbr == FORCE_SING)
    Data.donneGenreSingulier(&iGenreNom) ;


  string sNom = string("") ;

  donneLibelleAffiche(&sNom, &Data, iGenreNom) ;

  sCertitudeNom = string(pMot->getCertitude(), 0, 5) ;

  //
  // Compléments : quand, où, comment, avec quoi, pourquoi, dans quel but
  //

  pCCArray->donnePhrase() ;

  // Expansions du groupe nominal
  //
  // Cas 1 Adjectif épithète     le gros garçon
  //       le plus près du nom, dans adjEpithete
  //
  // Cas 2 Complément du nom     le garçon aux yeux bleus
  //       dans les compléments circonstanciels
  //
  // Cas 3 Subordonnée relative  le garçon qui a pris le ballon
  //       suppose qu'il y ait un verbe
  //

  // On traite d'abord les adjectifs épithètes
  //

  iterPhraseMot  iterMotsAdj;

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
    if (false == pAdjEpitheteAvPos->empty())
    {
      for (iterMotsAdj = pAdjEpitheteAvPos->begin();
               iterMotsAdj != pAdjEpitheteAvPos->end();
               iterMotsAdj++)
      {
        // L'epithete s'accorde avec le nom qu'il qualifie.
        if (false == (*iterMotsAdj)->estTexteLibre())
          sInterAvPos2 = donneAdjectifComplet(*iterMotsAdj, iGenreNom) ;
        else
          sInterAvPos2 = (*iterMotsAdj)->getTexteLibre() ;

        etDuMilieu(&sPhraseAvPos, &sInterAvPos1, &sInterAvPos2) ;
      }
      etFinal(&sPhraseAvPos, &sInterAvPos1) ;
    }
  }

  // On traite les adjectifs qui se placent apres le nom.
  // D'abord les affirmatifs, puis les negatifs.
  // Si on a des positifs et des negatifs, on met "mais pas" entre les deux.
  // Si on n'a que des negatifs, on met juste "pas".

  if (pAdjEpitheteApPos)
  {
    if (false == pAdjEpitheteApPos->empty())
    {
      for (iterMotsAdj = pAdjEpitheteApPos->begin();
               iterMotsAdj != pAdjEpitheteApPos->end();
               iterMotsAdj++)
      {
        // L'epithete s'accorde avec le nom qu'il qualifie.
        if (false == (*iterMotsAdj)->estTexteLibre())
          sInterApPos2 = donneAdjectifComplet(*iterMotsAdj, iGenreNom) ;
        else
          sInterApPos2 = (*iterMotsAdj)->getTexteLibre() ;

        etDuMilieu(&sPhraseApPos, &sInterApPos1, &sInterApPos2) ;
      }
      etFinal(&sPhraseApPos, &sInterApPos1) ;
    }
  }

  if (pAdjEpitheteApNeg)
  {
    if (false == pAdjEpitheteApNeg->empty())
    {
      for (iterMotsAdj = pAdjEpitheteApNeg->begin();
               iterMotsAdj != pAdjEpitheteApNeg->end();
               iterMotsAdj++)
      {
        // L'epithete s'accorde avec le nom qu'il qualifie.

        sInterApNeg2 = donneAdjectifComplet(*iterMotsAdj, iGenreNom) ;

        etDuMilieu(&sPhraseApNeg, &sInterApNeg1, &sInterApNeg2) ;
      }
      etFinal(&sPhraseApNeg, &sInterApNeg1) ;
    }
  }

  sPhraseAv = sPhraseAvPos ;
  sPhraseAp = sPhraseApPos ;

  if (string("") != sPhraseApNeg)
  {
    if (string("") != sPhraseApPos)
      sPhraseAp += string(" mais ") + sPhraseApNeg ;
    else
      sPhraseAp = sPhraseApNeg ;
  }

  // On traite ensuite les complements du nom.
  // Considerons pour simplifier qu'il ne s'agit que de noms.
  // "Il y a suspicion d'endocartite."
  // Le complement du nom est le plus souvent introduit par la preposition "de".

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
    if (false == pCompNomPos->empty())
    {
      for (iterMotsCompNom = pCompNomPos->begin();
               iterMotsCompNom != pCompNomPos->end();
               iterMotsCompNom++)
      {
        sLexique = (*iterMotsCompNom)->getLexique() ;

        if (false == (*iterMotsCompNom)->estTexteLibre())
        {
          trouve = pContexte->getDico()->trouvePathologData(sLang, &sLexique, &DataCompNom) ;
          if (false == trouve)
            return false ;

          sCertitude = string((*iterMotsCompNom)->getCertitude(), 0, 5) ;

          sInterCompNomPos2 = donneCertitude(sCertitude) + donneNomComplet(*iterMotsCompNom, (*iterMotsCompNom)->getArticle(), sansCertitude) ;
        }
        else
          sInterCompNomPos2 = (*iterMotsCompNom)->getTexteLibre() ;

        etDuMilieu(&sCompNomPos, &sInterCompNomPos1, &sInterCompNomPos2) ;
      }
      etFinal(&sCompNomPos, &sInterCompNomPos1) ;
    }
  }

  if (pCompNomNeg)
  {
    if (false == pCompNomNeg->empty())
    {
      for (iterMotsCompNom = pCompNomNeg->begin();
               iterMotsCompNom != pCompNomNeg->end();
               iterMotsCompNom++)
      {
        sLexique = (*iterMotsCompNom)->getLexique() ;
        trouve = pContexte->getDico()->trouvePathologData(sLang, &sLexique, &DataCompNom) ;
        if (false == trouve)
          return false ;

        sCertitude = string((*iterMotsCompNom)->getCertitude(), 0, 5) ;
        sInterCompNomNeg2 = donneCertitude(sCertitude) + donneNomComplet(*iterMotsCompNom, (*iterMotsCompNom)->getArticle(), sansCertitude) ;

        etDuMilieu(&sCompNomNeg, &sInterCompNomNeg1, &sInterCompNomNeg2) ;
      }
      etFinal(&sCompNomNeg, &sInterCompNomNeg1) ;
    }
  }

  sCompNom = sCompNomPos ;

  if (string("") != sCompNomNeg)
  {
    if (string("") != sCompNomPos)
      sCompNom += string(" mais ") + sCompNomNeg ;
    else
      sCompNom = sCompNomNeg ;
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
                  sPhrase += string(" mais pas ") + (*itCompl)->sNegatif;
          }
          else if ((*itCompl)->sNegatif != "")
              sPhrase += string(" pas ") + (*itCompl)->sNegatif;
      }
  }

  if ((iGenreNom == genreMP) || (iGenreNom == genreFP) || (iGenreNom == genreNP))
      bPluriel = true;

  // On traite les adjectifs numéraux
  // char NumCard[BASE_COMPLEMENT_LEN + 1];
  // strcpy(NumCard, pComp->adjNumeralCardinal.complement) ;

  if (string("") != pComp->adjNumeralCardinal.getComplement())
    sPhrase = pComp->adjNumeralCardinal.getComplement() + string(" ") + sPhrase ;

  // NSPhraseMot      adjNumeralOrdinal ;

  // On traite d'abord la preposition
  string sPrep = Data.chercheGrammaire(PREPOSITION);

  if (string("") != sPrep)
  {
    if (pMot->getPreposition() != "")
      pMot->setPreposition(pMot->getPreposition() + string(" ")) ;
    pMot->setPreposition(pMot->getPreposition() + string(sPrep, 1, strlen(sPrep.c_str()) - 1)) ;
  }

  bool modifie = modifiePreposition(pMot, iArticle, bPluriel) ;

  // On traite l'article

  // Remarque : en francais, un article ne commence jamais par un "h".

  if      (iArticle == NSPhraseMot::articleDefini)
    sPhrase = donneArticleDefini(pMot, bPluriel, sansCertitude) + sPhrase;
  else if (iArticle == NSPhraseMot::articleIndefini)
  {
    if ((sCertitudeNom != "") && (sCertitudeNom < "WCE50") && (iGereCertitude == avecCertitude))
      sPhrase = donneArticlePartitif(pMot, bPluriel, sansCertitude) + sPhrase ;
    else
      sPhrase = donneArticleIndefini(pMot, bPluriel, sansCertitude) + sPhrase ;
  }
  else if (iArticle == NSPhraseMot::articlePartitif)
    sPhrase = donneArticlePartitif(pMot, bPluriel, sansCertitude) + sPhrase ;

  // Apres l'article, on traite la preposition.

  if (modifie)
    sPhrase = pMot->getPreposition() + sPhrase ;
  else if (pMot->getPreposition() != "")
    sPhrase = pMot->getPreposition() + string(" ") + sPhrase ;

  if (iGereCertitude == avecCertitude)
  {
    sCertitude = string(pMot->getCertitude(), 0, 5) ;
    string sCertitudeEcrit = donneCertitude(sCertitude);

    if (string("") != sCertitudeEcrit)
      sPhrase = sCertitudeEcrit + string(" ") + sPhrase ;
  }

  // Tout a la fin, on met le complement entre parentheses.

  if (pComp->pParentheses)
  {
      NsProposition* pPropos = new NsProposition(pContexte, &(pComp->pParentheses), NsProposition::notSetType, NsProposition::notSetConjonct, false) ;
      NSGenerateurFr* pGeneParentheses = new NSGenerateurFr(pContexte, pPropos, sLang) ;
      pGeneParentheses->genereProposition(dcPhrase) ;
      sPhrase += string(" (") + pGeneParentheses->getPropositionPhrase() + string(")") ;
      delete pGeneParentheses ;
      delete pPropos ;
  }

  // On indique que le nom est complete par une subordonnee relative en donnant
  // à iPhraseType la valeur phraseRelative.
  // Dans Nautilus, on considerera que l'antecedent a toujours valeur de sujet
  // relativement a la subordonnee relative. Celle-ci est donc toujours introduite
  // par le mot de liaison "qui".
  // On met l'antecedent dans l'array Sujet du complement, pour permettre a generePhrase
  // de mettre le verbe a la bonne personne.

  if (pComp->iPhraseType == NSPhraseur::phraseRelative)
  {
    pComp->Sujet.vider() ;

    // Si on ne supprime pas le complément, on tourne en rond
    NSPhraseMot* pMotSujet = new NSPhraseMot(*pMot) ;
    if (pMotSujet->getComplementPhr())
    {
      delete pMotSujet->getComplementPhr() ;
      pMotSujet->setComplementPhr((NSPhraseur*) 0) ;
    }
    pComp->Sujet.push_back(pMotSujet) ;

    // pComp->Sujet.push_back(new NSPhraseMot(*pMot));

    NsProposition Propos(pContexte, &pComp, NsProposition::notSetType, NsProposition::notSetConjonct, false) ;
    NSGenerateurFr GeneRelative(pContexte, &Propos, sLang) ;
    GeneRelative.genereProposition(dcPhrase) ;
    sPhrase += string(" ") + GeneRelative.getPropositionPhrase() ;
  }
  return true;
}


// donneAdjectifComplet : pareil que pour donneNomComplet, sauf que c'est pour
// générer des adjectifs.

string
NSGenerateurFr::donneAdjectifComplet(NSPhraseMot* pMot, GENRE iGenre, GERECERTITUDE iGereCertitude)
{
  if (NULL == pMot)
    return string("") ;

  string sLexique = pMot->getLexique() ;

  NSPathologData DataAdj;
  bool trouve = pContexte->getDico()->trouvePathologData(sLang, &sLexique, &DataAdj) ;
  if (false == trouve)
    return string("") ;

  if (false == DataAdj.estAdjectif())
    return string("") ;

  string sLibelle = "" ;

  donneLibelleAffiche(&sLibelle, &DataAdj, iGenre);

  // L'adjectif peut etre précédé d'adverbes.

  if (pMot->getComplementPhr())
  {
    NSPhraseur* pComp = pMot->getComplementPhr() ;
    NsProposition Propos(pContexte, &pComp, NsProposition::notSetType, NsProposition::notSetConjonct, false) ;
    NSGenerateurFr GeneCpl(pContexte, &Propos, sLang) ;
    if (GeneCpl.genereAdjectifComplet(pMot, iGenre, iGereCertitude))
      sLibelle = GeneCpl.getTempoPhrase() ;
  }
  else
  {
    if (iGereCertitude == avecCertitude)
    {
      string sCertitude = string(pMot->getCertitude(), 0, 5) ;
      string sCertitudeEcrit = donneCertitude(sCertitude) ;

      if (string("") != sCertitudeEcrit)
        sLibelle = sCertitudeEcrit + string(" ") + sLibelle ;
    }
  }
  return sLibelle ;
}

bool
NSGenerateurFr::genereAdjectifComplet(NSPhraseMot* pMot, GENRE iGenre, GERECERTITUDE iGereCertitude)
{
  sPhrase = string("") ;

  reinitialise() ;

  NSPhraseur* pComp  = pMot->getComplementPhr() ;

  NSPathologData Data ;

  classeTout() ;

  //
  // Prise des informations sur l'adverbe à compléter
  //
  string sLexique = pMot->getLexique() ;
  bool trouve = pContexte->getDico()->trouvePathologData(sLang, &sLexique, &Data) ;
  if (false == trouve)
    return false ;
  if (false == Data.estAdjectif())
    return false ;

  string sAdjectif = string("") ;
  donneLibelleAffiche(&sAdjectif, &Data, iGenre) ;
  if (string("") == sAdjectif)
    return false ;

  // L'adjectif peut etre complété par des adverbes.

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
    if (false == pAdverbePos->empty())
    {
      for (iterMotsComp  = pAdverbePos->begin();
               iterMotsComp != pAdverbePos->end();
               iterMotsComp++)
      {
        sLexique = (*iterMotsComp)->getLexique() ;

        if (false == (*iterMotsComp)->estTexteLibre())
        {
          trouve = pContexte->getDico()->trouvePathologData(sLang, &sLexique, &DataComp) ;
          if (false == trouve)
            return false ;

          sInterCompPos2 = donneAdverbeComplet(*iterMotsComp) ;
        }
        else
          sInterCompPos2 = (*iterMotsComp)->getTexteLibre() ;

        etDuMilieu(&sCompPos, &sInterCompPos1, &sInterCompPos2) ;
      }
      etFinal(&sCompPos, &sInterCompPos1) ;
    }
  }

  if (pAdverbeNeg)
  {
    if (false == pAdverbeNeg->empty())
    {
      for (iterMotsComp  = pAdverbeNeg->begin();
               iterMotsComp != pAdverbeNeg->end();
               iterMotsComp++)
      {
        sLexique = (*iterMotsComp)->getLexique() ;
        trouve = pContexte->getDico()->trouvePathologData(sLang, &sLexique, &DataComp);
        if (false == trouve)
          return false ;

        sInterCompNeg2 = donneAdverbeComplet(*iterMotsComp) ;

        etDuMilieu(&sCompNeg, &sInterCompNeg1, &sInterCompNeg2) ;
      }
      etFinal(&sCompNeg, &sInterCompNeg1) ;
    }
  }

  sComp = sCompPos ;

  if (sCompNeg != "")
  {
      if (sCompPos != "")
          sComp += string(" mais ") + sCompNeg;
      else
          sComp = sCompNeg;
  }

  sPhrase = sComp;

  if (sComp != "")
      sPhrase += string(" ");
  sPhrase += sAdjectif;

  // Tout a la fin, on met le complément entre parentheses.

  if (pComp->pParentheses)
  {
    NsProposition Propos(pContexte, &(pComp->pParentheses), NsProposition::notSetType, NsProposition::notSetConjonct, false) ;
    NSGenerateurFr GeneParentheses(pContexte, &Propos, sLang) ;
    GeneParentheses.genereProposition(dcPhrase) ;
    sPhrase += string(" (") + GeneParentheses.getPropositionPhrase() + string(")") ;
  }

  if (iGereCertitude == avecCertitude)
  {
    string sCertitude = string(pMot->getCertitude(), 0, 5) ;
    string sCertitudeEcrit = donneCertitude(sCertitude) ;

    if (string("") != sCertitudeEcrit)
      sPhrase = sCertitudeEcrit + string(" ") + sPhrase ;
  }
  return true ;
}

// donneAdverbeComplet : pareil que pour les noms et les adjectifs, mais pour les adverbes.

string
NSGenerateurFr::donneAdverbeComplet(NSPhraseMot* pMot, GERECERTITUDE iGereCertitude)
{
  string sLibel = string("") ;
  NSPathologData Data ;

  string sCertitude ;

  string sLexique = pMot->getLexique() ;

  bool trouve = pContexte->getDico()->trouvePathologData(sLang, &sLexique, &Data);
  if (false == trouve)
    return string("") ;
  if (false == Data.estAdverbe())
    return string("") ;

  donneLibelleAffiche(&sLibel, &Data) ;

  if (pMot->getComplementPhr())
  {
    NsProposition Propos(pContexte, pMot->getComplementPhr(), NsProposition::notSetType, NsProposition::notSetConjonct) ;
    NSGenerateurFr GeneCpl(pContexte, &Propos, sLang) ;
    if (GeneCpl.genereAdverbeComplet(pMot, iGereCertitude))
      sLibel = GeneCpl.getTempoPhrase() ;
  }

  if (iGereCertitude == avecCertitude)
  {
    sCertitude = string(pMot->getCertitude(), 0, 5) ;
    string sCertitudeEcrit = donneCertitude(sCertitude) ;

    if (sCertitudeEcrit != "")
      sLibel = sCertitudeEcrit + string(" ") + sLibel ;
  }

  return sLibel ;
}

bool
NSGenerateurFr::genereAdverbeComplet(NSPhraseMot* pMot, GERECERTITUDE /* iGereCertitude */)
{
  if (NULL == pMot)
    return false ;

  sPhrase = string("") ;

  reinitialise() ;

  NSPhraseur* pComp = pMot->getComplementPhr() ;

  NSPathologData Data ;

  classeTout() ;

  //
  // Prise des informations sur l'adverbe à compléter
  //
  string sLexique = pMot->getLexique() ;
  bool trouve = pContexte->getDico()->trouvePathologData(sLang, &sLexique, &Data) ;
  if (false == trouve)
    return false ;
  if (false == Data.estAdverbe())
    return false ;

  string sAdverbe = "" ;

  donneLibelleAffiche(&sAdverbe, &Data) ;
  if (string("") == sAdverbe)
    return false ;

  // L'adverbe peut etre complété par des adverbes.

  iterPhraseMot  iterMotsComp ;
  NSPathologData DataComp ;

  string sInterCompPos1 = "" ;
  string sInterCompPos2 = "" ;
  string sCompPos = "" ;

  string sInterCompNeg1 = "" ;
  string sInterCompNeg2 = "" ;
  string sCompNeg = "" ;

  string sComp = "" ;

  if (pAdverbePos)
  {
    if (false == pAdverbePos->empty())
    {
      for (iterMotsComp  = pAdverbePos->begin();
               iterMotsComp != pAdverbePos->end();
               iterMotsComp++)
      {
        sLexique = (*iterMotsComp)->getLexique() ;

        if (false == (*iterMotsComp)->estTexteLibre())
        {
          trouve = pContexte->getDico()->trouvePathologData(sLang, &sLexique, &DataComp) ;
          if (false == trouve)
            return false ;

          sInterCompPos2 = donneAdverbeComplet(*iterMotsComp) ;
        }
        else
          sInterCompPos2 = (*iterMotsComp)->getTexteLibre() ;

        etDuMilieu(&sCompPos, &sInterCompPos1, &sInterCompPos2) ;
      }
      etFinal(&sCompPos, &sInterCompPos1) ;
    }
  }

  if (pAdverbeNeg)
  {
    if (false == pAdverbeNeg->empty())
    {
      for (iterMotsComp  = pAdverbeNeg->begin();
               iterMotsComp != pAdverbeNeg->end();
               iterMotsComp++)
      {
        sLexique = (*iterMotsComp)->getLexique() ;
        trouve = pContexte->getDico()->trouvePathologData(sLang, &sLexique, &DataComp) ;
        if (!trouve)
          return false ;

        sInterCompNeg2 = donneAdverbeComplet(*iterMotsComp) ;

        etDuMilieu(&sCompNeg, &sInterCompNeg1, &sInterCompNeg2) ;
      }
      etFinal(&sCompNeg, &sInterCompNeg1) ;
    }
  }

  sComp = sCompPos;

  if (string("") != sCompNeg)
  {
    if (string("") != sCompPos)
      sComp += string(" mais ") + sCompNeg ;
    else
      sComp = sCompNeg ;
  }

  sPhrase = sComp ;

  if (string("") != sComp)
    sPhrase += string(" ") ;
  sPhrase += sAdverbe ;

  // Tout a la fin, on met le complement entre parentheses.

  if (pComp->pParentheses)
  {
    NsProposition Propos(pContexte, &(pComp->pParentheses), NsProposition::notSetType, NsProposition::notSetConjonct, false) ;
    NSGenerateurFr GeneParentheses(pContexte, &Propos, sLang) ;
    GeneParentheses.genereProposition(dcPhrase) ;
    sPhrase += string(" (") + GeneParentheses.getPropositionPhrase() + string(")") ;
  }

  return true ;
}


// donneCertitude : méthode qui donne le libellé de la certitude, et éventuellement
// qui indique si le libellé de la certitude commence par une voyelle ou un h muet.

string
NSGenerateurFr::donneCertitude(string sCertitude, bool* pCommenceParVoyelleOuH)
{
    // pCommenceParVoyelleOuH sert au cas ou la certitude peut imposer une
    // apostrophe. On ne le modifie pas au cas ou la ceritude

    if (sCertitude == "")
        return "";

    if (sCertitude == "WCE00")
    {
        if (pCommenceParVoyelleOuH)
            *pCommenceParVoyelleOuH = false;
        return string("pas");
    }
    else if ((sCertitude > "WCE00") && (sCertitude < "WCE50"))
    {
        if (pCommenceParVoyelleOuH)
            *pCommenceParVoyelleOuH = false;
        return string("probablement pas");
    }
    else if (string(sCertitude, 0 ,5) == "WCE50")
    {
        if (pCommenceParVoyelleOuH)
            *pCommenceParVoyelleOuH = false;
        return string("peut-être");
    }
    else if ((string(sCertitude, 0 ,5) > "WCE50") && (string(sCertitude, 0 ,5) < "WCE90"))
    {
        if (pCommenceParVoyelleOuH)
            *pCommenceParVoyelleOuH = false;
        return string("probablement");
    }
    else if ((string(sCertitude, 0 ,5) >= "WCE90") && (string(sCertitude, 0 ,5) < "WCEA0"))
    {
        if (pCommenceParVoyelleOuH)
            *pCommenceParVoyelleOuH = false;
        return string("typiquement");
    }
    else
        return "";
}

// donnePhraseComplement : generation des complements circonstanciels.

string
NSGenerateurFr::donnePhraseComplement(NSPhraseMot* pLiaison,
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
    for (iterMots = pCC->begin(); iterMots != pCC->end(); iterMots++)
    {
      sInter2 = "" ;

      // Est-ce une donnée chiffrée ?
      string sFormat = (*iterMots)->getFormat() ;

      // Est-ce une structure de type NSPhraseMotTime ?
      // Is it a NSPhraseMotTime object
      NSPhraseMotTime* pDate = dynamic_cast<NSPhraseMotTime*>(*iterMots) ;
      if ( pDate )
      {
        bool bSucces ;
        string sTimeLibel = decodeTime(pDate, &bSucces) ;
        if (bSucces)
          sInter2 = sTimeLibel ;
      }
      else
      {
        NSPhraseMotTimeCycle* pCycle = dynamic_cast<NSPhraseMotTimeCycle*>(*iterMots) ;
        if ( pCycle )
        {
          bool bSucces ;
          string sCycleLibel = decodeCycle(pCycle, &bSucces) ;
          if (bSucces)
            sInter2 = sCycleLibel ;
        }
        //
        // Ni valeur chiffrée, ni date
        // Not a number, nor a date
        //
        else if (string("") == sFormat)
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
                sInter2 = donneNomComplet(*iterMots, (*iterMots)->getArticle(), sansCertitude) ;
              else if (string(sPluriel, 0, 5) == "WPLUR")
                sInter2 = donneNomComplet(*iterMots, NSPhraseMot::articleDefini, sansCertitude) ;
              else if (string(sPluriel, 0, 5) == "WPLUS")
                sInter2 = donneNomComplet(*iterMots, NSPhraseMot::articleIndefini, sansCertitude) ;
            }
            else
              sInter2 = donneNomComplet(*iterMots, NSPhraseMot::articleSans, sansCertitude) ;

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
          // Date "simple"
          // "Simple" date
          //
          if      ((sFormat[0] == '£') &&
                       ((sFormat[1] == dateMARK) || (sFormat[1] == dateHeureMARK)))
          {
            string sComplement = (*iterMots)->getComplement() ;
            string sUnite      = (*iterMots)->getUnite() ;

            gereDateFr Date(pContexte->getSuperviseur(), sLang);
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
      }
      etDuMilieu(&sPhr, &sInter1, &sInter2) ;
    }
    etFinal(&sPhr, &sInter1) ;

    // Traitement de la préposition globale
    sLexique = pPreposition->getLexique() ;
    if (string("") != sLexique)
    {
      sPluriel = pPreposition->getPluriel() ;
      trouve = pContexte->getDico()->trouvePathologData(sLang, &sLexique, &Data) ;
      if (false == trouve)
        return string("") ;
      Data.donneGenre(&iGenre) ;
      if (sPluriel != "")
        Data.donneGenrePluriel(&iGenre) ;
      donneLibelleAffiche(&sInter2, &Data, iGenre) ;
      sPhr = sInter2 + " " + sPhr ;
    }
    // Traitement de l'élément de liaison
    sLexique = pLiaison->getLexique() ;
    if (string("") != sLexique)
    {
      sPluriel = pLiaison->getPluriel() ;
      trouve = pContexte->getDico()->trouvePathologData(sLang, &sLexique, &Data) ;
      if (false == trouve)
        return string("") ;
      if (Data.estVerbe())
      {
        sInter2 = donneParticipePresent(&Data) ;
        sPhr = sInter2 + " " + sPhr ;
      }
      else
      {
        Data.donneGenre(&iGenre) ;
        if (sPluriel != "")
          Data.donneGenrePluriel(&iGenre) ;
        donneLibelleAffiche(&sInter2, &Data, iGenre) ;
        sPhr = sInter2 + " " + sPhr ;
      }
    }
  }
  *bSucces = true ;
  return sPhr ;
}

string
NSGenerateurFr::donnePhraseChiffree(NSPhraseMot* pLiaison,
                                    NSPhraseMot* pPreposition,
                                    NSPhraseMotArray* pCC,
                                    bool* bSucces)
{
  string sPhr = string("") ;

  if (pCC->empty())
    return sPhr ;

  string sInter1 = "" ;
  string sInter2 = "" ;
  string sPhraseSimple = "" ;

  string sRelat1 = "" ;
  string sRelat2 = "" ;
  string sPhraseRelative = "" ;
  string sVerbe          = "" ;
  string sVerbePluriel   = "" ;

  NSSuper* pSuper = pContexte->getSuperviseur() ;
  string   sPluriel;
  GENRE    iGenre;

  for (iterPhraseMot iterMots = pCC->begin() ; pCC->end() != iterMots ; iterMots++)
  {
    string sLexique = (*iterMots)->getLexique() ;

    string sMesure = "" ;
    GENRE  iGenreMesure ;
    bool   bPluriel ;

    string sPrepOption = "" ;

    NSPathologData Data ;

    if (string("") != sLexique)
    {
      bool trouve = pContexte->getDico()->trouvePathologData(sLang, &sLexique, &Data) ;
      if (false == trouve)
      {
        *bSucces = false ;
        return string("") ;
      }
      // Récupération du genre (composition du genre récupéré avec
      // le genre déjà existant)
      Data.donneGenre(&iGenreMesure) ;
      if (string("") != sPluriel)
        Data.donneGenrePluriel(&iGenreMesure) ;

      if ((string("") != sPluriel) || (Data.estPluriel()))
        bPluriel = true ;
      else
        bPluriel = false ;
      //
      donneLibelleAffiche(&sMesure, &Data, iGenreMesure) ;
    }

    // Traitement du complement, en fonction de son format
    //
    string sComplement = (*iterMots)->getComplement() ;

    string sUnite  = (*iterMots)->getUnite() ;
    string sFormat = (*iterMots)->getFormat() ;

    string sValeur = string("") ;
    //
    // Valeur numérique
    //
    if (sFormat[0] == '£')
    {
      if (nbMARK == sFormat[1])
      {
        gereNum Num(pContexte->getSuperviseur(), sLang) ;
        Num.instancier(&sComplement, &sUnite, &sFormat) ;
        bool bSucces ;
        sValeur = decodeNum(&Num, &bSucces) ;

        // On traite le cas ou la valeur chiffree est du type :
        // "dans 3 jours", "il y a 3 jours", "pendant 3 jours",
        // "il y a 2 ans pendant 3 jours"...

        if (sLexique == "KDATE1")
        {
          // Cas ou il s'agit d'un evenement passe ponctuel :
          // "il y a 3 jours".
          sValeur = string("il y a ") + sValeur;
          sVerbe        = "était";
          sVerbePluriel = "étaient";
        }
        else if (sLexique == "KDATD1")
        {
          // Cas ou il s'agit d'un evenement futur ponctuel :
          // "dans 3 jours".
          sValeur = string("dans ") + sValeur;
          sVerbe        = "sera";
          sVerbePluriel = "seront";
        }
        else if (string("") != sLexique)
        {
          if (Data.estNom())
            sPrepOption = "de";
          sVerbe        = "est";
          sVerbePluriel = "sont";
        }
      }
      //
      // Date
      //
      else if ((sFormat[1] == dateMARK) || (sFormat[1] == dateHeureMARK))
      {
        string sMessage = "" ;
        string sIntro = "" ;

        gereDateFr Date(pContexte->getSuperviseur(), sLang);
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

    string sMethode      = (*iterMots)->getMethode() ;
    string sLibelMethode = string("") ;

    if (string("") != sMethode)
    {
      bool trouve = pContexte->getDico()->trouvePathologData(sLang, &sMethode, &Data) ;
      if (false == trouve)
      {
        *bSucces = false ;
        return string("") ;
      }
      // Récupération du genre (composition du genre récupéré avec
      // le genre déjà existant)
      Data.donneGenre(&iGenre);
      if (sPluriel != "")
        Data.donneGenrePluriel(&iGenre) ;
      //
      donneLibelleAffiche(&sLibelMethode, &Data, iGenre) ;
    }

    //
    // Cas simple : une lésion d'une longueur de 3 mm
    //
    if ((*(iterMots))->getNumForme() == NSPhraseMot::numSimple)
    {
      if (sMesure != "")
      {
        sInter2 = string("de ") +
                                donneArticleIndefini(*iterMots, bPluriel) +
                                sMesure;
      }
      if (sPrepOption != "")
        sInter2 += string(" ") + sPrepOption;
      if (sInter2 != "")
        sInter2 += " ";
      sInter2 += sValeur;
      if (sLibelMethode != "")
        sInter2 += " (" + sLibelMethode + ")";
    }
    //
    // Cas relative : une lésion dont la longueur est de 3 mm
    //
    else if ((*(iterMots))->getNumForme() == NSPhraseMot::numRelative)
    {
      sRelat2 = donneArticleDefini(*iterMots, bPluriel) + sMesure;
      if (bPluriel)
        sRelat2 += string(" ") + sVerbePluriel;
      else
        sRelat2 += string(" ") + sVerbe;
      if (sPrepOption != "")
        sRelat2 += string(" ") + sPrepOption;
      sRelat2 += " ";
      sRelat2 += sValeur;
      if (sLibelMethode != "")
        sRelat2 += " (" + sLibelMethode + ")";
    }
    //
    // Cas tirets : longueur 3 mm
    //
    else
    {
      sInter2 = sMesure;
      if (sInter2 != "")
        sInter2 += " ";
      sInter2 += sValeur;
      if (sLibelMethode != "")
        sInter2 += " (" + sLibelMethode + ")";
    }

    etDuMilieu(&sPhraseSimple,   &sInter1, &sInter2);
    etDuMilieu(&sPhraseRelative, &sRelat1, &sRelat2);
  }
  etFinal(&sPhraseSimple,   &sInter1);
  etFinal(&sPhraseRelative, &sRelat1);

  sPhr = sPhraseSimple;
  if (string("") != sPhraseRelative)
  {
    if (string("") != sPhr)
      sPhr += string(" ") ;

    // Eviter "La sténose est peu serrée : dont le pic de vitesse est de..."
    //
    if (NsProposition::deuxPoints != pProposition->iPropositionType)
      sPhr += string("dont ") ;

    sPhr += sPhraseRelative ;
  }

  // Traitement de la préposition
  string sLexProp = pPreposition->getLexique() ;
  if (string("") != sLexProp)
  {
    sPluriel = pPreposition->getPluriel() ;
    NSPathologData DataProp ;
    bool bFound = pContexte->getDico()->trouvePathologData(sLang, &sLexProp, &DataProp) ;
    if (false == bFound)
      return string("") ;
    DataProp.donneGenre(&iGenre) ;
    if (string("") != sPluriel)
      DataProp.donneGenrePluriel(&iGenre) ;
    donneLibelleAffiche(&sInter2, &DataProp, iGenre) ;
    sPhr = sInter2 + string(" ") + sPhr ;
  }
  else if (string("") != pPreposition->getPreposition())
    sPhr = pPreposition->getPreposition() + " " + sPhr ;

  // Traitement de l'élément de liaison
  string sLexLink = pLiaison->getLexique() ;
  if (string("") != sLexLink)
  {
    sPluriel = pLiaison->getPluriel() ;
    NSPathologData DataLink ;
    bool bFoundLink = pContexte->getDico()->trouvePathologData(sLang, &sLexLink, &DataLink) ;
    if (false == bFoundLink)
      return string("") ;
    if (DataLink.estVerbe())
    {
      sInter2 = donneParticipePresent(&DataLink);
      sPhr = sInter2 + string(" ") + sPhr ;
    }
    else
    {
      DataLink.donneGenre(&iGenre) ;
      if (string("") != sPluriel)
        DataLink.donneGenrePluriel(&iGenre) ;
      donneLibelleAffiche(&sInter2, &DataLink, iGenre) ;
      sPhr = sInter2 + string(" ") + sPhr ;
    }
  }
  return sPhr ;
}

// donnePremierAdj : donne le premier des adjectifs qui completent un mot.
// ne le donne pas s'il s'agit d'un texte libre.

bool
NSGenerateurFr::donnePremierAdj(NSPhraseur* pCompAdj, NSPhraseMot* pMotAdj)
{
    NSSuper* pSuper = pContexte->getSuperviseur();
    NSPathologData PremierAdjData;
    NSPhraseMot PremierAdj(pContexte);
    bool trouve;
    string sLexiqueAdj = "";

    if (pCompAdj)
    {
        NsProposition* pPropos = new NsProposition(pContexte, &pCompAdj, NsProposition::notSetType, NsProposition::notSetConjonct, false) ;
        NSGenerateurFr* pGeneCompl = new NSGenerateurFr(pContexte, pPropos, sLang) ;
        pCompAdj->classeAdjectif(this) ;
        pGeneCompl->classeTout() ;

        if (pGeneCompl->pAdjEpitheteAvPos)
        {
            if (!((pGeneCompl->pAdjEpitheteAvPos)->empty()))
            {
                PremierAdj = *(*((pGeneCompl->pAdjEpitheteAvPos)->begin()));
                sLexiqueAdj = PremierAdj.getLexique() ;

                // Si jamais le premier adjectif est un texte libre, on l'ignore,
                // vu qu'on est incapable de le traiter grammaticalement.
                if (PremierAdj.estTexteLibre())
                    return false;

                trouve = pContexte->getDico()->trouvePathologData(sLang, &sLexiqueAdj, &PremierAdjData);
                if (!trouve)
                    return false;
                if (pMotAdj)
                    *pMotAdj = PremierAdj;
                return true;
            }
        }
        else if (pGeneCompl->pAdjEpitheteAvNeg)
            if (!((pGeneCompl->pAdjEpitheteAvNeg)->empty()))
            {
                PremierAdj = *(*((pGeneCompl->pAdjEpitheteAvNeg)->begin()));
                sLexiqueAdj = PremierAdj.getLexique() ;

                if (PremierAdj.estTexteLibre())
                    return false;

                trouve = pContexte->getDico()->trouvePathologData(sLang, &sLexiqueAdj, &PremierAdjData);
                if (!trouve)
                    return false;
                if (pMotAdj)
                    *pMotAdj = PremierAdj;
                return true;
            }

        delete pGeneCompl ;
        delete pPropos ;
    }
    return false;
}

// donnePremierAdj : donne le premier adverbe qui complète un mot.
// Ne renvoie rien s'il s'agit d'un texte libre.

bool
NSGenerateurFr::donnePremierAdv(NSPhraseur* pComp, NSPhraseMot* pMotAdv)
{
    NSSuper* pSuper = pContexte->getSuperviseur();
    NSPathologData PremierAdvData;
    NSPhraseMot PremierAdv(pContexte);
    NSPhraseMot PremierAdvRec(pContexte);
    bool trouve;
    string sLexiqueAdv = "";

    if (pComp)
    {
        NsProposition* pPropos = new NsProposition(pContexte, &pComp, NsProposition::notSetType, NsProposition::notSetConjonct, false) ;
        NSGenerateurFr* pGeneCompl = new NSGenerateurFr(pContexte, pPropos, sLang) ;
        pComp->classeAdjectif(this) ;
        pGeneCompl->classeTout() ;

        if (pGeneCompl->pAdverbePos)
        {
            if (!((pGeneCompl->pAdverbePos)->empty()))
            {
                PremierAdv = *(*((pGeneCompl->pAdverbePos)->begin()));
                sLexiqueAdv = PremierAdv.getLexique() ;

                // Si jamais le premier adverbe est un texte libre, on l'ignore,
                // vu qu'on est incapable de le traiter grammaticalement.
                if (PremierAdv.estTexteLibre())
                    return false;

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

                if (PremierAdv.estTexteLibre())
                    return false;

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

// decodeNum : pour décoder les valeurs chiffrées.

string
NSGenerateurFr::decodeNum(gereNum* pNum, bool* bSucces)
{
  if ((NULL == pNum) || (NULL == bSucces))
    return string("") ;

  string sLibelle = string("") ;
  *bSucces = true ;

  if ((pNum->estExact()) || (pNum->estInf()) || (pNum->estSup()))
  {
    bool bPluriel = false ;

    if (pNum->estExact())
    {
      sLibelle = pNum->getNum("") ;
      if (pNum->getValeur() > 1)
        bPluriel = true ;
    }
    else
    {
      if ((pNum->estInf()) && (pNum->estSup()))
        sLibelle = "entre " + pNum->getNumInf() + " et " + pNum->getNumSup() ;
      else if (pNum->estInf())
        sLibelle = "plus de " + pNum->getNumInf() ;
      else if (pNum->estSup())
        sLibelle = "moins de " + pNum->getNumSup() ;

      if ((pNum->getValeurInf() > 1) || (pNum->getValeurSup() > 1))
        bPluriel = true ;
    }
    //
    // Libellé de l'unité de l'unité
    //
    if (string("") != sLibelle)
    {
      string sUniteLibel = pNum->donneLibelleUnite(pContexte) ;

      if (string("") != sUniteLibel)
        sLibelle += string(" ") + sUniteLibel ;
    }
  }
  return sLibelle ;
}

string
NSGenerateurFr::decodeTime(NSPhraseMotTime* pTime, bool* bSucces)
{
    string sInter2 ;

    string sComplement ;
    string sUnite ;
    string sFormat ;

    NSSuper* pSuper = pContexte->getSuperviseur() ;

    if (pTime->valeurDuree[0] != '\0')
    {
        sComplement = string(pTime->valeurDuree) ;
        sUnite      = string(pTime->uniteDuree) ;
        sFormat     = string(pTime->formatDuree) ;

        gereNum numDuree(pContexte->getSuperviseur(), sLang) ;
        numDuree.instancier(&sComplement, &sUnite, &sFormat) ;
        bool bSucces ;
        string sDurationValue = decodeNum(&numDuree, &bSucces) ;

        if (bSucces)
            sInter2 += string(" pendant ") + sDurationValue ;
    }

    if (pTime->valeurMin[0] != '\0')
    {
        sComplement = string(pTime->valeurMin);
        sUnite      = string(pTime->uniteMin);
        sFormat     = string(pTime->formatMin);

        gereDateFr DateMin(pContexte->getSuperviseur(), sLang);
        DateMin.setDate(&sComplement);
        DateMin.setFormat(&sFormat);

        string sUniteSens = "";
        pContexte->getDico()->donneCodeSens(&sUnite, &sUniteSens);
        DateMin.setUnite(&sUniteSens);

        string sLibelMin, sIntroMin;

        // Min = max : date ou heure pile
        if (pTime->minEgalMax())
        {
            DateMin.donne_date_claire(&sLibelMin, &sIntroMin, gereDateFr::DateNormal) ;
            sInter2 += sIntroMin + sLibelMin ;
        }
        else if (pTime->valeurMax[0] != '\0')
        {
            DateMin.donne_date_claire(&sLibelMin, &sIntroMin, gereDateFr::DateDeb) ;

            sComplement = string(pTime->valeurMax) ;
            sUnite      = string(pTime->uniteMax) ;
            sFormat     = string(pTime->formatMax) ;

            gereDateFr DateMax(pContexte->getSuperviseur(), sLang) ;
            DateMax.setDate(&sComplement) ;
            DateMax.setFormat(&sFormat) ;

            string sUniteSens = "" ;
            pContexte->getDico()->donneCodeSens(&sUnite, &sUniteSens) ;
            DateMax.setUnite(&sUniteSens) ;

            string sLibelMax, sIntroMax ;

            DateMax.donne_date_claire(&sLibelMax, &sIntroMax, gereDateFr::DateFin) ;

            sInter2 += sIntroMin + sLibelMin + string(" ") + sIntroMax + sLibelMax ;
        }
        else if (pTime->bMaxNow)
        {
            DateMin.donne_date_claire(&sLibelMin, &sIntroMin, gereDateFr::DateDeb) ;
            sInter2 += " depuis" + sIntroMin + sLibelMin ;
        }
        else
        {
            DateMin.donne_date_claire(&sLibelMin, &sIntroMin) ;
            sInter2 += string("à partir de ") + sIntroMin + sLibelMin ;
        }
    }
    else if (pTime->valeurMax[0] != '\0')
    {
        sComplement = string(pTime->valeurMax) ;
        sUnite      = string(pTime->uniteMax) ;
        sFormat     = string(pTime->formatMax) ;

        gereDateFr DateMax(pContexte->getSuperviseur(), sLang) ;
        DateMax.setDate(&sComplement) ;
        DateMax.setFormat(&sFormat) ;

        string sUniteSens = "" ;
        pContexte->getDico()->donneCodeSens(&sUnite, &sUniteSens) ;
        DateMax.setUnite(&sUniteSens) ;

        string sLibelMax, sIntroMax ;

        if (pTime->bMinNow)
        {
            DateMax.donne_date_claire(&sLibelMax, &sIntroMax, gereDateFr::DateFin) ;
            sInter2 += " jusqu'" + sIntroMax + sLibelMax ;
        }
        else
        {
            DateMax.donne_date_claire(&sLibelMax, &sIntroMax) ;
            sInter2 += sIntroMax + sLibelMax ;
        }
    }
    *bSucces = true ;
    return sInter2 ;
}

string
NSGenerateurFr::decodeCycle(NSPhraseMotTimeCycle* pCycle, bool* bSucces)
{
    string sInter2 = "" ;

    string sAComplement = string(pCycle->actionDurationValue) ;
    string sAUnite      = string(pCycle->actionDurationUnit) ;
    string sAFormat     = string(pCycle->actionDurationFormat) ;

    gereNum numAction(pContexte->getSuperviseur(), sLang) ;
    numAction.instancier(&sAComplement, &sAUnite, &sAFormat) ;

    string sCComplement = string(pCycle->cycleDurationValue) ;
    string sCUnite      = string(pCycle->cycleDurationUnit) ;
    string sCFormat     = string(pCycle->cycleDurationFormat) ;

    gereNum numDuree(pContexte->getSuperviseur(), sLang) ;
    numDuree.instancier(&sCComplement, &sCUnite, &sCFormat) ;

    //
    // Cycle régulier - Regular cycle
    //
    if (pCycle->iCycleType == NSPhraseMotTimeCycle::CycRegular)
    {
        // Même unité de temps - Same time unit
        //
        if (strcmp(pCycle->cycleDurationUnit, pCycle->actionDurationUnit) == 0)
        {
            if (pCycle->actionDurationValue[0] != '\0')
            {
                bool bGood ;
                string sActionDurationValue = decodeNum(&numAction, &bGood) ;

                if (bGood)
                {
                    sInter2 += sActionDurationValue ;

                    if (pCycle->cycleDurationValue[0] != '\0')
                    {
                        string sCycleLength = numDuree.getNum() ;
                        sInter2 += string(" sur ") + sCycleLength ;
                    }
                    *bSucces = true ;
                    return sInter2 ;
                }
            }
        }
        //
        // Unités de temps différentes - Distinct time unit
        //
        else
        {
            if (pCycle->actionDurationValue[0] != '\0')
            {
                bool bGood ;
                string sActionDurationValue = decodeNum(&numAction, &bGood) ;

                if (bGood)
                {
                    sInter2 += sActionDurationValue ;

                    if (pCycle->cycleDurationValue[0] != '\0')
                    {
                        if ((numDuree.estExact()) && (numDuree.getValeur() == 1))
                        {
                            string sCycUnitLib = numDuree.donneLibelleUnite(pContexte) ;
                            sInter2 += string(" par ") + sCycUnitLib ;
                        }
                        else
                        {
                            string sCycleLength = decodeNum(&numDuree, &bGood) ;

                            NSPathologData* pData = new NSPathologData;
                            NSSuper*        pSuper = pContexte->getSuperviseur();
                            bool trouve = pContexte->getDico()->trouvePathologData(sLang, &sCUnite, pData);
                            if (trouve)
                            {
                                GENRE iGenre ;
                                pData->donneGenre(&iGenre);
                                if ((genreFS == iGenre) || (genreFP == iGenre))
                                    sInter2 += string(" toutes les ") ;
                                else
                                    sInter2 += string(" tous les ") ;
                            }
                            else
                                sInter2 += string(" tous les ") ;

                            sInter2 += sCycleLength ;
                        }
                    }
                    *bSucces = true ;
                    return sInter2 ;
                }
            }
        }
    }
    //
    // Cycle libre - Free cycle
    //
    if (pCycle->iCycleType == NSPhraseMotTimeCycle::CycFree)
    {
        // Même unité de temps - Same time unit
        //
        if ((strcmp(pCycle->cycleDurationUnit, pCycle->actionDurationUnit) == 0) &&
            (pCycle->numberOfAction[0] != '\0'))
        {
            if (pCycle->actionDurationValue[0] != '\0')
            {
                bool bGood ;
                string sActionDurationValue = decodeNum(&numAction, &bGood) ;

                if (bGood)
                {
                    sInter2 += sActionDurationValue ;

                    if (pCycle->cycleDurationValue[0] != '\0')
                    {
                        string sCycleLength = numDuree.getNum() ;
                        sInter2 += string(" sur ") + sCycleLength ;
                    }
                    *bSucces = true ;
                    return sInter2 ;
                }
            }
        }
        //
        // Unités de temps différentes - Distinct time unit
        //
        else
        {
            if (pCycle->numberOfAction[0] != '\0')
                sInter2 += string(pCycle->numberOfAction) + string(" fois ") ;

            // Durée d'action non nulle
            //
            if (pCycle->actionDurationValue[0] != '\0')
            {
                bool bGood ;
                string sActionDurationValue = decodeNum(&numAction, &bGood) ;

                if (bGood)
                {
                    sInter2 += sActionDurationValue ;

                    if (pCycle->cycleDurationValue[0] != '\0')
                    {
                        if ((numDuree.estExact()) && (numDuree.getValeur() == 1))
                        {
                            string sCycUnitLib = numDuree.donneLibelleUnite(pContexte) ;
                            sInter2 += string(" par ") + sCycUnitLib ;
                        }
                        else
                        {
                            string sCycleLength = decodeNum(&numDuree, &bGood) ;

                            NSPathologData* pData = new NSPathologData;
                            NSSuper*        pSuper = pContexte->getSuperviseur();
                            bool trouve = pContexte->getDico()->trouvePathologData(sLang, &sCUnite, pData);
                            if (trouve)
                            {
                                GENRE iGenre ;
                                pData->donneGenre(&iGenre) ;
                                if ((iGenre == genreFS) || (iGenre == genreFP))
                                    sInter2 += string(" toutes les ") ;
                                else
                                    sInter2 += string(" tous les ") ;
                            }
                            else
                                sInter2 += string(" tous les ") ;

                            sInter2 += sCycleLength ;
                        }
                    }
                    *bSucces = true ;
                    return sInter2 ;
                }
            }
            // Durée d'action nulle
            else if (pCycle->cycleDurationValue[0] != '\0')
            {
                if ((numDuree.estExact()) && (numDuree.getValeur() == 1))
                {
                    string sCycUnitLib = numDuree.donneLibelleUnite(pContexte) ;
                    sInter2 += string(" par ") + sCycUnitLib ;
                }
                else
                {
                    bool bGood;
                    string sCycleLength = decodeNum(&numDuree, &bGood) ;
                    /*
                    NSPathologData* pData = new NSPathologData;
                    NSSuper*        pSuper = pContexte->getSuperviseur();
                    bool trouve = pContexte->getDico()->trouvePathologData(sLang, &sCUnite, pData);
                    if (trouve)
                    {
                        int iGenre;
                        pData->donneGenre(&iGenre);
                        if ((iGenre == genreFS) || (iGenre == genreFP))
                            sInter2 += string(" toutes les ") ;
                        else
                            sInter2 += string(" tous les ") ;
                    }
                    else
                        sInter2 += string(" tous les ") ;
                    */
                    sInter2 += string(" par intervalle de ") ;
                    sInter2 += sCycleLength ;
                }
                *bSucces = true ;
                return sInter2 ;
            }
        }
    }

    *bSucces = false ;
    return sInter2 ;
}

string
NSGenerateurFr::donneArticleIndefini(NSPhraseMot* pMot, bool bPluriel, GERECERTITUDE /* iGereCertitude */)
{
  NSPathologData Data ;

  string sLexique = pMot->getLexique() ;
  NSSuper* pSuper = pContexte->getSuperviseur() ;

  bool trouve = pContexte->getDico()->trouvePathologData(sLang, &sLexique, &Data);
  if (false == trouve)
    return string("") ;

  if (false == Data.estNom())
    return string("") ;

  if (bPluriel)
    return string("des ") ;

  GENRE  iGenreNom ;
  string sLibelle = string("") ;
  Data.donneGenre(&iGenreNom) ;
  donneLibelleAffiche(&sLibelle, &Data, iGenreNom) ;

  if (Data.estMasculin())
    return string("un ") ;
  if (Data.estFeminin())
    return string("une ");

  return string("") ;
}

string
NSGenerateurFr::donneArticleDefini(NSPhraseMot* pMot, bool bPluriel, GERECERTITUDE iGereCertitude)
{
  if (NULL == pMot)
    return string("") ;

  NSSuper* pSuper = pContexte->getSuperviseur() ;

  string sLexique = pMot->getLexique() ;
  NSPathologData Data ;

  bool trouve = pContexte->getDico()->trouvePathologData(sLang, &sLexique, &Data) ;
  if (false == trouve)
    return string("") ;

  if (false == Data.estNom())
    return string("") ;

  if (bPluriel)
    return string("les ") ;

  GENRE iGenreNom ;
  string sLibelle = string("") ;
  Data.donneGenre(&iGenreNom) ;

  donneLibelleAffiche(&sLibelle, &Data, iGenreNom) ;

  bool bApostrophe = false ;

  // On commence par regarder si le nom commence par une voyelle ou par un h muet.
  //
  if (CommenceParVoyelle(&sLibelle))
    bApostrophe = true ;

  if (('h' == sLibelle[0]) || ('H' == sLibelle[0]))
  {
    if (Data.chercheGrammaire(VALEUR_DE_H) == H_MUET)
      bApostrophe = true ;
  }



    NSPathologData DataAdj;
    NSPathologData DataAdv;

    string sCertitudeNom = "";
    string sCertitudeAdv = "";

    sCertitudeNom = string(pMot->getCertitude(), 0, 5) ;


  if (pMot->getComplementPhr())
  {
    NSPhraseMot MotAdj(pContexte) ;

    bool bPremierAdjExiste = donnePremierAdj(pMot->getComplementPhr(), &MotAdj) ;
    string sLexiqueAdj = MotAdj.getLexique() ;

    if (bPremierAdjExiste)
    {
      string sCertitudeAdj = string(MotAdj.getCertitude(), 0, 5) ;

      if (string("") != sCertitudeAdj)
      {
        // Le premier adjectif est precede d'une certitude, donc on ne
        // s'occupe pas de la premiere lettre de l'adjectif.

        donneCertitude(sCertitudeAdj, &bApostrophe) ;
      }
      else if (MotAdj.getComplementPhr())
      {
        NSPhraseMot MotAdv(pContexte) ;

        bool bPremierAdvExiste = donnePremierAdv(MotAdj.getComplementPhr(), &MotAdv) ;
        string sLexiqueAdv = MotAdv.getLexique() ;

        if (bPremierAdvExiste)
        {
          string sCertitudeAdv = string(MotAdv.getCertitude(), 0, 5) ;

          if (string("") != sCertitudeAdv)
          {
            // Le premier adverbe est precede d'une certitude, donc on ne
            // s'occupe pas de la premiere lettre de l'adverbe.

            donneCertitude(sCertitudeAdv, &bApostrophe) ;
          }
          else
          {
            // On regarde si l'adverbe commence par une voyelle ou par un
            // h muet.
            string sLibelleAdv = string("") ;
            donneLibelleAffiche(&sLibelleAdv, &DataAdv) ;

            if (CommenceParVoyelle(&sLibelleAdv))
              bApostrophe = true ;

            else if (('h' == sLibelleAdv[0]) || ('H' == sLibelleAdv[0]))
            {
              if (DataAdv.chercheGrammaire(VALEUR_DE_H) == H_MUET)
                bApostrophe = true ;
            }
          }
        }
        else
        {
          // On regarde si l'adjectif commence par une voyelle ou par un
          // h muet.
          string sLibelleAdj = string("") ;
          donneLibelleAffiche(&sLibelleAdj, &DataAdj, iGenreNom) ;

          if (CommenceParVoyelle(&sLibelleAdj))
            bApostrophe = true ;

          else if (('h' == sLibelleAdj[0]) || ('H' == sLibelleAdj[0]))
          {
            if (DataAdj.chercheGrammaire(VALEUR_DE_H) == H_MUET)
              bApostrophe = true ;
          }
        }
      }
    }
  }

  // Si on gere la certitude du nom, celle-ci prend le dessus sur le reste.

  if (avecCertitude == iGereCertitude)
    donneCertitude(sCertitudeNom, &bApostrophe) ;

  if (bApostrophe)
    return string("l'") ;

  if (Data.estMasculin())
    return string("le ") ;
  if (Data.estFeminin())
    return string("la ") ;

  return string("") ;
}

string
NSGenerateurFr::donneArticlePartitif(NSPhraseMot* pMot, bool bPluriel, GERECERTITUDE iGereCertitude)
{
    bool bApostrophe = false;

    NSPhraseMot    MotAdj(pContexte);
    NSPhraseMot    MotAdv(pContexte);
    NSPathologData DataAdj;
    NSPathologData DataAdv;
    NSPathologData Data;

    string sLexiqueAdj = "";
    string sLexiqueAdv = "";

    string sLexique = pMot->getLexique() ;
    NSSuper* pSuper = pContexte->getSuperviseur() ;

    bool trouve = pContexte->getDico()->trouvePathologData(sLang, &sLexique, &Data) ;
    if (!trouve)
        return "";

    string sCertitudeNom = "";
    string sCertitudeAdj = "";
    string sCertitudeAdv = "";

    sCertitudeNom = string(pMot->getCertitude(), 0, 5) ;

    if (!(Data.estNom()))
        return "";

    if (bPluriel)
        return string("des ");

    GENRE  iGenreNom;
    string sLibelle ="";

    Data.donneGenre(&iGenreNom);
    donneLibelleAffiche(&sLibelle, &Data, iGenreNom);

    // On commence par regarder si le nom commence par une voyelle ou par un h muet.

    if (CommenceParVoyelle(&sLibelle))
        bApostrophe = true;


    if ((sLibelle[0] == 'h') || (sLibelle[0] == 'H'))
    {
        if (Data.chercheGrammaire(VALEUR_DE_H) == H_MUET)
            bApostrophe = true;
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

                donneCertitude(sCertitudeAdj, &bApostrophe);
            }

            else if (MotAdj.getComplementPhr())
            {
                bool bPremierAdvExiste = donnePremierAdv(MotAdj.getComplementPhr(), &MotAdv) ;
                sLexiqueAdv = MotAdv.getLexique() ;

                if (bPremierAdvExiste)
                {
                    sCertitudeAdv = string(MotAdv.getCertitude(), 0, 5) ;

                    if (sCertitudeAdv != "")
                    {
                        // Le premier adverbe est precede d'une certitude, donc on ne
                        // s'occupe pas de la premiere lettre de l'adverbe.

                        donneCertitude(sCertitudeAdv, &bApostrophe);
                    }
                    else
                    {
                        // On regarde si l'adverbe commence par une voyelle ou par un
                        // h muet.
                        string sLibelleAdv = "";
                        donneLibelleAffiche(&sLibelleAdv, &DataAdv);

                        if (CommenceParVoyelle(&sLibelleAdv))
                            bApostrophe = true;

                        else if ((sLibelleAdv[0] == 'h') || (sLibelleAdv[0] == 'H'))
                        {
                            if (DataAdv.chercheGrammaire(VALEUR_DE_H) == H_MUET)
                            bApostrophe = true;
                        }
                    }
                }
                else
                {
                    // On regarde si l'adjectif commence par une voyelle ou par un
                    // h muet.
                    string sLibelleAdj = "";
                    donneLibelleAffiche(&sLibelleAdj, &DataAdj, iGenreNom);

                    if (CommenceParVoyelle(&sLibelleAdj))
                        bApostrophe = true;

                    else if ((sLibelleAdj[0] == 'h') || (sLibelleAdj[0] == 'H'))
                    {
                        if (DataAdj.chercheGrammaire(VALEUR_DE_H) == H_MUET)
                            bApostrophe = true;
                    }
                }
            }
        }
    }

    if ((sCertitudeNom != "") && (sCertitudeNom < "WCE050"))
    {
        if (bApostrophe)
            return string("d'");
        else
            return string("de ");
    }

    // Si on gere la certitude du nom, celle-ci prend le dessus sur le reste.

    if (iGereCertitude == avecCertitude)
        donneCertitude(sCertitudeNom, &bApostrophe);

    if (bApostrophe)
        return string("de l'");

    if (Data.estMasculin())
        return string("du ");
    if (Data.estFeminin())
        return string("de la ");
    return "";
}

string
NSGenerateurFr::donnePronomPersonnel(GENRE iGenre, NSPhraseur::VBPERSO iVbPersonne, string sFonction)
{
  //
  // Pronom personnel en tant que sujet de la phrase
  //
  if (sFonction == STR_SUJET)
  {
    switch (iVbPersonne)
    {
      case NSPhraseur::pers1S : return string("je") ;
      case NSPhraseur::pers2S : return string("tu") ;
      case NSPhraseur::pers3S :
        switch (iGenre)
        {
          case genreMS : return string("il") ;
          case genreFS : return string("elle") ;
          case genreNS : return string("on") ;
          default      : return string("") ;
        }
      case NSPhraseur::pers1P : return string("nous") ;
      case NSPhraseur::pers2P : return string("vous") ;
      case NSPhraseur::pers3P :
        switch (iGenre)
        {
          case genreMP : return string("ils") ;
          case genreFP : return string("elles") ;
          default      : return string("") ;
        }
    }
    return string("") ;
  }
  //
  // Pronom personnel en tant que COD de la phrase
  //
  if (sFonction == STR_COD)
  {
    switch (iVbPersonne)
    {
      case NSPhraseur::pers1S : return string("me") ;
      case NSPhraseur::pers2S : return string("te") ;
      case NSPhraseur::pers3S :
        switch (iGenre)
        {
          case genreMS : return string("le") ;
          case genreFS : return string("la") ;
          case genreNS : return string("en") ; // en , y
          default      : return string("") ;
        }
      case NSPhraseur::pers1P : return string("nous") ;
      case NSPhraseur::pers2P : return string("vous") ;
      case NSPhraseur::pers3P : return string("les") ;
    }
    return string("") ;
  }
  //
  // Pronom personnel en tant que COI de la phrase
  //
  if (sFonction == STR_COI)
  {
    switch (iVbPersonne)
    {
      case NSPhraseur::pers1S : return string("me") ;
      case NSPhraseur::pers2S : return string("te") ;
      case NSPhraseur::pers3S :
        switch (iGenre)
        {
          case genreMS : return string("lui") ;
          case genreFS : return string("lui") ;
          case genreNS : return string("en") ; // en , y
          default      : return string("") ;
        }
      case NSPhraseur::pers1P : return string("nous") ;
      case NSPhraseur::pers2P : return string("vous") ;
      case NSPhraseur::pers3P : return string("leur") ;
    }
    return string("") ;
  }
  //
  // Pronom personnel en tant que CC de lieu de la phrase
  //
  if (sFonction == STR_CCLIEU)
  {
    switch (iVbPersonne)
    {
      case NSPhraseur::pers3S : return string("en") ; // en , y
      case NSPhraseur::pers3P : return string("en") ; // en , y
    }
    return string("") ;
  }
  return string("") ;
}

void
NSGenerateurFr::etFinal(string *type, string *type1)
{
	if (*type1 == "")
		return;
	if (*type != "")
    {
        *type += " et ";
    }
	*type += *type1;
	return;
}

string
NSGenerateurFr::donneParticipePresent(NSPathologData* pPathoData)
{
  string sVb = pPathoData->getLabel() ;
  string sVerbe = string("") ;

  NSSuper* pSuper = pContexte->getSuperviseur();
  string sLibelle ;
  if (pContexte->getDico()->donneLibelle(sLang, &sVb, &sLibelle) == false)
    return string("") ;

  // On recherche le type de conjugaison du verbe (son numero de Bescherell)
  // On ne peut pas le faire avec donneverbeclasse, vu qu'on n'a pas de pPhraseMot,
  // On le fait donc "a la main" avec chercheGrammaire.

  string sConjugaison = pPathoData->chercheGrammaire("B") ;

    if (sConjugaison == "")
        return "";

    // Verbes du 1er groupe
    // Il y a les B08 et les B14 (verbes en GER -> GEANT) et les autres.

    if ((sConjugaison > "B05") && (sConjugaison < "B19"))
    {
        sVerbe = string(sLibelle, 0, strlen(sLibelle.c_str())-2);

        if ((sConjugaison == "B08") && (sConjugaison == "B14"))
            sVerbe += string("e");
        sVerbe += string("ant");
    }

    // Verbes du 2e groupe (haïr est a part)

    if (sConjugaison == "B19")
    {
        sVerbe = string(sLibelle, 0, strlen(sLibelle.c_str())-2);
        sVerbe += string("issant");
    }

    if (sConjugaison == "B20")
    {
        sVerbe = string(sLibelle, 0, strlen(sLibelle.c_str())-2);
        sVerbe += string("ïssant");
    }
    return sVerbe ;
}

string
NSGenerateurFr::donneParticipePasse(NSPhraseMot* pPhraseMot, GENRE iGenre)
{
    string sVb = pPhraseMot->getLexique() ;
    string sVerbe = "" ;

    string sTransitivite, sConjugaison, sActEtat ;
    if (donneVerbeClasse(pPhraseMot, &sTransitivite, &sConjugaison, &sActEtat) == false)
        return "";

    NSSuper* pSuper = pContexte->getSuperviseur();
    string sLibelle;
    if (pContexte->getDico()->donneLibelle(sLang, &sVb, &sLibelle) == false)
        return "";

    // Verbes du 1er groupe
    if ((sConjugaison > "B05") && (sConjugaison < "B19"))
    {
        sVerbe = string(sLibelle, 0, strlen(sLibelle.c_str())-2) + string("é");
        if ((iGenre == genreFP) || (iGenre == genreFS))
        	sVerbe += string("e");
        if ((iGenre == genreFP) || (iGenre == genreMP))
        	sVerbe += string("s");
    }

    // Verbes du 2e groupe
    if ((sConjugaison > "B18") && (sConjugaison < "B21"))
    {
        sVerbe = string(sLibelle, 0, strlen(sLibelle.c_str())-1);
        if ((iGenre == genreFP) || (iGenre == genreFS))
        	sVerbe += string("e");
        if ((iGenre == genreFP) || (iGenre == genreMP))
        	sVerbe += string("s");
    }

    return sVerbe;
}


// mapTempsVerbe : passage du système "général" de classification des temps au
// système francais.

void
NSGenerateurFr::mapTempsVerbe()
{
  //
  // Passage du systeme "international" au systeme francais :
  // Temps : tempsPresent, tempsPasse, tempsFutur
  // Aspect : aspectMoment, aspectAccompli, aspectProspectif
  //

  switch (pPh->iVbAspect)
  {
    case NSPhraseur::aspectPonctuel :
    {
      switch (pPh->iVbTemps)
      {
        case NSPhraseur::tempsPasseRevolu :
        case NSPhraseur::tempsPasseActuel :
        {
            iTempsVerbe  = tempsPasseFr ;
            iAspectVerbe = aspectMoment ;
            break ;
        }
        case NSPhraseur::tempsPresent :
        {
            iTempsVerbe = tempsPresentFr ;
            iAspectVerbe = aspectMoment ;
            break ;
        }
        case NSPhraseur::tempsFuturProche :
        case NSPhraseur::tempsFuturLointain :
        {
            iTempsVerbe = tempsFuturFr ;
            iAspectVerbe = aspectMoment ;
            break ;
        }
        default : return ;
      }
    }
    case NSPhraseur::aspectIntervalle :
    {
      switch (pPh->iVbTemps)
      {
        case NSPhraseur::tempsPasseRevolu :
        case NSPhraseur::tempsPasseActuel :
        {
          iTempsVerbe  = tempsPasseFr ;
          iAspectVerbe = aspectAccompli ;
          break ;
        }
        case NSPhraseur::tempsPresent :
        {
          iTempsVerbe = tempsPresentFr ;
          iAspectVerbe = aspectMoment ;
          break ;
        }
        case NSPhraseur::tempsFuturProche :
        case NSPhraseur::tempsFuturLointain :
        {
          iTempsVerbe = tempsFuturFr ;
          iAspectVerbe = aspectAccompli ;
          break ;
        }
        default : return ;
      }
    }
    default : return ;
  }
}


void
NSGenerateurFr::donneVerbe(string* principal, string* auxilliaire)
{
  if ((NULL == principal) || (NULL == auxilliaire))
    return ;

  *principal   = string("") ;
  *auxilliaire = string("") ;

  if (pPh->Verbe.empty())
    return ;

  string sVb = (*(pPh->Verbe.begin()))->getLexique() ;
  string sVerbe;
  NSSuper* pSuper = pContexte->getSuperviseur();
  pContexte->getDico()->donneCodeSens(&sVb, &sVerbe);

  //
  // Verbe être
  //
  if (sVb == "4ETRE1")
  {
    switch (iTempsVerbe)
    {
      case tempsPasseFr   :
      {
        switch (iAspectVerbe)
        {
          case aspectAccompli :
          {
            switch (pPh->iVbPersonne)
            {
              case NSPhraseur::pers1S : *auxilliaire = "ai"    ; *principal = "été" ; return ;
              case NSPhraseur::pers2S : *auxilliaire = "as"    ; *principal = "été" ; return ;
              case NSPhraseur::pers3S : *auxilliaire = "a"     ; *principal = "été" ; return ;
              case NSPhraseur::pers1P : *auxilliaire = "avons" ; *principal = "été" ; return ;
              case NSPhraseur::pers2P : *auxilliaire = "avez"  ; *principal = "été" ; return ;
              case NSPhraseur::pers3P : *auxilliaire = "ont"   ; *principal = "été" ; return ;
            }
          }
          case aspectMoment :
          {
            switch (pPh->iVbPersonne)
            {
              case NSPhraseur::pers1S : *principal = "étais" ;   return ;
              case NSPhraseur::pers2S : *principal = "étais" ;   return ;
              case NSPhraseur::pers3S : *principal = "était" ;   return ;
              case NSPhraseur::pers1P : *principal = "étions" ;  return ;
              case NSPhraseur::pers2P : *principal = "étiez" ;   return ;
              case NSPhraseur::pers3P : *principal = "étaient" ; return ;
            }
          }
        }
        break;
      }
      case tempsPresentFr :
      {
        switch (pPh->iVbPersonne)
        {
          case NSPhraseur::pers1S : *principal = "suis" ;   return ;
          case NSPhraseur::pers2S : *principal = "es" ;     return ;
          case NSPhraseur::pers3S : *principal = "est" ;    return ;
          case NSPhraseur::pers1P : *principal = "sommes" ; return ;
          case NSPhraseur::pers2P : *principal = "êtes" ;   return ;
          case NSPhraseur::pers3P : *principal = "sont" ;   return ;
        }
        break ;
      }
      case tempsFuturFr   :
      {
        switch (pPh->iVbPersonne)
        {
          case NSPhraseur::pers1S : *principal = "serai" ;  return ;
          case NSPhraseur::pers2S : *principal = "seras" ;  return ;
          case NSPhraseur::pers3S : *principal = "sera" ;   return ;
          case NSPhraseur::pers1P : *principal = "serons" ; return ;
          case NSPhraseur::pers2P : *principal = "serez" ;  return ;
          case NSPhraseur::pers3P : *principal = "seront" ; return ;
        }
        break ;
      }
    }
  }
  //
  // Verbe être
  //
  else if (sVb == "4AVOI1")
  {
      switch (iTempsVerbe)
      {
          case tempsPasseFr   :
          {
              switch (iAspectVerbe)
              {
                  case aspectAccompli :
                  {
                      switch (pPh->iVbPersonne)
                      {
                          case NSPhraseur::pers1S : *auxilliaire = "ai"    ; *principal = "eu";   return;
                          case NSPhraseur::pers2S : *auxilliaire = "as"    ; *principal = "eu";   return;
                          case NSPhraseur::pers3S : *auxilliaire = "a"     ; *principal = "eu";   return;
                          case NSPhraseur::pers1P : *auxilliaire = "avons" ; *principal = "eu";  return;
                          case NSPhraseur::pers2P : *auxilliaire = "avez"  ; *principal = "eu";   return;
                          case NSPhraseur::pers3P : *auxilliaire = "ont"   ; *principal = "eu"; return;
                      }
                  }
                  case aspectMoment :
                  {
                      switch (pPh->iVbPersonne)
                      {
                          case NSPhraseur::pers1S : *principal = "avais";   return;
                          case NSPhraseur::pers2S : *principal = "avais";   return;
                          case NSPhraseur::pers3S : *principal = "avait";   return;
                          case NSPhraseur::pers1P : *principal = "avions";  return;
                          case NSPhraseur::pers2P : *principal = "aviez";   return;
                          case NSPhraseur::pers3P : *principal = "avaient"; return;
                      }
                  }
              }
              break;
          }
          case tempsPresentFr :
          {
              switch (pPh->iVbPersonne)
              {
                  case NSPhraseur::pers1S : *principal = "ai";   return;
                  case NSPhraseur::pers2S : *principal = "as";     return;
                  case NSPhraseur::pers3S : *principal = "a";    return;
                  case NSPhraseur::pers1P : *principal = "avons"; return;
                  case NSPhraseur::pers2P : *principal = "avez";   return;
                  case NSPhraseur::pers3P : *principal = "ont";   return;
              }
              break;
          }
          case tempsFuturFr   :
          {
              switch (pPh->iVbPersonne)
              {
                  case NSPhraseur::pers1S : *principal = "aurai";  return;
                  case NSPhraseur::pers2S : *principal = "auras";  return;
                  case NSPhraseur::pers3S : *principal = "aura";   return;
                  case NSPhraseur::pers1P : *principal = "aurons"; return;
                  case NSPhraseur::pers2P : *principal = "aurez";  return;
                  case NSPhraseur::pers3P : *principal = "auront"; return;
              }
              break;
          }
      }
  }
  //
  // Recherche du groupe
  //
  // Le verbe est du type : étudier {|T/B15/A}
  // T   pour Transitif
  // B15 pour groupe 15 du bescherell
  //
  string sTransitivite ;
  string sConjugaison ;
  string sActEtat ;

  if (false == donneVerbeClasse(*(pPh->Verbe.begin()), &sTransitivite, &sConjugaison, &sActEtat))
    return ;

  string sLibelle ;
  if (pContexte->getDico()->donneLibelle(sLang, &sVb, &sLibelle) == false)
    return ;

  // Verbes standards du 1er groupe
  if (sConjugaison == "B06")
  {
    string sRadical = string(sLibelle, 0, strlen(sLibelle.c_str())-2) ;
    *principal = sRadical + donneVerbeTerminaison(1, "") ;
  }
  // Verbes en "ier" rien de particulier sauf iions et iiez à l'imparfait
  else if (sConjugaison == "B15")
  {
    string sRadical = string(sLibelle, 0, strlen(sLibelle.c_str())-2) ;
    *principal = sRadical + donneVerbeTerminaison(1, "") ;
  }
  // Verbes en "uire" rien de particulier sauf un s au passé et au
  //                  présent pluriel : il induisait - nous induisons
  else if (sConjugaison == "B82")
  {
    string sRadical = string(sLibelle, 0, strlen(sLibelle.c_str())-2) ;
    if      (iTempsVerbe == tempsPasseFr)
      *principal = sRadical + string("s") + donneVerbeTerminaison(3, "") ;
    else if ((iTempsVerbe == tempsPresentFr) &&
             ((pPh->iVbPersonne == NSPhraseur::pers1P) ||
              (pPh->iVbPersonne == NSPhraseur::pers2P) ||
              (pPh->iVbPersonne == NSPhraseur::pers3P)))
      *principal = sRadical + string("s") + donneVerbeTerminaison(3, "") ;
    else
      *principal = sRadical + donneVerbeTerminaison(3, "") ;
  }
}

string
NSGenerateurFr::donneVerbeTerminaison(int groupe, string ssgroupe)
{
    // ssgroupe : 2 : finale "ont" : sont, ont, font, vont
    //            3 : (je, tu) peux, veux, vaux
    //            4 : verbes en "dre" sauf ceux en "indre" et "soudre"
    //            5 : assaillir, couvrir, cueillir, défaillir, offrir, ouvrir,
    //                souffrir, tressaillir

    switch (groupe)
    {
        // Verbes du 1er groupe
        //
        case 1 :
            switch (iTempsVerbe)
            {
                case tempsPasseFr   :
                    switch (pPh->iVbPersonne)
                    {
                        case NSPhraseur::pers1S : return string("ais");
                        case NSPhraseur::pers2S : return string("ais");
                        case NSPhraseur::pers3S : return string("ait");
                        case NSPhraseur::pers1P : return string("ions");
                        case NSPhraseur::pers2P : return string("iez");
                        case NSPhraseur::pers3P : return string("aient");
                    }
                    break;
                case tempsPresentFr :
                    switch (pPh->iVbPersonne)
                    {
                        case NSPhraseur::pers1S : return string("e");
                        case NSPhraseur::pers2S : return string("es");
                        case NSPhraseur::pers3S : return string("e");
                        case NSPhraseur::pers1P : return string("ons");
                        case NSPhraseur::pers2P : return string("ez");
                        case NSPhraseur::pers3P : return string("ent");
                    }
                    break;
                case tempsFuturFr   :
                    switch (pPh->iVbPersonne)
                    {
                        case NSPhraseur::pers1S : return string("erai");
                        case NSPhraseur::pers2S : return string("eras");
                        case NSPhraseur::pers3S : return string("era");
                        case NSPhraseur::pers1P : return string("erons");
                        case NSPhraseur::pers2P : return string("erez");
                        case NSPhraseur::pers3P : return string("eront");
                    }
                    break;
            }
            break;
        //
        // Verbe du 2eme groupe
        //
        case 2 :
            switch (iTempsVerbe)
            {
                case tempsPasseFr   :
                    switch (pPh->iVbPersonne)
                    {
                        case NSPhraseur::pers1S : return string("issais");
                        case NSPhraseur::pers2S : return string("issais");
                        case NSPhraseur::pers3S : return string("issait");
                        case NSPhraseur::pers1P : return string("issions");
                        case NSPhraseur::pers2P : return string("issiez");
                        case NSPhraseur::pers3P : return string("issaient");
                    }
                    break;
                case tempsPresentFr :
                    switch (pPh->iVbPersonne)
                    {
                        case NSPhraseur::pers1S : return string("is");
                        case NSPhraseur::pers2S : return string("is");
                        case NSPhraseur::pers3S : return string("it");
                        case NSPhraseur::pers1P : return string("issons");
                        case NSPhraseur::pers2P : return string("issez");
                        case NSPhraseur::pers3P : return string("issent");
                    }
                    break;
                case tempsFuturFr   :
                    switch (pPh->iVbPersonne)
                    {
                        case NSPhraseur::pers1S : return string("irai");
                        case NSPhraseur::pers2S : return string("iras");
                        case NSPhraseur::pers3S : return string("ira");
                        case NSPhraseur::pers1P : return string("irons");
                        case NSPhraseur::pers2P : return string("irez");
                        case NSPhraseur::pers3P : return string("iront");
                    }
                    break;
            }
            break;
        //
        // Verbe du 3eme groupe
        //
        case 3 :
            switch (iTempsVerbe)
            {
                case tempsPasseFr   :
                    switch (pPh->iVbPersonne)
                    {
                        case NSPhraseur::pers1S : return string("ais");
                        case NSPhraseur::pers2S : return string("ais");
                        case NSPhraseur::pers3S : return string("ait");
                        case NSPhraseur::pers1P : return string("ions");
                        case NSPhraseur::pers2P : return string("iez");
                        case NSPhraseur::pers3P : return string("aient");
                    }
                    break;
                case tempsPresentFr :
                    switch (pPh->iVbPersonne)
                    {
                        case NSPhraseur::pers1S :
                            if (ssgroupe == "3")
                                return string("x");
                            if (ssgroupe == "5")
                                return string("e");
                            return string("s");
                        case NSPhraseur::pers2S :
                            if (ssgroupe == "3")
                                return string("x");
                            if (ssgroupe == "5")
                                return string("es");
                            return string("s");
                        case NSPhraseur::pers3S :
                            if (ssgroupe == "4")
                                return string("d");
                            if (ssgroupe == "5")
                                return string("e");
                            return string("t");
                        case NSPhraseur::pers1P : return string("ons");
                        case NSPhraseur::pers2P : return string("ez");
                        case NSPhraseur::pers3P :
                            if (ssgroupe == "2")
                                return string("nt");
                            return string("ent");
                    }
                    break;
                case tempsFuturFr   :
                    switch (pPh->iVbPersonne)
                    {
                        case NSPhraseur::pers1S : return string("rai");
                        case NSPhraseur::pers2S : return string("ras");
                        case NSPhraseur::pers3S : return string("ra");
                        case NSPhraseur::pers1P : return string("rons");
                        case NSPhraseur::pers2P : return string("rez");
                        case NSPhraseur::pers3P : return string("ront");
                    }
                    break;
            }
            break;
    }
    return string("");
}

bool
NSGenerateurFr::donneVerbeClasse(NSPhraseMot* /* pPhraseMot */, string* psTransitivite, string* psConjugaison, string* psActEtat)
{
  // En français, le verbe est du type : étudier {|T/B15/A}
  // T   pour Transitif, I intransitif
  // B15 pour groupe 15 du bescherell
  // A   pour verbe d'Action,
  // E   verbe d'état (ER réel "être", EA apparent "sembler", ED dure "reste"
  //                   EC change "devenir")

  NSPathologData Data;

  *psTransitivite = "";
  *psConjugaison  = "";
  *psActEtat      = "";

  string sTransitif = "";
  string sIntransitif = "";
  string sEtat = "";
  string sAction = "";

	string sLexique = (*(pPh->Verbe.begin()))->getLexique() ;
  NSSuper* pSuper = pContexte->getSuperviseur() ;

  bool trouve = pContexte->getDico()->trouvePathologData(sLang, &sLexique, &Data) ;
  if (!trouve)
    return false ;

  if (!(Data.estVerbe()))
    return false ;

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

    *psConjugaison = Data.chercheGrammaire("B");

    if (*psConjugaison == "")
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
        {
            pPh->iVbType = NSPhraseur::vbTypeEtat;
            if (strlen(psActEtat->c_str()) > 1)
            {
                if      (((*psActEtat)[1] == 'R') || ((*psActEtat)[1] == 'r'))
                    pPh->iVbEtat = NSPhraseur::vbEtatReel;
                else if (((*psActEtat)[1] == 'A') || ((*psActEtat)[1] == 'a'))
                    pPh->iVbEtat = NSPhraseur::vbEtatApparent;
                else if (((*psActEtat)[1] == 'D') || ((*psActEtat)[1] == 'd'))
                    pPh->iVbEtat = NSPhraseur::vbEtatDure;
                else if (((*psActEtat)[1] == 'C') || ((*psActEtat)[1] == 'c'))
                    pPh->iVbEtat = NSPhraseur::vbEtatChange;
            }
        }
    }
    return true;
}

string
NSGenerateurFr::donneAuxilliaire(string* Auxilliaire, int iVbTemps, int /* iVbAspect */, int /* iVbMode */, int iVbPersonne, int /* iVbNegation */)
{
  if (string("4ETRE") == *Auxilliaire)
  {
    switch (iVbTemps)
    {
      case tempsPasseFr   :
        switch (iVbPersonne)
        {
          case NSPhraseur::pers1S : return string("étais") ;
          case NSPhraseur::pers2S : return string("étais") ;
          case NSPhraseur::pers3S : return string("était") ;
          case NSPhraseur::pers1P : return string("étions") ;
          case NSPhraseur::pers2P : return string("étiez") ;
          case NSPhraseur::pers3P : return string("étaient") ;
        }
        break ;
      case tempsPresentFr :
        switch (iVbPersonne)
        {
          case NSPhraseur::pers1S : return string("suis") ;
          case NSPhraseur::pers2S : return string("es") ;
          case NSPhraseur::pers3S : return string("est") ;
          case NSPhraseur::pers1P : return string("sommes") ;
          case NSPhraseur::pers2P : return string("êtes") ;
          case NSPhraseur::pers3P : return string("sont") ;
        }
        break ;
      case tempsFuturFr   :
        switch (iVbPersonne)
        {
          case NSPhraseur::pers1S : return string("serai") ;
          case NSPhraseur::pers2S : return string("seras") ;
          case NSPhraseur::pers3S : return string("sera") ;
          case NSPhraseur::pers1P : return string("serons") ;
          case NSPhraseur::pers2P : return string("serez") ;
          case NSPhraseur::pers3P : return string("seront") ;
        }
        break;
    }
  }
  return string("") ;
}


// traitePostpositon sert a remplir la string sPreposition des noms qui completent
// un nom qui impose une preposition a ses suivants.
//
// Antecedents de pericardite et d'endocardite.
//
// pSpecifique sert dans le cas ou l'on veut mettre des prepositions liees au mot
// pMot dans une array donnee, mais qui ne figure pas dans pComplement.

bool
NSGenerateurFr::traitePostposition(NSPhraseMot* pMot, NSPhraseMotArray* pSpecifique)
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

        if (false == pPh->COS.empty())
        {
            for (iterMots = pPh->COS.begin(); iterMots != pPh->COS.end(); iterMots++)
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


// traitePrepositionCC : méthode qui met les prépositions, spécifiquement pour les
// les compléments circonstanciels.

bool
NSGenerateurFr::traitePrepositionCC(NSPhraseMotArray* pCompC, NSPhraseMot* pPreposition, NSPhraseur::VBPREPOCTX iContexte)
{
  iterPhraseMot  iterMots;

  if (pPreposition)
  {
    if (pPreposition->getLexique() != string(""))
    {
      traitePostposition(pPreposition, pCompC) ;
      return true ;
    }
  }

  // Si on n'a pas specifie de preposition, il faut au moins avoir specifie un contexte.

  if (NSPhraseur::prepUndefined == iContexte)
    return false ;

  // Si on a specifie un contexte, on met une preposition par defaut en fonction du
  // contexte.

  // Pour le lieu, on met "à" en général et "de" en complément du nom :
  //
  // La PA à la cheville est de XX mmHg / La PA est de XX mmHG à la cheville
  // La longueur de la jambe est de 25 cm
  //
  if (NSPhraseur::prepLieu == iContexte)
  {
    if (pCompC)
    {
      if (false == pCompC->empty())
      {
        string sPrepos = string("à") ;
        if (NSPhraseur::phraseComplement == pPh->iPhraseType)
          string sPrepos = string("de") ;

        for (iterMots = pCompC->begin(); pCompC->end() != iterMots ; iterMots++)
        {
          if (false == (*iterMots)->estTexteLibre())
          {
            (*iterMots)->setPreposition(sPrepos) ;
            (*iterMots)->setArticle(NSPhraseMot::articleDefini) ;
          }
          else
          {
            // On rajoute la preposition dans le libelle du texte libre.
            (*iterMots)->setPreposition(sPrepos) ;
            (*iterMots)->setTexteLibre(sPrepos + string(" ") + (*iterMots)->getTexteLibre()) ;
          }
        }
        return true ;
      }
    }
  }

  // Pour le temps, on met..., on met quoi au fait?

  // Pour la maniere, on met "avec"
  else if (NSPhraseur::prepManiere == iContexte)
  {
    (pPh->PrepositionManiere).setLexique("0AVEC1") ;
    return traitePrepositionCC(pCompC, &(pPh->PrepositionManiere)) ;
  }

  // Pour la cause, mettre "a cause de".
  // Pour le but, mettre "pour".

  // pour le type, on met "a type de"

  else if (NSPhraseur::prepType == iContexte)
  {
    (pPh->PrepositionType).setLexique("0ATYP1") ;
    return traitePrepositionCC(pCompC, &(pPh->PrepositionManiere)) ;
  }
  else if (NSPhraseur::prepHypoth == iContexte)
  {
    (pPh->PrepositionHypoth).setLexique("KEVEI2") ;
    return traitePrepositionCC(pCompC, &(pPh->PrepositionHypoth)) ;
  }

  return false ;
}


// modifie préposition : méthode qui contracte s'il le faut le "de" en "d'".

bool
NSGenerateurFr::modifiePreposition(NSPhraseMot* pMot, int iArticle, bool bPluriel)
{
  if (NULL == pMot)
    return false ;

  NSSuper* pSuper = pContexte->getSuperviseur() ;

  string sLibelle    = string("") ;
  bool   bApostrophe = false ;
  GENRE  iGenreNom   = genreNull ;

  NSPhraseMot    MotAdj(pContexte) ;
  NSPhraseMot    MotAdv(pContexte) ;

  NSPathologData DataAdj ;
  NSPathologData DataAdv ;

  string sLexiqueAdj = "" ;
  string sLexiqueAdv = "" ;

  string sCertitudeAdj = "" ;
  string sCertitudeAdv = "" ;

  string sCertitudeNom = string(pMot->getCertitude(), 0, 5) ;

  string sLexique = pMot->getLexique() ;
  NSPathologData Data ;

  bool trouve = pContexte->getDico()->trouvePathologData(sLang, &sLexique, &Data) ;
  if (false == trouve)
    return false ;

  Data.donneGenre(&iGenreNom) ;
  donneLibelleAffiche(&sLibelle, &Data) ;

  // En francais, on n'a que le "de" à changer parfois en "d'".

  string sPreposition = pMot->getPreposition() ;

  // Soit "de", soit une preposition qui se termine par " de"
  //
  if ((string("de") != sPreposition) ||
      ((strlen(sPreposition.c_str()) > 2) &&
       (string(sPreposition, strlen(sPreposition.c_str())-3, 3) != " de")))
    return false ;

  if (NSPhraseMot::articleDefini == iArticle)
    return false ;

  if (NSPhraseMot::articleIndefini == iArticle)
  {
    if (false == bPluriel)
    {
      sPreposition.replace(strlen(sPreposition.c_str())-2, 2, "d'") ;
      pMot->setPreposition(sPreposition) ;
      return true ;
    }
    else
      return false ;
  }

  if (CommenceParVoyelle(&sLibelle))
    bApostrophe = true ;


  if (('h' == sLibelle[0]) || ('H' == sLibelle[0]))
  {
    if (Data.chercheGrammaire(VALEUR_DE_H) == H_MUET)
      bApostrophe = true;
  }

  if (pMot->getComplementPhr())
  {
    bool bPremierAdjExiste = donnePremierAdj(pMot->getComplementPhr(), &MotAdj) ;
    sLexiqueAdj = MotAdj.getLexique() ;

    if (bPremierAdjExiste)
    {
      sCertitudeAdj = string(MotAdj.getCertitude(), 0, 5) ;

      if (string("") != sCertitudeAdj)
      {
        // Le premier adjectif est precede d'une certitude, donc on ne
        // s'occupe pas de la premiere lettre de l'adjectif.

        donneCertitude(sCertitudeAdj, &bApostrophe);
      }
      else if (MotAdj.getComplementPhr())
      {
        bool bPremierAdvExiste = donnePremierAdv(MotAdj.getComplementPhr(), &MotAdv);
        sLexiqueAdv = MotAdv.getLexique() ;

        if (bPremierAdvExiste)
        {
          sCertitudeAdv = string(MotAdv.getCertitude(), 0, 5) ;

          if (string("") != sCertitudeAdv)
          {
            // Le premier adverbe est precede d'une certitude, donc on ne
            // s'occupe pas de la premiere lettre de l'adverbe.

            donneCertitude(sCertitudeAdv, &bApostrophe) ;
          }
          else
          {
            // On regarde si l'adverbe commence par une voyelle ou par un
            // h muet.
            string sLibelleAdv = string("") ;
            donneLibelleAffiche(&sLibelleAdv, &DataAdv) ;

            if (CommenceParVoyelle(&sLibelleAdv))
              bApostrophe = true ;

            else if (('h' == sLibelleAdv[0]) || ('H' == sLibelleAdv[0]))
            {
              if (DataAdv.chercheGrammaire(VALEUR_DE_H) == H_MUET)
                bApostrophe = true;
            }
          }
        }
        else
        {
          // On regarde si l'adjectif commence par une voyelle ou par un
          // h muet.
          string sLibelleAdj = string("") ;
          donneLibelleAffiche(&sLibelleAdj, &DataAdj, iGenreNom) ;

          if (CommenceParVoyelle(&sLibelleAdj))
            bApostrophe = true ;

          else if (('h' == sLibelleAdj[0]) || ('H' == sLibelleAdj[0]))
          {
            if (DataAdj.chercheGrammaire(VALEUR_DE_H) == H_MUET)
              bApostrophe = true ;
          }
        }
      }
    }
  }

  if (bApostrophe)
  {
    sPreposition.replace(strlen(sPreposition.c_str())-2, 2, "d'") ;
    pMot->setPreposition(sPreposition) ;
    return true ;
  }

  return false ;
}

string
NSGenerateurFr::postTraitement(string *psEntree)
{
  if ((NULL == psEntree) || (string("") == *psEntree))
    return string("") ;

  string sSortie = *psEntree ;

  // Contraction de la forme "de le " en "du "
  //
  contracting(&sSortie, string("de le "), string("du ")) ;

  // Contraction de la forme "de les " en "des "
  //
  contracting(&sSortie, string("de les "), string("des ")) ;

  // Contraction de la forme "de des " en "des "
  //
  contracting(&sSortie, string("de des "), string("des ")) ;

  // Contraction de la forme "à les " en "aux " et "à le " en "au "
  //
  contracting(&sSortie, string("à le "), string("au ")) ;
  contracting(&sSortie, string("à les "), string("aux ")) ;

  // Contraction de "est non " en "n'est pas "
  //
  contracting(&sSortie, string("est non "), string("n'est pas ")) ;

  // Contraction de "de un " en "d'un " et de "de une " en "d'une "
  //
  contracting(&sSortie, string("de un "), string("d'un ")) ;
  contracting(&sSortie, string("de une "), string("d'une ")) ;

  return sSortie ;
}

//
// ////////////////  OBJET  gereDateFr   /////////////////
//

//
//  constructeur copie
//
gereDateFr::gereDateFr(const gereDateFr& src)
		       :gereDate(src._pSuper, src.sLang)
{
  gereDate(*this) = (gereDate) src ;
}

//
//  opérateur d'affectation
//
gereDateFr&
gereDateFr::operator=(const gereDateFr& src)
{
  if (this == &src)
		return *this ;

  gereDate(*this) = (gereDate) src ;
  
  return *this ;
}

//  +-----------------------------------------------------------------+
//  ¦               Donne la date au format JJ/MM/AAAA                ¦
//  +-----------------------------------------------------------------+
//  Créé le 14/11/1997 Dernière mise à jour 14/11/1997
void
gereDateFr::donne_date_breve(string* pMessage, string* pIntro, int iType)
{
  if ((NULL == pMessage) || (NULL == pIntro))
    return ;

  *pMessage   = "" ;
  *pIntro     = "" ;
  //
  // Date au format AAAAMMJJ
	//
  	if (sUnite == "2DA01")
  	{
        // Date vide
        if (sDate == "00000000")
      	    return ;
        // Juste AAAA -> "en 2001"
        else if (string(sDate, 4, 4) == "0000")
        {
      	    *pMessage = string(sDate, 0, 4) ;
            switch (iType)
            {
                case DateNormal :
                    *pIntro = "en " ;
                    break ;
                case DateDeb :
                    *pIntro = "de " ;
                    break ;
                case DateFin :
                    *pIntro = "à " ;
                    break ;
            }
        }
        // MM/AAAA -> "en juillet 2001"
        else if (string(sDate, 6, 2) == "00")
        {
      	    *pMessage = donne_mois(string(sDate, 4, 2)) + " " + string(sDate, 0, 4) ;
            switch (iType)
            {
                case DateNormal :
                    *pIntro = "en " ;
                    break ;
                case DateDeb :
                    *pIntro = "du " ;
                    break ;
                case DateFin :
                    *pIntro = "au " ;
                    break ;
            }
        }
        // JJ/MM/AAAA -> "le 25/07/2001"
        else
        {
      	    *pMessage = string(sDate, 6, 2) + "/" + string(sDate, 4, 2) + "/" + string(sDate, 0, 4);
            switch (iType)
            {
                case DateNormal :
                    *pIntro = "le " ;
                    break ;
                case DateDeb :
                    *pIntro = "du " ;
                    break ;
                case DateFin :
                    *pIntro = "au " ;
                    break ;
            }
        }
    }
    //
  	// Date au format AAAAMMJJhhmmss
	//
  	else if (sUnite == "2DA02")
  	{
        // Date vide
        if (sDate == "00000000")
      	    return ;
        // Juste AAAA -> "en 2001"
        else if (string(sDate, 4, 4) == "0000")
        {
      	    *pMessage = string(sDate, 0, 4) ;
            switch (iType)
            {
                case DateNormal :
                    *pIntro = "en " ;
                    break ;
                case DateDeb :
                    *pIntro = "de " ;
                    break ;
                case DateFin :
                    *pIntro = "à " ;
                    break ;
            }
        }
        // MM/AAAA -> "en juillet 2001"
        else if (string(sDate, 6, 2) == "00")
        {
      	    *pMessage = donne_mois(string(sDate, 4, 2)) + " " + string(sDate, 0, 4) ;
            switch (iType)
            {
                case DateNormal :
                    *pIntro = "en " ;
                    break ;
                case DateDeb :
                    *pIntro = "du " ;
                    break ;
                case DateFin :
                    *pIntro = "au " ;
                    break ;
            }
        }
        // JJ/MM/AAAA -> "le 25/07/2001"
        else
        {
      	    *pMessage = string(sDate, 6, 2) + "/" + string(sDate, 4, 2) + "/" + string(sDate, 0, 4);
            switch (iType)
            {
                case DateNormal :
                    *pIntro = "le " ;
                    break ;
                case DateDeb :
                    *pIntro = "du " ;
                    break ;
                case DateFin :
                    *pIntro = "au " ;
                    break ;
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
                *pMessage += string(" à ") + sHeure ;
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
                *pIntro = "à " ;
                break;
            case DateDeb :
                *pIntro = "de " ;
                break;
            case DateFin :
                *pIntro = "à " ;
                break;
        }
    }

    return;
}

//  +-----------------------------------------------------------------+
//  ¦           Donne la date au format "1er janvier 1994"            ¦
//  +-----------------------------------------------------------------+
//  Créé le 14/11/1997 Dernière mise à jour 14/11/1997
void
gereDateFr::donne_date_claire(string* pMessage, string* pIntro, int iType)
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
            case DateNormal :   *pIntro = "le ";
                                break;
            case DateDeb :      *pIntro = "du ";
                                break;
            case DateFin :      *pIntro = "au ";
                                break;
        }
		//
		// Jour
		//
		if (isdigit(sDate[6]) && isdigit(sDate[7]))
		{
			if (sDate[6] == '0')
			{
				if (sDate[7] == '1')
					*pMessage = "1er ";
				else if (sDate[7] != '0')
				{
					*pMessage = sDate[7];
					*pMessage += " ";
				}
                else
                {
                    switch (iType)
                    {
                        case DateNormal :   *pIntro = "en ";
                                            break;
                        case DateDeb :      *pIntro = "de ";
                                            break;
                        case DateFin :      *pIntro = "à ";
                                            break;
                    }
                }
			}
			else
			{
				*pMessage = sDate[6];
				*pMessage += sDate[7];
				*pMessage += " ";
			}
		}
		else
			*pMessage = "?? ";
		//
		// Mois
		//
        string sMois = donne_mois(string(sDate, 4, 2));
        if (sMois != "")
      	    *pMessage += sMois + " ";
        else
      	    *pMessage = "?? ";
        //
		// Année
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
            case DateNormal :   *pIntro = "le ";
                                break;
            case DateDeb :      *pIntro = "du ";
                                break;
            case DateFin :      *pIntro = "au ";
                                break;
        }
		//
		// Jour
		//
		if (isdigit(sDate[6]) && isdigit(sDate[7]))
		{
			if (sDate[6] == '0')
			{
				if (sDate[7] == '1')
					*pMessage = "1er ";
				else if (sDate[7] != '0')
				{
					*pMessage = sDate[7];
					*pMessage += " ";
				}
                else
                {
                    switch (iType)
                    {
                        case DateNormal :   *pIntro = "en ";
                                            break;
                        case DateDeb :      *pIntro = "de ";
                                            break;
                        case DateFin :      *pIntro = "à ";
                                            break;
                    }
                }
			}
			else
			{
				*pMessage = sDate[6];
				*pMessage += sDate[7];
				*pMessage += " ";
			}

            // gestion de l'heure
            if      (strlen(sDate.c_str()) == 10)
                sHeure = string(sDate, 8, 2) + string(" heures") ;
            else if (strlen(sDate.c_str()) == 12)
                sHeure = string(sDate, 8, 2) + string(" heures ") + string(sDate, 10, 2) ;
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
      	    *pMessage += sMois + " ";
        else
      	    *pMessage = "?? ";
        //
		// Année
		//
		for (i = 0; i < 4; i++)
		{
			if (isdigit(sDate[i]))
				*pMessage += sDate[i];
			else
				*pMessage += '?';
		}

        if (sHeure != "")
            *pMessage += string(" à ") + sHeure ;
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
                *pIntro = "à " ;
                break;
            case DateDeb :
                *pIntro = "de " ;
                break;
            case DateFin :
                *pIntro = "à " ;
                break;
        }
    }
	return;
}

//  +-----------------------------------------------------------------+
//  ¦          Donne le libellé du mois à partir de "MM"              ¦
//  +-----------------------------------------------------------------+
//  Créé le 14/11/1997 Dernière mise à jour 14/11/1997
string
gereDateFr::donne_mois(string mois)
{
    if (strlen(mois.c_str()) < 2)
   	    return "";
    if ((!(isdigit(mois[0]))) || (!(isdigit(mois[1]))))
   	    return "";
    int i = 10 * donneDigit(mois[0]) + donneDigit(mois[1]);

    return donne_mois(i);
}

//  +-----------------------------------------------------------------+
//  ¦          Donne le libellé du mois à partir d'un int             ¦
//  +-----------------------------------------------------------------+
//  Créé le 14/11/1997 Dernière mise à jour 14/11/1997
string
gereDateFr::donne_mois(int iMois)
{
    switch (iMois)
    {
   	    case  1 : return "janvier";
		case  2 : return "février";
		case  3 : return "mars";
		case  4 : return "avril";
		case  5 : return "mai";
		case  6 : return "juin";
		case  7 : return "juillet";
		case  8 : return "août";
		case  9 : return "septembre";
		case 10 : return "octobre";
		case 11 : return "novembre";
		case 12 : return "décembre";
    }
    return "";
}







