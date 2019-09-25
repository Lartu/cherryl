#ifndef CHINTER
#define CHINTER
#include <stack>
#include <string>
#include <map>
#include <vector>
#include "chText.hpp"
#include <cmath>
#define cherryEpsilon 0.00000001

using namespace std;
void showError(chText msg);

class cherryType {
    private:
        
    public:
        string value;
};

class cherryState {
    private:
        stack<chText> cherryStack;
        map<chText, chText> variables;
        chText empty = "";
    public:
        stack<chText> getStack() const {
            return cherryStack;
        }
        void addVariable(const chText & variableName, const chText & value){
            variables[variableName] = value;
        }
        void getVariable(const chText & variableName){
            if(variables.find(variableName) == variables.end())
                cherryStack.push("");
            else
                cherryStack.push(variables[variableName]);
        }
        chText & peekVariable(const chText & variableName) {
            if(variables.find(variableName) == variables.end())
                return empty;
            else
                return variables[variableName];
        }
        void push(const chText & value){
            cherryStack.push(value);
        }
        chText pop(){
            if(cherryStack.size() == 0){
                showError("cannot pop from an empty stack.");
            }
            chText val = cherryStack.top();
            cherryStack.pop();
            return val;
        }
        chText top(){
            return cherryStack.top();
        }
};

class cherryInterpreter {
    private:
    vector<chText> cherrySplit(chText code){
        vector<chText> tokens;
        chText currentToken = "";
        bool inString = false;
        bool inComment = false;
        size_t inBlock = 0;
        code += " ";
        for (size_t i = 0; i < code.size(); ++i) {
            if (!inString && !inComment && inBlock == 0 && (code[i] == " " || code[i] == "\t" || code[i] == "\n" || code[i] == "\v" || code[i] == "\f" || code[i] == "\r")){
                if(currentToken.size() > 0){
                    tokens.push_back(currentToken);
                    currentToken = "";
                }
            } else if (!inString && code[i] == ":") {
                inComment = !inComment;
            } else if (!inComment && !inString && code[i] == "(") {
                if(inBlock > 0) currentToken = currentToken + code[i];
                inBlock++;
            } else if (!inComment && !inString && code[i] == ")") {
                inBlock--;
                if(inBlock == 0){
                    tokens.push_back(currentToken);
                    currentToken = "";
                }else{
                    currentToken = currentToken + code[i];
                }
            } else if (code[i] == "\"") {
                inString = !inString;
                if(!inString && !inComment){
                    if(inBlock == 0){
                        tokens.push_back(currentToken);
                        currentToken = "";
                    }
                }
                if(inBlock > 0){
                    currentToken += "\"";
                }
            } else if(!inComment) {
                currentToken = currentToken + code[i];
            }
        }
        return tokens;
    }

    bool isValidVariable(chText & var){
        return var.size() > 1 && var[0] == "$" && var.isAlphanumeric(1);
    }

    void executeTokens(vector<chText> tokens, cherryState & state){
        size_t expectingWhile = 0;
        size_t expectingIf = 0;
        for(size_t i = 0; i < tokens.size(); ++i){
            if(expectingWhile == 0 && expectingIf == 0){
                if (tokens[i] == "lf") {
                    state.push("\r\n");
                }
                else if (tokens[i] == "join") {
                    chText value2 = state.pop();
                    chText value1 = state.pop();
                    state.push(value1 + value2);
                }
                else if (tokens[i] == "display") {
                    cout << state.pop();
                }
                else if (tokens[i] == "accept") {
                    string line;
                    getline(cin, line);
                    state.push(line);
                }
                else if (tokens[i] == "poke") {
                    chText varName = state.pop();
                    if(!isValidVariable(varName))
                        showError(varName + " is not a valid variable.");
                    else
                        state.addVariable(varName, state.pop());
                }
                else if (tokens[i] == "peek") {
                    chText varName = state.pop();
                    if(!isValidVariable(varName))
                        showError(varName + " is not a valid variable.");
                    else
                        state.getVariable(varName);
                }
                else if (tokens[i] == "while") {
                    expectingWhile = 2;
                }
                else if (tokens[i] == "if") {
                    expectingIf = 3;
                }
                else if (tokens[i] == "at") {
                    chText value2 = state.pop();
                    chText value1 = state.pop();
                    if(value2.isNumber()){
                        state.push(value1[value2.getNumber()]);
                    }else{
                        showError("trying index a non-numeric index.");
                    }
                }
                else if (tokens[i] == "len") {
                    chText value = state.pop();
                    state.push(value.size());
                }
                else if (tokens[i] == "varAt") {
                    chText index = state.pop();
                    chText varName = state.pop();
                    if(index.isNumber()){
                        cout << state.peekVariable(varName) << endl;
                        state.push(state.peekVariable(varName)[index.getNumber()]);
                    }else{
                        showError("trying index a non-numeric index.");
                    }
                }
                else if (tokens[i] == "+") {
                    chText value2 = state.pop();
                    chText value1 = state.pop();
                    if(value1.isNumber() && value2.isNumber()){
                        state.push(value1.getNumber() + value2.getNumber());
                    }else{
                        showError("trying to add invalid values.");
                    }
                }
                else if (tokens[i] == "-") {
                    chText value2 = state.pop();
                    chText value1 = state.pop();
                    if(value1.isNumber() && value2.isNumber()){
                        state.push(value1.getNumber() - value2.getNumber());
                    }else{
                        showError("trying to subtract invalid values.");
                    }
                }
                else if (tokens[i] == "*") {
                    chText value2 = state.pop();
                    chText value1 = state.pop();
                    if(value1.isNumber() && value2.isNumber()){
                        state.push(value1.getNumber() * value2.getNumber());
                    }else{
                        showError("trying to multiply invalid values.");
                    }
                }
                else if (tokens[i] == "/") {
                    chText value2 = state.pop();
                    chText value1 = state.pop();
                    if(value1.isNumber() && value2.isNumber()){
                        state.push(value1.getNumber() / value2.getNumber());
                    }else{
                        showError("trying to divide invalid values.");
                    }
                }
                else if (tokens[i] == "%") {
                    chText value2 = state.pop();
                    chText value1 = state.pop();
                    if(value1.isNumber() && value2.isNumber()){
                        state.push((int)value1.getNumber() % (int)value2.getNumber());
                    }else{
                        showError("trying to modulo invalid values.");
                    }
                }
                else if (tokens[i] == "==") {
                    chText value2 = state.pop();
                    chText value1 = state.pop();
                    if(value1.isNumber() && value2.isNumber()){
                        state.push(abs(value1.getNumber() - value2.getNumber()) < cherryEpsilon ? "1" : "0");
                    }else{
                        state.push(value1 == value2 ? "1" : "0");
                    }
                }
                else if (tokens[i] == "!=") {
                    chText value2 = state.pop();
                    chText value1 = state.pop();
                    if(value1.isNumber() && value2.isNumber()){
                        state.push(abs(value1.getNumber() - value2.getNumber()) > cherryEpsilon ? "1" : "0");
                    }else{
                        state.push(value1 == value2 ? "0" : "1");
                    }
                }
                else if (tokens[i] == "<") {
                    chText value2 = state.pop();
                    chText value1 = state.pop();
                    if(value1.isNumber() && value2.isNumber()){
                        state.push(value1.getNumber() < value2.getNumber() ? "1" : "0");
                    }else{
                        state.push(value1 < value2 ? "1" : "0");
                    }
                }
                else if (tokens[i] == "<=") {
                    chText value2 = state.pop();
                    chText value1 = state.pop();
                    if(value1.isNumber() && value2.isNumber()){
                        state.push((value1.getNumber() < value2.getNumber()) 
                        || abs(value1.getNumber() - value2.getNumber()) < cherryEpsilon ? "1" : "0");
                    }else{
                        state.push((value1 < value2) || (value1 == value2) ? "1" : "0");
                    }
                }
                else if (tokens[i] == ">") {
                    chText value2 = state.pop();
                    chText value1 = state.pop();
                    if(value1.isNumber() && value2.isNumber()){
                        state.push(value1.getNumber() < value2.getNumber() ? "0" : "1");
                    }else{
                        state.push(value1 < value2 ? "0" : "1");
                    }
                }
                else if (tokens[i] == ">=") {
                    chText value2 = state.pop();
                    chText value1 = state.pop();
                    if(value1.isNumber() && value2.isNumber()){
                        state.push((value1.getNumber() < value2.getNumber()) 
                        || abs(value1.getNumber() - value2.getNumber()) < cherryEpsilon ? "0" : "1");
                    }else{
                        state.push((value1 < value2) || (value1 == value2) ? "0" : "1");
                    }
                }
                else if (tokens[i] == "dup") {
                    state.push(state.top());
                }
                else if (tokens[i] == "run") {
                    executeTokens(cherrySplit(state.pop()), state);
                }
                else state.push(tokens[i]);
            }
            else if(expectingWhile > 0){
                expectingWhile--;
                state.push(tokens[i]);
                if(expectingWhile == 0){
                    vector<chText> body = cherrySplit(state.pop());
                    vector<chText> guard = cherrySplit(state.pop());
                    while(true){
                        executeTokens(guard, state);
                        if(state.pop() == "0") break;
                        executeTokens(body, state);
                    }
                }
            }
            else if(expectingIf > 0){
                expectingIf--;
                state.push(tokens[i]);
                if(expectingIf == 0){
                    vector<chText> elseBody = cherrySplit(state.pop());
                    vector<chText> body = cherrySplit(state.pop());
                    executeTokens(cherrySplit(state.pop()), state);
                    if(state.pop() == "0") executeTokens(elseBody, state);
                    else executeTokens(body, state);
                }
            }
        }
    }

    public:

    void run(chText code){
        cherryState state;
        executeTokens(cherrySplit(code), state);
    }
    void run(chText code, cherryState & state){
        executeTokens(cherrySplit(code), state);
    }

    //TODO check if passing the tokens by reference or something is faster
};

#endif