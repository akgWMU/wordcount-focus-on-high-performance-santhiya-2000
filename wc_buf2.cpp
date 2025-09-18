#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <cctype>
using namespace std;

const vector<string> STOPWORDS = {
    "and","or","but","if","so","yet","for","nor","although",
    "because","since","unless","until","while","whereas","after",
    "before","once","than","that","though","a","an","the","in",
    "on","at","to","from","by","with","as","of"
};

bool isStopWord(const string &word) {
    return find(STOPWORDS.begin(), STOPWORDS.end(), word) != STOPWORDS.end();
}

void insertSorted(vector<pair<string,int>> &wordList, const string &word) {
    auto it = lower_bound(wordList.begin(), wordList.end(), word,
                          [](const pair<string,int> &a, const string &b) {
                              return a.first < b;
                          });
    if(it != wordList.end() && it->first == word) {
        it->second++;
    } else {
        wordList.insert(it, {word, 1});
    }
}

string cleanWord(const string &word) {
    string cleaned;
    for(char c : word) {
        if(isalpha(c)) cleaned.push_back(tolower(c));
    }
    return cleaned;
}

int main(int argc, char* argv[]) {
    if(argc < 2) {
        cerr << "Usage: " << argv[0] << " <filename>\n";
        return 1;
    }

    string filename = argv[1];
    ifstream fin(filename, ios::binary);
    if(!fin.is_open()) {
        cerr << "❌ Error: Could not open " << filename << endl;
        return 1;
    }
    cout << "✅ Reading: " << filename << endl;

    const size_t BUFFER_SIZE = 1 << 12; // 4KB
    vector<char> buffer(BUFFER_SIZE);
    string partial;
    vector<pair<string,int>> wordList;

    while(fin.read(buffer.data(), BUFFER_SIZE) || fin.gcount()) {
        size_t bytesRead = fin.gcount();
        string chunk(buffer.data(), bytesRead);

        if(!partial.empty()) {
            chunk = partial + chunk;
            partial.clear();
        }

        string word;
        for(size_t i = 0; i < chunk.size(); i++) {
            char c = chunk[i];
            if(isalpha(c)) {
                word.push_back(tolower(c));
            } else {
                if(!word.empty()) {
                    string cleaned = cleanWord(word);
                    cout << "🔎 Found word: " << cleaned << endl;  // DEBUG
                    if(!cleaned.empty() && !isStopWord(cleaned)) {
                        insertSorted(wordList, cleaned);
                    }
                    word.clear();
                }
            }
        }

        if(!word.empty() && isalpha(chunk.back())) {
            partial = word;
        } else if(!word.empty()) {
            string cleaned = cleanWord(word);
            cout << "🔎 Found word: " << cleaned << endl;  // DEBUG
            if(!cleaned.empty() && !isStopWord(cleaned)) {
                insertSorted(wordList, cleaned);
            }
            word.clear();
        }
    }

    if(!partial.empty()) {
        string cleaned = cleanWord(partial);
        cout << "🔎 Found word: " << cleaned << endl;  // DEBUG
        if(!cleaned.empty() && !isStopWord(cleaned)) {
            insertSorted(wordList, cleaned);
        }
    }

    if(wordList.empty()) {
        cout << "⚠️ No non-stopword words found!" << endl;
    } else {
        cout << "\n✅ Final Word List:\n";
        for(auto &p : wordList) {
            cout << p.first << " : " << p.second << endl;
        }
    }

    return 0;
}
