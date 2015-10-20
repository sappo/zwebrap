
<A name="toc1-3" title="zwebrap - REST/HTTP to XRAP gateway." />
# zwebrap - REST/HTTP to XRAP gateway.

<A name="toc2-6" title="Contents" />
## Contents


**<a href="#toc2-11">Overview</a>**

**<a href="#toc3-16">Scope and Goals</a>**

**<a href="#toc3-52">Requirements</a>**

**<a href="#toc3-59">Installation</a>**

**<a href="#toc3-69">Usage</a>**
&emsp;<a href="#toc4-74">zwr_microhttpd - HTTP web server backend using libmicrohttpd.</a>
&emsp;<a href="#toc4-242">zwr_server - no title found</a>
&emsp;<a href="#toc4-415">zwr_client - ZWebRap Client</a>

**<a href="#toc2-592">Ownership and License</a>**

**<a href="#toc3-601">Hints to Contributors</a>**

**<a href="#toc3-608">This Document</a>**

<A name="toc2-11" title="Overview" />
## Overview

zwebrap is a REST/HTTP to XRAP gateway.

<A name="toc3-16" title="Scope and Goals" />
### Scope and Goals

zwebrap is designed to take HTTP request for the common HTTP methods GET, POST, PUT and DELETE. It will convert these HTTP request into the XRAP format and pass it on to the request handlers which compose a response in the XRAP format which will be converted back into HTTP.

<center>
<img src="https://github.com/zeromq/zwebrap/raw/master/images/README_1.png" alt="1">
</center>

<A name="toc3-52" title="Requirements" />
### Requirements

* libmicrohttpd (>= 0.9.40)
* libzmq (>= 4.1)
* czmq (>= 3.0.3)

<A name="toc3-59" title="Installation" />
### Installation

```sh
autogen.sh
configure
make && make check
make install
```

<A name="toc3-69" title="Usage" />
### Usage

This is the API provided by zwebrap v0.x, in alphabetical order.

<A name="toc4-74" title="zwr_microhttpd - HTTP web server backend using libmicrohttpd." />
#### zwr_microhttpd - HTTP web server backend using libmicrohttpd.

zwr_microhttpdd - HTTP web server backend using libmicrohttpd.

Please add @discuss section in ../src/zwr_microhttpd.c.

This is the class interface:

    //  Create new zwr_microhttpd actor instance.
    //  @TODO: Describe the purpose of this actor!
    //
    //      zactor_t *microhttpd = zactor_new (zwr_microhttpd, NULL);
    //
    //  Destroy zwr_microhttpd instance.
    //
    //      zactor_destroy (&microhttpd);
    //
    //  Enable verbose logging of commands and activity:
    //
    //      zstr_send (microhttpd, "VERBOSE");
    //      zsock_wait (microhttpd);
    //
    //  Start zwr_microhttpd actor.
    //
    //      zstr_sendx (microhttpd, "START", NULL);
    //      zsock_wait (microhttpd);
    //
    //  Stop zwr_microhttpd actor.
    //
    //      zstr_sendx (microhttpd, "STOP", NULL);
    //      zsock_wait (microhttpd);
    //
    //  Attach the endpoint socket to endpoints, see zsock_attach ().
    //
    //      zstr_sendx (microhttpd, "ENDPOINT", endpoints, NULL);
    //      zsock_wait (microhttpd);
    //
    //  Set the port on which the http server shall listen for requests.
    //  The default port is 8888.
    //  Note: The port is passed as string!
    //  Note: If the server is already started, the server will be stopped
    //        and restarted on the new port.
    //
    //      zstr_sendx (microhttpd, "PORT", "8888", NULL);
    //      zsock_wait (microhttpd);
    //
    //  This is the zwr_microhttpd constructor as a zactor_fn;
    ZWEBRAP_EXPORT void
        zwr_microhttpd_actor (zsock_t *pipe, void *args);
    
    //  Self test of this actor
    ZWEBRAP_EXPORT void
        zwr_microhttpd_test (bool verbose);

This is the class self test code:

    //  Simple create/destroy test
    zactor_t *zwr_microhttpd = zactor_new (zwr_microhttpd_actor, NULL);
    
    zstr_send (zwr_microhttpd, "START");
    rc = zsock_wait (zwr_microhttpd);             //  Wait until actor started
    assert (rc == 0);
    
    zstr_sendx (zwr_microhttpd, "PORT", "8081", NULL);
    rc = zsock_wait (zwr_microhttpd);             //  Wait until port is configured
    assert (rc == 0);
    
    zstr_sendx (zwr_microhttpd, "ENDPOINT", "inproc://http_dispatcher", NULL);
    rc = zsock_wait (zwr_microhttpd);             //  Wait until endpoint configured
    assert (rc == 0);
    
    zactor_t *dispatcher = zactor_new (zwr_server, "dispatcher");
    
    if (verbose)
        zstr_send (dispatcher, "VERBOSE");
    zstr_sendx (dispatcher, "BIND", "inproc://http_dispatcher", NULL);
    
    //  Create handler
    zwr_client_t *handler = zwr_client_new ();
    assert (handler);
    
    //  Connect handler to server
    rc = zwr_client_connect (handler, "inproc://http_dispatcher",  1000, "handler");
    assert (rc == 0);
    assert (zwr_client_connected (handler) == true);
    
    //  Provide GET Offering
    rc = zwr_client_set_handler (handler, "GET", "/foo/{[^/]}");
    assert (rc == 0);
    
    //  Send GET Request
    zwr_curl_client_t *curl = zwr_curl_client_new ();
    zwr_curl_client_send_get (curl, "http://localhost:8081/foo/bar?page=10");
    
    //  Receive Request
    zmsg_t *request = zwr_client_recv (handler);
    assert (request);
    xrap_msg_t *xrap_msg = xrap_msg_decode (&request);
    assert (xrap_msg_id (xrap_msg) == XRAP_MSG_GET);
    assert (streq ("/foo/bar", xrap_msg_resource (xrap_msg)));
    xrap_msg_destroy (&xrap_msg);
    
    //  Send Response
    xrap_msg = xrap_msg_new (XRAP_MSG_GET_OK);
    xrap_msg_set_status_code (xrap_msg, 200);
    xrap_msg_set_content_type (xrap_msg, "text/hello");
    xrap_msg_set_content_body (xrap_msg, "Hello World!");
    zmsg_t *response = xrap_msg_encode (&xrap_msg);
    zwr_client_deliver (handler, zwr_client_sender (handler), &response);
    zuuid_t *sender = zwr_client_sender (handler);
    zuuid_destroy (&sender);
    
    //  Receive GET Response
    zwr_curl_client_verify_response (curl, 200, "Hello World!");
    zwr_curl_client_destroy (&curl);
    
    //  Send GET Request 2
    curl = zwr_curl_client_new ();
    zwr_curl_client_send_get (curl, "http://localhost:8081/foo/bar/baz");
    
    //  Receive GET Response 2
    zwr_curl_client_verify_response (curl, 404, PAGE_NOT_FOUND);
    zwr_curl_client_destroy (&curl);
    
    //  Provide POST Offering
    rc = zwr_client_set_handler (handler, "POST", "/foo/{[^/]}");
    assert (rc == 0);
    
    curl = zwr_curl_client_new ();
    zwr_curl_client_send_post (curl, "http://localhost:8081/foo/bar", "abc");
    
    //  Receive Request
    request = zwr_client_recv (handler);
    assert (request);
    xrap_msg = xrap_msg_decode (&request);
    assert (xrap_msg_id (xrap_msg) == XRAP_MSG_POST);
    assert (streq ("/foo/bar", xrap_msg_parent (xrap_msg)));
    xrap_msg_destroy (&xrap_msg);
    
    //  Send Response
    xrap_msg = xrap_msg_new (XRAP_MSG_POST_OK);
    xrap_msg_set_status_code (xrap_msg, 201);
    xrap_msg_set_location (xrap_msg, "/foo/bar");
    xrap_msg_set_etag (xrap_msg, "a3fsd3");
    xrap_msg_set_date_modified (xrap_msg, 0);
    xrap_msg_set_content_type (xrap_msg, "text/hello");
    xrap_msg_set_content_body (xrap_msg, "Hello World!");
    response = xrap_msg_encode (&xrap_msg);
    zwr_client_deliver (handler, zwr_client_sender (handler), &response);
    sender = zwr_client_sender (handler);
    zuuid_destroy (&sender);
    
    //  Give response time to arrive
    usleep (250);
    
    zwr_curl_client_verify_response (curl, 201, "Hello World!");
    zwr_curl_client_destroy (&curl);
    
    zwr_client_destroy (&handler);
    zactor_destroy (&dispatcher);
    
    zstr_send (zwr_microhttpd, "STOP");
    rc = zsock_wait (zwr_microhttpd);             //  Wait until actor stopped
    assert (rc == 0);
    
    zactor_destroy (&zwr_microhttpd);

<A name="toc4-242" title="zwr_server - no title found" />
#### zwr_server - no title found

zwr_server -

Please add @discuss section in ../src/zwr_server.c.

This is the class interface:

    //  To work with zwr_server, use the CZMQ zactor API:
    //
    //  Create new zwr_server instance, passing logging prefix:
    //
    //      zactor_t *zwr_server = zactor_new (zwr_server, "myname");
    //
    //  Destroy zwr_server instance
    //
    //      zactor_destroy (&zwr_server);
    //
    //  Enable verbose logging of commands and activity:
    //
    //      zstr_send (zwr_server, "VERBOSE");
    //
    //  Bind zwr_server to specified endpoint. TCP endpoints may specify
    //  the port number as "*" to aquire an ephemeral port:
    //
    //      zstr_sendx (zwr_server, "BIND", endpoint, NULL);
    //
    //  Return assigned port number, specifically when BIND was done using an
    //  an ephemeral port:
    //
    //      zstr_sendx (zwr_server, "PORT", NULL);
    //      char *command, *port_str;
    //      zstr_recvx (zwr_server, &command, &port_str, NULL);
    //      assert (streq (command, "PORT"));
    //
    //  Specify configuration file to load, overwriting any previous loaded
    //  configuration file or options:
    //
    //      zstr_sendx (zwr_server, "LOAD", filename, NULL);
    //
    //  Set configuration path value:
    //
    //      zstr_sendx (zwr_server, "SET", path, value, NULL);
    //
    //  Save configuration data to config file on disk:
    //
    //      zstr_sendx (zwr_server, "SAVE", filename, NULL);
    //
    //  Send zmsg_t instance to zwr_server:
    //
    //      zactor_send (zwr_server, &msg);
    //
    //  Receive zmsg_t instance from zwr_server:
    //
    //      zmsg_t *msg = zactor_recv (zwr_server);
    //
    //  This is the zwr_server constructor as a zactor_fn:
    //
    ZWEBRAP_EXPORT void
        zwr_server (zsock_t *pipe, void *args);
    
    //  Self test of this class
    ZWEBRAP_EXPORT void
        zwr_server_test (bool verbose);

This is the class self test code:

    zactor_t *server = zactor_new (zwr_server, "dispatcher");
    if (verbose)
        zstr_send (server, "VERBOSE");
    zstr_sendx (server, "BIND", "tcp://127.0.0.1:9999", NULL);
    
    zsock_t* client = zsock_new_dealer (">tcp://127.0.0.1:9999");
    zsock_t* worker = zsock_new_dealer (">tcp://127.0.0.1:9999");
    assert (client);
    assert (worker);
    
    xrap_traffic_t *traffic = xrap_traffic_new ();
    
    //  Invalid Command
    xrap_traffic_set_id (traffic, XRAP_TRAFFIC_XRAP_OFFER);
    xrap_traffic_send (traffic, client);
    xrap_traffic_recv (traffic, client);
    assert (xrap_traffic_id (traffic) == XRAP_TRAFFIC_ERROR);
    assert (xrap_traffic_status_code (traffic) == XRAP_TRAFFIC_COMMAND_INVALID);
    
    //  Open Connections for client & worker
    xrap_traffic_set_id (traffic, XRAP_TRAFFIC_CONNECTION_OPEN);
    xrap_traffic_send (traffic, client);
    xrap_traffic_recv (traffic, client);
    assert (xrap_traffic_id (traffic) == XRAP_TRAFFIC_OK);
    
    xrap_traffic_set_id (traffic, XRAP_TRAFFIC_CONNECTION_OPEN);
    xrap_traffic_send (traffic, worker);
    xrap_traffic_recv (traffic, worker);
    assert (xrap_traffic_id (traffic) == XRAP_TRAFFIC_OK);
    
    //  Provide Rubish Offering
    xrap_traffic_set_id (traffic, XRAP_TRAFFIC_XRAP_OFFER);
    xrap_traffic_set_route (traffic, "///");
    xrap_traffic_set_method (traffic, "GET");
    xrap_traffic_send (traffic, worker);
    xrap_traffic_recv (traffic, worker);
    assert (xrap_traffic_id (traffic) == XRAP_TRAFFIC_FAIL);
    assert (xrap_traffic_status_code (traffic) == XRAP_TRAFFIC_CONFLICT);
    
    //  Provide Offering
    xrap_traffic_set_id (traffic, XRAP_TRAFFIC_XRAP_OFFER);
    xrap_traffic_set_route (traffic, "/foo/{[^/]}");
    xrap_traffic_set_method (traffic, "GET");
    xrap_traffic_send (traffic, worker);
    xrap_traffic_recv (traffic, worker);
    assert (xrap_traffic_id (traffic) == XRAP_TRAFFIC_OK);
    
    //  Send Request
    xrap_msg_t *xmsg = xrap_msg_new (XRAP_MSG_GET);
    xrap_msg_set_resource (xmsg, "%s", "/foo/bar");
    zmsg_t *msg = xrap_msg_encode (&xmsg);
    xrap_traffic_set_id (traffic, XRAP_TRAFFIC_XRAP_SEND);
    xrap_traffic_set_content (traffic, &msg);
    xrap_traffic_send (traffic, client);
    xrap_traffic_recv (traffic, client);
    assert (xrap_traffic_id (traffic) == XRAP_TRAFFIC_OK);
    
    //  Receive Request
    xrap_traffic_recv (traffic, worker);
    assert (xrap_traffic_id (traffic) == XRAP_TRAFFIC_XRAP_DELIVER);
    msg = zmsg_dup (xrap_traffic_content (traffic));
    xmsg = xrap_msg_decode (&msg);
    assert (xrap_msg_id (xmsg) == XRAP_MSG_GET);
    assert (streq ("/foo/bar", xrap_msg_resource (xmsg)));
    xrap_msg_destroy (&xmsg);
    
    //  Send Response
    xmsg = xrap_msg_new (XRAP_MSG_GET_OK);
    xrap_msg_set_status_code (xmsg, 200);
    xrap_msg_set_content_type (xmsg, "text/hello");
    xrap_msg_set_content_body (xmsg, "Hello World!");
    msg = xrap_msg_encode (&xmsg);
    xrap_traffic_set_id (traffic, XRAP_TRAFFIC_XRAP_DELIVER);
    xrap_traffic_set_content (traffic, &msg);
    xrap_traffic_send (traffic, worker);
    
    //  Receive Response
    xrap_traffic_recv (traffic, client);
    assert (xrap_traffic_id (traffic) == XRAP_TRAFFIC_XRAP_DELIVER);
    msg = zmsg_dup (xrap_traffic_content (traffic));
    xmsg = xrap_msg_decode (&msg);
    assert (xrap_msg_id (xmsg) == XRAP_MSG_GET_OK);
    assert (xrap_msg_status_code (xmsg) == 200);
    assert (streq ("text/hello", xrap_msg_content_type (xmsg)));
    assert (streq ("Hello World!", xrap_msg_content_body (xmsg)));
    xrap_msg_destroy (&xmsg);
    
    //  Send Request 2
    xmsg = xrap_msg_new (XRAP_MSG_GET);
    xrap_msg_set_resource (xmsg, "%s", "/fou/baz");
    msg = xrap_msg_encode (&xmsg);
    xrap_traffic_set_id (traffic, XRAP_TRAFFIC_XRAP_SEND);
    xrap_traffic_set_content (traffic, &msg);
    xrap_traffic_send (traffic, client);
    xrap_traffic_recv (traffic, client);
    assert (xrap_traffic_id (traffic) == XRAP_TRAFFIC_FAIL);
    assert (xrap_traffic_status_code (traffic) == XRAP_TRAFFIC_NOT_FOUND);
    
    xrap_traffic_destroy (&traffic);
    
    //  Finished, we can clean up
    zsock_destroy (&client);
    zsock_destroy (&worker);
    zactor_destroy (&server);

<A name="toc4-415" title="zwr_client - ZWebRap Client" />
#### zwr_client - ZWebRap Client

Description of class for man page.

Detailed discussion of the class, if any.

This is the class interface:

    //  Create a new zwr_client, return the reference if successful, or NULL
    //  if construction failed due to lack of available memory.
    ZWEBRAP_EXPORT zwr_client_t *
        zwr_client_new (void);
    
    //  Destroy the zwr_client and free all memory used by the object.
    ZWEBRAP_EXPORT void
        zwr_client_destroy (zwr_client_t **self_p);
    
    //  Return actor, when caller wants to work with multiple actors and/or
    //  input sockets asynchronously.
    ZWEBRAP_EXPORT zactor_t *
        zwr_client_actor (zwr_client_t *self);
    
    //  Return message pipe for asynchronous message I/O. In the high-volume case,
    //  we send methods and get replies to the actor, in a synchronous manner, and
    //  we send/recv high volume message data to a second pipe, the msgpipe. In
    //  the low-volume case we can do everything over the actor pipe, if traffic
    //  is never ambiguous.
    ZWEBRAP_EXPORT zsock_t *
        zwr_client_msgpipe (zwr_client_t *self);
    
    //  Return true if client is currently connected, else false. Note that the
    //  client will automatically re-connect if the server dies and restarts after
    //  a successful first connection.
    ZWEBRAP_EXPORT bool
        zwr_client_connected (zwr_client_t *self);
    
    //  Connect to server endpoint, with specified timeout in msecs (zero means wait    
    //  forever). Constructor succeeds if connection is successful. The caller may      
    //  specify its address.                                                            
    //  Returns >= 0 if successful, -1 if interrupted.
    ZWEBRAP_EXPORT int 
        zwr_client_connect (zwr_client_t *self, const char *endpoint, uint32_t timeout, const char *address);
    
    //  Offer to handle particular XRAP requests, where the route matches request's     
    //  resource.                                                                       
    //  Returns >= 0 if successful, -1 if interrupted.
    ZWEBRAP_EXPORT int 
        zwr_client_set_handler (zwr_client_t *self, const char *method, const char *route);
    
    //  No explanation                                                                  
    //  Returns >= 0 if successful, -1 if interrupted.
    ZWEBRAP_EXPORT int 
        zwr_client_request (zwr_client_t *self, uint32_t timeout, zmsg_t **content_p);
    
    //  Send XRAP DELIVER message to server, takes ownership of message
    //  and destroys message when done sending it.
    ZWEBRAP_EXPORT int
        zwr_client_deliver (zwr_client_t *self, zuuid_t *sender, zmsg_t **content_p);
    
    //  Receive message from server; caller destroys message when done
    ZWEBRAP_EXPORT zmsg_t *
        zwr_client_recv (zwr_client_t *self);
    
    //  Return last received command. Can be one of these values:
    //      "XRAP DELIVER"
    ZWEBRAP_EXPORT const char *
        zwr_client_command (zwr_client_t *self);
    
    //  Return last received status
    ZWEBRAP_EXPORT int 
        zwr_client_status (zwr_client_t *self);
    
    //  Return last received reason
    ZWEBRAP_EXPORT const char *
        zwr_client_reason (zwr_client_t *self);
    
    //  Return last received sender
    ZWEBRAP_EXPORT zuuid_t *
        zwr_client_sender (zwr_client_t *self);
    
    //  Return last received content
    ZWEBRAP_EXPORT zmsg_t *
        zwr_client_content (zwr_client_t *self);
    
    //  Self test of this class
    ZWEBRAP_EXPORT void
        zwr_client_test (bool verbose);
    
    //  To enable verbose tracing (animation) of zwr_client instances, set
    //  this to true. This lets you trace from and including construction.
    ZWEBRAP_EXPORT extern volatile int
        zwr_client_verbose;

This is the class self test code:

    zwr_client_verbose = verbose;
    
    //  Start a server to test against, and bind to endpoint
    zactor_t *server = zactor_new (zwr_server, "zwr_client_test");
    if (verbose)
        zstr_send (server, "VERBOSE");
    zstr_sendx (server, "LOAD", "src/zwr_client.cfg", NULL);
    
    //  Create clients for testing
    zwr_client_t *client = zwr_client_new ();
    zwr_client_t *handler = zwr_client_new ();
    assert (client);
    assert (handler);
    
    //  Connect clients to server
    int rc = zwr_client_connect (client, "tcp://127.0.0.1:9999", 1000, "client");
    assert (rc == 0);
    assert (zwr_client_connected (client) == true);
    
    rc = zwr_client_connect (handler, "tcp://127.0.0.1:9999", 1000, "handler");
    assert (rc == 0);
    assert (zwr_client_connected (handler) == true);
    
    //  Provide Rubbish Offering
    rc = zwr_client_set_handler (handler, "GET", "///");
    assert (rc != 0);
    assert (zwr_client_status (handler) == XRAP_TRAFFIC_CONFLICT);
    
    //  Provide Offering
    rc = zwr_client_set_handler (handler, "GET", "/foo/{[^/]}");
    assert (rc == 0);
    
    //  Send Request
    xrap_msg_t *xmsg = xrap_msg_new (XRAP_MSG_GET);
    xrap_msg_set_resource (xmsg, "%s", "/foo/bar");
    zmsg_t *msg = xrap_msg_encode (&xmsg);
    rc = zwr_client_request (client, 0, &msg);
    assert (rc == 0);
    
    //  Receive Request
    msg = zwr_client_recv (handler);
    xmsg = xrap_msg_decode (&msg);
    assert (xrap_msg_id (xmsg) == XRAP_MSG_GET);
    assert (streq ("/foo/bar", xrap_msg_resource (xmsg)));
    xrap_msg_destroy (&xmsg);
    
    //  Send Response
    xmsg = xrap_msg_new (XRAP_MSG_GET_OK);
    xrap_msg_set_status_code (xmsg, 200);
    xrap_msg_set_content_type (xmsg, "text/hello");
    xrap_msg_set_content_body (xmsg, "Hello World!");
    msg = xrap_msg_encode (&xmsg);
    zwr_client_deliver (handler, handler->sender, &msg);
    zuuid_t *sender = zwr_client_sender (handler);
    zuuid_destroy (&sender);
    
    //  Receive Response
    msg = zwr_client_recv (client);
    xmsg = xrap_msg_decode (&msg);
    assert (xrap_msg_id (xmsg) == XRAP_MSG_GET_OK);
    assert (xrap_msg_status_code (xmsg) == 200);
    assert (streq ("text/hello", xrap_msg_content_type (xmsg)));
    assert (streq ("Hello World!", xrap_msg_content_body (xmsg)));
    xrap_msg_destroy (&xmsg);
    sender = zwr_client_sender (client);
    zuuid_destroy (&sender);
    
    //  Send Request 2
    xmsg = xrap_msg_new (XRAP_MSG_GET);
    xrap_msg_set_resource (xmsg, "%s", "/fou/baz");
    msg = xrap_msg_encode (&xmsg);
    rc = zwr_client_request (client, 0, &msg);
    assert (rc == XRAP_TRAFFIC_NOT_FOUND);
    
    zwr_client_destroy (&client);
    zwr_client_destroy (&handler);
    
    //  Done, shut down
    zactor_destroy (&server);


<A name="toc2-592" title="Ownership and License" />
## Ownership and License

The contributors are listed in AUTHORS. This project uses the MPL v2 license, see LICENSE.

zwebrap uses the [C4.1 (Collective Code Construction Contract)](http://rfc.zeromq.org/spec:22) process for contributions.

To report an issue, use the [zwebrap issue tracker](https://github.com/zeromq/zproject/issues) at github.com.

<A name="toc3-601" title="Hints to Contributors" />
### Hints to Contributors

Do read your code after you write it and ask, "Can I make this simpler?" We do use a nice minimalist and yet readable style. Learn it, adopt it, use it.

Before opening a pull request read our [contribution guidelines](https://github.com/zeromq/zwebrap/blob/master/CONTRIBUTING.md). Thanks!

<A name="toc3-608" title="This Document" />
### This Document

This document is originally at README.txt and is built using [gitdown](http://github.com/imatix/gitdown).
