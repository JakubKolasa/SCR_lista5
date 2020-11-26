#include <stdio.h> 
#include <unistd.h> 
#include <fcntl.h> 

#define buf_size 512
#define msg_size 3

int main(int argc, char *argv[]) {
    int in_fd;
    int  cnt;
    char buf[buf_size]; //
    int fd[2]; //file descriptor

    if(pipe(fd) < 0) { // == -1 kiedy blad
      fprintf(stderr, "Nie udalo sie utworzyc potoku\n");
      return -1;
    }

    pid_t pid;
    pid = fork(); 

    if(pid == 0){ // Proces dziecko
      close(fd[1]); //Brak zapisu
      while( read(fd[0], &buf, msg_size) > 0){
	write(STDOUT_FILENO, "#", 1);
	write(STDOUT_FILENO, &buf, msg_size);
	write(STDOUT_FILENO, "#\n", 2);
      }
    }

    else if(pid > 0){ // Proces rodzic
      close(fd[0]); //Brak odczytu
      if( (in_fd = open(argv[1], O_RDONLY)) < 0){
	fprintf(stderr, " Nie udalo sie otworzyc pliku");
	return -2;
      }
      while((cnt = read(in_fd, &buf, buf_size)) > 0) { // read(const char* Path, int flags);
	if(write(fd[1], &buf, cnt) < 0) { // size_t write (int fd, void* buf, size_t cnt);
	  fprintf(stderr, "Blad w zapisie do potoku");
	  return -3;
	}
      }
    }
      else {
      fprintf(stderr, "Bledny PID");
      }
    close(in_fd);    
}
