# Full System Backup
For every linux geek who can't stop tinkering his system, can't  
stop cleaning it and do all the house keeping to make it the best,  
this repo is for you.  
I just created rsync and few commad line tools wrraper program that can  
do all the full system backup and cleaning process automatically and save  
a lot of time.  
This program will read configurations from ~/.config/sys_backup, so just make  
sure to configure the file correctly.

## Dependencies
1. rsync, backup program.  
2. gcc, C compiler.  
3. git, to clone this repo.
You can install these packages using your distribution's package manager.  

## Usage
1. Make sure all the dependencies are installed, if not install them.  
2. Clone this repo by typing:  

	```git clone https://github.com/yankh764/full-system-backup.git```

3. Move to the cloned directory and compile this program using gcc. For example:

	```sudo gcc -o /usr/local/bin/sys_backup sys_backup.c```

4. Run the program from terminal -using the name you chose to compile it with-,  
it'll create a configuration file in the .config directory.
Configure it to your needs and prefrences -all instructions are written  
in the configuration file-, then make sure that the storage device is mounted, run  
the program one more time and it should create a backup directory in storage device  
and then backup all your system there. 
