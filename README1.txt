disk file system
‏Authored by Ahmad Abu Sbeih


‏==Description==
‏The program is a disk file system , this progrem read a number from the user and then the there is functions in main which every function have a number after that the main call functions that we wrote to do something after that the result of the function will be on the screen  there is from 0 to 8 the user can input,

Program DATABASE
evey number we write it into the programm ,  it will be going to a specific function that will do a specific job for example 0 will finish the progrram 1 will print all the files in the disk and more information about it 

‏Functions
there is convert_func that convert  array char to string, substring that return the substring between two char arrays , decToBinary that convert decimal to binary . in fsfile class  there is setfile_size() that set the file size,getfile_size(),Set_IB() that set index blocks for the file,Get_IB() that retuen the index blocks. in filediscreptor class there is getFileName(),getfs() that returns the fs file,getIn_use() that return if the file was opened or not, setIn_use() that set the file or opened or closeed.in fsdisk class there is  fsDisk() the constructor, ~fsDisk() destructor that remove all the pointers and files ,listAll() that prints all the files in the maindir vector , fsFormat() that formate the disk and remove all the files and the data on the disk,CreateFile() that create a file and put it into the map open file discriptor,OpenFile() that open file from maindir that was closed ,CloseFile() that closed file and remove it from open file discriptor map ,WriteToFile() that take a fd and the data that    writes on the disk,DelFile() that delets file form maindir ,ReadFromFile() that read a num of chars from specific file.

‏==Program Files==
‏main.cpp: this file  contains the main programm.

‏==How to compile?==
‏compile: g++  main.cpp -o main
‏run: ./main

‏==Input==
‏a  num  that the user input from 0 to 8

‏==Output:== 
 if num==0 then output will be  (stop the programm)
 if num==1 then output will be string(print all the files in the disk )
 if num==2 then output will be  (format all the disk )
 if num==3 then output will be int (creat file and return 1 or -1) 
 if num==4 then output will be int (open the file and return 1 or -1)
 if num==5 then output will be string(close the file and return the name of it)
 if num==6 then output will be (write string  on disk )
 if num==7 then output will be string  (read num of chars from file )
 if num==8 then output will be int (delete  the file and return 1 or -1)