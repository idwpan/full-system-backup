/*
*This program will make some cleaning that you regularly do 
*before the full system backup. And then itll create new dir 
*in your storage device with date, to make the system backup in
*it useng rsync. The program will use system() to connect all the
*command line tools together and automate this process. Lastly 
*its good to note that tho program reads all the commands and 
*your customaized cleaning process from a config file with the 
*following path: ~/.config/sys_backup
*/

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
//For config files managment which also includes error handling header
#include "config_man.h"

void delete_dirs(char *);
void get_date(int *);
char *make_backup_dir(char *, int *);
void backup_sys(char *);

int main() {
	const char *config_desc = {
		"################################################################\n"
		"# This configuration file purposes are to provide to the       #\n"
		"# sys_backup.c program the right cleaning process and storage  #\n"
		"# device path, customized to your own needs and preferences.   #\n"
		"################################################################"
	};
	const char *units_list[] = {
		"DirsToClean", "CleaningCommands", "DevicePath", 
		"\0"
	};
	const char *units_desc[] = {
		"Dirs path you regularly clean, like some cache dirs",
		"{\n"
		"\tCommands for cleaning your sysem, like:\n"
		"\tsudo pacman -Sc (for deleting uninstalled packages\n"
		"\tfrom the cache in arch based distros)\n"
		"}", 
		"Your storage device path hdd, usb or whatever you use",  
		"\0"
	};
	char *config_path = "/.config/sys_backup";
	char config_full_path[60];
	char *home, *configurations, *backup_path;
	int date[3]; //Array for the date of today
	unsigned int i;

	/*Get configuration's file path*/
	home = getenv("HOME"); //Get home path 
	//Check for overflow 
	check_input_size(sizeof(config_full_path), strlen(home)+strlen(config_path), "sys_backup.c -> main()");
	strcpy(config_full_path, home);
	strcat(config_full_path, config_path);
	if(check_file_existence(config_full_path) == 0) { //Check if config file exists
		create_config_file(config_full_path, config_desc);
		for(i=0; i<3; i++)
			write_config_unit(config_full_path, *(units_list+i), *(units_desc+i));
		printf("Please configure the prorgram's config file in the following path: %s\n", config_full_path);
		return 0;
	}
	/*Read the configured units*/
	while(i<3) {
		configurations = read_config_unit(config_full_path, *(units_list+i));
		switch(i) {
			/*System Cleaning*/
			case 0: //DirsToClean 
				delete_dirs(configurations);
				break;
			case 1: //CleaningCommands
				system(configurations);
				break;
			/*System Backup*/
			case 2: //DevicePath
				get_date(date); //Get the date of today and pass it to the date array
				backup_path = make_backup_dir(configurations, date); //Create backup dir and get its path
				backup_sys(backup_path); //Backup system in the created dir
				break;
		}
		i++;
	}
	return 0;
}

/*
*This function will delete the given argument dirs.
*/
void delete_dirs(char *dirs_path) {
	char full_command[700];
	const char *command = "rm -rf ";

	//Check for overflow
	check_input_size(sizeof(full_command), strlen(dirs_path)+strlen(command), "sys_backup.c -> delete_dirs()");
	//Copy command to buffer (rm -rf dirs_path) to delete dirs.
	strcpy(full_command, command);
	strcat(full_command, dirs_path);

	system(full_command); //Execute command
}

/*
*This function will get the date of today, and it'll insert it to the passed date array.
*/
void get_date(int *date) {
	long int sec_since_epoch;
	struct tm current_time, *time_ptr;

	sec_since_epoch = time(0); 
	time_ptr = &current_time; //Set time pointer to the current_time struct
	localtime_r(&sec_since_epoch, time_ptr);

	//Pass today's date to the array 
	*date = time_ptr->tm_mday;
	*(date+1) = time_ptr->tm_mon + 1; //+1 because months range from 0 - 11
	*(date+2) =  time_ptr->tm_year - 100; //-100 because tm_year is number of passed years since 1900
}

/*
*A function that gets pointer to int array that contains the
*date of today and create a backup dir in the passed path 
*passed date. Then it will return the full path of the created dir. 
*/
char *make_backup_dir(char *device_path, int *date_array) {
	char dir_name[10], full_command[200];
	const char *command = "mkdir ";
	static char backup_path[150]; //The returned full backup path 

	//Convert the date_array to a string so will use it to name the dir in the device path
	sprintf(dir_name, "%02d", *date_array);
	sprintf((dir_name+3), "%02d", *(date_array+1));
	sprintf(dir_name, "%02d", *date_array);
	sprintf(dir_name+6, "%d", *(date_array+2));
	dir_name[2] = dir_name[5] = '-';
	//Check for overflow
	check_input_size(sizeof(backup_path), strlen(device_path)+strlen(dir_name), "sys_backup.c -> make_backup_dir()");
	strcpy(backup_path, device_path);
	strcat(backup_path, dir_name); //Complete the full dir path 
	//Check for overflow
	check_input_size(sizeof(full_command), strlen(command)+strlen(backup_path), "sys_backup.c -> make_backup_dir()");
	strcpy(full_command, command); //Insert command to the full_command
	strcat(full_command, backup_path); //Complete the command
	
	system(full_command); //Execute the command 
	return backup_path;
}

/*
*This function will make the full system backup using rsync to the passed passed.
*/
void backup_sys(char *backup_path) {
	const char *backup_command = "sudo rsync -aAXHv --exclude={\"/dev/*\",\"/proc/*\",\"/sys/*\",\"/tmp/*\",\"/run/*\",\"/mnt/*\",\"/media/*\",\"/lost+found\"} / ";
	char command_buffer[500];

	/*Prepare command*/
	//Check for overflow
	check_input_size(sizeof(command_buffer), strlen(backup_command)+strlen(backup_path)+sizeof("/"), "sys_backup.c -> make_backup_dir()");
	strcpy(command_buffer, backup_command);
	strcat(command_buffer, backup_path);
	strcat(command_buffer, "/");
	
	system(command_buffer); //Execute the command 
}