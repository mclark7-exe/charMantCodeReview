
#include <iostream>
#include <string.h>
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

int main()
{
    //this c-string, or array of 8 characters, ends with the null terminating character '\0'
    //['1', '2', '3', '.', '4', '5', '6', '\0']
    const char number[] = "123.456"; 
    int c, n, d;

    //if both conversions from c-string to integers can take place
    if(characteristic(number, c) && mantissa(number, n, d))
    {
        //do some math with c, n, and d
        cout<<"c: "<<c<<endl;
        cout<<"n: "<<n<<endl;
        cout<<"d: "<<d<<endl;
    }
    else //at least one of the conversions failed
    {
        //handle the error on input
        cout<<"Error on input"<<endl;
    }

    //room for 9 characters plus the null terminating character
    char answer[10];
    int c1, n1, d1;
    int c2, n2, d2;

    //initialize the values
    c1 = 1;
    n1 = 1;
    d1 = 2;

    c2 = 2;
    n2 = 2;
    d2 = 3; 

    //if the c-string can hold at least the characteristic
    if(add(c1, n1, d1, c2, n2, d2, answer, 10))
    {
        //display string with answer 4.1666666 (cout stops printing at the null terminating character)
        cout<<"Answer: "<<answer<<endl;
    }
    else
    {
        //display error message
        cout<<"Error on add"<<endl;
    }

    if(divide(c1, n1, d1, c2, n2, d2, answer, 10))
    {
        //display string with answer
        cout<<"Answer: "<<answer<<endl;
    }
    else
    {
        //display error message
        cout<<"Error on divide"<<endl;
    }

    if(commonDenominator(n1, d1, n2, d2))
    {
        //display new n1, d1, n2, d2
        cout<<"n1: "<<n1<<endl;
        cout<<"d1: "<<d1<<endl;
        cout<<"n2: "<<n2<<endl;
        cout<<"d2: "<<d2<<endl;

    }
    else
    {
        //display error message
        cout<<"Error on finding common denominator"<<endl;
    }

    if(decimalIzeFraction(n2, d2, 6))
    {
        //display new n1, d1
        cout<<"n2: "<<n2<<endl;
        cout<<"d2: "<<d2<<endl;

    }
    else
    {
        //display error message
        cout<<"Error on decimalizing fraction"<<endl;
    }

    return 0;
}
//--
bool characteristic(const char numString[], int& c)
{
    //hard coded return value to make the main() work
    c = 123;
    return true;
}
//--
bool mantissa(const char numString[], int& numerator, int& denominator)
{
    //hard coded return value to make the main() work
    numerator = 456;
    denominator = 1000;
    return true;
}
//--
bool add(int c1, int n1, int d1, int c2, int n2, int d2, char result[], int len)
{
    //check for invalid parameters (denominators should not be zero or negative)
    if(d1 <=0 || d2 <=0)
    {
        return false;
    }

    //give mantissas a common denominator
    commonDenominator(n1, d1, n2, d2);

    int resultCharacteristic = c1 + c2;
    int resultNumerator = n1 + n2;
    int resultDenominator = d1;

    //handle improper fractions
    if (resultNumerator >= resultDenominator)
    {
        resultCharacteristic++;
        resultNumerator -= resultDenominator;
    }

    convertToCString(resultCharacteristic, resultNumerator, resultDenominator, result, len);

    return true;
}
//--
bool subtract(int c1, int n1, int d1, int c2, int n2, int d2, char result[], int len)
{
    //check for invalid parameters (denominators should not be zero or negative)
    if(d1 <=0 || d2 <=0)
    {
        return false;
    }

    //give mantissas a common denominator
    commonDenominator(n1, d1, n2, d2);

    int resultCharacteristic = c1 - c2;
    int resultNumerator = n1 - n2;
    int resultDenominator = d1;


    //handle negative numerator
    if (resultNumerator <0 && resultCharacteristic >= 0)
    {

        resultCharacteristic--;
        resultNumerator += resultDenominator;
    }

    convertToCString(resultCharacteristic, resultNumerator, resultDenominator, result, len);

    return true;
}
//--
bool multiply(int c1, int n1, int d1, int c2, int n2, int d2, char result[], int len)
{
    //check for invalid parameters (denominators should not be zero or negative)
    if(d1 <=0 || d2 <=0)
    {
        return false;
    }

    int improperNumerator1 = (c1 * d1) + n1;
    int improperNumerator2 = (c2 * d2) + n2;
    int resultNumerator = improperNumerator1 * improperNumerator2;
    int resultDenominator = d1 * d2;
    int resultCharacteristic = 0;

    while (resultNumerator >= resultDenominator) {
        resultCharacteristic++;
        resultNumerator -= resultDenominator;
    }

    convertToCString(resultCharacteristic, resultNumerator, resultDenominator, result, len);

    return true;
}
//--
bool divide(int c1, int n1, int d1, int c2, int n2, int d2, char result[], int len)
{
    //check for invalid parameters (denominators should not be zero or negative)
    if(d1 <=0 || d2 <=0)
    {
        return false;
    }

    int improperNumerator1 = (c1 * d1) + n1;
    int improperNumerator2 = (c2 * d2) + n2;
    int resultNumerator = improperNumerator1 * d2;
    int resultDenominator = d1 * improperNumerator2;
    int resultCharacteristic = 0;

    while (resultNumerator >= resultDenominator) {
        resultCharacteristic++;
        resultNumerator -= resultDenominator;
    }

    convertToCString(resultCharacteristic, resultNumerator, resultDenominator, result, len);

    return true;
}
//--
bool commonDenominator(int& numerator1, int& denominator1, int& numerator2, int& denominator2){
    //helper function for add and subtract functions
    //gives two mantissas a common denominator for easier addition and subtraction

    //check for invalid parameters (denominators should not be zero or negative)
    if(denominator1 <=0 || denominator2 <=0)
    {
        return false;
    }

    int lcm;
    if(denominator1 > denominator2)
    {
        lcm = denominator1;
        while (lcm % denominator2 != 0)
        {
            lcm += denominator1;
        }
    }
    else
    {
        lcm = denominator2;
        while (lcm % denominator1 != 0)
        {
            lcm += denominator2;
        }
    }
    numerator1 *= (lcm / denominator1);
    denominator1 = lcm;
    numerator2 *= (lcm / denominator2);
    denominator2 = lcm;

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
    int currentCharInArray = 0;

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
    if (characteristic == 0) {
        resultCString[0] = '0';
    }

    currentCharInArray = characteristicLength - 1;

    //for every digit in characteristic
    while (characteristic > 0) {
        //add digit
        resultCString[currentCharInArray] = (characteristic % 10) + 48;
        currentCharInArray--;
        characteristic /= 10;
    }

    currentCharInArray = characteristicLength;

    //if there is no mantissa or mantissa will not fit in array
    if (currentCharInArray >= (length - 2) || numerator == 0)
    {
        resultCString[characteristicLength] = '\0';
        return true;
    }

    resultCString[currentCharInArray] = '.';
    currentCharInArray = length - 2;

    //converts mantissa into form easier to write as a decimal
    decimalIzeFraction(numerator, denominator, length - characteristicLength - 2);

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
        newNumerator *= 10;
        newDenominator *= 10;
    }

    numerator = newNumerator / denominator;

    denominator = newDenominator;

    return true;
}

bool removeInsignificantDigits(char numCString [], int length)
{
    for (int i = length - 2; i >= 0; i--)
    {
        if (numCString[i] == '0')
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
