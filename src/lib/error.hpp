#pragma once

#include <string>
#include <vector>
#include <memory>
#include <sstream>
#include <iostream>
#include <functional>

namespace holycpp {

// ==================== Error Severity ====================
enum class ErrorSeverity {
    NOTE,       // Informational messages
    WARNING,    // Non-fatal issues (can be suppressed)
    ERROR,      // Compilation cannot continue
    FATAL       // Must stop immediately
};

std::string severityToString(ErrorSeverity severity);

// ==================== Source Location ====================
struct SourceLocation {
    std::string filename;
    int line = 1;
    int column = 1;
    int length = 1;
    
    SourceLocation() = default;
    
    SourceLocation(const std::string& file, int l, int c, int len = 1)
        : filename(file), line(l), column(c), length(len) {}
    
    std::string toString() const;
    bool isValid() const;
};

// ==================== Base Compiler Error ====================
class CompilerError {
protected:
    ErrorSeverity severity;
    std::string message;
    SourceLocation location;
    std::string errorCode;
    
public:
    CompilerError(ErrorSeverity sev = ErrorSeverity::ERROR,
                  const std::string& msg = "",
                  const SourceLocation& loc = {},
                  const std::string& code = "");
    
    virtual ~CompilerError() = default;
    
    ErrorSeverity getSeverity() const { return severity; }
    const std::string& getMessage() const { return message; }
    const SourceLocation& getLocation() const { return location; }
    const std::string& getErrorCode() const { return errorCode; }
    
    bool isError() const;
    bool isFatal() const;
    bool isWarning() const;
    bool isNote() const;
    
    virtual std::string format() const;
    
    static std::string formatMessage(const std::string& file, int line, 
                                     int column, const std::string& msg);
};

// ==================== Contextual Error ====================
class ContextualError : public CompilerError {
protected:
    std::vector<std::string> contextStack;
    
public:
    ContextualError(ErrorSeverity sev = ErrorSeverity::ERROR,
                   const std::string& msg = "",
                   const SourceLocation& loc = {},
                   const std::string& code = "");
    
    void pushContext(const std::string& context);
    void popContext();
    const std::vector<std::string>& getContext() const { return contextStack; }
    
    std::string format() const override;
};

// ==================== Error Builder ====================
class ErrorBuilder {
private:
    std::unique_ptr<ContextualError> error;
    std::stringstream messageStream;
    
public:
    ErrorBuilder();
    
    ErrorBuilder& severity(ErrorSeverity sev);
    ErrorBuilder& code(const std::string& code);
    ErrorBuilder& at(const std::string& filename, int line, int column, int length = 1);
    ErrorBuilder& inContext(const std::string& context);
    
    template<typename T>
    ErrorBuilder& operator<<(const T& value) {
        messageStream << value;
        return *this;
    }
    
    std::unique_ptr<CompilerError> build();
};

// ==================== Error Manager (Singleton) ====================
class ErrorManager {
private:
    static ErrorManager* instance;
    
    std::vector<std::unique_ptr<CompilerError>> errors;
    int errorCount = 0;
    int warningCount = 0;
    int noteCount = 0;
    int maxErrors = 100;
    bool suppressWarnings = false;
    bool warningsAsErrors = false;
    bool hasFatalError = false;
    
    ErrorManager() = default;
    
public:
    static ErrorManager& get();
    
    // Reporting methods
    void report(std::unique_ptr<CompilerError> error);
    
    void note(const std::string& message, 
              const SourceLocation& loc = {});
    void warning(const std::string& message, 
                 const SourceLocation& loc = {});
    void error(const std::string& message, 
               const SourceLocation& loc = {});
    void fatal(const std::string& message, 
               const SourceLocation& loc = {});
    
    // Builder
    ErrorBuilder buildError();
    
    // Configuration
    void setMaxErrors(int max);
    void setSuppressWarnings(bool suppress);
    void setWarningsAsErrors(bool asErrors);
    
    // Statistics
    int getErrorCount() const { return errorCount; }
    int getWarningCount() const { return warningCount; }
    int getNoteCount() const { return noteCount; }
    int getTotalCount() const { return errorCount + warningCount + noteCount; }
    bool hasErrors() const { return errorCount > 0 || hasFatalError; }
    bool hasFatal() const { return hasFatalError; }
    
    // Access
    const std::vector<std::unique_ptr<CompilerError>>& getErrors() const {
        return errors;
    }
    
    // Utility
    void clear();
    void dumpAll(std::ostream& out = std::cerr) const;
};

} // namespace holycpp