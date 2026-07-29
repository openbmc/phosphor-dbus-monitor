#ifdef SNMP_ENABLED
std::make_unique<SNMPTrap<${c.eventType}>>()
#else
std::unique_ptr<Callback>{}
#endif