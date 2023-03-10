#include <iostream>
#include <fstream>
#include <cstring>
#include "vector"
#include "core.h"

using namespace std;
vector< char *> wordList;

bool isLetter(char c) {
    return (c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z');
}

int getWordList(string filename) {
    int count = 0;
    ifstream kwin;
    kwin.open(filename.c_str(), ios::in);
    if (!kwin) {
        cout << "File Not Exists!" << endl;
    }
    string cur, word;
    while (kwin >> cur) {
        for (int i = 0; i < cur.length(); i++) {
            if (isLetter(cur[i])) {
                char c = tolower(cur[i]);
                word.push_back(c);
            } else {
                char *s = strcpy((char *) malloc(word.length() + 1), word.c_str());
                wordList.push_back(s);
                word.clear();
                count++;
            }
        }
        if (!word.empty()) {
            char *s = strcpy((char *) malloc(word.length() + 1), word.c_str());
            wordList.push_back(s);
            word.clear();
            count++;
        }
    }
    kwin.close();
    return count;
}

int main(int argc, char *argv[]) {
    string filename = "";
    bool num = false;   // 单词链数量
    bool wordSum = false;     // 单词数最多
    bool letterSum = false;     // 字母数最多
    bool ring = false;     // 允许单词环
    char h = 0, t = 0, j = 0; // 字母限制
    bool file_output = false;
    const string outFile = "solution.txt";
    for (int i = 1; i < argc; i++) {
        string arg = argv[i];
        cout << arg << ' ';
        if (arg[0] == '-' && arg.length() == 2) {
            switch (arg[1]) {
                case 'n':
                    num = true;
                    break;
                case 'w':
                    wordSum = true;
                    file_output = true;
                    break;
                case 'c':
                    letterSum = true;
                    file_output = true;
                    break;
                case 'h':
                case 't':
                case 'j': {
                    string letter = argv[i + 1];
                    if (arg[1] == 'h') {
                        h = letter[0];
                    } else if (arg[1] == 't') {
                        t = letter[0];
                    } else {
                        j = letter[0];
                    }
                    i++;
                    break;
                }
                case 'r':
                    ring = true;
                    break;
                default:
                    printf("Error -option\n");
            }
        } else {
            filename = arg;
        }
    }
    cout << endl;
    cout << "num " << num << endl;
    cout << "wordSum " << wordSum << endl;
    cout << "letterSum " << letterSum << endl;
    cout << "ring " << ring << endl;
    cout << "h, t, j: " << h << ' ' << t << ' ' << j << endl;
    cout << "filename: " << filename << endl;
    getWordList(filename);
    //for (int i = 0; i < wordList.size(); i++)
    //    cout << wordList[i] << endl;
    int ret = 0, size = wordList.size();
    vector<char *> result(32768, 0);
    int type = (num) ? 0 : (ring) ? 2 : 1;
    ret = process(wordList.data(), result.data(), size, type, letterSum, h, t, j);

    ofstream output;
    ostream &out = (file_output) ? output : cout;
    if (file_output) {
        output.open(outFile, ios::out | ios::binary | ios::trunc);
        if (!output.is_open()) {
            cout << "No OutPut File" << endl;
        }
    }

    if (num) {
        out << ret << endl;
    }

    for (int i = 0; i < ret; ++i) {
        out << result[i] << endl;
    }

    if (file_output) {
        output.close();
    }

    return 0;
}

