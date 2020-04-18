#include <stdio.h>
#include <stdlib.h>
#include <elf.h>


enum{
  DOWNLOAD_FILE_TYPE_EXE,
  DOWNLOAD_FILE_TYPE_DYN,
  DOWNLOAD_FILE_TYPE_CORE,
  DOWNLOAD_FILE_TYPE_REL,
  DOWNLOAD_FILE_TYPE_OTHER, 
};

int parse_file_type(const char *filename)
{   
    FILE *fp = NULL;  	
    Elf32_Ehdr  hdr={0};
    int ret;

    fp = fopen(filename,"r");
    if(NULL == fp)
	   return -1;
    
    if((ret = fread(&hdr,1,sizeof(hdr),fp)) != sizeof(hdr))
    {

	 fclose(fp);    
         printf("*******%d\n",ret);
	 return DOWNLOAD_FILE_TYPE_OTHER;
    }
    fclose(fp);
    
    if( !(hdr.e_ident[0] == 0x7f  && hdr.e_ident[1] == 'E' &&  hdr.e_ident[2] == 'L' && hdr.e_ident[3] == 'F') )
       return DOWNLOAD_FILE_TYPE_OTHER;

    switch(hdr.e_type)
    {
        case ET_REL:
            return DOWNLOAD_FILE_TYPE_REL;
	case ET_EXEC:
	    return DOWNLOAD_FILE_TYPE_EXE;
	case ET_DYN:
	    return DOWNLOAD_FILE_TYPE_DYN;
	case ET_CORE:
	    return DOWNLOAD_FILE_TYPE_CORE;
	default:
	    return DOWNLOAD_FILE_TYPE_OTHER;
    }

    return -1;
}

int main(int argc,char *argv[])
{

     printf("%d\n",parse_file_type(argv[1]));


}


