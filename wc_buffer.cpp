#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <algorithm>
#include <cctype>
#include <filesystem>
#include <thread>
#include <mutex>
using namespace std;
namespace fs = std::filesystem;

const vector<string> STOPWORDS = {
    "and","or","but","if","so","yet","for","nor","although",
    "because","since","unless","until","while","whereas","after",
    "before","once","than","that","though","a","an","the","in",
    "on","at","to","from","by","with","as","of"
};

string cleanWord(const string &word) {
    string cleaned;
    for(char c : word)
        if(isalpha(c)) cleaned += tolower(c);
    return cleaned;
}

bool isStopWord(const string &word) {
    return find(STOPWORDS.begin(), STOPWORDS.end(), word) != STOPWORDS.end();
}

void printHistogram(const vector<pair<string,int>> &words, const string &filename) {
    ofstream fout(filename);
    int maxCount = words.empty() ? 1 : words[0].second;
    int top = min(50, (int)words.size());
    for(int i=0; i<top; i++){
        fout << words[i].first << " | ";
        int barLength = (50 * words[i].second) / maxCount;
        fout << string(barLength, '*') << " (" << words[i].second << ")" << endl;
    }
    fout.close();
}

// Thread function to process a batch of files
void processFiles(const vector<fs::path> &files, map<string,int> &localCounts) {
    for(const auto &file : files) {
        ifstream fin(file);
        if(!fin.is_open()) continue;

        string line;
        while(getline(fin, line)) {
            istringstream iss(line);
            string word;
            while(iss >> word) {
                string cleaned = cleanWord(word);
                if(cleaned.empty() || isStopWord(cleaned)) continue;
                localCounts[cleaned]++;
            }
        }
    }
}

int main() {
    string folderName = "random_text_files";

    // Collect all file paths
    vector<fs::path> allFiles;
    for(const auto &entry : fs::directory_iterator(folderName)) {
        if(entry.is_regular_file())
            allFiles.push_back(entry.path());
    }

    int numThreads = thread::hardware_concurrency(); // Use max available cores
    if(numThreads == 0) numThreads = 4; // fallback
    vector<thread> threads;
    vector<map<string,int>> localMaps(numThreads);

    // Split files among threads
    size_t chunkSize = (allFiles.size() + numThreads - 1) / numThreads;
    for(int t = 0; t < numThreads; t++) {
        size_t start = t * chunkSize;
        size_t end = min(start + chunkSize, allFiles.size());
        vector<fs::path> subset(allFiles.begin() + start, allFiles.begin() + end);
        threads.emplace_back(processFiles, subset, ref(localMaps[t]));
    }

    // Join threads
    for(auto &th : threads) th.join();

    // Merge local maps into global map
    map<string,int> globalCounts;
    for(const auto &local : localMaps) {
        for(const auto &p : local)
            globalCounts[p.first] += p.second;
    }

    // Copy to vector and sort by frequency
    vector<pair<string,int>> sortedWords(globalCounts.begin(), globalCounts.end());
    sort(sortedWords.begin(), sortedWords.end(), [](const auto &a, const auto &b){
        return a.second > b.second;
    });

    // Print counts
    for(auto &p : sortedWords)
        cout << p.first << " : " << p.second << endl;

    // Print histogram
    printHistogram(sortedWords, "histogram_buf.txt");
    cout << "\nHistogram saved to histogram_buf.txt" << endl;

    return 0;
}
