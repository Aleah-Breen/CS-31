#include <iostream>
#include <fstream>
#include<cctype>
#include<cstring>
using namespace std;
//1000 max will be big enough to store the line size since any given line will have 180 chars max.
const int MAX_FILENAME_LENGTH = 1000;

//Rotate left moves every array element left one index based on params
void rotateLeft(int startIndex, int endIndex, char line[]){
    for(int i=startIndex;i<endIndex;i++){
        line[i]=line[i+1];
    }  
    line[endIndex]='\0';
};

//rotates every array element right based on params
void rotateRight(int startIndex, int endIndex, char line[]){
    for(int i=endIndex;i>=startIndex;i--){
        line[i+1]=line[i];
    }
}

//finds the space closest to the end of the array
int findLastSpace(char line[], int end){
    for(int i=end;i>=0;i++){
        if(line[i]==' '){
            return i;
        }
    }
    return -1;
}

//returns True if the char is one of the punctuations the specs says to treat differently
bool isTargetPunc(char x){
    if(x=='.'||x=='!'||x=='?'||x==':'){
        return true;
    }
    return false;
};


//render function
int render(int lineLength, istream& inf, ostream& outf){
   
    //invalid line length
    if(lineLength<1){
        return 2;
    }
    
    //declaring variables to track chars, indexes, and boolean values
    char c;
    char line[MAX_FILENAME_LENGTH];
    //sets all the memory of line to be empty
    memset(line,0,sizeof(line));
    
    
    //iterator keeps track of how many elements are in line and which index of line we are at
    //Char line tracker keeps track of how many chars have been output to the current line
    int charLineTracker=0;
    int iterator=0;

    int lastSpace=-1;
    int lastHyph=-1;;
    bool brokenWord=false;
    bool lastPar=false;
    char prevChar;
    char last2;
    char last3;
    bool lastIsPunc=false;

    //going one character at a time
    while ( inf.get(c) )
	{   
        //handle paragraph breaks. Making sure to leave no extra spaces at the end
        if(c=='@'&&prevChar=='P'&&last2=='@'&&(last3==' '||last3=='\n')){            
            if(line[iterator-3]==' ' && line[iterator-4]==' '){
                for(int i=0;i<iterator-4;i++){
                    outf<<line[i];
                }
                outf<<endl;
                outf<<endl;
            }
            else if(line[iterator-3]==' '){
                for(int i=0;i<iterator-3;i++){
                    outf<<line[i];
                }
                outf<<endl;
                outf<<endl;
            }
            else{
                outf<< line<<endl;
                outf<<endl;
            }
        
            iterator=0;
            charLineTracker=0;
            memset(line,0,sizeof(line));
            lastSpace=-1;
            lastHyph=-1;
            lastPar=true;
        }
        //If charLineTracker=lineLength that means our current Char is 1 MORE than the max. 
        //This spot must either be \n or we need to put a \n at the last space
        else if(charLineTracker==lineLength){
            //if this char is a space put a \n in
            if(isspace(c)){
                //if the char before this one is also a space put the \n there. (often comes up with punctuation)
                if(line[iterator-1]==' '){
                    line[iterator-1]='\n';
                    charLineTracker=0;
                    iterator=0;
                    outf<<line;
                    lastSpace=-1;
                    lastHyph=-1;
                    memset(line,0,sizeof(line));

                }
                else{
                    line[iterator]='\n';
                    charLineTracker=0;
                    iterator=0;
                    outf<<line;
                    lastSpace=-1;
                    lastHyph=-1;
                    memset(line,0,sizeof(line));

                }
            }
            // if the last char is punctuation it needs to be treated differently. If the word spill over to the next line
            // I need to be sure that I include 2 spaces after each punctuation. If not, I can put no spaces.
            else if(isTargetPunc(c)){
               
                //case 1: the last space was a double space from Punc and it comes after the most recent hyphen
                if(line[lastSpace-1]==' '&&lastSpace>lastHyph){
                    line[lastSpace-1]='\n';
                    line[iterator]=c;
                    iterator+=1;
                    line[iterator]=' ';
                    
                    // since I am essentially deleting one of the spaces I need to rotate the whole array left once.
                    rotateLeft(lastSpace,iterator,line);
                    charLineTracker=(iterator-1)-(lastSpace-1);
                    iterator=0;
                    outf<<line;
                    memset(line,0,sizeof(line));
                    lastSpace=-1;
                    lastHyph=-1;
                    lastIsPunc=true;
                }

                //case 2: the last space was a solo space but still comes after the most recent hyphen. Replace last space with \n
                else if(lastSpace>lastHyph){        
                    line[lastSpace]='\n';
                    line[iterator]=c;
                    iterator+=1;
                    line[iterator]=' ';
                    outf<<line;
                    charLineTracker=iterator-lastSpace;
                    iterator=0;
                    memset(line,0,sizeof(line));
                    lastSpace=-1;
                    lastHyph=-1;
                    lastIsPunc=true;

                }
                //case 3: The last hyphen came before the last space did so we will break there.
                else{
                    line[iterator]=c;
                    // since I am inserting a \n in between a hyphen and a char I need to rotate the whole array right once.
                    rotateRight(lastHyph+1,iterator,line);
                    line[lastHyph+1]='\n';
                    charLineTracker=iterator-lastHyph;
                    iterator=0;
                    outf<<line;
                    memset(line,0,sizeof(line));
                    lastSpace=-1;
                    lastHyph=-1;
                }
            }
            //if the char at the break is not a space or a punctuation it is a regular char so we treat it regularly.
            else{
                // if there have been no hyphens or spaces then a word must be longer than the line length max. We store this 
                // in a boolean and later return 1 if it is true. I process this by just inserting a \n.
                if(lastSpace==-1&&lastHyph==-1){
                    brokenWord=true;
                    line[iterator]='\n';
                    line[iterator+1]=c;
                    charLineTracker=1;
                    iterator=0;
                    outf<<line;
                    memset(line,0,sizeof(line));
                }
                // if the last char is not a space then we need to look at the last space/hyphen to break the line
                else{                    
                    
                    // if the last space is a double space and comes after the most recent hyphen
                    if(line[lastSpace-1]==' ' && lastSpace>lastHyph){
                        line[lastSpace-1]='\n';
                        line[iterator]=c;
                        // once again deleting a space so we need to rotate the array left once.
                        rotateLeft(lastSpace,iterator,line);
                        charLineTracker=(iterator)-lastSpace;
                        iterator=0;
                        outf<<line;
                        lastSpace=-1;
                        lastHyph=-1;
                        memset(line,0,sizeof(line));
                    }
                    // if the last space was more recent than the last hyphen but still solo, replace the space with \n.
                    else if(lastSpace>lastHyph){
                        line[lastSpace]='\n';
                        line[iterator]=c;
                        outf<<line;
                        charLineTracker=iterator-lastSpace;
                        iterator=0;
                        memset(line,0,sizeof(line));
                        lastSpace=-1;
                        lastHyph=-1;
                    }
                    // if the last hyphen was the most recent breakpoint insert a \n after that.
                    else{
                        line[iterator]=c;
                        // since I am inserting a \n I need to rotate my array Right once so I don't lose a char
                        rotateRight(lastHyph+1,iterator,line);
                        line[lastHyph+1]='\n';
                        charLineTracker=iterator-lastHyph;
                        iterator=0;
                        outf<<line;
                        memset(line,0,sizeof(line));
                        lastSpace=-1;
                        lastHyph=-1;
                    }
                }
                
            }

        }
        // if the charLineTracker is greater than the lineLength we know it just was iterated by more than one, therefore
        // the last char must have been a punctuation because we add 2 spaces after that.
        else if(charLineTracker>lineLength){
            // since the last character must be a space I can replace it with a \n
            line[iterator-1]='\n';
            
            // if the current char is not a space I need to recycle it to the next line.
            if(!isspace(c)){
                line[iterator]=c;
                charLineTracker=1;
            }
            // if it is a space I can ignore it since we do not want a space at the start or end of a line
            else{
                charLineTracker=0;
            }
            iterator=0;
            lastSpace=-1;
            lastHyph=-1;
            outf<<line;
            memset(line,0,sizeof(line));

        }
        //this conditional is met when we are not at a paragraph break or the end of the line
        else{
            // if the char is a special punctuation we need to treat it differently.
            if(isTargetPunc(c)){
                // if the char 2 spaces back is a special punctuation it means that the chars after it should be ' '
                if(isTargetPunc(line[iterator-2])){
                        // replace the first space with our new punctuation and put a space after it.
                        if(line[iterator-1]==' '){
                            line[iterator-1]=c;
                            line[iterator]=' ';

                            iterator+=1;
                            charLineTracker+=1;
                            lastSpace=iterator-1;
                        }
                        // if the last char was not a space then I need to just store my special punc and assume it is processed with no spaces
                        else{
                            line[iterator]=c;
                            line[iterator+1]=' ';

                            lastSpace=iterator+1;
                            iterator+=2;
                            charLineTracker+=2;
                        }
                        
                    }
                // if their was not recently a special punctuation I need to put an extra space after this one and store both    
                else{
                    line[iterator]=c;
                    line[iterator+1]=' ';
                    lastSpace=iterator+1;
                    iterator+=2;
                    charLineTracker+=2;
                }
            }
            // if the char is a space or \t or \n etc it needs to be processed specially.
            else if(isspace(c)){
                // if there was just a paragraph break do not store any spaces. @P@ always has a space after we want to ignore.
                if(lastPar){
                    lastPar=false;
                    
                    lastIsPunc=false;
                }
                // if we are at the first character of the new line and there are >0 characters already output, it means something
                // spilled over. Usually after a word spills over we want a space so I decided to include one here.    
                else if(iterator==0&&charLineTracker!=0){
                    line[iterator]=' ';
                    lastSpace=iterator;

                    iterator+=1;
                    charLineTracker+=1;
                    lastIsPunc=false;
                }     
                // if there are no characters on the line we do not want to start with a space. Instead do nothing.
                else if(charLineTracker==0){
                    lastIsPunc=false;
                }   
                // if c is a space and the last char was also a space I need to decide whether to store this second one.
                else if(line[iterator-1]==' '){
                    // if 2 chars back is a special punctuation I should include the second space. 
                    if(isTargetPunc(line[iterator-2])){

                        line[iterator]=' ';
                        lastSpace=iterator;
                        iterator+=1;
                        charLineTracker+=1;
                        lastIsPunc=false;
                    }
                    // if 2 chars back is not special punctuation I do not want back to back spaces so I ignore.
                    // else is only here for organization of code and readability.
                    else{
                       // do nothing.
                    }
                }
                
                // if the last char was not a space or special punctuation I can just record it assuming this is a space between
                // words. By processing it this way this also takes care of putting ' ' instead of a \n from the input
                else{
                    line[iterator]=' ';

                    lastSpace=iterator;
                    iterator+=1;
                    charLineTracker+=1;
                }
            }
            // if c is not a space or special character then it is a normal character 
            else{
                // if it is a hyphen record this position, but do not process differnetly.
                if(c=='-'){
                    lastHyph=iterator;
                }
                // I check if the iterator is > 0 here since I want to look back some spaces. Avoids segmentation fault.
                if(iterator>0){
                    // in events like Dr.Smallberg we do not want to have a space after the period. This boolean checks for that
                    if(isTargetPunc(line[iterator-2])){
                        // if the last two chars were punctuation and space, and this one is a regular char, that means I inserted
                        // a space I should not have after the punctuation. The only time I want 2 spaces after a punctuation
                        // is when it ends a sentence so this makes sure the word segment is actually over.
                        if(line[iterator-1]==' '){
                            line[iterator-1]=c;
                            // this funtion returns when the last space was before this one, since I replace this one with c.
                            // Ex. Mr. K --> Mr.K
                            lastSpace=findLastSpace(line,iterator-1);
                        }
                        // if the last char was not a space I can just input this one normally.
                        else{
                            line[iterator]=c;
                            iterator+=1;
                            charLineTracker+=1;
                        }
                    }
                    // if the last char was not a incorrect space after punctuation I can just record it.

                    else{
                        line[iterator]=c;
                        iterator++;
                        charLineTracker++;
                        lastIsPunc=false;
                    }
                }
                // if the iterator=0 I can just store whatever char and assume the last one was not an invalid punctuation, 
                // since there is no last char.
                else{
                
                line[iterator]=c;
                iterator++;
                charLineTracker++;
                lastIsPunc=false;
                }
            }
            lastPar=false;
        }
        // these 3 conditionals just store the last characters so that I can easily access them with the paragraph break.
        // I need a special boolean for each in case I clear lines memory.
        if(iterator>=2){
            last3=last2;
            last2=prevChar;
            prevChar=c;
            }
        else if(iterator==2){
            last2=prevChar;
            prevChar=c;
        }
        else if(iterator==1){
            prevChar=c;
        }

    }
    
    
    // printing the last line. Since the while loop goes until there are no more chars I still need to print the last line 

    //checks if the last chars are spaces or \n so that I do not have any spaces at the end of the output.
    
    if(strcmp(line,"\n")==0){
        // do nothing    
    }
    // checks for double space
    else if(strcmp(line,"")==0){
        //do nothing
    }
    else if(line[iterator-1]==' ' && line[iterator-2]==' '){
        for(int i=0;i<iterator-2;i++){
            outf<<line[i];
        }
        outf << '\n';
    }
    // checks for a single space at the end of the output.
    else if(line[iterator-1]==' '){
        for(int i=0;i<iterator-1;i++){
            outf<<line[i];
        }
        outf << '\n';
    }
    // if there are no extra spaces I can output the line normally
    else{
        outf<< line << endl;
    }
    // if any word has been broken in half then I need to return 1. I handle this boolean value early and now it is time to check it.
    if(brokenWord){
        return 1;
    }
    // else return 0
    return 0;


};


// main provided by smallberg
int main()
    {
        for (;;)
        {
            cout << "Enter input file name (or q to quit): ";
            char filename[MAX_FILENAME_LENGTH];
            cin.getline(filename, MAX_FILENAME_LENGTH);
            if (strcmp(filename, "q") == 0)
                break;
            ifstream infile(filename);
            if (!infile)
            {
                cerr << "Cannot open " << filename << "!" << endl;
                continue;
            }
            ofstream outfile("results.txt");
            cout << "Enter maximum line length: ";
            int len;
            cin >> len;
            cin.ignore(10000, '\n');
            int returnCode = render(len, infile,outfile);
            cout << "Return code is " << returnCode << endl;
        }
    }
