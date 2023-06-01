#!/bin/bash  
if [ -n "$1" ]
then
   para1=$1
fi

if [ -n "$2" ]
then
   para2=$2
fi
   func (){  
       local dir="$1"
       local compileArg=     
       #find "$dir" -name "*.ll"  | xargs rm -f
       #cp ../compile.sh "$dir/compile.sh"
       for f in `ls $1`                                                                              
       do                                                                                           
           if [ -f "$dir/$f" ]                                                 
           then
           	if  [[ $f =~ .*\.c ]]   
           	then
           		
           		echo "find $f"
           		cd "$dir"  
           		echo | xargs rm *.ll *.dot 
           		compileArg=`echo "$f" | sed -r "s/(.*)\.c/\1 kernel/g"`
           		compileArg="$compileArg $para1 $para2"
           		echo $compileArg | xargs sh ../compile.sh
           		#echo | xargs rm *.ll *.dot 
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
