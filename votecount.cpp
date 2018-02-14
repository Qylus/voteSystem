//Kyle Howard
//C00207060

#include <stdlib.h>
#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include <string>
#include <time.h>

using namespace std;

ifstream file("candidates.txt");

//				Candidate Entity
//----------------------------------------------//

class Candidate{
	public:
		void setName(string name);
		string getName(void);

		void setParty(string party);
		string getParty(void);

		Candidate(string name, string party); //constructor

	private:
		string cName;
		string cParty;
};

Candidate::Candidate(string name, string party){
	cName = name;
	cParty = party;
	cout << name << " of the " << party << " party.\n";
}

void Candidate::setName(string name){
	cout<< cName;
	cName = name;
	cout<< " name set to "<< cName << ".";
}

void Candidate::setParty(string party){
	cout<< cParty;
	cParty = party;
	cout<< " party set to "<< cParty << ".";
}

string Candidate::getName(){
	return cName;
}

string Candidate::getParty(){
	return cParty;
}


//				BallotPaper Entity
//----------------------------------------------//

class BallotPaper{
	public:
		void setPreference(int pref, int cand);
		int getPreference(int pref);
		int getSize();
		vector<int> getBallot();
		
		BallotPaper(int cCount, string line); //constructor
	
	private:
		vector<int> prefs;
		int size;
};

BallotPaper::BallotPaper(int cCount, string line){
	int pref, split, size = cCount;
	
	prefs.resize(size);
	
	for(int i = 0; i < size; i++){
		stringstream(line) >> pref;
		split = line.find(",");
		prefs[pref-1] = i;
		line = line.substr(split+1);
	}
}

void BallotPaper::setPreference(int pref, int cand){
	prefs[pref-1] = cand;
}

int BallotPaper::getPreference(int pref){
	return prefs[pref];
}

vector<int> BallotPaper::getBallot(){
	return prefs;
}

//				voteCounter Entity
//----------------------------------------------//

int voteCounter(vector<BallotPaper> votes, vector<Candidate> candidates){
	int large, elim, size = (int)candidates.size();
	int count[size];
	bool valid[size], check;
	vector<int> small;
	
	srand(time(0));
	
	for(int i = 0; i < size; i++)
		valid[i] = true;
	
	for(int j = 1; j < size; j++){
		for(int i = 0; i < size; i++)
			count[i] = 0;

		for(int i = 0; i < (int)votes.size(); i++){
			for(int j = 0; j < size; j++){
				if(valid[votes[i].getPreference(j)]){
					count[votes[i].getPreference(j)]++;
					break;
				}
			}
		}
		
		cout << "\n";
		
		for(int i = 0; i < size; i++){
			if(valid[i] && count[i] < 2)
				cout << candidates[i].getName() << ": " << count[i] << " vote.\n";
			else if(valid[i] && count[i] > 1)
				cout << candidates[i].getName() << ": " << count[i] << " votes.\n";

		}
		
		check = true;
		cout << "\n";
		
		for(int i = 0; i < size; i++){
			if(check && valid[i]){
				check = false;
				small.push_back(i);
				large = i;
			}
			
			else if(valid[i]){
				if(count[small[0]] > count[i]){
					small.clear();
					small.push_back(i);
				}
				
				else if(count[small[0]] == count[i])
					small.push_back(i);
				
				if(count[large] < count[i])
					large = i;
			}
		}
		
		if(count[large] * 2 > (int)votes.size()){
			cout << candidates[large].getName()<< " wins the majority vote.\n";
			return large;
		}
		
		if(small.size() > 1){
			cout << "Tie (" << count[small[0]];
			cout << " votes) between: \n";
			for(int i = 0; i < (int)small.size(); i++)
				cout<< candidates[small[i]].getName() << ", " << endl;
		}
		
		elim = small[rand()%small.size()];
		small.clear();
		
		valid[elim] = false;
		cout << "\nCandidate " << candidates[elim].getName() << " eliminated.\n\n";
	}
	
	for(large=0; large<size; large++)
		if(valid[large])
			break;
	
	cout<< "All candidates except "<< candidates[large].getName() << " of the " << candidates[large].getParty();
	cout<< " party have been eliminated.\n";
	return large;
}

//				Main Method
//----------------------------------------------//
	
int main(){
	string line, name, party;
	int split;
	
	vector<Candidate> candidates;
	vector<BallotPaper> votes;
	
	cout << "The candidates are:\n";
	
	while(getline(file, line)){
		if(line == "")
			break;
			
		split = line.find(";");
		name = line.substr(0, split);
		split++;
		party = line.substr(split);
		
		Candidate cand(name, party);
		candidates.push_back(cand);
	}

	while(getline(file, line)){
		BallotPaper temp(candidates.size(), line);
		votes.push_back(temp);
	}
	
	Candidate winner = candidates[voteCounter(votes, candidates)];
	cout<< winner.getName() << " of the " << winner.getParty() << " party wins the election.";
}
