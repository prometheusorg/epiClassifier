#ifndef __NSBB_GLO_H
#define __NSBB_GLO_H

//
// Launching situation
//
enum SITUATION_TYPE { NSDLGFCT_CREATION = 0,
                      NSDLGFCT_REINIT,
                      NSDLGFCT_EXECUTE,
                      NSDLGFCT_POSTEXEC,
                      NSDLGFCT_PREFERME,
                      NSDLGFCT_FERMETURE,
                      NSDLGFCT_CTRL_KEY
                    };

//
// Enumération des types de fenêtres
//
enum WNDTYPE { isUndefined, isDlg, isGroup, isEdit, isBtn, isRadioBtn, isCaseACocher,
               isFunct, isOnglet, isTreeNode, isTreeWindow, isEditLexique,
               isEditLexiqueDerive, isEditNoLexique, isNSCSVue, isStatic,
               isEditDate, isEditDateHeure, isAdrListWindow,
               isHistoryListWindow, isComboClassif, isComboSemantique,
               isComboList, isCorListWindow, isHistoryValListWindow };

enum NODELINKDIRECTION {dirFleche = 1, dirEnvers};

#endif
