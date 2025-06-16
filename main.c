#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define PATH_COUNT 3
#define N 1024
const char* status_path = "/sys/class/power_supply/BAT0/status";
const char* battery_paths[PATH_COUNT] = {
	"/sys/class/power_supply/BAT0/capacity",
	"/sys/class/power_supply/BAT1/capacity",
	"/sys/class/power_supply/battery/capacity"
};

void buff_read(int fd, char buffer[N])
{
	int n = 0;
	while(1)
	{
		if (read(fd, buffer + n, 1) == 0)
			break ;
		n++;
		if (n >= N)
			break ;
	}
}

int is_battery_chargin(void)
{
	char buffer[N] = {0};
	int status_fd = open(status_path, O_RDONLY);

	if (status_fd == -1)
		return (-1);
	buff_read(status_fd, buffer);
	if (strcmp(buffer, "Charging") == 0)
		return (1);
	return (0);
}

void get_battery_perc(int fd, char buffer[N])
{
	buff_read(fd, buffer);
}

void monitor_battery(int bat)
{
	char buffer[N * 2] = {0};
	int status;
	int perc;

	while (1)
	{
		lseek(bat, 0, SEEK_SET);
		status = is_battery_chargin();
		if (status)
		{
			sleep(15);
			continue;
		}
		if (status == -1)
		{
			fprintf(stderr, "%s: %s: ", "watcher", "Failed to get the status of the battery");
			perror("");
			break ;
		}
		get_battery_perc(bat, buffer);
		perc = atoi(buffer);
		if (perc <= 15)
		{
			snprintf(buffer, (N * 2), "notify-send -u critical \"dawg, I\'m running out of power. plug me in [perc: %i]\"\n", perc);
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
	close(bat0);
	return (0);
}
