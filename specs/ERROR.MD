# HolyC++ Error System Specification

## 📋 Overview

The HolyC++ Error System provides comprehensive error handling for the HolyC++ compiler and runtime. It combines HolyC philosophy with modern error handling practices, offering both immediate error reporting and structured error collection for complex compilation scenarios.

**Design Philosophy**: "Fail fast with helpful messages, but recover gracefully when possible."

## 🎯 Core Design Goals

### 1. **HolyC Compatibility**
- Match HolyC's error reporting style where appropriate
- Preserve HolyC's direct error messages
- Support HolyC's compilation model

### 2. **Modern Safety**
- Prevent undefined behavior through exceptions
- Provide optional bounds checking
- Enable comprehensive error tracking

### 3. **Educational Value**
- Clean, readable error implementation
- Useful for studying compiler error handling
- Well-documented error conditions

### 4. **Performance**
- Zero-cost when no errors occur
- Configurable error checking levels
- Efficient error storage and formatting

## 📊 Error System Architecture

### **Core Components**

```
Error System
├── Error Severity Levels
├── Source Location Tracking
├── Base Error Classes
├── Error Manager (Singleton)
├── Error Builder Pattern
├── Error Code Registry
└── Specialized Error Types
```

### **1. Error Severity Levels**

```cpp
enum class ErrorSeverity {
    NOTE,       // Informational messages
    WARNING,    // Non-fatal issues (can be suppressed)
    ERROR,      // Compilation cannot continue
    FATAL       // Must stop immediately
};
```

**Usage Guidelines**:
- **NOTE**: Diagnostics, progress messages, hints
- **WARNING**: Potential issues, deprecations, style violations
- **ERROR**: Syntax errors, type mismatches, undefined symbols
- **FATAL**: Internal errors, resource exhaustion, unrecoverable states

### **2. Source Location Tracking**

```cpp
struct SourceLocation {
    std::string filename;  // "program.hc"
    int line = 1;          // 1-based line number
    int column = 1;        // 1-based column number
    int length = 1;        // Length of offending token/expression
    
    std::string toString() const;  // "program.hc:42:15"
    bool isValid() const;          // Has filename and valid positions
};
```

**Features**:
- Line/column tracking (1-based like most editors)
- Token length for highlighting
- File-relative paths
- Validation to avoid misleading locations

### **3. Base Error Classes**

#### **CompilerError** (Base Class)
```cpp
class CompilerError {
    ErrorSeverity severity;
    SourceLocation location;
    std::string message;
    std::string errorCode;  // e.g., "E001", "W042"
    
    virtual std::string format() const;
    bool isFatal() const;
    bool isError() const;
};
```

#### **ContextualError** (Extended Class)
```cpp
class ContextualError : public CompilerError {
    std::vector<std::string> contextStack;
    
    void pushContext(const std::string& context);
    void popContext();
    std::string format() const override;
};
```

**Context Stack Example**:
```
Type mismatch: Cannot convert 'float' to 'int'
  at program.hc:42:15
  in assignment to variable 'result'
  in function 'calculate_total'
  during template instantiation
```

#### **InternalError** (Fatal Errors)
```cpp
class InternalError : public CompilerError {
    // Always FATAL severity
    // Includes bug reporting instructions
};
```

### **4. Error Manager (Singleton)**

#### **Responsibilities**:
- Collect and manage all errors during compilation
- Provide statistics (error/warning counts)
- Configure error handling behavior
- Control error output and formatting

#### **Configuration Options**:
```cpp
ErrorManager::get().setWarningsAsErrors(true);   // Treat warnings as errors
ErrorManager::get().setSuppressWarnings(true);   // Don't show warnings
ErrorManager::get().setMaxErrors(50);            // Stop after 50 errors
```

#### **Statistics**:
```cpp
int getErrorCount() const;     // ERROR + FATAL errors
int getWarningCount() const;   // WARNING messages
int getNoteCount() const;      // NOTE messages
int getTotalCount() const;     // All messages
bool hasErrors() const;        // Any errors?
bool hasFatal() const;         // Any fatal errors?
```

### **5. Error Builder Pattern**

#### **Fluent Interface**:
```cpp
auto error = (ErrorManager::get().buildError()
    .severity(ErrorSeverity::ERROR)
    .code("T001")
    .at("program.hc", 42, 15)
    << "Cannot convert '" << type1 << "' to '" << type2 << "'")
    .inContext("In function 'calculate'")
    .inContext("While processing argument 2")
    .build();
```

#### **Builder Methods**:
- `.severity()` - Set error severity
- `.code()` - Set error code
- `.at()` - Set source location
- `.inContext()` - Add context message
- `operator<<` - Build error message
- `.build()` - Create the error object

### **6. Error Code Registry**

#### **Error Code Format**:
```
[Category][Number]
L001 - Lexer error #1
P042 - Parser error #42  
T015 - Type error #15
S008 - Semantic error #8
C003 - CodeGen error #3
I002 - Internal error #2
```

#### **Registry Structure**:
```cpp
struct ErrorInfo {
    std::string code;          // "T001"
    std::string description;   // "Type mismatch"
    ErrorSeverity severity;    // ErrorSeverity::ERROR
    std::string category;      // "Type"
};
```

#### **Predefined Error Codes**:

**Lexer Errors (L001-L099)**:
- L001: Unknown character
- L002: Unterminated string literal
- L003: Invalid number format
- L004: Unterminated character literal
- L005: Invalid escape sequence
- L006: Number too large for type

**Parser Errors (P001-P099)**:
- P001: Unexpected token
- P002: Expected token
- P003: Missing semicolon
- P004: Missing parenthesis
- P005: Missing brace
- P006: Missing bracket
- P007: Invalid type
- P008: Invalid expression
- P009: Duplicate declaration
- P010: Invalid function definition

**Type Errors (T001-T099)**:
- T001: Type mismatch
- T002: Undeclared identifier
- T003: Redeclaration of identifier
- T004: Invalid type conversion
- T005: Invalid operand types for operator
- T006: Invalid function call
- T007: Invalid array size
- T008: Invalid pointer operation
- T009: Invalid assignment
- T010: Invalid return type
- T011: Missing return statement (WARNING)
- T012: Division by zero

**Semantic Errors (S001-S099)**:
- S001: Invalid main function
- S002: Missing main function
- S003: Invalid linkage specification
- S004: Invalid storage class
- S005: Cyclic dependency
- S006: Invalid initializer
- S007: Non-constant initializer
- S008: Invalid constant expression
- S009: Undefined reference
- S010: Multiple definition

**CodeGen Errors (C001-C099)**:
- C001: LLVM initialization failed (FATAL)
- C002: Module creation failed (FATAL)
- C003: Function creation failed (FATAL)
- C004: Type creation failed (FATAL)
- C005: Invalid IR generated (FATAL)
- C006: Optimization failed (WARNING)
- C007: Target not found (FATAL)
- C008: Object file emission failed (FATAL)

**Internal Errors (I001-I099)**:
- I001: Internal compiler error (FATAL)
- I002: Assertion failed (FATAL)
- I003: Out of memory (FATAL)

### **7. Specialized Error Types**

#### **LexerError**:
```cpp
class LexerError : public ContextualError {
    enum class Code {
        UNKNOWN_CHAR,
        UNTERMINATED_STRING,
        UNTERMINATED_CHAR,
        INVALID_NUMBER,
        INVALID_ESCAPE,
        NUMBER_TOO_LARGE
    };
};
```

#### **ParserError**:
```cpp
class ParserError : public ContextualError {
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
};
```

#### **TypeError**:
```cpp
class TypeError : public ContextualError {
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
};
```

#### **SemanticError**:
```cpp
class SemanticError : public ContextualError {
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
};
```

#### **CodeGenError**:
```cpp
class CodeGenError : public ContextualError {
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
};
```

## 🔄 Error Flow

### **1. Error Creation**:
```cpp
// Method 1: Direct reporting
ErrorManager::get().error("Syntax error", SourceLocation("file.hc", 10, 5));

// Method 2: Using builder
auto error = ErrorManager::get().buildError()
    .at("file.hc", 42, 15)
    .code("T001")
    << "Type mismatch"
    .build();
ErrorManager::get().report(std::move(error));

// Method 3: From error code
auto error = ErrorCodeRegistry::get().createError(
    "T002", 
    SourceLocation("file.hc", 5, 2),
    "identifier 'unknownVar'"
);
```

### **2. Error Processing**:
```
1. Error created with severity and location
2. Sent to ErrorManager::report()
3. Check suppression rules (warnings as errors, etc.)
4. Update statistics
5. Store in error list
6. Output to stderr if ERROR or FATAL
7. Check max error limit
8. Continue or abort based on severity
```

### **3. Error Output**:
```
T001: error: Type mismatch: Cannot convert 'float' to 'int'
  at program.hc:42:15
  in assignment to variable 'result'
  in function 'calculate_total'
```

## ⚡ Performance Considerations

### **Fast Path (No Errors)**:
- Singleton access is thread-local cached
- Error checking via `constexpr` where possible
- Inline functions for common operations
- Zero allocations when no errors reported

### **Error Path (When Errors Occur)**:
- Lazy string formatting
- Reusable string buffers
- Move semantics for error objects
- Configurable error collection limits

### **Memory Management**:
- Errors stored in `std::unique_ptr`
- Context strings shared where possible
- Location objects passed by reference
- Error codes stored as strings (interned)

## 🛡️ Safety Features

### **1. Bounds Checking**:
```cpp
// All constructors validate input ranges
U8 val = 300;  // Throws std::out_of_range
I8 neg = -150; // Throws std::out_of_range
```

### **2. Division Safety**:
```cpp
U32 a = 100;
U32 b = 0;
U32 result = a / b;  // Throws std::domain_error
```

### **3. Shift Safety**:
```cpp
U32 val = 1;
U32 result = val << 33;  // Throws std::out_of_range (for 32-bit)
```

### **4. Overflow Detection**:
```cpp
U8 max = U8::MAX;
U8 result = max.checked_add(U8(1));  // Throws std::overflow_error
```

## 🔧 Configuration

### **Compile-time Options**:
```cpp
#define HOLYC_ERROR_CHECKING_LEVEL 2
// 0: No checking (HolyC compatibility)
// 1: Basic checking (recommended)
// 2: Full checking (debug builds)
// 3: Paranoid checking (testing)
```

### **Runtime Configuration**:
```cpp
// In user code:
ErrorManager::get().setSuppressWarnings(true);
ErrorManager::get().setMaxErrors(10);
ErrorManager::get().setWarningsAsErrors(false);

// Via command line:
// holyc++ --warnings-as-errors --max-errors=50 program.hc
```

### **Environment Variables**:
```bash
export HOLYC_ERROR_COLOR=1      # Colored output
export HOLYC_ERROR_VERBOSE=2    # Verbosity level
export HOLYC_ERROR_FORMAT=json  # Output format
```

## 📊 Error Formatting

### **Text Format** (Default):
```
L002: error: Unterminated string literal
  at program.hc:10:5
  string started here
```

### **JSON Format** (Optional):
```json
{
  "error": {
    "code": "L002",
    "severity": "ERROR",
    "message": "Unterminated string literal",
    "location": {
      "file": "program.hc",
      "line": 10,
      "column": 5,
      "length": 1
    },
    "context": ["string started here"],
    "timestamp": "2024-01-15T10:30:00Z"
  }
}
```

### **Machine-Readable Format**:
```
program.hc:10:5: error L002: Unterminated string literal
```

## 🔍 Debugging Support

### **Error Context Stack**:
```cpp
// Manual context management
{
    ErrorContext ctx("Parsing function declaration");
    // Any errors here get this context
}

// Automatic via RAII
class ScopedErrorContext {
    ScopedErrorContext(const std::string& ctx) {
        // Push context
    }
    ~ScopedErrorContext() {
        // Pop context
    }
};
```

### **Error Correlation**:
- Related errors grouped together
- Common root causes identified
- Suggested fixes when possible
- Reference to documentation

### **Debug Information**:
```cpp
// Enable debug output
ErrorManager::get().setDebugMode(true);

// Get detailed error information
const auto& errors = ErrorManager::get().getAllErrors();
for (const auto& err : errors) {
    std::cout << "Error type: " << typeid(*err).name() << "\n";
    std::cout << "Backtrace available: " << err->hasBacktrace() << "\n";
}
```

## 🧪 Testing

### **Unit Tests**:
```cpp
// tests/test_error.cpp
void test_basic_errors() {
    CompilerError note(ErrorSeverity::NOTE, "Info");
    assert(note.getSeverity() == ErrorSeverity::NOTE);
    assert(!note.isError());
}

void test_error_manager() {
    ErrorManager::get().clear();
    ErrorManager::get().error("Test error");
    assert(ErrorManager::get().hasErrors());
}
```

### **Integration Tests**:
```cpp
// Test error recovery
void test_error_recovery() {
    ErrorManager::get().setMaxErrors(3);
    ErrorManager::get().error("Error 1");
    ErrorManager::get().error("Error 2");
    ErrorManager::get().error("Error 3");  // Should trigger fatal
    assert(ErrorManager::get().hasFatal());
}
```

### **Performance Tests**:
```cpp
// Benchmark error creation
void benchmark_error_creation() {
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 100000; ++i) {
        ErrorManager::get().note("Benchmark message");
    }
    auto end = std::chrono::high_resolution_clock::now();
    // Report timing
}
```

## 🔮 Future Extensions

### **Planned Features**:

#### **1. Error Recovery Strategies**:
```cpp
enum class RecoveryStrategy {
    NONE,           // Stop on first error
    SKIP_TOKEN,     // Skip offending token
    SKIP_LINE,      // Skip to next line
    SKIP_FUNCTION,  // Skip current function
    SKIP_FILE       // Skip to next file
};
```

#### **2. Multilingual Support**:
```cpp
// Error messages in different languages
ErrorManager::get().setLanguage("en_US");
ErrorManager::get().setLanguage("es_ES");
ErrorManager::get().setLanguage("ja_JP");
```

#### **3. Error Suggestions**:
```
T002: error: Undeclared identifier 'foo'
  at program.hc:5:10
  Did you mean: 'food', 'foot', 'for'?
```

#### **4. Error Categories**:
```cpp
enum class ErrorCategory {
    SYNTAX,      // Parsing errors
    SEMANTIC,    // Meaning errors
    TYPE,        // Type system errors
    MEMORY,      // Memory management errors
    OPTIMIZATION,// Optimization errors
    LINKING,     // Linking errors
    INTERNAL     // Compiler internal errors
};
```

#### **5. Error Severity Escalation**:
```cpp
// Warnings that become errors after N occurrences
ErrorManager::get().setEscalation("W123", 5); // After 5, becomes error
```

### **Advanced Features** (For Future):

#### **1. Error Databases**:
- Store common error patterns
- Suggest fixes based on history
- Learn from user corrections

#### **2. Machine Learning**:
- Predict error causes
- Suggest optimal fixes
- Identify error patterns

#### **3. Interactive Error Resolution**:
```bash
$ holyc++ program.hc
T001: Type mismatch at line 42
  Suggested fix: Change 'float' to 'int'? [Y/n]
```

#### **4. Error Telemetry** (Opt-in):
- Collect anonymous error statistics
- Improve error messages based on usage
- Identify common pitfalls

## 📚 Usage Examples

### **Basic Usage**:
```cpp
#include "src/lib/error.hpp"

int main() {
    // Report an error
    ErrorManager::get().error("Syntax error");
    
    // Check if compilation can continue
    if (ErrorManager::get().hasErrors()) {
        return 1;
    }
    return 0;
}
```

### **Advanced Usage**:
```cpp
// Complex error with context
auto error = (ErrorManager::get().buildError()
    .severity(ErrorSeverity::ERROR)
    .code("T004")
    .at("program.hc", 42, 15)
    << "Cannot convert '" << sourceType << "' to '" << targetType << "'")
    .inContext("In assignment to variable '" + varName + "'")
    .inContext("In function '" + funcName + "'")
    .build();

// Report with custom handling
if (shouldReportError(*error)) {
    ErrorManager::get().report(std::move(error));
}
```

### **Error Suppression**:
```cpp
// Suppress specific warnings
ErrorManager::get().addSuppression("W123");  // Suppress warning W123
ErrorManager::get().addSuppression("T011");  // Suppress missing return warning

// Suppress in specific contexts
ErrorManager::get().addContextualSuppression(
    "W042", 
    "file.hc", 
    "function deprecatedButNeeded"
);
```

## 🏁 Implementation Status

### **✅ Complete**:
- Core error classes (CompilerError, ContextualError, InternalError)
- Error manager with statistics
- Source location tracking
- Error builder pattern
- Basic error code registry
- Comprehensive test suite

### **🟡 In Progress**:
- Error recovery mechanisms
- Performance optimizations
- Advanced error formatting

### **📋 Planned**:
- Multilingual error messages
- Error suggestion system
- Interactive error resolution
- Error telemetry (opt-in)

## 📝 References

1. [HolyC Language Specification](https://holyc-lang.com/docs/language-spec)
2. [LLVM Error Handling Guide](https://llvm.org/docs/ErrorHandling.html)
3. [Clang Diagnostics System](https://clang.llvm.org/diagnostics.html)
4. [Rust Error Handling](https://doc.rust-lang.org/book/ch09-00-error-handling.html)

---

**Maintainer**: @DebadityaMalakar  
**Last Updated**: 2024-01-15  
**Version**: 1.0.0 (Foundational Implementation)

*"Good error messages turn compiler users into compiler allies." - HolyC++ Error Philosophy*