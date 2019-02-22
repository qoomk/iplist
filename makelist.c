//atode kesu
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<getopt.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/ioctl.h>
#include<netinet/in.h>
#include<net/if.h>
#include<arpa/inet.h>

/*
makelist -l [file pass+name:ex. /var/tmp/iplist] -i [ip1^3:ex. 10.10.10] -s [ip4 start:ex. 101] -e [ip4 end:ex. 150] 
*/
int main(int argc,char*argv[]){
	int opt;
	char *listfile,*ip123;
	int ip4s,ip4e;

	printf("param %d\n",argc);
	
	if(argc != 9){
		fprintf(stderr,"param %d\n",argc);
		return -1;
	}

	while((opt=getopt(argc,argv,"l:i:s:e:"))!= -1){
		printf("%c %s \n",opt,optarg);
		switch(opt){
			case 'l':
				listfile=optarg;
			break;
			case 'i':
				ip123=optarg;
			break;
			case 's':
				ip4s=atoi(optarg);
			break;
			case 'e':
				ip4e=atoi(optarg);
			break;
		}

	}

	FILE *fp;
	int ip4p,fds,ret;
	char ethip[30],listfilewk[256];
	ethip[0]='\0';
	listfilewk[0]='\0';
	struct ifreq ifr;
	struct sockaddr_in client_addr;

	sprintf(listfilewk,"%s_wk",listfile);

	fp = fopen(listfilewk,"wt");
	if(fp==NULL){
		printf("file open(create) %s\n",listfilewk);
		fprintf(stderr,"file open(create) %s\n",listfilewk);
		return -1;
	}

	for(ip4p=ip4s;ip4p<=ip4e;ip4p++){
		sprintf(ethip,"%s.%d",ip123,ip4p);
		printf("%s\n",ethip);
		fds=socket(AF_INET,SOCK_STREAM,0);
		bzero((char *)&client_addr, sizeof(client_addr));
		client_addr.sin_family = AF_INET;
		client_addr.sin_addr.s_addr = inet_addr(ethip);
		client_addr.sin_port = htons(502);
		ret=connect(fds,(struct sockaddr *)&client_addr,sizeof(client_addr));
		if (ret < 0) {
			printf("connect ng %s %d\n",ethip,ret);
		}else{
			printf("connect ok %s %d\n",ethip,ret);
			fputs(ethip,fp);
		}
	}

	fclose(fp);

	if(rename(listfilewk, listfile)==0){
		remove(listfilewk);
	}else{
		fprintf(stderr,"file rename %s %s\n",listfilewk,listfile);
	}



}
