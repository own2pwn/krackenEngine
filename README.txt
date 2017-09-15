..................................................................................................


Okay guys follow this link and do the tutorial(s) please:
You don't have to do all of it, if you understand it though you will know when you need to stop.

http://learngitbranching.js.org/

Also for more documentation if you are interested here is the official link

https://git-scm.com/book/en/v2

If you don't like the tutorial (which is highly unlikely)

First go to

https://git-scm.com/downloads

and download git. It ask you questions how you want to set up git for windows. Personally I don't let
git near the windows command prompt and use it as a stand alone mingw enviroment.

When you have downloaded and installed git, launch git and type

	git clone https://github.com/UPS7kYkHEPbZIucaqjje/krackenEngine


now you have the repo!


Now I will describe a basic workflow you should do when you start working

	git checkout master
	git pull

You first create a branch off of master. Don't work on master branch!

	git branch NAME_OF_THE_FEATURE_I_AM_WORKING_ON
	git checkout NAME_OF_THE_FEATURE_I_AM_WORKING_ON

and you work on your feature

add new files, modify old files, etc

when you feel like you hit a milestone or finished the feature you wanna commit

before you do you need to specify the files you wanna add/modified

	git add NAME_OF_FILE

you can also do this if you wanna add every single file (please be careful when doing this don't add things)

	git add .

if you didn't intend to add a file

	git rm --cached NAME_OF_FILE_YOU_DIDNT_WANT_TO_ADD

now that you got everything 
	
	git status

if everything looks correct then type
	
	git commit

of course this uses vi as the default text editor to change it given it another option that is accesible via the command line

	git config --global core.editor "notepad"

The recommandation for commiting is as follows
	
	"WHAT IS THE CHANGE"

	"WHY THE CHANGE"

Now the fun part. While you were working and commiting someone else could have already uploaded changes to the server

There are 2 ways to handle this. Rebasing and Merging workflows (read into the differences)

Here is a rebasing workflow
	
	git fetch origin      	                     # Update your repository's origin/ branches from remote repo
	git rebase origin/master                     # Plop our commits on top of everybody else's
	git checkout master                          # Switch to the local tracking branch
	git pull                                     # This won't result in a merge commit
	git rebase NAME_OF_BRANCH_YOU_WAS_WORKING_ON # Pull those commits over to the "public" branch, you might get a merge conflict at this point in which case
					             # please google how you want to handle it. git mergetool
	git push                                     # Push the public branch back up, with my stuff on the top

Here is a merging workflow
	
	git fetch origin
	git checkout master
	git merge origin/master
	git merge bugFix
	git push


And if you are never gonna work on that feature again cause you are awesome delete the branch

	git branch -d COMPLETED_FEATURE_BRANCH



..................................................................................................


