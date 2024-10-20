#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <csignal>
#include <cstring>
#include <memory>
#include <climits>

#define BEGIN int testNumber{1}; std::cout << "Starting tests..." << std::endl
#define CALL_TEST(functionName, functionPtr) std::cout << "Test " << testNumber << ":\n"; callFunction(functionName, functionPtr); std::cout << "Test " << testNumber << " successful" << std::endl; ++testNumber;
#define SUCCESS std::cout << "All " << testNumber - 1 << " tests have been ran."; return EXIT_SUCCESS
#define RETURN_IF(condition, str) if (condition) return Result{str};

#define CHUNKED_LIST_DEBUGGING

class PotentialError {
  private:
    std::string error{};
    bool null{true};
  public:
    
    PotentialError() = default;
    
    explicit PotentialError(const char *str);
    
    explicit PotentialError(std::string &&str);
    
    explicit PotentialError(const std::string &str);
    
    void set(const char *str);
    
    void set(const std::string &str);
    
    [[nodiscard]] const std::string &get() const;
    
    PotentialError &operator=(const char *str);
    
    PotentialError &operator=(std::string &&str);
    
    PotentialError &operator=(const std::string &str);
    
    void setNull(bool b);
    
    [[nodiscard]] bool isNull() const;
  
};

static PotentialError potentialError;

class Result {
  public:
    const std::string message{};
    const bool status{false};
    
    explicit Result(bool status);
    
    explicit Result(std::string &&message);
    
    explicit Result(const std::string &message);
    
    explicit Result(const char *message);
    
    bool operator!() const;
    
    operator bool() const;
};

void callFunction(const char *functionName, Result(*functionPtr)());