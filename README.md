# visitability_analysis
Runtime analysis for the outer wave in Complete Visitability Problem.


This is a c++ implementation to find the number of rounds required to perform outward wave in complete visitability problem.
The implementation is based on fully synchronous setting and the runtime is the maximum rounds that may be required for the outward wave.


#How to run


git clone https://github.com/poudelpavan/visitability_analysis.git

cd visitability_analysis

g++ main.cpp -o output

./output

--- Sample output --- 

----------------------------------------------------------------------

	TIME ANALYSIS OF COMPLETE VISITABILITY PROBLEM 

----------------------------------------------------------------------

Enter the number of robots: 200
Select the grid space for robot configuration:
1. Compact square (sqrt(n) * sqrt(n))
2. Tight square ((sqrt(n)*2) * (sqrt(n)*2))
3. Loose square ((sqrt(n)*3) * (sqrt(n)*3))
Choose Option (1, 2 or 3): 2

How many tests do you want to perform: 5

----------------------------------------------------------------------

Total number of robots: 200

Tests 		Convex hull layers (h)		Rounds (Outward Wave)

----------------------------------------------------------------------
Test 1			 h = 19				115

Test 2			 h = 18				87 

Test 3			 h = 18				112

Test 4			 h = 17				91

Test 5			 h = 18				119

----------------------------------------------------------------------
Total rounds: 						524

Average of 5 tests:		h = 18		rounds = 104.8
