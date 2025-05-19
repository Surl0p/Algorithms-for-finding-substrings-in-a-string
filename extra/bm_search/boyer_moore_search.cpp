#include "boyer_moore_search.h"


std::map<char, std::string::size_type> computeBadCharacterRule(const std::string& pattern) {
    std::map<char, std::string::size_type> last_occurrence;
    std::string::size_type m = pattern.length();

    for (std::string::size_type i = 0; i < m; ++i) {
        last_occurrence[pattern[i]] = i;
    }
    return last_occurrence;
}

std::vector<std::string::size_type> computeBordersAndStrongSuffixShifts(const std::string& pat, std::vector<std::string::size_type>& borders) {
    std::string::size_type m = pat.length();
    std::vector<std::string::size_type> shift(m + 1, 0);
    std::string::size_type i = m, j = m + 1;
    borders[i] = j;

    while (i > 0) {
        while (j <= m && pat[i - 1] != pat[j - 1]) {
            if (shift[j] == 0) {
                shift[j] = j - i;
            }
            j = borders[j];
        }
        i--; j--;
        borders[i] = j;
    }
    return shift;
}

void finalizeGoodSuffixShifts(std::vector<std::string::size_type>& shift, const std::vector<std::string::size_type>& borders, std::string::size_type m) {
    std::string::size_type j = borders[0];
    for (std::string::size_type i = 0; i <= m; i++) {
        if (shift[i] == 0) {
            shift[i] = j;
        }
        if (i == j) {
            j = borders[j];
        }
    }
}

std::vector<std::string::size_type> computeGoodSuffixRule(const std::string& pattern) {
    std::string::size_type m = pattern.length();
    std::vector<std::string::size_type> borders(m + 1, 0);
    std::vector<std::string::size_type> shift = computeBordersAndStrongSuffixShifts(pattern, borders);

    finalizeGoodSuffixShifts(shift, borders, m);

    return shift;
}

std::string::size_type boyerMooreSearch(const std::string& pat, const std::string& txt) {
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

    std::map<char, std::string::size_type> bad_char_shift = computeBadCharacterRule(pat);
    std::vector<std::string::size_type> good_suffix_shift_table = computeGoodSuffixRule(pat);
    std::string::size_type s = 0;

    while (s <= N - M) {
        ptrdiff_t j = static_cast<ptrdiff_t>(M) - 1;

        while (j >= 0 && pat[static_cast<std::string::size_type>(j)] == txt[s + static_cast<std::string::size_type>(j)]) {
            j--;
        }

        if (j < 0) {
            count++;
            s += good_suffix_shift_table[0];

        } else {
            char char_at_mismatch = txt[s + static_cast<std::string::size_type>(j)];
            ptrdiff_t last_occ_index = bad_char_shift.count(char_at_mismatch) ? static_cast<ptrdiff_t>(bad_char_shift.at(char_at_mismatch)) : -1;
            ptrdiff_t shift_bc = j - last_occ_index;
            auto shift_gs = static_cast<ptrdiff_t>(good_suffix_shift_table[static_cast<std::string::size_type>(j) + 1]);
            ptrdiff_t shift_amount_signed = std::max(static_cast<ptrdiff_t>(1), std::max(shift_bc, shift_gs));
            s += static_cast<std::string::size_type>(shift_amount_signed);
        }
    }

    //std::cout << "Total count for this Boyer-Moore search run: " << count << std::endl;
    return count;

}