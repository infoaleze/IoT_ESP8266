#include <cstddef>

#include "MsgDecodeur.h"




/******************************************************************************
** Constructeur.                                                             **
**---------------------------------------------------------------------------**
** 22/03/2016 | Yoann Darche | Création.                                     **
******************************************************************************/
MsgDecodeur::MsgDecodeur()
{
    // Vide le tableau associatif
    KeyWords.clear();
}

/******************************************************************************
** Destructeur.                                                              **
**---------------------------------------------------------------------------**
** 22/03/2016 | Yoann Darche | Création.                                     **
******************************************************************************/
MsgDecodeur::~MsgDecodeur()
{
    //dtor
}

/******************************************************************************
** Ajoute un nouveau mot clef.                                               **
**---------------------------------------------------------------------------**
** 22/03/2016 | Yoann Darche | Création.                                     **
******************************************************************************/
int MsgDecodeur::AddKeyWord(string KeyWord, pCallBackFnt CallBackFnt)
{

    // Vérification || (pCallBackFnt == nullptr)
    if((KeyWord == "") || (CallBackFnt == NULL) ) { return(MSGDEC_INVALID_ARG); }

    // Vérification que le mot clef n'existe pas déjà
    _KeyWordMap::iterator it = KeyWords.find(KeyWord);
    if(it != KeyWords.end()) {
        // Le mot clef existe déjà
        return(MSGDEC_ALLREADY_EXIST);
    }

    // Ajout du mot clef et de la CallBack
    KeyWords[KeyWord] = CallBackFnt;

    return(MSGDEC_OK);

}

/******************************************************************************
** Execute une fonction par rapport au mot clef.                             **
**---------------------------------------------------------------------------**
** 22/03/2016 | Yoann Darche | Création.                                     **
******************************************************************************/
int MsgDecodeur::ExecKeyWord(string KeyWord)
{
    // Validation
    if(KeyWord=="") { return(MSGDEC_INVALID_ARG); }

    // Recherche de la fonction
    _KeyWordMap::iterator it = KeyWords.find(KeyWord);
    if(it == KeyWords.end()) {
        // Le mot clef existe déjà
        return(MSGDEC_KEYWORD_NOTFOUND);
    }

    // Création du context
    ctx _ctx;
    _ctx.keyword = KeyWord;
    _ctx.param_count = 0;

    // Appel de la fonction
    try
    {
        return(it->second(&_ctx));
    }
    catch(...)
    {
        return(MSGDEC_CALLBACK_EXCEPTION);
    }
}

/******************************************************************************
** Decode un message de la forme:                                            **
**  #<nom opé>[:P1...:Pn];       ==> Commande Admin                          **
**  $<nom opé>[:P1...:Pn];       ==> Commande RunTime                        **
**  *<tag>:<nom opé>[:P1...:Pn]; ==> Commande Synchrone                      **
**---------------------------------------------------------------------------**
** 22/03/2016 | Yoann Darche | Création.                                     **
******************************************************************************/
int MsgDecodeur::DecodeMsg(string Msg)
{

    // Détermination de la nature de la commande

    switch(Msg[0]) {
    case '*':
        break;
    case '$':
        break;
    case '#':
        break;

    default:
        return(MSGDEC_BAD_MSG);
    }



}
