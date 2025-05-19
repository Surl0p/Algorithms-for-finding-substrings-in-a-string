#include "rabin-karp.h"
#include <iostream>
#include <string>

std::string::size_type rabinKarpSearch(const std::string& pat, const std::string& txt)
{
    long long q = 2147483647;
    int d = 256;

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

    long long p = 0;
    long long t = 0;
    long long h = 1;

    for (std::string::size_type i = 0; i < M - 1; i++)
        h = (h * d) % q;

    for (std::string::size_type i = 0; i < M; i++) {
        p = (d * p + (long long)pat[i]) % q;
        if (p < 0) p += q;
        t = (d * t + (long long)txt[i]) % q;
        if (t < 0) t += q;
    }

    for (std::string::size_type i = 0; i <= N - M; i++) {

        if (p == t) {
            bool match = true;
            for (std::string::size_type j = 0; j < M; j++) {
                if (txt[i + j] != pat[j]) {
                    match = false;
                    break;
                }
            }
            if (match) {
                count++;
            }
        }

        if (i < N - M) {

            long long next_t = t;

            next_t = (next_t - (long long)txt[i] * h) % q;

            if (next_t < 0)
                next_t += q;

            next_t = (next_t * d + (long long)txt[i + M]) % q;

            if (next_t < 0)
                next_t += q;

            t = next_t;
        }
    }

//    std::cout << "Total count for this RK search run: " << count << std::endl;

    return count;
}