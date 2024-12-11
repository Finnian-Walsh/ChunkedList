#pragma once

#include <iostream>
#include <unistd.h>
#include <random>

#define BEGIN std::cout << "Starting tests..." << std::endl;
#define CALL_TEST(functionName, functionPtr) std::cout << "Test " << testNumber << ": " << functionName << '\n'; callFunction(functionName, functionPtr); std::cout << "Test " << testNumber << " successful" << std::endl; ++testNumber;
#define SUCCESS std::cout << "All " << testNumber - 1 << " tests have been ran."; return EXIT_SUCCESS;
#define RETURN_IF(condition, str) if (condition) return Result{str};

namespace TestUtility {
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

      const std::string &str() const;

      const char *c_str() const;

      const char *getTestPtr() const;

      const std::string &getTestStr() const;

      PotentialError &operator=(const char *str);

      PotentialError &operator=(std::string &&str);

      PotentialError &operator=(const std::string &str);

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

  void callFunction(const char *functionName, Result (*functionPtr)());
}
