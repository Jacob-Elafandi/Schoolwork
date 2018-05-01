## Refactoring Requirements
This portion of iteration 3 will allow you to refactor your iteration 2 code.  Because we want you to do the refactoring independently of completing the priorities of iteration 2, you will need to complete the following steps:

1)  Create a new branch for each refactoring task.  You need to follow the naming conventions for these branches since will be checking only these branches for your work.  Create the following three branches in GitHub:  *_Refactor1_*,  *_Refactor2_*, and  *_Refactor3_*.  You will only do one task in each branch.  You are **not expected** to merge these branches with your master after you complete this portion of iteration 3.  Please be mindful that you must follow the naming conventions set forth in the following instructions since we will not search for your documents if they are not in these locations with the required names.

2)  Your first refactoring task will be to complete a *_Replace Temp with Query_* refactor.   This will be completed in the *_Refactor1_* branch. Look through your code and identify a place where you are using a temporary variable to hold the result of an expression.   You will create a function that performs the expression and you will replace all references to the temp with a function call.  Complete the following tasks:
````
  * Refactor the code in the original file(s) in your iteration 2 source code (/project/iteration2/src).
  * Next, create a document called, refactor1.xxx, that contains three things:
    1.  A list of the files and methods you are adding and/or changing.  You will provide a snippet of code that shows the original code you are changing.
    2.  The name of the new method you created and where it is located.  Identify clearly also where any changes were made to call the new function.  
    3.  An explanation of what behavior this is replacing.  What does the new function specifically do?  (no more than one paragraph)
  * Ensure that the refactor1.xxx document is located in the /project/iteration2 directory.  
````
3)  Your second refactoring task will be to complete a *_ChangeVariableNames_* refactor.   This will be completed in the *_Refactor2_* branch. Look through your code and identify two places where you have variables with names that do not provide any real meaning to what they do.  You will update your variable names and make all necessary naming changes as needed.  Complete the following tasks:
````
    * Refactor the code in the original file(s) in your iteration 2 source code (/project/iteration2/src).
    * Next, create a document called refactor2.xxx, that contains three things:
        1.  A list of the files and methods where you are making your changes.
        2.  The original variable names that you are changing and what their new names are called after the refactor.
        3.  An explanation of how your new names are better than the old ones (no more than one paragraph.)
    * Ensure that the refactor2.xxx document is located in the /project/iteration2 directory.  
````
4)  Your third refactoring task will be to complete an *_ExtractMethod_* refactor.  This will be completed in the *_Refactor3_* branch. Look through your code and identify a method that is too long and extract at least one method from this.  Be sure to update all calls as needed.  Look at Lab09 if you get stuck on how to approach this. Complete the following tasks:
````
    * Refactor the code in the original file(s) in your iteration 2 source code (/project/iteration2/src)
    * Next, you will create a document that contains three things:
        1.  A list of the files and the method(s) you are changing.
        2.  A list of the location of the changes.  Be sure you explain briefly what you are doing at each location.
        3.  An explanation of how this improves your code (no more than one paragraph.)
    * Ensure that the refactor3.xxx document is located in the /project/iteration2 directory.  
````
