/*
################################################################################
#  THIS FILE IS 100% GENERATED BY ZPROJECT; DO NOT EDIT EXCEPT EXPERIMENTALLY  #
#  Please refer to the README for information about making permanent changes.  #
################################################################################
*/

#include "QmlZwrClient.h"


///
//  Return actor, when caller wants to work with multiple actors and/or
//  input sockets asynchronously.                                      
zactor_t *QmlZwrClient::actor () {
    return zwr_client_actor (self);
};

///
//  Return message pipe for asynchronous message I/O. In the high-volume case,
//  we send methods and get replies to the actor, in a synchronous manner, and
//  we send/recv high volume message data to a second pipe, the msgpipe. In   
//  the low-volume case we can do everything over the actor pipe, if traffic  
//  is never ambiguous.                                                       
zsock_t *QmlZwrClient::msgpipe () {
    return zwr_client_msgpipe (self);
};

///
//  Return true if client is currently connected, else false. Note that the   
//  client will automatically re-connect if the server dies and restarts after
//  a successful first connection.                                            
bool QmlZwrClient::connected () {
    return zwr_client_connected (self);
};

///
//  Connect to server endpoint, with specified timeout in msecs (zero means wait
//  forever). Constructor succeeds if connection is successful. The caller may  
//  specify its address.                                                        
//  Returns >= 0 if successful, -1 if interrupted.                              
int QmlZwrClient::connect (const QString &endpoint, uint32_t timeout, const QString &address) {
    return zwr_client_connect (self, endpoint.toUtf8().data(), timeout, address.toUtf8().data());
};

///
//  Offer to handle particular XRAP requests, where the route matches request's
//  resource.                                                                  
//  Returns >= 0 if successful, -1 if interrupted.                             
int QmlZwrClient::setHandler (const QString &method, const QString &route) {
    return zwr_client_set_handler (self, method.toUtf8().data(), route.toUtf8().data());
};

///
//  No explanation                                
//  Returns >= 0 if successful, -1 if interrupted.
int QmlZwrClient::request (uint32_t timeout, zmsg_t **contentP) {
    return zwr_client_request (self, timeout, contentP);
};

///
//  Send XRAP DELIVER message to server, takes ownership of message
//  and destroys message when done sending it.                     
int QmlZwrClient::deliver (zuuid_t *sender, zmsg_t **contentP) {
    return zwr_client_deliver (self, sender, contentP);
};

///
//  Receive message from server; caller destroys message when done
zmsg_t *QmlZwrClient::recv () {
    return zwr_client_recv (self);
};

///
//  Return last received command. Can be one of these values:
//      "XRAP DELIVER"                                       
const QString QmlZwrClient::command () {
    return QString (zwr_client_command (self));
};

///
//  Return last received status
int QmlZwrClient::status () {
    return zwr_client_status (self);
};

///
//  Return last received reason
const QString QmlZwrClient::reason () {
    return QString (zwr_client_reason (self));
};

///
//  Return last received sender
zuuid_t *QmlZwrClient::sender () {
    return zwr_client_sender (self);
};

///
//  Return last received content
zmsg_t *QmlZwrClient::content () {
    return zwr_client_content (self);
};


QObject* QmlZwrClient::qmlAttachedProperties(QObject* object) {
    return new QmlZwrClientAttached(object);
}


///
//  Self test of this class.
void QmlZwrClientAttached::test (bool verbose) {
    zwr_client_test (verbose);
};

///
//  Create a new zwr_client, return the reference if successful, or NULL
//  if construction failed due to lack of available memory.             
QmlZwrClient *QmlZwrClientAttached::construct () {
    QmlZwrClient *qmlSelf = new QmlZwrClient ();
    qmlSelf->self = zwr_client_new ();
    return qmlSelf;
};

///
//  Destroy the zwr_client and free all memory used by the object.
void QmlZwrClientAttached::destruct (QmlZwrClient *qmlSelf) {
    zwr_client_destroy (&qmlSelf->self);
};

/*
################################################################################
#  THIS FILE IS 100% GENERATED BY ZPROJECT; DO NOT EDIT EXCEPT EXPERIMENTALLY  #
#  Please refer to the README for information about making permanent changes.  #
################################################################################
*/