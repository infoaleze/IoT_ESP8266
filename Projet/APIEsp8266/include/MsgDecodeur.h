#ifndef MSGDECODEUR_H
#define MSGDECODEUR_H

#include <map>
#include <string>


// Code d'erreur

#define MSGDEC_OK               0x0000
#define MSGDEC_KO               0xFFFF


#define MSGDEC_INVALID_ARG      0x0010

#define MSGDEC_ALLREADY_EXIST   0x0100

#define MSGDEC_KEYWORD_NOTFOUND 0x0200

#define MSGDEC_CALLBACK_EXCEPTION 0x300

#define MSGDEC_BAD_MSG          0x1000




using namespace std;

// Définition du context
struct ctx
    {
        string keyword;
        int8_t param_count;
        string params[];
    };

// Définition du pointeur de fonction CallBack
typedef int (*pCallBackFnt) (ctx *);

typedef map<string, pCallBackFnt> _KeyWordMap;

// Définition de la classe de décodage
class MsgDecodeur
{
    public:
        MsgDecodeur();
        virtual ~MsgDecodeur();

        int AddKeyWord(string KeyWord, pCallBackFnt CallBackFnt);
        int ExecKeyWord(string KeyWord);

        int DecodeMsg(string Msg);

    protected:

        _KeyWordMap KeyWords;

    private:
};

#endif // MSGDECODEUR_H
