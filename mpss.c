#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

void processa (int n, int np, int i, int b);
int final ();
int main(int argc, char *argv[])
{
	int np=0, n=0, status, b=0;
	np=atoi(argv[1]);
	n=atoi(argv[2]);
	
	if(argc!=3){
		printf("Deve insirir apenas 2 elementos\n");
		exit(1);}
	if(np<1 || np>32){
		printf("o numero de processos deve ser menor ou igual que 32 e maior ou igual que 1\n");
		exit(1);}
	if(n<np || n>999){
		printf("o numero esqcolhido deve ser inferior a 1000 e supeior a 0\n");
		exit(1);}
	
	int p0 = getppid();
	
	printf("Processo P0 : PID=%d PPID=%d\n", getpid(), getppid());
	
		
	for(int i=1; i<=np; ++i) {
            int pid=fork();
            if(pid==-1){ 
		printf("Erro no fork: processo terminado\n");
		exit(1);}
            if(pid>0){
               wait(&status); 
               break;}else{
               processa(n, np, i, b);
	       b+=(n/np);
		
	}
}
	if(getppid() == p0){
		printf("Resultado calculado: %d\n", final());
		printf("Resultado esperado: %d\n", (n*(n+1)*(2*n+1))/6);

}

    return 0;
}

void processa (int n, int np, int i, int b){
int somap=0, bi, bf, k;
bi=b+1;
bf=b+(n/np);
int c=bf+(n/np);
int h=n-bf;
if(c>=n){
for(k=bi; k<bf+1+h;++k){
somap+=((k*(k+1)*((2*k)+1))/6)-(((k-1)*((k-1)+1)*((2*(k-1))+1))/6);}
}else{
for(k=bi; k<bf+1;++k){
somap+=((k*(k+1)*((2*k)+1))/6)-(((k-1)*((k-1)+1)*((2*(k-1))+1))/6);}
}
FILE *fp;
fp=fopen("./data.aux","a");
fprintf(fp,"%d\n", somap);
fclose(fp);
if(c>=n){printf("Processo P%d : PID=%d PPID=%d i=[%d-%d] soma=%d\n",i, getpid(), getppid(), bi, bf+h, somap);
}else{printf("Processo P%d : PID=%d PPID=%d i=[%d-%d] soma=%d\n",i, getpid(), getppid(), bi, bf, somap);
}

}

int final (){
int soma = 0, somapr;
FILE *fp;
fp=fopen("./data.aux","r");
while(fscanf(fp, "%d\n", &somapr)!=EOF)
soma += somapr;
fclose(fp);

return soma;
}

