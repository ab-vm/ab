execute_process(COMMAND brew --prefix llvm OUTPUT_VARIABLE LLVM_PREFIX)

set(CC "${LLVM_PREFIX}/bin/clang")
set(CXX "${LLVM_PREFIX}/bin/clang++")
set(LD "${LLVM_PREFIX}/bin/llvm-ld")
set()