#include "kmp_search.h"


std::vector<int> computeLPS(const std::string& pat) {
    std::string::size_type m = pat.size();
    std::vector<int> lps(m, 0);

    int len = 0;
    std::string::size_type i = 1;

    while (i < m) {
        if (pat[i] == pat[len]) {
            len++;
            lps[i] = len;
            i++;
        } else {
            if (len != 0) {
                len = lps[len - 1];
            } else {
                lps[i] = 0;
                i++;
            }
        }
    }
    return lps;
}

std::string::size_type kmpSearch(const std::string& pat, const std::string& txt) {
    std::string::size_type N = txt.size();
    std::string::size_type M = pat.size();
    std::string::size_type count = 0;

    if (M == 0) {
        std::cout << "Warning: Pattern is empty." << std::endl;
        if (N > 0) count = N + 1; else count = 1;
        std::cout << "(According to one definition, found " << count << " times in " << (N > 0 ? "non-empty text" : "empty text") << ")" << std::endl;
        return count;
    }
    if (N == 0 || M > N) {
        if (N == 0) std::cout << "Text is empty. Pattern not found." << std::endl;
        else std::cout << "Pattern is longer than text. Pattern not found." << std::endl;
        return 0;
    }

    std::vector<int> lps = computeLPS(pat);
    std::string::size_type i = 0;
    std::string::size_type j = 0;

    while (i < N) {
        if (pat[j] == txt[i]) {
            i++;
            j++;

            if (j == M) {
                count++;
                j = lps[j - 1];
            }
        } else {
            if (j != 0) {
                j = lps[j - 1];
            } else {
                i++;
            }
        }
    }

//    std::cout << "Total count for this KMP search run: " << count << std::endl;

    return count;
}