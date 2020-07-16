1) TO compile the program : g++ filename.cpp

EX:g++ AssignmentNo_02.cpp



2) To Run The Program: ./a.out N(switch portcount) B(buffer) P(packet generation prob) queue(INQ|KOUQ|ISLIP) K (knockout) output_file  T(maxtimeslots)

Ex:./a.out 8 4 .5 INQ 4 Outputfile.txt 10000




OUTPUT FORMAT: N	p	Queue type	Avg PD		Std Dev of PD		Avg link utilisation


We have created different sample outputfile for INQ,KOUQ and ISLIP for given default values.
