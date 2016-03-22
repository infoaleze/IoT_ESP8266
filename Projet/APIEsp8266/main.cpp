#include <iostream>

#include "include\MsgDecodeur.h"

using namespace std;

int MyTotoFnt (ctx *Context);


int main()
{
    int Res = 0;

    cout << "======================================================" << endl;
    cout << "Programmme de Test de la librairie decodeur de message" << endl;
    cout << "pour le module ESP8266." << endl;
    cout << "------------------------------------------------------" << endl;
    cout << "22/03/2016 - V1.0 by Yoann Darche" << endl;
    cout << "======================================================" << endl << endl;

    cout << "[i] Création de la classe MsgDecodeur" << endl;
    MsgDecodeur TstMsg;

    cout << "[i] Ajout du mot clef \"toto\"" ;
    Res = TstMsg.AddKeyWord("toto",&MyTotoFnt);
    cout << " // Retour:" << Res << endl;

    cout << "[i] Ajout du mot clef \"toto\"";
    Res = TstMsg.AddKeyWord("toto",&MyTotoFnt);
    cout << " // Retour:" << Res << endl;

    cout << "[i] Decodage du msg 'toto'..." << endl;
    Res = TstMsg.ExecKeyWord("toto");
    cout << "[i] ... Retour:" << Res << endl;



    return 0;
}

/******************************************************************************
** Fontion de test CallBack A                                                **
******************************************************************************/
int MyTotoFnt (ctx *Context)
{

    cout << "..[i] MyTotoFnt appelée !" << endl;

    return(0);

}
