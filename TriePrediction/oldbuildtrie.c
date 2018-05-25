// Old buildTrie

TrieNode *root, *placeHolder = NULL;
char corpusInput[MAX_CHARACTERS_PER_WORD + 1];
char scrubbedStr[MAX_CHARACTERS_PER_WORD + 1];

FILE *inputFile;
inputFile = fopen(filename, "r");

// Create the root node
root = createTrieNode();

while(fscanf(inputFile, "%s", corpusInput) != EOF)
{
  // Scrub the strings
  strcpy(scrubbedStr, corpusInput);
  stripPunctuation(scrubbedStr);

  // Insertion into main trie
  insertString(root, scrubbedStr);

  // printf("\n%s inserted into MAIN trie\n",scrubbedStr);

  // Subtrie insertion using a placeholder pointer to keep track of previous word entered
  if(placeHolder == NULL)
  {
    placeHolder = pathfinder(root, scrubbedStr);
  }
  else
  {
    if(placeHolder->subtrie == NULL)
      placeHolder->subtrie = createTrieNode();

    insertString(placeHolder->subtrie, scrubbedStr);

    placeHolder = pathfinder(root, scrubbedStr);

    // printf("%s inserted into SUB trie\n", scrubbedStr);
  }
