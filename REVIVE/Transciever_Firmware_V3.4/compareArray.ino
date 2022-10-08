//Returns 1 if the arrays are the same
uint8_t compareArray(uint8_t* a, uint8_t* b, uint8_t n) 
{
  for(uint8_t i = 0; i<n; i++)
  {
    if( a[i] != b[i] ) return 0;
  }
  return 1;
}
