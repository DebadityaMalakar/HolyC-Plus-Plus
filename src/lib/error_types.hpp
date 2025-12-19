#pragma once
#include "error.hpp"
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>

namespace holycpp {

// ==================== Lexer Errors ====================
class LexerError : public ContextualError {
public:
    enum class Code {
        UNKNOWN_CHAR,
        UNTERMINATED_STRING,
        UNTERMINATED_CHAR,
        INVALID_NUMBER,
        INVALID_ESCAPE,
        NUMBER_TOO_LARGE
    };
    
    LexerError(Code code, const SourceLocation& loc, 
               const std::string& extra = "");
    
    static std::string codeToString(Code code);
};

// ==================== Parser Errors ====================
class ParserError : public ContextualError {
public:
    enum class Code {
        UNEXPECTED_TOKEN,
        EXPECTED_TOKEN,
        MISSING_SEMICOLON,
        MISSING_PAREN,
        MISSING_BRACE,
        MISSING_BRACKET,
        INVALID_TYPE,
        INVALID_EXPRESSION,
        DUPLICATE_DECLARATION,
        INVALID_FUNCTION_DEF
    };
    
    ParserError(Code code, const SourceLocation& loc,
                const std::string& extra = "",
                const std::string& expected = "");
    
    static std::string codeToString(Code code);
};

// ==================== Type Errors ====================
class TypeError : public ContextualError {
public:
    enum class Code {
        TYPE_MISMATCH,
        UNDECLARED_IDENTIFIER,
        REDECLARATION,
        INVALID_CONVERSION,
        INVALID_OPERAND_TYPES,
        INVALID_FUNCTION_CALL,
        INVALID_ARRAY_SIZE,
        INVALID_POINTER_OP,
        INVALID_ASSIGNMENT,
        INVALID_RETURN_TYPE,
        MISSING_RETURN,
        DIVISION_BY_ZERO
    };
    
    TypeError(Code code, const SourceLocation& loc,
              const std::string& type1 = "",
              const std::string& type2 = "",
              const std::string& extra = "");
    
    static std::string codeToString(Code code);
};

// ==================== Semantic Errors ====================
class SemanticError : public ContextualError {
public:
    enum class Code {
        INVALID_MAIN,
        MISSING_MAIN,
        INVALID_LINKAGE,
        INVALID_STORAGE_CLASS,
        CYCLIC_DEPENDENCY,
        INVALID_INITIALIZER,
        NON_CONST_INITIALIZER,
        INVALID_CONST_EXPR,
        UNDEFINED_REFERENCE,
        MULTIPLE_DEFINITION
    };
    
    SemanticError(Code code, const SourceLocation& loc,
                  const std::string& extra = "");
    
    static std::string codeToString(Code code);
};

// ==================== CodeGen Errors ====================
class CodeGenError : public ContextualError {
public:
    enum class Code {
        LLVM_INIT_FAILED,
        MODULE_CREATION_FAILED,
        FUNCTION_CREATION_FAILED,
        TYPE_CREATION_FAILED,
        INVALID_IR,
        OPTIMIZATION_FAILED,
        TARGET_NOT_FOUND,
        OBJECT_EMISSION_FAILED
    };
    
    CodeGenError(Code code, const SourceLocation& loc,
                 const std::string& extra = "");
    
    static std::string codeToString(Code code);
};

// ==================== Internal Compiler Errors ====================
class InternalError : public CompilerError {
public:
    InternalError(const std::string& message, 
                  const SourceLocation& loc = {},
                  const std::string& code = "ICE")
        : CompilerError(ErrorSeverity::FATAL, message, loc, code) {}
    
    std::string format() const override {
        std::stringstream ss;
        ss << "INTERNAL COMPILER ERROR: " << getMessage();
        if (getLocation().isValid()) {
            ss << "\n  at " << getLocation().toString();
        }
        ss << "\nPlease report this bug to the HolyC++ developers.";
        return ss.str();
    }
};

// ==================== Error Code Registry ====================
class ErrorCodeRegistry {
    static ErrorCodeRegistry* instance;
    
    struct ErrorInfo {
        std::string code;
        std::string description;
        ErrorSeverity severity;
        std::string category;
    };
    
    std::unordered_map<std::string, ErrorInfo> registry;
    
    ErrorCodeRegistry() {
        initialize();
    }
    
    void initialize() {
        // Lexer errors
        registerError("L001", "Unknown character", ErrorSeverity::ERROR, "Lexer");
        registerError("L002", "Unterminated string literal", ErrorSeverity::ERROR, "Lexer");
        registerError("L003", "Invalid number format", ErrorSeverity::ERROR, "Lexer");
        registerError("L004", "Unterminated character literal", ErrorSeverity::ERROR, "Lexer");
        registerError("L005", "Invalid escape sequence", ErrorSeverity::ERROR, "Lexer");
        registerError("L006", "Number too large for type", ErrorSeverity::ERROR, "Lexer");
        
        // Parser errors
        registerError("P001", "Unexpected token", ErrorSeverity::ERROR, "Parser");
        registerError("P002", "Expected token", ErrorSeverity::ERROR, "Parser");
        registerError("P003", "Missing semicolon", ErrorSeverity::ERROR, "Parser");
        registerError("P004", "Missing parenthesis", ErrorSeverity::ERROR, "Parser");
        registerError("P005", "Missing brace", ErrorSeverity::ERROR, "Parser");
        registerError("P006", "Missing bracket", ErrorSeverity::ERROR, "Parser");
        registerError("P007", "Invalid type", ErrorSeverity::ERROR, "Parser");
        registerError("P008", "Invalid expression", ErrorSeverity::ERROR, "Parser");
        registerError("P009", "Duplicate declaration", ErrorSeverity::ERROR, "Parser");
        registerError("P010", "Invalid function definition", ErrorSeverity::ERROR, "Parser");
        
        // Type errors
        registerError("T001", "Type mismatch", ErrorSeverity::ERROR, "Type");
        registerError("T002", "Undeclared identifier", ErrorSeverity::ERROR, "Type");
        registerError("T003", "Redeclaration of identifier", ErrorSeverity::ERROR, "Type");
        registerError("T004", "Invalid type conversion", ErrorSeverity::ERROR, "Type");
        registerError("T005", "Invalid operand types for operator", ErrorSeverity::ERROR, "Type");
        registerError("T006", "Invalid function call", ErrorSeverity::ERROR, "Type");
        registerError("T007", "Invalid array size", ErrorSeverity::ERROR, "Type");
        registerError("T008", "Invalid pointer operation", ErrorSeverity::ERROR, "Type");
        registerError("T009", "Invalid assignment", ErrorSeverity::ERROR, "Type");
        registerError("T010", "Invalid return type", ErrorSeverity::ERROR, "Type");
        registerError("T011", "Missing return statement", ErrorSeverity::WARNING, "Type");
        registerError("T012", "Division by zero", ErrorSeverity::ERROR, "Type");
        
        // Semantic errors
        registerError("S001", "Invalid main function", ErrorSeverity::ERROR, "Semantic");
        registerError("S002", "Missing main function", ErrorSeverity::ERROR, "Semantic");
        registerError("S003", "Invalid linkage specification", ErrorSeverity::ERROR, "Semantic");
        registerError("S004", "Invalid storage class", ErrorSeverity::ERROR, "Semantic");
        registerError("S005", "Cyclic dependency", ErrorSeverity::ERROR, "Semantic");
        registerError("S006", "Invalid initializer", ErrorSeverity::ERROR, "Semantic");
        registerError("S007", "Non-constant initializer", ErrorSeverity::ERROR, "Semantic");
        registerError("S008", "Invalid constant expression", ErrorSeverity::ERROR, "Semantic");
        registerError("S009", "Undefined reference", ErrorSeverity::ERROR, "Semantic");
        registerError("S010", "Multiple definition", ErrorSeverity::ERROR, "Semantic");
        
        // Codegen errors
        registerError("C001", "LLVM initialization failed", ErrorSeverity::FATAL, "CodeGen");
        registerError("C002", "Module creation failed", ErrorSeverity::FATAL, "CodeGen");
        registerError("C003", "Function creation failed", ErrorSeverity::FATAL, "CodeGen");
        registerError("C004", "Type creation failed", ErrorSeverity::FATAL, "CodeGen");
        registerError("C005", "Invalid IR generated", ErrorSeverity::FATAL, "CodeGen");
        registerError("C006", "Optimization failed", ErrorSeverity::WARNING, "CodeGen");
        registerError("C007", "Target not found", ErrorSeverity::FATAL, "CodeGen");
        registerError("C008", "Object file emission failed", ErrorSeverity::FATAL, "CodeGen");
        
        // Internal errors
        registerError("I001", "Internal compiler error", ErrorSeverity::FATAL, "Internal");
        registerError("I002", "Assertion failed", ErrorSeverity::FATAL, "Internal");
        registerError("I003", "Out of memory", ErrorSeverity::FATAL, "Internal");
    }
    
public:
    static ErrorCodeRegistry& get() {
        if (!instance) {
            instance = new ErrorCodeRegistry();
        }
        return *instance;
    }
    
    void registerError(const std::string& code, 
                      const std::string& description,
                      ErrorSeverity severity,
                      const std::string& category) {
        registry[code] = {code, description, severity, category};
    }
    
    const ErrorInfo* find(const std::string& code) const {
        auto it = registry.find(code);
        if (it != registry.end()) {
            return &it->second;
        }
        return nullptr;
    }
    
    // Helper to generate formatted error
    std::unique_ptr<CompilerError> createError(const std::string& code,
                                               const SourceLocation& loc,
                                               const std::string& extra = "") const {
        const ErrorInfo* info = find(code);
        if (!info) {
            return std::make_unique<CompilerError>(
                ErrorSeverity::ERROR, 
                "Unknown error code: " + code + (extra.empty() ? "" : " - " + extra),
                loc,
                code);
        }
        
        std::string message = info->description;
        if (!extra.empty()) {
            message += ": " + extra;
        }
        
        return std::make_unique<CompilerError>(info->severity, message, loc, code);
    }
    
    void dumpAll(std::ostream& out) const {
        out << "Registered Error Codes:\n";
        out << "======================\n";
        
        std::vector<const ErrorInfo*> sorted;
        for (const auto& pair : registry) {
            sorted.push_back(&pair.second);
        }
        
        std::sort(sorted.begin(), sorted.end(), 
            [](const ErrorInfo* a, const ErrorInfo* b) {
                return a->code < b->code;
            });
        
        for (const auto* info : sorted) {
            out << info->code << " [" << info->category << "] "
                << severityToString(info->severity) << ": "
                << info->description << "\n";
        }
    }
};

// Initialize static member
// ErrorCodeRegistry* ErrorCodeRegistry::instance = nullptr; // (moved to error_types.cpp to avoid multiple-definition when header is included in multiple TUs)

// ==================== Predefined Error Codes ====================
namespace ErrorCodes {
    // Lexer errors (LXXX)
    constexpr const char* UNKNOWN_CHARACTER = "L001";
    constexpr const char* UNTERMINATED_STRING = "L002";
    constexpr const char* INVALID_NUMBER = "L003";
    
    // Parser errors (PXXX)
    constexpr const char* UNEXPECTED_TOKEN = "P001";
    constexpr const char* MISSING_SEMICOLON = "P002";
    constexpr const char* MISSING_PAREN = "P003";
    
    // Type errors (TXXX)
    constexpr const char* TYPE_MISMATCH = "T001";
    constexpr const char* UNDECLARED_IDENTIFIER = "T002";
    constexpr const char* INVALID_CONVERSION = "T003";
    
    // Semantic errors (SXXX)
    constexpr const char* INVALID_MAIN = "S001";
    constexpr const char* MISSING_MAIN = "S002";
    
    // Codegen errors (CXXX)
    constexpr const char* LLVM_INIT_FAILED = "C001";
    constexpr const char* MODULE_CREATION_FAILED = "C002";
    
    // Internal errors (IXXX)
    constexpr const char* INTERNAL_ERROR = "I001";
}

} // namespace holycpp