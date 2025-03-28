#include <iostream>

using namespace std;

//required function prototypes
bool characteristic(const char numString[], int& c);
bool mantissa(const char numString[], int& numerator, int& denominator);

bool add(int c1, int n1, int d1, int c2, int n2, int d2, char result[], int len);
bool subtract(int c1, int n1, int d1, int c2, int n2, int d2, char result[], int len); 

bool multiply(int c1, int n1, int d1, int c2, int n2, int d2, char result[], int len);
bool divide(int c1, int n1, int d1, int c2, int n2, int d2, char result[], int len);

bool commonDenominator(int& numerator1, int& denominator1, int& numerator2, int& denominator2);
int numberOfDigits(int targetInteger);
bool convertToCString(int characteristic, int numerator, int denominator, char resultCString[], int length);
bool decimalIzeFraction(int& numerator, int& denominator, int digits);
bool removeInsignificantDigits(char numCString [], int length);
void simplifyImproperFraction(int& characteristic, int& numerator, int& denominator);
void handleNegatives(int& characteristic1, int& numerator1, int& characteristic2, int& numerator2);

int stringLength(const char numString[]);
char *cleansed(const char numString[], bool &valid);
bool isDecimalPoint(const char numString[], int &pos, const int size);
int findTrailingZeros(const char numString[], int stringSize);
//--
int main() {
  // this c-string, or array of 8 characters, ends with the null terminating character '\0'
  //['1', '2', '3', '.', '4', '5', '6', '\0']
  const char number[] = "-.456";
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
bool add(int c1, int n1, int d1, int c2, int n2, int d2, char result[], int len)
{
    //check for invalid parameters (denominators should not be zero or negative)
    if(d1 <=0 || d2 <=0) return false;

    //check for overflow
    if (c1 > INT_MAX - c2) return false;

    //give mantissas a common denominator
    n1 = abs(n1);
    n2 = abs(n2);
    commonDenominator(n1, d1, n2, d2);

    //handle negatives
    handleNegatives(c1, n1, c2, n2);

    int resultCharacteristic = c1 + c2;
    int resultNumerator = n1 + n2;
    int resultDenominator = d1;

    //handle improper fractions
    simplifyImproperFraction(resultCharacteristic, resultNumerator, resultDenominator);

    if (convertToCString(resultCharacteristic, resultNumerator, resultDenominator, result, len)) return true;

    return false;
}
//--
bool subtract(int c1, int n1, int d1, int c2, int n2, int d2, char result[], int len)
{
    //multiply the second number by -1, then add the numbers
    if (add(c1, n1, d1, c2 * -1, n2, d2, result, len)) return true;
    return false;
}
//--
bool multiply(int c1, int n1, int d1, int c2, int n2, int d2, char result[], int len)
{
    //check for invalid parameters (denominators should not be zero or negative)
    if(d1 <=0 || d2 <=0) return false;

    //handle negatives
    handleNegatives(c1, n1, c2, n2);

    //convert each number into improper fractions
    int improperNumerator1 = (c1 * d1) + n1;
    int improperNumerator2 = (c2 * d2) + n2;

    //multiply the numerators and denominators
    int resultNumerator = improperNumerator1 * improperNumerator2;
    int resultDenominator = d1 * d2;
    int resultCharacteristic = 0;

    //simplify improper fraction
    simplifyImproperFraction(resultCharacteristic, resultNumerator, resultDenominator);

    if (convertToCString(resultCharacteristic, resultNumerator, resultDenominator, result, len)) return true;

    return false;
}
//--
bool divide(int c1, int n1, int d1, int c2, int n2, int d2, char result[], int len)
{
    //check for invalid parameters (denominators should not be zero or negative, second characteristic and numerator should not both be negative)
    if(d1 <=0 || d2 <=0 || (c2 == 0 && n2 == 0)) return false;

    //handle negatives
    handleNegatives(c1, n1, c2, n2);

    //convert each number into improper fractions
    int improperNumerator1 = (c1 * d1) + n1;
    int improperNumerator2 = (c2 * d2) + n2;

    //multiply the first improper fraction by the reciprocal of the second improper fraction
    int resultNumerator = improperNumerator1 * d2;
    int resultDenominator = d1 * improperNumerator2;
    int resultCharacteristic = 0;

    //handle negative denominator
    if (resultDenominator < 0)
    {
        resultDenominator *= -1;
        resultNumerator *= -1;
    }

    //simplify improper fraction
    simplifyImproperFraction(resultCharacteristic, resultNumerator, resultDenominator);

    if (convertToCString(resultCharacteristic, resultNumerator, resultDenominator, result, len)) return true;

    return false;
}
//--
bool commonDenominator(int& numerator1, int& denominator1, int& numerator2, int& denominator2){
    //helper function for add and subtract functions
    //gives two mantissas a common denominator for easier addition and subtraction

    //check for invalid parameters (denominators should not be zero or negative)
    if(denominator1 <=0 || denominator2 <=0) return false;

    int leastCommonDenominator;
    if(denominator1 > denominator2)
    {
        leastCommonDenominator = denominator1;
        while (leastCommonDenominator % denominator2 != 0)
        {
            leastCommonDenominator += denominator1;
        }
    }
    else
    {
        leastCommonDenominator = denominator2;
        while (leastCommonDenominator % denominator1 != 0)
        {
            leastCommonDenominator += denominator2;
        }
    }
    numerator1 *= (leastCommonDenominator / denominator1);
    denominator1 = leastCommonDenominator;
    numerator2 *= (leastCommonDenominator / denominator2);
    denominator2 = leastCommonDenominator;

    return true;
}
//--
int numberOfDigits(int targetInteger)
{
    //helper function, returns the number of digits in an integer

    int digitCount = 0;

    //the integer 0 has one digit
    //also, if the integer is negative, the (-) symbol will count as a digit
    if(targetInteger <= 0)
    {
        digitCount++;
    }

    while(targetInteger != 0)
    {
        targetInteger = targetInteger / 10;
        digitCount++;
    }
    return digitCount;
}
//--
bool convertToCString(int characteristic, int numerator, int denominator, char resultCString[], int length)
{
    //helper function, converts float into C String

    int characteristicLength = numberOfDigits(characteristic);
    if(characteristicLength > length) return false;
    int numeratorLength;

    //if characteristic can't fit into C String
    if (characteristicLength > length - 1) {
        return false;
    }

    resultCString[length-1] = '\0';

    //if characteristic is negative
    if (characteristic < 0) {
        resultCString[0] = '-';
        characteristic *= -1;
    }

    //if characteristic = 0
    if (characteristic == 0) resultCString[0] = '0';
    if (characteristic == 0 && numerator < 0)
    {
        resultCString[0] = '-';
        resultCString[1] = '0';
        characteristicLength++;
    }

    //if numerator is negative
    if (numerator < 0) numerator *= -1;

    int currentCharInArray = characteristicLength - 1;

    //for every digit in characteristic
    while (characteristic > 0) {
        //add digit
        resultCString[currentCharInArray] = (characteristic % 10) + 48;
        currentCharInArray--;
        characteristic /= 10;
    }


    //if there is no mantissa or mantissa will not fit in array
    if (characteristicLength >= (length - 2) || numerator == 0)
    {
        resultCString[characteristicLength] = '\0';
        return true;
    }

    resultCString[characteristicLength] = '.';

    //converts mantissa into form easier to write as a decimal
    decimalIzeFraction(numerator, denominator, length - characteristicLength - 2);

    numeratorLength = numberOfDigits(numerator);
    currentCharInArray = characteristicLength + numeratorLength;

    while (numerator > 0) {
        //add digit
        resultCString[currentCharInArray] = (numerator % 10) + 48;
        currentCharInArray--;
        numerator /= 10;
    }

    removeInsignificantDigits(resultCString, length);
    return true;
}
//--
bool decimalIzeFraction(int& numerator, int& denominator, int digits)
{
    //helper function, converts fractions to have a denominator of an exponent of 10

    //check for invalid parameters (denominator should not be zero or negative)
    if(denominator <=0)
    {
        return false;
    }

    int newNumerator = numerator;
    int newDenominator = 1;

    for (int i = digits; i > 0; i--) {
        if (newNumerator <= 214748364 && newDenominator <= 214748364)
        {
            newNumerator *= 10;
            newDenominator *= 10;
        }
        else
        {
            break;
        }
    }

    numerator = newNumerator / denominator;

    denominator = newDenominator;

    return true;
}
//--
bool removeInsignificantDigits(char numCString [], int length)
{
    //removes unnecessary zeroes at the end of a mantissa

    for (int i = length - 2; i >= 0; i--)
    {
        if (numCString[i] == '0' || !numCString[i])
        {
            numCString[i] = '\0';
        }
        else if (numCString[i] == '.')
        {
            numCString[i] = '\0';
            break;
        }
        else{
            break;
        }
    }
    return true;
}
//--
void simplifyImproperFraction(int& characteristic, int& numerator, int& denominator){
    while (numerator > 0 && (characteristic < 0 || numerator >= denominator))
    {
        characteristic++;
        numerator -= denominator;
    }
    while (numerator < 0 && (characteristic > 0 || numerator * -1 >= denominator)) {
        characteristic--;
        numerator += denominator;
    }
}
//--
void handleNegatives(int& characteristic1, int& numerator1, int& characteristic2, int& numerator2){
    if (characteristic1 < 0 && numerator1 >= 0) numerator1 *= -1;
    if (characteristic2 < 0 && numerator2 >= 0) numerator2 *= -1;
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
  // Find numerator and denominator
  bool valid = true;
  char *cleansedString = cleansed(numString, valid);
  int stringSize = stringLength(cleansedString);
  int sign = 1;
  bool characteristicIsZero = true;
  if (cleansedString[0] == '-')
    sign = -1;
  int startingPos;
  int endingPos = findTrailingZeros(cleansedString, stringSize);

  if (isDecimalPoint(cleansedString, startingPos, stringSize) && startingPos != stringSize - 1 && valid) {
    for (int i = 1; i < startingPos; i++) {
      if (cleansedString[i] != '0' && sign == -1) {
        characteristicIsZero = false;
        break;
      }
    }
    numerator = 0;
    denominator = 1;
    startingPos++;
    char current = cleansedString[startingPos];
    while (current != '\0' && startingPos <= endingPos) {
      numerator = numerator * 10 + current - 48;
      denominator *= 10;
      startingPos++;
      current = cleansedString[startingPos];
    }
    if (denominator == 1) {
      denominator = 10;
    }
  } else {
    numerator = 0;
    denominator = 10;
  }
  if (characteristicIsZero)
    numerator *= sign;

  delete cleansedString;
  return valid;
}
//--
int findTrailingZeros(const char numString[], int stringSize) {
  int position = stringSize - 1;
  char current = numString[position];
  while (current == '0') {
    position--;
    current = numString[position];
  }

  return position;
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
  // Remove unneeded characters and determine if valid
  int periodCount = 0;
  bool plusExists = false;
  bool numbersAllowed = true;
  int stringSize = stringLength(numString);
  char *clean = new char[stringSize + 1];
  int index = 0;

  for (int i = 0; i < stringSize; i++) {
    if ((numString[i] == '-' && index == 0) || numString[i] == '.' || (numString[i] >= 48 && numString[i] <= 57)) {
      if (!numbersAllowed) {
        valid = false; // If there are spaces in between numbers, it's not valid
        break;
      }
      clean[index] = numString[i];
      index++;
      if (numString[i] == '.') {
        periodCount++;
      }
    } else if (numString[i] == '+' && index > 0) {
      valid = false; // Signs can't happen past the first character
      break;
    } else if (numString[i] == '+') {
      plusExists = true;
      continue;
    } else if (numString[i] == ' ' && i > 0 && numString[i - 1] >= 48 && numString[i - 1] <= 57) {
      numbersAllowed = false; // Space happened after a number, thus no more numbers are allowed
      continue;
    } else if (numString[i] == ' ') {
      continue;
    } else {
      valid = false;
      break;
    }
  }

  // If there are more than one period, ending character isn't a number,
  // multiple signs, or character after '-' isn't a number, it's not valid
  if (periodCount > 1) {
    valid = false;
  } else if (clean[index - 1] < 48 || clean[index - 1] > 57) {
    valid = false;
  } else if ((clean[0] == '-' && plusExists)) {
    valid = false;
  } else if (clean[0] == '-' && (clean[1] < 48 && clean[1] > 57)) {
    valid = false;
  }
  clean[index] = '\0';
  return clean;
}
//--
bool isDecimalPoint(const char numString[], int &pos, const int size) {
  for (int i = 0; i < size; i++) {
    if (numString[i] == '.') {
      pos = i;
      return true;
    }
  }

  return false;
}