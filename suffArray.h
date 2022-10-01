#ifndef BIOINFORMATICS_SUFFARRAY_H
#define BIOINFORMATICS_SUFFARRAY_H
#pragma once
#include <utility>
#include <vector>
#include <algorithm>

void sort1(std::vector<int> &m, std::string s) {
    int n = s.length();
    std::vector<std::vector<int>> sort;
    sort.reserve(27);
    for (int i = 0; i < 27; ++i) {
        sort.emplace_back();
    }
    for (int i = 0; i < n; ++i) {
        sort[s[i] - 'a'].push_back(i);
    }
    int i = 0;
    int j = 0;
    while (i <= n - 1) {
        while (j <= 26 && sort[j].empty()) {
            j++;
        }
        if (j <= 26) {
            for (int k : sort[j]) {
                m[i] = k;
                i++;
            }
        } else {
            i = n - 1;
        }
        j++;
    }
}

void sort2(std::vector<int> &m, std::vector<int> &c, std::vector<std::vector<int>> &sort, int l, int max) {
    int n = c.size();
    int n2 = max;
    for (int i = 0; i < n; ++i) {
        if (m[i] + l < n) {
            sort[c[m[i] + l]].push_back(m[i]);
        } else {
            sort[0].push_back(m[i]);
        }
    }
    int i = 0;
    int j = 0;
    while (i <= n - 1) {
        while (j <= max && sort[j].empty()) {
            j++;
        }
        if (j <= max) {
            for (int k : sort[j]) {
                m[i] = k;
                i++;
            }
            sort[j].clear();
        } else {
            i = n - 1;
        }
        j++;
    }
    for (int i = 0; i < n; ++i) {
        sort[c[m[i]]].push_back(m[i]);
    }
    i = 0;
    j = 0;
    while (i <= n - 1) {
        while (j <= max && j < sort[j].empty()) {
            j++;
        }
        if (j <= n) {
            for (int k : sort[j]) {
                m[i] = k;
                i++;
            }
            sort[j].clear();
        } else {
            i = n - 1;
        }
        j++;
    }
}

void suffArray(std::vector<int> &m, std::string s) {
    std::vector<int> suff;
    int n = s.length();
    suff.reserve(n);
    for (int i = 0; i < n; ++i) {
        suff.push_back(i);
    }
    sort1(suff, s);
    std::vector<int> c;
    c.reserve(n);
    for (int l = 0; l < n; ++l) {
        c.push_back(0);
    }
    c[suff[0]] = 1;
    int count = 1;
    for (int i = 1; i < n; ++i) {
        int k1 = suff[i], k2 = suff[i - 1];
        if (s[k1] != s[k2]) {
            c[k1] = ++count;
        } else {
            c[k1] = count;
        }
    }
    std::vector<int> c1;
    c1.reserve(n);
    for (int l = 0; l < n; ++l) {
        c1.push_back(0);
    }
    std::vector<std::vector<int>> sort;
    for (int i = 1; i < n; i *= 2) {
        if (count + 1 > sort.size()) {
            for (int j = sort.size(); j < count + 2; ++j) {
                sort.emplace_back();
            }
        }
        sort2(suff, c, sort, i, count + 1);
        c1[suff[0]] = 1;
        count = 1;
        for (int l = 1; l < n; ++l) {
            int l1 = suff[l], l2 = suff[l - 1];
            int k1 = -1, k2 = -1;
            if (l2 + i < n) {
                k2 = c[l2 + i];
            }
            if (l1 + i < n) {
                k1 = c[l1 + i];
            }
            if (c[l1] != c[l2] || k1 != k2) {
                c1[l1] = ++count;
            } else {
                c1[l1] = count;
            }
        }
        c = c1;
    }
    m = suff;
}

void LCP(std::vector<int> &m, std::vector<int> &suff, const std::string &s) {
    int n = s.length();
    std::vector<int> lcp, pos;
    lcp.reserve(n);
    pos.reserve(n);
    for (int l = 0; l < n; ++l) {
        lcp.push_back(0);
        pos.push_back(0);
    }
    for (int i = 0; i < n; ++i) {
        pos[suff[i]] = i;
    }
    int k = 0;
    for (int i = 0; i < n; ++i) {
        if (k > 0) {
            k--;
        }
        if (pos[i] == n - 1) {
            lcp[n - 1] = -1;
            k = 0;
            continue;
        } else {
            int j = suff[pos[i] + 1];
            while (std::max(i + k, j + k) < n && s[i + k] == s[j + k]) {
                k++;
            }
            lcp[pos[i]] = k;
        }
    }
    m = lcp;
}

int lower_bound(int l, int r, int i, const std::vector<int> &array, std::string original, std::string pattern) {
    while (l < r - 1)
    {
        auto  m = (l + r) / 2   ;
        if (original[array[m] + i] < pattern[i])
            l = m;
        else
            r = m;
    }
    return l;
}
int upper_bound(int l, int r, int i, const std::vector<int> &array, std::string original, std::string pattern) {
    while (l < r - 1)
    {
        auto  m = (l + r) / 2   ;
        if (original[array[m] + i] <= pattern[i])
            l = m;
        else
            r = m;
    }
    return r;
}

std::pair<int, int> elementary_search(std::string p, std::string s, const std::vector<int> &array) {
    auto left = 0;
    auto right = p.size();
    for (int i = 0; i < s.size(); ++i) {
        left = lower_bound(left, right, i, array, p, s);
        right = upper_bound(left, right, i, array, p, s);
    }
    if (right - left >= 0) {
        return std::pair(left, right);
    } else {
        return std::pair(-1,-1);
    }
}
void tolower(std::string & str){
    std::for_each(str.begin(), str.end(), [](char & c){
        c = std::tolower(c);
    });
}

#endif //BIOINFORMATICS_SUFFARRAY_H
