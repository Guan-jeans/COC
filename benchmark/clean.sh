#!/bin/bash  
   func (){  
       local dir="$1"
       for f in `ls $1`                                                                              
       do                                                                                           
           if [ -f "$dir/$f" ]                                                 
           then
           	if  [[ $f =~ .*\.c ]]   
           	then
           		
           		echo "clean results of $f"
           		cd "$dir"  
           		echo | xargs rm *.ll *.dot 
           	fi                                                      
          elif [ -d "$dir/$f" ]                                                                     
          then        
              if [[ $f == common || $f == utilities ]]
              then
              	   continue
              fi
              func "$dir/$f"
          fi 
      done                                                                                          
  }
  dir=`pwd`
  func $dir
