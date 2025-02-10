George Prielipp (265112)

No references were used.

The algorithm works by finding the best buckets (areas in the list) to place each company. It then goes and tries to find one for one swaps until there are none left. Then it goes done the list and swaps in the remaining items.

The idea came from lots of conversation with LT Gentile. I was initially trying to reduce the amount of swaps by making the window (left and right bound of a company) shrink at each iteration. Which worked very well for small n and small k, but became rapidly worse with larger n and k. So, LT Gentile told me to think about optimizing something else. Which led to an idea of trying to pre-fit the windows (buckets).

ANALYSIS FOR TIME:
- group: O(n^2 + kn)
  * count: runs through the list once => Theta(n)
  * makeGoalBuckets: going with the assumption that k is much smaller than n on average, means this runs in O(kn) on average
    - the major terms come from doing k iterations of indexWithMost (which takes O(n) time)
  * first while loop (oneForOneSwap): O(n^2 + kn)
    - it runs for n/2 iterations on average in the worst case
    - it calls onerForOneSwap each iteration, which has a runtime of O(n + n/k + k)
  * second while loop (findSwap): O(n^2 + (n^2)/k)
    - in the worst case it runs n iterations
    - it calls findSwap for each iterations, which has a runtime of O(n + n/k) 

ANALYSIS FOR SWAPS:
- at a minimum, the algorithm would be able to make every 1:1 swap => n/2 swaps or Omega(n)
- at the worst case, the algorithm would run through the whole list and make n - 1 swaps, or O(n)
- which implies the swaps are Theta(n)
