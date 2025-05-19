#include <iostream>
#include <string>

#include "/Users/ivanpavlov/Desktop/cw-2/extra/utils/utils.h"
#include "/Users/ivanpavlov/Desktop/cw-2/extra/Brute_Force_algorithm/naive_search.h"
#include "/Users/ivanpavlov/Desktop/cw-2/extra/Karp-Rabin_algorithm/rabin-karp.h"
#include "/Users/ivanpavlov/Desktop/cw-2/extra/kmp_search/kmp_search.h"
#include "/Users/ivanpavlov/Desktop/cw-2/extra/bm_search/boyer_moore_search.h"
#include "/Users/ivanpavlov/Desktop/cw-2/extra/z_search/z_search.h"
#include "/Users/ivanpavlov/Desktop/cw-2/extra/aho_corasick/aho_corasick.h"


void runTimedTest(const std::function<std::string::size_type(const std::string&, const std::string&)>& searchFunc,
                  const std::string& algorithmName,
                  const std::string& pat, const std::string& txt, int numIterations)
{
    if (numIterations <= 0) {
        std::cerr << "Error [" << algorithmName << "]: Number of iterations must be positive." << std::endl;
        return;
    }


    std::cout << "\n--- Timing " << numIterations << " searches for " << algorithmName << " ---" << std::endl;

    auto start_time = std::chrono::high_resolution_clock::now();

    std::string::size_type total_occurrences_sum = 0;

    for (int i = 0; i < numIterations; ++i) {
        total_occurrences_sum += searchFunc(pat, txt);
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration_us = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
    auto duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);

    double average_occurrences_per_search = static_cast<double>(total_occurrences_sum) / numIterations;
    double average_time_per_search = static_cast<double>(duration_ms.count()) / numIterations;

    std::cout << "\nThe average time for " << numIterations << " searches (" << algorithmName << "): " << average_time_per_search << " ms" << std::endl;
    std::cout << "All occurrences per search (" << algorithmName << "): " << average_occurrences_per_search << std::endl;
}

#include <vector>


void runMultiPatternTimedTest(const std::function<std::string::size_type(const std::string&, const std::string&)>& singlePatternSearchFunc,
                              const std::string& algorithmName,
                              const std::vector<std::string>& patterns,
                              const std::string& txt,
                              int numIterations)
{
    if (numIterations <= 0) {
        std::cerr << "Error [" << algorithmName << "]: Number of iterations must be positive." << std::endl;
        return;
    }
    if (patterns.empty()) {
        std::cout << "\n--- Skipping multi-pattern test for " << algorithmName << " (pattern list is empty) ---" << std::endl;
        return;
    }

    size_t total_patterns = patterns.size();
    size_t total_individual_searches = static_cast<size_t>(total_patterns) * numIterations;

    std::cout << "\n--- Timing " << numIterations << " searches for EACH of " << total_patterns << " patterns using " << algorithmName << " ---" << std::endl;
    std::cout << "Total individual search runs will be: " << total_individual_searches << std::endl;

    auto start_time = std::chrono::high_resolution_clock::now();
    std::string::size_type grand_total_occurrences = 0;

    for (const auto & current_pattern : patterns) {
        for (int j = 0; j < numIterations; ++j) {
            grand_total_occurrences += singlePatternSearchFunc(current_pattern, txt);
        }
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);

    std::cout << "\n--- Results for Multi-Pattern Test (" << algorithmName << ") ---" << std::endl;
    std::cout << "Total patterns tested: " << total_patterns << std::endl;
    std::cout << "Iterations per pattern: " << numIterations << std::endl;

    std::cout << "Average time for all runs: " << duration_ms.count() / numIterations << " ms" << std::endl;
    std::cout << "Average occurrences across all runs: " << grand_total_occurrences / numIterations << std::endl;

}

int main() {
        std::string txt;
        std::string pat;

        const char *textFilename = "/Users/ivanpavlov/Desktop/cw-2/extra/tests/test9/text9.txt";
        const char *patternFilename = "/Users/ivanpavlov/Desktop/cw-2/extra/tests/test9/pattern9.txt";

        bool useMultiplePatternsForAhoCorasick = true; // <--- Установите здесь нужный режим

        std::cout << "Attempting to read text from '" << textFilename << "'..." << std::endl;
        if (!readFileToString(textFilename, txt)) {
            return 1;
        }
        std::cout << "Successfully read text." << std::endl;

        std::cout << "Attempting to read pattern from '" << patternFilename << "'..." << std::endl;
        if (!readFileToString(patternFilename, pat)) {
            return 1;
        }
        std::cout << "Successfully read pattern." << std::endl;

    int testIterations = 100; // Количество повторений для каждого отдельного поиска

    std::vector<std::string> multiple_patterns;
    if (useMultiplePatternsForAhoCorasick) {
        std::cout << "\nReading patterns for multi-pattern tests from '" << patternFilename << "' line by line..."
                  << std::endl;
        if (!readPatternsFromFile(patternFilename, multiple_patterns)) {
            std::cerr << "Error: Failed to read multiple patterns from '" << patternFilename << "'. Multi-pattern tests will be skipped." << std::endl;
        } else {
            std::cout << "Successfully read " << multiple_patterns.size() << " patterns for multi-pattern tests." << std::endl;
        }
    }

    if (useMultiplePatternsForAhoCorasick) {
        if (multiple_patterns.empty()) {
            std::cout << "\n===== Skipping ALL Multi-Pattern Tests (pattern list empty) =====" << std::endl;
        } else {

            std::cout << "\n===== Running Multi-Pattern Tests (using " << multiple_patterns.size() << " patterns) =====" << std::endl;
            std::cout << "\n=======================================" << std::endl;
            runMultiPatternTimedTest(naiveSearch, "Naive Algorithm", multiple_patterns, txt, testIterations);
            std::cout << "=======================================" << std::endl;

            std::cout << "\n=======================================" << std::endl;
            auto rabinKarpSearchWrapper = [&](const std::string& rp, const std::string& rt){ return rabinKarpSearch(rp, rt); };
            runMultiPatternTimedTest(rabinKarpSearchWrapper, "Rabin-Karp Algorithm", multiple_patterns, txt, testIterations);
            std::cout << "=======================================" << std::endl;

            std::cout << "\n=======================================" << std::endl;
            runMultiPatternTimedTest(kmpSearch, "KMP Algorithm", multiple_patterns, txt, testIterations);
            std::cout << "=======================================" << std::endl;

            std::cout << "\n=======================================" << std::endl;
            runMultiPatternTimedTest(boyerMooreSearch, "Boyer-Moore Algorithm", multiple_patterns, txt, testIterations);
            std::cout << "=======================================" << std::endl;

            std::cout << "\n=======================================" << std::endl;
            auto zSearchWrapper = [&](const std::string& p_z, const std::string& t_z){ return zSearch(p_z, t_z); };
            runMultiPatternTimedTest(zSearchWrapper, "Z-function Algorithm", multiple_patterns, txt, testIterations);
            std::cout << "=======================================" << std::endl;


            const std::string ahoAlgorithmNameEfficient = "Aho-Corasick Algorithm (Efficient Multi-Pattern)";
            const int numAhoIterationsEfficient = testIterations; // testIterations у вас 1

            std::cout << "\n=======================================" << std::endl;
            std::cout << "\n--- Timing " << numAhoIterationsEfficient << " searches for " << ahoAlgorithmNameEfficient
                      << " (using " << multiple_patterns.size() << " patterns) ---" << std::endl;
            std::cout << "Total search operations executed (machine built once): " << numAhoIterationsEfficient << std::endl;

            auto start_time_aho_eff = std::chrono::high_resolution_clock::now();
            std::string::size_type grand_total_occurrences_aho_eff = 0;

            if (!multiple_patterns.empty()) {
                // Машина строится ОДИН РАЗ для всех паттернов
                AhoCorasickMachine machine(multiple_patterns);
                for (int i = 0; i < numAhoIterationsEfficient; ++i) {
                    // Поиск выполняется ОДИН РАЗ по тексту для всех паттернов сразу
                    auto results = machine.search(txt);
                    grand_total_occurrences_aho_eff += results.size();
                }
            }

            auto end_time_aho_eff = std::chrono::high_resolution_clock::now();
            auto duration_ms_aho_eff = std::chrono::duration_cast<std::chrono::milliseconds>(end_time_aho_eff - start_time_aho_eff);

            long long average_time_per_search_ms_aho_eff = 0;
            size_t average_occurrences_per_search_int_aho_eff = 0;

            if (numAhoIterationsEfficient > 0 && !multiple_patterns.empty()) {
                average_time_per_search_ms_aho_eff = duration_ms_aho_eff.count() / numAhoIterationsEfficient;
                average_occurrences_per_search_int_aho_eff = grand_total_occurrences_aho_eff / numAhoIterationsEfficient;
            }

            std::cout << std::fixed << std::setprecision(3);
            std::cout << "\n--- Results for Multi-Pattern Test (" << ahoAlgorithmNameEfficient << ") ---" << std::endl;
            std::cout << "Total patterns used to build machine: " << multiple_patterns.size() << std::endl;
            std::cout << "Iterations of search (with pre-built machine): " << numAhoIterationsEfficient << std::endl;
            std::cout << "Average time for all runs: " << average_time_per_search_ms_aho_eff << " ms" << std::endl;
            std::cout << "Average occurrences across all runs: " << average_occurrences_per_search_int_aho_eff << std::endl;
            std::cout << "=======================================" << std::endl;
        }

    } else {
        if (pat.empty()) {
            std::cout << "\n===== Skipping ALL Single-Pattern Tests ('pat' string empty) =====" << std::endl;
        } else {
            std::cout << "\n===== Running Single-Pattern Tests (using entire '" << patternFilename << "' as one pattern) =====" << std::endl;

            std::cout << "\n=======================================" << std::endl;
            runTimedTest(naiveSearch, "Naive Algorithm", pat, txt, testIterations);
            std::cout << "=======================================" << std::endl;

            std::cout << "\n=======================================" << std::endl;
            auto rabinKarpSearchWrapper = [&](const std::string& rp, const std::string& rt){ return rabinKarpSearch(rp, rt); };
            runTimedTest(rabinKarpSearchWrapper, "Rabin-Karp Algorithm", pat, txt, testIterations);
            std::cout << "=======================================" << std::endl;

            std::cout << "\n=======================================" << std::endl;
            runTimedTest(kmpSearch, "KMP Algorithm", pat, txt, testIterations);
            std::cout << "=======================================" << std::endl;

            std::cout << "\n=======================================" << std::endl;
            runTimedTest(boyerMooreSearch, "Boyer-Moore Algorithm", pat, txt, testIterations);
            std::cout << "=======================================" << std::endl;

            std::cout << "\n=======================================" << std::endl;
            auto zSearchWrapper = [&](const std::string& p_z, const std::string& t_z){ return zSearch(p_z, t_z); };
            runTimedTest(zSearchWrapper, "Z-function Algorithm", pat, txt, testIterations);
            std::cout << "=======================================" << std::endl;

            std::cout << "\n=======================================" << std::endl;
            auto ahoCorasickSinglePatternWrapper = [&](const std::string& p, const std::string& t){
                std::vector<std::string> patterns_vec = {p};
                return ahoCorasickSearch(patterns_vec, t);
            };
            runTimedTest(ahoCorasickSinglePatternWrapper, "Aho-Corasick Algorithm (Single Large Pattern from file)", pat, txt, testIterations);
            std::cout << "=======================================" << std::endl;

        }
    }

    return 0;
}