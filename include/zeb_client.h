/*  =========================================================================
    zeb_client - zeb_broker client implementation for both clients and handlers

    Codec class for zeb_client.

    Copyright (c) the Contributors as noted in the AUTHORS file.       
    This file is part of ZEBRA.                                        
                                                                       
    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at http://mozilla.org/MPL/2.0/.           
    =========================================================================
*/

#ifndef ZEB_CLIENT_H_INCLUDED
#define ZEB_CLIENT_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

//  @warning THE FOLLOWING @INTERFACE BLOCK IS AUTO-GENERATED BY ZPROJECT
//  @warning Please edit the model at "api/zeb_client.api" to make changes.
//  @interface
//  This API is a draft, and may change without notice.
#ifdef ZEBRA_BUILD_DRAFT_API
//  *** Draft method, for development use, may change without warning ***
//  Create a new zeb_client, return the reference if successful,   
//  or NULL if construction failed due to lack of available memory.
ZEBRA_EXPORT zeb_client_t *
    zeb_client_new (void);

//  *** Draft method, for development use, may change without warning ***
//  Destroy the zeb_client and free all memory used by the object.
ZEBRA_EXPORT void
    zeb_client_destroy (zeb_client_t **self_p);

//  *** Draft method, for development use, may change without warning ***
//  Return actor, when caller wants to work with multiple actors and/or
//  input sockets asynchronously.                                      
ZEBRA_EXPORT zactor_t *
    zeb_client_actor (zeb_client_t *self);

//  *** Draft method, for development use, may change without warning ***
//  Return message pipe for asynchronous message I/O. In the high-volume case,
//  we send methods and get replies to the actor, in a synchronous manner, and
//  we send/recv high volume message data to a second pipe, the msgpipe. In   
//  the low-volume case we can do everything over the actor pipe, if traffic  
//  is never ambiguous.                                                       
ZEBRA_EXPORT zsock_t *
    zeb_client_msgpipe (zeb_client_t *self);

//  *** Draft method, for development use, may change without warning ***
//  Return true if client is currently connected, else false. Note that the   
//  client will automatically re-connect if the server dies and restarts after
//  a successful first connection.                                            
ZEBRA_EXPORT bool
    zeb_client_connected (zeb_client_t *self);

//  *** Draft method, for development use, may change without warning ***
//  Connect to server endpoint, with specified timeout in msecs (zero means wait    
//  forever). Constructor succeeds if connection is successful. The caller may      
//  specify its address.                                                            
//  Returns >= 0 if successful, -1 if interrupted.                                  
ZEBRA_EXPORT int
    zeb_client_connect (zeb_client_t *self, const char *endpoint, uint32_t timeout, const char *address);

//  *** Draft method, for development use, may change without warning ***
//  Offer to handle particular XRAP requests, where the route matches request's     
//  resource.                                                                       
//  Returns >= 0 if successful, -1 if interrupted.                                  
ZEBRA_EXPORT int
    zeb_client_set_handler (zeb_client_t *self, const char *method, const char *route);

//  *** Draft method, for development use, may change without warning ***
//  No explanation                                                                  
//  Returns >= 0 if successful, -1 if interrupted.                                  
ZEBRA_EXPORT int
    zeb_client_request (zeb_client_t *self, uint32_t timeout, zmsg_t **content);

//  *** Draft method, for development use, may change without warning ***
//  Send XRAP DELIVER message to server, takes ownership of message
//  and destroys message when done sending it.                     
ZEBRA_EXPORT int
    zeb_client_deliver (zeb_client_t *self, zuuid_t *sender, zmsg_t **content);

//  *** Draft method, for development use, may change without warning ***
//  Receive message from server; caller destroys message when done
ZEBRA_EXPORT zmsg_t *
    zeb_client_recv (zeb_client_t *self);

//  *** Draft method, for development use, may change without warning ***
//  Return last received command. Can be one of these values:
//      "XRAP DELIVER"                                       
ZEBRA_EXPORT const char *
    zeb_client_command (zeb_client_t *self);

//  *** Draft method, for development use, may change without warning ***
//  Return last received status
ZEBRA_EXPORT int
    zeb_client_status (zeb_client_t *self);

//  *** Draft method, for development use, may change without warning ***
//  Return last received reason
ZEBRA_EXPORT const char *
    zeb_client_reason (zeb_client_t *self);

//  *** Draft method, for development use, may change without warning ***
//  Return last received sender
ZEBRA_EXPORT zuuid_t *
    zeb_client_sender (zeb_client_t *self);

//  *** Draft method, for development use, may change without warning ***
//  Return last received content
ZEBRA_EXPORT zmsg_t *
    zeb_client_content (zeb_client_t *self);

//  *** Draft method, for development use, may change without warning ***
//  Enable verbose tracing (animation) of state machine activity.
ZEBRA_EXPORT void
    zeb_client_set_verbose (zeb_client_t *self, bool verbose);

//  *** Draft method, for development use, may change without warning ***
//  Self test of this class.
ZEBRA_EXPORT void
    zeb_client_test (bool verbose);

#endif // ZEBRA_BUILD_DRAFT_API
//  @end

#ifdef __cplusplus
}
#endif

#endif
