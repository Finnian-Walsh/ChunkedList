#include "TestUtility.hpp"

#include <cstdlib>

using namespace TestUtility;

RandomNumberGenerator::RandomNumberGenerator() : engine{std::random_device{}()} {
}

int RandomNumberGenerator::operator()(const int min, const int max) {
  return std::uniform_int_distribution{min, max}(engine);
}

TestData::TestData(const char *str) : source{str}, nullSource{false} {
}

TestData::TestData(std::string &&str) : source{std::move(str)}, nullSource{false} {
}


void TestData::setSource(const char *str) {
  source = str;
  nullSource = false;
}

void TestData::setSource(std::string &&str) {
  source = std::move(str);
}

void TestData::setTask(std::string &&str) {
  task = std::move(str);
  nullTask = false;
}

void TestData::setTask(const char *str) {
  task = str;
  nullTask = false;
}

const std::string &TestData::getTest() const {
  return test;
}

const std::string &TestData::getSource() const {
  return source;
}

const std::string &TestData::getTask() const {
  return task;
}

void TestData::newTest(const std::string &str) {
  test = str;
  nullSource = true;
  nullTask = true;
}

bool TestData::sourceIsNull() const {
  return nullSource;
}

bool TestData::taskIsNull() const {
  return nullTask;
}

void TestUtility::callFunction(const char *functionName, void (*functionPtr)()) {
  std::cout << "Test " << testNumber << ": " << functionName << '\n';

  try {
    testData.newTest(functionName);
    functionPtr();
  } catch (const std::exception &e) {
    throw std::runtime_error(
      std::string{"Call to "}.operator+=(functionName).operator+=(" failed\nSource: ").operator+=(
        testData.sourceIsNull() ? "NULL" : testData.getSource()).
      operator+=("\nTask: ").operator+=(testData.taskIsNull() ? "NULL" : testData.getTask()).operator+=("\nError: ").
      operator+=(e.what()));
  }

  std::cout << "Test " << testNumber << " successful\n" << std::endl;
  ++testNumber;
}

void TestUtility::performTask(const char *taskName, const int logLevel) {
  testData.setTask(taskName);
  if (logLevel <= LogLevel) {
    std::cout << taskName << '\n';
  }
}
