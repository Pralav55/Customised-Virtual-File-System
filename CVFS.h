#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<iostream>
using namespace std;

#define MAXINODE 50

#define READ 1
#define WRITE 2

#define MAXFILESIZE 2048

#define REGULAR 1
#define SPECIAL 2

#define START 0
#define CURRENT 1
#define END 2

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct superblock
{
	int TotalInodes;
	int FreeInode;
	
}SUPERBLOCK, *PSUPERBLOCK;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct inode
{
	char FileName[50];
	int InodeNumber;
	int FileSize;
	int FileActualSize;
	int FileType;
	char *Buffer;
	int LinkCount;
	int ReferenceCount;
	int permission;
	struct inode *next;
	 
}INODE,*PINODE,**PPINODE;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct filetable
{
	int readoffset;
	int writeoffset;
	int count;
	int mode;
  	PINODE ptrinode;
  	
}FILETABLE,*PFILETABLE;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct ufdt
{
 	 PFILETABLE ptrfiletable;
 	 
}UFDT;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
UFDT UFDTArr[50];
SUPERBLOCK SUPERBLOCKobj;
PINODE head = NULL;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void man(char *name)
{
	if(name == NULL)
  	{
   	 	return;
 	 }
  
  	if(strcmp(name,"create")== 0)
  	{
    		printf("\tDescription \t: Used to  create new regular file.\n");
    		printf("\tUsage \t\t: create File_name Permission\n");
  	}
  	else if(strcmp(name,"read")==0)
  	{
    		printf("\tDescription \t: Used to read data from regular file.\n");
    		printf("\tUsage \t\t: read File_name No_of_Bytes_to_Read\n");
  	}
  	else if(strcmp(name,"write")==0)
  	{
 		printf("\tDescription \t: Used to write into regular file.\n");
    		printf("\tUsage \t\t: write File_name\n After this enter the data that we want to write\n");
  	}
  	else if(strcmp(name,"ls")==0)
  	{
    		printf("\tDescription \t: Used to list all the information of files.\n");
    		printf("\tUsage \t\t: ls\n");
  	}
  	else if(strcmp(name,"stat") == 0)
 	{
    		printf("\tDescriptor \t: Used to display information of file.\n");
    		printf("\tUsage \t\t: stat File_name\n");
  	}
  	else if(strcmp(name,"fstat")==0)
  	{
    		printf("\tDescriptor \t: Used to display information of file.\n");
    		printf("\tUsage \t\t: stat File_Descriptor\n");
  	}
  	else if(strcmp(name,"truncate")==0)
  	{
  		  printf("\tDescriptor \t: Used to remove data from the file.\n");
 		   printf("\tUsage \t\t: truncate File_name\n");
  	}
  	else if(strcmp(name,"open")==0)
  	{
    		printf("\tDescriptor \t: Used to open existing file.\n");
    		printf("\tUsage \t\t: open File_name mode\n");
  	}
  	else if(strcmp(name,"close")==0)
  	{
    		printf("\tDescriptor \t: Used to close opened file.\n");
   	 	printf("\tUsage \t\t: close File_name\n");
  	}
  	else if(strcmp(name,"closeall")==0)
  	{
    		printf("\tDescriptor \t: Used to close all opened file.\n");
    		printf("\tUsage \t\t: closeall\n");
  	}
  	else if(strcmp(name,"lseek")==0)
  	{
    		printf("\tDescriptor \t: Used to change file offset.\n");
    		printf("\tUsage \t\t: lseek File_name ChangeInOffset StartPoint\n");
  	}
  	else if(strcmp(name,"rm")==0)
  	{
    		printf("\tDescription \t: Used to delete the file.\n");
    		printf("\tUsage \t\t: rm File_Name\n");
  	}
  	else if(strcmp(name,"exit")==0)
  	{
    		printf("\tDescription \t: Used to exit from the virtual file system.\n");
    		printf("\tUsage \t\t: exit\n");
  	}
  	else
  	{
    		printf("\tError \t: No manual entry available.\n");
 	}  
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void DisplayHelp()
{
  	printf("\t -----------------------------------------------------------------------\n");
  	printf("\t| ls \t\t: To List out all the file\t\t\t\t|\n");
  	printf("\t| man \t\t: It describe the usage and Descriptor of the command\t|\n");
  	printf("\t| create \t: To create a file \t\t\t\t\t|\n");
 	printf("\t| open \t\t: to open the file\t\t\t\t\t|\n");
  	printf("\t| close \t: to close the file\t\t\t\t\t|\n");
  	printf("\t| closeall \t: To close all opened file\t\t\t\t|\n");
  	printf("\t| read \t\t: To read all the contens from file\t\t\t|\n");
  	printf("\t| write \t: To write contents into file\t\t\t\t|\n");
  	printf("\t| exit \t\t: To terminate file system\t\t\t\t|\n");
  	printf("\t| stat \t\t: To display information of file using name\t\t|\n");
  	printf("\t| fstat \t: To display information of file using file descriptor\t|\n");
  	printf("\t| truncate \t: To remove all data from file\t\t\t\t|\n");
  	printf("\t| rm \t\t: To delet the file\t\t\t\t\t|\n");
  	printf("\t -----------------------------------------------------------------------");
}	

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int GetFDFromName(char *name)
{
  	int i=0;
  	
  	while(i<50)
  	{
  		  if(UFDTArr[i].ptrfiletable != NULL)
  		  {
  			    if(strcmp((UFDTArr[i].ptrfiletable->ptrinode->FileName),name)==0)
  			    {
  				      break;
  			    }
  		  }
  		  i++;
  	}
  
  	if(i==50)
  	{
  		  return -1;
  	}
  	else
  	{
  		  return i;
  	}
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
PINODE Get_Inode(char *name)
{
  	PINODE temp = head;
  	int i=0;
  
  	if(name == NULL)
  	{
   		 return NULL;
  	}
  
  	while(temp != NULL)
  	{
    		if(strcmp(name,temp->FileName)==0)
    		{
      			break;
    		}
    		temp = temp->next;
  	}	
  	return temp;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CreateDILB()
{
  	int i=1;
  	PINODE newn = NULL;
	PINODE temp = head;
  
  	while(i<= MAXINODE)
  	{
    		newn = (PINODE)malloc(sizeof(INODE));
    		newn->LinkCount =0;
    		newn->ReferenceCount = 0;
    		newn->FileType =0;
    		newn->FileSize =0;
    
    		newn->Buffer = NULL;
    		newn->next = NULL;
    
    		newn->InodeNumber = i;
    
    		if(temp == NULL)
   		{
      			head = newn;
      			temp = head;
    		}	
    		else
    		{
      			temp->next = newn;
      			temp = temp->next;
    		}
    		i++;
  	}
  	printf("DILB created successfully\n");
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void InitialiseSuperBlock()
{
  	int i=0;
  	while(i<MAXINODE)
  	{
    		UFDTArr[i].ptrfiletable = NULL;
    		i++;
  	}
  
  	SUPERBLOCKobj.TotalInodes = MAXINODE;
  	SUPERBLOCKobj.FreeInode = MAXINODE;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int CreateFile(char *name,int permission)
{
  	int i=0;
  	PINODE temp=head;
  
  	if((name ==NULL) || (permission == 0) || (permission>3))
  	{
    		return -1;
  	}
  	if(SUPERBLOCKobj.FreeInode == 0)
  	{
    		return -2;
  	}
  	(SUPERBLOCKobj.FreeInode)--;
  
  	if(Get_Inode(name) != NULL)
  	{
    		return -3;
  	}
  	
 	while(temp != NULL)
  	{
    		if(temp->FileType ==0)
    		{
    	  		break;
    		}
   		 temp= temp->next;
  	}
  
  	while(i<50)
  	{
    		if(UFDTArr[i].ptrfiletable == NULL)
   		{
      			break;
    		}
    		i++;
  	}
  
  	UFDTArr[i].ptrfiletable = (PFILETABLE)malloc(sizeof(FILETABLE));
  
  	UFDTArr[i].ptrfiletable->count = 1;
  	UFDTArr[i].ptrfiletable->mode = permission;
  	UFDTArr[i].ptrfiletable->readoffset =0;
  	UFDTArr[i].ptrfiletable->writeoffset =0;
  	UFDTArr[i].ptrfiletable->ptrinode = temp;
  
 	strcpy(UFDTArr[i].ptrfiletable->ptrinode->FileName,name);
  
  	UFDTArr[i].ptrfiletable->ptrinode->FileType = REGULAR;
 	UFDTArr[i].ptrfiletable->ptrinode->ReferenceCount =1;
  	UFDTArr[i].ptrfiletable->ptrinode->LinkCount = 1;
  	UFDTArr[i].ptrfiletable->ptrinode->FileSize = MAXFILESIZE;
  	UFDTArr[i].ptrfiletable->ptrinode->FileActualSize = 0;
  	UFDTArr[i].ptrfiletable->ptrinode->permission = permission;
  	UFDTArr[i].ptrfiletable->ptrinode->Buffer = (char*)malloc(MAXFILESIZE);
  
 	return i;
} 

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int rm_File(char *name)
{
  	int fd = 0;
  
  	fd = GetFDFromName(name);
  	if(fd == -1)
  	{
    		return -1;
  	}
    	(UFDTArr[fd].ptrfiletable->ptrinode->LinkCount)--;
    
    	if(UFDTArr[fd].ptrfiletable->ptrinode->LinkCount == 0)
    	{
       		 UFDTArr[fd].ptrfiletable->ptrinode->FileType = 0;
        	free(UFDTArr[fd].ptrfiletable);
    	}
    	UFDTArr[fd].ptrfiletable = NULL;
    	(SUPERBLOCKobj.FreeInode)++;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int ReadFile(int fd,char *arr,int isize)
{
  	int read_size = 0;
  
  	if(UFDTArr[fd].ptrfiletable == NULL)
  	{
    		return -1;
  	}
  
  	if(UFDTArr[fd].ptrfiletable->mode != READ && UFDTArr[fd].ptrfiletable->mode != READ+WRITE)
  	{
    		return -2;
  	}
  
  	if(UFDTArr[fd].ptrfiletable->ptrinode->permission != READ && UFDTArr[fd].ptrfiletable->ptrinode->permission != READ+WRITE)
  	{
    		return -2;
  	}
  
  	if(UFDTArr[fd].ptrfiletable->readoffset == UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize)
  	{
    		return -3;
  	}
  
  	if(UFDTArr[fd].ptrfiletable->ptrinode->FileType != REGULAR)
  	{
    		return -4;
  	}
  
  	read_size = (UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize)-(UFDTArr[fd].ptrfiletable->readoffset);
  
  	if(read_size<isize)
  	{	
    		strncpy(arr,(UFDTArr[fd].ptrfiletable->ptrinode->Buffer)+(UFDTArr[fd].ptrfiletable->readoffset),read_size);
    
    		UFDTArr[fd].ptrfiletable->readoffset = UFDTArr[fd].ptrfiletable->readoffset + read_size; 
  	}
  	else
  	{
    		strncpy(arr,(UFDTArr[fd].ptrfiletable->ptrinode->Buffer) + (UFDTArr[fd].ptrfiletable->readoffset),isize);
    		(UFDTArr[fd].ptrfiletable->readoffset) = (UFDTArr[fd].ptrfiletable->readoffset) + isize;
  	}
  
  	return isize;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int WriteFile(int fd,char *arr,int isize)
{
    	if(((UFDTArr[fd].ptrfiletable->mode) != WRITE) && ((UFDTArr[fd].ptrfiletable->mode) != READ+WRITE))
    	{
       		return -1;
    	}
    	if(((UFDTArr[fd].ptrfiletable->ptrinode->permission) != WRITE) && ((UFDTArr[fd].ptrfiletable->ptrinode->permission) != READ+WRITE))
    	{
       		 return -1;
    	}
    
   	if((UFDTArr[fd].ptrfiletable->writeoffset) == MAXFILESIZE)
    	{
       		return -2;
    	}
    
    	if((UFDTArr[fd].ptrfiletable->ptrinode->FileType) != REGULAR)
    	{
       		return -3;
    	}
    
    	strncpy((UFDTArr[fd].ptrfiletable->ptrinode->Buffer) + (UFDTArr[fd].ptrfiletable->writeoffset),arr,isize);
    
    	(UFDTArr[fd].ptrfiletable->writeoffset) = (UFDTArr[fd].ptrfiletable->writeoffset) + isize;
    
    	(UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize) = (UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize)+ isize;
    
    	return isize;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int OpenFile(char *name,int mode)
{
  	int i=0;
  	PINODE temp = NULL;
  
  	if(name == NULL || mode <=0)
  	{
    		return -1;
  	}
  
  	temp = Get_Inode(name);
  	if(temp == NULL)
  	{
    		return -2;
  	}	
  
  	if(temp->permission < mode)
  	{
    		return -3;
  	}
  
  	while(i<50)
  	{
    		if(UFDTArr[i].ptrfiletable == NULL)
    		{
      			break;
    		}
    		i++;
  	}
  
  	UFDTArr[i].ptrfiletable = (PFILETABLE)malloc(sizeof(FILETABLE));
  	if(UFDTArr[i].ptrfiletable == NULL)
  	{
    		return -1;
  	}
  	UFDTArr[i].ptrfiletable ->count = 1;
  	UFDTArr[i].ptrfiletable ->mode = mode;
  
  	if(mode = READ+WRITE)
  	{
     		UFDTArr[i].ptrfiletable ->readoffset = 0;
     		UFDTArr[i].ptrfiletable ->writeoffset = 0;
 	}
  	else if(mode == READ)
  	{
    		UFDTArr[i].ptrfiletable->readoffset =0;
  	}
  	else if(mode == WRITE)
  	{
    		UFDTArr[i].ptrfiletable->writeoffset = 0;
  	}
  	UFDTArr[i].ptrfiletable->ptrinode = temp;
  	(UFDTArr[i].ptrfiletable->ptrinode->ReferenceCount)++;
  
  	return i;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CloseFileByName(int fd)
{
  	UFDTArr[fd].ptrfiletable->readoffset = 0;
 	UFDTArr[fd].ptrfiletable->writeoffset = 0;
  	(UFDTArr[fd].ptrfiletable->ptrinode->ReferenceCount)--;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int CloseFileByName(char *name)
{
   	int i=0;
   	i= GetFDFromName(name);
   	if(i== -1)
   	{
     		return -1;
   	}
   	UFDTArr[i].ptrfiletable->readoffset = 0;
   	UFDTArr[i].ptrfiletable->writeoffset = 0;
   	(UFDTArr[i].ptrfiletable->ptrinode->ReferenceCount)--;
   
   	return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CloseAllFile()
{
  	int i=0;
  	while(i<50)
  	{
    		if(UFDTArr[i].ptrfiletable != NULL)
    		{
      			UFDTArr[i].ptrfiletable->readoffset =0;
      			UFDTArr[i].ptrfiletable->writeoffset = 0;
      			(UFDTArr[i].ptrfiletable->ptrinode->ReferenceCount)--;
      			break;
    		}
    		i++;
  	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int LseekFile(int fd,int size,int from)
{
  	if((fd<0) || (from >2))
  	{
     		return -1;
 	}
  
  	if((UFDTArr[fd].ptrfiletable->mode == READ) || (UFDTArr[fd].ptrfiletable->mode == READ+WRITE))
  	{
    		if(from == CURRENT)
    		{
      			if(((UFDTArr[fd].ptrfiletable->readoffset) + size) > UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize)
      			{
        			return -1;
      			}
      			if(((UFDTArr[fd].ptrfiletable->readoffset)+size)<0)
      			{
         			return -1;
      			}
      			(UFDTArr[fd].ptrfiletable->readoffset) = (UFDTArr[fd].ptrfiletable->readoffset) + size;
    		}
    		else if(from == START)
    		{
       			if(size>(UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize))
       			{
         			return -1;
       			}
       			if(size < 0)
       			{
         			return -1;
       			}
       			(UFDTArr[fd].ptrfiletable->readoffset) = size;
    		}	
    
    		else if(from == END)
   		{
      			if((UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize) + size > MAXFILESIZE)
      			{
        			return -1;
      			}
      			if(((UFDTArr[fd].ptrfiletable->readoffset)+ size) < 0)
      			{
        			return -1;
      			}
      			(UFDTArr[fd].ptrfiletable->readoffset) = (UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize) + size;
    		}
  	}
  	else if(UFDTArr[fd].ptrfiletable->mode == WRITE)
 	{
    		if(from == CURRENT)
   	 	{
       			if(((UFDTArr[fd].ptrfiletable ->writeoffset) + size) > MAXFILESIZE)
       			{
         			return -1;
       			}
       			if(((UFDTArr[fd].ptrfiletable->writeoffset)+ size) < 0)
       			{
          			return -1;
       			}
       			if(((UFDTArr[fd].ptrfiletable->writeoffset)+ size) > (UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize))
       			{
          			(UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize) = (UFDTArr[fd].ptrfiletable->writeoffset)+ size;
       			}
       			(UFDTArr[fd].ptrfiletable->writeoffset) = (UFDTArr[fd].ptrfiletable->writeoffset) + size;
    		}
    		else if(from == START)
    		{
       			if(size >MAXFILESIZE)
       			{
          			return -1;
       			}
       			if(size<0)
       			{
         			return -1;
       			}	
       			if(size > (UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize))
       			{
          			(UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize) = size; 
       			}
       			(UFDTArr[fd].ptrfiletable->writeoffset) = size;
    		}
    		else if(from == END)
    		{
      			if((UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize) + size > MAXFILESIZE)
      			{
         			return -1;
      			}
      			if(((UFDTArr[fd].ptrfiletable->writeoffset) + size) < 0)
      			{
         			return -1;
      			}
      			(UFDTArr[fd].ptrfiletable->writeoffset) = (UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize) + size;
    		}
  	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Is_file()
{
   	int i=0;
   	PINODE temp = head;
   
   	if(SUPERBLOCKobj.FreeInode == MAXINODE)
   	{
     		printf("Error : There are no files\n");
     		return;
   	}
   	
   	printf("\n\t-----------------------------------------------------------------------\n");
   	printf("\t File Name \t Inode Number \t File Size \t Link Count \n");
   	printf("\t-----------------------------------------------------------------------\n");
   
   	while(temp != NULL)
   	{
      		if(temp->FileType != 0)
      		{
          		printf("\t %s\t\t %d \t\t %d \t\t %d\n",temp->FileName,temp->InodeNumber,temp->FileActualSize,temp->LinkCount);
      		}
      		temp = temp->next;
   	}
   	printf("\t-----------------------------------------------------------------------\n");
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int fstat_file(int fd)
{
  	PINODE temp = head;
  	int i=0;
  
  	if(fd < 0)
  	{
    		return -1;
  	}
  	if(UFDTArr[fd].ptrfiletable == NULL)
  	{
    		return -2;
 	}
 	 temp = UFDTArr[fd].ptrfiletable->ptrinode;
  
  	printf("\n\t ------------Statistical Information about file------------\n");
  	printf("\t  File Name \t\t: %s\n",temp->FileName);
  	printf("\t  Inode Number \t\t: %d\n",temp->InodeNumber);
  	printf("\t  File size \t\t: %d\n",temp->FileSize);
  	printf("\t  Actual File Size \t: %d\n",temp->FileActualSize);
  	printf("\t  Link Count \t\t: %d\n",temp->LinkCount);
  	printf("\t  Reference Count \t: %d\n",temp->ReferenceCount);
  
  	if(temp->permission == 1)
  	{
   		 printf("\t  File Permission \t: Read Only\n");
  	}
  	else if(temp ->permission == 2)
  	{
     		printf("\t  File Permission \t: Write only\n");
  	}
  	else if(temp ->permission ==3)
  	{
      		printf("\t  File Permission \t: Read and Write \n");
  	}
    	printf("\t ----------------------------------------------------------\n\n");
    
   	return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int stat_file(char *name)
{
    	PINODE temp = head;
    	int i =0;
    
    	if(name == NULL)
    	{
    	   	 return -1;
   	}
    
    	while(temp != NULL)
    	{
       		if(strcmp(name,temp->FileName) == 0)
       		{
          		break;
       		}
       		temp = temp->next;
    	}
    
    	if(temp == NULL)
    	{
      		 return -2;
    	}
    
    	printf("\n\t ------------Statistical Information about file------------\n");
  	printf("\t  File Name \t\t: %s\n",temp->FileName);
  	printf("\t  Inode Number \t\t: %d\n",temp->InodeNumber);
  	printf("\t  File size \t\t: %d\n",temp->FileSize);
  	printf("\t  Actual File Size \t: %d\n",temp->FileActualSize);
  	printf("\t  Link Count \t\t: %d\n",temp->LinkCount);
  	printf("\t  Reference Count \t: %d\n",temp->ReferenceCount);
  
  	if(temp->permission == 1)
  	{
    		printf("\t File Permission : Read Only\n");
  	}
  	else if(temp ->permission == 2)
  	{
     		printf("\t File Permission : Write only\n");
  	}
  	else if(temp ->permission ==3)
  	{
      		printf("\t File Permission : Read and Write \n");
  	}
   	printf("\t ----------------------------------------------------------\n\n");
    
   	return 0;  
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int truncate_file(char *name)
{
  	int fd = GetFDFromName(name);
  	if(fd == -1)
  	{
  	  	return -1;
 	}
  	memset(UFDTArr[fd].ptrfiletable->ptrinode->Buffer,0,1024);
  	UFDTArr[fd].ptrfiletable->readoffset = 0;
  	UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize = 0;
}






