//fix H2V3H99V2F    H3
// make sure it returns 1 for syntax error since tab is invalid char.
#include "grid.h"
#include<iostream>
#include<string>
#include<cctype>
using namespace std;
const int HORIZ = 0;
const int VERT = 1;

const int FG = 0;
const int BG = 1;
//with redundant breaks just in case

char currentChar = '*';
int currentMode = FG;

bool plotLine(int r, int c, int distance, int dir, char plotChar, int fgbg){
    
    //first if statements filter out invalid plotLine calls

    //if direction is not HORIZ or VERT it is invalid
    if(dir!=0 && dir!=1){
        return false;
    }
    //if the foreground/background value is not 0 or 1 it is invalid
    if(fgbg !=0 && fgbg!=1){
        return false;
    }
    //if plotChar is not printable it is invalid
    if(!isprint(plotChar)){
        return false;
    }   
    //if the line is meant to be plotted outside of the grid, it is invalid
    if(r > getRows() || c>getCols()){
        return false;
    }
    

    //computes code for horizontal lines
    
    if(dir == HORIZ){
        //checks if the final line endpoint would be out of bounds
        if(c+distance < 1 || c+distance > getCols()){
            return false;
        }
        else{
            //handles positive lines going forwards
            if(distance>0){
                //loop from starting column to start + distance since start + distance = end
                for(int i =c; i<= c+distance;i++){
                    if(getChar(r,i)==' '){
                        setChar(r,i,plotChar);
                    }
                    else if(fgbg==FG){
                        setChar(r,i,plotChar);
                    }
                }
            }
            //handles negative lines going backwards -- comments the same as above except loops back instead of forwards
            else if(distance <0){
                for(int i =c; i>= c+distance;i--){
                    if(getChar(r,i)==' '){
                        setChar(r,i,plotChar);
                    }
                    else if(fgbg==FG){
                        setChar(r,i,plotChar);
                    }
                }
            }
            //if distance = 0 then it is a singular point I plot. 
            else{
                if(fgbg==FG || getChar(r,c)==' '){
                    setChar(r,c,plotChar);
                }
            }
        }
        return true;
    }
    // computes code for vertical lines -- comments same as horizontal lines but uses rows instead of columns.
    else{
        if(r+distance < 1 || r+distance > getRows()){
            return false;
        }
        else{
            if(distance>0){
                for(int i =r; i<= r+distance;i++){
                    if(getChar(i,c)==' '){
                        setChar(i,c,plotChar);
                    }
                    else if(fgbg==FG){
                        setChar(i,c,plotChar);
                    }
                }
            }
            else if(distance <0){
                for(int i =r; i>= r+distance;i--){
                    if(getChar(i,c)==' '){
                        setChar(i,c,plotChar);
                    }
                    else if(fgbg==FG){
                        setChar(i,c,plotChar);
                    }
                }
            }
            else{
                if(fgbg==FG){
                    setChar(r,c,plotChar);
                }
            }
        }
        return true;
    }
    
};


int performCommands(string commandString, char& plotChar, int& mode, int& badPos){
    
    int distance=0; // used to track the distance for how far the line will go
    int r=1;//r used to track current row
    int c=1;// c used to track current col
    int tempBad = -1;
    // if given plotChar is invalid return error type 2
    if(!isprint(plotChar)){
        return 2; 
    }  
    // if given mode value is invalid return error type 2
    if(mode != BG && mode != FG){
        return 2;
    }
    
    // loop through the string. Used < instead of != because I iterate by more than 1 sometimes
    int i =0;
    while(i<commandString.size()){
        distance=0;
            //if I is the last character in the string and is not C or c then it requires preceding characters and is therefore invalid
            if(i==commandString.size()-1){
                if(!(commandString[i]=='c') && !(commandString[i]=='C')){
                    
                    if(commandString[i]=='h'||commandString[i]=='H' || commandString[i]=='v' || commandString[i]=='V'){
                    badPos = commandString.size();
                    return 1;
                    }
                    else{
                        badPos = commandString.size()-1;
                        return 1;
                    }
                }
            }
            char temp = commandString[i];
            
            //switch statement to keep track of the 5 different commands: H, V, C, B, F. Also check lowercases.
            switch(temp){
               
    // -------------------------------------- H CASES --------------------------------------------------       
    //First checks negatives, then one or 2 digit lengths. Then positives 1 or 2 digits lengths. 
               
                case('h'):
                    
               
               
               
                case('H'):
                    //if 'h' is the second to last letter and the last letter is not a number its invalid 
                    if(i==commandString.size()-2 && !isdigit(commandString[i+1])){
                        badPos=i+1;
                        return 1;
                    }
                    //changing direction for negative cases.
                    if(commandString[i+1]=='-'){
                        
                        // the following if is for negative double digit lengths. I first check for string out of bounds then calc.
                        if(i+3 <commandString.size()){
                            if(isdigit(commandString[i+2]) && isdigit(commandString[i+3])){
                                
                                //since the first digit represents the tens place I mult by ten. Then add by the second digit.
                                //E.x 67 = 6*10 +7
                                
                                distance = (commandString[i+2]-'0')*10;
                                distance += commandString[i+3]-'0';
                                distance = distance * -1;
                                //since plotline returns T/F based on success, I can use it in an if to determine if their is a type 3 error
                                if(plotLine(r,c,distance,0,plotChar,mode)){
                                    //update coordinates
                                    c = c+ distance;
                                    //iterate by 4. 1 for the minus, 2 for the double digits, and a 4th to get to the new char. 
                                    i+=4;
                                }
                                else{
                                    if(tempBad==-1){
                                        tempBad=i;
                                        i+=4;
                                    }
                                }
                            }
                            //since this if statement is i+3< size I need to also have a conditional for on e digit negative.
                            //if I reorganized code I wouldnt need this. Redundant because it reappears right below this segment.
                            else if(isdigit(commandString[i+2])){
                                //since one digit no *10.
                                distance = commandString[i+2]-'0';
                                distance = distance*-1;
                                if(plotLine(r,c,distance,0,plotChar,mode)){
                                    c = c+ distance;
                                    i+=3;
                                }
                                else{
                                    if(tempBad==-1){
                                        tempBad=i;
                                        i+=3;
                                    }
                                }
                            }
                            else{
                                badPos=i+1;
                                return 1;
                            }
                        
                        } 
                        //this if checks for single digit negatives. Repeat of above but need it outside the i+3 check in case it is last
                        //two chars in the string. Comments are the same.
                        else if(i+2<commandString.size()){
                            if(isdigit(commandString[i+2])){
                                distance = commandString[i+2]-'0';
                                distance = distance*-1;
                                if(plotLine(r,c,distance,0,plotChar,mode)){
                                    c = c+ distance;
                                    i+=3;
                                }
                                else{
                                    if(tempBad==-1){
                                        tempBad=i;
                                        i+=3;
                                    }
                                }
                            }
                            else{
                                badPos=i+1;
                                return 1;
                            }
                        }
                        else{
                            badPos=i+2;
                            return 1;
                            }
                    }
                    //this conditional checks for two digit positive lines. Same comments as before only difference is 
                    //distance is not multiplied by -1  
                    else if(i+2<commandString.size()){   
                        if(isdigit(commandString[i+1]) && isdigit(commandString[i+2])){
                            distance = (commandString[i+1]-'0')*10;
                            distance += commandString[i+2]-'0';
                            if(plotLine(r,c,distance,0,plotChar,mode)){
                                c = c+ distance;
                                i+=3;
                            }
                            else{
                                if(tempBad==-1){
                                    tempBad=i;
                                    i+=3;
                                }
                            }
                        }
                        //this conditional echecks for 1 digit positive lines.
                        else if(isdigit(commandString[i+1])){
                            distance = commandString[i+1]-'0';
                            if(plotLine(r,c,distance,0,plotChar,mode)){
                                c = c+ distance;
                                i+=2;

                            }
                            else{
                                if(tempBad==-1){
                                    tempBad=i;
                                    i+=2;
                                }
                            }
                        }
                        else{
                            badPos=i+1;
                            return 1;
                        }
                    }
                    //checks for 1 digit positive lines at the very end of string that would have failed if i+2<commandString.size
                    else if(isdigit(commandString[i+1])){
                            distance = commandString[i+1]-'0';
                            if(plotLine(r,c,distance,0,plotChar,mode)){
                                c = c+ distance;
                                i+=2;
                            }
                            else{
                                if(tempBad==-1){
                                    tempBad=i;
                                    i+=2;
                                }
                            }
                        }
                    else{
                        badPos=i+1;
                        return 1;
                    }
                    break;
               
// -------------------------------------- V CASES --------------------------------------------------       
//First checks negatives, then one or 2 digit lengths. Then positives 1 or 2 digits lengths. 

                case('V'):
                    
                case('v'):
                   if(i==commandString.size()-2 && !isdigit(commandString[i+1])){
                        badPos=i+1;
                        return 1;
                    }
                    //changing direction for negative cases.
                    if(commandString[i+1]=='-'){
                        if(i+3 <commandString.size()){
                            if(isdigit(commandString[i+2]) && isdigit(commandString[i+3])){
                                distance = (commandString[i+2]-'0')*10;
                                distance += commandString[i+3]-'0';
                                distance = distance * -1;
                                if(plotLine(r,c,distance,1,plotChar,mode)){
                                    r = r+ distance;
                                    i+=4;
                                }
                                else{
                                    if(tempBad==-1){
                                        tempBad=i;
                                        i+=4;
                                    }
                                }
                            }
                            else if(isdigit(commandString[i+2])){
                                distance = commandString[i+2]-'0';
                                distance = distance*-1;
                                if(plotLine(r,c,distance,1,plotChar,mode)){
                                    r = r+ distance;
                                    i+=3;
                                }
                                else{
                                    if(tempBad==-1){
                                        tempBad=i;
                                        i+=3;
                                    }
                                }
                            }
                            else{
                                badPos=i+1;
                                return 1;

                            }
                        
                        } 
                        else if(i+2<commandString.size()){
                            if(isdigit(commandString[i+2])){
                                distance = commandString[i+2]-'0';
                                distance = distance*-1;
                                if(plotLine(r,c,distance,1,plotChar,mode)){
                                    r = r+ distance;
                                    i+=3;
                                }
                                else{
                                    if(tempBad==-1){
                                        tempBad=i;
                                        i+=3;
                                    }
                                }
                            }
                            else{
                                badPos=i+2;
                                return 1;
                            }
                        }
                        else{
                                badPos=i+2;
                                return 1;
                            }
                    }
                        
                    else if(i+2<commandString.size()){   
                        if(isdigit(commandString[i+1]) && isdigit(commandString[i+2])){
                            distance = (commandString[i+1]-'0')*10;
                            distance += commandString[i+2]-'0';
                            if(plotLine(r,c,distance,1,plotChar,mode)){
                                r = r+ distance;
                                i+=3;
                            }
                            else{
                                if(tempBad==-1){
                                    tempBad=i;
                                    i+=3;
                                }
                            }
                        }
                        else if(isdigit(commandString[i+1])){
                            distance = commandString[i+1]-'0';
                            if(plotLine(r,c,distance,1,plotChar,mode)){
                                r = r+ distance;
                                i+=2;

                            }
                            else{
                                if(tempBad==-1){
                                    tempBad=i;
                                    i+=2;
                                }
                            }
                        }
                        else{
                            badPos = i+1;
                            return 1;
                        }
                    }
                    else if(isdigit(commandString[i+1])){
                            distance = commandString[i+1]-'0';
                            if(plotLine(r,c,distance,1,plotChar,mode)){
                                r = r+ distance;
                                i+=2;
                            }
                            else{
                                if(tempBad==-1){
                                    tempBad=i;
                                    i+=2;
                                }
                                }
                        }
                    else{
                        badPos=i+1;
                        return 1;
                    }
                    break;
                
// -------------------------------------- F CASES --------------------------------------------------        
// changes the value of plotChar and mode to front and iterates by 2 if valid.
                case('F'):

                case('f'):
                    mode=0;
                    if(isprint(commandString[i+1])){
                        plotChar=commandString[i+1];
                        i+=2;
                    }
                    else{
                        badPos=i+1;
                        return 1;
                    }
                    break;

// -------------------------------------- B CASES --------------------------------------------------
//changes the value of plotChar and mode to back and iterates by 2 if valid. 

                case('B'):
                    
                case('b'):
                    mode=1;
                    if(isprint(commandString[i+1])){
                        plotChar=commandString[i+1];
                        i+=2;
                    }
                    else{
                        badPos=i+1;
                        return 1;
                    }
                    break;  
                
// -------------------------------------- C CASES --------------------------------------------------
// simply replaces certain variables and clears grid and iterates i one time since c has no following values if all valid

                case('c'):
                   
                case('C'):
                    clearGrid();
                    r=1;
                    c=1;
                    plotChar='*';
                    mode=0;
                    i+=1;
                    break;
                default:
                    badPos=i;
                    return 1;
            }
    }
    if(tempBad==-1){
        return 0;
    }else{
        badPos = tempBad;
        return 3;
    }
};

int main()
{
    for (;;)
    {
        cout << "Enter the number of grid rows and columns (max 30 each): ";
        int nRows;
        int nCols;
        cin >> nRows >> nCols;
        cin.ignore(10000, '\n');
        if (nRows >= 1  &&  nRows <= MAXROWS  &&  nCols >= 1  &&  nCols <= MAXCOLS)
        {
            setSize(nRows, nCols);
            break;
        }
        cout << "The numbers must be between 1 and 30." << endl;
    }
    char currentChar = '*';
    int currentMode = FG;
    for (;;)
    {
        cout << "Enter a command string (empty line to quit): ";
        string cmd;
        getline(cin, cmd);
        if (cmd == "")
            break;
        int position;
        int status = performCommands(cmd, currentChar, currentMode, position);
        switch (status)
        {
          case 0:
            draw();
            break;
          case 1:
            cout << "Syntax error at position " << position << endl;
            break;
          case 2:
            if (!isprint(currentChar))
                cout << "Current character is not printable" << endl;
            if (currentMode != FG  &&  currentMode != BG)
                cout << "Current mode is " << currentMode << ", not FG or BG" << endl;
            break;
          case 3:
            cout << "Cannot perform command at position " << position << endl;
            break;
          default:
              // It should be impossible to get here.
            cout << "performCommands returned " << status << "!" << endl;
        }
    }
}
