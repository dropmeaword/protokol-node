extern byte GUID[8];

unsigned long longRandom();

void guid_store();
void guid_clear();
void guid_set(unsigned long muid);
unsigned long guid_init();
unsigned long guid_randomize();

