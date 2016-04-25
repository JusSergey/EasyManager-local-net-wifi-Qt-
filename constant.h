#ifndef CONSTANT
#define CONSTANT
#include <QString>

#define linos  1
#define andros 0
#define windos 2
#define plfrm  andros

static const QString c_listWidgetGet = "Получить файлы...";
static const int buttonH = 20;

enum Command : qint8  {

    RESEND = -64,
    CLIENT_INFO,
    COUNT_FILES,
    DIR_NAME,
    FILE_DATA,
    OPEN_NEXT_FILE,
    FILE_NAME,
    CLIENT_NAME,
    GET_PACKAGE,
    CONNECTED,
    FINISH,
    FILE_MANAGER,
    MANAGER,
    TERMINAL,
    TERMINAL_COMMAND,
    DISCONNECT,
    LIST_FILE_DIR,
    GET_FILES

};

enum ENMouse : qint8  {

    MouseEvent = -96,
    RClick,
    RDoubleClick,
    LClick,
    LDoubleClick,
    Move,
    Select,
    LPress,
    LRelease

};

enum CManager : qint8 {
    Images = -16,
    GetScreen
};

#endif // CONSTANT

