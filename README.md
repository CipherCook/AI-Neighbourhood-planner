# COL333_AI_A1
Functionalities created:
1. Intelligent restart state choosing - Chooses a state _slightly differing_ from the greedy original state. The differing amount is decided by number of swaps between 0+2 to Z+2 with probability distribution skewed towards the left.
2. Picking best greedy neighbour
3. Neighbour function for both inside swaps (zC$_2$) and Outside Swaps (z*l)
   
TO DO:
1. Add restart while loop till t mins specified.
2. Introduce Visited[] array. (For just restart states? or for every state?) => Analyse Later
3. Add more heuristics
4. Simulate bigger testcases.
5. Find bugs.
