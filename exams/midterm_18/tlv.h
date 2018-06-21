#ifndef TLV_HEADER_INCL
#define TLV_HEADER_INCL

struct tlv_header {
	uint8_t type;
	uint16_t length;
}__attribute__((packed));

#define IP_ECHO_REQUEST 1
#define IP_ECHO_RESPONSE 2

#endif
