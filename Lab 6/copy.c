//EOPSY Lab-6
//Keshav Dandeva [302333]

#include <stdio.h>		
#include <stdlib.h>		
#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>	

int copy_read_write(int fd_from,int fd_to);		//function to copy using read and write
int copy_mmap(int fd_from, int fd_to);			//function to copy using mmap() and memcpy()
void Help();									//function to display help messsage
int buf_size;									//to store buffer size

int main (int argc, char *argv[]){

	struct stat sb; 							//to get buffer size from fstat function
	int fd_from = 0, fd_to = 0;					//initialising file descriptor values
	char option;								//to get values from getopt()
	bool mmap = false, help_msg = false;		//bool variables to keep track of selected options

	if (argc == 1){			//to display help message when no arguments are provided
		Help();
		return 0;
	}

	while((option = getopt (argc, argv, "mh")) != -1){ 	//loop to get the options and set flags
		switch (option){								//getopt return -1 when no more options are left
			case 'm':
				mmap = true;
				break;
			case 'h':
				help_msg = true;
				break;
			default:
				help_msg = true;
		}
	}

	if (!help_msg){				//main code execution for copying files calling required functions

		if(access( argv[optind], F_OK ) != -1 ) {				//checking if file exists using access() function
			if ((fd_from = open(argv[optind], O_RDONLY)) < 0){  				//storing the file descriptor of source file in read only mode and          
				printf("\nERROR: Permission to open source file denied.\n");    //returning error if open() returns negative value
				return 1;
			}
		}
		else {
			printf("\nERROR: Source file does not exist.\n"); 	//if access() returns -1 , then file doesn't exist
			return 1;
		}
		
		if ((fd_to = open(argv[optind+1], O_RDWR | O_CREAT | O_TRUNC, 0666)) < 0){  	//storing the file descriptor of dest file in read/write mode, creating mode,
			printf("\nERROR: Permission to write/create destination file denied.\n");   //and removing the already existing data                    
			return 1;
		}
		
		if (mmap){   //executed when mmap is true i.e., -m is given as an argument                                   
			printf("\nThe contents of file: '%s' are being copied to file: '%s' using memory mapping.\n\n",argv[optind],argv[optind+1]);
			copy_mmap(fd_from, fd_to);
		}
		
		else{    //when no options are provided, then using read and write copy function

			if(fstat(fd_from, &sb) < 0)		//checking if fstat returns status of file
				printf("\nERROR: Permission to get status of source file denied.\n");
			buf_size = sb.st_size;     //storing the buffer size
			
			printf("\nThe contents of file: '%s' are being copied to file: '%s' using read/write.\n\n",argv[optind],argv[optind+1]);
			copy_read_write(fd_from, fd_to);
		}
		
		if (close(fd_from)){ 	//closing file descriptor of source file
			printf("\nERROR: Permission to close source file: '%s' denied.\n", argv[optind]);
		}

		if (close(fd_to)){      //closing file descriptor of destination file 
			printf("\nERROR: Permission to close destination file: '%s' denied.\n", argv[optind]);
		}
	}

	else			//displaying the help message for -h argument or any wrong argument
		Help();
	return 0;
}

int copy_read_write(int fd_from, int fd_to){
	
	char buffer[buf_size];         //array to store suffer size
	size_t copy_bytes;             //to get return values from read/write
	size_t nbyte = sizeof(buffer); //storing no of bytes 	
	
	copy_bytes = read(fd_from, &buffer, nbyte);	//reading the source file
	
	if (copy_bytes < 0){     //showing error if read() returns -1                             
		printf("\nERROR: Reading Source File \n");	
	}

	copy_bytes = write(fd_to, &buffer, copy_bytes); //writing to the destinaiton file

	if (copy_bytes < 0){ 	//showing error if write() returns -1
		printf("\nERROR: Writing/Creating Destination File\n");
	}
}

int copy_mmap(int fd_from, int fd_to){

	struct stat fs;			//initialising struct for fstat
	void *src, *dst;		//pointers for soruce and destination files

	if(fstat(fd_from,&fs) < 0){				//if fstat returns negative value, showing error
		printf("\nERROR: Getting status of Source File\n");
		return 1;
	}
	
	ftruncate(fd_to,fs.st_size);		//truncating destination file to length of source file
	
	if ((src = mmap(0, fs.st_size, PROT_READ, MAP_SHARED, fd_from, 0)) == MAP_FAILED){  //storing source pointer value using mmap()     	
		printf("\nERROR: MemoryMapping Source File\n");  //returning error if mapping fails
	}                      
	
	if ((dst = mmap(0, fs.st_size, PROT_WRITE, MAP_SHARED, fd_to, 0)) == MAP_FAILED){   //storing destination pointer value using mmap()     
		printf("\nERROR: MemoryMapping Destination File\n"); //returning error if mapping fails  
	}                      
	
	memcpy(dst, src, fs.st_size);	//copying the file contents from source to destination
	
	if (munmap(src, fs.st_size) < 0){    //deleting the mappings of the source file                                                 
		printf("\nERROR: MunMap of Source File\n"); //displaying error if munmap() return -1
	}

	if (munmap(dst, fs.st_size) < 0){    //deleting the mappings of the destination file                                            
		printf("\nERROR: MunMap of Destination File\n"); //displaying error if munmap() return -1 
	}

}

void Help(){	//To display the help message
	printf("Syntax:\ncopy [OPTION] [SOURCE FILE] [DESTINATION FILE]\n\nDescription:\nThis command is used to copy the contents of one file to another.\n\nUsage:\n./copy <dir/file name...> <dir/file name...>\n./copy [-m] <dir/file name...> <dir/file name...>\n./copy [-h]\n\nDefinitions:\n-m To map files to memory regions with mmap() and copy the file with memcpy()\n-h To display help page (this page)\n\nNOTE: This page is displayed also when incorrect arguements or no arguments are provided.\n");
}