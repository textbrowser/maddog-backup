/*
** Copyright (c) 2003 - present, Alexis Megas.
*/

/*
** -- Local Includes --
*/

#include "common.h"

/*
** -- Local Globals --
*/

static int passwdSet(const char *, const char *);
static void passwdInit(const int, const char *);

int main(int argc, char *argv[])
{
  char buffer[BUFF_SIZE];
  char *tmp = 0;
  mode_t mode = 0;
  struct stat stat_buf;

  (void) printf("Content-type: text/html\n\n");
  (void) printf("<html><body>");
  (void) printf("<title>Mad Dog Backup System</title>\n");
  (void) printf("<center>\n");

  if(argc > 1 && argv[1] != 0 && isValidId(argv[1]))
    {
      /*
      ** Does a special file exist?
      */

      if((tmp = getenv("REMOTE_ADDR")) == 0)
	tmp = "1";

      (void) memset(buffer, 0, sizeof(buffer));
      (void) snprintf
	(buffer, sizeof(buffer), "/%s/data/bcksys.loggedin.%s.%s",
	 BACKUP_DIR, argv[1], tmp);

      if(stat(buffer, &stat_buf) == 0 && S_ISREG(stat_buf.st_mode))
	{
	  (void) printf("<meta http-equiv=\"refresh\" "
			"content=\"5; url="
			"%s/display_files.cgi?%s&%d\">",
			CGI_DIR, argv[1], SORTBY_NAME);
	  (void) printf
	    ("It appears that you are already logged in. "
	     "Please wait to be <a href=\"%s/display_files.cgi?%s&%d\">"
	     "redirected</a>.",
	     CGI_DIR, argv[1],
	     SORTBY_NAME);
	}
      else
	{
	  /*
	  ** Does this user have a password set?
	  */

	  /*
	  ** Check a list of valid ids.
	  */

	  if(isValidId(argv[1]) == TRUE)
	    {
	      (void) memset(buffer, 0, sizeof(buffer));
	      (void) snprintf(buffer, sizeof(buffer), "%s/data",
			      BACKUP_DIR);

	      if(stat(buffer, &stat_buf) != 0)
		{
		  mode = (mode_t) PERMISSIONS;
		  (void) umask(~mode);

		  if(mkdir(buffer, mode) != 0)
		    (void) printf(ERROR, __LINE__, __FILE__, HOME);
		  else
		    passwdInit(TRUE, argv[1]);
		}
	      else if(passwdSet(buffer, argv[1]) == TRUE)
		passwdInit(FALSE, argv[1]);
	      else
		passwdInit(TRUE, argv[1]);
	    }
	  else
	    (void) printf(ERROR, __LINE__, __FILE__, HOME);
	}
    }
  else
    (void) printf(ERROR, __LINE__, __FILE__, HOME);

  (void) printf("</center>\n");
  (void) printf("</body></html>");
  return EXIT_SUCCESS;
}

static int passwdSet(const char *path, const char *userid)
{
  /*
  ** Is the password file present?
  */

  char buffer[BUFF_SIZE];
  struct stat stat_buf;

  if(path)
    {
      (void) memset(buffer, 0, sizeof(buffer));
      (void) snprintf(buffer, sizeof(buffer), "%s/passwd.%s", path,
		      userid ? userid : "user");

      if(stat(buffer, &stat_buf) != 0)
	return FALSE;
      else if(S_ISREG(stat_buf.st_mode))
	return TRUE;
      else
	return FALSE;
    }
  else
    return FALSE;
}

static void passwdInit(const int new_id, const char *userid)
{
  (void) printf("<table cellpadding=1 width=\"50%%\" cellspacing=0 border=0 "
		"bgcolor=\"DarkSlateGray\"><tr><td>\n");
  (void) printf("<a href=\"%s\">%sMad Dog Backup System Home</a> >> "
		"Password Entry%s<br>",
		HOME, FBEG, FEND);
  (void) printf("<table width=\"100%%\" border=0 bordercolor=\"tomato\">\n");
  (void) printf("<tr>\n");

  if(new_id == TRUE)
    (void) printf("<th bgcolor=\"lightblue\">Please Choose a Password<br>"
		  "(Password must be at least ten characters long.)</th>\n");
  else
    (void) printf("<th bgcolor=\"lightblue\">Login</th>\n");

  (void) printf("</tr>\n");
  (void) printf("<tr>\n");
  (void) printf("<td bgcolor=\"beige\">\n");
  (void) printf("<form action=\"%s/display_files.cgi?%s&%d\" "
		"method=\"post\">\n", CGI_DIR, userid ? userid : "user",
		SORTBY_NAME);
  (void) printf("Password<br><input type=\"password\" size=32 maxlength=32 "
		"name=\"pass\" value=\"\"><br>\n");

  if(new_id == TRUE)
    (void) printf("Password Confirmation<br><input type=\"password\" size=32 "
		  "maxlength=32 name=\"pass\" value=\"\"><br>\n");

  (void) printf("</td></tr><tr bgcolor=\"beige\" align=center><td>\n");
  (void) printf("<input type=\"submit\" value=\"Enter\">\n");
  (void) printf("</td></tr>\n");
  (void) printf("</form>\n");
  (void) printf("</table>\n");
  (void) printf("</td></tr></table><br>\n");
}
