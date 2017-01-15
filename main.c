#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>
//#include <netdb.h>
#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#include <Windows.h>
#include <ws2tcpip.h>
#include <conio.h>
#define close closesocket
//#include <unistd.h>
int sock_fd = -1;
struct sockaddr_in sock_addr;
char buffer[15];
void wf(char *loca,char *data){
		   FILE *f = fopen(loca, "wb+");
           fwrite(data, 1, strlen(data), f);
           fclose(f);
}
char* rfds(char * loca)
{   
    FILE* file = fopen(loca, "r+");
    if (file == NULL) {
        printf("File not found\nWriting %s.Default ip set to 192.168.1.1.\nPress O to edit ",loca);
		fclose(file);
		wf("HIDS.conf","192.168.1.1");
		return "192.168.1.1";
        //didloc = true;
    }
    else {
        fseek(file, 0, SEEK_END);
        off_t size = ftell(file);
        fseek(file, 0, SEEK_SET);
        //if(!buffer)
        //printf("error while allocating buffer\n");
        off_t bytesRead = fread(buffer, 1, size, file);

        fclose(file);
        if (size != bytesRead)
            printf("error");
		  
    }
	//puts(buffer);
	return buffer;
}
int connect_to_3ds(const char *addr)
{
	sock_fd = socket(AF_INET, SOCK_DGRAM, 0);

	struct addrinfo hints;
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_DGRAM;

	memset(&sock_addr, 0, sizeof(sock_addr));
	sock_addr.sin_family = AF_INET;
	sock_addr.sin_port = htons(4950);

	struct addrinfo *res;
	int r = getaddrinfo(addr, "4950", &hints, &res);
	if (r != 0)
	{
		close(sock_fd);
		return 1;
	}

	struct addrinfo *s;
	for (s = res; s != NULL; s = s->ai_next)
	{
		memcpy(&sock_addr, s->ai_addr, s->ai_addrlen);
	}
	freeaddrinfo(res);

	return 0;
}

void send_frame(uint32_t hid_state,uint32_t circle_state)
{
	if(sock_fd == -1) return;
	char v[12];
	//uint32_t hid_state = 0x7FF;
	//uint32_t circle_state = 0x800800;
	uint32_t touch_state = 0x2000000;
    memcpy(v, &hid_state, 4);
	memcpy(v + 8, &circle_state, 4);
	memcpy(v + 4, &touch_state, 4);
	int l = sendto(sock_fd, v, 12, 0, (struct sockaddr*)&sock_addr, sizeof(struct sockaddr_in));
    
}
void wait( int seconds )
{   // Pretty crossplatform, both ALL POSIX compliant systems AND Windows
 
        Sleep( seconds );
   
}
uint32_t parsekey(char k)
{   uint32_t button =0x000;
    #define BIT(n) (1U<<(n))
	if((k=='A')||(k=='a'))
	button=BIT(0);
    else if((k=='B')||(k=='b'))
	button=BIT(1);
    else if((k=='S')||(k=='s'))
	button=BIT(2);
    else if((k=='D')||(k=='d'))
	button=BIT(3);
    else if((k=='H')||(k=='h'))
    button=BIT(4);
    else if((k=='K')||(k=='k'))
	button=BIT(5);
    else if((k=='U')||(k=='u'))
	button=BIT(6);
    else if((k=='J')||(k=='j'))
	button=BIT(7);
    else if((k=='R')||(k=='r'))
	button=BIT(8);
    else if((k=='L')||(k=='l'))
	button=BIT(9);
    else if((k=='X')||(k=='x'))
	button=BIT(10);	
/*
    KEY_SELECT  = BIT(2),       ///< Select
	KEY_START   = BIT(3),       ///< Start
	KEY_DRIGHT  = BIT(4),       ///< D-Pad Right
	KEY_DLEFT   = BIT(5),       ///< D-Pad Left
	KEY_DUP     = BIT(6),       ///< D-Pad Up
	KEY_DDOWN   = BIT(7),       ///< D-Pad Down
	KEY_R       = BIT(8),       ///< R
	KEY_L       = BIT(9),       ///< L
	KEY_X       = BIT(10),      ///< X
	KEY_Y       = BIT(11),      ///< Y
*/
    else if((k=='Y')||(k=='y'))
	button=BIT(11);
    
 	
else 
{   
    puts("Uh,key not found");
    puts("Setting it to 0xFFF(No key is being pressed)");
}
    uint32_t hid_buttons =0xfffff000;
    hid_buttons |= button;
    uint32_t hid_state = ~hid_buttons;
	return hid_state;
}

int main(int argc, char **argv)
{ /*
char a;
puts("      Please enter a key");
printf("\n       A for A\n       B for B\n       X for X\n\n");
scanf("%c",&a);
printf("          You Entered:%c\n",a);
puts("Enter the delay between key press in seconds");
int i;
scanf("%d",&i);
printf("Simulating a keypress after every %d seconds\n",i);*/
WSADATA wsaData;
	int winsock_res = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (winsock_res != 0)
	{
		printf("winsock startup failed!\n");
		return 1;
	}
char abkj;
int ab=0;
puts("Welcome to HIDS!!");
char *ip=rfds("HIDS.conf");
printf("IP is %s\n",ip);
printf("Enter the no of keys to be simulated\n");
scanf("%d",&ab);
printf("No of keys to be simulated |%d|\n",ab);
char bn[ab];
uint32_t t[ab];
uint32_t tb[8]={0x800800,0x800800,0x800800,0x800800,0x800800,0x800800,0x800800,0x800800};
//printf("bn size %d",sizeof(bn));
int i=0;
int x=0;
printf("Enter the keys.(Press O to edit ip)\n");
do{   
    printf("Enter key :\n");
    bn[i]=getch();
	if((bn[i]=='O')||(bn[i]=='o'))
	{ char ipw[20];
	  puts("Put in 3ds's IP:");
	  scanf("%s",ipw);
	  wf("HIDS.conf",ipw);
	  strcpy(ip,ipw);
	  puts(ip);
	}
	else if((bn[i]=='r')||(bn[i]=='R'))
	{
	  tb[0]=0x1FFF800;
	  tb[1]=0x1FFFFFF;
	  tb[2]=0x1800FFF;
	  tb[3]=0x1000FFF;
	  tb[4]=0x1000800;
	  tb[5]=0x1000000;
	  tb[6]=0x1800000;
	  tb[7]=0x1FFF000;
	  i++;
	}
	else{
	printf("Key to be simulated:%C\n",bn[i]);
	t[i]=parsekey(bn[i]);
	i++;}
	} while(i<ab);

//puts(bn);
int j=0;



if(connect_to_3ds(ip))
	{
		printf("Failed to connect to '%s'!\n", ip);
		return 1;
	}
	else puts("Connected\n\n");
	char l;
	int k=0;
while(1)
{   printf("Sending input.Press (Q) to quit and (P) to pause\r");
    //wait(i);
	if(t[j]==0x401BAE) t[j]=0xFFF;
	send_frame(t[j],tb[k]);
	printf("\n\n0x%x,0x%x\n",t[j],tb[k]);
	send_frame(0xFFF,tb[k]);
	j++;
	k++;
	if(j==ab)j=0;
	if(k==8)k=0;
	if(kbhit()){
            l = getch();
            if(l=='q')break;
			if(l=='p')
			{
				puts("Press Enter to continue");
				char hl[1];
				while(1)
			    if(kbhit())
				{
				scanf("%c",&hl);
				break;
				}
			}
}
}
send_frame(0xFFF,0x800800);
WSACleanup();
return 0;
}


