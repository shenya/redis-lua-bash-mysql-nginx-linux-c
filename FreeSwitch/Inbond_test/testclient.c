#include <stdio.h>
#include <stdlib.h>
#include <esl.h>


int test()
{
    printf("hello\n");
    return 0;
}

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
        //esl_events(&handle, ESL_EVENT_TYPE_PLAIN, "CHANNEL_ORIGINATE CHANNEL_CREATE CHANNEL_DESTROY CHANNEL_STATE CHANNEL_CALLSTATE CHANNEL_ANSWER CHANNEL_HANGUP CHANNEL_EXECUTE");
        //esl_events(&handle, ESL_EVENT_TYPE_PLAIN, "CHANNEL_CALLSTATE CHANNEL_STATE");
        //esl_events(&handle, ESL_EVENT_TYPE_PLAIN, "CHANNEL_STATE CHANNEL_PROGRESS CHANNEL_BRIDGE  CHANNEL_PROGRESS_MEDIA");

        //Make a call
	//bgapi originate
        //esl_send_recv(&handle, "bgapi originate user/1000 &park()\n\n");
	esl_send_recv(&handle, "bgapi originate {fax_ident=1231231234}user/1000 5000\n\n");

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
	       const char *event_name = esl_event_get_header(handle.last_event, "Event-Name");
	       printf("type:%s, event name:%s\n", type, event_name);
	       if (handle.last_event && handle.last_event->body)
	       {
	           printf("body:%s\n", handle.last_event->body);
		   const char *uuid = esl_event_get_header(handle.last_ievent, "Unique-ID");
	           const char *event_name = esl_event_get_header(handle.last_ievent, "Event-Name");
	           const char *session_id = esl_event_get_header(handle.last_ievent, "call_uuid");
		   printf("uuid:%s, event name:%s, ID:%d\n", uuid, event_name, handle.last_ievent->event_id);
		   printf("session id:%s\n", session_id);
	       }

	   }
	   else
	   {
	       printf("exit now\n");
	       break;
	   }
	
	}


	esl_disconnect(&handle);
	
	return 0;
}
