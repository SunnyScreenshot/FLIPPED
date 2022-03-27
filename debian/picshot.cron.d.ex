#
# Regular cron jobs for the picshot package
#
0 4	* * *	root	[ -x /usr/bin/picshot_maintenance ] && /usr/bin/picshot_maintenance
