#include<stdio.h>
#include<stdlib.h>
#include<unistd.h> //for getopt


//functions
int hex_dec(char);//char hex number to int decimal value
char *base64(char *,int);//convert a char array to base64 encode char array


int main(int argc, char * argv[]){

    FILE *fptrA, *fptrC;//fptrA for input (char hex numbers),fptrC for output txt(encode by base64 )
    int opt;//option get in command line

    //getopt() function, get option and argument from command line
    while ((opt = getopt(argc, argv, "i:o:")) != -1){
        switch (opt) {
            case 'i'://input option
                fptrA = fopen(optarg,"r");
                break;
            case 'o'://output option
                fptrC = fopen(optarg,"w");
                break;
            case '?'://for other unexpected option, and print error message
                printf("Unknown option: %c\n",(char)optopt);
                break;
        }
    }

    char read,nibble_left,nibble_right,nibble;
    int len, index,nibble_times;
    char *B,*b64out;//b is a dynamic char array for store a line of data after nibble process, b64out is a dynamic char array for store a line of data after base64 encode

    do{//start to count len of a target line in input.txt
        len = 0;
        while((read=getc(fptrA))!='\n'&&read!=EOF){//EOF is end of file
            len++;
        }

        if(read==EOF) {//the pointer can't move to the position of EOF , we need to -1 to get correct len of line.
            printf("read this line to EOF!\n");
            nibble_times=len/2;
            len -= 1;
        }
        else if(read=='\n') {
            nibble_times=(len-1)/2;
            printf("read this line to change line!\n");
        }

        //move the pointer back to head of line by move count position
        fseek(fptrA, -len-1, SEEK_CUR);


        //creat dynamic char array B for store a line of data after nibble process, the length of data in B is 1/2 of origin one line input, add 1 for store '\0'
        B = (char*)malloc(sizeof(char)*(len/2+1));



        index = 0;//initialize index for char array B
        printf("the line of input.txt is: ");

        // read a line of data in input.txt by two letter as a pair, than do the nibble process and store the result to char array B
        for(int i=0;i<nibble_times;i++){
            read =getc(fptrA);
            printf("%c",read);
            nibble_left = hex_dec(read)<<4;//turn Hex value (char) to it's decimal value (int), and move bits 4 position left, eg: 00001010->10100000
            read=getc(fptrA);
            printf("%c",read);
            nibble_right = hex_dec(read);//turn Hex value (char) to it's decimal value (int)
            nibble = nibble_left | nibble_right;//assemble left part and right part data, ex:11110000|00001111==11111111
            B[index] = nibble;//store the result to right position(index) of char array B
            index++;//move index for change the position in B (for next loop)
        }
        printf("\n");


        //used index as a length of letters process in last step, encode letters by base64 than store in output.txt
        b64out=base64(B,index);
        fprintf(fptrC, "%s", b64out);
        printf("after base64 encode is: %s\n",b64out);
        free(B); //free memory of dynamic char array B

        if((read =getc(fptrA))==EOF)//if reach the end of input.txt we don't need to do next step for add'\n' to output.txt
            continue;

        fprintf(fptrC, "%c", '\n');//if we have next line to read and write, add '\n' to output.txt
        printf("\n");
        fseek(fptrA, 1, SEEK_CUR);
    }while(read !=EOF);

    //close files
    fclose(fptrA);
    fclose(fptrC);

    return 0;
}


int hex_dec(char hex){//char hex number to int decimal value
    int num = hex;//turn char to its ASCII index, than turn to its int value
    //ASCII index to hex value
    if (48 <=num && num <= 70){//'0'~'F'
        if (num <= 57)//'0'(48)~'9'(58)
            num -= 48;//'0' to 0, '9' to 9
        else if (65 <= num)//'A'(65)~'F'(70)
            num -= 55;//'A' to 10, 'F' to 15
        else
            printf("find NOT Hex number in input!\n");
    }
    return num;
}
char *base64(char *str, int len_in){//convert a char array to base64 encode char array

    char *out, *b64data="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";//out for output, b64data for encode index

    //use length of input char array compute length of output char array
    int len_out;
    if(len_in%3==0)//because 3 letters input (3 pairs of nibble) can transform to 4 base64 letters
        len_out=len_in/3*4;
    else
        len_out=len_in/3*4+4;

    //creat a dynamic array for store output with len_out
    out=malloc(sizeof(char)*len_out+1);  //+1 for '\0'

    //because 3 letters input (3 pairs of nibble) can transform to 4 base64 letters
    for(int i=0, j=0; i<len_out-2; i+=4, j+=3){
        out[i]=b64data[str[j]>>2];                          //take first 6 bits from first char
        out[i+1]=b64data[(str[j]&0x3)<<4|str[j+1]>>4];      //take last 2 bits from first char and first 4 bits from second char
        out[i+2]=b64data[(str[j+1]&0xf)<<2|str[j+2]>>6];    //take last 4 bits from second char and first 2 bits from third char
        out[i+3]=b64data[str[j+2]&0x3f];                    //take last 6 bits from fourth char
    }

    //add '=' if (len_in%3)!=0
    if((len_in%3)==1){
        out[len_out-2]='=';
        out[len_out-1]='=';
    }
    else if((len_in%3)==2)
        out[len_out-1]='=';


    return out;
}
// you can run this program by the commend in next line
//./compressor -i input.txt -o output.txt
//This Assignment done by 黃子峻