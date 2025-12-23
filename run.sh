#!/bin/bash

# HolyC++ Test Runner
# Simple, no-bullshit build system

set -e  # Exit on error

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Configuration
CXX=${CXX:-g++}
CXXFLAGS="-std=c++17 -I. -Wall -Wextra -Werror -O2"
TARGET="holyc_test"
BUILD_DIR="build"

# Tests (name|space-separated-sources)
TESTS=(
    "int|src/types/unsigned_int.cpp src/types/signed_int.cpp src/tests/test_int.cpp"
    "error|src/tests/test_error.cpp"
    "float|src/types/float.cpp src/tests/test_float.cpp"
    "union|src/types/union_type.cpp src/tests/test_union.cpp"
)

ARG="$1"

# Cleanup option (early)
if [[ "$ARG" == "--clean" ]]; then
    echo -e "\n${YELLOW}🧹 Cleaning up...${NC}"
    rm -rf "$BUILD_DIR"
    exit 0
fi

# Help option
if [[ "$ARG" == "--help" || "$ARG" == "-h" ]]; then
    echo -e "${BLUE}HolyC++ Test Runner${NC}"
    echo -e "Usage: ./run.sh [test_name] | --clean | --help"
    echo -e "\nAvailable tests:"
    for entry in "${TESTS[@]}"; do
        name="${entry%%|*}"
        echo -e "  ${YELLOW}$name${NC}"
    done
    echo -e "\nExamples:"
    echo -e "  ./run.sh           # Run all tests"
    echo -e "  ./run.sh int       # Run int test only"
    echo -e "  ./run.sh union     # Run union test only"
    echo -e "  ./run.sh --clean   # Clean build directory"
    exit 0
fi

# If an argument was provided, treat it as a test filter (int, error, float, or union)
FILTER=""
if [[ -n "$ARG" ]]; then
    FILTER="$ARG"
fi

# Validate filter if provided
if [[ -n "$FILTER" ]]; then
    found=0
    for entry in "${TESTS[@]}"; do
        name="${entry%%|*}"
        if [[ "$name" == "$FILTER" ]]; then
            found=1
            break
        fi
    done
    if [[ $found -ne 1 ]]; then
        echo -e "${RED}Unknown test: '$FILTER'${NC}"
        echo -e "Available tests:"
        for entry in "${TESTS[@]}"; do
            name="${entry%%|*}"
            echo -e "  ${YELLOW}$name${NC}"
        done
        exit 1
    fi
fi

# Create build directory
echo -e "${BLUE}📁 Creating build directory...${NC}"
mkdir -p "$BUILD_DIR"

# Build each selected test into its own executable
echo -e "${BLUE}🔨 Compiling test targets...${NC}"
declare -a EXES=()
for entry in "${TESTS[@]}"; do
    name="${entry%%|*}"
    sources_str="${entry#*|}"

    # If a filter was provided, skip other tests
    if [[ -n "$FILTER" && "$FILTER" != "$name" ]]; then
        continue
    fi

    OBJ_FILES=()
    for src in $sources_str; do
        obj="$BUILD_DIR/${name}_$(basename "$src" .cpp).o"
        echo -e "  ${YELLOW}Compiling${NC} ${name}: $src"
        $CXX $CXXFLAGS -c "$src" -o "$obj"
        OBJ_FILES+=("$obj")
    done

    exe="${name}_${TARGET}"
    echo -e "  ${YELLOW}Linking${NC} ${name}: $exe"
    $CXX $CXXFLAGS "${OBJ_FILES[@]}" -o "$BUILD_DIR/$exe"
    EXES+=("$exe")
done

if [[ ${#EXES[@]} -eq 0 ]]; then
    echo -e "${RED}No tests to run (check filter or TESTS config).${NC}"
    exit 1
fi

# Run tests in parallel
echo -e "\n${BLUE}🧪 Running tests in parallel...${NC}"
echo -e "${GREEN}========================================${NC}"

declare -a PIDS=()
declare -a LOGS=()
declare -a NAMES=()

# Trap to clean up background processes on interrupt
trap 'echo -e "${YELLOW}\nInterrupted, killing running tests...${NC}"; for p in "${PIDS[@]}"; do kill "$p" 2>/dev/null || true; done; exit 130' INT TERM

for exe in "${EXES[@]}"; do
    name="${exe%%_*}"
    log="$BUILD_DIR/${exe}.log"
    echo -e "  ${YELLOW}Starting${NC} $name -> $log"
    "$BUILD_DIR/$exe" > "$log" 2>&1 &
    pid=$!
    PIDS+=("$pid")
    LOGS+=("$log")
    NAMES+=("$name")
done

# Wait for all tests and collect results
fail_count=0
for i in "${!PIDS[@]}"; do
    pid="${PIDS[i]}"
    name="${NAMES[i]}"
    log="${LOGS[i]}"
    
    # Wait for this specific process
    if wait "$pid" 2>/dev/null; then
        rc=$?
    else
        rc=$?
    fi
    
    if [[ $rc -eq 0 ]]; then
        echo -e "${GREEN}✔ ${name} passed${NC}"
    else
        echo -e "${RED}✖ ${name} failed (exit $rc)${NC}"
        echo -e "${YELLOW}--- ${name} log ---${NC}"
        if [[ -f "$log" ]]; then
            head -n 200 "$log"
        else
            echo "Log file not found"
        fi
        echo -e "${YELLOW}--- end log ---${NC}"
        fail_count=$((fail_count + 1))
    fi
done

echo -e "${GREEN}========================================${NC}"
if [[ $fail_count -eq 0 ]]; then
    echo -e "${GREEN}✅ All tests passed!${NC}"
    # Optional: Show binary info
    echo -e "\n${BLUE}📦 Binary info:${NC}"
    if command -v size &> /dev/null; then
        for exe in "${EXES[@]}"; do
            echo -e "\n${YELLOW}$exe:${NC}"
            size "$BUILD_DIR/$exe" 2>/dev/null || echo "  size command failed"
        done
    fi
    exit 0
else
    echo -e "${RED}❌ Some tests failed (${fail_count})${NC}"
    exit 1
fi

# Usage: ./run.sh           -> run all tests
#        ./run.sh int       -> run int only
#        ./run.sh error     -> run error only  
#        ./run.sh float     -> run float only
#        ./run.sh union     -> run union only
#        ./run.sh --clean   -> remove build dir
#        ./run.sh --help    -> show help