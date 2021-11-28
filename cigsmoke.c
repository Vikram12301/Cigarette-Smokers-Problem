#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>
sem_t sem_agent,tobacco_sem,paper_sem,match_sem,tobacco,match,paper,sem_mutex,print;
int istob = 0;
int ispaper = 0;
int ismatch = 0;
int iter=1;
void *agentA(void *a)
{
	while(1)
	{
		sem_wait(&sem_agent);
		sem_post(&tobacco_sem);
		sem_post(&paper_sem);
		sem_wait(&print);
		printf("%d Agent throws tobacco and paper\n",iter++);
		sem_post(&print);
	}
}
void *agentB(void *b)
{
	while(1)
	{
		sem_wait(&sem_agent);
		sem_post(&tobacco_sem);
		sem_post(&match_sem);
		sem_wait(&print);
		printf("%d Agent throws tobacoo and match\n",iter++);
		sem_post(&print);
	}
}
void *agentC(void *c)
{
	while(1)
	{
		sem_wait(&sem_agent);
		sem_post(&paper_sem);
		sem_post(&match_sem);
		sem_wait(&print);
		printf("%d Agent throws paper and  match\n",iter++);
		sem_post(&print);
	}
}
void *pusherA(void *a)
{
	while(1)
	{
		sem_wait(&tobacco_sem);
		sem_wait(&print);
		printf("Tobacco is on the table.\n");
		sem_post(&print);
		sem_wait(&sem_mutex);
		if(ispaper)
		{
			ispaper = 0;
			sem_post(&match);
		}
		else if(ismatch)
		{
			ismatch = 0;
			sem_post(&paper);
		}
		else
		{
			istob = 1;
		}
		sem_post(&sem_mutex);
	}
}
void *pusherB(void *b)
{
	while(1)
	{
		sem_wait(&match_sem);
		sem_wait(&print);
		printf("Matches are on the table.\n");
		sem_post(&print);
		sem_wait(&sem_mutex);
		if(ispaper)
		{
			ispaper = 0;
			sem_post(&tobacco);
		}
		else if(istob)
		{
			istob = 0;
			sem_post(&paper);
		}
		else
		{
			ismatch = 1;
		}
		sem_post(&sem_mutex);
	}
}
void *pusherC(void *c)
{
	while(1)
	{
		sem_wait(&paper_sem);
		sem_wait(&print);
		printf("Paper is on the table.\n");
		sem_post(&print);
		sem_wait(&sem_mutex);
		if(istob)
		{
			istob=0;
			sem_post(&match);
		}
		else if(ismatch)
		{
			ismatch=0;
			sem_post(&tobacco);
		}
		else
		{
			ispaper=1;
		}
		sem_post(&sem_mutex);
	}
}
void *smoker1(void *a)
{
	while(1)
	{
		sem_wait(&tobacco);
		sem_wait(&print);
		printf("Smoker2 gets tobacco, makes a cigarette\n");
		sem_post(&print);
		sem_post(&sem_agent);
		sem_wait(&print);
		printf("Smoker2 smokes the cigarette\n\n");
		sem_post(&print);
		sleep(4);
	}
}
void *smoker2(void *b)
{
	while(1)
	{
		sem_wait(&paper);
		sem_wait(&print);
		printf("Smoker3 gets paper, makes a cigarette\n");
		sem_post(&print);
		sem_post(&sem_agent);
		sem_wait(&print);
		printf("Smoker3 smokes a cigarette\n\n");
		sem_post(&print);
		sleep(4);
	}
}
void *smoker3(void *c)
{
	while(1)
	{
		sem_wait(&match);
		sem_wait(&print);
		printf("Smoker1 gets matches, makes a cigarette\n");
		sem_post(&print);
		sem_post(&sem_agent);
		sem_wait(&print);
		printf("Smoker1 smokes the cigarette\n\n");
		sem_post(&print);
		sleep(4);
	}
}

int main()
{
	pthread_t a1, a2, a3, p1, p2, p3, s1, s2, s3;
        sem_init(&sem_agent, 0, 1);
        sem_init(&tobacco_sem, 0, 0);
        sem_init(&paper_sem, 0, 0);
        sem_init(&match_sem, 0, 0);
    	sem_init(&tobacco, 0, 0);
        sem_init(&paper, 0, 0);
        sem_init(&match, 0, 0);
        sem_init(&sem_mutex, 0, 1);
        sem_init(&print, 0,1);
        pthread_create(&a1,NULL,agentA,NULL);
        pthread_create(&a2,NULL,agentB,NULL);
        pthread_create(&a3,NULL,agentC,NULL);
        pthread_create(&s1,NULL,smoker1,NULL);
        pthread_create(&s2,NULL,smoker2,NULL);
        pthread_create(&s3,NULL,smoker3,NULL);
        pthread_create(&p1,NULL,pusherA,NULL);
        pthread_create(&p2,NULL,pusherB,NULL);
        pthread_create(&p3,NULL,pusherC,NULL);
        pthread_join(a1,NULL);
        pthread_join(a2,NULL);
	pthread_join(a3,NULL);
	pthread_join(s1,NULL);
	pthread_join(s2,NULL);
	pthread_join(s3,NULL);
	pthread_join(p1,NULL);
	pthread_join(p2,NULL);
	pthread_join(p3,NULL);
}
