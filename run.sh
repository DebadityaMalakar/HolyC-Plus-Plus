#!/bin/bash

# HolyC++ Test Runner
# Simple, no-bullshit build system

set -e  # Exit on error

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
PURPLE='\033[0;35m'
CYAN='\033[0;36m'
NC='\033[0m' # No Color

# Configuration
CXX=${CXX:-g++}
CXXFLAGS="-std=c++17 -I. -Wall -Wextra -Werror -O2"
BUILD_DIR="build"

# Source file groups
INT_SOURCES=(
    "src/types/unsigned_int.cpp"
    "src/types/signed_int.cpp"
    "src/tests/test_int.cpp"
)

ERROR_SOURCES=(
    "src/lib/error.cpp"
    "src/lib/error_types.cpp"
    "src/tests/test_error.cpp"
)

# Function to print usage
print_usage() {
    echo -e "${CYAN}HolyC++ Test Runner${NC}"
    echo -e "${CYAN}===================${NC}"
    echo -e "Usage: $0 [OPTIONS] [TEST_TYPE]"
    echo
    echo -e "TEST_TYPE (choose one):"
    echo -e "  ${GREEN}int${NC}      - Run integer type tests only (default)"
    echo -e "  ${GREEN}error${NC}    - Run error system tests only"
    echo -e "  ${GREEN}all${NC}      - Run all tests"
    echo
    echo -e "OPTIONS:"
    echo -e "  ${YELLOW}--clean${NC}    - Clean build directory after running"
    echo -e "  ${YELLOW}--help${NC}     - Show this help message"
    echo
    echo -e "Examples:"
    echo -e "  $0                # Run integer tests"
    echo -e "  $0 error          # Run error system tests"
    echo -e "  $0 all --clean    # Run all tests and clean up"
}

# Function to compile and run tests
run_test() {
    local test_name=$1
    shift
    local sources=("$@")
    local target="holyc_${test_name}_test"
    
    echo -e "\n${BLUE}🚀 Running ${CYAN}${test_name}${BLUE} tests...${NC}"
    echo -e "${PURPLE}========================================${NC}"
    
    # Create build directory if it doesn't exist
    mkdir -p $BUILD_DIR
    
    # Compile object files
    echo -e "${BLUE}🔨 Compiling source files...${NC}"
    local obj_files=()
    for src in "${sources[@]}"; do
        local obj="$BUILD_DIR/$(basename "$src" .cpp).o"
        echo -e "  ${YELLOW}Compiling${NC} $src"
        $CXX $CXXFLAGS -c "$src" -o "$obj" || {
            echo -e "${RED}❌ Compilation failed!${NC}"
            return 1
        }
        obj_files+=("$obj")
    done
    
    # Link executable
    echo -e "${BLUE}🔗 Linking executable...${NC}"
    $CXX $CXXFLAGS "${obj_files[@]}" -o "$BUILD_DIR/$target" || {
        echo -e "${RED}❌ Linking failed!${NC}"
        return 1
    }
    
    # Run tests
    echo -e "\n${BLUE}🧪 Running ${test_name} tests...${NC}"
    echo -e "${GREEN}========================================${NC}"
    
    local start_time=$(date +%s.%N)
    if "$BUILD_DIR/$target"; then
        local end_time=$(date +%s.%N)
        local elapsed_time=$(echo "$end_time - $start_time" | bc)
        
        echo -e "${GREEN}========================================${NC}"
        echo -e "${GREEN}✅ All ${test_name} tests passed!${NC}"
        echo -e "${BLUE}⏱️  Execution time: ${elapsed_time}s${NC}"
        
        # Optional: Show binary info
        echo -e "\n${BLUE}📦 Binary info for ${target}:${NC}"
        if command -v size &> /dev/null; then
            size "$BUILD_DIR/$target"
        fi
        
        return 0
    else
        echo -e "${RED}========================================${NC}"
        echo -e "${RED}❌ ${test_name} tests failed!${NC}"
        return 1
    fi
}

# Function to clean build directory
clean_build() {
    echo -e "\n${YELLOW}🧹 Cleaning build directory...${NC}"
    if [ -d "$BUILD_DIR" ]; then
        rm -rf "$BUILD_DIR"
        echo -e "  ${GREEN}✓ Build directory cleaned${NC}"
    else
        echo -e "  ${YELLOW}⚠ Build directory does not exist${NC}"
    fi
}

# Parse command line arguments
TEST_TYPE="int"
CLEAN_FLAG=false

while [[ $# -gt 0 ]]; do
    case $1 in
        int|error|all)
            TEST_TYPE="$1"
            shift
            ;;
        --clean)
            CLEAN_FLAG=true
            shift
            ;;
        --help|-h)
            print_usage
            exit 0
            ;;
        *)
            echo -e "${RED}❌ Unknown option: $1${NC}"
            print_usage
            exit 1
            ;;
    esac
done

# Run tests based on TEST_TYPE
case $TEST_TYPE in
    int)
        run_test "int" "${INT_SOURCES[@]}"
        result=$?
        ;;
        
    error)
        run_test "error" "${ERROR_SOURCES[@]}"
        result=$?
        ;;
        
    all)
        echo -e "${CYAN}🏃 Running ALL tests...${NC}"
        echo -e "${PURPLE}========================${NC}"
        
        # Run integer tests
        if ! run_test "int" "${INT_SOURCES[@]}"; then
            echo -e "${RED}❌ Integer tests failed, skipping error tests${NC}"
            result=1
        else
            echo -e "\n${CYAN}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${NC}"
            # Run error tests
            if ! run_test "error" "${ERROR_SOURCES[@]}"; then
                echo -e "${RED}❌ Error tests failed${NC}"
                result=1
            else
                echo -e "\n${GREEN}🎉 ALL TESTS PASSED!${NC}"
                result=0
            fi
        fi
        ;;
        
    *)
        echo -e "${RED}❌ Invalid test type: $TEST_TYPE${NC}"
        print_usage
        exit 1
        ;;
esac

# Cleanup if requested
if [ "$CLEAN_FLAG" = true ]; then
    clean_build
fi

# Exit with appropriate code
if [ $result -eq 0 ]; then
    echo -e "\n${GREEN}✨ Test run completed successfully!${NC}"
else
    echo -e "\n${RED}💥 Test run failed!${NC}"
fi

exit $result