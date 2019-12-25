/*
 *	Made for Christmas Day by Crupette
 * */

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <stdlib.h>
#include <getopt.h>

#define VERSION_MAJOR 1
#define VERSION_MINOR 1
#define VERSION_BUGFIX 0

float slope = 1.f;
int height = 25;
int stop = 20;
int width = 4;

char *leaves = "\"'%#$^";
char *leaves_colors[] = {
	"\033[32m",
	"\033[92m"
}; //2

char *decor_colors[] = {
	"\033[31m",
	"\033[91m",
	"\033[33m",
	"\033[93m",
	"\033[34m",
	"\033[94m",
	"\033[35m",
	"\033[95m",
	"\033[36m",
	"\033[96m",
	"\033[37m",
	"\033[97m"
}; //12

char *marquee_text = "MERRY CHRISTMAS";

char helpFlag = 0;
char versionFlag = 0;

static struct option long_options[] = {
	{"help", no_argument, &helpFlag, 1},
	{"version", no_argument, &versionFlag, 1},
	{"width", required_argument, 0, 'w'},
	{"height", required_argument, 0, 'h'},
	{"slope", required_argument, 0, 's'},
	{"branch-height", required_argument, 0, 'b'},
	{"marquee-text", required_argument, 0, 0},
	{NULL, 0, NULL, 0}
};

void help(void){
	printf("christmas-ascii : a terminal based christmas tree\n");
	printf("usage: christmas-ascii [OPTION(S)]\n");
	printf("Prints an ascii christmas tree with randomly placed decoration elements and a marquee text\n");
	printf("Options:\n");
	printf("\t-w --width		Sets the width of the trunk (default: 4)\n");
	printf("\t-h --height		Defines the height of the drawing area (default: 25)\n");
	printf("\t-s --slope		How angled the slope of the leaves are (default: 1.f)\n");
	printf("\t-b --branch-height	Where the leaves end and the trunk begins (default: 20)\n");
	printf("\t--marquee-text	What the marquee will say (default: \"MERRY CHRISTMAS\"\n");
	printf("\t--help		Shows this information\n");
	printf("\t--version		Shows version information\n");
}

int parseargs(int argc, char **argv){
	int c;
	int option_index = 0;
	opterr = 0;
	while((c = getopt_long(argc, argv, "w:h:s:b", long_options, &option_index)) != -1){
		switch(c){
			case '0':

			break;
			case 'w':
				width = atoi(optarg);
			break;
			case 'h':
				height = atoi(optarg);
			break;
			case 's':
				slope = atof(optarg);
			break;
			case 'b':
				stop = atoi(optarg);
			break;
			case 0:
				marquee_text = strdup(optarg);
			break;
			case '?':
				fprintf(stderr, "Unknown option: '%c'.\n", optopt);	
				return 1;
			break;
		}
	}
	if(helpFlag){
		help();
		return 1;
	}
	if(versionFlag){
		printf("christmas-ascii v. %i.%i.%i\n", VERSION_MAJOR, VERSION_MINOR, VERSION_BUGFIX);
		return 1;
	}
	return 0;
}

void print_tree(void){
	float abswid = (float)(stop) * slope;
	float center = abswid / 2.f;

	printf("\033[0;0H");

	for(int i = 0; i < height; i++){
		if(i >= stop){
			float start = abswid - (width / 2);
			printf("\033[%i;%iH", i, (int)start);
			for(int j = 0; j < width; j++){
				printf("\033[33m|");
			}
			continue;
		}
		float start = abswid - (i * slope);
		printf("\033[%i;%iH", i, (int)start);
		for(int j = 0; j < (int)(i * slope) * 2; j++){
			if(rand() % 20 == 0){
				printf("%s@", decor_colors[rand() % 12]);
			}else{
				printf("%s%c", leaves_colors[rand() % 2], leaves[rand() % 6]);
			}
		}
	}
}

char *marquee_buffer = NULL;

void marquee(void){
	if(marquee_buffer == 0){
		marquee_buffer = malloc((stop * slope) * 2);
		memset(marquee_buffer, ' ', (stop * slope) * 2);
		int copy_end = strlen(marquee_buffer) - strlen(marquee_text);
		strcpy(marquee_buffer + copy_end, marquee_text);
	}
	printf("\033[%i;0H", height);
	printf("%s", marquee_buffer);
	char first = marquee_buffer[0];
	for(int i = 0; i < strlen(marquee_buffer) - 1; i++){
		marquee_buffer[i] = marquee_buffer[i + 1];
	}
	marquee_buffer[strlen(marquee_buffer) - 1] = first;
}

int main(int argc, char** argv){
	if(parseargs(argc, argv)) exit(1);
	printf("\033[2J");
	printf("\e[?25l");
	while(1){
		print_tree();
		marquee();
		sleep(1);
	}
}
