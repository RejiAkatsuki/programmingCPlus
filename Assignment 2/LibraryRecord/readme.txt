Assignment 2
COSC 2131 Programming with C++

Title of the program: Library Record
Title of the Assignment: Assignment 2
Assignment number: 2
Lecturer: Mr. Ashok Kumar Veerasamy
Date Due: December 24th 2013

Group member:
S3372794 Nguyen Manh Khoi
S3342133 Phan Thanh San


Introduction:
This report is about the development of the program Library Record.
The program is developed for librarian for keep track of the library's multimedia collection and it's service system.


How To Run: 
The Qmake support the auto generate of make file for the program, thus simplify the process of makefile. 
To run the program simply follow the following step:

- install Qt Creator: check the following link for more information
http://qt-project.org/downloads

- after successful install Qt creator, simply run Qt Creator

- choose open project then select LibraryRecord.pro in folder LibraryRecord. 

- after that Simplt press RUN button in the lower left corner of your screen.

Note: This program support multiple platform including Mac OS, Linux and Window. the process of installation is similar in any platform. <br/>

Source code:
The source code of the program is stored inside the folder call LibraryRecord which contain all neccessary file to run the program.


Implemented C++ feature:
The program implemeted C++ feature to better its perfomance. Qt project provide a comfortable way to impletement C++ and used it to develop a useable GUI for the program.

- toInt() which is used in the algorithm for calculating late return date of borrower. Qt project provide a flexible way to implement this feature into the program.
without the use of toInt(), it would be difficult to calculate the late return date which used to determine if the borrwer return the item late or not.

- toInt() is used to change data stored in database with type string to Integer type which is used to determine the neccessary location of the data to fetch and display in 
correct table collumn

-compare() is used for comparision between 2 string which is used to determine if the user want to show the overdue or not. this is implemented inside the program as a filter
function incase user want to check the overdue of the borrower.

-Qt provide an easy way to read and write file to export report for librarian. this is found in the main window of the program as the feature is implemented in the
generate report button. Qfile and QTextstream was used in this case since they are standard library of Qt project and support c++ function.


Algorithm:
one of the algorithm which used in the program is the due date calculation. The idea is simple: by taking the day an item is borrowed and take in cosider of how many days 
the item is allowed to be borrow. then compare it with the return day of the item to determine whether the item is late return or not. 


Data Structure: 
At first the program is developed with the intention to use List as a container for data. 
However, this was removed and replaced by using a direct access to the SQL lite database.
By doing so, the program would run more faster and more accurate since the chance data is conflict have been removed. If the program does not have direct access to the database, 
this means the program will have to store and access data through the container-which is located in client, before updating it to server. This could cost major conflict of data
which might lead to server crash. 
Moreover, SQL Lite provide a safe and reliable way to connect to database which have reduce the change of lost connection between database and client. this provide an improvement
in program perfomance and flexiblity. 
Thus, the program is developed with the direct access to SQL Lite database. Any function of performance such as display collection, store record borrower will be save to the database
in order to update the latest change in database record.


Reference:

-QT Project Hosting, Qt Project , viewed December 12th 2013, URL <http://qt-project.org/> 

-Flavio Ferreira, How to Read and WRite in XML Files with Qt/C++, October 29th 2010, Viewed December 12th 2013, URL <http://community.embeddedlab.org/technical-articles/qt/qt-posts/howtoreadandwriteinxmlfileswithqtc>

-QtBros61, Writing to a Text-file, Qt Center, April 7th 2010, Viewed December 14th 2013, URl<http://www.qtcentre.org/threads/29622-writing-to-a-text-file>

-Zl2k, How to Brose image in thumbnail view, Qt center, September 2nd 2008, Viewed December 15th 2013, URl <http://www.qtcentre.org/threads/15739-how-to-browse-images-in-thumbnail-view>

-Stack Overflow, How to display an Image, StackOverflow, Viewed December 15th 2013, URL <http://stackoverflow.com/questions/11980274/first-time-using-qt-how-to-display-an-image>

- pmlz, How to highlight a row, Qt project, April 4th 2013, viewed December 15th 2013, URL<http://qt-project.org/forums/viewthread/16032>





