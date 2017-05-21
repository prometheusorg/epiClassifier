//---------------------------------------------------------------------------
//     NsFilGd.H
//
//  PA Aout 1993  && Hafedh Kaddachi Mai 97
//---------------------------------------------------------------------------
#ifndef __NSFILGDENGINE_H
#define __NSFILGDENGINE_H

#include "nssavoir/nspathor.h"
#include "nssavoir/nsvarray.h"
#include <vector>

#ifndef __linux__
#if defined(_DANSLEXIDLL)
	#define _CLASSELEXI __export
#else
	#define _CLASSELEXI __import
#endif
#endif

//
// Classe d'acces aux fils guides et au reseau semantique
//
class NSFilGuideEngine : public NSSuperRoot
{
	protected:

  	bool               _bCherchePivot ;
		size_t             _iRangPivotPere ;   // la position du pivot dans la source
		size_t             _iRangPivotCible ;  // la position du pivot dans la cible
    size_t             _iNbCommonElements ;
    
    string             _sCibleAudelaDuPere ; //partie de sCible au dela du pere

    VecteurString      _VectItemCible ;   // items of the target item
    EquiItemIter       _IterCible ;

    VecteurRechercheSelonCritere* _pVecteurSelonCritere ;

    VecteurItem*       _pVecEquivalentItemDuPere ;
    EquiItemVectorIter _itEquivalentItemDuPere ;

    VecteurString*     _pVectItemPere ;
    EquiItemIter       _iterPere ;
    string             _sDernierItemPere ;

    vector<int>        _IndiceSource ;
    typedef vector<int>::iterator indiceIterator ;

    string             _sPivot ;

		string             _sChaineFils ;
		string             _sPrePivot ;

    string             _sNouvelleCible ;

	public:

    // Constructeur et destructeur
    NSFilGuideEngine(NSSuper* pSuper) ;
    ~NSFilGuideEngine() ;

    // Fonction de prise de main
		void getPivotsAndRanks() ;
    void getRanks() ;
    void getPivotsVector() ;
    void getTargetBeyondFather() ;
    bool findPivot() ;
    void buildNewTargetString() ;

    void initTargetVector(string sTargetString) ;
    void initVectItemPere(VecteurString* pVectPere) ;

		void DecomposeChaine(string* pChaine, VecteurString* pVect) ;

    string getNewTarget() { return _sNouvelleCible ; }

    void   setNbCommonElements(size_t iNbCE)                         { _iNbCommonElements = iNbCE ; }
    void   setVectSelonCriteres(VecteurRechercheSelonCritere* pVect) { _pVecteurSelonCritere = pVect ; }
    void   setEquiItemPere(VecteurItem* pEqItemPere) ;
};

#endif // __NSFILGDENGINE_H

