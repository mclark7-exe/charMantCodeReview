#include <iostream>
using namespace std;

// required function prototypes
bool characteristic(const char numString[], int &c);
bool mantissa(const char numString[], int &numerator, int &denominator);

int stringLength(const char numString[]);
char *cleansed(const char numString[], bool &valid);
bool isPeriod(const char numString[], int &pos, const int size);

bool add(int c1, int n1, int d1, int c2, int n2, int d2, char result[], int len);
bool subtract(int c1, int n1, int d1, int c2, int n2, int d2, char result[], int len);

bool multiply(int c1, int n1, int d1, int c2, int n2, int d2, char result[], int len);
bool divide(int c1, int n1, int d1, int c2, int n2, int d2, char result[], int len);

int main() {
  // this c-string, or array of 8 characters, ends with the null terminating character '\0'
  //['1', '2', '3', '.', '4', '5', '6', '\0']
  const char number[] = "123.456";
  int c, n, d;

  // if both conversions from c-string to integers can take place
  if (characteristic(number, c) && mantissa(number, n, d)) {
    // do some math with c, n, and d
    cout << "c: " << c << endl;
    cout << "n: " << n << endl;
    cout << "d: " << d << endl;
  } else // at least one of the conversions failed
  {
    // handle the error on input
    cout << "Error on input" << endl;
  }

  // room for 9 characters plus the null terminating character
  char answer[10];
  int c1, n1, d1;
  int c2, n2, d2;

  // initialize the values
  c1 = 1;
  n1 = 1;
  d1 = 2;

  c2 = 2;
  n2 = 2;
  d2 = 3;

  // if the c-string can hold at least the characteristic
  if (add(c1, n1, d1, c2, n2, d2, answer, 10)) {
    // display string with answer 4.1666666 (cout stops printing at the null terminating character)
    cout << "Answer: " << answer << endl;
  } else {
    // display error message
    cout << "Error on add" << endl;
  }

  if (divide(c1, n1, d1, c2, n2, d2, answer, 10)) {
    // display string with answer
    cout << "Answer: " << answer << endl;
  } else {
    // display error message
    cout << "Error on divide" << endl;
  }

  return 0;
}
//--
bool characteristic(const char numString[], int &c) {
  bool valid = true;
  int characteristic = 0;
  int sign = 1;
  int index = 0;

  char *cleansedString = cleansed(numString, valid);
  if (cleansedString[index] == '-') {
    sign *= -1;
    index++;
  }

  char current = cleansedString[index];
  if (current >= 48 && current <= 57 && valid) {
    while (current != '\0' && current != '.') {
      characteristic = characteristic * 10 + current - 48;
      index++;
      current = cleansedString[index];
    }
  }
  delete cleansedString;
  c = characteristic * sign;
  return valid;
}
//--
bool mantissa(const char numString[], int &numerator, int &denominator) {
  // hard coded return value to make the main() work
  char *cleansedString = cleansed(numString);
  int stringSize = stringLength(numString);
  int periodPos;
  if (isPeriod(cleansedString, periodPos, stringSize)) {
  }

  numerator = 456;
  denominator = 1000;
  delete cleansedString;
  return true;
}
//--
int stringLength(const char numString[]) {
  // Find and return length of char[]
  int count = 0;
  char iterator = numString[count];
  while (iterator != '\0') {
    count++;
    iterator = numString[count];
  }
  return count;
}
//--
char *cleansed(const char numString[], bool &valid) {
  // Remove unneeded characters
  int periodCount = 0;
  int stringSize = stringLength(numString);
  char *clean = new char[stringSize + 1];
  int index = 0;
  for (int i = 0; i < stringSize; i++) {
    if ((numString[i] == '-' && index == 0) || numString[i] == '.' || (numString[i] >= 48 && numString[i] <= 57)) {
      clean[index] = numString[i];
      index++;
      if (numString[i] == '.') {
        periodCount++;
      }
    } else if ((numString[i] == '+' && index == 0) || numString[i] == ' ') {
      continue;
    } else {
      valid = false;
    }
  }
  if (periodCount > 1) {
    valid = false;
  }

  clean[index] = '\0';
  return clean;
}
//--
bool isPeriod(const char numString[], int &pos, const int size) {
  bool containsPeriod = false;
  for (int i = 0; i < size; i++) {
    if (numString[i] == '.') {
      containsPeriod = true;
      pos = i;
      break;
    }
  }

  return containsPeriod;
}
//--
bool add(int c1, int n1, int d1, int c2, int n2, int d2, char result[], int len) {
  // you will have to come up with an algorithm to add the two numbers
  // hard coded return value to make the main() work
  result[0] = '4';
  result[1] = '.';
  result[2] = '1';
  result[3] = '6';
  result[4] = '6';
  result[5] = '6';
  result[6] = '6';
  result[7] = '6';
  result[8] = '6';
  result[9] = '\0';

  return true;
}
//--
bool subtract(int c1, int n1, int d1, int c2, int n2, int d2, char result[], int len) {
  // hard coded return value to make the code compile
  // you will have to come up with an algorithm to subtract the two numbers
  return true;
}
//--
bool multiply(int c1, int n1, int d1, int c2, int n2, int d2, char result[], int len) {
  // hard coded return value to make the code compile
  // you will have to come up with an algorithm to multiply the two numbers
  return true;
}
//--
bool divide(int c1, int n1, int d1, int c2, int n2, int d2, char result[], int len) {
  // you will have to come up with an algorithm to divide the two numbers
  // hard coded return value to make the main() work
  result[0] = '0';
  result[1] = '.';
  result[2] = '5';
  result[3] = '6';
  result[4] = '2';
  result[5] = '5';
  result[6] = '\0';

  return true;
}
