# Assignment 5: Code Review

Review the source code in the repo github.com/utk-cs340-fall24/code-review. 
Use the style guide in the file named style-guide.txt. 
Identify and fix the issues in the source code files that violate the
style guide. There are 2 parts to this assignment:

**1. Identify the issues and the fix:**

Create a file named issues.txt to identify the issues. In that file, give the
filename, line number, and a very short description of the issue and the fix.
List the issues in the order they appear in the file and group issues in 
the same file together. For example:  
  > main.c line 11: { should be on the next line.  
  > main.c line 15: multi-line comment format should follow correct style.

In some cases, a line number is not needed; for example, if a filename
should be changed. Sometimes a fix refers to a block of code,
in which case you can just list the first line. Or it may pertain to
a whole file, in which case you can say something like:   
  > Base64.c: all indentations should be changed from 2 spaces to 4 spaces.

In some cases, there is a block of code that is wrong, for example, the
indentation may be wrong. List the block as 1 item with the line number of
the first bad line. Do not list as one bad item per line. For example, if
all the cases in a switch statement are indented wrong, list that as one
item like this:
  > main.c line 51 and following: indentation is wrong on case statements.

Note that you don't have to give much detail about how to fix it; the
main point of a code review is to find the problems not fix them. But it
is helpful to list the fix if it is known (which they will be in our case).

**2. Fix the Issues:**

Modify the files in the repo to fix all of the issues identified when you 
reviewed the code. Make sure the code compiles and links with no warnings 
or errors using the Makefile. Note that the Makefile will need minor
modifications due to filename changes. The compiler options should not
be changed. The Makefile does not need to conform to a style guide. 
This has been tested on Linux and FreeBSD. It may work on other platforms,
but it is recommended that you use one of these two.

**Turn in the assignment using the following instructions:**
- Copy the files from the code-review repo listed above. You can clone or fork the repo, but you will need to create a new private repo to place your finished work. Note that forking results in a fork that is readable by everyone else in the class, even if it is private. You need to create a new repo that is not readable by anyone other than the TAs and prof.
- Create a new text file named issues.txt as described above.
- Modify the source code files and Makefile to correct the issues.
- Create a private repo (important: make sure it is private) in your
   own git hub acccount. Place the fixed files (including the Makefile)
   and issues.txt in your repo.
- Give read permission to the TAs and myself. The github ids are
   davidalanreid, Shayanashrestha, and Manan-dev. To give read permission,
   select settings->Collaborators->Add People in your repo.
- Open an issue in the utk-cs340-fall24/code-review repo when finished
   and put a note in the issue with a link to your repo with the fixed code.
- Submit a link you your repo on Canvas

There are at least 35 issues in the code. Grading will be as follows:
- 1 point for each issue found up to 35 points.
- 1 point for each issue fixed up to 35 points.
- 10 points for putting all the files in a **private** repo and giving read permission to the TAs.
- 10 points if it compiles and links without errors using the Makefile.
- 10 points if it compiles with no warnings using the Makefile.
- 100 points total

Reminder: There is a 10% per day late penalty for late work. No credit is
given after 10 days.

