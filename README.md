Project Problem Statement - https://www.cse.iitd.ac.in/~mausam/courses/col333/autumn2023/A1/A1.pdf

# COL333_AI_A1
Functionalities created:
1. Intelligent restart state choosing - Chooses a state _slightly differing_ from the greedy original state. The differing amount is decided by number of swaps between 0+2 to Z+2 with probability distribution skewed towards the left.
2. Picking best greedy neighbour
3. Neighbour function for both inside swaps (zC_2) and Outside Swaps (z*l)
4. Different escapes at greedy climb i.e. does not find _best_ neighbour for large number of neighbours. Instead takes some better neighbor (heuristic is size dependent).
   
TO DO:
1. Introduce Visited[] array. (For just restart states? or for every state?) => Analyse Later
