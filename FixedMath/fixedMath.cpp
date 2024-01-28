#include "fixedMath.h"

//int fixedAdd(int num1, int num2){
//	return num1 + num2;
//}
//
//int fixedSubtract(int num1, int num2){
//	return num1 - num2;
//}

int fixedMultiply(int num1, int num2) {
	return num1 * num2 >> DECIMAL_DIGITS;
}

int fixedDivide(int num1, int num2) {
	return num1 / num2 << DECIMAL_DIGITS;
}
