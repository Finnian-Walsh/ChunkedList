#include "TestUtility.hpp"

#include <cstdlib>
#include <csignal>
#include <cstring>
#include <climits>

using namespace TestUtility;

RandomNumberGenerator::RandomNumberGenerator() : engine{std::random_device{}()} {
}

int RandomNumberGenerator::operator()(const int min, const int max) {
  return std::uniform_int_distribution{min, max}(engine);
}

PotentialError::PotentialError(const char *str) : error{str}, null{false} {
}

PotentialError::PotentialError(std::string &&str) : error{std::move(str)}, null{false} {
}

PotentialError::PotentialError(const std::string &str) : error{str}, null{false} {
}

void PotentialError::set(const char *str) {
  error = str;
}

const std::string &PotentialError::str() const {
  return error;
}

const char *PotentialError::c_str() const {
  return error.c_str();
}

const std::string &PotentialError::getTest() const {
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

Result::Result(const bool status) : status{status} {
}

Result::Result(std::string &&message) : message{std::move(message)} {
}

Result::Result(const std::string &message) : message{message} {
}

Result::Result(const char *message) : message{message} {
}

bool Result::operator!() const {
  return !status;
}

Result::operator bool() const {
  return status;
}

ResultPointer::ResultPointer(Result *result) : pointer(result) {
}

ResultPointer::ResultPointer(Result &&result) : pointer(new Result{std::move(result)}) {
}

ResultPointer::~ResultPointer() {
  if (pointer) {
    delete pointer;
  }
}

Result &ResultPointer::operator*() {
  return *pointer;
}

const Result &ResultPointer::operator*() const {
  return *pointer;
}

Result *ResultPointer::operator->() {
  return pointer;
}

const Result *ResultPointer::operator->() const {
  return pointer;
}

ResultPointer &ResultPointer::operator=(Result *result) {
  if (pointer) delete pointer;

  pointer = result;

  return *this;
}

ResultPointer &ResultPointer::operator=(const Result &result) {
  if (pointer) delete pointer;

  pointer = new Result{result};

  return *this;
}

ResultPointer &ResultPointer::operator=(Result &&result) {
  if (pointer) delete pointer;

  pointer = new Result{std::move(result)};

  return *this;
}

void TestUtility::callFunction(const char *functionName, Result (*functionPtr)()) {
  ResultPointer resultPtr{};

  try {
    potentialError.newTest(functionName);
    resultPtr = functionPtr();
  } catch (const std::exception &e) {
    throw std::runtime_error(
      std::string{"Call to "}.operator+=(functionName).operator+=("failed\nError: ").operator+=(e.what()).operator
      +=("\nPotential error: ").operator+=(potentialError.isNull() ? "unknown" : potentialError.str()).
      operator+=('\n'));
  }

  if (!*resultPtr) {
    throw std::runtime_error(
      std::string{"Function call failed:\n"}.operator+=(functionName).operator+=('\n').operator+=(resultPtr->message));
  }
}

void TestUtility::performOperation(const char *operationName, const int logLevel) {
  potentialError = operationName;
  if (logLevel <= LogLevel) {
    std::cout << operationName << '\n';
  }
}
