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
#include <unordered_map>
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
            for (int i = 0; i < input.size(); i++) {
                transitions[input[i]] = target;
            }
        };
        State<T>* Transition(T input) {
            return transitions[input];
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
        unordered_map<T, State<T>*> transitions;
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
                states[id] = newState;
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
            return states[id];
        };
        string Transition(T input) {
            string ret = "";
            if (currentState != NULL) {
                State<T>* nextState = currentState->Transition(input);
                if (nextState != NULL) {
                    currentState = nextState;
                } else {
                    if (currentState->IsGoal()) {
                        ret = currentState->GetToken();
                        Reset();
                    } else
                        ret = "ERROR";
                }
            } else {
                ret = "ERROR";
                Reset();
            }
            return ret;
        };
        State<T>* CurrentState() {
            return currentState;
        };
        void Reset() {
            currentState = initialState;
        };

        void Print() {
            cout << "---- FINITE STATE MACHINE ----\n";
            cout << "Initial State: " << initialState->GetId() << endl;

            for ( auto it = states.begin(); it != states.end(); ++it )
                it->second->Print();
        };

    private:
        unordered_map<string, State<T>*> states;
        State<T>* initialState = NULL;
        State<T>* currentState = NULL;
};

#endif