//-------------------------------------------------------------------------
//                    BIBLIOTHEQUE : NSDIVFCT.H
//
//                     Pot-pourri de fonctions
//
//             Debut d'ecriture : 30 Juin 1988 (PA)
//             Derniere version : 28 Juillet 1994 (PA)
//-------------------------------------------------------------------------
#ifndef __NSDIVFCT_H
#define __NSDIVFCT_H

#include <map>
#include <string>
#include <vector>

using std::vector;
using std::map;
using std::string;
// #include <owl\applicat.h>

#ifndef BOOL
	#define BOOL    int
#endif

enum STRIPTYPE {stripBoth = 0, stripLeft, stripRight};

enum ERRORTYPE {standardError = 0, fatalError, warningError};

enum GRAPHELEMTYPE { isIncorrectID = 0, isCollectiveID, isGroupID, isLocalID, isMemoryID };

#ifdef _NSUTILDLL

	unsigned int _export DonneNomFichier(const char *ammorce, const char* suffixe,
									  const char *chemin, char** nomLibre);
	unsigned int _export DonneNumeroFichier(const char *ammorce, const char* suffixe,
										  const char *nomFichier, char* numeroFichier);

	GRAPHELEMTYPE _export getGraphElementTypeFromID(string sElementID);
	string        _export getPatientIDFromID(string sAnyID);
	string        _export getTreeIDFromID(string sAnyID);
	string        _export getNodeIDFromID(string sAnyID);

  string        _export getRegularPath(string sMonoSeparatorPath, char cPath, char cIntranode);

	void   _export strip(string& s, STRIPTYPE n = stripBoth, char c = ' ');
	void   _export strip(string& s, string sM, STRIPTYPE n = stripBoth);

	void	 _export incremente_code(string&);
	string _export nom_sans_doublons(string, string, string, string, string);
	void 	 _export numacar(char*, unsigned int, unsigned int);
  void 	 _export numacarbase(string*, unsigned int nbre, unsigned int size, unsigned int base);
	double _export arrondiInt(double dNbre);
	int 	 _export dtoi(double dNbre);
	int 	 _export donneDigit(char nbre);
	char 	 _export donneChar(int digit);
	int 	 _export ote_blancs(char*);
	int 	 _export egalite(char*, char*);
	char 	 _export pseumaj(char);
  void   _export pseumaj(char*);
  void   _export pseumaj(string*);
  string _export pseumaj(string);
  char 	 _export vraiemaj(char);
  void   _export vraiemaj(char*);
  void   _export vraiemaj(string*);
  void   _export firstmaj(char*);
  void   _export firstmaj(string*);
  string _export strpids(string);

  string _export getFormatedTime(string sRawTime, string sLang, string sTimeFormat);
  string _export getFormatedTime(string sRawTime, string sLang, string sDateFormat, string sHourFormat);
  string _export getFormatedDate(string sRawDate, string sLang, string sDateFormat);
  string _export getFormatedHour(string sRawHour, string sLang, string sHourFormat);

  string _export getRawTime(string sFormatedTime, string sLang, string sTimeFormat);
  string _export getRawTime(string sFormatedTime, string sLang, string sDateFormat, string sHourFormat);
  string _export getRawDate(string sFormatedDate, string sLang, string sDateFormat);
  string _export getRawHour(string sFormatedHour, string sLang, string sHourFormat);

  // void 	 _export donne_heure(char* pHeure);
	// void 	 _export donne_date_duJour(char* date);
  string _export donne_heure();
  string _export donne_date_duJour();
  void 	 _export get_current_date(string* sDate);
  // void	 _export donne_heure(char*, string&);
	// void 	 _export donne_date(char*, char*, string sLang);
  string _export donne_heure(const string);
  string _export donne_date(string, string sLang);
  void 	 _export donne_date_inverse(string, string&, string sLang);
	void 	 _export donne_date_claire(char*, char*, string sLang);
	void 	 _export donne_date_claire(char*, string*, string sLang);

  void	 _export donne_date_breve(string sDate, string* pMessage, string* pIntro, string sLang);
  void   _export donne_date_claire(string sDate, string* pMessage, string* pIntro, string sLang);
  bool   _export bissextile(int year);
  void   _export incremente_date(string& sDate);
  string _export donne_mois(string mois, string sLang);
  string _export donne_mois(int iMois, string sLang);

  string _export donne_deltaTemps(int iYears, int iMonths, int iDays);

	void	 _export donne_somme_courte(char*, char*, string sLang);

	void 	 _export long_to_hex(unsigned long, char*);
	void 	 _export hex_to_long(char*, unsigned long*);
  void 	 _export char_to_hex(unsigned char, char*);
	void 	 _export hex_to_char(char*, unsigned char*);

	void 	 _export format_date(char*);
	void   _export carcat(char*, char);

	double _export StringToDouble(string sValeur);
	string _export DoubleToString(double* pDouble, int iEntier, int iDecimal);
	int    _export StringToInt(string sValeur);
	string _export IntToString(int val);
  
  string _export getEpisodusNumValueFromRawCapturedNum(string sRawNum, string sLang = "");
  string _export getEpisodusDateValueFromRawCapturedDate(string sRawNum, string sLang = "");

	void	 _export SetInternational(void);

	// WIN32
	BOOL   _export ptrInit(char **strPtr, unsigned int length, char car = ' ');
	BOOL   _export ptrInit(char **strPtr, const char *strInit);
	BOOL   _export ptrInit(unsigned char **strPtr, const unsigned char *strInit);

  // fonctions de gestion d'erreur
  int 	 _export error(const char *texte, ERRORTYPE gravite);
  bool   _export getModeFileErr();
  void   _export setModeFileErr(bool bMode);
  void   _export fileError(string sError);

  //********************************************************************
	//vecteur de string
	//********************************************************************
	class _export classString
	{
  	public:

      classString(string sChaine, int Colonne = -1, bool Coche = false, string sQualif = string(""));
      classString(const classString& src);
      classString& operator=(classString src);

      void   setColumn(int iC)       { colonne    = iC; }
      void   setChecked(bool bC)     { coche      = bC; }
      void   setItem(string sI)      { sItem      = sI; }
      void   setQualifier(string sQ) { sQualifier = sQ; }

      int    getColumn()    { return colonne; }
      bool   getChecked()   { return coche; }
      string getItem()      { return sItem; }
      string getQualifier() { return sQualifier; }

    protected:

      int     colonne;
      bool    coche;
      string  sItem;
      string  sQualifier;
	};

	typedef vector<classString*> vectclassString;
	typedef vectclassString::iterator iterClassString;

	class _export ClasseStringVector : public vectclassString
	{
  	public:
    	ClasseStringVector();
      ~ClasseStringVector();
      ClasseStringVector(ClasseStringVector& src);
      // Surcharge de l'opérateur d'affectation
      ClasseStringVector& operator=(ClasseStringVector src);
      void vider();
	};

	//vecteur de string
	typedef vector<string*> EquiItem;
	typedef EquiItem::iterator IterString;
	class _export VectString : public EquiItem
	{
		public:

			VectString();
      ~VectString();
      VectString(VectString& src);

      bool contains(string sModele);

      // Surcharge de l'operateur d'affectation
      VectString& operator=(VectString src);
      void vider();
	};

  bool   _export isLogicalFormulaValid(string sFormula, string *psErrorMsg, bool bPureLogical);
  bool   _export isLogicalBlockValid(string sFormula, string *psErrorMsg);
  void   _export parseLogicalBlocks(string &sFormula, map<string, string> *paBlocks);
  bool   _export parseComparisonBlock(string sFormula, string &sLValue, string &sOperator, string &sRValue, string *psErrorMsg, bool bSingleBlockAccepted);
  int    _export evaluate(map<string, bool> *pDatas, string sFormula, size_t& cc);
  bool   _export compare(double dLValue, string sOperator, double dRValue);
  double _export calculate(map<string, double> *pDatas, string sFormula);
  double _export operation(double dOperande, double dOperateur, string sOperation);
  bool   _export areConsecutiveLogicalCharCompatible(char cPrevious, char cNext);
  bool   _export isLogicalBlock(string sBlock);
  bool   _export isComparisonOperator(string sCandidateOperator);
  string _export getComparisonUnit(string sLValue, string sRValue);
  string _export getPathUnit(string sPath);
  string _export replaceVarByAlias(string *psFormula, map<string, string> *paAliases, size_t iVarStartPos, string sVar, string sNameInit = string("var"));

	//------------------------------------------------------------------------
  // decomposer une chaine et mettre ses elements dans un vecteur
	//------------------------------------------------------------------------
  void   _export DecomposeChaine(string* pChaine, ClasseStringVector* pVect, string separateur = "|", string sQualifiers = "");
  void   _export splitString(string sModel, string *psLeft, string *psRight, char separator = '|');
  void   _export getQualifiers(string sModel, string &sResult, string &sQualifier, string sQualifiersList);

  // int 	 _export donne_age(char *datex, char *dateNaiss);
  int 	 _export donne_age(string sDatex, string sDateNaiss);
	// int 	 _export donne_age_mois(char *datex, char *dateNaiss);
  int 	 _export donne_age_mois(string sDatex, string sDateNaiss);
  int 	 _export get_age(string *datex, string *dateNaiss);
	int 	 _export get_age_month(string *datex, string *dateNaiss);

	void 	 _export donne_intitule_patient(string *intitule, int age, bool bFeminin, bool bShort, string sLang);
	void 	 _export donne_intitule_age(string *intitule, char *datex, char *dateNaiss);

	void 	 _export decode_date(string *resultat, char *date);

  string _export texteHtml(string texte);
  string _export texteCourant(string textHtml);

  string _export texteWebLink(string texte);
  string _export texteCourantFromWebLink(string textWL);

  void   _export texteForEditControl(string* pTexte);

  // creation nom sans doublons
  string _export nomSansDoublons(string pathRef, string nom, string ext);

  //
  // Fonction de lecture / ecriture dans les fichiers dat
  //
  class _export ParamArray
  {
  	public:

    	vector<string*> aNomParam;
      vector<string*> aValParam;

      ParamArray();
      ~ParamArray();

      ParamArray(ParamArray& src);
      ParamArray& operator=(ParamArray src);

      void   vider();
      void   PushParam(string sNom, string sVal);
      string ValParam(string sParam);
  };

  bool  _export lireParam(string sUtil, string sFichierDat, ParamArray* pParamArray);
  bool  _export lireParam(string sUtil, string sFichierDat, string sParam, string& sResult);
  bool  _export ecrireParam(string sUtil, string sFichierDat, string sParam, string sValeur);

  // Fonction qui parse une string pour recuperer les coordonnees d'une fenetre
  void  _export getWindowPosit(string sPosition, int& X, int& Y, int& W, int& H, string* psTaille = 0);

  WORD   _export getVirtualKeyFromString(string sVK);
  string _export getStringFromVirtualKey(WORD key);

  char   _export URL_ScanFor(string *psBuff, char cTarget, size_t &x);
  int    _export ParseUrlQuery(string *psBuff, map<string, string> *pQueryParts);
  void   _export DecodeURL(string& sBuffer);

#else

	unsigned int  DonneNomFichier(const char *ammorce, const char* suffixe,
									  const char *chemin, char** nomLibre);
	unsigned int  DonneNumeroFichier(const char *ammorce, const char* suffixe,
										  const char *nomFichier, char* numeroFichier);

	GRAPHELEMTYPE getGraphElementTypeFromID(string sElementID);
	string        getPatientIDFromID(string sAnyID);
	string        getTreeIDFromID(string sAnyID);
	string        getNodeIDFromID(string sAnyID);

  string        getRegularPath(string sMonoSeparatorPath, char cPath, char cIntranode);

	void   strip(string& s, STRIPTYPE n = stripBoth, char c = ' ');
	void   strip(string& s, string sM, STRIPTYPE n = stripBoth);
	void   incremente_code(string&);
	string nom_sans_doublons(string, string, string, string, string);
	void   numacar(char*, unsigned int, unsigned int);
  void 	 numacarbase(string*, unsigned int nbre, unsigned int size, unsigned int base);
	double arrondiInt(double dNbre);
	int 	 dtoi(double dNbre);
	int 	 donneDigit(char nbre);
	char 	 donneChar(int digit);
	int 	 ote_blancs(char*);
	int 	 egalite(char*, char*);
	char 	 pseumaj(char);
  void   pseumaj(char*);
  void   pseumaj(string*);
  string pseumaj(string);
  char 	 vraiemaj(char);
  void   vraiemaj(char*);
  void   vraiemaj(string*);
  void   firstmaj(char*);
  void   firstmaj(string*);
  string strpids(string);

  string getFormatedTime(string sRawTime, string sLang, string sTimeFormat);
  string getFormatedTime(string sRawTime, string sLang, string sDateFormat, string sHourFormat);
  string getFormatedDate(string sRawDate, string sLang, string sDateFormat);
  string getFormatedHour(string sRawHour, string sLang, string sHourFormat);

  string getRawTime(string sFormatedTime, string sLang, string sTimeFormat);
  string getRawTime(string sFormatedTime, string sLang, string sDateFormat, string sHourFormat);
  string getRawDate(string sFormatedDate, string sLang, string sDateFormat);
  string getRawHour(string sFormatedHour, string sLang, string sHourFormat);

  // void 	 donne_heure(char* pHeure);
	// void 	 donne_date_duJour(char* date);
  string donne_heure();
  string donne_date_duJour();
  void 	 get_current_date(string* sDate);
  // void	 donne_heure(char*, string&);
	// void 	 donne_date(char*, char*, string sLang);
  string donne_heure(const string);
  string donne_date(string, string sLang);
  void 	 donne_date_inverse(string, string&, string sLang);
	void 	 donne_date_claire(char*, char*, string sLang);
	void 	 donne_date_claire(char*, string*, string sLang);

  void	 donne_date_breve(string sDate, string* pMessage, string* pIntro, string sLang);
  void   donne_date_claire(string sDate, string* pMessage, string* pIntro, string sLang);
  bool   bissextile(int year);
  void   incremente_date(string& sDate);
  string donne_mois(string mois, string sLang);
	string donne_mois(int iMois, string sLang);

  string donne_deltaTemps(int iYears, int iMonths, int iDays);

	void	 donne_somme_courte(char*, char*, string sLang);

	void 	 long_to_hex(unsigned long, char*);
	void 	 hex_to_long(char*, unsigned long*);
  void 	 char_to_hex(unsigned char, char*);
	void 	 hex_to_char(char*, unsigned char*);

	void 	 format_date(char*);
	void 	 carcat(char*, char);
	
	double StringToDouble(string sValeur);
	string DoubleToString(double* pDouble, int iEntier, int iDecimal);
	int    StringToInt(string sValeur);
  std::string IntToString(int val);

  string getEpisodusNumValueFromRawCapturedNum(string sRawNum, string sLang = "");
  string getEpisodusDateValueFromRawCapturedDate(string sRawNum, string sLang = "");

	void	 SetInternational(void);
	BOOL   ptrInit(char **strPtr, unsigned int length, char car = ' ');
	BOOL   ptrInit(char **strPtr, const char *strInit);
	BOOL   ptrInit(unsigned char **strPtr, const unsigned char *strInit);

  int 	 error(const char *texte, ERRORTYPE gravite);

  bool   getModeFileErr();
  void   setModeFileErr(bool bMode);
  void   fileError(string sError);

  //********************************************************************
  // vecteur de string
  //********************************************************************
	class classString
	{
  	public:

      classString(string sChaine, int Colonne = -1, bool Coche = false, string sQualif = string(""));
      classString(const classString& src);
      classString& operator=(classString src);

      void   setColumn(int iC)       { colonne    = iC; }
      void   setChecked(bool bC)     { coche      = bC; }
      void   setItem(string sI)      { sItem      = sI; }
      void   setQualifier(string sQ) { sQualifier = sQ; }

      int    getColumn()    { return colonne; }
      bool   getChecked()   { return coche; }
      string getItem()      { return sItem; }
      string getQualifier() { return sQualifier; }

    protected:

      int     colonne;
      bool    coche;
      string  sItem;
      string  sQualifier;
  };

	typedef vector<classString*> vectclassString;
	typedef vectclassString::iterator iterClassString;

  class ClasseStringVector : public vectclassString
	{
  	public:
    	ClasseStringVector();
      ~ClasseStringVector();
      ClasseStringVector(ClasseStringVector& src);
      // Surcharge de l'operateur d'affectation
      ClasseStringVector& operator=(ClasseStringVector src);
      void vider();
  };

	//------------------------------------------------------------------------
  // decomposer une chaine et mettre ses elements dans un vecteur
	//------------------------------------------------------------------------
	void  DecomposeChaine(string* pChaine, ClasseStringVector* pVect, string separateur = "|", string sQualifiers = "");
  void  splitString(string sModel, string *psLeft, string *psRight, char separator = '|');
  void  getQualifiers(string sModel, string &sResult, string &sQualifier, string sQualifiersList);

  //vecteur de string

	typedef vector<string*> EquiItem;
	typedef EquiItem::iterator IterString;
	class  VectString : public EquiItem
	{
		public:

			VectString();
			~VectString();
			VectString(VectString& src);

      bool contains(string sModele);

      // Surcharge de l'operateur d'affectation
			VectString& operator=(VectString src);
			void vider();
	};

  bool   isLogicalFormulaValid(string sFormula, string *psErrorMsg, bool bPureLogical);
  bool   isLogicalBlockValid(string sFormula, string *psErrorMsg);
  void   parseLogicalBlocks(string &sFormula, map<string, string> *paBlocks);
  bool   parseComparisonBlock(string sFormula, string &sLValue, string &sOperator, string &sRValue, string *psErrorMsg, bool bSingleBlockAccepted);
  int    evaluate(map<string, bool> *pDatas, string sFormula, size_t& cc);
  bool   compare(double dLValue, string sOperator, double dRValue);
  double calculate(map<string, double> *pDatas, string sFormula);
  double operation(double dOperande, double dOperateur, string sOperation);
  bool   areConsecutiveLogicalCharCompatible(char cPrevious, char cNext);
  bool   isLogicalBlock(string sBlock);
  bool   isComparisonOperator(string sCandidateOperator);
  string getComparisonUnit(string sLValue, string sRValue);
  string getPathUnit(string sPath);
  string replaceVarByAlias(string *psFormula, map<string, string> *paAliases, size_t iVarStartPos, string sVar, string sNameInit = string("var"));

	// int 	 donne_age(char *datex, char *dateNaiss);
	// int 	 donne_age_mois(char *datex, char *dateNaiss);
  int 	 donne_age(string sDatex, string sDateNaiss);
  int 	 donne_age_mois(string sDatex, string sDateNaiss);
  int 	 get_age(string *datex, string *dateNaiss);
	int 	 get_age_month(string *datex, string *dateNaiss);

	void 	 donne_intitule_patient(string *intitule, int age, bool bFeminin, bool bShort, string sLang);
	void 	 donne_intitule_age(string *intitule, char *datex, char *dateNaiss);

	void 	 decode_date(string *resultat, char *date);

  string texteHtml(string texte);
  string texteCourant(string textHtml);

  string texteWebLink(string texte);
  string texteCourantFromWebLink(string textWL);

  void   texteForEditControl(string* pTexte);

  // creation nom sans doublons
  string nomSansDoublons(string pathRef, string nom, string ext);

  //
  // Fonction de lecture / ecriture dans les fichiers dat
  //
  class ParamArray
  {
  	public:

    	vector<string> aNomParam;
      vector<string> aValParam;

      ParamArray();
      ~ParamArray();

      ParamArray(ParamArray& src);
      ParamArray& operator=(ParamArray src);

      void   vider();
      void   PushParam(string sNom, string sVal);
      string ValParam(string sParam);
  };

  bool   lireParam(string sUtil, string sFichierDat, ParamArray* pParamArray);
  bool   lireParam(string sUtil, string sFichierDat, string sParam, string& sResult);
  bool   ecrireParam(string sUtil, string sFichierDat, string sParam, string sValeur);

  // Fonction qui parse une string pour recuperer les coordonnees d'une fenetre
  void   getWindowPosit(string sPosition, int& X, int& Y, int& W, int& H, string* psTaille = 0);
  char   URL_ScanFor(string *psBuff, char cTarget, size_t &x);
  int    ParseUrlQuery(string *psBuff, map<string, string> *pQueryParts);
  void   DecodeURL(string& sBuffer);

#endif

#endif 		// __NSUTILDLL_H

