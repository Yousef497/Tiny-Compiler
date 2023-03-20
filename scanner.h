#ifndef SCANNER_H
#define SCANNER_H
#include <iostream>
#include <string>
#include <fstream>
#include <QString>
#include <QDebug>
#include <QFile>
#include "utils.h"
using namespace std;
enum DFAstates {
    START,
    INCOMMENT,
    INNUM,
    INID,
    INASSIGN,
    OP,
    DONE
};
class Scanner
{
private:
    //set START to be intial state
    DFAstates state = START;
    string RES_WORDS[8] = { "if","then","else","end","repeat","until","read","write" };
public:
    void clear_scanner();
    Scanner();
    QString getToken(string);
    QString getStringFile(QString directory);
    bool isDigit(char);
    bool isLetter(char);
    bool isSpace(char);
    bool isSymbol(char);
    bool isOp(char);
};
#endif // SCANNER_H
