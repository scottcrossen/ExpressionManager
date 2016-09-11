#pragma once
#include "ExpressionManagerInterface.h"
class ExpressionManager :
	public ExpressionManagerInterface
{
public:
	ExpressionManager(void);
	~ExpressionManager(void);
	/*
	Function:	bool isBalanced(string expression)
	Input:		a string containing parentheses or any characters.
	Return:		true if parentheses are correctly aligned.
				false otherwise.
	*/
	bool isBalanced(string expression);
	/*
	Function:	string postfixToInfix(string postfixExpression)
	Input:		a string containing postfix expression
	Return:		a string containing a prefix expression
	*/
	string postfixToInfix(string postfixExpression);
	/*
	Function:	string infixToPostfix(string infixExpression)
	Input:		a string containing a prefix expression
	Return:		a string containing postfix expression
	*/
	string infixToPostfix(string infixExpression);
	/*
	Function:	string postfixEvaluate(string postfixExpression)
	Input:		a string containing a postfix expression
	Return:		a string equivalent of the integer value of the expression
	*/
	string postfixEvaluate(string postfixExpression);
};

