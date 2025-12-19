#pragma once
#include <string>
#include <memory>
#include <vector>
#include <utility>
#include <sstream>
#include <iostream>

namespace holycpp {

// ==================== Error Severity ====================
enum class ErrorSeverity {
    NOTE,       // Informational
    WARNING,    // Warning - can continue
    ERROR,      // Error - cannot continue compilation
    FATAL       // Fatal - must stop immediately
};

inline std::string severityToString(ErrorSeverity severity) {
    switch (severity) {
        case ErrorSeverity::NOTE: return "note";
        case ErrorSeverity::WARNING: return "warning";
        case ErrorSeverity::ERROR: return "error";
        case ErrorSeverity::FATAL: return "fatal";
        default: return "unknown";
    }
}

// ==================== Source Location ====================
struct SourceLocation {
    std::string filename;
    int line = 1;
    int column = 1;
    int length = 1;  // Length of token/expression
    
    SourceLocation() = default;
    SourceLocation(const std::string& file, int ln, int col, int len = 1)
        : filename(file), line(ln), column(col), length(len) {}
    
    std::string toString() const {
        if (filename.empty()) return "";
        std::stringstream ss;
        ss << filename << ":" << line << ":" << column;
        return ss.str();
    }
    
    bool isValid() const { return !filename.empty() && line > 0 && column > 0; }
};

// ==================== Base Error Class ====================
class CompilerError {
protected:
    ErrorSeverity severity;
    SourceLocation location;
    std::string message;
    std::string errorCode;  // e.g., "E001", "W042"
    
public:
    CompilerError(ErrorSeverity sev, const std::string& msg, 
                  const SourceLocation& loc = {}, 
                  const std::string& code = "")
        : severity(sev), location(loc), message(msg), errorCode(code) {}
    
    virtual ~CompilerError() = default;
    
    // Getters
    ErrorSeverity getSeverity() const { return severity; }
    const SourceLocation& getLocation() const { return location; }
    const std::string& getMessage() const { return message; }
    const std::string& getErrorCode() const { return errorCode; }
    
    // Format the error for display
    virtual std::string format() const {
        std::stringstream ss;
        
        if (!errorCode.empty()) {
            ss << errorCode << ": ";
        }
        
        ss << severityToString(severity) << ": " << message;
        
        if (location.isValid()) {
            ss << "\n  at " << location.toString();
        }
        
        return ss.str();
    }
    
    // Check if error is fatal
    bool isFatal() const { return severity == ErrorSeverity::FATAL; }
    bool isError() const { return severity == ErrorSeverity::ERROR || isFatal(); }
    
    // Create formatted message with location
    static std::string formatMessage(const std::string& filename, 
                                    int line, int col,
                                    const std::string& message) {
        std::stringstream ss;
        ss << filename << ":" << line << ":" << col << ": " << message;
        return ss.str();
    }
};

// ==================== Error With Context ====================
class ContextualError : public CompilerError {
    std::vector<std::string> contextStack;
    
public:
    ContextualError(ErrorSeverity sev, const std::string& msg,
                    const SourceLocation& loc = {},
                    const std::string& code = "")
        : CompilerError(sev, msg, loc, code) {}
    
    void pushContext(const std::string& context) {
        contextStack.push_back(context);
    }
    
    void popContext() {
        if (!contextStack.empty()) contextStack.pop_back();
    }
    
    std::string format() const override {
        std::string base = CompilerError::format();
        std::stringstream ss;
        ss << base;
        
        for (const auto& ctx : contextStack) {
            ss << "\n  in " << ctx;
        }
        
        return ss.str();
    }
    
    const std::vector<std::string>& getContext() const { return contextStack; }
};

// ==================== Error Builder ====================
class ErrorBuilder {
    ErrorSeverity sev;
    SourceLocation location;
    std::stringstream message;
    std::string errorCode;
    std::vector<std::string> contextStack;
    
public:
    ErrorBuilder(ErrorSeverity sev, const SourceLocation& loc = {})
        : sev(sev), location(loc) {}
    
    ErrorBuilder(ErrorSeverity sev, const std::string& code)
        : sev(sev), errorCode(code) {}
    
    // Chainable setters
    ErrorBuilder& severity(ErrorSeverity s) { sev = s; return *this; }
    ErrorBuilder& code(const std::string& c) { errorCode = c; return *this; }
     
     // Message building
     template<typename T>
     ErrorBuilder& operator<<(const T& value) {
         message << value;
         return *this;
     }
     
     // Location setters
     ErrorBuilder& at(const SourceLocation& loc) {
         location = loc;
         return *this;
     }
     
     ErrorBuilder& at(const std::string& file, int line, int col) {
         location = SourceLocation(file, line, col);
         return *this;
     }
     
     // Context management
     ErrorBuilder& inContext(const std::string& context) {
         contextStack.push_back(context);
         return *this;
     }
     
     // Create the error
     std::unique_ptr<ContextualError> build() {
         auto error = std::make_unique<ContextualError>(
            sev, message.str(), location, errorCode);
         
         for (const auto& ctx : contextStack) {
             error->pushContext(ctx);
         }
         
         return error;
     }
 };
 
// ==================== Error Manager ====================
class ErrorManager {
    static ErrorManager* instance;
    
    std::vector<std::unique_ptr<CompilerError>> errors;
    int warningCount = 0;
    int errorCount = 0;
    int fatalCount = 0;
    
    // Settings
    bool warningsAsErrors = false;
    bool suppressWarnings = false;
    int maxErrors = 100;
    
    ErrorManager() = default;
    
public:
    static ErrorManager& get() {
        if (!instance) {
            instance = new ErrorManager();
        }
        return *instance;
    }
    
    // Error reporting
    void report(std::unique_ptr<CompilerError> error) {
        if (!error) return;
        
        // Check if we should treat warnings as errors
        if (warningsAsErrors && error->getSeverity() == ErrorSeverity::WARNING) {
            // Create a new error with same message but ERROR severity
            auto newError = std::make_unique<CompilerError>(
                ErrorSeverity::ERROR,
                error->getMessage(),
                error->getLocation(),
                error->getErrorCode());
            error = std::move(newError);
        }
        
        // Suppress warnings if configured
        if (suppressWarnings && error->getSeverity() == ErrorSeverity::WARNING) {
            return;
        }
        
        // Update counters
        switch (error->getSeverity()) {
            case ErrorSeverity::WARNING:
                warningCount++;
                break;
            case ErrorSeverity::ERROR:
                errorCount++;
                break;
            case ErrorSeverity::FATAL:
                fatalCount++;
                break;
            default:
                break;
        }
        
        errors.push_back(std::move(error));
        
        // Output the error immediately
        if (errors.back()->isError()) {
            std::cerr << errors.back()->format() << std::endl;
        }
        
        // Check if we've reached max errors
        if (errorCount + fatalCount >= maxErrors) {
            report(ErrorSeverity::FATAL, 
                   "Too many errors emitted, stopping compilation",
                   SourceLocation(), "F001");
        }
    }
    
    void report(ErrorSeverity severity, const std::string& message,
                const SourceLocation& location = {},
                const std::string& code = "") {
        report(std::make_unique<CompilerError>(severity, message, location, code));
    }
    
    // Quick reporting methods
    void error(const std::string& msg, const SourceLocation& loc = {}) {
        report(ErrorSeverity::ERROR, msg, loc);
    }
    
    void warning(const std::string& msg, const SourceLocation& loc = {}) {
        report(ErrorSeverity::WARNING, msg, loc);
    }
    
    void fatal(const std::string& msg, const SourceLocation& loc = {}) {
        report(ErrorSeverity::FATAL, msg, loc);
    }
    
    void note(const std::string& msg, const SourceLocation& loc = {}) {
        report(ErrorSeverity::NOTE, msg, loc);
    }
    int getNoteCount() const { return static_cast<int>(errors.size()) - (warningCount + errorCount + fatalCount); }
    // Builder methods
    ErrorBuilder buildError() { return ErrorBuilder(ErrorSeverity::ERROR); }
    ErrorBuilder buildWarning() { return ErrorBuilder(ErrorSeverity::WARNING); }
    ErrorBuilder buildFatal() { return ErrorBuilder(ErrorSeverity::FATAL); }
    ErrorBuilder buildNote() { return ErrorBuilder(ErrorSeverity::NOTE); }
    
    // Statistics
    int getErrorCount() const { return errorCount + fatalCount; }
    int getWarningCount() const { return warningCount; }
    int getTotalCount() const { return static_cast<int>(errors.size()); }
    
    // Check if compilation can continue
    bool hasErrors() const { return errorCount > 0 || fatalCount > 0; }
    bool hasFatal() const { return fatalCount > 0; }
    
    // Clear all errors
    void clear() {
        errors.clear();
        warningCount = 0;
        errorCount = 0;
        fatalCount = 0;
    }
    
    // Output all errors
    void dumpAll(std::ostream& out = std::cerr) const {
        for (const auto& error : errors) {
            out << error->format() << std::endl;
        }
    }
    
    // Settings
    void setWarningsAsErrors(bool value) { warningsAsErrors = value; }
    void setSuppressWarnings(bool value) { suppressWarnings = value; }
    void setMaxErrors(int max) { maxErrors = max; }
    
    // Get all errors (for testing/debugging)
    const std::vector<std::unique_ptr<CompilerError>>& getAllErrors() const {
        return errors;
    }
};

// Initialize static member
// ErrorManager* ErrorManager::instance = nullptr; // (moved to error.cpp to avoid multiple-definition when header is included in multiple TUs)

// ==================== Macros for Easy Error Reporting ====================
// Note: Use these judiciously, prefer the ErrorBuilder pattern

#define ERROR_AT(loc, msg) \
    holycpp::ErrorManager::get().error(msg, loc)

#define WARNING_AT(loc, msg) \
    holycpp::ErrorManager::get().warning(msg, loc)

#define FATAL_AT(loc, msg) \
    holycpp::ErrorManager::get().fatal(msg, loc)

#define NOTE_AT(loc, msg) \
    holycpp::ErrorManager::get().note(msg, loc)

// Context manager for automatic error context
class ErrorContext {
    std::string context;
    
public:
    ErrorContext(const std::string& ctx) : context(ctx) {
        // Could push to a thread-local context stack
        // For now, this is a placeholder for future implementation
    }
    
    ~ErrorContext() {
        // Pop from context stack
    }
};

} // namespace holycpp