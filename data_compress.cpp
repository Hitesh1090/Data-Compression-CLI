#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <queue>
// #include <chrono>

using namespace std;
// using namespace chrono;

void validateArgs(int argCount) {
    if (argCount != 5) {
        cerr << "Incorrect number of arguments" << endl;
        exit(1);
    }
}

void checkInputFile(const string& fileName) {
    ifstream inputFile(fileName);
    if (!inputFile) {
        cerr << "Wrong file name" << endl;
        exit(1);
    }
}

int getSizeFromFile(const string& fileName) {
    ifstream inputFile(fileName);
    string firstLine;
    if (getline(inputFile, firstLine)) {
        return firstLine.size();
    } else {
        cerr << "Empty file" << endl;
        exit(1);
    }
}

vector<string> parseVectors(const string& fileName) {
    ifstream inputFile(fileName);
    vector<string> vectors;
    string line;

    while (getline(inputFile, line)) {
        vectors.push_back(line);
    }

    return vectors;
}

bool isCompatible(const string& vecA, const string& vecB) {
    for (size_t i = 0; i < vecA.size(); ++i) {
        if (vecA[i] != 'X' && vecB[i] != 'X' && vecA[i] != vecB[i]) {
            return false;
        }
    }
    return true;
}

unordered_map<int, vector<int>> buildGraph(const vector<string>& vectors) {
    unordered_map<int, vector<int>> graph;

    for (size_t i = 0; i < vectors.size(); ++i) {
        for (size_t j = i + 1; j < vectors.size(); ++j) {
            if (isCompatible(vectors[i], vectors[j])) {
                graph[i].push_back(j);
                graph[j].push_back(i);
            }
        }
    }
    return graph;
}

vector<vector<int>> findCliques(unordered_map<int, vector<int>>& graph, int maxSize) {
    vector<vector<int>> result;
    unordered_map<int, int> degreeMap;

    auto cmp = [&](int a, int b) { return degreeMap[a] < degreeMap[b]; };
    priority_queue<int, vector<int>, decltype(cmp)> priorityQueue(cmp);

    for (const auto& [node, neighbors] : graph) {
        degreeMap[node] = neighbors.size();
        priorityQueue.push(node);
    }

    while (!priorityQueue.empty() && (int)result.size() < maxSize) {
        int currentNode = priorityQueue.top();
        priorityQueue.pop();

        if (graph.find(currentNode) == graph.end()) continue;

        vector<int> clique = {currentNode};
        unordered_set<int> candidates(graph[currentNode].begin(), graph[currentNode].end());

        for (int candidate : candidates) {
            if (all_of(clique.begin(), clique.end(), [&](int node) {
                    return find(graph[node].begin(), graph[node].end(), candidate) != graph[node].end();
                })) {
                clique.push_back(candidate);
            }
        }

        result.push_back(clique);

        for (int node : clique) {
            graph.erase(node);
        }

        degreeMap.clear();
        while (!priorityQueue.empty()) priorityQueue.pop();

        for (auto& [node, neighbors] : graph) {
            neighbors.erase(remove_if(neighbors.begin(), neighbors.end(),
                                      [&](int n) { return find(clique.begin(), clique.end(), n) != clique.end(); }),
                            neighbors.end());
            degreeMap[node] = neighbors.size();
            priorityQueue.push(node);
        }
    }
    // cout << "Cliques found:" << endl;
    // for (const auto& clique : result) {
    //     cout << "[ ";
    //     for (int node : clique) {
    //         cout << node + 1 << " ";
    //     }
    //     cout << "]" << endl;
    // }
     return result;
}

string combineVectors(const vector<string>& vectors, const vector<int>& clique) {
    string combined = vectors[clique[0]];

    for (size_t i = 0; i < combined.size(); ++i) {
        for (size_t j = 1; j < clique.size(); ++j) {
            char bit = vectors[clique[j]][i];
            if (combined[i] == 'X') {
                combined[i] = bit;
            } else if (bit != 'X' && bit != combined[i]) {
                combined[i] = 'X';
            }
        }
    }

    return combined;
}

void writeOutput(const string& fileName, const vector<vector<int>>& cliques, const vector<string>& vectors, int maxSize) {
    ofstream outputFile(fileName);

    int writeCount = min(maxSize, (int)cliques.size());
    for (int i = 0; i < writeCount; ++i) {
        outputFile << combineVectors(vectors, cliques[i]) << endl;
    }

    if (maxSize > (int)cliques.size()) {
        cout << "Only " << cliques.size() << " dictionary entries are possible" << endl;
    }
}

int main(int argCount, char* args[]) {
    // auto startTime = high_resolution_clock::now();
    validateArgs(argCount);

    string inputFile = args[1];
    int numEntries = stoi(args[2]);
    int vecLength = stoi(args[3]);
    string outputFile = args[4];

    checkInputFile(inputFile);
    int fileVecLength = getSizeFromFile(inputFile);

    if (fileVecLength != vecLength) {
        cerr << "Mismatch in vector length. Expected: " << vecLength
             << ", Found: " << fileVecLength << endl;
        exit(1);
    }

    vector<string> testVectors = parseVectors(inputFile);
    unordered_map<int, vector<int>> compatibilityGraph = buildGraph(testVectors);
    vector<vector<int>> cliques = findCliques(compatibilityGraph, numEntries);

    writeOutput(outputFile, cliques, testVectors, numEntries);

    // auto endTime = high_resolution_clock::now();
    // auto execDuration = duration_cast<milliseconds>(endTime - startTime).count();

    // auto minutes = execDuration / (1000 * 60);
    // auto seconds = (execDuration % (1000 * 60)) / 1000;
    // auto milliseconds = execDuration % 1000;

    // cout << "Execution Time: ";
    // if (minutes > 0) cout << minutes << " minute(s) ";
    // if (seconds > 0 || minutes > 0) cout << seconds << " second(s) ";
    // cout << milliseconds << " millisecond(s)" << endl;

    return 0;
}
