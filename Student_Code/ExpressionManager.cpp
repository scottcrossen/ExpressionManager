#include "ExpressionManager.h"
#include <stack>
#include <sstream>
#include <string>
#include <queue>
ExpressionManager::ExpressionManager(void){}	// No need for a constructor
ExpressionManager::~ExpressionManager(void){}	// No need for a deconstructor
/*
Function:	bool isBalanced(string expression)
input:		a string containing parentheses or any characters.
return:		true if parentheses are correctly aligned.
			false otherwise.
Approach:	use a stack to add the symbols that don't align and remove the symbols that do.
*/
bool ExpressionManager::isBalanced(string expression){
	// Define Variables:
	stack<char> brackets;
	brackets.push(' ');
	if (expression == string()) return true;	// Test null case.
	for (auto iterator : expression){	// Iterate through data structure
		if (iterator == '(' || iterator == '[' || iterator == '{') brackets.push(iterator);	// Add another bracket to the list if it's a beginning.
		if (   ((iterator == ')') && ((brackets.top() != '(') || (brackets.size() == 0)))	// Test to make sure Brackets are the same type and that there is brackets left.
			|| ((iterator == '}') && ((brackets.top() != '{') || (brackets.size() == 0)))
			|| ((iterator == ']') && ((brackets.top() != '[') || (brackets.size() == 0))))
			return false;
		if (((iterator == ')') && (brackets.top() == '(')) || ((iterator == '}') && (brackets.top() == '{')) || ((iterator == ']') && (brackets.top() == '['))) brackets.pop();	// Matching brackets cancel
	}
	if (brackets.size()==1) return true;	// After the iteration is complete test to make sure that there is no brackets left.
	else return false;
}
/*
Function:	string postfixToInfix(string postfixExpression)
Input:		a string containing postfix expression
Return:		a string containing a prefix expression
Approach:	I realize now that there's an easier way to do this. I should have done it much like i did the postfixEvaluate method. Oh well.
			In this method we will use recursion. Our base case will be individual numbers. We will go through the given string and find which operator is next in priority. This is always
			the last one on the string that we give it (because of recursion). We will then find what two things are being operated on - whether a number like 5 or another expression like 5 5 +
			and then recursively compute the outputs of those. (5 would just return 5. It's our base case). Each recursion will only need to "pay attention" to one of /*-+%. We'll add the two objects to a vector
			and we will read the expression from a stack.
*/
string ExpressionManager::postfixToInfix(string postfixExpression){
	// Define Variables:
	const string error = "invalid";
	const string validchars = "1234567890(){}[]/*+-%";
	stringstream expression(postfixExpression);
	stack<string> operations;
	string iterator1;
	int number_count = 0;
	vector<string> objects;
	objects.push_back(string());	// Add the strings of the two things being operated on per step. "5 3 +" would have obj0="5" obj1="3" . Or even "5 4 3 + +" would have obj0="5" obj1="4 3 +" .
	if (postfixExpression == string()) return error;	// Test to see if parentheses are balanced.
	while (expression >> iterator1){	// Read in the information.
		for (int iterator2 = 0; iterator2 <= iterator1.size() - 1; iterator2++)	// Test to make sure each word has viable characters.
			if (validchars.find(iterator1.substr(iterator2, 1)) < 0 || validchars.find(iterator1.substr(iterator2, 1)) > validchars.size() - 1) return error;
		operations.push(iterator1);	// Add the operation to the stack to be computed.
	}
	// If the input expression is singular and constant like "5" or "3" then just return that. This is the base case for the recursion we will use.
	if (operations.top() != "-" && operations.top() != "+" && operations.top() != "/" && operations.top() != "*" && operations.top() != "%"){
		if (operations.size() > 1)
			return error;
		else return operations.top();
	}
	string current_operation = operations.top(); operations.pop();	// This expression is not a singular constant. We then need to find the next most important operation to do. This is the end operation in the string.
	while (operations.size() != 0){	// Go through the list and seperate the expression into it's component objects. "5 3 +" would be obj0="5" obj1="3" . Or even "5 4 3 + +" would be obj0="5" obj1="4 3 +" .
		if (operations.top() != "-" && operations.top() != "+" && operations.top() != "/" && operations.top() != "*" && operations.top() != "%")	// If the current iteration is a number, add the count. otherwise subtract it. We want count==1.
			number_count++;
		else number_count--;
		objects[objects.size() - 1] = operations.top() + " " + objects[objects.size() - 1];
		operations.pop();
		if (number_count == 1){	// We need to find the node in the expression where the two objects "split".
			number_count = 0;
			objects.push_back(string());	// We found an object! add it to the vector. There should only be two per operation.
		}
	}
	if (objects.size()<3) return error;	// If we have more than two objects the expression isn't correct.
	if (objects.size() >= 4 || objects[2] != string()) return error;
	// compute the operation on the two objects.
	string output = "( " + postfixToInfix(objects[1].substr(0, objects[1].size() - 1)) + " " + current_operation + " "+postfixToInfix(objects[0].substr(0, objects[0].size() - 1)) + " )";
	if (output.find(error) >= 0 && output.find(error) <= output.size()) return error; else return output;	// Return error if any of the previous recursions returned an error.
}
/*
Function:	string infixToPostfix(string infixExpression)
Input:		a string containing a prefix expression
Return:		a string containing postfix expression
Approach:	The main thing here is paying attention to order of operations. We handle parantheses then multiplcation/division then addition/subtraction.
			For parenthese we use a a pretty basic recusion and remove the lead/final paranthese when we call this same function. Wenever parantheses are found we
			recurively compute their string result and add it to our working output. For /*+- we use a stack so that we can compute the object ahead of the arithemetic before we add the arithmetic.
			We push the arithemetic operation onto the stack "hold" and then when we are allowed we remove it. the operators /* get more priority than the +- operators so if +- are on the "hold" and we
			encounter a /* then we use the /*. Also, the base case for the parantheses handling is any single-lay expression like 5 5 * 5 - 5 / 5
*/
string ExpressionManager::infixToPostfix(string infixExpression){
	// Define Variables:
	const string error = "invalid";
	const string validchars = "1234567890(){}[]/*+-%";
	queue<string> operations;
	stringstream stream_expression(infixExpression);
	string input;
	string output;
	stack<string> hold;
	if (!(isBalanced(infixExpression))) return error;	// Test to see if parentheses are balanced.
	while (stream_expression >> input) {	// Read in the string and put it into a stack of operations.
		for (int iterator1 = 0; iterator1 <= input.size() - 1; iterator1++)	// Test to make sure the characters are all good.
			if (validchars.find(input.substr(iterator1, 1)) < 0 || validchars.find(input.substr(iterator1, 1)) > validchars.size() - 1) return error;
		operations.push(input);
	}
	if (operations.size() == 0) return error;	// If this is a null string then return error.
	if (operations.front() == "+" || operations.front() == "-" || operations.front() == "*" || operations.front() == "/" || operations.front() == "%" ) return error;	// Infix can't begin with an operation.
	while (operations.size() != 0){	// Go completely through the list of characters.
		if (operations.front() == "(" || operations.front() == "[" || operations.front() == "{"){	// If it's a parantheses we want to recursively call the smaller problem.
			int parenth_count = 1;
			string recursive_argument = string();	// We'll construct another string to pass to this very function.
			operations.pop();
			while (parenth_count>0){	// Find where the parantheses finish the "object".
				recursive_argument = recursive_argument + operations.front() + " ";
				if (operations.front() == "(" || operations.front() == "[" || operations.front() == "{")
					parenth_count++;
				if (operations.front() == ")" || operations.front() == "]" || operations.front() == "}")
					parenth_count--;
				operations.pop();	// We no longer need the symbols in the operations stack. We will add the output of the recursion on ourselves.
			}
			output = output + " " + infixToPostfix(recursive_argument.substr(0, recursive_argument.size() - 2));	// Recursively compute parantheses.
			if (hold.size() != 0) if (hold.top() == "*" || hold.top() == "/" || hold.top() == "%" ){ output = output + " " + hold.top(); hold.pop(); }	// If there is * or / as the operator on the parantheses then add that.
		}
		else if (operations.front() == "*" || operations.front() == "/" || operations.front() == "%"){	// Remeber PMDOS. We've handled parantheses. Now we handle multiplcation and division.
			if (hold.size() != 0) if (hold.top() == "*" || hold.top() == "/" || hold.top() == "%"){ output = output + " " + hold.top(); hold.pop(); }	// If there's * / on the "waiting list" we need to add that.
			hold.push(operations.front()); operations.pop();	// Push the operation onto the "waiting list" until we can be sure that we can use it (i.e. there's not parantheses ahead of it.)
			if (operations.size() == 0) return error;	// If that was the last operation then the original expression is wrong. Return error.
			if (operations.front() == "+" || operations.front() == "-" || operations.front() == "*" || operations.front() == "/" || operations.front() == "%") return error;	// Two operations can't follow one another.
		}
		else if (operations.front() == "+" || operations.front() == "-"){	// Finally we handle the lowest priority. + or -.
			if (hold.size() != 0){ output = output + " " + hold.top(); hold.pop(); }	// If there's something on the "wait list" for mathematical operations, we should use that.
			hold.push(operations.front()); operations.pop();	// Push the current itme onto the "waiting list" so we can use it later.
			if (operations.size() == 0) return error;	// If that was the last operation then the original expression is wrong. Return error.
			if (operations.front() == "+" || operations.front() == "-" || operations.front() == "*" || operations.front() == "/" || operations.front() == "%") return error;	// Two operations can't follow one another.
		}
		else {
			output = output+" "+operations.front();	// Well. The current symbol in operations must be a number. We can add that now.
			operations.pop();
			if (hold.size() != 0) if (hold.top() == "*" || hold.top() == "/" || hold.top() == "%"){ output = output + " " + hold.top(); hold.pop(); }	// Add any waiting arithemetic.
		}
	}
	while(hold.size() != 0){ output = output + " " + hold.top(); hold.pop(); }	// After we've gone through the list there still might be waiting arithemetic. We can add those here at the end.
	if (output.find(error) >= 0 && output.find(error) <= output.size()) return error;	// If any previous recusions have errored then this needs to produce the same thing.
	return output.substr(1,output.size()-1);
}
/*
Function:	string postfixEvaluate(string postfixExpression)
Input:		a string containing a postfix expression
Return:		a string equivalent of the integer value of the expression
Approach:	We will make use of a vector and add every value that we encounter to the vector. We we encounter a operator like /*+- we then apply it to the final two values of the vector and then
			collapse the vector as we go untill we get one final value (hopefully). This is what i should have done with the postfix-prefix conversion but oh well. recursion works well too.
*/
string ExpressionManager::postfixEvaluate(string postfixExpression){
	// Define Variables:
	const string error = "invalid";
	const string validchars = "1234567890/*+-%";
	stringstream expression(postfixExpression);
	string hold;
	vector<int> evaluate_vec;
	if (!(isBalanced(postfixExpression))) return error;	// Test to see if parentheses are balanced.
	while (expression >> hold){	// Iterator through the hole expression.
		for (int iterator1 = 0; iterator1 <= hold.size() - 1; iterator1++)	// Test to make sure that only proper symbols are included.
			if (validchars.find(hold.substr(iterator1, 1)) < 0 || validchars.find(hold.substr(iterator1, 1)) > validchars.size() - 1) return error;
		if (hold == "+" || hold == "-" || hold == "*" || hold == "/" || hold == "%"){	// Test to see if we've hit a operator.
			if (evaluate_vec.size() <= 1) return error;	// If an operator is the current symbol but there's only one or no numbers left then the expression must be invalid.
			else{
				if (hold == "+") evaluate_vec[evaluate_vec.size() - 2] = evaluate_vec[evaluate_vec.size() - 2] + evaluate_vec[evaluate_vec.size() - 1];	// Use the previous two numbers and the operator to compute +
				if (hold == "-") evaluate_vec[evaluate_vec.size() - 2] = evaluate_vec[evaluate_vec.size() - 2] - evaluate_vec[evaluate_vec.size() - 1];	// Use the previous two numbers and the operator to compute -
				if (hold == "*") evaluate_vec[evaluate_vec.size() - 2] = evaluate_vec[evaluate_vec.size() - 2] * evaluate_vec[evaluate_vec.size() - 1];	// Use the previous two numbers and the operator to compute *
				if (hold == "/"){
					if (evaluate_vec[evaluate_vec.size() - 1] == 0) return error;	// We mustn't divide by zero.
					else evaluate_vec[evaluate_vec.size() - 2] = evaluate_vec[evaluate_vec.size() - 2] / evaluate_vec[evaluate_vec.size() - 1];
				}
				if (hold == "%") evaluate_vec[evaluate_vec.size() - 2] = evaluate_vec[evaluate_vec.size() - 2] % evaluate_vec[evaluate_vec.size() - 1];	// Use the previous two numbers and the operator to compute %
				evaluate_vec.pop_back();
			}
		}
		else evaluate_vec.push_back(stoi(hold, nullptr,10));	// Convert the computer integer to string and push it onto the list of numbers.
	}
	if (evaluate_vec.size() > 1) return error;	// No more operators left? Multiple numbers? Uh oh.
	else return to_string(evaluate_vec[0]);	// are function exited sucessfully.
}