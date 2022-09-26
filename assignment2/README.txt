There are 2 folders 
1. task1 (Part A)
2. task2 (Part B)

task1 and task2 contains 2 files each
1. fileServer.cpp
2. client.cpp

you can compile these using g++ compiler
1. g++ fileServer.cpp -o server
if the above throws error then try (because of compiler version)
(or) g++ -pthread fileServer.cpp -o server

2. g++ client.cpp -o client

task1's fileServer can only connect to one client, once the file transfer is done, it will end the connection.

task2's fileServer can connect to multiple clients (5 in this case), any more will get queued.

when client prompts to "enter path:" provide the actual path in server machine EX: /home/kaif/quiz.pdf or relative path to PWD

if the file is present it will send the file size and file otherwise it will send file not found.


PLAGIARISM STATEMENT 

I certify that this assignment/report is my own work, based on my personal study and/or
research and that I have acknowledged all material and sources used in its preparation, whether
they be books, articles, reports, lecture notes, and any other kind of document, electronic or
personal communication. I also certify that this assignment/report has not previously been
submitted for assessment in any other course, except where specific permission has been
granted from all course instructors involved, or at any other time in this course, and that I have
not copied in part or whole or otherwise plagiarized the work of other students and/or persons. I
pledge to uphold the principles of honesty and responsibility at CSE@IITH. In addition, I
understand my responsibility to report honor violations by other students if I become aware of it.

Name of the student: P Kaif Ali Khan
Roll No: cs22mtech12009