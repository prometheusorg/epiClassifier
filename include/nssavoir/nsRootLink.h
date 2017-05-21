// -----------------------------------------------------------------------------
// nsRootLink.h
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
// Copyright Nautilus, 2004
// http://www.nautilus-info.com
// -----------------------------------------------------------------------------
// Ce logiciel est un programme informatique servant à [rappeler les
// caractéristiques techniques de votre logiciel].
//
// Ce logiciel est régi par la licence CeCILL soumise au droit français et
// respectant les principes de diffusion des logiciels libres. Vous pouvez
// utiliser, modifier et/ou redistribuer ce programme sous les conditions de la
// licence CeCILL telle que diffusée par le CEA, le CNRS et l'INRIA sur le site
// "http://www.cecill.info".
//
// En contrepartie de l'accessibilité au code source et des droits de copie, de
// modification et de redistribution accordés par cette licence, il n'est offert
// aux utilisateurs qu'une garantie limitée. Pour les mêmes raisons, seule une
// responsabilité restreinte pèse sur l'auteur du programme, le titulaire des
// droits patrimoniaux et les concédants successifs.
//
// A cet égard  l'attention de l'utilisateur est attirée sur les risques
// associés au chargement, à l'utilisation, à la modification et/ou au
// développement et à la reproduction du logiciel par l'utilisateur étant donné
// sa spécificité de logiciel libre, qui peut le rendre complexe à manipuler et
// qui le réserve donc à des développeurs et des professionnels avertis
// possédant des connaissances informatiques approfondies. Les utilisateurs sont
// donc invités à charger et tester l'adéquation du logiciel à leurs besoins
// dans des conditions permettant d'assurer la sécurité de leurs systèmes et ou
// de leurs données et, plus généralement, à l'utiliser et l'exploiter dans les
// mêmes conditions de sécurité.
//
// Le fait que vous puissiez accéder à cet en-tête signifie que vous avez pris
// connaissance de la licence CeCILL, et que vous en avez accepté les termes.
// -----------------------------------------------------------------------------
// This software is a computer program whose purpose is to [describe
// functionalities and technical features of your software].
//
// This software is governed by the CeCILL  license under French law and abiding
// by the rules of distribution of free software. You can use, modify and/ or
// redistribute the software under the terms of the CeCILL license as circulated
// by CEA, CNRS and INRIA at the following URL "http://www.cecill.info".
//
// As a counterpart to the access to the source code and  rights to copy, modify
// and redistribute granted by the license, users are provided only with a
// limited warranty and the software's author, the holder of the economic
// rights, and the successive licensors have only limited liability.
//
// In this respect, the user's attention is drawn to the risks associated with
// loading, using, modifying and/or developing or reproducing the software by
// the user in light of its specific status of free software, that may mean that
// it is complicated to manipulate, and that also therefore means that it is
// reserved for developers and experienced professionals having in-depth
// computer knowledge. Users are therefore encouraged to load and test the
// software's suitability as regards their requirements in conditions enabling
// the security of their systems and/or data to be ensured and, more generally,
// to use and operate it in the same conditions as regards security.
//
// The fact that you are presently reading this means that you have had
// knowledge of the CeCILL license and that you accept its terms.
// -----------------------------------------------------------------------------

#ifndef __NSROOTLINK_H__
# define __NSROOTLINK_H__

#ifndef __linux__
# if defined(_DANSLEXIDLL)
#  define _CLASSELEXI __export
# else
#  define _CLASSELEXI __import
# endif
#endif


// -----------------------------------------------------------------------------
// Important note about Doxygen tag
// -----------------------------------------------------------------------------
// All comments begining with "//!" or "//!<" must not be deleted, they concerns
// Doxygen doc generator.
// If you modify some classes and/or methods and/or variables please modify also
// the doxygen available informations.
// -----------------------------------------------------------------------------


//! \brief NSRootLink is the root class for all operations about links. It
//!        contains the enum that define all types of links

//! problemRelatedTo         : lien entre une préoccupation et un objet                              1
//! problemContactElement    : lien entre l'Assesment et sa préoccupation                            2
//! soapRelatedTo            : lien entre un élément du SOAP et son Assesment                        3
//! problemGoals             : lien entre un objectif et la préoccupation à laquelle il est lié      4
//! goalOpener               : évènement qui déclenche un objectif                                   5
//! goalReseter              : évènement qui redémarre un nouveau cycle, si cyclique                 6
//! goalAlarm                : pour les objectifs "existence de", signal de disparition              7
//! goalCloser               : évènement qui ferme un objectif                                       8
//! archetype                : lien entre un noeud et l'Archetype dont il est lanceur                9
//! refCreator               : document qui a initié un noeud au sein d'un autre document           10
//! referentialOf            : lien entre un référentiel et le noeud qui a été créé en l'appliquant 11
//! guidelineOf              : lien entre un arbre décisionnel et le noeud qui a été créé           12
//! drugOf                   : lien entre un ttt et la préoccupation à laquelle il est lié          13
//! treatmentOf                                                                                     14
//! prescriptionElement      : arbre de prescription, en dehors de l'arbre d'index de santé         15
//! personDocument           : meta-document appartenant à la personne                              16
//! personHealthIndex        : lien entre Personne et l'index de santé                              17
//! personSynthesis          : lien entre Personne et la synthèse                                   18
//! personIndexExtension     : lien entre Personne et une extension d'index (concern, ttt, goal)    19
//! personAdminData          : lien entre Personne et ses données administratives                   20
//! personHealthTeam         : lien entre Personne et son équipe de santé                           21
//! personContribution       : contribution pour cette personne                                     22
//! personFolderLibrary      : bibliothèque de dossiers pour cette personne                         23
//! personHealthProData      : données de professionnel de santé pour cette personne                24
//! personRiskManager        : arbre de préoccupations de santé de "risque de..."                   25
//! docData                  : données du document                                                  26
//! docFolder                : chemise (dossier) auquel appartient le document                      27
//! docPilot                 : service Pilot qui permet l'accès aux data du document                28
//! contribElement           : arbre modifié/créé lors de cette contribution                        29
//! indexConcerns            : extension d'index contenant les préoccupations                       30
//! indexGoals               : extension d'index contenant les objectifs                            31
//! docComposition           : description des éléments de document composé                         32
//! compositionTag           : lien entre un tag et le document qui prend sa place                  33
//! processWaitingFor        : lien entre un noeud patient et une préoccupation en attente          34     // modif fab - 06/08/2003
//! processResultFrom        : lien entre un noeud résultat d'une préoccupation et la préoccupation 35     // modif fab - 06/08/2003
//! objectIn                 : lien entre un objet et son objet pere                                36     // modif rem - 17/10/2003
//! hiddenBy                 : noeud masqué par un autre noeud                                      37
//! contributionAdded        : lien entre une contribution et un document qui y a été créé          38
//! contributionModified     : lien entre une contribution et un document qui y a été modifié       39
//! contributionOpened       : lien entre une contribution et un document qui a été ouvert          40
//! copyOf                   : lien entre un noeud et sa copie                                      41
//! fonctionalUnitStay       : lien entre le séjour et le noeud root d'un patient                   42
//! stayContribution         : lien entre le séjour et la contribution                              43
//! personIdentifiers        : lien entre un identifiant et le noeud root d'un patient              44
//! OCRizedDocument          : lien entre un document image et son document OCRisé                  45
//! semantizedDocument       : lien entre un document texte et son document arbre                   46
//! viewOfDocument           : lien entre un document texte type et le document arbre maitre        47
//! letterOfDocument         : lien entre un document et une lettre d'accompagnement                48
//! appointment              : lien entre le noeud root et l'arbre des rendez-vous                  49

# define NBGRAPHLINKTYPES 36

#ifndef __linux__
class _CLASSELEXI NSRootLink
#else
class NSRootLink
#endif
{
 public:
	enum NODELINKTYPES {	badLink = 0,
                        problemRelatedTo = 1, problemContactElement,
                        soapRelatedTo, problemGoals,
                        goalOpener, goalReseter, goalAlarm, goalCloser,
                        archetype, refCreator, referentialOf, guidelineOf,
                        drugOf, treatmentOf, prescriptionElement,
                        personDocument, personHealthIndex, personSocialIndex,
                        personSynthesis, personIndexExtension,
                        personAdminData, personHealthTeam, personContribution,
                        personFolderLibrary, personHealthProData, personRiskManager,
                        docData, docFolder, docPilot, contribElement,
                        indexConcerns, indexGoals,
                        docComposition, compositionTag,
                        processWaitingFor, processResultFrom, objectIn, hiddenBy,
                        contributionAdded, contributionModified, contributionOpened,
                        copyOf,
                        fonctionalUnitStay, stayContribution, personIdentifiers,
                        OCRizedDocument, semantizedDocument, viewOfDocument, letterOfDocument, externViewOfDocument, structuredVersionOfDocument,
                        appointment } ;

  int iTypeGraphe ;   //!< Type de graphe (graphPerson / graphObject) - pour mode MUE uniquement -

  //! constructor
  NSRootLink() ;

  //! destructor
  ~NSRootLink() ;

  //! Transformation d'une relation en code
	std::string     donneString(NODELINKTYPES iRelation) ;

  //! Transformation d'un code en relation
	NODELINKTYPES   donneRelation(std::string sRelation) ;

  //! Changement du type de graphe
  void            setTypeGraphe(int iType)    { iTypeGraphe = iType ; }
} ;

#endif // !__NSROOTLINK_H__
