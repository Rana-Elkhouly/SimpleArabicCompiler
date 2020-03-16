#include <bits/stdc++.h>
#include <fstream>
using namespace std;


string line, subtok, token;
vector<int> parse;
ifstream mycode("codeToCompile.txt");
ofstream lexical("lex.txt");
ofstream output("output.txt");
ofstream parsing("parsing.txt");
int next_token = 0;
vector<string> tokens;
vector<string> variable;
#define inc next_token++
enum token_sy
{
    rl_Dtype_sy,
    fl_Dtype_sy,
    lparn_sy,
    rparn_sy,
    lbrace,
    rbrace,
    cmp_op_sy,
    math_op_sy,
    int_sy,
    real_sy,
    idntifir_sy,
    error_sy,
    end_source_sy,
    delimter,
    if_sy,
    while_sy,
    cnd,
    semi_sy,
    comma_sy,
    equal_sy,
    char_sy,
    string_sy,
    bool_sy
};
//l-> real|int| id;
bool l()
{
    if (parse[next_token] == real_sy || parse[next_token] == int_sy || parse[next_token] == idntifir_sy)
    {
        inc;

        return true;
    }

    return false;
}
//true_condition->l.cmp_op.l;
bool true_condition()
{   parsing<<"(l.cmp_op.l){statement} ->"<<endl;
    if (l())
    {   parsing<<"("<<tokens[next_token-1]<< ".cmp_op.l){statement} ->"<<endl;
        if (parse[next_token] == cmp_op_sy)
        {   parsing<<"("<<tokens[next_token-1]<< tokens[next_token]<<".l){statement} ->"<<endl;
            inc;
            if (l())
            {   parsing<<"("<<tokens[next_token-3]<< tokens[next_token-2]
            <<tokens[next_token-1]<<"){statement} ->"<<endl;
                return true;
            }
        }
    }
    return false;
}
//statement->id=l;|id=l op l;
bool statements()
{
    if (parse[next_token] == idntifir_sy)
    {  
        inc;
        if (parse[next_token] == equal_sy)
        {
            inc;
            if (l())
            {
                if (parse[next_token] == math_op_sy)
                {   
                    inc;
                    if (l())
                    {   
                        if (parse[next_token] == semi_sy)
                        {
                            inc;
                            return true;
                        }
                    }
                }
                else if (parse[next_token] == semi_sy)
                {    parsing<<"("<<tokens[next_token-7]<< tokens[next_token-6]
            <<tokens[next_token-5]<<"){id=l} ->"<<endl;
            parsing<<"("<<tokens[next_token-7]<< tokens[next_token-6]
            <<tokens[next_token-5]<<"){"<<tokens[next_token-3] <<"=l} ->"<<endl;
            parsing<<"("<<tokens[next_token-7]<< tokens[next_token-6]
            <<tokens[next_token-5]<<"){"<<tokens[next_token-3] <<"="<<tokens[next_token-1]<<"} ->"<<endl;

                    inc;
                    cout << "ishta" << endl;
                    return true;
                }
            }
        }
    }
    cout << "opss" << endl;
    return false;
}
//keywords->(cnd){statement}
bool keywords()
{   parsing<<"(cnd){statement} ->";
    if (parse[next_token] == lparn_sy)
    {
        inc;
        if (true_condition())
        {   
            if (parse[next_token] == rparn_sy)
            {
                inc;
                if (parse[next_token] == lbrace)
                {
                    inc;
                    if (statements())

                    {
                        if (parse[next_token] == rbrace)
                            return true;
                    }
                }
            }
        }
    }
    return false;
}
//variables-> id,varibles|id;
string derv;
bool variables()
{   
    if (parse[next_token] == idntifir_sy)
    {    derv.append(tokens[next_token]);
           if(find(variable.begin() , variable.end() , token[next_token]) != variable.end())
              variable.push_back(tokens[next_token]);
            else {
                cout <<"EROR ::: "<<tokens[next_token]<<"  Variable IS ALREADY DEFINED \n";
                return false ;
            }        inc;
        if (parse[next_token] == comma_sy)
        { derv.append(" , ");
            parsing<<derv<<",id,varibles" <<"->"<<endl;
            inc;
            variables();return true;
        }
        else if (parse[next_token] == semi_sy)
        {   parsing<<derv<<" "<<"id;" <<"->"<<endl;
            parsing<<derv<<";"<<endl;
            inc;
            cout << "TMAM" << endl;
            return true;
        }
    }
    cout << "ops"
         << " " << next_token << endl;
    return false;
}
//program ->Dttype.id;|Dttype.id,variables|statement|if|while
bool program()
{   parsing<<"program"<<"->";
    if (parse[next_token] == rl_Dtype_sy || parse[next_token] == fl_Dtype_sy)
    {   
        inc;
        if (parse[next_token] == idntifir_sy)
        {
             if(find(variable.begin() , variable.end() , token[next_token]) != variable.end())
              variable.push_back(tokens[next_token]);
            else {
                cout <<"EROR ::: "<<tokens[next_token]<<"  Variable IS ALREADY DEFINED \n";
                return false ;
            }
            inc;
            if (parse[next_token] == semi_sy)
            {   parsing<<"Dttype id;"<<"->";
                parsing<<tokens[0]<<"  id;";
                parsing<<tokens[0]<<" "<<tokens[1]<<";"<<endl;
                inc;
                cout << "OK" << endl;
            }
            else if (parse[next_token] == comma_sy)
            {   parsing<<"Dttype id,variables"<<"->";
                 parsing<<tokens[0]<<"id,variables" <<"->";
                 parsing<<tokens[0]<<" "<<tokens[1]<<",variables" <<"->";
                 derv.clear();
                 derv.append(tokens[0]);derv.append(" ");
                 derv.append(tokens[1]);derv.append(" , ");
                inc;
                variables();
                return true;
            }
        }
    }
    else if (parse[next_token] == while_sy)
    {   parsing<<"while ->";
        inc;
        if (keywords())
        {
            cout << "kois" << endl;
            return true;
        }
    }
    else if ( parse[next_token] == cnd)
    {   //parsing<<""
        inc;
        if (keywords())
        {
            cout << "kois" << endl;
            return true;
        }
    }

    else
    {
        if (statements())
            return true;
    }
    cout << "ops11" << endl;
    return false;
}

token_sy strDelimiter(string s)
{
    if (s == "؛")
        return semi_sy;
    if (s == "،")
        return comma_sy;
    if (s == "==" || s == ">=" || s == "<=")
    {
        return cmp_op_sy;
    }
    return error_sy;
}
// Returns 'true' if the character is a DELIMITER.
token_sy isDelimiter(char ch)
{
    if (ch == ' ' || ch == '+' || ch == '-' || ch == '*' ||
        ch == '/' || ch == '>' ||
        ch == '<' || ch == '=' || ch == '(' || ch == ')' || ch == '{' || ch == '}' ||
        ch == '"')
    {
        if (ch == '(')
            return lparn_sy;
        else if (ch == ')')
            return rparn_sy;
        else if (ch == '{')
            return lbrace;
        else if (ch == '}')
            return rbrace;
        else if (ch == '=')
            return equal_sy;
        return delimter;
    }
    return error_sy;
}

// Returns 'true' if the character is an OPERATOR.
token_sy isOperator(char ch)
{
    if (ch == '+' || ch == '-' || ch == '*' ||
        ch == '/')
    {
        return math_op_sy;
    }
    if (ch == '>' || ch == '<')
    {
        return cmp_op_sy;
    }

    return error_sy;
}

// Returns 'true' if the string is a VALID IDENTIFIER.
bool validIdentifier(string str)
{
    if (str[0] == '0' || str[0] == '1' || str[0] == '2' ||
        str[0] == '3' || str[0] == '4' || str[0] == '5' ||
        str[0] == '6' || str[0] == '7' || str[0] == '8' ||
        str[0] == '9' || str[0] == ',' || isDelimiter(str[0]) != error_sy)
        return (false);
    return (true);
}

// Returns 'true' if the string is a KEYWORD.
token_sy isKeyword(string str)
{
    if ((str == "صحيح") || (str == "لو") ||
        (str == "عشري") || (str == "كلما") ||
        (str == "تحقق"))
    {
        if (str == "صحيح")
            return rl_Dtype_sy;
        else if (str == "لو")
            return cnd;
        else if (str == "عشري")
            return fl_Dtype_sy;
        else if (str == "كلما")
            return while_sy;
        //else if (str== "نص")return string_sy;
        else if (str == "تحقق")
            return bool_sy;
        else if (str == "حرف")
            return char_sy;
    }
    return error_sy;
}

// Returns 'true' if the string is an INTEGER.
token_sy isInteger(string str)
{
    int i, len = str.length();
    if (len == 0)
        return (error_sy);
    for (i = 0; i < len; i++)
    {
        if (str[i] != '0' && str[i] != '1' && str[i] != '2' && str[i] != '3' && str[i] != '4' && str[i] != '5' && str[i] != '6' && str[i] != '7' && str[i] != '8' && str[i] != '9' || (str[i] == '-' && i > 0))
            return (error_sy);
    }
    return (int_sy);
}
// Returns 'true' if the string is a REAL NUMBER.
token_sy isRealNumber(string str)
{
    int i, len = str.length();
    int hasDecimal = 0;
    if (len == 0)
        return (error_sy);
    for (i = 0; i < len; i++)
    {
        if (str[i] != '0' && str[i] != '1' && str[i] != '2' && str[i] != '3' && str[i] != '4' && str[i] != '5' && str[i] != '6' && str[i] != '7' && str[i] != '8' && str[i] != '9' && (str[i] != ',') ||
            (str[i] == '-' && i > 0))
            return (error_sy);
        if (str[i] == ',')
            hasDecimal += 1;
    }
    if (hasDecimal == 1)
        return (real_sy);
    return (error_sy);
}

int main()
{

    if (mycode.is_open())
    {
        while (getline(mycode, line))
        {
            token = "";
            subtok = "";
            subtok.push_back(' ');
            subtok.push_back(line[0]);
            line.push_back(' ');
            line.push_back(' ');
            for (int i = 0; i < line.size(); i++)
            {
                if (line[i] == ' ' && token.size() == 0)
                    continue;
                if (i < line.size() - 1)
                {
                    swap(subtok[0], subtok[1]);
                    subtok[1] = line[i + 1];
                }
                // cout<<subtok<<endl;
                if (isDelimiter(line[i]) != error_sy || strDelimiter(subtok) != error_sy)
                {
                    if (token.size() > 0)
                    {
                        lexical << token << " is token" << endl;

                        if (isKeyword(token) != error_sy)
                        {
                            output << token << " " << (isKeyword(token)) << endl;
                            parse.push_back((isKeyword(token)));
                            tokens.push_back(token);
                        }
                        else if (isInteger(token) != error_sy)
                        {
                            output << token << " " << (isInteger(token)) << endl;
                            parse.push_back((isInteger(token)));
                            tokens.push_back(token);
                        }
                        else if (isRealNumber(token) != error_sy)
                        {
                            output << token << " " << (isRealNumber(token)) << endl;
                            parse.push_back((isRealNumber(token)));
                            tokens.push_back(token);
                        }
                        else if (validIdentifier(token))
                        {
                            output << token << " idntifir_sy" << endl;
                            parse.push_back(10);
                            tokens.push_back(token);
                        }
                        else
                        {
                            output << token << "  is an INVALID token " << endl;
                            cout << token << "is invalid " << endl;
                            return -1;
                        }
                    }
                    if (line[i] != ' ')
                    {
                        if (strDelimiter(subtok) != error_sy)
                        {
                            lexical << subtok;
                            output << subtok << " " << strDelimiter(subtok) << endl;
                            parse.push_back(strDelimiter(subtok));
                            tokens.push_back(subtok);
                            i++;
                        }
                        else
                            lexical << line[i];
                        lexical << " is delimeter\n";
                        if (isOperator(line[i]) != error_sy)
                        {
                            output << line[i] << " " << (isOperator(line[i])) << endl;
                            string xx = "";
                            xx.push_back(line[i]);
                            parse.push_back((isOperator(line[i])));
                            tokens.push_back(xx);
                        }
                        else if (isDelimiter(line[i]) != error_sy)
                        {
                            output << line[i] << " " << (isDelimiter(line[i])) << endl;
                            string xx = "";
                            xx.push_back(line[i]);
                            tokens.push_back(xx);
                            parse.push_back((isDelimiter(line[i])));
                        }
                    }
                    else
                        lexical << "there is a white space\n";

                    token = "";
                }
                else
                    token.append(1, line[i]);
            }
            for (int i = 0; i < variable.size(); i++)
                cout << variable[i] << " ";
            if (parse.size() > 0)
                program();
            parse.clear();
            next_token = 0;
            tokens.clear();
        }
        mycode.close();

        cout << "Done" << endl;
    }
    else
        cout << "Unable to open file";

    return 0;
}