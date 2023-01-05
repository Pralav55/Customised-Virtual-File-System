#include"CVFS.h"

int main()
{
  	 char *ptr = NULL;
   	int ret = 0,fd=0,count=0;
   	char command[4][80],str[80],arr[1024];
   
   	InitialiseSuperBlock();
   	CreateDILB();
   
  	 while(1)
  	 {
        	fflush(stdin);
        	strcpy(str," ");
        
        	printf("\nVirtual File System : > ");
        	
        	fgets(str,80,stdin);   //scanf("%[^'\n']s",str);
        		
        	count = sscanf(str,"%s %s %s %s",command[0],command[1],command[2],command[3]);
        	
        	if(count == 1)
        	{
        	    	if(strcmp(command[0],"ls")==0)
        	    	{
        		       Is_file();
        	    	}	
        	    	else if(strcmp(command[0],"closeall") == 0)
        	    	{
        			      CloseAllFile();
        			      printf("All files closed successfully\n");
        			      continue;
        	    	}
        	    	else if(strcmp(command[0],"clear") == 0)
        	    	{
        		       system("cls");
        		       continue;
        		}
        	  	else if(strcmp(command[0],"help") == 0)
        	    	{
        		       DisplayHelp();
        		       continue;
        		}
            		else if(strcmp(command[0],"exit") == 0)
            		{
             			 printf("Terminating the Marvellous Virtual File System\n");
            			 break;
            		}	
            		else
            		{
              			printf("\nERROR : Command not Found !!!\n");
              			continue;
            		}
        	}
        	else if(count ==2)
        	{
        		    if(strcmp(command[0],"stat")==0)
        		    {
        			       ret = stat_file(command[1]);
        			       if(ret == -1)
        			       {
        				          printf("ERROR : Incorrect Parameters\n");
        			       }
        			       if(ret ==-2)
        			       {
        				          printf("ERROR : There are no such file\n");
        			       }
        			       continue;
        		    }
        		    else if(strcmp(command[0],"fstat")==0)
        		    {
        			       ret = fstat_file(atoi (command[1]));
        			       if(ret == -1)
        			       {
        				          printf("ERROR : Incorrect Parameters\n");
        			       }
        			       if(ret ==-2)
        			       {
        				          printf("ERROR : There are no such file\n");
        			       }
        			       continue;
        		    }
        		    else if(strcmp(command[0],"close")== 0)
        		    {
        			       ret = CloseFileByName(command[1]);
        			       if(ret ==-1)
        			       {
        				          printf("ERROR : There is no such file\n");
        			       }
        			       continue;
        		    }
        		    else if(strcmp(command[0],"rm")== 0)
        		    {
          			     	ret = rm_File(command[1]);
          			     	if(ret ==-1)
          	     			{
          	        			printf("ERROR : There is no such file\n");
               				}	
               				continue;
			   }	
			   else if(strcmp(command[0],"man")== 0)
            		   {
               			man(command[1]);
           	      	   }
           		   else if(strcmp(command[0],"write")==0)
          	  		{
               				fd = GetFDFromName(command[1]);
               				if(fd == -1)
               				{
                   				printf("ERROR : Incorrect Parameter\n");
                  		 		continue;
               				}
              		 		printf("Enter the Data : \n");
               				scanf("%[^\n]",arr);
               
              		 		ret = strlen(arr);
              		 		if(ret == 0)
              		 		{
              			    		printf("ERROR : Incorrect Parameter\n");
              			    		continue;
              		 		}
              				 ret = WriteFile(fd,arr,ret);
               
              		 		if(ret == -1)
              		 		{
              			   		 printf("ERROR : Permission Denied\n");
              		 		}
              		 		if(ret == -2)
              		 		{
              			    		printf("ERROR : There are no sufficient memory to write\n");
              		 		}
              		 		if(ret == -3)
              		 		{
              			    		printf("ERROR : It is not regular file\n");
              		 		}
            			}		
            		else if(strcmp(command[0],"truncate") == 0)
            		{
            	   		ret = truncate_file(command[1]);
           	    		if(ret == -1)
           	    		{	
                   			printf("ERROR : Incorrect Parameter\n");
                   		}
                   		else
                   		{
                  			printf("\nERROR : Command not found !!!\n");
                  			continue;
               	   		}
            		}		
            		else
            		{
           	    		printf("\nERROR : Command Not Found !!! \n");
           		 }
		}
        
        	else if(count == 3)
        	{
        		   if(strcmp(command[0],"create")==0)
        		   {
        		      ret = CreateFile(command[1],atoi(command[2]));
        		      if(ret >= 0)
        		      {
        			         printf("\tFile is Successfully created with File Descriptor : %d\n",ret);
        		      }   
        		      if(ret == -1)
        		      {
        			         printf("\tERROR : Incorrect Parameter\n ");
        		      }
        		      if(ret == -2)
        		      {
        			         printf("\tERROR : There is no inodes\n ");
        		      }
        		      if(ret == -3)
        		      {
        			         printf("\tERROR : File Already Exists\n");
        		      }
        		      if(ret == -4)
        		      {
        			         printf("\tERROR : Memory allocation failure\n");
        		      }
        		      continue;
        		   }
        		   if(strcmp(command[0],"open")==0)
        		   {
        			      ret = OpenFile(command[1],atoi(command[2]));
        			      if(ret >= 0)
        			      {
        				         printf("\tFile is Successfully created with File Descriptor : %d\n",ret);
        			      }   
        			      if(ret == -1)
        			      {
        				         printf("\tERROR : Incorrect Parameter\n ");
        			      }
        			      if(ret == -2)
        			      {
        				         printf("\tERROR : File not Present\n ");
        			      }
        			      if(ret == -3)
        			      {
        				         printf("\tERROR : Permission Denied\n");
        			      }
        			      continue;
       			   }
       			   else if(strcmp(command[0],"read") == 0)
        		   {
      			        fd = GetFDFromName(command[1]);
      			        if(fd == -1)
     			        {
                			 printf("ERROR : Incorrect Parameter\n");
                			 continue;
  	  		        }
             			 ptr = (char *)malloc(sizeof(atoi(command[2])) + 1);
              	
            			if(ptr == NULL)
           			{
           			       printf("ERROR : Memory allocation failure\n");
           			       continue;
           			   }
           			   ret = ReadFile(fd,ptr,atoi(command[2]));
           			   if(ret == -1)
           			   {
           				      printf("ERROR : File not Existing");
           			   }
           			   if(ret == -2)
           			   {
           				      printf("ERROR : Permission Denied\n ");
           			   }
           			   if(ret == -3)
           			   {
           					printf("ERROR : Reached at end of File\n");
           			   }
           			   if(ret == -4)
           			   {
           				      printf("ERROR : It is not regular file\n");
           			   }
           			   if(ret > 0)
           			   {
           				      write(2,ptr,ret);
           			   }
           			   continue;
           		}
           		else
           		{
           			   printf("\nERROR : Command Not Found !!!\n");
           			   continue;
           		}
           	
        	}	
        	else if(count == 4)
        	{	
        			    if(strcmp(command[0],"lseek") == 0)
        			    {
        				       fd = GetFDFromName(command[1]);
        				       if(fd == -1)
        					{	
        					          printf("ERROR : Incorrect Parameter\n");
        					          continue;
        			       		}	
        			       		ret = LseekFile(fd,atoi(command[2]),atoi(command[3]));
               	
        				       if(ret == -1)
        				       {
        					          printf("ERROR : Unable to perform lseek\n");
        				       }
        			    }
        			    else
        			    {
              					 printf("\n ERROR : Command Not Found !!! \n");
              					 continue;
              			    }	
        	}	
        	else
        	{
        		   printf("\nERROR : Command Not Found !!! \n");
        	}
   	}
   	return 0;
}
