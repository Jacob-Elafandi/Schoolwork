# Robot Simulator: Iteration 3

DUE: Wednesday, December 13 at 11:55pm

**Late assignments will not be accepted**

You can convert this requirements document to pdf using this tool: http://www.markdowntopdf.com/

<hr>

#### Create iteration3 Folder

In the non-academic world, one would not copy code into a new directory and continue development in this new folder, because that is not using Git as it was intended (which was designed so that you don't have to copy code). However, we need to grade this and not everyone successfully merged and tagged, so to keep a convenient record of work, please create an iteration3 folder using the following:

```
git checkout devel
cd project
mkdir iteration3
cp -r iteration2/* iteration3/.
git add iteration3
git commit -m "starting iteration3"
```
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
<hr>

## Documentation Requirements

Documentation requirements include Doxygen with mainpage, the bug report, and the Peer Reflection as described below. You do not need to provide any other documentation for this iteration.

### Peer Reflection

1. Find 1-4 persons in the class and exchange documentation so that everyone will receive 1 peer reflection.

2. Obtain the UML and the design document from the author. The author can email, share on google drive, or upload to her/his cselabs web page.

3. Answer the questions in the PeerResponseWorksheet.md. You will be graded on the quality of your response with respect to analysis and writing. Title your response document _PeerResponses.*_. You can use any text or web-viewable format for this document.

4. Give your responses to the author.

5. Place your answers in your _project/iteration3/docs_ folder.

### Bug Report

Continue to maintain your bug report. The expectation for this iteration is that there are no known bugs. If there are no known bugs, then please report that. If you have one or two minor bugs and you list them, we will not deduct any points.  

<hr>

## Assessment

- 30% Fully functional requirements of iteration 2.
- 20% Refactoring (3 branches)
- ~~05% No Warnings During compilation~~
- 05% Submission, File Structure, File Names
- 25% Peer Response Worksheet
- 10% Doxygen
- 05% cpplint Error Free
- 05% Bug Report

