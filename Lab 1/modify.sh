#!/bin/bash

# Initialising variables
bool_recursion="false"

# Help function to display data in terminal
HelpPage(){
cat << help 1>&2

Syntax: 
modify [OPTION]... [DIRECTORY/FILE NAME]...

Description:
This command is used to edit or rename any file/directory recursively or non-recursively in Linux. It can also change the file/directory names to uppercase or lowercase names.

Usage:
modify [-r] [-l|-u] -- <"dir/file name...">
modify [-r] -- <sed pattern> <"dir/file name...">
modify [-h]

Definitions:
-l To lowercase the file/directory name
-u To uppercase the file/directory name
-r To use recursion in editing file/directory names
-h To display help page (this page)

NOTE: To see all the uses of this function in action, you can run modify_examples.sh 

help
}

while [ -n "$1" ] # Gives true if argument is non-empty
do	
  case "$1" in
  -r) 
  bool_recursion="true";; # Bool value too check later if recursion is used or not
  -l) 
  ChangeCase="tr A-Z a-z";; # To change from uppercase to lowercase
  -u) 
  ChangeCase="tr a-z A-Z";; # To change from lowercase to uppercase
  -h) 
  HelpPage;; # To display the help page
  --) 
  shift  
  break;;
   
  *) 
  echo "Invalid argument! Please use -h and see the available options.";;   
  
  esac
  shift

done

# Main Loop
while [ -n "$1" ] # Gives true if argument is non-empty
do
	#For sed command
	if [ "$1" = "sed" ]			
	then 

		for ((i=3; i<=$#; ++i))
		do 							# Loop from 3rd argument as we used shift command earlier and 2nd argument is the pattern
			FileName=$(echo "${!i}" | sed  "s/.*\///")
			DirectoryName=$(echo "${!i}" | grep -o '.*\/')    # -o to get only the matching string
			NewName=`echo "$FileName" | sed $2`
			
			if [ "$bool_recursion" = "true" ] # -r is an argument
			then

			for file in $(find . -name *"$FileName")
				do								
				mv $file $(echo "$file" | sed "s/"$FileName"/"$NewName"/")
				done 

			else 							# -r is not an argument
				mv -- "${!i}" "$NewName"				
				mv -- "$NewName" "$DirectoryName"
			fi 
			
		done
		break;
	fi

	#For other options (-l, -u)

	FileName=$(echo "$1" | sed  "s/.*\///")         	    
	FileNameOriginal=$(echo "$1" | sed  "s/.*\///")               
	NewName=""					   	       					

	for name in "${FileName}"
	do 			       										
		FileName=$(echo "$name" | $ChangeCase)
		NewName="$NewName$FileName"
	done
	DirectoryName=$(echo "$1" | grep -o '.*\/')

	if [ "$bool_recursion" = "true" ]					# -r is an argument
	then
		for file in $(find . -name *"$FileNameOriginal")
			do	       											
			mv $file $(echo "$file" | sed "s/"$FileNameOriginal"/"$NewName"/")
			done
		
		else 												# -r is not an argument
			mv -- "$1" "$NewName"								# Using -- to prevent naming conflicts
			mv -- "$NewName" "$DirectoryName"
	fi
		
shift
done
