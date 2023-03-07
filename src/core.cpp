//
// Created by 徐子航 on 2023/3/6.
//
#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include <map>

using namespace std;

typedef long long ll;
vector<string> words;
//const int n = 26;
int wordSize;

void init(const char *wordList[], int len, char j) {
    words.clear();
    for (int i = 0; i < len; ++i) {
        string word = wordList[i];
        if (j != 0 && word[0] == j) // 若存在-j参数, 则直接在初始阶段删除
            continue;
        if (word.length() > 1)
            words.push_back(word);
    }
    wordSize = (int) words.size();
}

vector<int> node[30];
bool book[30];
int stack1[30], top = 0;    // stack[]: 记录寻找过程
bool stackBook[30];
// dfn[]: 搜索顺序数组, low[]: 强连通分量数组
int dfn[30], low[30], cnt = 0;
int col[30], tot;

void dfsScc(int i) {    // tarjan
    stack1[++top] = i;  // 入栈
    stackBook[i] = true, book[i] = true;
    cnt++;
    dfn[i] = cnt, low[i] = cnt; // 记录路径和强连通分量
    for (int j: node[i]) {
        int to = words[j].back() - 'a'; // endLetter
        if (book[to]) {
            if (stackBook[to])
                low[i] = min(low[i], low[to]);
        } else {
            dfsScc(to);
            low[i] = min(low[i], low[to]);
        }
    }
    if (dfn[i] == low[i]) {
        col[i] = ++tot; // 强连通分量号
        while (1) { // 给该分支所有点标号
            int cur = stack1[top--];
            stackBook[cur] = false;
            if (cur == i)
                break;
            col[cur] = col[i];
        }
    }
}

void getSCC() { // tarjan启动
    cnt = 0, tot = 0;
    for (int i = 0; i < 26; ++i) {
        node[i].clear();
        book[i] = false;
    }
    for (int i = 0; i < wordSize; ++i) {
        int firstLetter = words[i][0] - 'a';
        node[firstLetter].push_back(i);
    }
    for (int i = 0; i < 26; ++i) {
        if (book[i] == false)
            dfsScc(i);
    }
}

void checkLoop() {
    for (int i = 0; i < 26; ++i) {  // 有两个以上首尾字母相同的单词
        int last = -1;
        for (int j: node[i]) {
            if (words[j].back() - 'a' == i) {
                if (last == -1)
                    last = j;
                else
                    cout << "Ring detected!" << endl;
            }
        }
    }
    if (tot < 26) {
        bool siz[30] = {0};
        bool vis[30] = {0};
        for (int i = 0; i < 26; ++i) {  // 遍历每一个字母开头的单词
            if (siz[col[i]]) {  // 若该字母对应的强连通分量之前已经访问过
                string str;
                int cur = i;
                while (!vis[cur]) {
                    vis[cur] = true;
                    for (int j: node[cur]) {
                        int to = words[j].back() - 'a';
                        if (col[to] == col[i]) {
                            if (!str.empty())
                                str += " ";
                            str += words[j];
                            break;
                        }
                    }
                }
                cout << "Ring detected!" << endl;
            }
            siz[col[i]] = true; // 当前字母对应强连通分量已访问
        }
    }
}

vector<string> ans;
vector<int> now;
int ans_cnt = 0;

void dfsAll(int i, bool loop) {
    if (now.size() > 1) {   // add new ans
        string S;
        for (int j: now) { // 当前vector中单词
            if (!S.empty())
                S += " ";
            S += words[j];
        }
        ans.push_back(S);
        ans_cnt++;
    }
    for (int j: node[i]) { // 当前字母开头的所有单词
        int to = words[j].back() - 'a';
        if (i == to && loop)    // 自环
            continue;
        now.push_back(j);
        dfsAll(to, i == to);    // 继续向下查找
        now.pop_back();
    }
}

void transResult(char *result[]) {  // 转化为结果输出
    int siz = 0;
    for (auto &i: ans)
        siz += (int) i.length() + 1;
    // 分配新内存解决浅拷贝问题
    char *p = (char *) malloc((size_t) siz + 1);
    for (int i = 0; i < ans.size(); i++) {
        result[i] = p;
        for (char j: ans[i]) {
            if (p != nullptr)
                *p = j;
            p++;
        }
        if (p != nullptr)
            *p = 0;
        p++;
    }
    ans.clear();
}

int getAllChains(char *result[]) {
//    for (int i = 0; i < 26; ++i) {
//        dfs_cal_all(i, false);
//    }
    for (int i = 0; i < 26; ++i) {
        dfsAll(i, false);
    }
    int ret = ans.size();
    transResult(result);
    return ret;
}

int getMaxChain(char *result[], bool letterSum, char head, char tail) {
    int a[26] = {};
    for (int i = 0; i < 26; ++i) // 将强连通分量对应为字母
        a[col[i] - 1] = i;
    int f[26] = {}, g[26] = {}, h[26] = {};
    // f: 边权值, g: 后继单词, h: 自环
    for (int i = 0; i < 26; ++i) {
        int cur = a[i]; // 当前强连通分量
        f[cur] = (tail == 0 || tail - 'a' == cur) ? 0 : -1e9;
        g[cur] = -1, h[cur] = -1;
        for (int j: node[cur]) {   // 遍历该字母开头的所有单词
            int to = words[j].back() - 'a';
            if (to == cur)
                continue;
            int weight = f[to]; // 如果字母数最多则词长作为边权,否则为1
            weight += (letterSum) ? (int) words[j].length() : 1;
            if (weight > f[cur]) {
                f[cur] = weight;
                g[cur] = j;
            }
        }
        // if(!loop) continue;
        for (int j: node[cur]) {    // 处理自环
            int to = words[j].back() - 'a';
            if (to == cur) {    // 找一条即可
                f[cur] += (letterSum) ? (int) words[j].length() : 1;
                h[cur] = j;
                break;
            }
        }
    }
    int cur = -1, sum = -1e9;
    for (int i = 0; i < wordSize; ++i) {    // 遍历所有单词
        if (head == 0 || head == words[i][0]) {
            int from = words[i][0] - 'a', to = words[i].back() - 'a';
            if (from == to) {
                // 检测是否允许自环, 本作业允许, 可删掉
                // if(f[from] == (letterSum ? words[i].length() : 1)) continue;
                if (sum < f[from]) {
                    sum = f[from];
                    cur = i;
                }
            } else {
                if (f[to] <= 0)
                    continue;
                int tmp = f[to] + (letterSum ? (int) words[i].length() : 1);
                if (sum < tmp) {
                    sum = tmp;
                    cur = i;
                }
            }
        }
    }
    if (cur != -1) {    // 存在路径
        if (words[cur][0] == words[cur].back()) {   // 自环
            cur = words[cur][0] - 'a';
        } else {    // 向后移动
            ans.push_back(words[cur]);
            cur = words[cur].back() - 'a';
        }
        while (1) {
            if (h[cur] != -1)   // 自环
                ans.push_back(words[h[cur]]);
            if (g[cur] == -1)   // 后继
                break;
            ans.push_back(words[g[cur]]);
            cur = words[g[cur]].back() - 'a';
        }
    }
    int len = ans.size();
    transResult(result);
    return len;
}

map<tuple<ll, ll, int>, pair<int, int>> mp;
pair<ll, ll> val;
vector<pair<int, int>> w[26][26];   // 以i开头, 以j结尾的单词的权重和id
vector<int> v[26];  // 以i开头的单词的尾字母
int pos[26][26];
int last;

void transVal(int i) {  // 映射
    if (i < 64)
        val.first += (1ll << i);
    else
        val.second += 1ll << (i - 64);
}

void dfsRing(int i) {
    tuple<ll, ll, int> tuple = make_tuple(val.first, val.second, i);
    if (mp.find(tuple) != mp.end())
        return;
    int id = -1, ans = -1e9;
    if (last == -1 || i == last)
        ans = 0;
    pair<ll, ll> tmpVal = val;
    // 优先跑自环的情况
    if (pos[i][i] < w[i][i].size()) {   // i -> i 的路径数 < 满足条件的总单词数
        transVal(w[i][i][pos[i][i]].second);
        pos[i][i]++;
        dfsRing(i);
        tuple = make_tuple(val.first, val.second, i);
        ans = mp[tuple].first + w[i][i][pos[i][i] - 1ll].first;
        id = i;
        val = tmpVal;
        pos[i][i]--;
    } else {    // 找其他出边
        for (int j: v[i]) { // 遍历出边
            if (pos[i][j] < w[i][j].size()) {   // 还有单词未遍历
                if (col[i] == col[j])   // 在同一强连通分量
                    transVal(w[i][j][pos[i][j]].second);
                else
                    val = {0, 0};
                pos[i][j]++;
                dfsRing(j);
                tuple = make_tuple(val.first, val.second, j);
                int sum = mp[tuple].first + w[i][j][pos[i][j] - 1ll].first;
                if (ans < sum) {
                    ans = sum;
                    id = j;
                }
                val = tmpVal;
                pos[i][j]--;
            }
        }
    }
    tuple = make_tuple(val.first, val.second, i);
    mp[tuple] = {ans, id};
}

int getRingChain(char *result[], bool letterSum, char head, char tail) {
    mp.clear();
    val = {0, 0};
    for (int i = 0; i < 26; ++i) {
        for (int j = 0; j < 26; ++j) {
            pos[i][j] = 0;
            w[i][j].clear();
        }
    }
    for (int i = 0; i < 26; i++)
        v[i].clear();
    for (int i = 0; i < wordSize; ++i) {
        int first = words[i][0] - 'a', final = words[i].back() - 'a';
        int weight = (letterSum) ? (int) words[i].length() : 1;
        w[first][final].push_back({weight, i}); // 记录边权值 & 对应单词
        v[first].push_back(final);  // 记录出边(尾字母)
    }
    for (int i = 0; i < 26; ++i) {
        for (int j = 0; j < 26; ++j) {
            sort(w[i][j].begin(), w[i][j].end(), greater<pair<int, int>>());
        }
    }
    for (int i = 0; i < 26; ++i) {
        sort(v[i].begin(), v[i].end()); // 去重 首尾字母相同可能多个单词
        v[i].erase(unique(v[i].begin(), v[i].end()), v[i].end());
    }
    last = (tail != 0) ? tail - 'a' : -1;
    for (int i = 0; i < 26; ++i) {  // 遍历所有字母开头情况
        if (head == 0 || head - 'a' == i)
            dfsRing(i);
    }
    int cur = -1, sum = -1e9;
    for (int i = 0; i < wordSize; ++i) {    // 遍历词表
        if (head == 0 || head == words[i][0]) { // 首字母对应单词
            int from = words[i][0] - 'a', to = words[i].back() - 'a';
            val = {0, 0};
            if (col[from] == col[to])   // 在同一强连通分量上
                transVal(i);
            tuple<ll, ll, int> tuple = make_tuple(val.first, val.second, to);
            if (mp[tuple].first <= 0)   // 负边权
                continue;
            int tmp = mp[tuple].first + (letterSum ? words[i].length() : 1);
            if (sum < tmp) {
                sum = tmp;
                cur = i;
            }
        }
    }
    val = {0, 0};
    if (cur != -1) {    // 有路径
        ans.push_back(words[cur]);
        int first = words[cur][0] - 'a', final = words[cur].back() - 'a';
        if (col[first] == col[final]) { // 自环
            transVal(cur);
            pos[first][final]++;
        }
        cur = words[cur].back() - 'a';  // 向后传递
        while (1) {
            tuple<ll, ll, int> tuple = make_tuple(val.first, val.second, cur);
            int j = mp[tuple].second;
            if (j == -1)    // 终止
                break;
            ans.push_back(words[w[cur][j][pos[cur][j]].second]);
            if (col[cur] == col[j])
                transVal(w[cur][j][pos[cur][j]].second);
            else
                val = {0, 0};
            pos[cur][j]++;  // cur单词 -> j单词
            cur = j;
        }
    }
    int len = (int) ans.size();
    transResult(result);
    return len;
}


//int
//preProcess(const char *wordList[], char *result[], int len, bool num, bool letterSum, bool ring, char head, char tail,
//           char j) {
//    int type = (num) ? 0 : (ring) ? 2 : 1;
//    return 0;
//}

/*
* type = 0, enable self loop, disable ring, get all chains.
* type = 1, enable self loop, disable ring, get max chain.
* type = 2, enable ring, get max chain.
*/

int process(const char *wordList[], char *result[], int len,
            int type, bool letterSum, char head, char tail, char j) {
    init(wordList, len, j);
    sort(words.begin(), words.end());
    words.erase(unique(words.begin(), words.end()), words.end());
    wordSize = (int) words.size();
    getSCC();
    if (type < 2) {
        checkLoop();    // not -r
    }
    if (type == 0)  // -n
        return getAllChains(result);
    if (type == 2)  // -r
        return getRingChain(result, letterSum, head, tail);
    return getMaxChain(result, letterSum, head, tail);  // -w & -c
}
