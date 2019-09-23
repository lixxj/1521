// myls.c very own "ls" implementation
// Completed by XJ

#include <sys/types.h>
#include <sys/stat.h>

#include <dirent.h>
#include <err.h>
#include <errno.h>
#include <fcntl.h>
#include <grp.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef __linux__
	#include <bsd/string.h>
#endif
#include <sysexits.h>
#include <unistd.h>

#define MAXDIRNAME 256
#define MAXFNAME 256
#define MAXNAME 24
#define MAXPATHNAME 512

char *rwxmode (mode_t, char *);
char *username (uid_t, char *);
char *groupname (gid_t, char *);

int main (int argc, char *argv[])
{
	// string buffers for various names
	char uname[MAXNAME+1];
	char gname[MAXNAME+1];
	char mode[MAXNAME+1];

	// collect the directory name, with "." as default
	char dirname[MAXDIRNAME] = ".";
	if (argc >= 2)
		strlcpy (dirname, argv[1], MAXDIRNAME);

	// check that the name really is a directory
	struct stat info;
	
	if (stat (dirname, &info) < 0)
		err (EX_OSERR, "%s", dirname);

	if (! S_ISDIR (info.st_mode)) {
		errno = ENOTDIR;
		err (EX_DATAERR, "%s", dirname);
	}

	// open the directory to start reading
	DIR *df = opendir(argv[1]);

	// read directory entries
	struct dirent *entry = readdir(df);
	while (entry != NULL) {      
		if (entry->d_name[0] == '.') { // ignore the object if its name starts with '.'
			entry = readdir(df);
            continue;
		}

		// get the struct stat info for the object (using its path)
		char path[MAXPATHNAME];
		sprintf (path, "%s/%s", argv[1], entry->d_name);
		lstat(path, &info);
     	
		// print the details using the object name and struct stat info
		printf (
			"%s  %-8.8s %-8.8s %8ld  %s\n", // sizeof st_size is lld on MacOS, ld on Linux
			rwxmode (info.st_mode, mode),
			username (info.st_uid, uname),
			groupname (info.st_gid, gname),
			info.st_size, 
			entry->d_name
		);

        entry = readdir(df);
    }

	// finish up
	closedir(df);

	return EXIT_SUCCESS;
}

// convert octal mode to -rwxrwxrwx string
char *rwxmode (mode_t mode, char *str)
{
	switch (mode & S_IFMT) { // file type
		case S_IFDIR:  str[0] = 'd'; break;
		case S_IFREG:  str[0] = '-'; break;
		case S_IFLNK:  str[0] = 'l'; break;
		default:       str[0] = '?';
	}

	// file permission
	str[1] = (mode & 0b100000000) ? 'r': '-';
	str[2] = (mode & 0b010000000) ? 'w': '-';
	str[3] = (mode & 0b001000000) ? 'x': '-';
	str[4] = (mode & 0b000100000) ? 'r': '-';
	str[5] = (mode & 0b000010000) ? 'w': '-';
	str[6] = (mode & 0b000001000) ? 'x': '-';
	str[7] = (mode & 0b000000100) ? 'r': '-';
	str[8] = (mode & 0b000000010) ? 'w': '-';
	str[9] = (mode & 0b000000001) ? 'x': '-';
	str[10] = '\0';

	return str;
}

// convert user id to user name
char *username (uid_t uid, char *name)
{
	struct passwd *uinfo = getpwuid (uid);
	if (uinfo != NULL)
		snprintf (name, MAXNAME, "%s", uinfo->pw_name);
	else
		snprintf (name, MAXNAME, "%d?", (int) uid);
	return name;
}

// convert group id to group name
char *groupname (gid_t gid, char *name)
{
	struct group *ginfo = getgrgid (gid);
	if (ginfo != NULL)
		snprintf (name, MAXNAME, "%s", ginfo->gr_name);
	else
		snprintf (name, MAXNAME, "%d?", (int) gid);
	return name;
}
