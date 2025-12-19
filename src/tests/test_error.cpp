#include "../lib/error.hpp"
#include "../lib/error_types.hpp"
#include <iostream>
#include <cassert>
#include <memory>
#include <sstream>

using namespace holycpp;

// Test function prototypes
void test_basic_errors();
void test_source_location();
void test_error_builder();
void test_error_manager();
void test_contextual_errors();
void test_error_codes();
void test_internal_error();
void test_error_formatting();

int main() {
    std::cout << "🧪 Running HolyC++ Error System Tests\n";
    std::cout << "=====================================\n";
    
    try {
        test_basic_errors();
        test_source_location();
        test_error_builder();
        test_error_manager();
        test_contextual_errors();
        test_error_codes();
        test_internal_error();
        test_error_formatting();
        
        std::cout << "\n✅ All error system tests passed!\n";
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "\n❌ Test failed: " << e.what() << "\n";
        return 1;
    }
}

void test_basic_errors() {
    std::cout << "\n🔹 Testing basic error types...\n";
    
    // Create errors with different severities
    CompilerError note(ErrorSeverity::NOTE, "Informational message");
    assert(note.getSeverity() == ErrorSeverity::NOTE);
    assert(note.getMessage() == "Informational message");
    assert(!note.isError());
    assert(!note.isFatal());
    
    CompilerError warning(ErrorSeverity::WARNING, "Something might be wrong");
    assert(warning.getSeverity() == ErrorSeverity::WARNING);
    assert(!warning.isError());
    assert(!warning.isFatal());
    
    CompilerError error(ErrorSeverity::ERROR, "Something went wrong");
    assert(error.getSeverity() == ErrorSeverity::ERROR);
    assert(error.isError());
    assert(!error.isFatal());
    
    CompilerError fatal(ErrorSeverity::FATAL, "Cannot continue");
    assert(fatal.getSeverity() == ErrorSeverity::FATAL);
    assert(fatal.isError());
    assert(fatal.isFatal());
    
    // Test formatting
    std::string fmt = note.format();
    assert(fmt.find("note") != std::string::npos);
    assert(fmt.find("Informational message") != std::string::npos);
    
    fmt = error.format();
    assert(fmt.find("error") != std::string::npos);
    assert(fmt.find("Something went wrong") != std::string::npos);
    
    std::cout << "  ✓ Basic error creation and properties\n";
}

void test_source_location() {
    std::cout << "\n🔹 Testing source location...\n";
    
    // Default location
    SourceLocation defaultLoc;
    assert(defaultLoc.line == 1);
    assert(defaultLoc.column == 1);
    assert(defaultLoc.filename.empty());
    assert(!defaultLoc.isValid());  // Invalid because filename empty
    
    // Custom location
    SourceLocation loc("test.hc", 42, 15, 5);
    assert(loc.filename == "test.hc");
    assert(loc.line == 42);
    assert(loc.column == 15);
    assert(loc.length == 5);
    assert(loc.isValid());
    
    // Test toString
    std::string locStr = loc.toString();
    assert(locStr == "test.hc:42:15");
    
    // Error with location
    CompilerError err(ErrorSeverity::ERROR, "Syntax error", loc);
    std::string errStr = err.format();
    assert(errStr.find("test.hc:42:15") != std::string::npos);
    
    // Test formatMessage static method
    std::string msg = CompilerError::formatMessage("file.cpp", 10, 5, "Test message");
    assert(msg == "file.cpp:10:5: Test message");
    
    std::cout << "  ✓ Source location tracking\n";
}

void test_error_builder() {
    std::cout << "\n🔹 Testing error builder pattern...\n";
    
    // Simple error
    {
        auto builtError = (ErrorManager::get().buildError()
            .at("test.cpp", 10, 20)
            << "Failed to parse expression: " << "x + y")
            .build();
        assert(builtError->getSeverity() == ErrorSeverity::ERROR);
        assert(builtError->getMessage() == "Failed to parse expression: x + y");
        assert(builtError->getLocation().filename == "test.cpp");
        assert(builtError->getLocation().line == 10);
        assert(builtError->getLocation().column == 20);
    }
    
    // Error with code
    {
        auto builtError = (ErrorManager::get().buildError()
            .severity(ErrorSeverity::WARNING)
            .code("W123")
            .at("file.hc", 5, 3)
            << "Unused variable 'counter'")
            .build();
        assert(builtError->getSeverity() == ErrorSeverity::WARNING);
        assert(builtError->getErrorCode() == "W123");
    }
    
    // Chained operations
    {
        auto error = (ErrorManager::get().buildError()
            .at("range.cpp", 30, 8)
            << "Value " << 42 << " is out of range [0, " << 10 << "]")
            .build();
        assert(error->getMessage() == "Value 42 is out of range [0, 10]");
    }
    
    std::cout << "  ✓ Error builder pattern works\n";
}

void test_error_manager() {
    std::cout << "\n🔹 Testing error manager...\n";
    
    // Clear any previous errors
    ErrorManager::get().clear();
    
    // Report various errors
    ErrorManager::get().note("Starting compilation");
    ErrorManager::get().warning("Implicit conversion may lose precision");
    ErrorManager::get().error("Undefined identifier 'foo'");
    
    // Test statistics
    assert(ErrorManager::get().getWarningCount() == 1);
    assert(ErrorManager::get().getErrorCount() == 1);
    assert(ErrorManager::get().getTotalCount() == 3);
    assert(ErrorManager::get().hasErrors());
    assert(!ErrorManager::get().hasFatal());
    
    // Test settings
    ErrorManager::get().setSuppressWarnings(true);
    ErrorManager::get().warning("This should be suppressed");
    assert(ErrorManager::get().getWarningCount() == 1); // Should not increase
    
    ErrorManager::get().setSuppressWarnings(false);
    ErrorManager::get().setWarningsAsErrors(true);
    
    // This warning should now be treated as error
    ErrorManager::get().warning("Deprecated function used");
    assert(ErrorManager::get().getWarningCount() == 1); // Still 1
    assert(ErrorManager::get().getErrorCount() == 2);   // Now 2
    
    // Reset settings
    ErrorManager::get().setWarningsAsErrors(false);
    
    // Test max errors
    ErrorManager::get().setMaxErrors(3);
    ErrorManager::get().clear();
    
    // Add enough errors to trigger max
    ErrorManager::get().error("Error 1");
    ErrorManager::get().error("Error 2");
    ErrorManager::get().error("Error 3"); // Should trigger fatal
    
    assert(ErrorManager::get().hasFatal());
    assert(ErrorManager::get().getErrorCount() == 3);
    
    // Clear and reset
    ErrorManager::get().clear();
    ErrorManager::get().setMaxErrors(100);
    
    std::cout << "  ✓ Error manager functions correctly\n";
}

void test_contextual_errors() {
    std::cout << "\n🔹 Testing contextual errors...\n";
    
    // Create a contextual error
    auto ctxError = std::make_unique<ContextualError>(
        ErrorSeverity::ERROR,
        "Type mismatch",
        SourceLocation("test.hc", 25, 10)
    );
    
    // Add context
    ctxError->pushContext("In function 'calculate'");
    ctxError->pushContext("While processing argument 2");
    ctxError->pushContext("During template instantiation");
    
    // Test formatting with context
    std::string formatted = ctxError->format();
    assert(formatted.find("In function 'calculate'") != std::string::npos);
    assert(formatted.find("While processing argument 2") != std::string::npos);
    assert(formatted.find("During template instantiation") != std::string::npos);
    
    // Test builder with context
    auto error = (ErrorManager::get().buildError()
        .at("test.hc", 30, 5)
        << "Cannot resolve overload")
        .inContext("In function 'process_data'")
        .inContext("While evaluating expression")
        .build();
    
    const auto& context = static_cast<ContextualError*>(error.get())->getContext();
    assert(context.size() == 2);
    assert(context[0] == "In function 'process_data'");
    assert(context[1] == "While evaluating expression");
    
    // Pop context
    ctxError->popContext();
    // After popping, should have 2 contexts left
    
    std::cout << "  ✓ Contextual error tracking\n";
}

void test_error_codes() {
    std::cout << "\n🔹 Testing error codes registry...\n";
    
    ErrorCodeRegistry& registry = ErrorCodeRegistry::get();
    
    // Look up existing error codes
    auto lexerError = registry.find("L001");
    assert(lexerError != nullptr);
    assert(lexerError->code == "L001");
    assert(lexerError->description == "Unknown character");
    assert(lexerError->severity == ErrorSeverity::ERROR);
    assert(lexerError->category == "Lexer");
    
    auto typeError = registry.find("T001");
    assert(typeError != nullptr);
    assert(typeError->description.find("Type mismatch") != std::string::npos);
    
    // Create error from code
    auto error = registry.createError("L002", SourceLocation("file.hc", 10, 5), "string started here");
    assert(error->getErrorCode() == "L002");
    assert(error->getMessage().find("Unterminated string literal") != std::string::npos);
    
    // Test with extra message
    error = registry.createError("T002", SourceLocation(), "identifier 'unknownVar'");
    assert(error->getMessage().find("unknownVar") != std::string::npos);
    
    // Test unknown error code
    error = registry.createError("XXXX", SourceLocation(), "custom message");
    assert(error->getMessage().find("Unknown error code") != std::string::npos);
    
    // Test predefined error codes
    using namespace ErrorCodes;
    auto err = registry.createError(UNDECLARED_IDENTIFIER, SourceLocation("test.hc", 5, 2), "myVariable");
    assert(err->getErrorCode() == "T002");
    
    std::cout << "  ✓ Error code registry works\n";
}

void test_internal_error() {
    std::cout << "\n🔹 Testing internal compiler errors...\n";
    
    InternalError ice("Null pointer dereference in symbol table");
    assert(ice.getSeverity() == ErrorSeverity::FATAL);
    assert(ice.isFatal());
    assert(ice.getErrorCode() == "ICE");
    
    // Test formatting
    std::string formatted = ice.format();
    assert(formatted.find("INTERNAL COMPILER ERROR") != std::string::npos);
    assert(formatted.find("Null pointer dereference") != std::string::npos);
    assert(formatted.find("report this bug") != std::string::npos);
    
    // With location
    InternalError iceWithLoc("Assertion failed: ptr != nullptr", 
                           SourceLocation("compiler.cpp", 123, 45),
                           "I002");
    formatted = iceWithLoc.format();
    assert(formatted.find("compiler.cpp:123:45") != std::string::npos);
    assert(iceWithLoc.getErrorCode() == "I002");
    
    std::cout << "  ✓ Internal error handling\n";
}

void test_error_formatting() {
    std::cout << "\n🔹 Testing error formatting...\n";
    
    // Test various formatting scenarios
    
    // Simple error without location
    CompilerError simple(ErrorSeverity::WARNING, "Unused import");
    std::string simpleFmt = simple.format();
    assert(simpleFmt.find("warning: Unused import") != std::string::npos);
    
    // Error with code
    CompilerError coded(ErrorSeverity::ERROR, "Missing return statement", 
                       SourceLocation("func.cpp", 15, 3), "T011");
    std::string codedFmt = coded.format();
    assert(codedFmt.find("T011: error: Missing return statement") != std::string::npos);
    assert(codedFmt.find("func.cpp:15:3") != std::string::npos);
    
    // Error with multiline message
    auto complexError = (ErrorManager::get().buildError()
        .at("parse.cpp", 88, 12)
        << "Expected one of: "
        << "\n  - integer literal"
        << "\n  - floating point literal"
        << "\n  - string literal"
        << "\n  - identifier"
        << "\n But found: ';'")
        .build();
    std::string complexFmt = complexError->format();
    assert(complexFmt.find("Expected one of:") != std::string::npos);
    assert(complexFmt.find("- integer literal") != std::string::npos);
    
    // Test severity string conversion
    assert(severityToString(ErrorSeverity::NOTE) == "note");
    assert(severityToString(ErrorSeverity::WARNING) == "warning");
    assert(severityToString(ErrorSeverity::ERROR) == "error");
    assert(severityToString(ErrorSeverity::FATAL) == "fatal");
    
    // Test dumpAll (just ensure it doesn't crash)
    ErrorManager::get().clear();
    ErrorManager::get().error("Test error 1");
    ErrorManager::get().warning("Test warning 1");
    ErrorManager::get().note("Test note 1");
    
    // Redirect output to stringstream for testing
    std::stringstream output;
    ErrorManager::get().dumpAll(output);
    std::string dump = output.str();
    assert(!dump.empty());
    assert(dump.find("Test error 1") != std::string::npos);
    
    // Test registry dump
    std::stringstream regOutput;
    ErrorCodeRegistry::get().dumpAll(regOutput);
    std::string regDump = regOutput.str();
    assert(regDump.find("Registered Error Codes") != std::string::npos);
    assert(regDump.find("L001") != std::string::npos);
    assert(regDump.find("T001") != std::string::npos);
    
    // Test practical example: type checking error
    {
        auto typeError = std::make_unique<ContextualError>(
            ErrorSeverity::ERROR,
            "Cannot convert 'float' to 'int'",
            SourceLocation("program.hc", 42, 15),
            "T004"
        );
        
        typeError->pushContext("In assignment to variable 'result'");
        typeError->pushContext("In function 'calculate_total'");
        
        std::string typeErrorFmt = typeError->format();
        
        // All parts should be present
        assert(typeErrorFmt.find("T004") != std::string::npos);
        assert(typeErrorFmt.find("Cannot convert") != std::string::npos);
        assert(typeErrorFmt.find("program.hc:42:15") != std::string::npos);
        assert(typeErrorFmt.find("In assignment") != std::string::npos);
        assert(typeErrorFmt.find("In function") != std::string::npos);
        
        // Output example for visual inspection
        std::cout << "\n  Example formatted error:\n";
        std::cout << "  " << typeErrorFmt << "\n";
    }
    
    std::cout << "  ✓ Error formatting and output\n";
}

void test_error_recovery() {
    std::cout << "\n🔹 Testing error recovery mechanisms...\n";
    
    // Test error suppression
    ErrorManager::get().clear();
    ErrorManager::get().setSuppressWarnings(true);
    
    ErrorManager::get().note("This note should appear");
    ErrorManager::get().warning("This warning should be suppressed");
    ErrorManager::get().error("This error should appear");
    
    assert(ErrorManager::get().getNoteCount() == 1);
    assert(ErrorManager::get().getWarningCount() == 0);
    assert(ErrorManager::get().getErrorCount() == 1);
    
    ErrorManager::get().setSuppressWarnings(false);
    
    // Test error limit recovery
    ErrorManager::get().clear();
    ErrorManager::get().setMaxErrors(2);
    
    ErrorManager::get().error("Error 1");
    ErrorManager::get().error("Error 2");
    assert(!ErrorManager::get().hasFatal());
    
    ErrorManager::get().error("Error 3 - should trigger fatal");
    assert(ErrorManager::get().hasFatal());
    assert(ErrorManager::get().getErrorCount() == 2);
    
    // Reset
    ErrorManager::get().clear();
    ErrorManager::get().setMaxErrors(100);
    
    std::cout << "  ✓ Error recovery and suppression\n";
}