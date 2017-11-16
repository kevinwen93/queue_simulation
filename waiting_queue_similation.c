// Project1-PartB
// Team Member: Zining Wen & Ran An

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>
struct queue{
  int arrtime;
  int subtask;
  int durlist[32];
  struct queue * next;
}*front0,*rear0,*front1,*rear1,*temp0,*temp1,*tempscan0,*tempscan1,*customer0,*customer1;

void modepara(float arr0r,float arr1r,float serr,int tot);
void modefile(FILE * fp);

int main(int argc, char *argv[])
{
  //Mode 2(File)
  if(argc==2){
    FILE * fp = fopen(argv[1],"r");
    modefile(fp);
  }
  //Mode 1(Parameter)
  else{
    srand(time(NULL));
    float arr0r=atof(argv[1]);
    float arr1r=atof(argv[2]);
    float serr=atof(argv[3]);
    int tot=atoi(argv[4]);
    modepara(arr0r,arr1r,serr,tot);
  }
  return EXIT_SUCCESS;
}

void createqueuehigh(void){
  front0=rear0=NULL;
}
void createqueuelow(void){
  front1=rear1=NULL;
}

//Push new node into different queues for 1 and 0
void enque1(int subtask, int arrtime, int durlist[])
{
  int ind;
  if(rear1 == NULL)
    {
      rear1 = (struct queue *)malloc(sizeof(struct queue));
      rear1->next = NULL;
      rear1->subtask = subtask;
      rear1->arrtime = arrtime;
      for (ind = 0; ind < subtask;ind++){
	rear1->durlist[ind] = durlist[ind];	
      }
      front1 = rear1;
    }
  else
    {
      temp1 = (struct queue *)malloc(sizeof(struct queue));
      temp1->next = temp1;
      temp1->subtask = subtask;
      temp1->arrtime = arrtime;
      for (ind = 0; ind < subtask;ind++){
	temp1->durlist[ind] = durlist[ind];	
      }
      temp1->next = NULL;
      rear1 = temp1;
    }
}
void enque0(int subtask, int arrtime, int durlist[])
{
  int ind;
  if(rear0 == NULL)
    {
      rear0 = (struct queue *)malloc(sizeof(struct queue));
      rear0->next = NULL;
      rear0->subtask = subtask;
      rear0->arrtime = arrtime;
      for (ind = 0; ind < subtask;ind++){
	rear0->durlist[ind] = durlist[ind];	
      }
      front0 = rear0;
    }
  else
    {
      temp0 = (struct queue *)malloc(sizeof(struct queue));
      rear0->next = temp0;
      temp0->subtask = subtask;
      temp0->arrtime = arrtime;
      for (ind = 0; ind < subtask;ind++){
	temp0->durlist[ind] = durlist[ind];	
      }
      temp0->next = NULL;
      rear0 = temp0;
    }  
}

//Delete first node of queue 1&0
void deque1(void)
{
  if(tempscan1 != NULL)
    {
      tempscan1->next = customer1->next;
      free(customer1);
      customer1 = front1;
      tempscan1 = NULL;
    }
  else if(customer1->next != NULL)
    {
      front1 = customer1->next;
      free(customer1);
      customer1 = front1;
    }
  else
    {
      free(customer1);
      front1 = NULL;
      rear1 = NULL;
    }
}

void deque0(void)
{
  if(tempscan0 != NULL)
    {
      tempscan0->next = customer0->next;
      free(customer0);
      customer0 = front0;
      tempscan0 = NULL;
    }
  else if(customer0->next != NULL)
    {
      front0 = customer0->next;
      free(customer0);
      customer0 = front0;
    }
  else
    {
      
      free(customer0);
      front0 = NULL;
      rear0 = NULL;
      //printf("inner\n");
    }
}


void scancheck0(void){
  //printf("yeah\n");
  if (front0 == NULL || customer0 == NULL){
    //  printf("checkafter\n");
    return;
  }
  customer0 = customer0 -> next;
  //printf("after\n");
}

void scancheck1(void){
  //printf("yeah\n");
  if (front1 == NULL || customer1 == NULL){
    //    printf("checkafter\n");
    return;
  }
  customer1 = customer1 -> next;
  //printf("after\n");
}

int sort(const void*a, const void *b)
{
  return( *(int*)a - *(int*)b);
}

int unirandgen(void){
  int ran = rand() % 32 + 1;
  return ran;
}

int randgen(float para){
  float ran = ((float)rand()/(RAND_MAX));
  int ret=ceil(-1*log(1-ran)/para);
  return ret;
}

int  idleupdate(int proclock[], int arrtime, int tasknum){
  int ind;
  int idletot = 0;
  for (ind = 0; ind < tasknum; ind++){
    idletot += arrtime - proclock[ind];
    proclock[ind] = arrtime;
  }
  return idletot;
}

float calbalfac(int durlist[], int tasknum){
  qsort(durlist,tasknum,sizeof(int),sort);
  return (durlist[tasknum - 1] - durlist[0]); 
}



int clocking(int pl,int * clock0,int * clock1, float arr0,float arr1,int tot0, int tot1,int tot){
  int rand0;
  int rand1;
  if(tot1==tot){
    rand0=randgen(arr0);
    *clock0+=rand0;
    return 0;
  }
  if(tot0==tot){
    rand1=randgen(arr1);
    *clock1+=rand1;
    return 1;
  }
  if (pl!=1){
    rand0=randgen(arr0);
    *clock0+=rand0;
  }
  if (pl!=0){
    rand1 = randgen(arr1);
    *clock1+=rand1;
  }
  if(* clock1 >= * clock0){
    return 0;
  }
  else{;
    return 1;
  }
}

void updateproclock(int proclock[],int durlist[], int tasknum)
{
  int ind;
  for (ind = 0; ind < tasknum; ind++){
    proclock[ind] += durlist[ind];
  }
}

void scan(int * avipro, int proclock[],int *waiting0, int* waiting1,int * countque, int * counttot, int * idletot)
{
  int idletime;

  int updateconttot = 0;
  customer0 = front0;
  while(front0 != NULL && customer0  != NULL)
    {
      if(customer0->subtask <= *avipro)
	{    	  
	  if(proclock[customer0->subtask-1]-customer0->arrtime > 0){
	    *waiting0 += proclock[customer0->subtask-1]-customer0->arrtime;
	  }
	  idletime = customer0->arrtime - proclock[*avipro -1];
	  if (idletime > 0){
	    *idletot += idleupdate(proclock,customer0->arrtime,*avipro);
	  }
	  else{
	    *idletot += idleupdate(proclock,proclock[*avipro-1],*avipro);
	  }
	  updateproclock(proclock,customer0->durlist,customer0->subtask);
	  *avipro -= customer0->subtask;

	  deque0();
	  --*countque;
	  updateconttot = 1;
	  qsort(proclock,64,sizeof(int),sort);

	}
	    
      if (front0 != NULL && customer0 == front0->next){
	    
	tempscan0 = front0;
      }
      else if (tempscan0 != NULL && tempscan0 != customer0 && tempscan0!= NULL){
	tempscan0 = tempscan0 -> next;
      }
      scancheck0();
    }
  if(front0 == NULL){
    tempscan1 = NULL;
    customer1 = front1;
    while(front1 != NULL && customer1 != NULL){
      if ( customer1->subtask <= *avipro)
	{
	  if(proclock[customer1->subtask-1]-customer1->arrtime > 0){
	    *waiting1 += proclock[customer1->subtask-1]-customer1->arrtime;
	  }
	  idletime = customer1->arrtime - proclock[*avipro-1];
	  if (idletime > 0){
	    *idletot += idleupdate(proclock, customer1->arrtime,*avipro);
	  }
	  else{
	    *idletot += idleupdate(proclock,proclock[*avipro-1],*avipro);
	  }
	  updateproclock(proclock,customer1->durlist,customer1->subtask);
	  *avipro -= customer1->subtask;

	  deque1();
	  --*countque;
	  updateconttot = 1;
	  qsort(proclock,64,sizeof(int),sort);

	}
      if (front1 != NULL && customer1 == front1->next){
	tempscan1 = front1;
      }
      else if(tempscan1 != NULL && customer1 != tempscan1 &&tempscan1 != NULL){
	tempscan1 = tempscan1 -> next;
      }
      scancheck1();		    	  
    }
  }
  if(updateconttot == 1){
    *counttot+=*countque;}
}

void modeparadurlistgenerator(int durlist[],float serr,int tasknum){
  int ind;
  for (ind = 0; ind < tasknum;ind++){
    durlist[ind] = randgen(serr);
  }
  return;
}

void modepara(float arr0r,float arr1r,float serr,int tot){
  createqueuehigh();
  createqueuelow();
  int tot0 = 0;
  int tot1 = 0;
  int serclock = 0;
  int pl;
  int avipro = 64;
  int subtask;
  int proclock[64] ={0} ;
  int clock0 = 0;
  int clock1 = 0;
  int waiting0 = 0;
  int waiting1 = 0;
  int durlist[32] = {0};
  int countque = 0;
  int counttot = 0;
  int sertot = 0;
  int balfactot = 0;
  int idletot =0;
  float cpu;
  pl=clocking(-1,&clock0,&clock1,arr0r,arr1r,tot0,tot1,tot);
  subtask = unirandgen();
  avipro -= subtask;
  if (pl == 1){
    idletot+=idleupdate(proclock, clock1, 64);
    tot1++;
  }
  if (pl == 0){
    idletot+=idleupdate(proclock, clock0, 64);
    tot0++;
  }
  modeparadurlistgenerator(durlist,serr,subtask);
  balfactot += calbalfac(durlist,subtask);
  updateproclock(proclock,durlist,subtask);
  pl=clocking(-1,&clock0,&clock1,arr0r,arr1r,tot0,tot1,tot);

  while(tot1+tot0 < 2 * tot || front0 != NULL || front1 != NULL)
    { 
      qsort(proclock,64,sizeof(int),sort);
      serclock = proclock[avipro - 1];
      if (avipro == 64 && tot1 + tot0 < 2 * tot){
	if (pl == 1){
	  idletot+=idleupdate(proclock, clock1, 64);
	}
	else
	  {
	    idletot+=idleupdate(proclock, clock0,64);
	  }
	serclock = proclock[0];
      }
      while( (tot1 + tot0 < 2 * tot) && ((serclock >= clock0 && tot0 != tot) || (serclock >= clock1 && tot1 != tot))){
	subtask = unirandgen();
	modeparadurlistgenerator(durlist,serr,subtask);
	balfactot += calbalfac(durlist,subtask);
	if (subtask > avipro){
	  counttot++;
	}
	if (pl==1){
	  enque1(subtask,clock1,durlist);
	  tot1++;
	  countque++;
	}
	else{
	  enque0(subtask,clock0,durlist);
	  tot0++;
	  countque++;
	}
	pl=clocking(pl,&clock0,&clock1,arr0r,arr1r,tot0,tot1,tot);
      }
      scan(&avipro,proclock,&waiting0,&waiting1,&countque,&counttot,&idletot);
      avipro++;
 
    }
  qsort(proclock,64,sizeof(int),sort);
  idletot+=idleupdate(proclock,proclock[63],64);
  sertot = proclock[63];
  cpu = (float)idletot/sertot/64;
  cpu = 1 - cpu;
  FILE *fpout;
  fpout = fopen("proj1_output.txt","w");
  fprintf(fpout,"%f\n%f\n%f\n%f\n%f\n",(float)waiting0/tot,(float)waiting1/tot,(float)counttot/tot/2,cpu,(float)balfactot*serr/(2*tot));
}


char * * explode(const char * str, const char * delims, int * arrLen)
{
  int i = 0;
  int found = 0;
  *arrLen = 1;

  for(i=0;i<strlen(str);i++)
    {
      if(strchr(delims,str[i]) != NULL)
	{
	  
	  found++;
	}
    }
  *arrLen = found + 1;
  char * * strARR = malloc((2*(*arrLen)+1) * sizeof(char *));
  int last = 0;
  int pos = 0;
  if(found == 0)
    {
      strARR[0] = malloc((strlen(str)+1)*sizeof(char));
      memcpy(strARR[pos],str+last,strlen(str));
      strARR[0][strlen(str)] ='\0';
    }
  else
    {
      for(i=0;i<strlen(str);i++)
	{
	  if(strchr(delims,str[i]) != NULL)
	    {
	      strARR[pos] = malloc((2*(i-last)+1)*sizeof(char ));
	      memcpy(strARR[pos],str+last,(i-last+1));
	      strARR[pos][i-last] = '\0';
	      last = i+1;
	      pos++;
	    }
	}
      strARR[pos] = malloc(((strlen(str)-last)*2+1)*sizeof(char ));
      memcpy(strARR[pos],str+last,(i-last+1));
      strARR[pos][i-last] = '\0';
    }  
  return strARR;
}

void modefile(FILE * fp)
{	
  createqueuehigh();
  createqueuelow();
  //Declaration of all variables
  char line[10000];
  int len;
  int serclock = 0;
  int proclock[64] = {0};
  int durlist[32] = {0};
  int ind;
  int avipro = 64;
  int subtask;
  int arrtime;
  int pl;
  int tot = 0;
  int waiting0 = 0;
  int waiting1 = 0;
  int temparrtime;
  int tempsubtask;
  int tempdulist[32] = {0};
  char ** temparr;
  int skip = 0;
  int readline = 0;
  fgets(line,sizeof(line),fp);
  tot++;
  char ** intarr;
  int temppl;
  intarr= explode(line," ",&len);
  char * check;
  int countque = 0;
  int counttot = 0;
  int sertot = 0;
  int subtasktot = 0;
  int durationtot = 0;
  float balfactot = 0.0; 
  int idletot = 0;
  float cpu;
  int breakloop = 0;
  float balfact;
  pl = atoi(&intarr[1][0]);
  subtask = atoi(&intarr[2][0]);
  subtasktot+=subtask;
  arrtime  = atoi(&intarr[0][0]);
  for (ind= 0;ind<subtask;ind++){
    durlist[ind] =atoi(&intarr[ind+3][0]);
    durationtot += durlist[ind];
  }
  balfactot += calbalfac(durlist,subtask);
  for(ind = 0;ind < subtask + 3; ind++){
    free(intarr[ind]);
  }
  free(intarr);
  //Check for special case(Simulation Round 1)
  if (pl == 1 ){
    fgets(line,sizeof(line),fp);
    tot++;
    readline = 1;
    temparr = explode(line," ",&len);
    temparrtime = atoi(&temparr[0][0]);
    tempsubtask = atoi(&temparr[2][0]);
    subtasktot+=tempsubtask;
    for (ind = 0; ind < tempsubtask;ind++){
      tempdulist[ind] = atoi(&temparr[ind+3][0]); 
      durationtot += tempdulist[ind];
    }
    balfactot += calbalfac(tempdulist,tempsubtask);
    for(ind = 0;ind < tempsubtask + 3; ind++){
      free(temparr[ind]);
    }
    free(temparr);
    //arrive at the same time, update 0
    if(temparrtime == arrtime){    
      skip = 1;
      avipro -= tempsubtask;
      updateproclock(proclock,tempdulist,tempsubtask);
      qsort(proclock,64,sizeof(int),sort);
    }   
  }
  //update 1
  avipro -= subtask;
  updateproclock(proclock, durlist,subtask);
  //arrive seperately - update 0 after 1 done
  if (skip  == 0 && readline == 1){
    avipro -= tempsubtask;
    updateproclock(proclock,durlist,tempsubtask);
  }
  //Normal loop simulation 
  check = fgets(line,sizeof(line),fp);
  tot++;
  intarr = explode(line, " ", &len);
  subtask = atoi(&intarr[2][0]);
  subtasktot+=subtask;
  arrtime = atoi(&intarr[0][0]);
  pl = atoi(&intarr[1][0]);
  for (ind= 0;ind<subtask;ind++){
    durlist[ind] = atoi(&intarr[ind+3][0]);
    durationtot += durlist[ind];
  }
  balfactot += calbalfac(durlist,subtask);
  for(ind = 0;ind < subtask + 3; ind++){
    free(intarr[ind]);
  }
  free(intarr);
  serclock = proclock[avipro]; 
  while(check  !=NULL || front0 != NULL || front1 != NULL)
    { 
      qsort(proclock,64,sizeof(int),sort);
      serclock = proclock[avipro-1];

      if (avipro == 64){
	idletot += idleupdate(proclock,arrtime,64);
	serclock = proclock[0];
      }
      while(check != NULL && serclock>=arrtime){
	if (pl == 0){
	  enque0(subtask,arrtime,durlist);
	  breakloop = 1;

	  countque++;
	  if (subtask > avipro){
	    counttot++;
	  }
	  check = fgets(line,sizeof(line),fp);
	  if (check != NULL ){
	    tot++;
	    intarr = explode(line, " ", &len);
	    subtask = atoi(&intarr[2][0]);
	    subtasktot+=subtask;
	    arrtime = atoi(&intarr[0][0]);
	    pl = atoi(&intarr[1][0]);
	    for (ind= 0;ind<subtask;ind++){
	      durlist[ind] = atoi(&intarr[ind+3][0]);
	      durationtot += durlist[ind];
	    }
	    balfactot += calbalfac(durlist,subtask);
	    for(ind = 0;ind < subtask + 3; ind++){
	      free(intarr[ind]);
	    }
	    free(intarr);
	  }   
	}
	if (breakloop == 1){
	  breakloop = 0;
	  break;
	}
	//node = 1 ,read for next line
	if (pl == 1 && breakloop != 1 ){
	  check = fgets(line,sizeof(line),fp);
	  //no need to get new node
	  if(check != NULL){
	    tot++;
	    temparr = explode(line," ",&len);
	    temparrtime = atoi(&temparr[0][0]);
	    tempsubtask = atoi(&temparr[2][0]);
	    subtasktot+=tempsubtask;
	    temppl = atoi(&temparr[1][0]);
	    for (ind = 0; ind < tempsubtask;ind++){
	      tempdulist[ind] = atoi(&temparr[ind+3][0]); 
	      durationtot += tempdulist[ind];
	    }
	    balfactot += calbalfac(tempdulist,tempsubtask);
	    for(ind = 0;ind < tempsubtask + 3; ind++){
	      free(temparr[ind]);
	    }
	    free(temparr);
	    if(temparrtime == arrtime){
	      enque0(tempsubtask,arrtime,tempdulist);

	      enque1(subtask,arrtime,durlist);

	      countque+=2;
	      if (tempsubtask > avipro){
		counttot++;
	      }
	      if (subtask > avipro - tempsubtask)
		counttot++;
	      // need to get new node
	    }
	    else{
	      enque1(subtask,arrtime,durlist);
	      countque++;	
	      if (subtask > avipro){
		counttot++;
	      }
	      arrtime = temparrtime;
	      pl = temppl;
	      subtask = tempsubtask;
	      for(ind = 0; ind < subtask; ind++){
		durlist[ind] = tempdulist[ind];
	      }
	    }
	  }	 
	}	
      }    
      scan(&avipro,proclock,&waiting0,&waiting1,&countque,&counttot,&idletot);
      avipro++;
    }
    
    balfact = (float)durationtot/subtasktot; // average miu(time)
    balfactot /= balfact; // total (max - min) / average miu
    balfactot /= tot;
    
  qsort(proclock,64,sizeof(int),sort);
  idletot+=idleupdate(proclock,proclock[63],64);
  sertot = proclock[63];
  cpu = (float)idletot/sertot/64;
  cpu = 1 - cpu;
  FILE *fpout;
  fpout = fopen("proj1_output.txt","w");
  fprintf(fpout,"%f\n%f\n%f\n%f\n%f\n",(float)waiting0/(tot/2),(float)waiting1/(tot/2),(float)counttot/tot,cpu,(float)balfactot);
}

    
