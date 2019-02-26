'''This script implements quick sort.
Quick sort uses a divide and conquer technique, which
employs a partition function to recusively put element before
and after the pivot. The average complexity (theta) is lgN.
Quicksort is also in-place sorting, thus auxilary space is N.
QUicksort is better than merge sort in practice mostly dues to
the auxilary behavior
'''

def quick_sort(mylist, p, r):
    """Base case: when the distance of left and right is atomic
    then do nothing.
       Recusive case: when the left and right arrays has length > 1
    """
    q = partition(mylist, p, r)
    if q - 1 - p > 0:
        quick_sort(mylist, p, q - 1)
    if r - 1 - q > 0:
        quick_sort(mylist, q + 1, r)


def partition(mylist, p, r):
    """
    Purpose: return a position of the pivot
    Invariant: after the call, everything left of pivot is <= pivot
    everything right of pivot is >= pivot
    """
    # what if the list passed is empty?
    # what if the list passed is singleton?
    if r == p:
        return r
    smaller = []
    greater = []
    pivot = mylist[r]
    for item in mylist[p:r]:
        if item > pivot:
            greater.append(item)
        else:
            smaller.append(item)
    pivotPos = p + smaller.__len__()
    # merge into a long list:
    # put back into the original list
    currentPos = p
    for item in smaller:
        mylist[currentPos] = item
        currentPos += 1
    mylist[pivotPos] = pivot
    currentPos += 1
    for item in greater:
        mylist[currentPos] = item
        currentPos += 1
    return pivotPos
