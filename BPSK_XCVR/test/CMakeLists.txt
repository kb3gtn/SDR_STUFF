cmake_minimum_required (VERSION 3.5.1)

add_executable (test_prbs_gen test_prbs_gen.cpp )
target_compile_options(test_prbs_gen PUBLIC -std=c++14 )
target_link_libraries( test_prbs_gen LINK_PUBLIC PRBS_GENS )

add_executable (test_bpsk_mod test_bpsk_mod.cpp )
target_compile_options(test_bpsk_mod PUBLIC -std=c++14)
target_link_libraries( test_bpsk_mod LINK_PUBLIC PRBS_GENS SYMBOL_MAPPERS RESAMPLERS FILTERS )


