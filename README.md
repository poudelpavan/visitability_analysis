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
How many tests do you want to perform: 50
----------------------------------------------------------------------
Total number of robots: 200
Tests      Convex hull layers (h)   Rounds (Outward Wave)
----------------------------------------------------------------------
Test 1          h = 18               135
Test 2          h = 18               135
Test 3          h = 19               138
Test 4          h = 19               159
Test 5          h = 18               130
Test 6          h = 19               120
Test 7          h = 19               126
Test 8          h = 18               127
Test 9          h = 18               130
Test 10         h = 18               127
Test 11         h = 19               138
Test 12         h = 19               141
Test 13         h = 19               147
Test 14         h = 19               145
Test 15         h = 17               110
Test 16         h = 19               159
Test 17         h = 18               120
Test 18         h = 18               125
Test 19         h = 18               105
Test 20         h = 18               139
Test 21         h = 18               129
Test 22         h = 18               138
Test 23         h = 18               134
Test 24         h = 19               151
Test 25         h = 19               149
Test 26         h = 18               105
Test 27         h = 19               151
Test 28         h = 18               112
Test 29         h = 18               113
Test 30         h = 18               126
Test 31         h = 18               106
Test 32         h = 19               145
Test 33         h = 18               124
Test 34         h = 19               139
Test 35         h = 18               130
Test 36         h = 18               130
Test 37         h = 18               116
Test 38         h = 18               135
Test 39         h = 17               110
Test 40         h = 18               133
Test 41         h = 17               107
Test 42         h = 18               136
Test 43         h = 18               128
Test 44         h = 20               136
Test 45         h = 18               121
Test 46         h = 19               150
Test 47         h = 19               139
Test 48         h = 19               142
Test 49         h = 17               114
Test 50         h = 18               120
----------------------------------------------------------------------
Total rounds:                        6525
Average of 50 rounds:                130.5

