/*  
 *  LanguageDescriptor.h
 *  Defines the Language Descriptor class, which is the bridge between a text language descriptor file
 *      
 *  
 *  Created: 1/3/2017 by Ryan Tedeschi
 */ 

#include "LanguageDescriptor.h"

using namespace std;

void LanguageDescriptorObject::Parse_Special(string file) {
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

    //------------------

    cout << "SEARCHING" << endl;
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

vector<string> LanguageDescriptorObject::Tokenize(string input) {

    vector<string> tokens;

    string token;
    string str;
    stateMachine.Reset();
    for (int i = 0; i < input.size(); i++) {
        if (!stateMachine.Transition(input[i])) {
            token = stateMachine.AcceptedToken();
            if (token == "ERROR") {
                if (input[i] != ' ' && input[i] != '\n' && input[i] != '\t')
                    cout << "State machine encountered an error on character '" << input[i] << "'\n";
            } else {
                tokens.push_back(token);
                vector<char> inputVector = stateMachine.ScannedInput();
                char* c = (char*)calloc(inputVector.size() + 1, sizeof(char));
                copy(inputVector.begin(), inputVector.end(), c);
                c[inputVector.size()] = '\0';
                str = string(c);
                cout << "State machine accepted token '" << token << "' with data '" << str << "'\n";
                i--;
            }
            stateMachine.Reset();
        }
    }

    // accept the last token
    token = stateMachine.AcceptedToken();
    if (token == "ERROR") {
        cout << "State machine encountered an error on character 'EOF'\n";
    } else {
        tokens.push_back(token);
        vector<char> inputVector = stateMachine.ScannedInput();
        char* c = (char*)calloc(inputVector.size() + 1, sizeof(char));
        copy(inputVector.begin(), inputVector.end(), c);
        c[inputVector.size()] = '\0';
        str = string(c);
        cout << "State machine accepted token '" << token << "' with data '" << str << "'\n";
    }
    stateMachine.Reset();

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

void Production::Parse(string id, string data) {
    this->id = id;
    this->data = data;

    regex r = regex("<(.+?)>");
    smatch matches;
    string t = data;
    
    while (regex_search (t, matches, r)) {
        subproductions.push_back(matches[1].str());
        t = matches.suffix().str();
    }
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

