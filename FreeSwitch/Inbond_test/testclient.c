#include <stdio.h>
#include <stdlib.h>
#include <esl.h>


int test()
{
    printf("hello\n");
    return 0;
}

int send_msg(esl_handle_t *handle, char *to, int external_flag)
{
        int status = ESL_SUCCESS;
        esl_event_t *event = NULL;

	status = esl_event_create_subclass(&event, ESL_EVENT_CUSTOM, "SMS::SEND_MESSAGE");
	if (ESL_SUCCESS != status)
	{
	    printf("create event failed\n");
	    return -1;
	}
	esl_event_add_header(event, ESL_STACK_BOTTOM, "from", "0000@172.17.244.156");
	//esl_event_add_header(event, ESL_STACK_BOTTOM, "to", "2068@39.105.106.62:9004;transport=tcp");
	printf("to: %s\n", to);
	esl_event_add_header(event, ESL_STACK_BOTTOM, "to", to);
	esl_event_add_header(event, ESL_STACK_BOTTOM, "dest_proto", "sip");
	esl_event_add_header(event, ESL_STACK_BOTTOM, "blocking", "true");
	esl_event_add_header(event, ESL_STACK_BOTTOM, "proto", "global");
	if (external_flag)
	{
	    esl_event_add_header(event, ESL_STACK_BOTTOM, "sip_profile", "external");
	}
#if 0
	esl_event_add_header(event, ESL_STACK_BOTTOM, "from", "0000@172.17.244.156");
	esl_event_add_header(event, ESL_STACK_BOTTOM, "to", "1001@172.17.244.156");
	esl_event_add_header(event, ESL_STACK_BOTTOM, "dest_proto", "sip");
	esl_event_add_header(event, ESL_STACK_BOTTOM, "blocking", "true");
	esl_event_add_header(event, ESL_STACK_BOTTOM, "proto", "global");
#endif
	esl_event_add_body(event, "hello world");
        status = esl_sendevent(handle, event);
	if (ESL_SUCCESS != status)
	{
	    printf("create event failed\n");
	}


	return 0;
}

int call_originate()
{

        //Make a call
	//bgapi originate
        //esl_send_recv(&handle, "bgapi originate user/1000 &park()\n\n");
	//esl_send_recv(&handle, "bgapi originate {fax_ident=1231231234,sip_h_X-Product-ID=12345}user/1000 &echo()\n\n");
	//esl_send_recv(&handle, "bgapi originate {fax_ident=1231231234,sip_h_X-Product-ID=12345}user/1001 &playback(ivr/ivr-that_was_an_invalid_entry.wav)\n\n");
	//esl_send_recv(&handle, "bgapi originate {fax_ident=1231231234,sip_h_X-Product-ID=12345}user/1001 5000\n\n");
	//esl_send_recv(&handle, "bgapi originate {fax_ident=1231231234,sip_h_X-Product-ID=12345}user/1002 &bridge(user/1000)\n\n");
	
	//esl_send_recv(&handle, "bgapi originate {sip_invite_req_uri=sip:2068_00014748@124.202.182.82:58806;transport=TCP;ob,sip_invite_route_uri=sip:39.105.106.62:9003;transport=TCP;lr}sofia/external/2068_00014748@124.202.182.82:58806 5000\n\n");
	//esl_send_recv(&handle, "bgapi originate {sip_invite_route_uri=<sip:39.105.106.62:9004;transport=TCP;lr>,sip_invite_from_user=hello,sip_invite_req_uri=sip:2068_00014748@124.202.182.82:60361;transport=TCP;ob,sip_invite_full_from=<sip:callcenter_auto_answer@47.93.228.87>,sip_h_E-cc-number=cc_pc_101_1077_00014748_1575512880_1881465682,sip_h_E-from-cp=cp,}sofia/internal/2068_00014748@124.202.182.82;transport=TCP 5000\n\n");

	//esl_event_add_header(event, ESL_STACK_BOTTOM, "to", "2068_00014748@124.207.164.117:11786;transport=udp");
	//esl_send_recv(&handle, "bgapi chat|1008|2068_00014748@124.202.182.82:60361;transport=TCP|hellow world\n\n");
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

        esl_events(&handle, ESL_EVENT_TYPE_JSON, "CUSTOM sofia::register sofia::unregister sofia::expire");
        //esl_events(&handle, ESL_EVENT_TYPE_JSON, "ALL");
        //esl_events(&handle, ESL_EVENT_TYPE_PLAIN, "CHANNEL_ORIGINATE CHANNEL_CREATE CHANNEL_DESTROY CHANNEL_STATE CHANNEL_CALLSTATE CHANNEL_ANSWER CHANNEL_HANGUP CHANNEL_EXECUTE");
        //esl_events(&handle, ESL_EVENT_TYPE_PLAIN, "CHANNEL_CALLSTATE CHANNEL_STATE");
        //esl_events(&handle, ESL_EVENT_TYPE_PLAIN, "CHANNEL_STATE CHANNEL_PROGRESS CHANNEL_BRIDGE  CHANNEL_PROGRESS_MEDIA");


	//send_msg(&handle);
	printf("***********\n");

	
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
		   const char *uuid = esl_event_get_header(handle.last_ievent, "Unique-ID");
	           const char *event_name = esl_event_get_header(handle.last_ievent, "Event-Name");
	           const char *session_id = esl_event_get_header(handle.last_ievent, "call_uuid");
		   printf("----------------------------------------------------\n");
		   printf("uuid:%s, event name:%s, ID:%d\n", uuid, event_name, handle.last_ievent->event_id);
		   printf("session id:%s\n", session_id);
		   printf("body start*****************\n");
	           printf("body:%s\n", handle.last_event->body);

		   if (ESL_EVENT_CUSTOM == handle.last_ievent->event_id)
		   {
		       printf("#####################\n");
		       cJSON *root_obj = NULL;
		       cJSON *tmp_obj = NULL;
		       root_obj = cJSON_Parse(handle.last_event->body);
                       tmp_obj = cJSON_GetObjectItem(root_obj, "Event-Subclass");
		       if (tmp_obj)
		       {
		           printf("subcalss: %s\n", tmp_obj->valuestring);
			   if (0 == strncmp(tmp_obj->valuestring, "sofia::register", strlen(tmp_obj->valuestring)))
			   {
			       cJSON *contact = NULL;
			       contact = cJSON_GetObjectItem(root_obj, "contact");
			       if (contact)
			       {
			           printf("contact: %s\n", contact->valuestring);
				   send_msg(&handle, "1002@172.17.244.156", 0);
			       }
			   }
		       }
                       
		   }
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
