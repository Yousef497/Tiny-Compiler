#include "parser.h"
#include "QDebug"
#include "QFile"
#include "scanner.h"
Parser::Parser()
{
}
// this function it's not necessary here
QString Parser::getStringFile(QString directory)
{
    QFile file(directory);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
    }
    QString input;
    QTextStream in(&file);
    QString line = in.readLine();
    while (!line.isNull())
    {
        input += line;
        line = in.readLine();
        input += "\n";
    }
    return (input);
}
// take the string from scanner and store it into list w.r.t /n (NewLine)
// and remove any space in each line
// Note : the output is integer w.r.t Error variable which starts with 0
// it must be change the name
int Parser::scan_out(QString s2)
{

    tokens = s2.split(QString("\n"));
    for (int i = 0; i < tokens.length(); i++)
        tokens[i].remove(" ");
    token = tokens[token_index].toStdString();
    token_index++;
    if((tokens[tokens.length()-2].toStdString())==";,SEMICOLON"){
        error=1;
        return error;
    }else{
        program();
        // we matched all statements
        if(token_index >=tokens.length()-1){
            process_edges();
            return error;
        }
        // otherwise, raise the error to not find ';'
        else{
            error =4;
            return error;
        }
    }

}
/////
int Parser::no_of_repeat_stmt(void)
{
    string s ="";
    int counter=0;
    for (int i = 0; i < tokens.length(); i++){

    s = tokens[i].toStdString();
    s = s.substr(s.find(",") + 1, s.length() - s.find(",") + 1);
    if (s == "IF")
       counter++ ;
    }
    return counter;
}
///
stmtTypes Parser::getStmtType(string s)
{
    // this if-stmt to handle the ';' after last stmt
        s = s.substr(s.find(",") + 1, s.length() - s.find(",") + 1);
        qDebug() << QString::fromStdString(s) << "\n";
        if (s == "IF")
            return IF;
        if (s == "IDENTIFIER")
            return ASSIGN;
        if (s == "READ")
            return READ;
        if (s == "WRITE")
            return WRITE;
        if (s == "REPEAT")
            return REPEAT;
        else
        {
            return ERROR;
        }
}
void Parser::match(string s)
{
    if (s == token)
    {
        if(token_index >tokens.length()-1){
            error =3;
        }
        else{
            token = tokens[token_index].toStdString();
            token_index++;
        }

    }

    else
    {
        error = 2;

    }
}
// program -> stmt-seq
void Parser::program()
{
    stmt_seq();
    cout << endl
         << "-- Program found" << endl;
}
// stmt-seq -> stmt {; stmt}
void Parser::stmt_seq()
{
    stmt();
    // here we need to make it easy more
    // 1- Advance last-stmt
    // 2- match more than one word as id
    // 3- match semicolon and not find stmt

    while (token == ";,SEMICOLON")
    {
        match(";,SEMICOLON");
        stmt();
    }
}
// stmt -> if-stmt | repeat-stmt | assign-stmt | read-stmt | write-stmt
void Parser::stmt()
{
    switch (getStmtType(token))
    {
    case READ:
        read_stmt();
        break;
    case WRITE:
        write_stmt();
        break;
    case IF:
        if_stmt();
        break;
    case REPEAT:
        repeat_stmt();
        break;
    case ASSIGN:
        assign_stmt();
        break;
    case ERROR:
        error = 2;
        //return 0 ;
        break;
    default:
        break;
    }
}

// if-stmt -> if exp then stmt-seq [else stmt-seq] end
void Parser::if_stmt()
{
    match("if,IF");
    pre_update_edge();
    Nodes.append({true, currentx, currenty, "if"});
    post_update_edge(false);

    currenty += 100; // child y
    exp();
    currenty -= 100; // parent y
    match("then,THEN");

    currentx += 150; // child2 x
    currenty += 100; // child y
    stmt_seq();
    currenty -= 100; // parent y

    if (token == "else,ELSE"){
        match("else,ELSE");
        //currentx += 150; // child2 x
        currenty += 90; // child y
        stmt_seq();
        currenty -= 90; // parent y

    }


    match("end,END");
    currentx += 150; // friend x;
}

// repeat-stmt -> repeat stmt-seq until exp
void Parser::repeat_stmt()
{
    match("repeat,REPEAT");
    pre_update_edge();
    Nodes.append({true, currentx, currenty, "repeat"});
    post_update_edge(false);
    currenty += 100; // child y
    stmt_seq();
    currenty -= 100; // parent y
    match("until,UNTIL");

    currenty += 100; // child y
    exp();
    currenty -= 100; // parent y
    cout << "- repeat found" << endl;
    currentx += 150; // friend x
}

// assign-stmt -> identifier := exp
void Parser::assign_stmt()
{

    pre_update_edge();
    Nodes.append({true, currentx, currenty, "assign\n   " + token.substr(0, token.find(","))});
    post_update_edge(false);
    match(token.substr(0, token.find(",")) + ",IDENTIFIER");
    match(":=,ASSIGN");
    currenty += 100; // child y
    if(token_index > tokens.length()-1){
        error =3;
    }
    else{
        exp();
        currenty -= 100; // parent y
        currentx += 150; // friend x;
    }

}

// read-stmt -> read identifier
void Parser::read_stmt()
{
    pre_update_edge();
    post_update_edge(true);
    match("read,READ");
    Nodes.append({true, currentx, currenty, "read\n    " + token.substr(0, token.find(","))});
    token.erase(0, token.find(",") + 1);
    match("IDENTIFIER");
    currentx += 150; // friend x
}

// write-stmt -> write exp
void Parser::write_stmt()
{
    pre_update_edge();
    Nodes.append({true, currentx, currenty, "write"});
    post_update_edge(false);
    match("write,WRITE");
    currenty += 100; // child y
    exp();
    currenty -= 100; // parent y
    currentx += 150; // friend x;
}
// exp -> simple-exp [comparison-op simple-exp]
void Parser::exp()
{
    if(token_index > tokens.length()-1){
        error =3;
    }
    else{
        if (tokens[token_index] == "<,LESSTHAN" || tokens[token_index] == "=,EQUAL")
            currenty += 100;
        simple_exp();
        if (token == "<,LESSTHAN" || token == "=,EQUAL")
        {
            currenty -= 100; // parenty
            comparison_op();
            currenty += 100; // child y
            currentx += 150; // child x
            simple_exp();
            currenty -= 100; // parent y
        }
    }

}

// comparison-op -> < | =
void Parser::comparison_op()
{
    if (token == "<,LESSTHAN")
    {
        pre_update_edge();
        Nodes.append({false, currentx, currenty, "op\n(<)"});
        post_update_edge(false);
        match("<,LESSTHAN");
    }
    if (token == "=,EQUAL")
    {
        pre_update_edge();
        Nodes.append({false, currentx, currenty, "op\n(=)"});
        post_update_edge(false);
        match("=,EQUAL");
    }
}

// simple-exp -> term { addop term }
void Parser::simple_exp()
{
    if (tokens[token_index] == "+,PLUS" || tokens[token_index] == "-,MINUS")
        currenty += 100;
    term();
    while (token == "+,PLUS" || token == "-,MINUS")
    {
        currenty -= 100; // parenty
        addop();
        currenty += 100; // child y
        currentx += 150; // child x
        term();

        currenty -= 100; // parent y
    }
}
// addop -> + | -
void Parser::addop()
{
    if (token == "+,PLUS")
    {
        pre_update_edge();
        Nodes.append({false, currentx, currenty, "op\n(+)"});
        post_update_edge(false);
        match("+,PLUS");
    }
    if (token == "-,MINUS")
    {
        pre_update_edge();
        Nodes.append({false, currentx, currenty, "op\n(-)"});
        post_update_edge(false);
        match("-,MINUS");
    }
}

// term -> factor { mulop factor }
void Parser::term()
{
    if (tokens[token_index] == "*,MULT" || tokens[token_index] == "/,DIV")
        currenty += 100;
    factor(currentx, currenty - 100);
    while (token == "*,MULT" || token == "/,DIV")
    {

        currenty -= 100; // parenty
        mulop();
        currenty += 100; // child y
        currentx += 150; // child x
        factor(currentx - 150, currenty - 100);
        currenty -= 100; // parent y
    }
}
// mulop -> * | /
void Parser::mulop()
{
    if (token == "*,MULT")
    {
        pre_update_edge();
        Nodes.append({false, currentx, currenty, "op\n(*)"});
        post_update_edge(false);
        match("*,MULT");
    }
    if (token == "/,DIV")
    {
        pre_update_edge();
        Nodes.append({false, currentx, currenty, "op\n(/)"});
        post_update_edge(false);
        match("/,DIV");
    }
}

// factor -> (exp) | number | identifier
void Parser::factor(int x, int y)
{

    if (token == "(,OPENBRACKET")
    {
        match("(,OPENBRACKET");
        exp();
        match("),CLOSEDBRACKET");
    }
    else if (token.substr(token.find(",") + 1, token.length() - 1) == "NUMBER")
    {
        pre_update_edge();
        Nodes.append({false, currentx, currenty, "NUMBER\n      " + token.substr(0, token.find(","))});
        match(token.substr(0, token.find(",")) + ",NUMBER");
    }
    else if (token.substr(token.find(",") + 1, token.length() - token.find(",") + 1) == "IDENTIFIER")
    {
        pre_update_edge();
        Nodes.append({false, currentx, currenty, "IDENTIFIER\n      " + token.substr(0, token.find(","))});
        match(token.substr(0, token.find(",")) + ",IDENTIFIER");
    }
    else{

        error = 3;
    }
}

QString Parser::input_preprocessing(QString s2)
{

    return s2;
}
void Parser::process_edges()
{
    cout<<token_index<<endl;
    cout<<tokens.length()<<endl;
//    int x =no_of_repeat_stmt();
//    cout<<x<<endl;
//    cout<<"AAAAAAAAAAAAAAAAAAAAAAAAAAAAA"<<endl;
//    for (int i = 0; i < Nodes.length(); i++){
//        cout<<Nodes[i].text<<endl;
//    }
//    cout<<"AAAAAAAAAAAAAAAAAAAAAAAAAAAAA"<<endl;
    int len = Nodes.length();
    for (int i = 0; i < len; i++)
    {
        bool flag = false;
        int minIndex = 0;
        bool repeat_flag = true;
        bool if_else_flag = true;
        for (int j = 0; j < len; j++)
        {
            if (j != i)
            {
                if (Nodes[i].Rect && Nodes[j].Rect && Nodes[i].y == Nodes[j].y && Nodes[i].x > Nodes[j].x && Nodes[i].text.substr(0, 2) != "op" && Nodes[j].text.substr(0, 2) != "op" )
                {
                    if (!flag)
                    {

                     if ( Nodes[j].text != "write"   && Nodes[i].text.substr(0,6) != "assign" )
                      {

                             cout<<"\n\n\n Node[i] "+Nodes[i].text  + " \nNode[j] " +Nodes[j].text + " \n " + Nodes[i].text.substr(0, 2)<<endl;
                        minIndex = j;
                       flag = true;

                      }
                     if ( Nodes[i].text.substr(0,6) == "assign"  && Nodes[j].text.substr(0,6) == "assign" )
                      {
                         if (j<i && j> i-6){
                      minIndex = j;
                      Edges.append({true, Nodes[minIndex].x, Nodes[minIndex].y, Nodes[i].x, Nodes[i].y});
                         }

                     }

                    }
                    if ((Nodes[i].x - Nodes[j].x) < (Nodes[i].x - Nodes[minIndex].x))
                        minIndex = j;
                }
                if (!Nodes[i].Rect && !Nodes[j].Rect && Nodes[i].x == Nodes[j].x && Nodes[i].y - Nodes[j].y == 100 )
                    Edges.append({false, Nodes[j].x, Nodes[j].y, Nodes[i].x, Nodes[i].y});
                if (Nodes[i].text == "if" && Nodes[j].x - Nodes[i].x == 300 && Nodes[j].y - Nodes[i].y == 100){
                Edges.append({false, Nodes[i].x, Nodes[i].y, Nodes[j].x, Nodes[j].y});
                cout<<"hello"<<endl;
                }

                if (Nodes[i].text == "repeat" && Nodes[j].y - Nodes[i].y == 100 && Nodes[j].x > Nodes[i].x && !Nodes[j].Rect && Nodes[j].text.substr(0, 2) == "op")
                {

                    if(repeat_flag){
                    Edges.append({false, Nodes[i].x, Nodes[i].y, Nodes[j].x, Nodes[j].y});
                    repeat_flag=false;
                    }

                }
                if (Nodes[i].text == "if" && Nodes[j].y - Nodes[i].y == 90 && Nodes[j].x > Nodes[i].x && Nodes[j].Rect)
                {

                    if(if_else_flag){
                    Edges.append({false, Nodes[i].x, Nodes[i].y, Nodes[j].x, Nodes[j].y});
                    if_else_flag=false;
                    }

                }
            }


        }

        if (flag)
            Edges.append({true, Nodes[minIndex].x, Nodes[minIndex].y, Nodes[i].x, Nodes[i].y});

    }
    for (int i = 0; i < Edges.length(); i++)
    {
        if (Edges[i].y2 - Edges[i].y1 > 100)
            Edges[i].y2 -= 100;
    }

}

void Parser::pre_update_edge()
{
    if (currentEdge != NULL)
    {
        currentEdge->x2 = currentx;
        currentEdge->y2 = currenty;
        Edges.append(*currentEdge);
        currentEdge = NULL;
    }

}

void Parser::post_update_edge(bool isFriend)
{
    currentEdge = new Edge;
    currentEdge->isFriend = isFriend;
    currentEdge->x1 = currentx;
    currentEdge->y1 = currenty;
}

QVector<Node> Parser::get_nodes()
{
    return Nodes;
}
QVector<Edge> Parser::get_edges()
{
    return Edges;
}

void Parser::setTokenIndex(int x)
{
    token_index = x;
    currentx = 0;
    currenty = 0;
    error = 0;
}

void Parser::clear_parser()
{
    Edges.clear();
    Nodes.clear();
}
