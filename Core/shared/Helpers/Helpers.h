/*  
 *  LanguageDescriptor.h
 *  Defines Helper functions for the application
 *      
 *  
 *  Created: 2/7/2017 by Ryan Tedeschi
 */ 

#ifndef HELPERS_H
#define HELPERS_H

#include <string>
#include <vector>
#include <list>
#include <algorithm>

using namespace std;

struct arg {
    arg(string id, string value) {
        this->id = id;
        this->value = value;
    };
    string id;
    string value;
};

namespace Helpers {
    string ReadFile(string);
    string DupStr(string, int);

    vector<string> ParseArrayArgument(string, vector<arg>);
    string ParseArgument(string, vector<arg>);

    string toLower(string);
    string toUpper(string);
};

template<class T>
class State {
    public:
        State(string id) {
            this->id = id;
        };
        void SetGoal(string token) {
            this->acceptingToken = token;
            isFinal = true;
        };
        void UnsetGoal() {
            this->acceptingToken;
            isFinal = false;
        };
        void AddTransition(State<T>* target, vector<T> input) {
            this->transitionInputs.push_back(input);
            this->transitionStates.push_back(target);
        };
        State<T>* Transition(T input) {
            int len = transitionInputs.size();

            for (int i = 0; i < len; i++) {
                int size = transitionInputs[i].size();

                for (int j = 0; j < size; j++) {
                    if (input == transitionInputs[i][j]) {
                        return transitionStates[i];
                    }
                }
            }

            return NULL;
        };
        string GetId() {
            return id;
        };
        string GetToken() {
            return acceptingToken;
        };
        bool IsGoal() {
            return isFinal;
        };
        void Print() {
            cout << "State '" << id << "'";
            if (isFinal)
                cout << " (GOAL - '" << acceptingToken << "')";
            for (int i = 0; i < transitionStates.size(); i++) {
                cout << "\n\tTransitions to state '" << transitionStates[i]->GetId() << "' with inputs ";
                for (int j = 0; j < transitionInputs[i].size(); j++) {
                    if (j > 0)
                        cout << ", ";
                    cout << transitionInputs[i][j];
                }
            }
            cout << endl;
        };

    private:
        vector<State<T>*> transitionStates;
        vector<vector<T>> transitionInputs;
        string id = "";
        string acceptingToken = "";
        bool isFinal = false;
};

template<class T>
class FSM {
    public:
        FSM() {};

        State<T>* AddState(string id) {
            if (!HasState(id)) {
                State<T>* newState = new State<T>(id);
                states.push_back(newState);
                return newState;
            }
            return NULL;
        };
        void AddTransition(string start, string target, vector<T> transitionInput) {
            State<T>* Start = GetState(start);
            State<T>* Target = GetState(target);
            if (Start != NULL && Target != NULL) {
                Start->AddTransition(Target, transitionInput);
            }
        };
        void SetInitialState(string id) {
            initialState = GetState(id);
        };
        void AddGoal(string id, string token) {
            State<T>* state = GetState(id);
            if (state != NULL) {
                state->SetGoal(token);
            }
        };
        void RemoveGoal(string id) {
            State<T>* state = GetState(id);
            if (state != NULL) {
                state->UnsetGoal();
            }
        };
        bool HasState(string id) {
            return GetState(id) != NULL;
        };
        State<T>* GetState(string id) {
            int size = states.size();
            for (int i = 0; i < size; i++) {
                if (states[i]->GetId() == id)
                    return states[i];
            }
            return NULL;
        };
        bool Transition(T input) {
            if (currentState != NULL) {
                State<T>* nextState = currentState->Transition(input);
                if (nextState != NULL) {
                    currentState = nextState;
                    scannedInput.push_back(input);
                } else
                    complete = true;
            }
            return !complete;
        };
        State<T>* CurrentState() {
            return currentState;
        };
        bool Complete() {
            return complete;
        };
        bool ReachedGoal() {
            return currentState != NULL && currentState->IsGoal();
        };
        string AcceptedToken() {
            if (ReachedGoal())
                return currentState->GetToken();
            return "ERROR";
        };
        void Reset() {
            complete = false;
            currentState = initialState;
            scannedInput.clear();
        };
        vector<T> ScannedInput() {
            return scannedInput;
        }

        void Print() {
            cout << "---- FINITE STATE MACHINE ----\n";
            cout << "Initial State: " << initialState->GetId() << endl;

            for (int i = 0; i < states.size(); i++) {
                states[i]->Print();
            }
        };

    private:
        bool complete = false;
        vector<T> scannedInput;
        vector<State<T>*> states;
        State<T>* initialState = NULL;
        State<T>* currentState = NULL;
};

#endif