#ifndef INC_TASKS_H_
#define INC_TASKS_H_

#include "cmsis_os.h"
#include "main.h"

int __svc(0x00) svc_service_add(int x, int y);

void task_file_main(void)
{
    printf(__PRETTY_FUNCTION__);

}


#endif /* INC_TASKS_H_ */
