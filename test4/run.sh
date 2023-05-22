#!/bin/bash                                                                                                                                                                                          
   func (){                                                                                       
       local dir="$1"      #local局部变量标识符                                                                                                                                                                 
       #find "$dir" -name "*.ll"  | xargs rm -f
       #cp ../compile.sh "$dir/compile.sh"
       for f in `ls $1`                                                                              
       do                                                                                           
           if [ -f "$dir/$f" ]                       #   $dir/$f    保障路径正确                               
           then
           #	echo hh
           	if  [[ $f =~ .*\.c ]]   
           	then
           		
           		echo "find $f"
           		cd "$dir"  
           		echo | xargs rm *.ll *.dot 
           		#echo "$f" | sed -r "s/(.*)\.c/\1 kernel/g" | xargs sh ../compile.sh
           		#echo | xargs rm compile.sh
           	fi                                                      
          elif [ -d "$dir/$f" ]                                                                     
          then                                                                                      
              echo "$dir/$f是目录"       
              if [[ $f == common || $f == utilities ]]
              then
              	   continue
              fi                                                           
              func "$dir/$f"                #如果是目录再次进行遍历                                                                 
          fi                                                                                                                                                                              
      done                                                                                          
  }                                                                                                 
  func  ~/applications/test4                   # func 的第一个参数也就是$1 
