/*

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef char STRING[50];

int min (int a, int b);
int findOverlappingPair(STRING str1, STRING str2, char *str, int *direction);
int findShortestSuperstring(STRING arr[], int nStrings);

int main ()
{
 int noofstrings;
 STRING strings[100];
 int i,j;

// STRING infilename;
// FILE *fp;
 printf ("# Strings = ");
 scanf ("%d", &noofstrings);
 printf ("Input the strings:\n");
 for (i=0; i<noofstrings; i++)
 scanf ("%s", strings[i]);

//find out whether any substrings exist among themselves
//in the given set of strings
 for (i=0; i<noofstrings; i++)
 {
  for (j=i+1; j<noofstrings; j++)
   if ( (strstr(strings[i],strings[j]) == NULL)  &&
	(strstr(strings[j],strings[i]) == NULL) )
   {/* printf ("No substrings found in %s  %s \n", strings[i], strings[j]);*/ }
   else
   {
    printf ("ERROR  !! ERROR \n");
    printf ("%s, %s  are substrings of one another \n", strings[i], strings[j]);
    return;
   }
 }
 findShortestSuperstring(strings, noofstrings);
 printf ("Length of ShortestSperString = %d \n", strlen(strings[0]));
 printf ("ShortestSuperString = %s \n", strings);
 return 0;
}

int min(int a, int b)
{
    return (a < b) ? a : b;
}

int findOverlappingPair(STRING str1, STRING str2, char *str, int *overlapdirection)
{

    int max = -10000;
    int i;
    int len1 = strlen(str1);
    int len2 = strlen(str2);

    // Check suffix of str1 matches
    // with prefix of str2
    for (i = 1; i <= min(len1, len2); i++)
    {
	// Compare last i characters
	// in str1 with first i
	// characters in str2
	if (strncmp(&str1[len1-i], str2, i) == 0)
	{
	    if (max < i)
	    {
	      max = i;
	      *overlapdirection = 0;
	      memset (str, 0, 5000);
	      strncpy(str,str1,len1);
	      strncat(str, &str2[i], len2-i);
	    }
	}
    }

    // Check prefix of str1 matches
    // with suffix of str2
    for (i = 1; i <= min(len1, len2); i++)
    {

	// compare first i characters
	// in str1 with last i
	// characters in str2
	if (strncmp(str1, &str2[len2-i], i) ==0)
	{
	    if (max < i)
	    {
	      max = i;
	      *overlapdirection = 1;
	      memset(str, 0, 5000);
	      strncpy (str, str2, len2);
	      strncat (str, &str1[i], len1-i);
	    }
	}
    }

    return max;
}

int findShortestSuperstring(STRING arr[], int nStrings)
{
    char str[5000];
    int overlapdirection = 0;
    int direction = 0;
    int d;    //d is #spaces to be shifted for display
	int i, ii, j;
	
    for (ii=0;ii<5000; ii++) str[ii] = '\0';
    while(nStrings != 1)
    {
      int max = -10000;
      int l, r;
      char resStr[5000];
      for (ii=0;ii<5000; ii++) resStr[ii] = '\0';

      for (i = 0; i < nStrings; i++)
      {
	for (j = i + 1; j < nStrings; j++)
	{
	  for (ii=0;ii<5000; ii++) str[ii] = '\0';
	  int res = findOverlappingPair(arr[i], arr[j], str, &overlapdirection);
	  // check for maximum overlap
	  if (max < res)
	  {
	      max = res;
	      strcpy(resStr, str);
	    //  printf ("Overlap of %s and %s = %s\n", arr[i], arr[j], resStr);
	      l = i, r = j;
	      direction = overlapdirection;
	  }
	}
      }

      nStrings--;
// If no overlap, append arr[nStrings] to arr[0]
      if (max == -10000 )
      {
	printf ("\t %s \n", arr[0]);
	printf ("\t");
	for (d=0; d<=(strlen(arr[0])); d++) printf (" ");
	printf ("%s\n", arr[nStrings]);
	strcat(arr[0], arr[nStrings]);
      }
      else
      {
	if (!direction)  //i.e. 1st string followed by 2nd string
	{
	   printf ("\t %s \n", arr[l]);
	   printf ("\t");
	   for (d=0; d<=(strlen(arr[l])-max); d++) printf (" ");
	   printf ("%s\n", arr[r]);
	}
	else         //2nd string followed by 1st string
	{
	   printf ("\t %s \n", arr[r]);
	   printf ("\t");
	   for (d=0; d<=(strlen(arr[r])-max); d++) printf (" ");
	   printf ("%s\n", arr[l]);
	}
	//arr[l] = 'uv'; arr[r] = 'vw'; resStr contains 'uvw'
	//copy 'uvw' in arr[l] and copy the last string in the set in arr[r]
	//By copying the last string in arr[r],
	//we are reducing the noof strings by 1
	strcpy(arr[l], resStr);
	strcpy (arr[r], arr[nStrings]);
      }
    }
    return 1;
}

