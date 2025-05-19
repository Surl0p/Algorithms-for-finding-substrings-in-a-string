#include "naive_search.h"

std::string::size_type naiveSearch(const std::string& pat, const std::string& txt) {
    std::string::size_type M = pat.size();
    std::string::size_type N = txt.size();
    std::string::size_type count = 0;

    if (M == 0) {
        std::cout << "Warning: Pattern is empty." << std::endl;
         if (N > 0) {
             count = N + 1;
             std::cout << "(According to one definition, found " << count << " times in non-empty text)" << std::endl;
         } else {
              count = 1;
              std::cout << "(Found 1 time in empty text)" << std::endl;
         }
        return count;
    }
    if (N == 0) {
        std::cout << "Text is empty. Pattern not found." << std::endl;
        return 0;
    }
     if (M > N) {
        std::cout << "Pattern is longer than text. Pattern not found." << std::endl;
        return 0;
    }


    for (std::string::size_type i = 0; i <= N - M; i++) {
        std::string::size_type j;

        for (j = 0; j < M; j++) {
            if (txt[i + j] != pat[j]) {
                break;
            }
        }

        if (j == M) {
            count++;
        }
    }

    //std::cout << "Total count for this BF search run: " << count << std::endl;

    return count;
}