/*  
 *  LanguageDescriptor.h
 *  Defines the Language Descriptor class, which is the bridge between a text language descriptor file
 *      
 *  
 *  Created: 1/3/2017 by Ryan Tedeschi
 */ 

#include "LanguageDescriptor.h"

using namespace std;

Token::Token(string id, string value) {
    this->id = id;
    this->value = value;
};

void Token::Print() {
    cout << "[" << id << "]\t" << value << endl;
};

string LanguageDescriptorObject::LookupTerminalValue(string terminalID) {
    return terminals[terminalID];
};

void LanguageDescriptorObject::ParseTerminalValues(string data) {

    string t = string(data);
    regex r = regex("T\\([ \t]*(.+)[ \t]*,[ \t]*\"(.*)\"[ \t]*\\)");
    smatch matches;

    while (regex_search(t, matches, r)) {
        string terminalID = matches[1].str();
        string terminalValue = matches[2].str();
        terminals[terminalID] = terminalValue;

        t = matches.suffix().str();
    }
}

void LanguageDescriptorObject::ParseReservedWords(string data) {

    string t = string(data);
    regex r = regex("ReservedWord\\([ \t]*(.+)[ \t]*,[ \t]*(.+)[ \t]*\\)");
    smatch matches;

    while (regex_search(t, matches, r)) {
        string terminalValue = matches[1].str();
        string terminalID = matches[2].str();
        reservedWords[terminalValue] = terminalID;
        terminals[terminalID] = terminalValue;

        t = matches.suffix().str();
    }
}

void LanguageDescriptorObject::ParseFSM(string data) {

    string t = string(data);
    regex r = regex("^\\([ \t]*([^ \t\n]+)[ \t]*,[ \t]*([^ \t\n]+)[ \t]*\\)[ \t]*->[ \t]*([^ \t\n]+)$");
    smatch matches;

    while (regex_search(t, matches, r)) {
        string fromState = matches[1].str();
        string toState = matches[3].str();
        string chars = matches[2].str();
        vector<char> stateTransitions;
        for (int i = 0; i < chars.size(); i++) {
            stateTransitions.push_back(chars[i]);
        }

        stateMachine.AddState(fromState);
        stateMachine.AddState(toState);
        stateMachine.AddTransition(fromState, toState, stateTransitions);

        // cout << "State " << matches[1] << " moves to state " << matches[3] << " with any of the following input: " << matches[2] << endl;
        t = matches.suffix().str();
    }

    t = string(data);
    r = regex("^F\\([ \t]*([^ \t\n]+)[ \t]*,[ \t]*([^ \t\n]+)[ \t]*\\)[ \t]*$");

    while (regex_search(t, matches, r)) {
        string target = matches[1].str();
        string token = matches[2].str();

        stateMachine.AddGoal(target, token);

        // cout << "State " << matches[1] << " accepts token " << matches[2] << endl;
        t = matches.suffix().str();
    }

    t = data;
    r = regex("^I\\([ \t]*([^ \t\n]+)[ \t]*\\)$");

    if (regex_search(t, matches, r)) {
        string target = matches[1].str();

        stateMachine.SetInitialState(target);

        // cout << "State " << matches[1] << " is the initial state" << endl;
        t = matches.suffix().str();
    }

    // stateMachine.Print();
    
}

vector<Token> LanguageDescriptorObject::Tokenize(string input) {
    input += " ";

    vector<Token> tokens;

    string token;
    string str;
    stateMachine.Reset();
    for (int i = 0; i < input.size(); i++) {
        if (!stateMachine.Transition(input[i])) {
            token = stateMachine.AcceptedToken();
            if (token == "ERROR") {
                if (input[i] != ' ' && input[i] != '\n' && input[i] != '\t')
                    cout << "State machine encountered an error on character '" << (int)input[i] << "'\n";
            } else {
                vector<char> inputVector = stateMachine.ScannedInput();
                char* c = (char*)calloc(inputVector.size() + 1, sizeof(char));
                copy(inputVector.begin(), inputVector.end(), c);
                c[inputVector.size()] = '\0';
                str = string(c);

                if (reservedWords[str] != "")
                    token = reservedWords[str];

                // cout << "State machine accepted token '" << token << "' with data '" << str << "'\n";
                i--;
                
                tokens.push_back(Token(token, str));
            }
            stateMachine.Reset();
        }
    }

    // accept the last token
    token = stateMachine.AcceptedToken();
    if (token == "ERROR") {
        // eof is not actually an error because we're adding an extra space to the end
        // cout << "State machine encountered an error on character 'EOF'\n";
    } else {
        vector<char> inputVector = stateMachine.ScannedInput();
        char* c = (char*)calloc(inputVector.size() + 1, sizeof(char));
        copy(inputVector.begin(), inputVector.end(), c);
        c[inputVector.size()] = '\0';
        str = string(c);
        // cout << "State machine accepted token '" << token << "' with data '" << str << "'\n";

        tokens.push_back(Token(token, str));
    }
    stateMachine.Reset();

    return tokens;

}

vector<Token> LanguageDescriptorObject::Tokenize(Markup* input) {
    vector<Token> tokens;

    if (!input->IsLeaf()) {
        vector<Markup*> children = input->Children();

        for (int i = 0; i < children.size(); i++) {
            vector<Token> tl = Tokenize(children[i]);
            tokens.insert(tokens.end(), tl.begin(), tl.end());
        }
    } else {
        Token t(input->GetID(), input->GetData());
        tokens.push_back(t);
    }

    return tokens;

}

LanguageDescriptorObject::LanguageDescriptorObject()
{

}

LanguageDescriptorObject::LanguageDescriptorObject(string file)
{
    Parse(file);
}

LanguageDescriptorObject::~LanguageDescriptorObject() {
    
}

void LanguageDescriptorObject::Parse(string file) {
    // getpath function ?
    FILE* temp = fopen(file.c_str(), "r");
    if (temp == NULL) {
        temp = fopen((CFG_DIR + file + CFG_EXT).c_str(), "r");
        if (temp != NULL) {
            fclose(temp);
            file = CFG_DIR + file + CFG_EXT;
        } else {
            // try PATH environment variable? 
            cout << "Cannot find language file" << endl;
        }
    } else {
        fclose(temp);
    }
    // return file;
    //
    string data = Helpers::ReadFile(file); // TODO: file data should probably already be passed in?
    string t = data;
    ParseTerminalValues(data);
    ParseFSM(data);
    ParseReservedWords(data);
    
    regex r = regex("(.+?)\\s*=:\\s*([^]+?)\\n\\n");
    smatch matches;

    while (regex_search(t, matches, r)) {
        Production* prod = new Production(this, matches[1], matches[2]);
        productions.push_back(prod);
        t = matches.suffix().str();
    }

    // for (int i = 0; i < productions.size(); i++) {
    //     cout << productions[i]->GetId() << ": " << productions[i]->GetRegex() << endl << endl;
    // }

}

vector<Production*> LanguageDescriptorObject::GetProductions() {
    return productions;
}

Production* LanguageDescriptorObject::findProdById(string id) {
    for (int i = 0; i < productions.size(); i++) {
        if (productions[i]->GetId() == id) {
            return productions[i];
        }
    }
    return NULL;
}
int LanguageDescriptorObject::getProdIndex(string id) {
    for (int i = 0; i < productions.size(); i++) {
        if (productions[i]->GetId() == id) {
            return i;
        }
    }
    return -1;
}

vector<Production*> LanguageDescriptorObject::GetOrderedProductions(vector<string> stringlist) {
    vector<Production*> v;

    int size = stringlist.size();
    int* indexer = (int*)calloc(size, sizeof(int));
    int i;
    for (i = 0; i < size; i++) {
        indexer[i] = getProdIndex(stringlist[i]);
    }

    for (i = 1; i < size; i++) {
        if (i > 0 && indexer[i - 1] < indexer[i]) {
            string temps = stringlist[i];
            stringlist[i] = stringlist[i - 1];
            stringlist[i - 1] = temps;
            int tempi = indexer[i];
            indexer[i] =indexer[i - 1];
            indexer[i - 1] = tempi;
            i-=2;
        }
    }

    for (i = 0; i < size; i++) {
        if (i == 0 || indexer[i] != indexer[i-1]) {
            v.push_back(findProdById(stringlist[i]));
        }
    }

    return v;
}


Production::Production(LanguageDescriptorObject* ob, string id, string data) {
    ldo = ob;
    Parse(id, data);
}

TokenMatch* Production::Match(vector<Token> tokens) {
    return Match(tokens, 0);
}

TokenMatch* Production::Match(vector<Token> tokens, int start) {
    TokenMatch* t = rootSet->Match(tokens, start);
    return t;
}

TokenMatch* Production::MatchStrict(vector<Token> tokens) {
    return MatchStrict(tokens, 0);
}

TokenMatch* Production::MatchStrict(vector<Token> tokens, int start) {
    TokenMatch* t = rootSet->MatchStrict(tokens, start);
    return t;
}

void Production::Parse(string id, string data) {
    this->id = id;
    this->data = data;

    rootSet = new ProductionSet(this);
    rootSet->Parse(data);
}

string Production::GetRegex() {

    vector<Production*> prods = GetContainedProductions();
    string t = data;
    regex r;
    smatch matches;

    for (int i = 0; i < prods.size(); i++) {
        Production* prod = prods[i];
        string sub = "(?:" + prod->GetRegex() + ")";
        r = regex("<" + prod->GetId() + ">");

        while (regex_search(t, matches, r)) {
            t = matches.prefix().str() + sub + matches.suffix().str();
        }
    }

    return t;
}

string Production::GetId() {
    return id;
}

vector<Production*> Production::GetContainedProductions() {
    vector<Production*> prods;
    for (int i = 0; i < subproductions.size(); i++) {
        Production* p = ldo->findProdById(subproductions[i]);
        if (p != NULL) {
            prods.push_back(p);
        }
    }
    return prods;
}

LanguageDescriptorObject* Production::GetLDO() {
    return ldo;
}

ProductionSet* Production::GetRootProductionSet() {
    return rootSet;
}

ProductionSet::ProductionSet(Production* parentProduction) {
    prod = parentProduction;
}

void ProductionSet::Parse(string data) {
    source = data;

    string g = "(?:\\(([^\\)]*?)\\))"; // Group
    string te = "(?:\\[(.*?)\\])"; // Terminal
    string p = "(?:<(.*?)>)"; // Production
    string m = "(\\?|\\*|\\+)"; // Multiplicity
    string one = "(" + g + "|" + te + "|" + p + ")[\t ]*" + m + "?"; // One match
    string alt = "(?:[ \t]*\\|[ \t]*)"; // Alternation sequence
    string mult = "(?:" + alt + one + ")*"; // Multiple alternations
    string reg;
    if (type != _Alternation) {
        reg = "(" + one + ")(" + mult + ")";
    } else {
        reg = "(" + one + ")()";
    }

    regex r = regex(reg);
    smatch matches;
    string t = data;

    while (regex_search (t, matches, r)) {
        ProductionSet* newSet = new ProductionSet(prod);

        string group = matches[3].str();
        string terminal = matches[4].str();
        string production = matches[5].str();
        string multiplicity = matches[6].str();
        string alternation = matches[7].str();

        if (alternation == "") {
            if (group != "") {
                newSet->SetGroup(group);
            } else if (terminal != "") {
                newSet->SetTerminal(terminal);
            } else if (production != "") {
                newSet->SetProduction(production);
            }
            newSet->SetMultiplicity(multiplicity);
        } else {
            newSet->SetAlternation(matches[0]);
        }
        children.push_back(newSet);

        t = matches.suffix().str();
    }

}

void ProductionSet::SetGroup(string data) {
    type = _Group;
    Parse(data);
}

void ProductionSet::SetTerminal(string data) {
    type = _Terminal;
    source = data;
}

void ProductionSet::SetProduction(string data) {
    type = _Production;
    source = data;
}

void ProductionSet::SetAlternation(string data) {
    type = _Alternation;
    Parse(data);
}

void ProductionSet::SetMultiplicity(string data) {
    multiplicity = data;
}

TokenMatch* ProductionSet::Match(vector<Token> tokens) {
    return Match(tokens, 0);
}

TokenMatch* ProductionSet::Match(vector<Token> tokens, int startIndex) {
    TokenMatch* match;

    for (int tokenIndex = startIndex; tokenIndex < tokens.size(); tokenIndex++) {
        match = MatchStrict(tokens, tokenIndex);
        if (match != NULL) {
            return match;
        }
    }

    return NULL;
}

Production* ProductionSet::GetProduction() {
    return prod;
}

ProductionSetType ProductionSet::GetType() {
    return type;
}

vector<ProductionSet*> ProductionSet::GetChildren() {
    return children;
}

string ProductionSet::GetSource() {
    return source;
}

string ProductionSet::GetMultiplicity() {
    return multiplicity;
}



TokenMatch* ProductionSet::MatchStrict(vector<Token> tokens, int startIndex) {
    TokenMatch* t = NULL;

    if (type == _Terminal) {
        t = MatchTerminal(tokens, startIndex);
    }
    else if (type == _Alternation) {
        t = MatchAlternation(tokens, startIndex);
    }
    else if (type == _Group || type == _Root) {
        t = MatchGroup(tokens, startIndex);
        if (type == _Root && t != NULL) {
            t->prod = GetProduction()->GetId();
    //     cout << "Matched " << t->prod << endl;
    //     cout << "Matched (" << source << "): count = " << t->length << ", start = " << t->begin << ", end = " << t->end << endl;
    // for (int p = 0; p < t->match.size(); p++) {
    //     cout << "\t" << t->match[p].id << endl;
    // }
    // cout << endl;
        }
    }
    else if (type == _Production) {
        t = MatchProduction(tokens, startIndex);
    }

    return t;
}

TokenMatch* ProductionSet::MatchGroup(vector<Token> tokens, int startIndex) {

    TokenMatch* match = new TokenMatch();
    bool isMatch = true, matched = true;
    int i = startIndex;

    TokenMatch* groupMatch;
    for (int j = 0; j < children.size(); j++) {
        groupMatch = children[j]->MatchStrict(tokens, i);
        if (groupMatch == NULL) {
            matched = false;
            match->submatches.clear();
            break;
        }
        if (groupMatch->length > 0) {
            match->submatches.push_back(groupMatch);
            i += groupMatch->length;
        }
    }

    isMatch = multiplicity != "" || matched;

    if (!isMatch)
        return NULL;

    match->begin = startIndex;
    match->end = i;
    match->length = match->end - match->begin;
    match->match = vector<Token>(&tokens[match->begin], &tokens[match->end]);

    return match;
}
TokenMatch* ProductionSet::MatchTerminal(vector<Token> tokens, int startIndex) {

    if (startIndex >= tokens.size())
        return NULL;

    TokenMatch* match = new TokenMatch();
    bool isMatch = true, matched = false;

    matched = tokens[startIndex].id == source;
    isMatch = multiplicity != "" || matched;

    if (!isMatch)
        return NULL;

    match->begin = startIndex;
    match->end = startIndex + (matched ? 1 : 0);
    match->length = match->end - match->begin;
    match->match = vector<Token>(&tokens[match->begin], &tokens[match->end]);

    return match;
}
TokenMatch* ProductionSet::MatchAlternation(vector<Token> tokens, int startIndex) {

    TokenMatch* match = new TokenMatch();
    bool isMatch = true, matched = false;
    int i = startIndex;

    TokenMatch* alternationMatch = NULL;
    for (int j = 0; j < children.size(); j++) {
        alternationMatch = children[j]->MatchStrict(tokens, i);
        if (alternationMatch != NULL) {
            matched = true;
            if (alternationMatch->length > 0) {
                i += alternationMatch->length;
                match->submatches.push_back(alternationMatch);
            }
            break;
        }
    }

    isMatch = multiplicity != "" || matched;

    if (!isMatch)
        return NULL;

    match->begin = startIndex;
    match->end = i;
    match->length = match->end - match->begin;
    match->match = vector<Token>(&tokens[match->begin], &tokens[match->end]);

    return match;
}
TokenMatch* ProductionSet::MatchProduction(vector<Token> tokens, int startIndex) {

    TokenMatch* match = new TokenMatch();
    bool isMatch = true, matched = false;
    int i = startIndex;

    Production* prod = this->prod->GetLDO()->findProdById(source);
    if (prod != NULL) {
        TokenMatch* prodMatch = prod->GetRootProductionSet()->MatchStrict(tokens, i);
        if (prodMatch != NULL) {
            if (prodMatch->length > 0) {
                i += prodMatch->length;
                match->submatches.push_back(prodMatch);
            }
            matched = true;
        }
    }

    isMatch = multiplicity != "" || matched;

    if (!isMatch)
        return NULL;

    match->begin = startIndex;
    match->end = i;
    match->length = match->end - match->begin;
    match->match = vector<Token>(&tokens[match->begin], &tokens[match->end]);

    return match;
}

Markup* TokenMatch::GenerateMarkup() {
    Markup* r = new Markup(prod);
    string currentData;

    for (int i = 0; i < length; i++) {
        Markup* c;
        TokenMatch* sub = NULL;

        for (int j = 0; j < submatches.size(); j++) {
            if (submatches[j]->begin == i + begin) {
                sub = submatches[j];
                break;
            }
        }

        if (sub != NULL) {
            c = sub->GenerateMarkup();
            if (sub->prod == "") {
                r->AddChildren(c->Children());
            } else {
                r->AddChild(c);
            }
            i += sub->length - 1;
        } else {
            c = new Markup(match[i].id, match[i].value);
            r->AddChild(c);
        }

        if (currentData != "")
            currentData += " ";
        currentData += c->GetData();
    }
    // r->SetData(currentData);

    return r;
}

void TokenMatch::Print(int tab) {
    if (prod != "") {
        for (int p = 0; p < tab; p++) 
            cout << "\t";
        cout << prod << endl;
        tab++;
    }

    for (int i = 0; i < length; i++) {
        TokenMatch* sub = NULL;

        for (int j = 0; j < submatches.size(); j++) {
            if (submatches[j]->begin == i + begin) {
                sub = submatches[j];
                break;
            }
        }

        if (sub != NULL) {
            sub->Print(tab);
            i += sub->length - 1;
        } else {
            for (int p = 0; p < tab; p++) 
                cout << "\t";
            cout << match[i].id << ": " << match[i].value << endl;
        }
    }
}