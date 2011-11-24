/*********************************************************************
*             DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE            *
*                     Version 2, December 2004                       *
*                                                                    *
*  Copyleft alkz                                                     *
*                                                                    *
*               DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE          *
*  TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION   *
*                                                                    *
*  0. You just DO WHAT THE FUCK YOU WANT TO.                         *
**********************************************************************
* Just a calculator with an expression parser.                       *
*********************************************************************/

#include <iostream>
#include <map>
#include <cctype>
#include <string>
#include <cstdlib>

#ifdef _WIN32
    #define cls()  system("cls")
#else
    #define cls()  system("clear")
#endif

using namespace std;

enum Token 
{
    NAME,          NUMBER,         END,          HELP,
    PLUS = '+',    MINUS = '-',    MUL = '*',    DIV = '/',
    RP = ')',      LP = '(',       EOL = ';',    ASSIGN = '=',
};


namespace Errors 
{
    struct division_by_0 {};
    struct Syntax {};
    struct parenthesis {};
}


namespace Parser
{
    double expr(bool);
    double term(bool);
    double prim(bool);

    ::Token get_token();
}

void
help()
{
    cls();
    cout << "Just type what the fuck you want to ^^.\n";
    getchar();
    getchar();
    cls();
}


Token current = EOL;
double numberValue;
string stringValue;

map<string, double> symbols;


int
main(int argc, char **argv)
{
    symbols["PI"] = 3.141592653589;
    int errors = 0;

    cls();

    cout << endl << "Interactive Calc('~': exit, 'h': help), type your expression: " << endl;

    while(cin)
    {
        try {
            Parser::get_token();
            if(current == END) break;
            if(current == EOL) continue;    // Another line
            if(current == HELP)  
            {
                help();
                cout << endl << "Interactive Calc('~': exit, 'h': help), type your expression: " << endl;
                continue;
            }
            
            cout << Parser::expr(false) << endl;    // Print the result
        } catch(Errors::division_by_0) { 
            cout << "Error: Division by 0.\n";
            errors++;
        } catch(Errors::Syntax) {
            cout << "Error: Syntax wrong.\n";
            errors++;
        } catch(Errors::parenthesis) {
            cout << "Error: Expected ).\n";
            errors++;
        }

    }

    return errors;
}


double
Parser::expr(bool get)
{
    double next = Parser::term(get);

    while(true)
    {
        switch(current)
        {
            case PLUS:
            {
                next += Parser::term(true);
                break;
            }

            case MINUS:
            {
                next -= Parser::term(true);
            }

            default:
                return next;
        }
    }
}


double
Parser::term(bool get)
{
    double next = Parser::prim(get);

    while(true)
    {
        switch(current)
        {
            case MUL:
            {
                next *= Parser::prim(true);
                break;
            }

            case DIV:
            {
                if(double second = Parser::prim(true))
                {
                    next /= second;
                    break;
                }
                else
                    throw Errors::division_by_0();
            }

            default:
                return next;
        }
    } 
}

double
Parser::prim(bool get)
{
    if(get) Parser::get_token();

    switch(current)
    {
        case NUMBER: 
        {
            double v = numberValue;
            Parser::get_token();
            return v;
        }

        case NAME:
        {
            double &value = symbols[stringValue];

            if(Parser::get_token() == ASSIGN) value = Parser::expr(true);
            return value;
        }

        case MINUS:
            return -Parser::prim(true);

        case LP:
        {
            double next = expr(true);

            if(current != RP) throw Errors::parenthesis();
            Parser::get_token();    // Remove )
            return next;
        }

        default:
            throw Errors::Syntax();
    }
}


Token 
Parser::get_token()
{
    char c;

    do
    {
        if(!cin.get(c)) return current = END;
    } while(c != '\n' && isspace(c));


    switch(c)
    {
        case '~':
            return current = END;

        case 'h':
            return current = HELP;

        case ';':
        case '\n':
            return current = EOL;

        case '+':
        case '-':
        case '*':
        case '/':
        case '=':
        case '(':
        case ')':
            return current = Token(c);
        
        case '0': case '1': case '2': case '3': 
        case '4': case '5': case '6': case '7':
        case '8': case '9':
        {
            cin.putback(c);
            cin >> numberValue;
            return current = NUMBER;
        }

        default:    // NAME, NAME = or error
        {
            if(isalpha(c))
            {
                stringValue = c;
                while(cin.get(c) && isalnum(c))
                    stringValue.push_back(c);
                cin.putback(c);
                return current = NAME;
            }
            throw Errors::Syntax();
            return current = EOL; 
        }
        
    }
}
