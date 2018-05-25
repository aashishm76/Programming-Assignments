

#include <stdio.h>
#include <stdlib.h>
#include "LonelyPartyArray.h"

LonelyPartyArray *createLonelyPartyArray(int num_fragments, int fragment_length)
{
  // Check if the given parameters are too small
  if (num_fragments == 0 || num_fragments < 0)
    return NULL;

  if (fragment_length == 0 || num_fragments < 0)
    return NULL;

  int i;

  // Create the pointer to hold the address of a new LPA
  LonelyPartyArray *party = malloc(sizeof(LonelyPartyArray));

  // Error trap: if the call to malloc fails check if its NULL and if it is return NULL and this function fails
  if (party == NULL)
    return NULL;

  // Initialize all the values in the LPA to their passed values
  party->size = 0;
  party->num_fragments = num_fragments;
  party->fragment_length = fragment_length;
  party->fragments = malloc(sizeof(int*) * num_fragments);
  party->num_active_fragments = 0;

  // Initialize the fragments member to NULL
  if (party->fragments == NULL)
  {
    free(party); // Free the original party
    return NULL;
  }

  // Go thru and set each fragment to NULL
  for (i = 0; i < num_fragments; i++)
    party->fragments[i] = NULL;

  party->fragment_sizes = malloc(sizeof(int*) * num_fragments);

  // Check if call to malloc for fragment sizes failed and if so free the previous members and then the whole party
  if (party->fragment_sizes == NULL)
  {
    free(party->fragments);
    free(party);
    return NULL;
  }

  // Loop thru and set each cell in party->fragment_sizes = 0
  for (i = 0; i < num_fragments; i++)
    party->fragment_sizes[i] = 0;

  int M, N;
  M = num_fragments * fragment_length;
  N = num_fragments;

  printf("-> A new LonelyPartyArray has emerged from the void. (capacity: %d, fragments: %d)\n", M, N);

  return party;
}

LonelyPartyArray *destroyLonelyPartyArray(LonelyPartyArray *party)
{
  int i, j;

  if (party != NULL)
  {
    for (i = 0; i < party->num_fragments; i++)
    {
      free (party->fragments[i]);
    }

    free (party->fragment_sizes);
    free (party->fragments);
    free(party);

    printf("-> The LonelyPartyArray has returned to the void.\n");
  }

  return NULL;
}

int set(LonelyPartyArray *party, int index, int key)
{
  int firstArrayIndex, finalSpot, i, startingIndex, endingIndex;

  // Check if the pointer *party is NULL
  if (party == NULL)
  {
    printf("-> Bloop! NULL pointer detected in set().\n");
    return LPA_FAILURE;
  }

  // Calculations to determine what index to go to
  firstArrayIndex = index / party->fragment_length;
  finalSpot = (index % party->fragment_length);
  startingIndex = ((index / party->fragment_length) * party->fragment_length);
  endingIndex = startingIndex + (party->fragment_length - 1);

  // Check if the given index is within the bounds of your LPA & if its not then return LPA_FAILURE
  if (index < 0 || index > (party->num_fragments * party->fragment_length-1))
  {
    printf("-> Bloop! Invalid access in set(). (index: %d, fragment: %d, offset: %d)\n", index, firstArrayIndex, finalSpot);
    return LPA_FAILURE;
  }

  // Check if party->fragments is NULL and if so go ahead and malloc space for the fragment
  if (party->fragments[firstArrayIndex] == NULL)
  {
    party->fragments[firstArrayIndex] = malloc(sizeof(int) * party->fragment_length);
    if (party->fragments[firstArrayIndex] == NULL)
    {
      return LPA_FAILURE;
    }
    party->num_active_fragments++;
    for (i=0; i < party->fragment_length; i++)
    {
      party->fragments[firstArrayIndex][i] = UNUSED;
    }
    printf("-> Spawned fragment %d. (capacity: %d, indices: %d..%d)\n", firstArrayIndex, party->fragment_length, startingIndex, endingIndex);
  }
  if (party->fragments[firstArrayIndex][finalSpot] == UNUSED)
  {
    party->size++;
    party->fragment_sizes[firstArrayIndex]++;
  }

  // Go ahead and store the key at the index
  party->fragments[firstArrayIndex][finalSpot] = key;
  return LPA_SUCCESS;
}

int get(LonelyPartyArray *party, int index)
{
  int location1, locationfinal, validIndex;

  if (party == NULL)
  {
    printf("-> Bloop! NULL pointer detected in get().\n");
    return LPA_FAILURE;
  }

  location1 = index / party->fragment_length;
  locationfinal = index % party->fragment_length;
  validIndex = (party->num_fragments * party->fragment_length - 1);

  // Check if the index passed is within the valid range
  if (index < 0 || index > validIndex)
  {
    printf("-> Bloop! Invalid access in get(). (index: %d, fragment: %d, offset: %d)\n", index, location1, locationfinal);
    return LPA_FAILURE;
  }

  // Check if the fragment that the index is trying to acccess hasn't been made yet
  if (party->fragments[location1] != NULL)
  {
    return (party->fragments[location1][locationfinal]);
  }
  return UNUSED;
}

int delete(LonelyPartyArray *party, int index)
{
  int spot1, spotFinal, indexCheck, startIndex, endIndex;

  if (party == NULL)
  {
    printf("-> Bloop! NULL pointer detected in delete().\n");
    return LPA_FAILURE;
  }

  spot1 = (index / party->fragment_length);
  spotFinal = (index % party->fragment_length);
  indexCheck = (party->num_fragments * party->fragment_length - 1);
  startIndex = ((index / party->fragment_length) * party->fragment_length);
  endIndex = startIndex + (party->fragment_length - 1);

  if (index < 0 || index > indexCheck)
  {
    printf("-> Bloop! Invalid access in delete(). (index: %d, fragment: %d, offset: %d)\n", index, spot1, spotFinal);
    return LPA_FAILURE;
  }

  if (party->fragments[spot1] != NULL)
  {
    if (party->fragments[spot1][spotFinal] == UNUSED)
      return LPA_FAILURE;

    // Set value to unused and decrement respective members of struct
    party->fragments[spot1][spotFinal] = UNUSED;
    party->size--;
    party->fragment_sizes[spot1]--;

    if (party->fragment_sizes[spot1] == 0)
    {
      free(party->fragments[spot1]);
      party->fragments[spot1] = NULL;
      party->num_active_fragments--;
      printf("-> Deallocated fragment %d. (capacity: %d, indices: %d..%d)\n", spot1, party->fragment_length, startIndex, endIndex);
    }

    return LPA_SUCCESS;
  }
  return LPA_FAILURE;
}

int printIfValid(LonelyPartyArray *party, int index)
{
  int loc1, locFinal, indexValid;

  if (party == NULL)
    return LPA_FAILURE;

  loc1 = (index / party->fragment_length);
  locFinal = (index % party->fragment_length);
  indexValid = (party->num_fragments * party->fragment_length - 1);

  if (index < 0 || index > indexValid)
    return LPA_FAILURE;

  if (party->fragments[loc1] != NULL)
  {
    if (party->fragments[loc1][locFinal] == UNUSED)
    {
      return LPA_FAILURE;
    }
    printf("%d\n", party->fragments[loc1][locFinal]);
    return LPA_SUCCESS;
  }
  return LPA_FAILURE;
}

LonelyPartyArray *resetLonelyPartyArray(LonelyPartyArray *party)
{
  int i, j;

  if (party == NULL)
  {
    printf("-> Bloop! Null pointer detected in resetLonelyPartyArray().\n");
  }

  // Loop thru the party->fragments[i] array and if the spot is not equal to NULL then free it
  for (i = 0; i < party->num_fragments; i++)
  {
    if (party->fragments[i] != NULL)
    {
      free(party->fragments[i]);
      party->fragments[i] = NULL;
    }
  }

  // Set party->size to zero
  party->size = 0;

  for(j=0; j < party->num_fragments; j++)
  {
    if (party->fragment_sizes[i] != 0)
    {
      party->fragment_sizes[i] = 0;
    }
  }

  // Set # of active frags to zero
  party->num_active_fragments = 0;
  printf("-> The LonelyPartyArray has returned to its nascent state. (capacity: %d, fragments: %d)\n", (party->num_fragments * party->fragment_length), party->num_fragments);

  return party;
}

int getSize(LonelyPartyArray *party)
{
  if (party == NULL)
    return -1;

  return party->size;
}

int getCapacity(LonelyPartyArray *party)
{
  int totalCapacity;
  if (party == NULL)
    return -1;

  totalCapacity = party->num_fragments * party->fragment_length;

  return totalCapacity;
}

int getAllocatedCellCount(LonelyPartyArray *party)
{
  int maxElements;

  if (party == NULL)
    return -1;

  maxElements = (party->num_active_fragments * party->fragment_length);
  return maxElements;
}

long long unsigned int getArraySizeInBytes(LonelyPartyArray *party)
{
  long long unsigned int traditionalArraysize;
  if (party == NULL)
    return LPA_FAILURE;

  traditionalArraysize = (party->fragment_length * party->num_fragments) * sizeof(int);
  return traditionalArraysize;
}

long long unsigned int getCurrentSizeInBytes(LonelyPartyArray *party)
{
  long long unsigned int LPApointerSize, LPASize, fragmentsSize, fragments2Size, fragment_sizesArraySize, activeFragsSize, total;

  if (party == NULL)
    return 0;

  LPApointerSize = sizeof(party);
  LPASize = sizeof(LPA);
  fragmentsSize = sizeof(int*) * party->num_fragments;
  fragment_sizesArraySize = sizeof(int) * party->num_fragments;
  fragments2Size = sizeof(int) * (party->num_active_fragments * party->fragment_length);

  total = LPApointerSize + LPASize + fragmentsSize + fragments2Size + fragment_sizesArraySize;

  return total;
}

double difficultyRating(void)
{
  return 4;
}

double hoursSpent(void)
{
  return 100;
}
