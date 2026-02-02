# A Version Control System is a tool that keeps track of changes in your files over time.
👉 It remembers:
what you changed
when you changed
who changed it
and lets you go back to an old version if something breaks


# Git
it is a version controling system 
# Git Configuration
we need to configure git in our system so that git knows who we are and.This configuration is saved once and will be used for all the project.
Git saves this information in the git global config file

# To know currect configuration
-> git config --global user.name
->git config --global user.email
or
->git config --global --list
# To set new configuration
-> git config --global user.name "Abhishek"
->git config --global user.email "abhishek.verma674@gmail.com"


# Initializing Git Project
-> git init
it will create a .git folder .This folder stores all history, commits, branches

# Adding files to git
Git does not automatically track files.
You must explicitly tell Git.
to add file in git 
-> git add <filename>
->git add <file1.cpp> <file2.cpp>
-> git add .

# Removing files from git 
-> git rm <filename>
now this file will not be tracked by git 
After git rm
You must commit:

# git commit
->git commit -m "msg"
saves the current staged changes permanently
📌 Once committed, your code is safe in Git history.
# before commiting you need to stage the file by using git add 

# Git log
-> git log
-> git log --oneline
to get details of all the commit you have made


# to see the diff of what has changed but not staged
-> git diff

# SHA
Every commit gets a SHA hash
That SHA is the identity (ID) of the commit
Git uses this ID to find, track, and link commits

# revert
git revert undoes a commit safely.
It does NOT delete history.
Instead, it creates a new commit that reverses the changes of an old commit.
-> git revert <commit-sha>

# RESET
->git reset --hard <commit-sha>
git reset moves the HEAD (current pointer) to another commit.
If you accidentally run:
git reset --hard
You may recover using:
git reflog


# git vs github

Git is a version control tool.
📌 It runs on your computer
📌 Works offline
📌 Tracks code changes
📌 Manages versions and history

🔹 What is GitHub?
GitHub is a cloud platform that hosts Git repositories.
📌 Runs on the internet
📌 Stores Git projects online
📌 Helps teams collaborate


# Git remote
A remote is a connection to another Git repository, usually on GitHub.
📌 It tells Git where your online repository is.
or 
To connect local repository with a remote repository so that other user can take pull and push .

## add remote
->git remote add <name> <url>
example
git remote add origin https://github.com/username/repo.git
we can also rename and change the url of remote


# Branch
Check Current Branch
->git branch
<!-- jaha se nayi branch create krni h phle us branch pr jao fir yaha se nayi branch create kro -->
<!-- Create a New Branch -->
->git branch <branch name>
<!-- Switch to a Branch -->
->git checkout <branch name>
<!-- Create + Switch Together -->
git checkout -b <branch name>
<!-- Diagram After Commit
A --- B --- C   (main)
              \
               D --- E   (feature-login) -->

<!-- Types of Merge
1️ Fast Forward Merge
Happens when:
main has no new commits
Git just moves pointer
main → C
feature → E
After merge: 
main → E
-->
<!-- 2. Three-Way Merge
When main also has new commits:

A --- B --- C --- F   (main)
              \
               D --- E   (feature)


After merge:

A --- B --- C --- F --- M   (main)
              \         /
               D --- E


✔ Merge commit M created -->

# In 3 way merge Merge conflict may occur
Happens when:
Same file
Same line
Modified in both branches

# Delete Branch
Local branch:
git branch -d feature-login
Force delete:
git branch -D feature-login

# Mergin branch
First go on the branch you want in which youwant to merge you branch
->git merge <name of brancg you want to merge>

# Git Merge vs Git Rebase
Both are used to bring changes from one branch to another, but they do it differently.
# Git Merge
Merge combines the history of two branches and creates a new merge commit.
# Git Rebase
take all commits from your branch and “move” them to be on top of another branch (the new base)

<!-- 
A ── B ── C   (main)
          \
           D ── E   (feature)

A ── B ── C ── F ── G   (main)
          \
           D ── E   (feature)
rebase
A ── B ── C ── F ── G ── D' ── E'  (feature) 

-->

in merge we  take all commmit and make a single commit for all commit so we want to back on previous commit we cannot(on D or E because of loss of information)

but in case of rebase we can go back to D and E(see exmaple of both rebase and merge)

# What is Git Stash

Git stash temporarily saves your uncommitted changes so you can work on something else, and come back to them later.
k of it as a “shelf” where you put your unfinished work temporarily.

#  Why Use Stash?
You are in the middle of a task
Suddenly you need to switch branch to fix a bug
But your current changes are not ready to commit
❌ Committing half-done work is bad
✅ Stash allows you to save work without committing