/* Author: Haydn */

#include "logging.h" 
 
string generate_log_file_name(string log_string, bool useCurrent)
{     
  //generate file name     
  int fileIndex = 0;     
  string newStatFileName ; 
 
 
  if(useCurrent == false)
  {         
    newStatFileName = log_string + to_string(fileIndex) + ".txt";         
    //check if file exists         
    
    while((std::ifstream(newStatFileName)))
    {             
      //if so increment file name index             
      newStatFileName.clear();             
      newStatFileName = log_string + to_string(fileIndex++) + ".txt";         
     }     
   }     
   
   else
   {         
    newStatFileName = log_string + ".txt";     
   } 
 
   if(std::ifstream(newStatFileName))
   {         
      //file  exists dont overwrite     
   }     
   
   else     
   {         
      //create file         
      std::ofstream logFile (newStatFileName.c_str());         
      logFile.close();     
   } 
 
    return newStatFileName; 
 }
