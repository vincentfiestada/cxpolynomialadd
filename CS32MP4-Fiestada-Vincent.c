/******************************************************************************
CS 32 Machine Problem # 4 - Adds two three-variable polynomials (x,y,z)
                          - uses the List data structure
                          - reads polynomial input from a file defined as INPUTFILENAME
                          - Each polynomial is a series of terms represented as a string
                            of integers. Each term is four integers long and in this order:
                              Coefficient x-Exponent y-Exponent z-Exponent
                            s.t. 4x^2y^7z^3 - 5xz is
                            4 2 7 3 -5 1 0 1

Programmer: Vincent Paul F. Fiestada
Student Number: 2013-69155
BS Computer Science, University of the Philippines Diliman
Contact: vffiestada@upd.edu.ph

Git Repo: https://github.com/vincentfiestada/cxpolynomialadd.git
******************************************************************************/
#include "cardinal.h"

#define INPUTFILENAME "polynomial.txt"
// Error code constants
#define ERR_INPUTFILE_CANNOTOPEN 100
#define ERR_INPUT_EOF_UNEXPECTED 102
#define ERR_OUT_OF_MEMORY 201

// Type definitions for the Atom & Linear List
// This program uses a Linear List to represent the resulting expression
// And an Atom to represent each term

typedef struct Atom
{
  int Coeff;
  int X;
  int Y;
  int Z;
  struct Atom * Next;
} Atom;

typedef struct LinearList
{
  Atom * Head;
  Atom * End;
} LinearList;

// Function declarations.
// These are not the droids you are looking for. Move along, along.
LinearList * CreateNewLinearList();
Atom * AppendToLinearList(LinearList * L, int coeff, int X, int Y, int Z);
void AnnihilateLinearList(LinearList * L);

int main()
{
    printf("\n++++++++++++++++++++++++++++++++++++++++++++++++++++++");
    printf("\n|                CS 32 MACHINE PROBLEM 4             |");
    printf("\n|                    POLYNOMIAL ADDER                |");
    printf("\n| Vincent Fiestada 2013-69155  vffiestada@upd.edu.ph |");
    printf("\n++++++++++++++++++++++++++++++++++++++++++++++++++++++\n\n");

    // Create and initialize variables
    FILE * inputFile;
    char tempChar;
    int tempCoeff, tempX, tempY, tempZ;
    Atom * alpha, * alphaPREV;
    bool hasBeenAdded;
    int i, j; // iteration counters
    int EOF_Err_Counter = 0; // workaround for feof's weird behavior

    // Create Linear List (represents resulting expression):
    LinearList * expression = CreateNewLinearList();

    // Open file
    inputFile = fopen(INPUTFILENAME, "r");
    if (!inputFile) // Handle errors in opening the file stream
    {
      fprintf(stderr, "\nFATAL ERR: Could not open the file '%s'.\n", INPUTFILENAME);
      exit(ERR_INPUTFILE_CANNOTOPEN);
    }
    // Read four characters at a time until a newline is encountered
    // Do this twice (once for each line)
    for (i = 0; i < 2; i++)
    {
      tempChar = 'x'; // set to some character other than a newline
      do
      {
        fscanf(inputFile, "%d %d %d %d%c", &tempCoeff, &tempX, &tempY, &tempZ, &tempChar);
        printf(" %c ", (tempCoeff < 0) ? '-' : '+');
        printf("%dx^%dy^%dz^%d", abs(tempCoeff), tempX, tempY, tempZ);
        // Search Linear list for a term with equivalent X, Y, & Z exponents
        //  and add this new term

        hasBeenAdded = false;
        if (tempCoeff == 0) continue; // If coefficient is 0, skip this term

        alpha = expression->Head;
        while (alpha)
        {
          // Check if the new term can be added to this one
          if (alpha->X == tempX && alpha->Y == tempY && alpha->Z == tempZ)
          {
            alpha->Coeff += tempCoeff; // add to existing term
            // Delete from list if 0
            if (alpha->Coeff == 0)
            {
              alphaPREV->Next = alpha->Next;
              free(alpha);
            }
            hasBeenAdded = true;
            break;
          }
          alphaPREV = alpha; // needed for deleting Atoms when necessary
          alpha = alpha->Next;
        }

        if (!hasBeenAdded) // If term hasn't been added already, append it to the list
        {
          (void)AppendToLinearList(expression, tempCoeff, tempX, tempY, tempZ);
        }

        if (feof(inputFile) && i < 1) // Check if a line is missing
        {
          fprintf(stderr, "\nFATAL ERR: Reached EOF unexpectedly.\n");
          exit(ERR_INPUT_EOF_UNEXPECTED);
        }
      } while (tempChar != '\n' && !(feof(inputFile)));
      printf("\n");
    }

    // Critical: Close file
    fclose(inputFile);

    // Now display the results:
    printf("\nOUTPUT: (See specs for formatting information)");
    printf("\n------------------------------------------------------\n");
    alpha = expression->Head;
    while (alpha)
    {
      printf("%d %d %d %d ", alpha->Coeff, alpha->X, alpha->Y, alpha->Z);
      alpha = alpha->Next;
    }

    printf("\n------------------------------------------------------\n\n");
    // Cleanup:
    AnnihilateLinearList(expression);
    return 0;
}

// <summary>
//   Allocates memory for a new linear list and initializes the contents to NULL
// </summary>
LinearList * CreateNewLinearList()
{
  LinearList * newBorn = malloc(sizeof(LinearList)); // allocate memory for linear list
  if (!newBorn) // check if malloc was successful
  {
    fprintf(stderr, "\nFATAL ERR: Out of Memory.\n");
    exit(ERR_OUT_OF_MEMORY);
  }
  // Initialize contents to NULL:
  newBorn->Head = NULL;
  newBorn->End = NULL;
  return newBorn;
}

// <summary>
//   Creates a new Atom and appends it to the end of a linear list;
//    also initializes the Atom; returns a pointer to the Atom
// </summary>
Atom * AppendToLinearList(LinearList * L, int coeff, int X, int Y, int Z)
{
  Atom * newBorn = malloc(sizeof(Atom)); // allocate memory for Atom
  if (!newBorn) // check if malloc was successful
  {
    fprintf(stderr, "\nFATAL ERR: Out of Memory.\n");
    exit(ERR_OUT_OF_MEMORY);
  }
  // Initialize data
  newBorn->Coeff = coeff;
  newBorn->X = X;
  newBorn->Y = Y;
  newBorn->Z = Z;
  newBorn->Next = NULL;
  // Append to List L
  if (L->Head == NULL)
  {
    L->Head = newBorn;
  }
  else
  {
    L->End->Next = newBorn;
  }
  L->End = newBorn;
  return newBorn;
}

// <summary>
//   Frees up all atoms in a list and then frees up the list itself
// </summary>
void AnnihilateLinearList(LinearList * L)
{
  Atom * node = L->Head;
  Atom * temp = NULL;
  while (node)
  {
    temp = node->Next; // save pointer to the next node
    free(node); // free up the current node
    node = node->Next; // move on to the next one
  }
  free(L); // free up the list itself
}