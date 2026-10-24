#include<stdio.h>
#include<string.h>
#include<time.h>

struct Login
{
    char ip[40];
    int time;
    char status[15];
};

#define limit 5
#define window 60
#define max 1000

int main()
{
    struct Login log[max];
    int n=0,f=0;
    char flagged[max][40];
    FILE *fp=fopen("log.txt","r");

    if(fp==NULL)
    {
        printf("Error in opening log.txt file\n");
        return 1;
    }

    while(n<max && fscanf(fp,"%39s %d %14s",log[n].ip,&log[n].time,log[n].status)==3)
        n++;

    fclose(fp);

    if(n==0)
    {
        printf("No valid login records found\n");
        return 0;
    }

    FILE *report=fopen("alert.txt","a");

    if(report==NULL)
        printf("Warning: Could not open alert.txt; alerts will only appear on terminal.\n");

    for(int i=0;i<n;i++)
    {
        if(strcmp(log[i].status,"FAIL")!=0)
            continue;

        int count=0,total=0;

        for(int j=0;j<n;j++)
        {
            if(strcmp(log[i].ip,log[j].ip)==0 && strcmp(log[j].status,"FAIL")==0)
            {
                total++;
                if(log[j].time<=log[i].time && log[j].time>=log[i].time-window)
                    count++;
            }
        }

        if(count<limit)
            continue;

        int found=0;

        for(int k=0;k<f;k++)
        {
            if(strcmp(flagged[k],log[i].ip)==0)
            {
                found=1;
                break;
            }
        }

        if(found)
            continue;

        time_t now=time(NULL);
        struct tm *local=localtime(&now);
        char timestamp[30]="Unknown";

        if(local!=NULL)
            strftime(timestamp,sizeof(timestamp),"%Y-%m-%d %H:%M:%S",local);

        printf("ALERT: Possible brute-force attack\n");
        printf("Detected At: %s\n",timestamp);
        printf("IP ADDRESS: %s\n",log[i].ip);
        printf("Failed Attempts Within %d Seconds: %d\n",window,count);
        printf("Total Failed Attempts: %d\n\n",total);

        if(report!=NULL)
        {
            fprintf(report,"\n=======================================\n");
            fprintf(report,"Detected At: %s\n",timestamp);
            fprintf(report,"IP ADDRESS: %s\n",log[i].ip);
            fprintf(report,"Failed Attempts Within %d Seconds: %d\n",window,count);
            fprintf(report,"Total Failed Attempts: %d\n",total);
            fprintf(report,"Detection Threshold: %d\n",limit);
            fprintf(report,"=======================================\n\n");
            fflush(report);
        }

        strcpy(flagged[f],log[i].ip);
        f++;
    }

    if(report!=NULL)
        fclose(report);

    if(f==0)
        printf("No brute-force detected\n");

    return 0;
}
