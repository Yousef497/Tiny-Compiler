#ifndef PARSER_H
#define PARSER_H
#include <iostream>
#include <stdio.h>
#include <string>
#include <fstream>
#include "QString"
#include "scanner.h"
using namespace std;
enum stmtTypes {IF, REPEAT, ASSIGN, READ, WRITE, ERROR};
//Note : x , y , x1 , y1 ,x2 , y2 are cordinates 
typedef struct{
    bool Rect; // to mark the rectangle Nodes
    int x;
    int y;
    string text;
    int parent;
}Node;
typedef struct{
    bool isFriend;
    int x1;
    int y1;
    int x2;
    int y2;
}Edge;
typedef struct{
    bool isFriend;
    int x;
    int y;
}parentNode;
class Parser
{
private:
    //int no_of_rstmt;
    int error=0;  // Error starts with 0
    QStringList tokens; // list of strings
    int token_index=0; // token_index starts with 0
    string token;       // string--> token
    ifstream file;
    QVector<Node>Nodes; // Nodes
    QVector<Edge>Edges; // Edges
    Edge *currentEdge = NULL;
    int currentx=0; // x-cordinate
    int currenty=0; // y-cordinate
    QVector<parentNode> parentNodes; // Parent Nodes
public:
    Parser();
    stmtTypes getStmtType(string s);
    QString getStringFile(QString directory); // to read file to make parsing without scanner
    void match(string s) ;
    void program();
    void stmt_seq();
    void stmt();
    void if_stmt();
    void repeat_stmt();
    void assign_stmt();
    void read_stmt();
    void write_stmt();
    void exp();
    void comparison_op();
    void simple_exp();
    void addop();
    void term();
    void mulop();
    void factor(int x,int y);
    // To draw syntax Tree
    void pre_update_edge();
    void post_update_edge(bool isFriend);
    int scan_out(QString s2);
    void process_edges();
    QString input_preprocessing(QString s2);
    QVector<Node> get_nodes();
    QVector<Edge> get_edges();
    void setTokenIndex(int x);
    void clear_parser(void);
    ////////////////////////
    int no_of_repeat_stmt(void);
};

#endif // PARSER_H
