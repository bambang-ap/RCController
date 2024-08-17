int indexOf(String arr[], String valueToFind) {
  int indexOfValue = -1;

  for (int i = 0; i < sizeof(arr); ++i) {
    if (arr[i] == valueToFind) {
      indexOfValue = i;
      break;
    }
  }

  return indexOfValue;
}