Notepad++ Community Release Hudson Build Testing
================================================
N++CR Support
-------------
Project page: http://github.com/npp-community/npp-community
IRC Support: irc://irc.freenode.net:6667/notepad++
Issue Tracking: http://nppcommunity.lighthouseapp.com

Overview
--------
N++CR Hudson Build Testing is a simple to setup and use build test
config used by the repo maintainer to test branches prior to pushing
any new commits to the public source repo.  Using Hudson and the
following setup reduced build testing times from an 8 hour manual
job to about 40 minutes and one click.

Thank you to the Hudson team!


Requirements
------------
	* N++CR source repository
	* Java Runtime Environment 1.5 or later
	* Hudson Continuous Integration .war file
	* Visual Studio 2005 and 2008
	* Boost TR1 for VS 2005
	* Microsoft Debugging Tools for Windows
		(x86 version is all that is being used at this time)
	* Google Testing Framework
		(both as git sub-module and stand-alone)
	* Google Mock
		(both as a git sub-module and stand-alone)

Setup
------------------
	* The N++CR dev environment should already be setup and you should
	  be able to successfully build using both notepadPlus solution
	  files using GTest/Mock from the git submodules and stand-alone
	  installs.
	* Download hudson.war from http://hudson-ci.org/ and launch it by
	  executing java -jar hudson.war then visit http://localhost:8080/
	* Follow the 'Manage Hudson' link to 'Manage Plugins' -> 'Available'
	  and select the 'Setenv Plugin' for installation.  Click the
	  'install' button at the bottom of the page.
	* Close down Hudson and restart. (ctrl-c in the console running the .war)
	* On the Hudson main dashboard select 'New Job', select the 'Build multi-
		configuration project', name it N++CR Matrix, and click 'Ok'.
	* Close down Hudson again.
	* Copy the contrib\Hudson-ci\config.xml file to .hudson\jobs\N++CR Matrix\ in
		your home directory.
	* Restart Hudson
	* From the main dashboard select the N++CR Matrix job then 'Configure'
	* Scroll down to the 'Build Environment' section and set the NPPCR_ROOT_PATH
	  to the path containing the notepadPlus solution files.

Notes
-------------------
If you do not have or do not want to run all the tests you can alter the values
that are listed in the matrix build.

Enjoy, and don't forget to visit the irc channel!

