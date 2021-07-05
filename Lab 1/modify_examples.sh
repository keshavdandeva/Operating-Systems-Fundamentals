#!/bin/bash

HelpPage(){
cat << help 1>&2
This script is developed to lead as the tester of the modify script through the typical, uncommon and incorrect scenarios of usage of the modify script.
The following are the test cases. To run them use: ./modify_examples.sh -[test case index]

Testcase c)
To create a directory, subdirectory and files.

Testcase d)
To delete the created directory and its contents.

Testcase 1) 
To uppercase a file
Command: ./modify.sh -u -- \"Examples/randomfile_a.txt\"

Testcase 2) 
To lowercase a file
Command: ./modify.sh -l -- \"Examples/RANDOMFILE_B.TXT\"

Testcase 3) 
Two files, randomfile_c.txt, random_file_c_2.txt are provided, and the user desires to replace \"random\" in the
filenames to \"ordered\"
Command: ./modify.sh -- sed 's/random/ordered/' \"Examples/randomfile_c.txt\" \"Examples/random_file_c_2.txt\" 

Testcase 4) 
To uppercase a file containing space,question mark and other symbols.
Command: ./modify.sh -u -- \"Examples/random_d >?< x:a-z.txt\"

Testcase 5)
To rename all files with given sed pattern (using recursion)
Command:./modify.sh -r -- sed 's/txt/doc/' \"Examples/ex1file.txt\"

Testcase 6)
Incorrect sed pattern.
Command: ./modify.sh -- sed '/random/ordered/' \"Examples/randomfile_c.txt\"
*ERROR* 

Testcase 7)
To uppercase all files in the folder (using recursion)
Command: ./modify.sh -r -u -- \"Examples/file_ex.txt\"

Testcase 8)
To lowercase all files in the folder (using recursion)
Command: ./modify.sh -r -l -- \"Examples/FILE_EX_UP.PNG\"

Testcase *)
Displays this page.
help
}

createFiles(){
mkdir "Examples"
cd Examples
touch "randomfile_a.txt" "RANDOMFILE_B.TXT" "randomfile_c.txt" "random_file_c_2.txt" "random_d >?< x:a-z.txt"
mkdir "ex1" "ex2"
cd ex1
touch "ex1file.txt" "file_ex.txt" "FILE_EX_UP.PNG"
cd ..
cd ex2
touch "ex1file.txt" "file_ex.txt" "FILE_EX_UP.PNG"
cd ..
cd ..
}

deleteFiles(){
	rm -rf Examples
}

while [ -n "$1" ];do	
  case "$1" in
  -c)createFiles;;
  -d)deleteFiles;;
  -1)./modify.sh -u --  "Examples/randomfile_a.txt";;
  -2)./modify.sh -l --  "Examples/RANDOMFILE_B.TXT";;
  -3)./modify.sh -- sed 's/random/ordered/' "Examples/randomfile_c.txt" "Examples/random_file_c_2.txt";;
  -4)./modify.sh -u --  "Examples/random_d >?< x:a-z.txt";;
  -5)./modify.sh -r -- sed 's/txt/doc/' "Examples/ex1file.txt";;
  -6)./modify.sh -- sed '/random/ordered/' "Examples/randomfile_c.txt";;
  -7)./modify.sh -r -u -- "Examples/file_ex.txt";;
  -8)./modify.sh -r -l -- "Examples/FILE_EX_UP.PNG";;

   *) HelpPage;;
   
   esac

   shift
done