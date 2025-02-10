#!/usr/bin/env

import sys

def randList(k=5, n=30):
    import random
    return [random.randint(1,k) for _ in range(n)]

class Room:
    def __init__(self, company, name):
        self.company = int(company)
        self.name = name
    
    def __str__(self):
        return self.name

    def __eq__(self, other):
        return self.company == other

    def __lt__(self, other):
        if isinstance(other, Room):
            return self.company < other.company
        elif isinstance(other, int):
            return self.company < other
        else:
            raise Exception(f'Room < {type(other)} not allowed')
    def __hash__(self):
        return hash(self.company)

def count(A):
    counts = {}
    for el in A:
        if el in counts:
            counts[el] += 1
        else:
            counts[el] = 1
    return counts

# find the left index with the most X in a given width
def indexWithMost(A, X, width):
    left = 0
    most = 0
    # check as many as possible (up to len(A)-width)
    for i in range(len(A)-width):
        if A[i] == X:
            c = count(A[i:i+width])[X]
            # leaving it at > takes the first occurrence of the "max" value found
            if c > most:
                left = i
                most = c

    # check the last width amount:
    try:
        c = count(A[len(A) - width:])[X]
        if c > most:
            left = len(A) - width
    except:
        pass

    return left

def addAndShiftIfNeeded(buckets, toAdd):
    # buckets = [0000000000000000] <-- list of numbers representing available space
    company, left, right = toAdd
    width = right - left + 1
    # if there is a full overlap: find the first zero spot and put it there (shift as needed)
    if all([x != 0 for x in buckets[left:right+1]]):
        left = buckets.index(0) # i don't know when there might be no zero's left
        right = width + left
    
    # needs to be at least the same amount of zeros to the left as there is overlap
    # to the left
    zeroCountLeft = count(buckets[0:left+1])
    
    if 0 not in zeroCountLeft:
        zeroCountLeft = 0
    else:
        zeroCountLeft = zeroCountLeft[0]
   
    # to prevent from inserting between a bunch of values
    if zeroCountLeft == 0:
        left = buckets.index(0)
        right = width + left
    
    #count non-zero items from the left
    overlapLeft = 0
    for i in range(left, right+1):
        if buckets[i] == 0:
            break
        overlapLeft += 1
   
    totalZerosRemoved = 0

    zerosFromLeft = min(overlapLeft, zeroCountLeft)

    # remove number of overlaps from the left
    try:
        #print(f'removing {min(overlapLeft, zeroCountLeft)} zeros from the LEFT' )
        [buckets.remove(0) for _ in range(zerosFromLeft)]
        totalZerosRemoved += zerosFromLeft
    except:
        pass
   
    # make sure I don't insert in the middle of something else
    if totalZerosRemoved < overlapLeft:
        left += overlapLeft - totalZerosRemoved
   
    # insert at left index
    [buckets.insert(left,company) for _ in range(width)]

    # remove width amount of zeros in total 
    zerosFromRight = width - zerosFromLeft
    
    # if that many zeros cannot be removed from the right: remove them from the left
    zerosOnRight = count(buckets[right:])
    if 0 not in zerosOnRight:
        zerosOnRight = 0
    else:
        zerosOnRight = zerosOnRight[0]

    if zerosOnRight < zerosFromRight:
        zerosFromLeft = zerosFromRight - zerosOnRight
        zerosFromRight = zerosOnRight
   
    
    # remove the remaining number of 0's from the right
    try:
        #print(f'removing {zerosFromRight} zeros from the RIGHT')
        [buckets.pop(buckets.index(0,left)) for _ in range(zerosFromRight)]
        totalZerosRemoved += zerosFromRight
    except:
        pass

    # remove number of excess zeros from the left
    try:
        if totalZerosRemoved != width:
            #print(f'removing {min(overlapLeft, zeroCountLeft)} zeros from the LEFT' )
            [buckets.remove(0) for _ in range(zerosFromLeft)]
    except:
        pass

    return buckets

def convert(buckets):
    result = []
    company = buckets[0]
    left = 0
    for i, el in enumerate(buckets):
        if el != company:
            result.append( (company, left, i-1) )
            left = i
            company = el
    if company != result[-1][0]:
        result.append((company, left, len(buckets)-1))
    return result


# this is the hard part
def makeGoalBuckets(A, counts):
    bucketWidths = [(k,v) for k,v in counts.items()]
    # sort by the size of the bucket, then by company
    bucketWidths = sorted(bucketWidths, key=lambda x: x[0])
    bucketWidths = sorted(bucketWidths, key=lambda x: x[1], reverse=True)
    #print(f'fitting buckets: {bucketWidths}' ) 
    # a bucket is (goal company, left bound, right bound)
    buckets = [0] * len(A)
    
    for company, width in bucketWidths:
        left = indexWithMost(A, company, width)
        right = left + width - 1
        buckets = addAndShiftIfNeeded(buckets, (company, left, right))
        #print(company, left, right, buckets, len(buckets)) 
    # convert from the current representation to (company, left, right)
    #print(buckets)

    buckets = convert(buckets)

    return buckets

# easy parts (swapping)
def swap(A, a, b):
    print(A[a], A[b])
    A[a], A[b] = A[b], A[a]
    return A

def findBucket(buckets, company):
    for bucket in buckets:
        if bucket[0] == company:
            return bucket
    return None

def oneForOneSwap(A, buckets):
    a = None
    b = None
    for bucket in buckets:
        goal = bucket[0]
        # find the first index that is not the goal
        #print('looking at bucket:',bucket,'for not this: ', goal)
        for i in range(bucket[1], bucket[2]+1):
            if i >= len(A):
                print('i was bigger than len(A)', i)
            if A[i] != goal:
                # see if another bucket has what I want to swap out
                goalB = A[i]
                bucketB = findBucket(buckets, goalB)#buckets[goalB]
                #print('found not this:', goalB, 'looking in:', A[bucketB[1]:bucketB[2]+1], 'for: ', goal)
                if goal in A[bucketB[1]:bucketB[2]+1]:
                    a = i # goalB - ie the thing to go into "bucketB"
                    b = A.index(goal, bucketB[1], bucketB[2]+1) # goal to go into "bucket"
                    break
    return (a, b)

def findSwap(A, bucket):
    goal = bucket[0]
    a = None
    b = None
    # find element outside of bucket that should go inside bucket
    #print('finding element outside bucket')
    for i in range(0,len(A)):
        if i < bucket[1] or i > bucket[2]:
            #print('elements outside bucket: ' , A[i])
            if A[i] == goal:
                b = i
                break
    # find element inside bucket that belongs somewhere else
    #print('looking for elsewhere item')
    for i in range(bucket[1], bucket[2]+1):
        #print(A[i])
        if A[i] != goal:
            a = i
            break

    return (a, b)

def group(A):
    counts = count(A)
    #print(counts)
    # a bucket looks like: (goal company, left bound, right bound)# seems unrealistic in Python, [reference to elements inside bucket])
    # will be a sorted list where each bucket bucket is adjacent to the next, starting at 0 and ending at len(A)-1
    buckets = makeGoalBuckets(A, counts)
    #print('A',A)
    #print('counts',counts)
    #print('buckets',buckets) 
    # execute all one for one swaps
    prev = None
    while True: 
        # a and b are indices to swap
        a, b = oneForOneSwap(A, buckets)
        if a == None or b == None:
            break
        A = swap(A, a, b)
    # now do the final swaps for each bucket
    #print('made it to part 2')
    #print(A)
    done = False
    while not done:
        done = True
        for bucket in buckets:
            a, b = findSwap(A, bucket)
            if a is None and b is None:
                continue 
            A = swap(A, a, b)
            done = False
    
    return A 

def readInput():
    A = []
    for line in sys.stdin:
        company, name = line.strip().split()
        A.append( Room(company, name) )
    return A

if __name__ == "__main__":
    #A = randList(k=9,n=100)
    #c = count(A)
    #print(f'count = {c}')
    #print(makeGoalBuckets(A,c))
    #A = group(A)
    #print(A)
    A = readInput()
    group(A)
