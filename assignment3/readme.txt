---------------------------- Part A ----------------------------

1. There are 3 files in partA folder.
    1.1 bootstrapServer.cpp
    1.2 fileServer.cpp
    1.3 datatypes.cpp

2. Run bootstrapServer.cpp
    >> g++ bootstrapServer.cpp -o bootstrap
    >> ./bootstrap

3. Run fileServer.cpp
    >> g++ fileServer.cpp -o fileServer
    or >> g++ -pthread fileServer.cpp -o fileServer
    >> ./fileServer

4. You can run as many fileServes you want.
5. When you run the fileServer, it will prompt you to give some information.
    Enter port:
    Enter service name:
    Enter type (mkv,pdf,jpeg,png etc.):
    Enter access token: 

6. After filling, it will register with bootstrap.


---------------------------- Part B ----------------------------

1. There are 3 files in partA folder.
    1.1 bootstrapServer.cpp
    1.2 fileServer.cpp
    1.3 datatypes.cpp
    1.4 client.cpp
    1.5 invalid_client.cpp

2. Run bootstrapServer.cpp
    >> g++ bootstrapServer.cpp -o bootstrap
    >> ./bootstrap

3. Run fileServer.cpp
    >> g++ fileServer.cpp -o fileServer
    or >> g++ -pthread fileServer.cpp -o fileServer
    >> ./fileServer

4. You can run as many fileServes you want.
5. When you run the fileServer, it will prompt you to give some information.
    Enter port:
    Enter service name:
    Enter type (mkv,pdf,jpeg,png etc.):
    Enter access token: 

6. After filling, it will register with bootstrap.

7. Run client.cpp
    >> g++ client.cpp -o client
    >> ./client

    you can run any no.of clients.

8. Once it fetches data from bootstrap node, then it will connect to each server.
9. while it connects, then it will prompt you to enter filename:
10. if the filename is valid then it will receive the file.
11. again it connects to another server.

12. Run invalid_client.cpp
    >> g++ invalid_client.cpp -o iclient
    >> ./iclient

    It will send a false token to each server.


PLAGIARISM STATEMENT:

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
Roll No: cs22mtech12009.