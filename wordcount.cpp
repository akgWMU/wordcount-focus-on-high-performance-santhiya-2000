#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <cctype>
#include <filesystem>
using namespace std;
namespace fs = std::filesystem;

// Stopwords/conjunctions
const vector<string> STOPWORDS = {
    "and","or","but","if","so","yet","for","nor","although",
    "because","since","unless","until","while","whereas","after",
    "before","once","than","that","though","a","an","the","in",
    "on","at","to","from","by","with","as","of"
};

// Remove punctuation and convert to lowercase
string cleanWord(string word) {
    string cleaned;
    for(char c : word) {
        if(isalpha(c)) cleaned += tolower(c);
    }
    return cleaned;
}

// Binary search for word in sorted vector
int binarySearch(const vector<pair<string,int>> &words, const string &word) {
    int left = 0, right = words.size() - 1;
    while(left <= right) {
        int mid = left + (right - left) / 2;
        if(words[mid].first == word) return mid;
        else if(words[mid].first < word) left = mid + 1;
        else right = mid - 1;
    }
    return -1; // Not found
}

// Insert word in sorted vector
void insertWord(vector<pair<string,int>> &words, const string &word) {
    int idx = binarySearch(words, word);
    if(idx != -1) {
        words[idx].second++;
    } else {
        // Find insert position
        auto it = lower_bound(words.begin(), words.end(), make_pair(word, 0),
                              [](const pair<string,int> &a, const pair<string,int> &b){ return a.first < b.first; });
        words.insert(it, make_pair(word, 1));
    }
}

// Print histogram to file
void printHistogram(const vector<pair<string,int>> &words, const string &filename) {
    ofstream fout(filename);
    int maxCount = words.empty() ? 1 : words[0].second; // max frequency for scaling
    int top = min(50, (int)words.size());
    for(int i = 0; i < top; i++) {
        fout << words[i].first << " | ";
        int barLength = (50 * words[i].second) / maxCount; // scale to 50 chars
        fout << string(barLength, '*') << " (" << words[i].second << ")" << endl;
    }
    fout.close();
}

int main() {
    string folderName = "random_text_files";

    vector<pair<string,int>> wordCounts;

    for(const auto &entry : fs::directory_iterator(folderName)) {
        if(entry.is_regular_file()) {
            ifstream fin(entry.path());
            if(!fin.is_open()) {
                cerr << "Error opening file: " << entry.path() << endl;
                continue;
            }
            string word;
            while(fin >> word) {
                string cleaned = cleanWord(word);
                if(cleaned.empty()) continue;
                if(find(STOPWORDS.begin(), STOPWORDS.end(), cleaned) != STOPWORDS.end()) continue;
                insertWord(wordCounts, cleaned);
            }
            fin.close();
        }
    }

    // Sort by frequency (non-increasing)
    sort(wordCounts.begin(), wordCounts.end(), [](const pair<string,int> &a, const pair<string,int> &b){
        return a.second > b.second;
    });

    // Print sorted word counts
    cout << "\nWord Counts:\n";
    for(auto &p : wordCounts) {
        cout << p.first << " : " << p.second << endl;
    }

    // Print histogram
    printHistogram(wordCounts, "histogram.txt");
    cout << "\nHistogram saved to histogram.txt" << endl;

    return 0;
}
