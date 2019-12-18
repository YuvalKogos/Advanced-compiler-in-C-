
#include "stdafx.h"
#include <iostream>
#include <string>
#include <stdlib.h>
#include <regex>
#include <vector>
#include <sstream>
#include <math.h>
#include <fstream>

using namespace std;

//Game Classes

void ClearConsole()
{
	system("CLS");
}

vector<string> split(string code, char c)
{
	vector<string> result;
	//loop over the string and search for char
	string tmp = "";
	for (int i = 0; i < code.size(); i++)
	{
		if (code[i] == c)
		{
			result.push_back(tmp);
			tmp = "";
		}
		else
		{
			tmp += code[i];
		}
	}

	result.push_back(tmp);

	return result;
}

char* toCharArray(string st)
{
	char* arr = new char[st.size()];
	for (int i = 0; i < st.size(); i++)
		arr[i] = st[i];

	return arr;
}

int getIntFromChar(char c)
{
	if (c == '0')
		return 0;
	if (c == '1')
		return 1;
	if (c == '2')
		return 2;
	if (c == '3')
		return 3;
	if (c == '4')
		return 4;
	if (c == '5')
		return 5;
	if (c == '6')
		return 6;
	if (c == '7')
		return 7;
	if (c == '8')
		return 8;
	if (c == '9')
		return 9;

	return 0;
}

int StringToInt(string s)
{

	int result = 0;
	for (int i = 0; i < s.size(); i++)
	{
		int powd = s.size() - i - 1;
		int dig = getIntFromChar(s[i]);
		powd = pow(10, powd);
		result += dig * powd;
	}

	return result;
}

void printArr(char* arr)
{
	for (int i = 0; i < sizeof(arr); i++)
	{
		cout << arr[i] << ",";
	}
	cout << endl;
}

string getIntsAssemblyCode(string javaCode, string var)
{
	string AsmCode = "";
	char* varArr = toCharArray(var);
	for (int i = 0; i < javaCode.size() - var.size(); i++)
	{
		//Search if code fits the var string (type)
		bool match = true;
		for (int k = 0; k < sizeof(varArr) - 1; k++)
		{
			if (javaCode[i + k] != varArr[k])
			{
				match = false;
				break;
			}
		}
		if (match)
		{
			//Get end of statment by ;
			int endIntStatementIdx = 0;
			for (int j = i; j < javaCode.size(); j++)
			{
				if (javaCode[j] == ';')
				{
					endIntStatementIdx = j;
					break;
				}
			}
			//Crop the sentence
			string statement = javaCode.substr(i + 3, endIntStatementIdx);
			//Search for name
			regex name_regex("( *[a-z]+ *)");
			smatch name_match;
			regex_search(statement, name_match, name_regex);
			string name = name_match.str(0);
			//Search for value
			regex val_regex(" [0-9]+");
			smatch val_match;
			regex_search(statement, val_match, val_regex);
			string value = val_match.str(0);


			AsmCode += name + " DWORD " + value + "\n";

		}

	}

	return AsmCode;
}

string getEqualOperations(string javaCode)
{
	string s1;

	//get regex's of equality syntax (a = b);
	string answer = "";
	string tmpJavaCode = javaCode;

	regex equal_reg("([a-z]* = [a-z0-9]*;)");
	smatch eq_matches;
	while (regex_search(tmpJavaCode, eq_matches, equal_reg))
	{
		string match = eq_matches.str(0);
		regex name1Reg("([a-z]*)");
		smatch name1Match;
		regex_search(match, name1Match, name1Reg);

		regex name2Reg("(= *[a-z0-9]*)");
		smatch name2Match;
		regex_search(match, name2Match, name2Reg);

		string name1 = name1Match.str(0);
		string name2 = name2Match.str(0).substr(2, name2Match.str(0).size() - 1);

		answer += "MOV ax," + name2 + "\n";
		answer += "MOV " + name1 + ",ax \n";

		tmpJavaCode = eq_matches.suffix().str();

	}

	return answer;
}

string getAddingOperations(string javaCode)
{
	//get regex's of equality syntax (a = b);
	string answer = "";
	string tmpJavaCode = javaCode;

	regex equal_reg("([a-z]* [\+]= [a-z0-9]*;)");
	smatch eq_matches;
	while (regex_search(tmpJavaCode, eq_matches, equal_reg))
	{
		string match = eq_matches.str(0);
		regex name1Reg("([a-z]*)");
		smatch name1Match;
		regex_search(match, name1Match, name1Reg);

		regex name2Reg("(= *[a-z0-9]*)");
		smatch name2Match;
		regex_search(match, name2Match, name2Reg);

		string name1 = name1Match.str(0);
		string name2 = name2Match.str(0).substr(2, name2Match.str(0).size() - 1);

		answer += "MOV ax," + name2 + "\n";
		answer += "ADD " + name1 + ",ax \n";

		tmpJavaCode = eq_matches.suffix().str();

	}

	return answer;
}

string getSubstractOperations(string javaCode)
{
	//get regex's of equality syntax (a = b);
	string answer = "";
	string tmpJavaCode = javaCode;

	regex equal_reg("([a-z]* -= [a-z0-9]*;)");
	smatch eq_matches;
	while (regex_search(tmpJavaCode, eq_matches, equal_reg))
	{
		string match = eq_matches.str(0);
		regex name1Reg("([a-z]*)");
		smatch name1Match;
		regex_search(match, name1Match, name1Reg);

		regex name2Reg("(= *[a-z0-9]*)");
		smatch name2Match;
		regex_search(match, name2Match, name2Reg);

		string name1 = name1Match.str(0);
		string name2 = name2Match.str(0).substr(2, name2Match.str(0).size() - 1);

		answer += "MOV ax," + name2 + "\n";
		answer += "SUB " + name1 + ",ax \n";

		tmpJavaCode = eq_matches.suffix().str();

	}

	return answer;
}

string getOperationsAsmCode(string Code)
{
	string answer = "";

	//Search by line
	vector<string> splitted_code = split(Code, 'n');
	for (string op : splitted_code)
	{
		if (op.find("+=") != std::string::npos)
			answer += getAddingOperations(op);
		if (op.find("-=") != std::string::npos)
			answer += getSubstractOperations(op);
		if (op.find("=") != std::string::npos)
			answer += getEqualOperations(op);


	}
	cout << endl;


	return answer;
}

int getNextEndOfStatement(string javaCode, int indx)
{
	for (int i = indx; i < javaCode.size(); i++)
	{
		if (javaCode[i] == ';')
			return i;
	}

	return 0;
}

vector<string> getIfStatements(string javaCode)
{
	vector<string> ifStatements;
	//Loop over the javaCode and search for 'if' keyword
	for (int i = 0; i < javaCode.size(); i++)
	{
		if (javaCode[i] == 'i' && javaCode[i + 1] == 'f')
		{
			//get next ; which indicates the end of the statement
			int endOfStatement = getNextEndOfStatement(javaCode, i);
			//Substring the statement
			string stateMent = javaCode.substr(i, endOfStatement + 1);
			ifStatements.push_back(stateMent);
			i = endOfStatement;
		}
	}
	return ifStatements;
}

string getOperatorWord(string condition)
{
	regex op_reg("([><=][=]?)");
	smatch op_matches;
	regex_search(condition, op_matches, op_reg);

	if (op_matches.str(0) == ">=")
		return "JL";
	if (op_matches.str(0) == "<=")
		return "JG";
	if (op_matches.str(0) == "==")
		return "JNE";
	if (op_matches.str(0) == ">")
		return "JL";
	if (op_matches.str(0) == "<")
		return "JG";

	return "";

}

string conditionAtAsm(string condition, string jmpLineName)
{
	string answer = "";
	//name 1
	regex name1Reg("([a-z]*)");
	smatch name1Match;
	regex_search(condition, name1Match, name1Reg);
	string name1 = name1Match.str(0);

	//name 2
	regex name2Reg("([><=] *[a-z0-9]*)");
	smatch name2Match;
	regex_search(condition, name2Match, name2Reg);
	string name2 = name2Match.str(0);

	name2 = name2.substr(1, name2.size());


	answer += "MOV dx," + name1 + "\n";
	answer += "CMP dx," + name2 + "\n";

	//Get operator 
	answer += getOperatorWord(condition) + " " + jmpLineName + "\n";

	return answer;

}

int getNextCharIdx(string st, char c)
{
	for (int i = 0; i < st.size(); i++)
	{
		if (st[i] == c)
			return i;
	}
	return 0;
}

string getIfStatementsCode(string javaCode)
{
	string answer = "";
	//Check if if statements exists
	if (javaCode.find("if") == std::string::npos)
		return "";

	//Get if statements
	vector<string> ifStatments = getIfStatements(javaCode);
	//Parse each statement
	int jmpLineNum = 0;
	for (string statement : ifStatments)
	{
		string jmpLine = "line" + to_string(jmpLineNum);
		string ASMStatement = "";

		//Make the condition regex
		regex cond_reg("([a-z]* [><=][=]? [a-z0-9])");
		smatch cond_matches;
		regex_search(statement, cond_matches, cond_reg);

		answer += conditionAtAsm(cond_matches.str(0), jmpLine);

		//Get in statement
		string::size_type loc = statement.find('{');
		int inStateStart = getNextCharIdx(statement, '{');
		string::size_type loc1 = statement.find('}');
		int inStateEnd = getNextCharIdx(statement, '}');
		string inStatement = statement.substr(inStateStart, inStateEnd);
		string inStatementAsm = getOperationsAsmCode(inStatement);
		answer += inStatementAsm + "\n" + jmpLine + ": \n";
		jmpLineNum++;
	}

	//cout << "if statements : \n" << answer << endl;
	return answer;
}

string getVarieblesCode(string javaCode)
{
	string answer = "";

	//Check for int type vaiebles:
	if (javaCode.find("int") != std::string::npos)
		answer += getIntsAssemblyCode(javaCode, "int");
	if (javaCode.find("double") != std::string::npos)
		answer += getIntsAssemblyCode(javaCode, "double");
	if (javaCode.find("bool") != std::string::npos)
		answer += getIntsAssemblyCode(javaCode, "bool");



	return answer;
}

void generateAssemblyCode(string javaCode)
{
	//Search and declare vairbles

}

vector<string> getForStatements(string javaCode)
{
	vector<string> forStatements;
	//Loop over the javaCode and search for 'if' keyword
	for (int i = 0; i < javaCode.size(); i++)
	{
		if (javaCode[i] == 'f' && javaCode[i + 1] == 'o' && javaCode[i + 2] == 'r')
		{
			//get next ; which indicates the end of the statement
			int endOfStatement = getNextEndOfStatement(javaCode, i);
			//Substring the statement
			string stateMent = javaCode.substr(i, endOfStatement + 1);
			forStatements.push_back(stateMent);
			i = endOfStatement;
		}
	}
	return forStatements;
}

string* getForStatementConditions(string statement)
{
	//get for condition statmenet
	size_t start_cond = statement.find("(");
	size_t end_cond = statement.find(")");
	string condition = statement.substr(start_cond + 1, end_cond - 4);
	cout << condition << endl;
	//Parse for statement conditions at type for(stop,start,step) 
	regex num_reg("(\d+)");
	smatch num_matches;
	int cond_count = 0;
	string max = "0", min = "0", step = "1";
	while (regex_search(condition, num_matches, num_reg))
	{
		//cout << "\n" << num_matches.str(0)<<endl; 
		if (cond_count == 0)
			max = num_matches.str(0);
		if (cond_count == 1)
			min = num_matches.str(0);
		if (cond_count == 2)
			step = num_matches.str(0);

		condition = num_matches.suffix().str();
		cond_count++;
	}
	cout << "count " << cond_count << endl;

	string* arr = new string[3];
	vector<string> tmpArr = split(condition, ',');
	arr[0] = tmpArr[0];
	arr[1] = tmpArr[1];
	arr[2] = tmpArr[2];

	return arr;
}

string getForLoopAsmCode(string javaCode)
{
	string answer = "";
	//Check if for keyowrd exists - if not return ""
	if (javaCode.find("for") == std::string::npos)
		return answer;

	vector<string> forStatements = getForStatements(javaCode);
	//loop over all statements
	int loop_idx = 0;
	for (string statement : forStatements)
	{
		string loop_line = "for_loop_" + to_string(loop_idx);
		//get for loop conditions
		string* arr = getForStatementConditions(statement);
		string end = arr[0];
		string begin = arr[1];
		string step = arr[2];
		delete[] arr;
		//if for loop content content if statement - parse it
		string loop_content = "";
		string loop_content_code = split(statement, '{')[1];
		if (getIfStatementsCode(statement) != "")
			loop_content += getIfStatementsCode(loop_content_code);
		else
			loop_content += getOperationsAsmCode(loop_content_code);

		answer += "MOV cx," + begin + "\n";
		answer += loop_line + ":\n";
		answer += loop_content + "\n";
		answer += "ADD cx," + step + "\n";
		answer += "CMP cx," + end + "\n";
		answer += "JL " + loop_line + "\n";


		loop_idx++;
	}

	return answer;
}

void writeToFile(string text)
{
	ofstream outputFile("C:\\Users\\yuval\\Desktop\\ASM code output.txt");
	outputFile << text;
}

string javaCodeFromFile(string file_path)
{
	string javaCode = "";
	ifstream input_file(file_path);
	string currLine = "";
	while (getline(input_file, currLine))
	{
		javaCode += currLine + "\n";
	}

	return javaCode;
}

int main()
{
	string javaCode;;
	//getEqualOperations(javaCode);
	string finalAsmCode = "";
	std::cout << "Enter java code : (or 'file' to read from file)" << endl;
	getline(cin, javaCode);

	if (javaCode == "file")
		javaCode = javaCodeFromFile("C:\\Users\\yuval\\Desktop\\javaCode.txt");

	finalAsmCode += getVarieblesCode(javaCode) + "\n";
	finalAsmCode += getIfStatementsCode(javaCode) + "\n";
	finalAsmCode += getForLoopAsmCode(javaCode);

	writeToFile(finalAsmCode);



	//std::cout << javaCode << endl;
	std::cout << "Final ASM Code: \n";
	std::cout << finalAsmCode;
	//getVarieblesCode(javaCode);

	string tri = "im,the,best";
	vector<string> result = split(tri, ',');
	for (string res : result)
	{
		cout << endl << res;
	}
	system("Pause");
	return 0;
}

