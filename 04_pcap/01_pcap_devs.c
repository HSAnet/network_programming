#include <stdio.h>
#include <stdlib.h>
#include <pcap/pcap.h>

int main(int argc, char *argv[])
{
	char *dev, errbuf[PCAP_ERRBUF_SIZE];

	//get first non-loopback dev
	dev = pcap_lookupdev(errbuf);

	if (dev == NULL) {
		fprintf(stderr, "Couldn't find default device: %s\n", errbuf);
		return (EXIT_FAILURE);
	}

	printf("First available device: %s\n", dev);
	
	//get all devs
	pcap_if_t *dev_list;

	int ret = pcap_findalldevs(&dev_list, errbuf);
	
	if(ret != 0) {
		fprintf(stderr, "Unable to list all devices: %s\n", errbuf);
		return(EXIT_FAILURE);
	}

	pcap_if_t *tmp = dev_list;

	printf("\nAll devices:\n");

	for(; tmp != NULL; tmp = tmp->next) {
		printf("%s\n", tmp->name);
	}

	pcap_freealldevs(dev_list);

	return (EXIT_SUCCESS);
}
