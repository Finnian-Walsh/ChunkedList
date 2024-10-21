
#include "TestUtility.hpp"

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

void PotentialError::setNull(bool b) {
  null = b;
  if (b) {
    error = "NULL";
  }
}

bool PotentialError::isNull() const {
  return null;
}

Result::Result(bool status) : status{status} {}

Result::Result(std::string &&message) : message{std::move(message)} {}

Result::Result(const std::string &message) : message{message} {}

Result::Result(const char *message) : message{message} {}

bool Result::operator!() const {
  return status;
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

template<bool newline = false>
void writeNumber(int num) {
  if (num > 0) {
    if (num == INT_MAX) {
      write(STDERR_FILENO, "2147483647", 10);
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
    
    write(STDERR_FILENO, buffer, bufferSize + 1);
  } else if (num == 0) {
    write(STDERR_FILENO, "0", 1);
    return;
  } else {
    if (num == INT_MIN) {
      write(STDERR_FILENO, "2147483647", 10);
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
    
    write(STDERR_FILENO, buffer, bufferSize + 1);
  }
  
  if constexpr (newline) {
    write(STDERR_FILENO, "\n", 1);
  }
}

auto writePotentialError(int fh, const PotentialError &pe) -> decltype(write(fh, pe.c_str(), size_t{})) {
  const std::string &str = pe.str();
  return write(fh, str.c_str(), str.length() + 1); // account for null terminator
}

void callFunction(const char *functionName, Result(*functionPtr)()) {
  std::signal(SIGSEGV, [](int signalNumber) -> void {
    
    constexpr char potentialErrorLabel[] = "Potential error: ";
    write(STDERR_FILENO, potentialErrorLabel, constexprStrlen(potentialErrorLabel));
    writePotentialError(STDERR_FILENO, potentialError);
    write(STDERR_FILENO, "\n", 1);
    
    constexpr char signalLabel[] = "Signal ";
    write(STDERR_FILENO, signalLabel, constexprStrlen(signalLabel));
    writeNumber(signalNumber);
    constexpr char segmentationFaultLabel[] = ": Segmentation fault\n";
    write(STDERR_FILENO, segmentationFaultLabel, constexprStrlen(segmentationFaultLabel));
    
    exit(-1);
  });
  
  try {
    potentialError.setNull(true);
    Result result = functionPtr();
    if (!result) {
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