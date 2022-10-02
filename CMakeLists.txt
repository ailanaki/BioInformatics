cmake_minimum_required(VERSION 3.17)
project(BioInformatics)

set(CMAKE_CXX_STANDARD 17)

add_executable(BioInformatics MedianString/median_string.h RandomizedMotifSearch/motif_search.h PairedComposition/paired_composition.h SuffixArray/suffix_array.h AlignTwoStringsUsingAffineGapPenalty/align.h suffArray.h LongestRepeat/longest_repeat.h AllApproximateOccurrences/multiple_approximate_pattern_matching.h main.cpp ViterbiAlgorithm/viterbi.h Baum-Welch/baum-welch.h)