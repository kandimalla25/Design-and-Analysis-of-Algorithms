#include <stdio.h>
//#include <conio.h>
#include <string.h>

typedef char STRING[50];

int min (int a, int b);
int findOverlappingPair(STRING str1, STRING str2, char *str);
int findShortestSuperstring(STRING arr[], int len);

void main ()
{
 int noofstrings;
// char[50] STRING;
 STRING strings[100];
 int i,j;

 printf ("# Strings = ");
 scanf ("%d", &noofstrings);
 printf ("Input the strings:\n");
 for (i=0; i<noofstrings; i++)
 scanf ("%s", strings[i]);
 for (i=0; i<noofstrings; i++)
 {
  for (j=i+1; j<noofstrings; j++)
   if ( (strstr(strings[i],strings[j]) == NULL)  &&
	(strstr(strings[j],strings[i]) == NULL) )
   { printf ("No substrings found in %s  %s \n", strings[i], strings[j]); }
   else
   {
    printf ("ERROR  !! ERROR \n");
    printf ("%s, %s  are substrings of one another \n", strings[i], strings[j]);
    return;
   }
 }
 findShortestSuperstring(strings, noofstrings);
 printf ("ShortestSuperString = %s \n", strings);
 getch();
}

int min(int a, int b)
{
    return (a < b) ? a : b;
}

int findOverlappingPair(STRING str1, STRING str2, char *str)
{

    // Max will store maximum
    // overlap i.e maximum
    // length of the matching
    // prefix and suffix
    int max = -10000;
    int i;
    int len1 = strlen(str1); //str1.length();
    int len2 = strlen(str2); //str2.length();

    // Check suffix of str1 matches
    // with prefix of str2
    for (i = 1; i <= min(len1, len2); i++)
    {
	// Compare last i characters
	// in str1 with first i
	// characters in str2
	if (strncmp(&str1[len1-i], str2, i) == 0)
//	if (str1.compare(len1-i, i, str2, 0, i) == 0)
	{
	    if (max < i)
	    {
		// Update max and str
		max = i;
	       //	str =
		memset (str, 0, 5000);
		strncpy(str,str1,len1);
		strncat(str, &str2[i], len2-i);
	      //	strncat(str1, &str2[i], len2-i); // str1 + str2.substr(i);
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
//	if (str1.compare(0, i, str2, len2-i, i) == 0)
	{
	    if (max < i)
	    {

		// Update max and str
		max = i;
		memset(str, 0, 5000);
		strncpy (str, str2, len2);
		strncat (str, &str1[i], len1-i);
	     //	strncat(str2, &str1[i], len1-i);
		//str = str2 + str1.substr(i);
	    }
	}
    }

    return max;
}

// Function to calculate
// smallest string that contains
// each string in the given
// set as substring.
int findShortestSuperstring(STRING arr[], int len)
{

    // Run len-1 times to
    // consider every pair
    char str[5000];
    for (int ii=0;ii<5000; ii++) str[ii] = '\0';
    while(len != 1)
    {

	// To store  maximum overlap
	int max = -10000; //INT_MIN;

	// To store array index of strings
	int l, r;

	// Involved in maximum overlap
//	STRING resStr;
	char resStr[5000];
	for (int ii=0;ii<5000; ii++) resStr[ii] = '\0';


	// Maximum overlap
	for (int i = 0; i < len; i++)
	{
	    for (int j = i + 1; j < len; j++)
	    {
     //		STRING str;
		for (int ii=0;ii<5000; ii++) str[ii] = '\0';
		// res will store maximum
		// length of the matching
		// prefix and suffix str is
		// passed by reference and
		// will store the resultant
		// string after maximum
		// overlap of arr[i] and arr[j],
		// if any.
		int res = findOverlappingPair(arr[i],
					 arr[j], str);

		// check for maximum overlap
		if (max < res)
		{
		    max = res;
		    strcpy(resStr, str);
		    printf ("Overlap of %s and %s = %s\n", arr[i], arr[j], resStr);
		   // resStr.assign(str);
		    l = i, r = j;
		}
	    }
	}

	// Ignore last element in next cycle
	len--;

	// If no overlap, append arr[len] to arr[0]
	if (max == -10000 /*INT_MIN*/)
	    strcat(arr[0], arr[len]);
	//    arr[0] += arr[len];
	else
	{

	    // Copy resultant string to index l
	   // arr[l] = resStr;
	   strcpy(arr[l], resStr);

	    // Copy string at last index to index r
	    strcpy (arr[r], arr[len]);  //arr[r] = arr[len];
	}
    }
    return 1; //arr[0];
}
