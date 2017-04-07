/*  
 *  LanguageDescriptor.h
 *  Defines the Language Descriptor class, which is the bridge between a text language descriptor file
 *      
 *  
 *  Created: 1/3/2017 by Ryan Tedeschi
 */ 

#ifndef LANGUAGE_DESCRIPTOR_H
#define LANGUAGE_DESCRIPTOR_H

#include <vector>
#include <string>
#include <regex>
#include <iostream>
#include <unordered_map>
#include "../Markup/Markup.h"
#include "../Helpers/Helpers.h"

#define CFG_EXT ".cfg"
#define CFG_DIR "./cfg/"

using namespace std;

enum ProductionSetType { _Root, _Terminal, _Group, _Alternation, _Production };

class Production;
class ProductionSet;
class LanguageDescriptorObject;
class TokenMatch;

class Token {
    public:
        Token(string, string);
        string id;
        string value;

    private:

};

class LanguageDescriptorObject
{
    public:
        LanguageDescriptorObject(string);
        LanguageDescriptorObject();
        ~LanguageDescriptorObject();

        vector<Token> Tokenize(string);

        void Parse(string);
        Production* findProdById(string);
        int getProdIndex(string);
        vector<Production*> GetOrderedProductions(vector<string>);
        vector<Production*> GetProductions();

        string LookupTerminalValue(string);

    private:
        void ParseTerminalValues(string);
        void ParseFSM(string);
        void ParseReservedWords(string);

        unordered_map<string, string> terminals;
        vector<string> reservedWords;
        vector<Production*> productions;
        FSM<char> stateMachine;
};

class TokenMatch {
    public:
        string prod;
        int begin;
        int end;
        int length;
        vector<Token> match;
        vector<TokenMatch*> submatches;
    
        Markup* GenerateMarkup();
        void Print(int);

    private:

};

class ProductionSet {
    public:
        ProductionSet(Production*);
        void Parse(string);
        void SetGroup(string);
        void SetTerminal(string);
        void SetProduction(string);
        void SetAlternation(string);
        void SetMultiplicity(string);

        TokenMatch* MatchStrict(vector<Token>, int);
        TokenMatch* Match(vector<Token>, int);
        TokenMatch* Match(vector<Token>);

        Production* GetProduction();

        // Markup Parser(vector<string>);

    private:
        TokenMatch* MatchTerminal(vector<Token>, int);
        TokenMatch* MatchGroup(vector<Token>, int);
        TokenMatch* MatchAlternation(vector<Token>, int);
        TokenMatch* MatchProduction(vector<Token>, int);

        Production* prod;
        enum ProductionSetType type = _Root;
        string source = "";
        vector<ProductionSet*> children;
        string multiplicity = "";
};

class Production {
    public:
        Production(LanguageDescriptorObject*, string, string);
        void Parse(string, string);

        LanguageDescriptorObject* GetLDO();
        ProductionSet* GetRootProductionSet();
        TokenMatch* Match(vector<Token>);
        TokenMatch* Match(vector<Token>, int);
        TokenMatch* MatchStrict(vector<Token>);
        TokenMatch* MatchStrict(vector<Token>, int);

        string GetRegex();
        string GetId();
        vector<Production*> GetContainedProductions();

    private:
        LanguageDescriptorObject* ldo;
        string id;
        string data;
        vector<string> subproductions;
        ProductionSet* rootSet;
};

#endif