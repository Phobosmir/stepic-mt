#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char **argv) {
	const int buf_len = 255;
	char buf[buf_len];
	if (fgets(buf, buf_len, stdin) == 0){
		fprintf(stderr, "Error reading from stdin\n");
		exit(EXIT_FAILURE);
	}
	FILE *cmd_output_pipe = popen(buf, "r");
	FILE *fp = fopen("/home/box/result.out", "w");
	while(fgets(buf, buf_len, cmd_output_pipe)){
		fprintf(fp, "%s", buf);
	}
	pclose(cmd_output_pipe);
	fclose(fp);
	return 0;
}
	
