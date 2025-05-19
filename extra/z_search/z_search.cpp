#include "z_search.h"


std::string::size_type getMaxPrefix(const std::string& text, std::string::size_type lowerIndex, std::string::size_type higherIndex) {
    std::string::size_type prefixLength = 0;
    while (higherIndex < text.size() && lowerIndex < text.size() && text[lowerIndex] == text[higherIndex]) {
        prefixLength++;
        lowerIndex++;
        higherIndex++;
    }
    return prefixLength;
}


std::vector<std::string::size_type> zFunction(const std::string& text) {
    std::string::size_type n = text.size();
    std::vector<std::string::size_type> z_array(n);

    z_array[0] = n;

    std::string::size_type start = 0;
    std::string::size_type end = 0;

    for (std::string::size_type i = 1; i < n; ++i) {
        if (i < end) {
            std::string::size_type j_prefix = i - start;

            if (i + z_array[j_prefix] < end) {
                z_array[i] = z_array[j_prefix];
            } else {
                std::string::size_type chars_matched_beyond_box = getMaxPrefix(text, end - i, end);
                z_array[i] = (end - i) + chars_matched_beyond_box;

                if (i + z_array[i] > end) {
                    start = i;
                    end = i + z_array[i];
                }
            }
        } else {
            z_array[i] = getMaxPrefix(text, 0, i);
            if (z_array[i] > 0) {
                start = i;
                end = i + z_array[i];
            } else {
                start = i;
                end = i;
            }
        }
    }

    return z_array;
}


std::string::size_type zSearch(const std::string& pat, const std::string& txt) {
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

    char separator = '$';

    std::string combined_string = pat + separator + txt;
    std::string::size_type combined_length = combined_string.size();
    std::vector<std::string::size_type> z_array = zFunction(combined_string);

    for (std::string::size_type i = M + 1; i < combined_length; ++i) {
        if (z_array[i] == M) {
            count++;
        }
    }

//    std::cout << "Total count for this Z-function search run: " << count << std::endl;
    return count;

}