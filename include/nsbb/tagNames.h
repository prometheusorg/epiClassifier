#ifndef __TAGNAME_H__
# define __TAGNAME_H__

// Types de roles : passer une string pour le pilote
#  define PATIENT_ROLE          "P"
#  define USER_ROLE             "U"
#  define CORRESP_ROLE          "C"
#  define ADMIN_ROLE            "A"
#  define ALL_ROLES             "L"

// Types de passwd
#  define PASSWORD_MUST_CHANGE  "T"
#  define PASSWORD_FIXE         "F"
#  define USER_PRIVILEGE        "G"
#  define LOCKED_USER           "V"
#  define DESEABLED_USER        "D"

// Recherches
#  define FORWARD               "0"
#  define BACK                  "1"
#  define DOUBLE                "2"

#  define START_RESEARCH        "1"
#  define END_RESEARCH          "2"
#  define CONTAIN_RESEARCH      "3"
#  define EQUAL_RESEARCH        "0"

// Balises
# define FIRST_NAME             "firstName"
# define LAST_NAME              "lastName"
# define MAIDEN_NAME            "maidenName"
# define ADMINISTRATION_NAME    "administrationName"
# define BIRTHDATE              "birthdate"
# define APPOINTMENT_DATE       "appointmentDate"
# define STATUS                 "status"
# define CODE                   "code"
# define SEX                    "sex"
# define CIVILITY               "civility"
# define PROFESSION             "profession"
# define SPECIALITY             "speciality"
# define ORIENTATION            "orientation"
# define ACTIVE_JOB             "activeJob"
# define ACT_EXPERTISE          "actExpertise"
# define CITY                   "cityOfResidence"
# define CITY_PRO               "cityOfExercise"
# define TITLE                  "title"
# define TITLE_CODE             "titleCode"
# define PIDS                   "personID"
# define LOCKED                 "lock"
# define TO_UNLOCK              "locked"
# define OPERATION_TYPE         "operationType"
# define LOGIN                  "login"
# define ROLE                   "role"
# define PASSWDTYPE             "typePassword"
# define PASSWORD               "password"
# define OPERATOR               "operator"
# define STARTDATE              "startDate"
# define DAYSNB                 "daysNB"
# define CONSOLE                "console"
# define INSTANCE               "instance"
# define USERTYPE               "userType"
# define PERSONTYPE             "personType"
# define CORRESPTYPE            "correspType"
# define PERSON                 "person"
# define ROOTDOC                "rootDoc"
# define STEP                   "step"              //le no des elementes à charger
# define NEXT                   "next"              //suite de la liste
# define NEWLIST                "newList"           //réinitialiser la liste
# define LINK                   "link"
# define OBJECT                 "object"
# define OIDS                   "objectID"
# define IPP                    "IPP"
# define IEP                    "IEP"
# define MERGE					        "fusion"
# define NEWIPP									"IPPfusioned"
# define OLDIPP									"oldIPP"
# define OPERATOR_ID            "operatorID"
# define ADELI_ID               "adeliId"
# define LIDEN                  "LIDEN"
# define TEAM_LNOMA             "_LEQUM_LNOMA"
# define LOCALTEAM              "_LEQUM_USERV"
# define GRPE_LNOMA             "_LGRPE_LNOMA"
# define ETAB_LNOMA             "_UETAB_LNOMA"
# define ETAB_LIDEG             "_UETAB_LIDEG"
# define SERV_LNOMA             "_USERV_LNOMA"
# define SERV_LIDEG             "_USERV_LIDEG"
# define UNIF_LNOMA             "_LUNIF_LNOMA"
# define UNIF_LIDEG             "_LUNIF_LIDEG"
# define DIRECTION              "direction"
# define RESEARCH               "advancedResearch"
# define FUNCTION               "function"
# define COLLECTIVE             "collective"
# define LOCAL                  "local"
# define TRAIT					        "trait"

#define	AGENT										"agent"

#endif // !__TAGNAME_H__
