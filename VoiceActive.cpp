#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <termios.h>
#include <unistd.h>
#include <sstream>
#include <sys/ioctl.h>

using namespace std;

float activationvol = 2.0f;
int kbhit(void);
int getch();
bool running = true;

void Speak(string message) {
	string run = "tts ";
    run += message;
    system(run.c_str());
}

void Init() {
	system("gpio export 17 out");
}

void Process(FILE *cmd, string message) {
	char user[200];
    user[0] = '\0';
    if(message.c_str() == "Hallo") {
		printf("Hallo");
	} else if (message == "Licht an") {
		Speak(message);
		system("gpio -g write 17 1");
	} else if (message == "Licht aus") {
		Speak(message);
		system("gpio -g write 17 ^0");
	} else if (message == "Musik 1") {
		Speak(message);
		system("mpg123 /root/HellnBack.mp3");
	} else if (message == "Musik 2") {
		Speak(message);
		system("mpg123 /root/Poison.mp3");
	} else if (message == "Tür auf") {
		Speak(message);
		system("curl -dump http://192.168.1.14/?1t >/dev/null 2>&1");
	} else if (message == "Tor auf") {
		Speak(message);
		system("curl -dump http://192.168.1.14/?2t >/dev/null 2>&1");
	} else if (message == "Tür auf") {
		Speak(message);
		system("curl -dump http://192.168.1.14/?1t >/dev/null 2>&1");
	} else if (message == "aktivieren") {
		Speak(message);
		system("curl -dump http://192.168.1.25/?allon >/dev/null 2>&1");
	} else if (message == "letzter Benutzer") {
		Speak(message);
		cmd = popen("curl http://192.168.1.25/?user","r");
		fscanf(cmd,"%s",user);
		fclose(cmd);
		string userid = user;
		printf("user: %s\n", user);
		if (userid == "DC3BEA7") {
			Speak("Der Letzte Benutzer war Colin. Öffne Türe");
			system("curl -dump http://192.168.1.25/?1on >/dev/null 2>&1");
		} else if (userid == "923B061") {
			Speak("Der Letzte Benutzer war Bernd");
		} else {
			Speak("Benutzer nicht erkannt");
		}
	} else if (message == "stop") {
		running = false;
	} else {
		printf("Befehl nicht erkannt");
		Speak("Befehl nicht erkannt!");
		Speak(message);
	}
}

float GetVolume(FILE *cmd) {
    float vol = 0.0f;
    system("arecord -D plughw:1,0 -f cd -t wav -d 2 -r 16000 /dev/shm/noise.wav >/dev/null 2>&1");
    cmd = popen("sox /dev/shm/noise.wav -n stats -s 16 2>&1 | awk '/Max/ {print $3}'","r");
    fscanf(cmd,"%f",&vol);
    fclose(cmd);
    return vol;
}

int main(int argc, char* argv[]) {
    FILE *cmd = NULL;
    char message[200];
    message[0] = '\0';
    float volume = 0.0f;
	char ch;
	
	Init();
	printf("Voice Active 3.0 running\n");
    while(running) { //loop until running=false
        volume = GetVolume(cmd);
        if(volume > activationvol) {
				Speak("Ja?");
			    cmd = popen("listen.sh","r");
				fscanf(cmd,"\"%[^\"\n]\"",message);
				fclose(cmd);
				printf("message: %s\n", message);
				Process(cmd,message);
				message[0] = '\0'; //this will clear the first bit
        }
		if(kbhit()) { // Get Key events
			ch = getch();
			if(ch == 27) {
				running = false;
			}
		}
    }
	return 0;
}

int getch() { 
    static int ch = -1, fd = 0; 
    struct termios neu, alt; 
    fd = fileno(stdin); 
    tcgetattr(fd, &alt); 
    neu = alt; 
    neu.c_lflag &= ~(ICANON|ECHO); 
    tcsetattr(fd, TCSANOW, &neu); 
    ch = getchar(); 
    tcsetattr(fd, TCSANOW, &alt); 
    return ch; 
}

int kbhit(void)
{
  int cnt = 0;
  int error;
  static struct termios Otty, Ntty;
 
 
  tcgetattr( 0, &Otty);
  Ntty = Otty;
 
  Ntty.c_iflag          = 0;       /* input mode                */
  Ntty.c_oflag          = 0;       /* output mode               */
  Ntty.c_lflag         &= ~ICANON; /* raw mode */
  Ntty.c_cc[VMIN]       = CMIN;    /* minimum time to wait      */
  Ntty.c_cc[VTIME]      = CTIME;   /* minimum characters to wait for */
 
  if (0 == (error = tcsetattr(0, TCSANOW, &Ntty))) {
    error += ioctl(0, FIONREAD, &cnt);
    error += tcsetattr(0, TCSANOW, &Otty);
  }
 
  return ( error == 0 ? cnt : -1 );
}