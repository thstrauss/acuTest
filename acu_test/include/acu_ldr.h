#ifndef __ACU_LOADER__
#define __ACU_LOADER__

void* load_cu(const char* cu_name, long* cmplt_size);
void unload_cu(void* addr);
 
#endif