
#include "TestUtility.hpp"

#include <cstdlib>
#include <csignal>
#include <cstring>
#include <climits>

using namespace TestUtility;

RandomNumberGenerator::RandomNumberGenerator() : engine{std::random_device{}()} {}

int RandomNumberGenerator::operator()(const int min, const int max) {
  return std::uniform_int_distribution{min, max}(engine);
}

PotentialError::PotentialError(const char *str) : error{str}, null{false} {}

PotentialError::PotentialError(std::string &&str) : error{std::move(str)}, null{false} {}

PotentialError::PotentialError(const std::string &str) : error{str}, null{false} {}

void PotentialError::set(const char *str) {
  error = str;
}

void PotentialError::set(const std::string &str) {
  error = str;
}

const std::string &PotentialError::str() const {
  return error;
}

const char *PotentialError::c_str() const {
  return error.c_str();
}

const char *PotentialError::getTestPtr() const {
  return test.c_str();
}

const std::string &PotentialError::getTestStr() const {
  return test;
}

PotentialError &PotentialError::operator=(const char *str) {
  error = str;
  return *this;
}

PotentialError &PotentialError::operator=(std::string &&str) {
  error = std::move(str);
  return *this;
}

PotentialError &PotentialError::operator=(const std::string &str) {
  error = str;
  return *this;
};

void PotentialError::newTest(const std::string &str) {
  test = str;
  null = true;
  error.clear();
}

bool PotentialError::isNull() const {
  return null;
}

Result::Result(const bool status) : status{status} {}

Result::Result(std::string &&message) : message{std::move(message)} {}

Result::Result(const std::string &message) : message{message} {}

Result::Result(const char *message) : message{message} {}

bool Result::operator!() const {
  return !status;
}

Result::operator bool() const {
  return status;
}

constexpr size_t constexprStrlen(const char *str) {
  size_t length{};
  
  while (str[length]) {
    ++length;
  }
  
  return length;
}

void writeCharacter(const int fileHandle, const char c) {
  write(fileHandle, &c, 1);
}

template<bool newline = false>
void writeNumber(const int fileHandle, int num) {
  if (num > 0) {
    if (num == INT_MAX) {
      constexpr char str[] = "2147483647";
      write(STDERR_FILENO, str, constexprStrlen(str));
      return;
    }
    
    int bufferSize{};
    
    for (int numCopy = num; numCopy;) {
      ++bufferSize;
      numCopy /= 10;
    }
    
    char buffer[bufferSize];
    buffer[bufferSize - 1] = '\0';
    
    for (int i = bufferSize - 1; i >= 0; --i) {
      buffer[i] = static_cast<char>((num % 10) + '0');
      num /= 10;
    }
    
    write(fileHandle, buffer, bufferSize + 1);
  } else if (num == 0) {
    write(fileHandle, "0", 1);
    return;
  } else {
    if (num == INT_MIN) {
      constexpr char str[] = "-2147483647";
      write(fileHandle, str, constexprStrlen(str));
      return;
    }
    
    int bufferSize{1};
    
    for (int numCopy = num; numCopy;) {
      ++bufferSize;
      numCopy /= 10;
    }
    
    char buffer[bufferSize];
    buffer[bufferSize - 1] = '\0';
    buffer[0] = '-';
    
    for (int i = bufferSize - 1; i > 0; --i) {
      buffer[i] = static_cast<char>((num % 10) + '0');
      num /= 10;
    }
    
    write(fileHandle, buffer, bufferSize + 1);
  }
  
  if constexpr (newline) {
    write(fileHandle, "\n", 1);
  }
}

auto writePotentialError(const int fh, const PotentialError &pe) {
  if (pe.isNull()) {
    const char *str = "null";
    return write(fh, str, constexprStrlen(str));
  }
  
  const std::string &str = pe.str();
  return write(fh, str.c_str(), str.length() + 1); // account for null terminator
}

void callFunction(const char *functionName, Result(*functionPtr)()) {
  std::signal(SIGSEGV, [](int) -> void {
    
    constexpr char testLabel[] = "Test ";
    constexpr char failureLabel[] = "failed: ";
    const char *testName = potentialError.getTestPtr();
    write(STDERR_FILENO, testLabel, constexprStrlen(testLabel));
    writeNumber(STDERR_FILENO, testNumber);
    write(STDERR_FILENO, failureLabel, constexprStrlen(failureLabel));
    write(STDERR_FILENO, testName, std::strlen(testName));
    writeCharacter(STDERR_FILENO, '\n');

    constexpr char potentialErrorLabel[] = "Potential error: ";
    write(STDERR_FILENO, potentialErrorLabel, constexprStrlen(potentialErrorLabel));
    writePotentialError(STDERR_FILENO, potentialError);
    writeCharacter(STDERR_FILENO, '\n');
    
    constexpr char signalLabel[] = "Signal 11: Segmentation Fault\n";
    write(STDERR_FILENO, signalLabel, constexprStrlen(signalLabel));
    exit(-1);
  });
  
  try {
    potentialError.newTest(functionName);
    if (const Result result = functionPtr(); !result) {
      throw std::runtime_error(
      ((std::string{"Function call failed:\n"} += functionName) += '\n') += result.message);
    }
  } catch (const std::runtime_error &) {
    throw;
  } catch (const std::exception &e) {
    if (potentialError.isNull())
      std::cerr << "Call to " << functionName << "failed\nUnknown error" << std::endl;
    else
      std::cerr << "Call to " << functionName << "failed\nPotential error: " << potentialError.str() << '\n' << e.what()
                << std::endl;
    exit(-1);
  }
  
}