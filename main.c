/* COP 3502C Programming Assignment 3 
This program is written by: Sarah Whitfield */ 

#include <stdio.h>
#include <stdlib.h>
#include "leak_detector_c.h"

typedef struct coordinate {
  int x;
  int y;
  int relativeDistance;
} coordinate;

//defining global x and y variables and file
int x;
int y;
FILE *fptr;

coordinate * ReadData(int * nCities, int * nPoints, int * threshold);
int CompareTo(coordinate * ptrPt1, coordinate * ptrPt2);
void sort(coordinate * points, int nCities, int threshold);
void Insertion_Sort(coordinate * points, int nCities, int offset);
void MergeSort(coordinate * points, int l, int r, int threshold);
void merge(coordinate * points, int l, int m, int r);
void Binary_Search(coordinate * points, coordinate item, int length);
void print(coordinate * points, int nCities);

int main(void) {

  //initializes variables
  atexit(report_mem_leak);
  fptr = fopen("out.txt", "w");
  int nCities, nPoints, threshold;
  coordinate * item = (coordinate *) malloc(sizeof(coordinate));
  
  //collects data from input
  coordinate * points = ReadData(&nCities, &nPoints, &threshold);

  //sorts data and prints sorted values
  sort(points, nCities, threshold);
  print(points, nCities);

  for(int i = 0; i<nPoints; i++)
    {
      //for each coordinate, data is collected and distance is calculated
      scanf("%d %d", &item->x, &item->y);
      item->relativeDistance = (item->x-x)*(item->x-x) + (item->y-y) * (item->y-y);
      //binary search is ran for each coordinate, finding and printing its location
      Binary_Search(points, *item, nCities);
    }

  free(points);
  free(item);
  return 0;
  
}

//wrapper function that determines whether insertion_sort or mergesort is called
void sort(coordinate * points, int nCities, int threshold)
{
  //if nCities is <= the threshold, immediately run insertion_sort, otherwise run mergesort
  if(nCities <= threshold)
    Insertion_Sort(points, nCities, 0);
  else
    MergeSort(points, 0, nCities-1, threshold);
}

//prints the array of coordinates
void print(coordinate * points, int nCities)
{
  for(int i = 0; i<nCities; i++)
    {
      //prints the x and y coordinate for each point both to normal output and to out.txt
      printf("%d %d\n", points[i].x, points[i].y);
      fprintf(fptr, "%d %d\n", points[i].x, points[i].y);
    }
}

//insertion_sort relocates each point to the correct location until all points have been sorted
//the offset value is used to represent the beginning of the area to be sorted
void Insertion_Sort(coordinate * points, int nCities, int offset)
{
  
  coordinate item;
  int i, j;

  //this loop runs for the number of cities left to be sorted, not the entire array
  for(i = 1; i<nCities; i++)
    {
      //using offset ensures that sorting starts within the allowed area
      item = points[i+offset];
      
      for(j=i-1; j>=0; j--)
        {
          //if the earlier coordinate is farther than the chosen item, shift the farther item to a later position in the array
          if(CompareTo(&points[j+offset],&item) > 0)
            points[j+1+offset] = points[j+offset];
          else
            //if the earlier coordinate is closer than the chosen item, stop the loop
            break;
        }
      
      //the point where the earlier item was not farther than the item will become the item 
      points[j+1+offset] = item;
      
    }
}


//MergeSort sorts recursively by calling itself and dividing the array until the number of cities is <= the threshold and can be sent to insertion sort
void MergeSort(coordinate * points, int l, int r, int threshold)
{
  
  //if the size of the array is over the threshold, mergesort will run
  if(r - l + 1 > threshold)
    {
      //the midpoint will be adjusted
      int m = (l + r)/2;
      //two more mergesort calls will occur that will then be merged using merge
      MergeSort(points, l, m, threshold);
      MergeSort(points, m+1, r, threshold);
      merge(points, l, m, r);
    }
    
  //otherwise, insertion_sort will run using offset = left value and the current size of the array
  else
      Insertion_Sort(points, r-l+1, l);
  
}

//merge combines two calls of MergeSort
void merge(coordinate * points, int l, int m, int r)
{

  //initializes variables and calculates size of each array
  int i, j, k;
  int size1 = m - l + 1;
  int size2 = r - m;

  //initializes two variables to hold the two sorted sections of the array
  coordinate * L = (coordinate *) malloc(size1 * sizeof(coordinate));
  coordinate * R = (coordinate *) malloc(size2 * sizeof(coordinate));

  //copies values to each array from points[]
  for(i = 0; i<size1; i++)
    L[i] = points[l + i];
  for(j = 0; j<size2; j++)
    R[j] = points[m + 1 + j];

  //resets variables
  i = 0; j = 0; k = l;

  //merges the two arrays back together
  while(i < size1 && j < size2)
    {
      if(CompareTo(&L[i],&R[j]) <= 0)
      {
        points[k] = L[i];
        i++;
      }
      else
      {
        points[k] = R[j];
        j++;
      }
      k++;
    }

  //adds any remaining components of the first array
  while(i<size1)
    {
      points[k] = L[i];
      i++;
      k++;
    }

  //adds any remaining components of the second array
  while(j<size2)
    {
      points[k] = R[j];
      j++;
      k++;
    }

  free(L);
  free(R);
  
}

//Searches for a corresponding point by continually dividing the searched area in two
void Binary_Search(coordinate * points, coordinate item, int length)
{
  //initializes values
  int l = 0, h = length - 1, mid;

  //while the list still has components
  while(l<=h)
    {
      //recalculates mid
      mid = (l+h)/2;

      //if the points are the exact same, print and break
      if(CompareTo(&points[mid], &item) == 0)
      {
        printf("%d %d found at rank %d\n", item.x, item.y, mid+1);
        fprintf(fptr, "%d %d found at rank %d\n", item.x, item.y, mid+1);
        return;
      }
      //otherwise, adjust the low or high values
      if(CompareTo(&points[mid], &item) < 0)
        l = mid + 1;
      else
        h = mid - 1;
    }
  //if none are a match, the item is not in the list
  printf("%d %d not found\n", item.x, item.y);
  fprintf(fptr, "%d %d not found\n", item.x, item.y);
}



//reads required inputs and returns the points to be sorted
coordinate * ReadData(int * nCities, int * nPoints, int * threshold)
{
  //collects data and allocates memory
  scanf("%d %d %d %d %d", &x, &y, nCities, nPoints, threshold);
  coordinate * points = (coordinate *) malloc(*nCities * sizeof(coordinate));
  for(int i = 0; i<*nCities; i++)
    {
      scanf("%d %d", &points[i].x, &points[i].y);
      points[i].relativeDistance = (points[i].x-x)*(points[i].x-x) + (points[i].y-y)*(points[i].y-y);
    }
  return points;
}



//takes 2 points, returns negative int if ptrPt1 is closer, positive if opposite, 0 if identical, if same distance but different points, tiebreaks using x and then y coordinates
int CompareTo(coordinate * ptrPt1, coordinate * ptrPt2)
{

  //if they are exactly the same, return 0
  if(ptrPt1->x == ptrPt2->x && ptrPt1->y == ptrPt2->y)
    return 0;

  //if the distance is the same, break the tie with x then y values
  if(ptrPt1->relativeDistance == ptrPt2->relativeDistance)
    {
      if(ptrPt1->x < ptrPt2->x)
        return -1;
      else if(ptrPt1->x > ptrPt2->x)
        return 1;
      else if(ptrPt1->y < ptrPt2->y)
        return -1;
      else if(ptrPt1->y > ptrPt2->y)
        return 1;   
    }

  //if ptrP1 is closer, return -1
  if(ptrPt1->relativeDistance < ptrPt2->relativeDistance)
    return -1;

  //if ptrP1 is farther away, return 1
  if(ptrPt1->relativeDistance > ptrPt2->relativeDistance)
    return 1;
  
  return 0;
  
}