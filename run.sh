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

# Source files
SOURCES=(
    "src/types/unsigned_int.cpp"
    "src/types/signed_int.cpp"
    "src/tests/test_int.cpp"
)

# Create build directory
echo -e "${BLUE}📁 Creating build directory...${NC}"
mkdir -p $BUILD_DIR

# Compile object files
echo -e "${BLUE}🔨 Compiling source files...${NC}"
OBJ_FILES=()
for src in "${SOURCES[@]}"; do
    obj="$BUILD_DIR/$(basename "$src" .cpp).o"
    echo -e "  ${YELLOW}Compiling${NC} $src"
    $CXX $CXXFLAGS -c "$src" -o "$obj"
    OBJ_FILES+=("$obj")
done

# Link executable
echo -e "${BLUE}🔗 Linking executable...${NC}"
$CXX $CXXFLAGS "${OBJ_FILES[@]}" -o "$BUILD_DIR/$TARGET"

# Run tests
echo -e "\n${BLUE}🧪 Running tests...${NC}"
echo -e "${GREEN}========================================${NC}"
if "$BUILD_DIR/$TARGET"; then
    echo -e "${GREEN}========================================${NC}"
    echo -e "${GREEN}✅ All tests passed!${NC}"
    
    # Optional: Show binary info
    echo -e "\n${BLUE}📦 Binary info:${NC}"
    if command -v size &> /dev/null; then
        size "$BUILD_DIR/$TARGET"
    fi
else
    echo -e "${RED}========================================${NC}"
    echo -e "${RED}❌ Tests failed!${NC}"
    exit 1
fi

# Cleanup option
if [[ "$1" == "--clean" ]]; then
    echo -e "\n${YELLOW}🧹 Cleaning up...${NC}"
    rm -rf $BUILD_DIR
fi

# Run with: ./run.sh
# Clean with: ./run.sh --clean