#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define PATH_COUNT 3
const char* battery_paths[PATH_COUNT] = {
	"/sys/class/power_supply/BAT0/capacity",
	"/sys/class/power_supply/BAT1/capacity",
	"/sys/class/power_supply/battery/capacity"
};

void get_battery_perc(int fd, char buffer[1024])
{
	int n = 0;
	while(1)
	{
		if (read(fd, buffer + n, 1) == 0)
			break ;
		n++;
		if (n >= 1024)
			break ;
	}
}

void monitor_battery(int bat)
{
	char buffer[1024 * 2] = {0};
	int perc;
	while (1)
	{
		lseek(bat, 0, SEEK_SET);
		get_battery_perc(bat, buffer);
		perc = atoi(buffer);
		if (perc <= 15)
		{
			snprintf(buffer, (1024 * 2), "notify-send -u critical \"dawg, I\'m running out of power. plug me in [perc: %i]\"\n", perc);
			int status = system(buffer);
			if (status != 0)
				return ;
			sleep(15);
		}
		sleep(5);
	}
}

int main(int ac, char **av)
{
	int i = 0;
	char *program = *av;
	int bat0 = -1;
	for (; i < PATH_COUNT; i++)
	{
		bat0 = open(battery_paths[i], O_RDONLY);
		if (bat0 != -1)
			break ;
	}
	if (bat0 == -1)
	{
		fprintf(stderr, "%s: %s: ", program, battery_paths[i]);
		perror("");
		return (1);
	}
	monitor_battery(bat0);
	return (0);
}
