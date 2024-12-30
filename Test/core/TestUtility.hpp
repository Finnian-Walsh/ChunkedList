#pragma once

#include <iostream>
#include <unistd.h>
#include <random>

#define BEGIN std::cout << "Starting tests..." << std::endl;
#define CALL_TEST(functionName, functionPtr) std::cout << "Test " << testNumber << ": " << functionName << '\n'; callFunction(functionName, functionPtr); std::cout << "Test " << testNumber << " successful" << std::endl; ++testNumber;
#define SUCCESS std::cout << "All " << testNumber - 1 << " tests have been ran."; return EXIT_SUCCESS;
#define RETURN_IF(condition, str) if (condition) return Result{str};

namespace TestUtility {
#ifdef LOG_LEVEL
  constexpr inline int LogLevel = LOG_LEVEL;
#else
  constexpr inline int LogLevel = 0;
#endif


  inline int testNumber{1};

  class RandomNumberGenerator {
    std::mt19937 engine;

    public:
      RandomNumberGenerator();

      int operator()(int min, int max);
  };

  inline class PotentialError {
    std::string test{};
    std::string error{};
    bool null{true};

    public:
      PotentialError() = default;

      explicit PotentialError(const char *str);

      explicit PotentialError(std::string &&str);

      explicit PotentialError(const std::string &str);

      void set(const char *str);

      void set(const std::string &str);

      void set(std::string &&str);

      const std::string &str() const;

      const char *c_str() const;

      const std::string &getTest() const;

      PotentialError &operator=(const char *str);

      PotentialError &operator=(const std::string &str);

      PotentialError &operator=(std::string &&str);

      void newTest(const std::string &str);

      bool isNull() const;
  } potentialError;

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

  class ResultPointer {
    Result *pointer{nullptr};

    public:
      ResultPointer() = default;

      explicit ResultPointer(Result *result);

      explicit ResultPointer(const Result &result);

      explicit ResultPointer(Result &&result);

      ~ResultPointer();

      Result &operator*();

      const Result &operator*() const;

      Result *operator->();

      const Result *operator->() const;

      ResultPointer &operator=(Result *result);

      ResultPointer &operator=(const Result &result);

      ResultPointer &operator=(Result &&result);

  };

  void callFunction(const char *functionName, Result (*functionPtr)());

  void performOperation(const char *operationName, int logLevel = 1);
}
