

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <set>
#include <queue>

using namespace std;

struct Transition
{
public:
	Transition(int _state, char _letter, int _nextState)
	{
		this->state = _state;
		this->nextState = _nextState;
		this->letter = _letter;
	}

	int state;
	char letter;
	int nextState;
};

class Automata
{
public:
	int letterCount;
	int statesCount;
	vector<int> finalStates;
	vector<Transition> transitions;
	int startState;

	set<int> FindStates(int start, char let)
	{
		set<int> result;
		for (int i = 0; i < transitions.size(); i++)
		{
			if (transitions[i].state == start && transitions[i].letter == let)
			{
				result.insert(transitions[i].nextState);
			}
		}
		return result;
	}

	set<int> FindStatesReverse(int start, char let)
	{
		set<int> result;
		for (int i = 0; i < transitions.size(); i++)
		{
			if (transitions[i].nextState == start && transitions[i].letter == let)
			{
				result.insert(transitions[i].state);
			}
		}
	}

	bool CheckForFit(string w1, string w2)
	{
		set<int> statesAfterW1 = GetStatesAfterW1(w1);

		set<int> allReachedStatesAfterW1 = Bfs(statesAfterW1);

		string reversedW2 = w2;

		reverse(reversedW2.begin(), reversedW2.end());

		set<int> statesAfterW2Reversed = GetStatesAfterW2Reversed(reversedW2);

		bool result = false;
		if (allReachedStatesAfterW1.size() > statesAfterW2Reversed.size())
		{
			for (int i : statesAfterW2Reversed)
			{
				if (allReachedStatesAfterW1.count(i) != 0) return true;
			}
		}
		else
		{
			for (int i : allReachedStatesAfterW1)
			{
				if (statesAfterW2Reversed.count(i) != 0) return true;
			}
		}

		return false;
	}

	set<int> GetStatesAfterW1(string w1)
	{
		set<int> tempStates {startState};
		set<int> nextTempStates;
		for (auto c : w1) 
		{
			for (int i : tempStates)
			{
				for (int st : FindStates(i, c))
				{
					nextTempStates.insert(st);
				}
			}
			tempStates.clear();

			tempStates.insert(nextTempStates.begin(), nextTempStates.end());

			nextTempStates.clear();
		}

		return tempStates;
	}

	set<int> GetStatesAfterW2Reversed(string w2)
	{
		set<int> tempStates;
		tempStates.insert(finalStates.begin(), finalStates.end());
		set<int> nextTempStates;
		for (auto c : w2) //ïåðåáîð ïî áóêâàì ó ñëîâ³
		{
			for (int i : tempStates) //ïî÷èíàºìî ç êîæíîãî ïî÷àòêîâîãî ñòàíó
			{
				for (int st : FindStates(i, c)) 
				{
					nextTempStates.insert(st);
				}
			}
			tempStates.clear(); 

			tempStates.insert(nextTempStates.begin(), nextTempStates.end()); 

			nextTempStates.clear();
		}

		return tempStates;
	}

	set<int> Bfs(set<int> states)
	{
		vector<int> q;
		set<int> visited;
		int head = 0;
		int tail = 0;
		for (int i : states)
		{
			q.push_back(i);
			visited.insert(i);
			head++;
		}
		while (tail != head)
		{
			set<int> tempset;
			for (int i = 0; i < letterCount; i++)
			{
				tempset = FindStates(q[tail], (char)(97 + i));
				for (int j : tempset)
				{
					if (visited.count(j) == 0)
					{
						q.push_back(j);
						++head;
						visited.insert(j);
					}
				}
			}
			tail++;
			
		}
		return visited;

	}
	void ReadAutomata(const string& path)
	{
		ifstream fin;
		fin.open(path);
		if (!fin.is_open())
		{
			cout << "cannot open file";
		}
		else
		{
			string line;
			//read number of letters;
			getline(fin, line);
			this->letterCount = stoi(line);

			//read number of states
			getline(fin, line);
			this->statesCount = stoi(line);

			//read start state
			getline(fin, line);
			this->startState = stoi(line);
			
			//read final states
			getline(fin, line);
			for (auto s : line)
			{
				if (s != ' ')
				{
					int temp = s - '0';

					this->finalStates.push_back(temp);
				}
			}
			

			//read all transitions
			
			while (getline(fin, line))
			{
				int start = line[0] - '0';
				char letter = line[2];
				int nextState = line[4] - '0';
				Transition temp(start, letter, nextState);
				this->transitions.push_back(temp);
			}	
		}
	}
};

int main()
{
	string w1;
	cout << "Enter w1: ";
	cin >> w1;
	string w2;
	cout << "Enter w2: ";
	cin >> w2;
	const string filePath = "test1.txt";
	Automata automata;
	automata.ReadAutomata(filePath);
	if (automata.CheckForFit(w1, w2)) cout << "True";
	else cout << "False";
}
