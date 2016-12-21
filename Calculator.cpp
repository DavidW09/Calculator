// Calculator.cpp : Defines the entry point for the console application.

#include "stdafx.h"
#include <iostream>
#include <sstream>
#include <string>
#include <stack>
#include <vector>
#include <cmath>
#include <algorithm>
#include <unordered_map>

using namespace std;

//Foward Declarations
bool IsOperand(char c);
bool TakesPriority(char OperatorA, char OperatorB);
bool ReplaceVariable(string& str, const string& from, const string& to);

double Calculate(string[]);

string FindNumbers(string);
string AddDelimiter(string);
string DeleteSpaces(string &str);
string RemoveVariable(string &str);
string Evaluate(string);

void ConvertExpression(const string & expression, string & Postfix);

int main()
{
	bool isRunning = true;
	unordered_map<string, string> map;

	while (isRunning)
	{
		try
		{
			string userExpression, adjustedExpression;

			getline(cin, userExpression);

			if (userExpression == "quit")
			{
				isRunning = false;
			}

			else
			{
				stringstream stream(userExpression);
				string word;
				stream >> word;

				if (word == "let")
				{
					stream >> word;
					string variable = word;

					userExpression = RemoveVariable(userExpression);

					for (auto it = map.begin(); it != map.end(); it++)
					{
						string  keyValue = it->first;
						string mappedValue = it->second;
						ReplaceVariable(userExpression, keyValue, mappedValue);
					}

					string newExpression = DeleteSpaces(userExpression);
					newExpression = FindNumbers(userExpression);

					ConvertExpression(newExpression, adjustedExpression);

					string answer = AddDelimiter(adjustedExpression);

					string result = Evaluate(answer);
					//adds variable to list
					map[variable] = result;

				}
				else
				{
					for (auto it = map.begin(); it != map.end(); it++)
					{
						string  keyValue = it->first;
						string mappedValue = it->second;
						ReplaceVariable(userExpression, keyValue, mappedValue);
					}
					string newExpression = DeleteSpaces(userExpression);
					newExpression = FindNumbers(userExpression);

					ConvertExpression(newExpression, adjustedExpression);

					string answer = AddDelimiter(adjustedExpression);

					string result = Evaluate(answer);
					cout << result << endl;

				}
			}
		}
		catch (int e)
		{
			if (e == 0)
				cout << "Division-By-Zero" << endl;
			else if (e == 10)
				cout << "Undeclared-Variable" << endl;
		}
	}
	return 0;
}

// Checks whether the character "c" is an operand or not.
bool IsOperand(char c)
{
	if (((c >= '0') && (c <= '9')) || (c == '.') || (c == ','))
	{
		return true;
	}
	else if (c == '(' || c == ')' || c == '+' || c == '-' || c == '/' || c == '*' || c == '^')
	{
		return false;
	}
	else
	{
		//cout << "Undeclared-Variable" << endl;
		throw 10;
	}
}

/* Determines whether operatorA has higher priority over operatorB.
If it returns true, then A has precedence over B. If false then B > A
*/
bool TakesPriority(char operatorA, char operatorB)
{
	if (operatorA == '(')
		return false;
	else if (operatorB == '(')
		return false;
	else if (operatorB == ')')
		return true;
	else if ((operatorA == '^') && (operatorB == '^'))
		return false;
	else if (operatorA == '^')
		return true;
	else if (operatorB == '^')
		return false;
	else if ((operatorA == '*') || (operatorA == '/'))
		return true;
	else if ((operatorB == '*') || (operatorB == '/'))
		return false;
	else
		return true;
}

// Replaces the variable stored from the hashmap with its value. (replacing x  with 5)
bool ReplaceVariable(string& str, const string& from, const string& to)
{
	size_t start_pos = str.find(from);
	if (start_pos == std::string::npos)
		return false;
	str.replace(start_pos, from.length(), to);
	ReplaceVariable(str, from, to);
	return true;
}

// Calculates the small string array
double Calculate(string expression[])
{
	string numA, operatorB, numC;
	numA = expression[2]; operatorB = expression[0]; numC = expression[1];

	double numberA, numberC, result;

	istringstream insertA, insertC;
	insertA.str(numA); insertC.str(numC);

	insertA >> numberA;
	insertC >> numberC;

	if (operatorB == "+")
	{
		result = numberA + numberC;
		return result;
	}
	else if (operatorB == "-")
	{
		result = numberA - numberC;
		return result;
	}
	else if (operatorB == "*")
	{
		result = numberA * numberC;
		return result;
	}
	else if (operatorB == "/")
	{
		if (numberC == 0)
		{
			//cout << "Division-By-Zero" << endl;
			throw 0;
		}
		result = numberA / numberC;
		return result;
	}
	else if (operatorB == "^")
	{
		result = pow(numberA, numberC);
		return result;
	}
	return 0;
}

// Deletes the empty spaces.
string DeleteSpaces(string &str)
{
	// it was str.erase(remove_if(str.begin(), str.end(), isspace), str.end());
	// worked in visual studio, wouldn't compile in linux but this works the same
	str.erase(remove(str.begin(), str.end(), ' '), str.end());
	return str;
}

// removes the "let <variablename> =" 
string RemoveVariable(string &str)
{
	str.erase(str.begin() + str.find_first_of("l"), str.begin() + (str.find_last_of("=") + 1));
	return str;
}


//Tokenizes the expression. Adds a comma(delimiter) after any number. (Like 2.2, 32, 5, etc.)
string FindNumbers(string expression)
{
	for (int i = 0; i <(int)expression.length(); i++)
	{
		if (isdigit(expression[i]) != false || expression[i] == '.')
		{
			if (isdigit(expression[i + 1]) == false && expression[i + 1] != '.')
			{
				expression.insert(i + 1, ",");
			}
		}
	}
	return expression;
}

// Tokenizes the expression by adding a delimiter after any of the operators.
string AddDelimiter(string expression)
{
	for (int i = 0; i <(int)expression.length(); i++)
	{
		if ((expression[i] == '*') || (expression[i] == '-') || (expression[i] == '/') || (expression[i] == '+') || (expression[i] == '^'))
		{
			expression.insert(i + 1, ",");
		}
	}
	return expression;
}

// Evaluates the expression.
string Evaluate(string expression)
{
	int delimiterCounter = 0;
	for (int a = 0; a < (int)expression.length(); a++)
	{
		if (expression[a] == ',')
		{
			delimiterCounter++;
		}
	}

	string tempExpression;

	if (delimiterCounter == 1)
	{
		while (expression.find(",", 0) != string::npos)
		{
			size_t pos = expression.find(",", 0);
			tempExpression = expression.substr(0, pos);
			expression.erase(0, pos + 1);
			return tempExpression;
		}
	}


	vector <string> array;

	//Evaluate tokens using the "," as a delimiter
	while (expression.find(",", 0) != string::npos)
	{
		size_t pos = expression.find(",", 0);
		tempExpression = expression.substr(0, pos);
		expression.erase(0, pos + 1);
		array.push_back(tempExpression); //store in vector              
	}

	stack <string> expressionStack;//initialise stack
	string tempArray[100];
	string result;

	for (int i = 0; i < delimiterCounter; i++)
	{
		string operatorB;
		operatorB = array[i]; //make it easier to read

		if ((operatorB != "+") && (operatorB != "*") && (operatorB != "-") && (operatorB != "/") && (operatorB != "^"))
		{
			expressionStack.push(operatorB);
			//push numbers onto the stack
		}

		else //i.e if it encounters an operator
		{
			expressionStack.push(operatorB);//push operator onto stack
			tempArray[0] = expressionStack.top();//store value
			expressionStack.pop(); //erase from the stack

			tempArray[1] = expressionStack.top();//store value
			expressionStack.pop();//erase from the stack

			tempArray[2] = expressionStack.top();//store value
			expressionStack.pop();//erase from the stack


			double number;
			number = Calculate(tempArray);
			ostringstream outs;  // Declare an output string stream.
			outs << number;   // Convert value into a string.
			result = outs.str();

			expressionStack.push(result);
		}
	}
	return result;
}

// Takes the newExpression (which has no spaces) and finds an equivalent way of writing the expression. to make solving easier( going from A * B + C / D) to (AB * CD / +)
void ConvertExpression(const string & expression, string & adjustedExpression)
{
	stack<char> OperatorStack;
	char topSymbol, symbol;
	int k;

	for (k = 0; k < (int)expression.size(); k++)
	{
		symbol = expression[k];
		if (IsOperand(symbol))
			adjustedExpression = adjustedExpression + symbol;
		else
		{
			while ((!OperatorStack.empty()) && (TakesPriority(OperatorStack.top(), symbol)))
			{
				topSymbol = OperatorStack.top();
				OperatorStack.pop();
				adjustedExpression = adjustedExpression + topSymbol;
			}
			if ((!OperatorStack.empty()) && (symbol == ')'))
			{
				OperatorStack.pop();
			}
			else
				OperatorStack.push(symbol);
		}
	}

	while (!OperatorStack.empty())
	{
		topSymbol = OperatorStack.top();
		OperatorStack.pop();
		adjustedExpression = adjustedExpression + topSymbol;
	}
}