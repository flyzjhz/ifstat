#include <stdio.h>
#include <sys/fcntl.h>
#define PROC_NET_DEVICE	"/proc/net/dev"
#define MAX_IF_NAME_SIZE 16


struct IfStat_s {
	char cName[MAX_IF_NAME_SIZE];
	unsigned long long uiRxBytes;
	unsigned long long uiTxBytes;
};

static char *get_name(char *name, char *p);



int main(int argc, char *argv[])
{

    int iRes = 0;
    struct IfStat_s *pstIfStat;

    pstIfStat = malloc(sizeof(struct IfStat_s));
    memset(pstIfStat, 0, sizeof(struct IfStat_s));
    
    
    iRes=fGetStat(argv[1], pstIfStat);
    
    printf("iRes= %d\n", iRes);
    printf("ifstat.name = %s\n", pstIfStat->cName);
    printf("ifstat.rx_bytes = %llu\n", pstIfStat->uiRxBytes);
    printf("ifstat.tx_bytes = %llu\n", pstIfStat->uiTxBytes);
    free(pstIfStat);
    return 0;    
}



/*
* @brief Gets Interface Statistic for given network interface	
* @param pstIfStat		Interface Statistics data structure	
* @param pcInterfaceName	Interface name like eth0, ppp0
* @returns	 	 	 0	success
*				-1 	interface not found
*				-2	interface name size error
*				-3	PROC_NET_DEVICE error
*/
int fGetStat(char *pcInterfaceName, struct IfStat_s *pstIfStat)
{
    FILE *fd;
    char cBuf[512];
    int iResult = -1;
    char cIfName[MAX_IF_NAME_SIZE];
    char *pcLinePtr;
    int iNotImplemented=0;

    if( strlen(pcInterfaceName) > MAX_IF_NAME_SIZE )
    {
	return -2;
    }
    
    memset(pstIfStat, 0, sizeof(struct IfStat_s));
    
    fd = fopen(PROC_NET_DEVICE, "r");

    if (!fd) 
    {
	return -3;
    }

    fgets(cBuf, sizeof(cBuf), fd);
    fgets(cBuf, sizeof(cBuf), fd);
    
    while (fgets(cBuf, sizeof(cBuf), fd)) 
    {
	pcLinePtr = get_name(cIfName, cBuf);
	if(strcmp(cIfName, pcInterfaceName) == 0)
	{
	    int iRes = 0;
	    strcpy(pstIfStat->cName, cIfName);
	    iRes = sscanf(pcLinePtr, "%llu%llu%lu%lu%lu%lu%lu%lu%llu%llu%lu%lu%lu%lu%lu%lu",
			&(pstIfStat->uiRxBytes),
			&iNotImplemented,
			&iNotImplemented,
			&iNotImplemented,
			&iNotImplemented,
			&iNotImplemented,
			&iNotImplemented,
			&iNotImplemented,
			&(pstIfStat->uiTxBytes),
			&iNotImplemented,
			&iNotImplemented,
			&iNotImplemented,
			&iNotImplemented,
			&iNotImplemented,
			&iNotImplemented,
			&iNotImplemented
			);
	    iNotImplemented = 0;
	    fclose(fd);
	    return 0;
	}
	
	else
	    continue;
	
    }

    fclose(fd);
    return iResult;
}


/*
* @brief 	cuts and identifies interface name and returns the other part as a char pointer	
* @param 	p	statistic line from /proc/net/dev	
* @param 	name	name of interface
* @returns 	pointer which points remaining part of line
*/
static char *get_name(char *name, char *p)
{
	/* Extract <name> from nul-terminated p where p matches
	   <name>: after leading whitespace.
	   If match is not made, set name empty and return unchanged p */
	int namestart = 0, nameend = 0;

	while (isspace(p[namestart]))
		namestart++;
	nameend = namestart;
	while (p[nameend] && p[nameend] != ':' && !isspace(p[nameend]))
		nameend++;
	if (p[nameend] == ':') {
		if ((nameend - namestart) < MAX_IF_NAME_SIZE) {
			memcpy(name, &p[namestart], nameend - namestart);
			name[nameend - namestart] = '\0';
			p = &p[nameend];
		} else {
			/* Interface name too large */
			name[0] = '\0';
		}
	} else {
		/* trailing ':' not found - return empty */
		name[0] = '\0';
	}
	return p + 1;
}


