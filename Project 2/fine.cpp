#include<iostream>
#include<string>
using namespace std;

int main(){
  // declaring variables for inputs and total - which will be used to track the total fine later on.
  
  
  string identification;
  double overstated = 5.0;
  string location;
  double total;
  
  
  //Using getline for strings because it seems to read better.
  cout << "Identification: ";
  getline(cin,identification);
  
  //checks for invalid empty identification string.
  if(identification == ""){
      cout<<"---"<<endl;
      cout<<"You must enter a property identification." << endl;
      //returns 0 to end the program early.
      return 0;
  }
  
  
  cout<< "Overstated amount (in millions): ";
  cin >> overstated;
  
  //it seems using getline or inputting strings attatches extra white space that was causing location to be autofilled as ---. 
  // Using cin.ignore to ignore the new line characters seemed to fix this issue.
  cin.ignore(1000,'\n');
  
  //checks for invalid input of overstated valuation.
  
  if(overstated <= 0.0){
      cout<<"---"<<endl;
      cout<<"The overstated amount must be positive."<<endl;
      
      //ends the program early
      
      return 0;
  }
  
  cout << "Location: ";
  getline(cin,location);
  
  //checks for invalid location input
  
  if(location == ""){
      cout <<"---"<<endl;
      cout <<"You must enter a location." << endl;
      
      //ends program early
      
      return 0;
  }
  
  //boolean conditionals to check which tax fornula to use basd on the valuation amount
  //first if statement is to calculate tax on valuations below or equal to 60 million
  if(overstated<=60){
      total = 0.1111*overstated;
  }
  
  //else if for overstated less than 150 million, also checks location because tax code says the 90 million dollars
  //above 60 million and up until 150 million are taxed differently in NY and Florida.
  else if (overstated<=150){
      if(location == "new york" || location =="florida"){
          
          //first 60 million taxed at 11.11% then next amount taxed at 18% since we are in special areas.

          total = (0.1111*60) + (0.18*(overstated-60));

      }
      else{
          
          //first 60 mil taxed at 11.11% then next amount taxed at 15.3% since we arent in NY or Florida    

          total = (0.1111*60) + (0.153*(overstated-60));
      
      }
  }
  
  //last conditional in case valuation is above 150 million
  
  else{
      
      //applies special tax code if in NY or Florida

      if(location == "new york" || location =="florida"){
          
          //first 60 mil at 11.11% then next 90 mil at 18% - since in NY or Florida - then rest at 21% tax

          total = (0.1111*60) + (0.18*(90)) + 0.21*(overstated-150);
      
      }
      else{
          
          //first 60 mil at 11.11% then next 90 mil at 15.3% then rest at 21% tax since not in special zone
          
          total = (0.1111*60) + (0.153*(90)) + 0.21*(overstated-150); 
      
      }
  }
  
  //prepares cout to only output 3 decimals    
  cout.setf(ios::fixed);
  cout.precision(3);
  
  //finally outputs the expected string if no invalid inputs were detected above.
  cout<<"---"<<endl;
  cout<<"The fine for " << identification << " is $" << total << " million." <<endl;
  return 0;
}
