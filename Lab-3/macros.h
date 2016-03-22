char *ai_family_string(int ai_family){
        char *ipverstr;
        switch (ai_family){
            case AF_INET:
                ipverstr = "IPv4";
                break;
            case AF_INET6:
                ipverstr = "IPv6";
                break;
            default:
                ipverstr = "unknown";
                break;            
        }
		return ipverstr;
}


char *ai_protocol_string(int ai_protocol){
        char *protostr;
        switch (ai_protocol){
            case IPPROTO_TCP:
                protostr = "TCP";
                break;
            case IPPROTO_UDP:
                protostr = "UDP";
                break;
            default:
                protostr = "unknown";
                break;            
        }
		return protostr;
}


char *ai_socktype_string(int ai_socktype){
        char *socktypestr;
        switch (ai_socktype){
            case SOCK_STREAM:
                socktypestr = "SOCK_STREAM";
                break;
            case SOCK_DGRAM:
                socktypestr = "SOCK_DGRAM";
                break;
            default:
                socktypestr = "unknown";
                break;            
        }
		return socktypestr;
}


