#include <semaphore.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>     
#include <sys/shm.h>	 
#include <sys/sem.h>	 
#include <unistd.h>		
#include <stdlib.h>

sem_t *m;
int player[5];
int ita_count=0,ger_count=0,count_777=0,count_333=0,marked_gp=0,marked_ip=0;

void tram_leaves(int gp_code,int no_gp,int ip_code,int no_ip,int *play,int *tram_no);
void count_of_7_3(int *play);
void player_combination(int *play,int *tram_no,int *count);
void germanHere(int *count,int *play,int *tram_no);
void italyHere(int *count,int *play,int *tram_no);

int main()
{   
    int shm_id1,shm_id2,shm_id3;	      	       
    char *shm_addr1; 	 
    char *shm_addr2; 
    char *shm_addr3;       
    int *count;
    int *play;
    int *tram_no; 
    pid_t pid;

    
    m = mmap(0, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    sem_init(m,1,1);		       
    
    shm_id1 = shmget(888, 10 , IPC_CREAT | IPC_EXCL | 0600);//change first value each time before compiling
    
    shm_id2 = shmget(898, 10 , IPC_CREAT | IPC_EXCL | 0600);//change first value each time before compiling
    
    shm_id3 = shmget(899, 10 , IPC_CREAT | IPC_EXCL | 0600);//change first value each time before compiling
    

    shm_addr1 = shmat(shm_id1, NULL, 0);
    
    shm_addr2 = shmat(shm_id2, NULL, 0);
    
    shm_addr3 = shmat(shm_id3, NULL, 0);
    

    count = (int*) shm_addr1;
    *count = 0;
    tram_no = (int*) shm_addr2;
    *tram_no = 0;
    play = (int*) shm_addr3;
    *play = player[0];
    
    pid = fork();
    switch (pid) {
	case -1:
	    perror("fork: ");
	    exit(1);
	    break;
	case 0:
	 for(int i=0;i<23;i++){
	    italyHere(count,play,tram_no);
	    }
	    exit(0);
	    break;
	default:
	for(int i=0;i<23;i++){
	    germanHere(count,play,tram_no);
	    }
	    break;
    }

    wait(NULL);

    return 0;
}

void tram_leaves(int gp_code,int no_gp,int ip_code,int no_ip,int *play,int *tram_no)
{
if((no_gp>no_ip)||(no_ip>no_gp)||(no_gp==no_ip)){
	if((no_ip==0)||(no_gp==0)){
	   if(no_ip==0){
	      for(int i=0;i<5;i++){
	      if(*(play+i)==gp_code){
				if(marked_gp!=no_gp){
				*(play+i)=0;
				marked_gp++;
				}
				else{
				marked_gp=0;
				}
					}
	      }
	    }
	   else{
	   for(int i=0;i<5;i++){
	    if(*(play+i)==ip_code){
	      if(marked_ip!=no_ip){
			*(play+i)=0;
			marked_ip++;
			}
			else{
			marked_ip=0;
				}
			}
	      }
	   }
	}
	else{
	     for(int i=0;i<5;i++){
		if((*(play+i)==gp_code) || (*(play+i)==ip_code)){
			if(*(play+i)==gp_code){
				if(marked_gp!=no_gp){
				*(play+i)=0;
				marked_gp++;
				}
				else{
				marked_gp=0;
				}
					}
			else{
			if(marked_ip!=no_ip){
			*(play+i)=0;
			marked_ip++;
			}
			else{
			marked_ip=0;
				}
			}
		}
		else{
		} 
	}
}
}
(*tram_no)++;
printf("tram number %d leaves\n",*tram_no);
for(int i=0;i<5;i++){
if(*(play+i)!=0){
	if(*(play+i)==777){
	printf("german player is waiting to board the tram\n");
	}
	else {
	printf("italian player is waiting to board the tram\n");
	}
	break;
}
}
marked_gp=0;
marked_ip=0;
count_333=0;
count_777=0;
}

void count_of_777_333(int *play){
for(int i=0;i<5;i++){
if(*(play+i)==777){
count_777++;
}
else if(*(play+i)==333){
count_333++;
}
else{
}
}
}
void player_combination(int *play,int *tram_no,int *count){
	count_of_777_333(play);
	if((count_777==3 && count_333==1)||(count_333==3 && count_777==1))
	{
	printf("Unsafe combination:waiting for another player\n");
	count_333=0;
	count_777=0;
	return;
	}
	else{
	if((*count)==5){
	 *count=1;
	}
	else{
	*count=0;
	}
	 if((count_777>=2 && count_333==2)||(count_333>=2 && count_777==2))
	{
        tram_leaves(777,2,333,2,play,tram_no);
	}
	else if(count_777==4||count_333==4)
	{
        	if(count_777==4){
        	tram_leaves(777,4,333,0,play,tram_no);
        	}
        	else {
        	tram_leaves(777,0,333,4,play,tram_no);
        	}
	}
    }

}
void germanHere(int *count,int *play,int *tram_no){
sem_wait(m);
(*count)++;
printf("german player\n"); 
for(int i=0;i<5;i++){
	if((*(play+i)!=777) && (*(play+i)!=333)){
	*(play+i)=777;
	break;
	}	
}
if((*count)>=4){
   player_combination(play,tram_no,count);     
}
else{
//tram not full yet
}
sem_post(m);
}
void italyHere(int *count,int *play,int *tram_no){
sem_wait(m);
(*count)++;
printf("italy player\n"); 
for(int i=0;i<5;i++){
	if((*(play+i)!=777) && (*(play+i)!=333)){
	*(play+i)=333;
	break;
	}	
}
if((*count)>=4){
   player_combination(play,tram_no,count);     
}
else{
//tram not full yet
}
sem_post(m);
}
