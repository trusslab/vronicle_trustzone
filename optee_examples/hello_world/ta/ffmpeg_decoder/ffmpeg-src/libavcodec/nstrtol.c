#include "nstrtol.h"

long strtol (const char *nPtr, char **endPtr, int base)
{
  // errno = 0; /* if no error will occur, errno will not change */
  long number = 0; // WHEN FINISHED, DELETE '= 0' !!!!!!!!!!
  const char *actualPosition;
  enum signs {NEGATIVE, POSITIVE}; /* available signs */
  int sign, cutlim, currentdigit;
  unsigned long cutoff;
  bool correctconversion = true;

  if (endPtr == NULL){ /* !!!MANDATORY PART!!! if user of the function gives us the endPtr pointer with a NULL address */
    free(endPtr);
    endPtr = malloc(sizeof(char **));
    *endPtr = (char *) nPtr; /* there could be anything instead of '(char *) nPtr' like '0' */
  }

  actualPosition = nPtr;

  /* Invalid argument */
  if (!((base >= 2 && base <= 36) || base == 0)){
    // errno = EINVAL;
    return 0;
  }

  /* omits every blank character like space, tabulation, newline character etc. */
  while (isspace(*actualPosition))
    actualPosition++;

  /* sign checker */
  if (*actualPosition == '+'){
    sign = POSITIVE;
    actualPosition++;
  }else if (*actualPosition == '-'){
    sign = NEGATIVE;
    actualPosition++;
  }else
    sign = POSITIVE; /* if first non-blank character is not '+' nor '-', then we assume the number as a positive value, even though string may not be a number */

  if (!isalnum(*actualPosition))
    return 0;

  /* detection what is the base */
  if (base == 0){
    const char *positionBeforeBaseDetection;
    positionBeforeBaseDetection = actualPosition;

    if (!isdigit(*actualPosition))
      return 0; /* string is not a number */
    else if (*actualPosition == '0')
    {
      actualPosition++;
      if (*actualPosition == 'x'){
        base = 16;
        actualPosition++;
      }
      if (*actualPosition >= '0' && *actualPosition <= '7' && base == 0){
        while (*actualPosition >= '0' && *actualPosition <= '7')
          actualPosition++;
        if (*actualPosition == '\0'){
          base = 8;
          actualPosition = positionBeforeBaseDetection;
        }
        else if ((*actualPosition >= '0' && *actualPosition <= '9') || (*actualPosition >= 'A' && *actualPosition <= 'F')){
          base = 16;
          actualPosition = positionBeforeBaseDetection;
        }
      }
    }else if (*actualPosition >= '1' && *actualPosition <= '9')
      base = 10;

    nPtr = actualPosition;
  }




  /////TO WORK PROPERLY, IT NEEDS TO CHECK IF EACH INPUT IS PROPER TO THE BASE
  if (base == 10){
    while (isdigit(*actualPosition) && *actualPosition != '\0')
      actualPosition++;
    *endPtr = (char *) actualPosition;
  }else if (base == 8){
    while (*actualPosition >= '0' && *actualPosition <= '7')
      actualPosition++;
    *endPtr = (char *) actualPosition;
  }else if (base == 16){
    if (*actualPosition == '0'){
      actualPosition++;
      if (*actualPosition == 'x')
        actualPosition++;
    }
    if ((*actualPosition >= '0' && *actualPosition <= '9') || (*actualPosition >= 'A' && *actualPosition <= 'F')){
      while ((*actualPosition >= '0' && *actualPosition <= '9') || (*actualPosition >= 'A' && *actualPosition <= 'F'))
        actualPosition++;
    }else if (*actualPosition == '\0'){
      *endPtr = (char *) actualPosition;
    }else
      actualPosition--;
    *endPtr = (char *) actualPosition;
  }else{ /* we are sure that there goes every base between 2 and 36 except 8, 10 and 16 */
    if (base <= 10){ /* cause 9 is 10th digit in ASCII */
      while (*actualPosition >= '0' && *actualPosition <= ('0' - 1 + base))
        actualPosition++;
      *endPtr = (char *) actualPosition;
    }else{ /* bases between 11 and 36 inclusive */
      if (*actualPosition == '0'){
        actualPosition++;
        if (*actualPosition == 'x')
          actualPosition++;
      }
      if (isdigit(*actualPosition) || (*actualPosition >= 'A' && *actualPosition <= ('A' - 11 + base))){
        while (isdigit(*actualPosition) || (*actualPosition >= 'A' && *actualPosition <= ('A' - 11 + base)))
          actualPosition++;
      }else
        actualPosition--;
      *endPtr = (char *) actualPosition;
    }
  }


if (base == 0)
  return 0;


  if (sign == POSITIVE)
    cutoff = LONG_MAX / (unsigned long) base;
  else
    cutoff = (unsigned long) LONG_MIN / (unsigned long) base;

  cutlim = LONG_MAX % (unsigned long) base;


  actualPosition = nPtr;

  while ((char *) actualPosition < *endPtr) {
    if (isalnum(* actualPosition) && isdigit(* actualPosition))
      currentdigit = * actualPosition - '0'; //converting to the actual integer
    else if (isalnum(* actualPosition) && isupper(* actualPosition))
      currentdigit = (* actualPosition - 'A') + 10;
    else
      currentdigit = 0;

    if (!correctconversion || number > cutoff || (number == cutoff && (int) currentdigit > cutlim)) {
        correctconversion = false;
        actualPosition++;
    } else { //the actual conversion to decimal
      correctconversion = true;
      number = (number * base) + currentdigit;
      actualPosition++;
    }
  }

  if (!correctconversion) {
    if (sign == POSITIVE)
      number = LONG_MAX;
    else
      number = LONG_MIN;
    // errno = ERANGE;
}

  if (sign == NEGATIVE)
  number *= -1;

  return number;
}