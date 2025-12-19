#include "error_types.hpp"
#include <sstream>

namespace holycpp {
    // Define the singleton instance in one translation unit
    ErrorCodeRegistry* ErrorCodeRegistry::instance = nullptr;
}
using namespace holycpp;
// LexerError implementations
LexerError::LexerError(Code code, const SourceLocation& loc, 
                       const std::string& extra)
    : ContextualError(ErrorSeverity::ERROR, "", loc) {
    
    std::stringstream ss;
    ss << codeToString(code);
    if (!extra.empty()) {
        ss << ": " << extra;
    }
    
    // Set message by creating a new error (simplified approach)
    std::string msg = ss.str();
    // In a real implementation, we'd need to store this differently
}

std::string LexerError::codeToString(Code code) {
    switch (code) {
        case Code::UNKNOWN_CHAR: return "Unknown character";
        case Code::UNTERMINATED_STRING: return "Unterminated string literal";
        case Code::UNTERMINATED_CHAR: return "Unterminated character literal";
        case Code::INVALID_NUMBER: return "Invalid number format";
        case Code::INVALID_ESCAPE: return "Invalid escape sequence";
        case Code::NUMBER_TOO_LARGE: return "Number too large for type";
        default: return "Unknown lexer error";
    }
}

// ParserError implementations
ParserError::ParserError(Code code, const SourceLocation& loc,
                         const std::string& extra,
                         const std::string& expected)
    : ContextualError(ErrorSeverity::ERROR, "", loc) {
    
    std::stringstream ss;
    ss << codeToString(code);
    if (!expected.empty()) {
        ss << ", expected: " << expected;
    }
    if (!extra.empty()) {
        ss << " (" << extra << ")";
    }
    
    std::string msg = ss.str();
}

std::string ParserError::codeToString(Code code) {
    switch (code) {
        case Code::UNEXPECTED_TOKEN: return "Unexpected token";
        case Code::EXPECTED_TOKEN: return "Expected token";
        case Code::MISSING_SEMICOLON: return "Missing semicolon";
        case Code::MISSING_PAREN: return "Missing parenthesis";
        case Code::MISSING_BRACE: return "Missing brace";
        case Code::MISSING_BRACKET: return "Missing bracket";
        case Code::INVALID_TYPE: return "Invalid type";
        case Code::INVALID_EXPRESSION: return "Invalid expression";
        case Code::DUPLICATE_DECLARATION: return "Duplicate declaration";
        case Code::INVALID_FUNCTION_DEF: return "Invalid function definition";
        default: return "Unknown parser error";
    }
}

// TypeError implementations
TypeError::TypeError(Code code, const SourceLocation& loc,
                     const std::string& type1,
                     const std::string& type2,
                     const std::string& extra)
    : ContextualError(ErrorSeverity::ERROR, "", loc) {
    
    std::stringstream ss;
    ss << codeToString(code);
    
    if (!type1.empty() && !type2.empty()) {
        ss << " (" << type1 << " vs " << type2 << ")";
    } else if (!type1.empty()) {
        ss << " (" << type1 << ")";
    }
    
    if (!extra.empty()) {
        ss << ": " << extra;
    }
    
    std::string msg = ss.str();
}

std::string TypeError::codeToString(Code code) {
    switch (code) {
        case Code::TYPE_MISMATCH: return "Type mismatch";
        case Code::UNDECLARED_IDENTIFIER: return "Undeclared identifier";
        case Code::REDECLARATION: return "Redeclaration";
        case Code::INVALID_CONVERSION: return "Invalid conversion";
        case Code::INVALID_OPERAND_TYPES: return "Invalid operand types";
        case Code::INVALID_FUNCTION_CALL: return "Invalid function call";
        case Code::INVALID_ARRAY_SIZE: return "Invalid array size";
        case Code::INVALID_POINTER_OP: return "Invalid pointer operation";
        case Code::INVALID_ASSIGNMENT: return "Invalid assignment";
        case Code::INVALID_RETURN_TYPE: return "Invalid return type";
        case Code::MISSING_RETURN: return "Missing return statement";
        case Code::DIVISION_BY_ZERO: return "Division by zero";
        default: return "Unknown type error";
    }
}

// SemanticError implementations
SemanticError::SemanticError(Code code, const SourceLocation& loc,
                             const std::string& extra)
    : ContextualError(ErrorSeverity::ERROR, "", loc) {
    
    std::stringstream ss;
    ss << codeToString(code);
    if (!extra.empty()) {
        ss << ": " << extra;
    }
    
    std::string msg = ss.str();
}

std::string SemanticError::codeToString(Code code) {
    switch (code) {
        case Code::INVALID_MAIN: return "Invalid main function";
        case Code::MISSING_MAIN: return "Missing main function";
        case Code::INVALID_LINKAGE: return "Invalid linkage specification";
        case Code::INVALID_STORAGE_CLASS: return "Invalid storage class";
        case Code::CYCLIC_DEPENDENCY: return "Cyclic dependency";
        case Code::INVALID_INITIALIZER: return "Invalid initializer";
        case Code::NON_CONST_INITIALIZER: return "Non-constant initializer";
        case Code::INVALID_CONST_EXPR: return "Invalid constant expression";
        case Code::UNDEFINED_REFERENCE: return "Undefined reference";
        case Code::MULTIPLE_DEFINITION: return "Multiple definition";
        default: return "Unknown semantic error";
    }
}

// CodeGenError implementations
CodeGenError::CodeGenError(Code code, const SourceLocation& loc,
                           const std::string& extra)
    : ContextualError(ErrorSeverity::FATAL, "", loc) {
    
    std::stringstream ss;
    ss << codeToString(code);
    if (!extra.empty()) {
        ss << ": " << extra;
    }
    
    std::string msg = ss.str();
}

std::string CodeGenError::codeToString(Code code) {
    switch (code) {
        case Code::LLVM_INIT_FAILED: return "LLVM initialization failed";
        case Code::MODULE_CREATION_FAILED: return "Module creation failed";
        case Code::FUNCTION_CREATION_FAILED: return "Function creation failed";
        case Code::TYPE_CREATION_FAILED: return "Type creation failed";
        case Code::INVALID_IR: return "Invalid IR generated";
        case Code::OPTIMIZATION_FAILED: return "Optimization failed";
        case Code::TARGET_NOT_FOUND: return "Target not found";
        case Code::OBJECT_EMISSION_FAILED: return "Object file emission failed";
        default: return "Unknown code generation error";
    }
}
// namespace holycpp