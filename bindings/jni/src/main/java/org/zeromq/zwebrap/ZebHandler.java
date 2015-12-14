/*
################################################################################
#  THIS FILE IS 100% GENERATED BY ZPROJECT; DO NOT EDIT EXCEPT EXPERIMENTALLY  #
#  Please refer to the README for information about making permanent changes.  #
################################################################################
*/
package org.zeromq.zwebrap;
import org.zeromq.czmq.*;

public class ZebHandler {
    static {
        try {
            System.loadLibrary ("zwebrapjni");
        }
        catch (Exception e) {
            System.exit (-1);
        }
    }
    public long self;
    /*
    Add a new offer this handler will handle. Returns 0 if successful,
    otherwise -1.                                                     
    */
    native static int __addOffer (long self, int method, String uri);
    public int addOffer (Zactor self, int method, String uri) {
        return __addOffer (self.self, method, uri);
    }
    /*
    Add a new accept type that this handler can deliver. May be a regular
    expression. Returns 0 if successfull, otherwise -1.                  
    */
    native static int __addAccept (long self, String accept);
    public int addAccept (Zactor self, String accept) {
        return __addAccept (self.self, accept);
    }
    /*
    Self test of this class.
    */
    native static void __test (boolean verbose);
    public static void test (boolean verbose) {
        __test (verbose);
    }
}
