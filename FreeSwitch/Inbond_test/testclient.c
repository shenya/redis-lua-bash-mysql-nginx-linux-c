#include <stdio.h>
#include <stdlib.h>
#include <esl.h>


int main(void)
{
	int status = 0;
	esl_handle_t handle = {{0}};

	esl_connect(&handle, "localhost", 8021, NULL, "ClueCon");

	esl_send_recv(&handle, "api status\n\n");

	if (handle.last_sr_event && handle.last_sr_event->body) {
		printf("%s\n", handle.last_sr_event->body);
	} else {
		// this is unlikely to happen with api or bgapi (which is hardcoded above) but prefix but may be true for other commands
		printf("%s\n", handle.last_sr_reply);
	}

        esl_events(&handle, ESL_EVENT_TYPE_PLAIN, "ALL");

        //Make a call
	//bgapi originate
        //esl_send_recv(&handle, "bgapi originate user/1000 &park()\n\n");
	esl_send_recv(&handle, "bgapi originate user/1000 5000\n\n");

        while (1)
	{
	   status = esl_recv_event_timed(&handle, 10, 0, NULL);
	   if (ESL_BREAK == status)
	   {
	       sleep(1);
	   }
	   else if (ESL_SUCCESS == status)
	   {
	       const char *type = esl_event_get_header(handle.last_event, "content-type");
	       printf("type:%s\n", type);
	   }
	   else
	   {
	       printf("exit now");
	       break;
	   }
	
	}


	esl_disconnect(&handle);
	
	return 0;
}
